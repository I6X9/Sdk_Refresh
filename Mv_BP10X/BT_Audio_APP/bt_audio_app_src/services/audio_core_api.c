/**
 **************************************************************************************
 * @file    audio_core.c
 * @brief   audio core 
 *
 * @author  Sam
 * @version V1.0.0
 *
 * $Created: 2016-6-29 13:06:47$
 *
 * @Copyright (C) 2016, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#include <string.h>
#include <nds32_intrinsic.h>
#include "type.h"
#include "clk.h"
#include "freertos.h"
#include "audio_core_api.h"
#include "app_config.h"
#include "debug.h"
#include "ctrlvars.h"
#include "audio_effect.h"
#include "mode_switch_api.h"
#include "main_task.h"
#include "audio_core_api.h"

#ifdef CFG_APP_BT_MODE_EN
#include "bt_play_api.h"
#if (BT_HFP_SUPPORT == ENABLE)
#include "bt_hf_api.h"
#endif
#endif

typedef enum
{
	AC_RUN_CHECK,//���ڼ���Ƿ���Ҫ��ͣ���������Ҫ��ͣ������ͣ���ٸ�״̬
	AC_RUN_GET,
	AC_RUN_PROC,
	AC_RUN_PUT,
}AudioCoreRunState;

//ControlVariablesContext gCtrlVars;

//static AudioCoreContext		AudioCore;
AudioCoreContext		AudioCore;

static uint16_t AdjustLevelHigh	= 0;//(CFG_PARA_SAMPLES_PER_FRAME * 3 / 4); //����ǰbuf��֡��С����ȡʱ���ǳ���һ֡��
static uint16_t AdjustLevelLow	= 0;//(CFG_PARA_SAMPLES_PER_FRAME / 4);
static AudioCoreRunState AudioState = AC_RUN_GET;

#ifdef CFG_FUNC_FREQ_ADJUST //Ӳ����Ƶƫ��ǯλ�������֮��,(�����������ɼ�ǿ��Ƶ)
#define ADJUST_PERIOD_COUNT		16 //ˮλ���� ����������Ƽ�16,32/64/128,���ڼ��㣬�൱��100~2000mS���ҵ���һ�Ρ�
#define ADJUST_CLOCK_MAX		6 //��Ƶƫֵ���λ��(����)����Ӧregֵ;480M��Ƶ��ȡaudioclock regΪ������С���֣���λ1/256.
static int8_t FreqAsyncSource = 127;	//�첽��Դͨ����127Ϊȱʡֵ��δ���á�
static int16_t AsyncRemainLast;
static int32_t AsyncRemain;
static int8_t AdjustValue = 0;	//ϵͳ��Ƶ��(ƫ��)����ֵ ��С������(��λ��1/256),
static int8_t LastRise = 0;	//�ϴ�ˮλ�仯�����1:�� 0��ƽ -1:��
static uint8_t Count;

/**
 * @func        AudioCoreSourceFreqAdjustEnable
 * @brief       ʹ��ϵͳ��Ƶ��Ƶ΢����ʹ�ŵ�֮��ͬ��(���첽��Դ)
 * @param       uint8_t AsyncIndex   �첽��ƵԴ�����ŵ����
 * @param       uint16_t LevelLow   �첽��ƵԴ�����ŵ���ˮλ������ֵ
 * @param       uint16_t LevelHigh   �첽��ƵԴ�����ŵ���ˮλ������ֵ
 * @Output      None
 * @return      None
 * @Others      
 * Record
 * 1.Date        : 20180518
 *   Author      : pi.wang
 *   Modification: Created function
 */
void AudioCoreSourceFreqAdjustEnable(uint8_t AsyncIndex, uint16_t LevelLow, uint16_t LevelHigh)
{
	if(AsyncIndex >= AUDIO_CORE_SOURCE_MAX_MUN)
		return ;
	Count = 0;
	FreqAsyncSource = AsyncIndex;
	AsyncRemainLast = 0xffff;
	AsyncRemain = 0;
	AdjustValue = 0;
	AdjustLevelHigh = LevelHigh;
	AdjustLevelLow = LevelLow;
}

void AudioCoreSourceFreqAdjustDisable(void)
{
	FreqAsyncSource = 127;
	AsyncRemainLast = 0;
	Clock_AudioPllClockAdjust(PLL_CLK_1, 0, 0);//�ָ�ȱʡֵ
	Clock_AudioPllClockAdjust(PLL_CLK_2, 0, 0);
	AdjustValue = 0;
	Count = 0;
	AdjustLevelHigh = 0;//(CFG_PARA_SAMPLES_PER_FRAME * 3 / 4);
	AdjustLevelLow = 0;//(CFG_PARA_SAMPLES_PER_FRAME / 4);
}

void AudioCoreSourceFreqAdjust(void)
{
	int8_t 	Adjust = 0;//Ϊ�˱��ϵ���Ƶ��ƽ�������ο�΢��1��λ��
	int8_t	LevelRise = 0;
	if(FreqAsyncSource < AUDIO_CORE_SOURCE_MAX_MUN
		&& ((AudioCore.AudioSource[FreqAsyncSource].Enable)))// && AudioCore.AudioSource[FreqAsyncSource].FuncDataGetLen != NULL
	{
#ifdef CFG_APP_BT_MODE_EN
		if(GetSystemMode() == AppModeBtAudioPlay)
		{
			AsyncRemain += GetValidSbcDataSize();
		}
		else
#endif
		{
			AsyncRemain += AudioCore.AudioSource[FreqAsyncSource].FuncDataGetLen();
		}

		if(++Count >= ADJUST_PERIOD_COUNT)
		{
			AsyncRemain = AsyncRemain / Count;//ˮλ������
			if(AsyncRemainLast != 0xffff)//not first
			{
				if(AsyncRemain > AsyncRemainLast)//ˮλ��
				{
					LevelRise = 1;
					if((AsyncRemain > AdjustLevelHigh) //bufˮλ Խ���������λ
						|| (AsyncRemain >= AdjustLevelLow && (LastRise == 1)))//���������� ��������
					{
						Adjust = -1; //��ӿ�ϵͳʱ��/���ͷ�Ƶ����ֵ��
					}
				}
				else if(AsyncRemain < AsyncRemainLast)//ˮλ��
				{
					LevelRise = -1;
					if((AsyncRemain < AdjustLevelLow)//bufˮλ Խ���������λ
						|| ((AsyncRemain < AdjustLevelHigh) && (LastRise == -1))) //���������� �����µ�
					{
						Adjust = 1; //�轵��ϵͳʱ��/��߷�Ƶ����ֵ
					}
				}

				Adjust += AdjustValue; //����LastƵƫ����ֵ
#ifdef CFG_APP_BT_MODE_EN
				if(Adjust > ADJUST_CLOCK_MAX && ((GetSystemMode() == AppModeBtAudioPlay)||(GetSystemMode() == AppModeBtHfPlay))) //��Ƶƫǯλ
				{
					if(Adjust > ADJUST_CLOCK_MAX * 2)
					{
						Adjust = ADJUST_CLOCK_MAX * 2;
					}
				}
				else if(Adjust > ADJUST_CLOCK_MAX)
#else
				if(Adjust > ADJUST_CLOCK_MAX)
#endif
				{
					Adjust = ADJUST_CLOCK_MAX;
				}
				else if(Adjust < -ADJUST_CLOCK_MAX)
				{
					Adjust = -ADJUST_CLOCK_MAX;
				}
				if(Adjust != AdjustValue)//��Ƶֵ�����
				{
					AdjustValue = Adjust; //���汾��Ƶƫ ��Ƶ��
					//DBG("R:%d A:%d \n", (int)AsyncRemain, (int)AdjustValue);
					if(AdjustValue > 0) //���ȱʡ����߷�Ƶֵ
					{
						Clock_AudioPllClockAdjust(PLL_CLK_1, 0, AdjustValue);
						Clock_AudioPllClockAdjust(PLL_CLK_2, 0, AdjustValue);
					}
					else
					{
						Clock_AudioPllClockAdjust(PLL_CLK_1, 1, -AdjustValue);
						Clock_AudioPllClockAdjust(PLL_CLK_2, 1, -AdjustValue);
					}
				}
			}
			AsyncRemainLast = AsyncRemain;
			AsyncRemain = 0;//�ۻ���������
			Count = 0;
			LastRise = LevelRise;//���ݱ���ˮλ�仯��
		}
	}
}
#endif

/**
 * @brief       AudioCore ����Դ ���ȡ
 * @note		ֻ������ʹ�ܵ���Ƶ��buf��������1֡���Ż�DMA��������֡
*/
static bool AudioCoreSourceDateGet(void)
{
	uint32_t i;
	uint32_t Cnt = 0;
	bool IsSourceDataEnough = TRUE; 

	for(i = 0; i< AUDIO_CORE_SOURCE_MAX_MUN; i++)
	{
		if(AudioCore.AudioSource[i].Enable == FALSE)
		{
			Cnt++;//����һ��������������ͳ���Ƿ�����source������ֹ
			continue;
		}
		if((AudioCore.AudioSource[i].IsSreamData == FALSE)/**/
		|| (AudioCore.AudioSource[i].FuncDataGetLen == NULL))
		{
			continue;
		}
		//if(AudioCore.AudioSource[i].FuncDataGetLen() < CFG_PARA_SAMPLES_PER_FRAME)
		if(AudioCore.AudioSource[i].FuncDataGetLen() < mainAppCt.SamplesPreFrame)
		{
			IsSourceDataEnough = FALSE; 
			break;
		}
	}
	if(Cnt == AUDIO_CORE_SOURCE_MAX_MUN)//û��һ��ͨ���������ݣ��˳�
	{
		return FALSE;
	}
	if(!IsSourceDataEnough)
	{
		IsSourceDataEnough = TRUE;
		return FALSE;
	}

	for(i = 0; i< AUDIO_CORE_SOURCE_MAX_MUN; i++)
	{
		if(AudioCore.AudioSource[i].Enable == FALSE)
		{
			continue;
		}
		//���ȱ�����FRAME��������0
		//memset(AudioCore.AudioSource[i].PcmInBuf, 0, CFG_PARA_SAMPLES_PER_FRAME * AudioCore.AudioSource[i].PcmFormat * 2);
		memset(AudioCore.AudioSource[i].PcmInBuf, 0, mainAppCt.SamplesPreFrame * AudioCore.AudioSource[i].PcmFormat * 2);
		AudioCore.AudioSource[i].FuncDataGet(AudioCore.AudioSource[i].PcmInBuf, mainAppCt.SamplesPreFrame);
	}
	
#ifdef CFG_FUNC_FREQ_ADJUST
	AudioCoreSourceFreqAdjust();
#endif	
	return TRUE;
}

/**
 * @func        AudioCoreSinkDataSet
 * @brief       AudioCore ��Ч��� ������֡�� ��Ƶ���ϵͳ��buf
 * @param       None
 * @Output      DMA��������֡����Ƶ���buf
 * @return      bool
 * @note		��Ч�������buf����1֡ʱ��dma����1֡����
 * Record
*/
static bool AudioCoreSinkDataSet(void)
{
	uint32_t i;
	
	#ifdef CFG_RES_AUDIO_I2SOUT_EN
	uint32_t SendI2SDataIf=1;
	#endif
	for(i = 0; i< AUDIO_CORE_SINK_MAX_NUM; i++)
	{
		if((AudioCore.AudioSink[i].Enable == TRUE)
		&& (AudioCore.AudioSink[i].FuncDataSpaceLenSet() < mainAppCt.SamplesPreFrame))
		{

		#ifdef CFG_RES_AUDIO_I2SOUT_EN
			if(i==AUDIO_I2SOUT_SINK_NUM)
				{

				DBG("i2s dma full ,check tx pin wave!\n");
				SendI2SDataIf=0;
				continue;
				
				}
		#endif
			return FALSE;
		}
	}

	for(i = 0; i< AUDIO_CORE_SINK_MAX_NUM; i++)
	{
		#ifdef CFG_RES_AUDIO_I2SOUT_EN
		if(SendI2SDataIf==0&&i==AUDIO_I2SOUT_SINK_NUM)continue;
		#endif
	
		if((AudioCore.AudioSink[i].Enable == TRUE) && (AudioCore.AudioSink[i].FuncDataSet != NULL))
		{
			AudioCore.AudioSink[i].FuncDataSet(AudioCore.AudioSink[i].PcmOutBuf, mainAppCt.SamplesPreFrame);
		}
	}
	return TRUE;
}

void AudioCoreSourcePcmFormatConfig(uint8_t Index, uint16_t Format)
{
	if(Index < AUDIO_CORE_SOURCE_MAX_MUN)
	{
		AudioCore.AudioSource[Index].PcmFormat = Format;
	}
}

void AudioCoreSourceEnable(uint8_t Index)
{
	if(Index < AUDIO_CORE_SOURCE_MAX_MUN)
	{
		AudioCore.AudioSource[Index].Enable = TRUE;
	}
}

void AudioCoreSourceDisable(uint8_t Index)
{
	if(Index < AUDIO_CORE_SOURCE_MAX_MUN)
	{
		AudioCore.AudioSource[Index].Enable = FALSE;
	}
}

void AudioCoreSourceMute(uint8_t Index, bool IsLeftMute, bool IsRightMute)
{
	if(IsLeftMute)
	{
		AudioCore.AudioSource[Index].LeftMuteFlag = TRUE;
	}
	if(IsRightMute)
	{
		AudioCore.AudioSource[Index].RightMuteFlag = TRUE;
	}
}

void AudioCoreSourceUnmute(uint8_t Index, bool IsLeftUnmute, bool IsRightUnmute)
{
	if(IsLeftUnmute)
	{
		AudioCore.AudioSource[Index].LeftMuteFlag = FALSE;
	}
	if(IsRightUnmute)
	{
		AudioCore.AudioSource[Index].RightMuteFlag = FALSE;
	}
}

void AudioCoreSourceVolSet(uint8_t Index, uint16_t LeftVol, uint16_t RightVol)
{
	AudioCore.AudioSource[Index].LeftVol = LeftVol;
	AudioCore.AudioSource[Index].RightVol = RightVol;
}

void AudioCoreSourceVolGet(uint8_t Index, uint16_t* LeftVol, uint16_t* RightVol)
{
	*LeftVol = AudioCore.AudioSource[Index].LeftCurVol;
	*RightVol = AudioCore.AudioSource[Index].RightCurVol;
}

void AudioCoreSourceConfig(uint8_t Index, AudioCoreSource* Source)
{
	memcpy(&AudioCore.AudioSource[Index], Source, sizeof(AudioCoreSource));
}

void AudioCoreSinkEnable(uint8_t Index)
{
	AudioCore.AudioSink[Index].Enable = TRUE;
}

void AudioCoreSinkDisable(uint8_t Index)
{
	AudioCore.AudioSink[Index].Enable = FALSE;
}

void AudioCoreSinkMute(uint8_t Index, bool IsLeftMute, bool IsRightMute)
{
	if(IsLeftMute)
	{
		AudioCore.AudioSink[Index].LeftMuteFlag = TRUE;
	}
	if(IsRightMute)
	{
		AudioCore.AudioSink[Index].RightMuteFlag = TRUE;
	}
}

void AudioCoreSinkUnmute(uint8_t Index, bool IsLeftUnmute, bool IsRightUnmute)
{
	if(IsLeftUnmute)
	{
		AudioCore.AudioSink[Index].LeftMuteFlag = FALSE;
	}
	if(IsRightUnmute)
	{
		AudioCore.AudioSink[Index].RightMuteFlag = FALSE;
	}
}

void AudioCoreSinkVolSet(uint8_t Index, uint16_t LeftVol, uint16_t RightVol)
{
	AudioCore.AudioSink[Index].LeftVol = LeftVol;
	AudioCore.AudioSink[Index].RightVol = RightVol;
}

void AudioCoreSinkVolGet(uint8_t Index, uint16_t* LeftVol, uint16_t* RightVol)
{
	*LeftVol = AudioCore.AudioSink[Index].LeftCurVol;
	*RightVol = AudioCore.AudioSink[Index].RightCurVol;
}

void AudioCoreSinkConfig(uint8_t Index, AudioCoreSink* Sink)
{
	memcpy(&AudioCore.AudioSink[Index], Sink, sizeof(AudioCoreSink));
}


void AudioCoreProcessConfig(AudioCoreProcessFunc AudioEffectProcess)
{
	AudioCore.AudioEffectProcess = AudioEffectProcess;
}

/**
 * @func        AudioCoreConfig
 * @brief       AudioCore�����飬���ػ�API
 * @param       AudioCoreContext *AudioCoreCt
 * @Output      None
 * @return      None
 * @Others      �ⲿ���õĲ����飬����һ�ݵ�����
 */
void AudioCoreConfig(AudioCoreContext *AudioCoreCt)
{
	memcpy(&AudioCore, AudioCoreCt, sizeof(AudioCoreContext));
}

bool AudioCoreInit(void)
{
	return TRUE;
}

void AudioCoreDeinit(void)
{
	AudioState = AC_RUN_GET;
}

/**
 * @func        AudioCoreRun
 * @brief       ��Դ����->��Ч����+����->����
 * @param       None
 * @Output      None
 * @return      None
 * @Others      ��ǰ��audioCoreservice�����ϴ˹�����Ч������
 * Record
 */
extern uint32_t 	IsAudioCorePause;
extern uint32_t 	IsAudioCorePauseMsgSend;
void AudioProcessMain(void);
void AudioCoreRun(void)
{
	bool ret;
	switch(AudioState)
	{
		case AC_RUN_CHECK:
			if(IsAudioCorePause == TRUE)
			{
				if(IsAudioCorePauseMsgSend == TRUE)
				{
					MessageContext		msgSend;
					msgSend.msgId		= MSG_AUDIO_CORE_HOLD;
					MessageSend(GetAudioCoreServiceMsgHandle(), &msgSend);

					IsAudioCorePauseMsgSend = FALSE;
				}
				return;
			}
		case AC_RUN_GET:
			ret = AudioCoreSourceDateGet();
			if(ret == FALSE)
			{
				return;
			}
		case AC_RUN_PROC:
			//AudioCore.AudioProcess();
			AudioProcessMain();
			AudioState = AC_RUN_PUT;

		case AC_RUN_PUT:
			ret = AudioCoreSinkDataSet();
			if(ret == FALSE)
			{
				return;
			}
			//AudioState = AC_RUN_GET;
			AudioState = AC_RUN_CHECK;
			break;
		default:
			break;
	}
}

//��Ч�������������
//��micͨ·���ݰ������ͳһ����
//micͨ·���ݺ;���ģʽ�޹�
//��ʾ��ͨ·����Ч���������sink�˻�����
void AudioProcessMain(void)
{	
	AduioCoreSourceVolSet();

#ifdef CFG_FUNC_RECORDER_EN
	if(AudioCore.AudioSource[PLAYBACK_SOURCE_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSource[PLAYBACK_SOURCE_NUM].PcmFormat == 1)
		{
			uint16_t i;
			for(i = mainAppCt.SamplesPreFrame * 2 - 1; i > 0; i--)
			{
				AudioCore.AudioSource[PLAYBACK_SOURCE_NUM].PcmInBuf[i] = AudioCore.AudioSource[PLAYBACK_SOURCE_NUM].PcmInBuf[i / 2];
			}
		}
	}
#endif

	if(AudioCore.AudioSource[APP_SOURCE_NUM].Enable == TRUE)////music buff
	{
		#if (BT_HFP_SUPPORT == ENABLE) && defined(CFG_APP_BT_MODE_EN)
		if(GetSystemMode() != AppModeBtHfPlay)
		#endif
		{
			if(AudioCore.AudioSource[APP_SOURCE_NUM].PcmFormat == 1)
			{
				uint16_t i;
				for(i = mainAppCt.SamplesPreFrame * 2 - 1; i > 0; i--)
				{
					AudioCore.AudioSource[APP_SOURCE_NUM].PcmInBuf[i] = AudioCore.AudioSource[APP_SOURCE_NUM].PcmInBuf[i / 2];
				}
			}
		}
	}	
		
#if defined(CFG_FUNC_REMIND_SOUND_EN)	
	if(AudioCore.AudioSource[REMIND_SOURCE_NUM].Enable == TRUE)////remind buff
	{
		if(AudioCore.AudioSource[REMIND_SOURCE_NUM].PcmFormat == 1)
		{
			uint16_t i;
			for(i = mainAppCt.SamplesPreFrame * 2 - 1; i > 0; i--)
			{
				AudioCore.AudioSource[REMIND_SOURCE_NUM].PcmInBuf[i] = AudioCore.AudioSource[REMIND_SOURCE_NUM].PcmInBuf[i / 2];
			}
		}
	}	
#endif

	if(AudioCore.AudioEffectProcess != NULL)
	{
		AudioCore.AudioEffectProcess((AudioCoreContext*)&AudioCore);
	}
	//else
	//{
		//return;
	//}
	
    #ifdef CFG_FUNC_BEEP_EN
    if(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].Enable == TRUE)   ////dacx buff
	{
		Beep(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf, mainAppCt.SamplesPreFrame);
	}
    #endif

	AduioCoreSinkVolSet();//������
}

//ע��:
//֡���ȴ���192 sample

//�������뵭��					
#define MixerFadeVolume(a, b, c)  	\
    if(a > b + c)		    \
	{						\
		a -= c;				\
	}						\
	else if(a + c < b)	   	\
	{						\
		a += c;				\
	}						\
	else					\
	{						\
		a = b;				\
	}

void AduioCoreSourceVolSet(void)
{
	uint32_t i, j;
    
	uint16_t LeftVol, RightVol, VolStep = 4096 / mainAppCt.SamplesPreFrame + 1;

	if(GetSystemMode() == AppModeBtHfPlay)
	{
		AudioCore.AudioSource[0].PreGain = BT_HFP_MIC_DIGIT_GAIN;
		AudioCore.AudioSource[1].PreGain = BT_HFP_INPUT_DIGIT_GAIN;
	}
	else
	{
		AudioCore.AudioSource[0].PreGain = 4095;//0db
		AudioCore.AudioSource[1].PreGain = 4095;
	}
	
	for(j=0; j<AUDIO_CORE_SOURCE_MAX_MUN; j++)
	{
		if(!AudioCore.AudioSource[j].Enable)
		{
			continue;
		}
		if(AudioCore.AudioSource[j].LeftMuteFlag == TRUE)
		{
			LeftVol = 0;
		}
		else
		{
			LeftVol = AudioCore.AudioSource[j].LeftVol;
		}
		if(AudioCore.AudioSource[j].RightMuteFlag == TRUE)
		{
			RightVol = 0;
		}
		else
		{
			RightVol = AudioCore.AudioSource[j].RightVol;
		}
		if(AudioCore.AudioSource[j].PcmFormat == 2)//������
		{
			for(i=0; i<mainAppCt.SamplesPreFrame; i++)
			{
				AudioCore.AudioSource[j].PcmInBuf[2 * i + 0] = __nds32__clips((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[2 * i + 0]) * AudioCore.AudioSource[j].LeftCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12, (16)-1);//__SSAT((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[2 * i] * AudioCore.AudioSource[j].LeftCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12), 16);
				AudioCore.AudioSource[j].PcmInBuf[2 * i + 1] = __nds32__clips((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[2 * i + 1]) * AudioCore.AudioSource[j].RightCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12, (16)-1);//__SSAT((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[2 * i + 1] * AudioCore.AudioSource[j].RightCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12), 16);
				
				MixerFadeVolume(AudioCore.AudioSource[j].LeftCurVol, LeftVol, VolStep);
				MixerFadeVolume(AudioCore.AudioSource[j].RightCurVol, RightVol, VolStep);
			}
		}
		else
		{
			for(i=0; i<mainAppCt.SamplesPreFrame; i++)
			{
				AudioCore.AudioSource[j].PcmInBuf[i] = __nds32__clips((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[i]) * AudioCore.AudioSource[j].LeftCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12, (16)-1);//__SSAT((((((int32_t)AudioCore.AudioSource[j].PcmInBuf[i] * AudioCore.AudioSource[j].LeftCurVol) >> 12) * AudioCore.AudioSource[j].PreGain) >> 12), 16);
				MixerFadeVolume(AudioCore.AudioSource[j].LeftCurVol, LeftVol, VolStep);
			}
		}
	}
}


void AduioCoreSinkVolSet(void)
{
	uint32_t i;

	uint16_t VolStep = 4096 / mainAppCt.SamplesPreFrame + 1;

	if(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].LeftMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].LeftVol = 0;
		}
		if(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].RightMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].RightVol = 0;
		}

		for(i=0; i<mainAppCt.SamplesPreFrame; i++)
		{
			AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf[2 * i + 0] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf[2 * i + 0]) * AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].LeftCurVol) >> 12, (16)-1);//__SSAT((((int32_t)AudioCore.AudioSink[AUDIO_SINK_NUM].PcmOutBuf[2 * i] * AudioCore.AudioSink[AUDIO_SINK_NUM].LeftCurVol) >> 12), 16);
			AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf[2 * i + 1] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf[2 * i + 1]) * AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].RightCurVol) >> 12, (16)-1);//__SSAT((((int32_t)AudioCore.AudioSink[AUDIO_SINK_NUM].PcmOutBuf[2 * i + 1] * AudioCore.AudioSink[AUDIO_SINK_NUM].RightCurVol) >> 12), 16);

			MixerFadeVolume(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].LeftCurVol, AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].LeftVol, VolStep);
			MixerFadeVolume(AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].RightCurVol, AudioCore.AudioSink[AUDIO_DAC0_SINK_NUM].RightVol, VolStep);
		}
	}
	if(AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].LeftMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].LeftVol = 0;
		}
		if(AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].RightMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].RightVol = 0;
		}

		for(i=0; i<mainAppCt.SamplesPreFrame; i++)
		{
			AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].PcmOutBuf[2 * i + 0] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].PcmOutBuf[2 * i + 0]) * AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].LeftCurVol) >> 12, (16)-1);//
			AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].PcmOutBuf[2 * i + 1] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].PcmOutBuf[2 * i + 1]) * AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].RightCurVol) >> 12, (16)-1);//

			MixerFadeVolume(AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].LeftCurVol, AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].LeftVol, VolStep);
			MixerFadeVolume(AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].RightCurVol, AudioCore.AudioSink[AUDIO_RECORDER_SINK_NUM].RightVol, VolStep);
		}
	}

#if (defined(CFG_APP_BT_MODE_EN) && (BT_HFP_SUPPORT == ENABLE))
	//sink2: hfp sco send
	if(AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].LeftMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].LeftVol = 0;
		}
		if(AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].RightMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].RightVol = 0;
		}

		for(i=0; i<mainAppCt.SamplesPreFrame; i++)
		{
			AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].PcmOutBuf[i] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].PcmOutBuf[i]) * AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].LeftCurVol) >> 12, (16)-1);

			MixerFadeVolume(AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].LeftCurVol, AudioCore.AudioSink[AUDIO_HF_SCO_SINK_NUM].LeftVol, VolStep);
		}
	}
#endif

#ifdef CFG_RES_AUDIO_DACX_EN 	
	if(AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftVol = 0;
		}
		if(AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].RightMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].RightVol = 0;
		}

		for(i=0; i<mainAppCt.SamplesPreFrame; i++)
		{
			AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].PcmOutBuf[i] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].PcmOutBuf[i]) * AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftCurVol) >> 12, (16)-1);//__SSAT((((int32_t)AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].PcmOutBuf[i] * AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftCurVol) >> 12), 16);

			MixerFadeVolume(AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftCurVol, AudioCore.AudioSink[AUDIO_DACX_SINK_NUM].LeftVol, VolStep);
		}
	}
#endif

#ifdef CFG_RES_AUDIO_I2SOUT_EN
	if(AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].Enable == TRUE)
	{
		if(AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftVol = 0;
		}
		if(AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightMuteFlag == TRUE)
		{
			AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightVol = 0;
		}

		for(i=0; i<mainAppCt.SamplesPreFrame; i++)
		{		
			AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i + 0] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i + 0]) * AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftCurVol) >> 12, (16)-1);//__SSAT((((int32_t)AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i] * AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftCurVol) >> 12), 16);
			AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i + 1] = __nds32__clips((((int32_t)AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i + 1]) * AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightCurVol) >> 12, (16)-1);//__SSAT((((int32_t)AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf[2 * i + 1] * AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightCurVol) >> 12), 16);
		
			MixerFadeVolume(AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftCurVol, AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].LeftVol, VolStep);
			MixerFadeVolume(AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightCurVol, AudioCore.AudioSink[AUDIO_I2SOUT_SINK_NUM].RightVol, VolStep);
		}
	}
#endif
}

