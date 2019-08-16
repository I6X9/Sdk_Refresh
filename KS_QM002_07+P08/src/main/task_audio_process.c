/**
 *******************************************************************************
 * @file    task_audio_process.c
 * @author  Richard
 * @version V1.0.0
 * @date    15-11-2013
 * @brief   audio process task
 * @maintainer Halley
 *******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MVSILICON SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */
#include "type.h"
#include "clk.h"
#include "audio_adc.h"
#include "app_config.h"
#include "os.h"
#include "dac.h"
#include "mixer.h"
#include "echo.h"
#include "gpio.h"
#include "i2s.h"
#include "dev_detect_driver.h"
#include "sys_vol.h"
#include "sys_app.h"
#include "audio_encoder.h"
#include "bt_app_func.h"
#include "recorder_control.h"
#include "sound_remind.h"
#include "usb_audio_control.h"
#include "dc_blocker.h"
#include "silence_detector.h"
#include "device_audio.h"
#include "noise_gate.h"

#define MIC_BUF_LEN		192//128//256
int16_t AduioBuf[MIC_BUF_LEN * 2];		//����ADC��������������


#ifdef FUNC_MIC_ECHO_EN
#define ECHO_DELAY_SAMPLE	12000 //250ms?
uint8_t	EchoDelayStep = DEFAULT_DELAY_STEP;	//�����ӳ��12000�������㣬44.1KHZ�������¶�Ӧ272ms
uint8_t	EchoDepthStep = DEFAULT_DEPTH_STEP;	//����ǿ�����16384/32768����0.5
uint8_t echo_sbuf[(ECHO_DELAY_SAMPLE + 32)/2];
EchoContext  ct;
#ifdef MIC_ECHO_MIX_BYPASS_EN
int16_t AduioBuf_MIC_Bypass[MIC_BUF_LEN];		//MICͨ·ԭʼ���ݻ���buf
#endif
#endif


#ifdef FUNC_DC_BLOCKER
DCBlocker dcb;
#ifdef FUNC_SILENCE_DETECTOR
SilenceDetectorContext sdct;
#endif
#endif

#ifdef FUNC_MIC_NOISE_GATE_EN
NoiseGateCT ngct;
#endif
#include "mcu_circular_buf.h"
extern MCU_CIRCULAR_CONTEXT OTGCircularBuf;
extern int16_t OTG_DeviceRxBuf[1024];
#define OTG_AUDIO_RX_BUF_SIZE   sizeof(OTG_DeviceRxBuf)

void AudioProcessTaskEntrance(void)
{
	uint16_t SampleCnt;
	uint16_t i;
#ifdef FUNC_MIC_ECHO_EN
	bool	 OldMicEnable = FALSE;
#endif
#ifdef FUNC_DC_BLOCKER
	uint8_t init_dcb_channel = 1;		// ��ʼ�� DC blocker ������
	#ifdef FUNC_SILENCE_DETECTOR
	uint8_t	init_sd_channel = 1;
	int32_t	smoothed_pcm_amplitude;
	uint32_t silence_counter;
	#endif
#endif
	APP_DBG("Audio task Init...\n");
	AudioAnaSetChannel(AUDIO_CH_NONE);	

#ifdef FUNC_MIC_ECHO_EN
	//MIC ECHO��ʼ��
	memset(&ct, 0x00, sizeof(ct));
	echo_init(&ct, 1, DacAdcSampleRateGet(), CFG_ECHO_CUTOFF_FREQ, ECHO_DELAY_SAMPLE, echo_sbuf);
#endif
	//�ȶ�����ʼ��һ�����ݣ�������к�ǿPOP��
	for(i = 0; i < 1000; i++)
	{
		while(AdcPmemPcmRemainLenGet() < 44);
		AdcPcmDataRead((void*)AduioBuf, 44, 0x04);	
	}

#ifdef FUNC_DC_BLOCKER
	//�Ƚ�dc blocker��ʼ��Ϊ������
	init_dc_blocker(&dcb, init_dcb_channel);
	#ifdef FUNC_SILENCE_DETECTOR
	init_silence_detector(&sdct, init_sd_channel, 44100);
	silence_counter = 0;
	#endif
#endif

//ע�����ϲ�������������ʾ����Ҫ�ͻ�����ʵ�ʲ�Ʒ��������
#ifdef FUNC_MIC_NOISE_GATE_EN
	noise_gate_init(&ngct, 1, 44100, -45, -43, 5, 100, 500);
#endif
//	//��MICˮλ��䵽1024������
//	memset(AduioBuf, 0x00, sizeof(AduioBuf));
//	for(i = 0; i < 30; i++)
//	{
//		MixerSetData(MIXER_SOURCE_ADC, (void*)AduioBuf, MIC_BUF_LEN);
//		while(!MixerIsDone(MIXER_SOURCE_ADC));
//		if(MixerGetRemainSamples(MIXER_SOURCE_ADC) > 1536)
//		{
//			break;
//		}
//	}

	while(1)
	{
		if(gSys.CurModuleID == MODULE_ID_BT_HF)
		{
#ifdef BT_HF_RECORD_FUNC
			if(GetBtRecordState() == BT_RECORD_STATE_RECORDING)
			{
				uint8_t *	out_encoded_buf = NULL;
				uint32_t	out_encoded_buf_len = 0;

				if(audio_encoder_encode(&out_encoded_buf, &out_encoded_buf_len) == RT_SUCCESS)
				{
					EncodedDataToBuffer(out_encoded_buf, out_encoded_buf_len);
				}
				OSRescheduleTimeout(2); 
			}
			else
#endif
			{
				OSRescheduleTimeout(10);
			}
			continue;
		}

		OSRescheduleTimeout(1); 

        if(gSys.CurModuleID == MODULE_ID_I2SIN)
        {
            SampleCnt = I2sPmemPcmRemainLenGet();
        }
        else
        {
            SampleCnt = AdcPmemPcmRemainLenGet();
        }

		if(SampleCnt > MIC_BUF_LEN)
		{
			SampleCnt = MIC_BUF_LEN;
		}
		SampleCnt &= 0xFFFE;	//echoģ��Ҫ��ÿ�δ���ĵ���������ż��
		
#ifdef AUDIO_EFFECT_SWITCH_FADEOUT_FADEIN
		if((SampleCnt< MIC_BUF_LEN)
#if defined(FUNC_USB_AUDIO_EN) || defined(FUNC_USB_AUDIO_READER_EN)
			&& !(gSys.CurModuleID == MODULE_ID_USB_AUDIO || gSys.CurModuleID == MODULE_ID_USB_AUDIO_READER)
#endif
		)
		{
			continue;
		}
#endif			

#if defined(FUNC_USB_AUDIO_EN) || defined(FUNC_USB_AUDIO_READER_EN)
		if(gSys.CurModuleID == MODULE_ID_USB_AUDIO || 
			gSys.CurModuleID == MODULE_ID_USB_AUDIO_READER)
		{
			if(MixerIsDone(MIXER_SOURCE_USB))
			{
				uint16_t PacketLen;
				PacketLen = MCUCircular_GetSpaceLen(&OTGCircularBuf);
				if(PacketLen < OTG_AUDIO_RX_BUF_SIZE / 2)
				{
					PacketLen = MCUCircular_GetData(&OTGCircularBuf, (void*)AduioBuf, 192);
					if(PacketLen > 0)
					{
#if defined(FUNC_AMP_MUTE_EN) && defined(AMP_SILENCE_MUTE_EN)
						SilenceAmpMuteProc((void*)AduioBuf, PacketLen/4);
#endif
#ifdef FUNC_SPEC_TRUM_EN
						SpecTrumProc((void*)AduioBuf, PacketLen/4);
#endif
#ifdef FUNC_AUDIO_3D_EFFECT_EN
						Audio3dPro((int16_t *)AduioBuf, MIXER_FORMAT_STERO, USB_DAC_FREQ, PacketLen/4);
#endif
#ifdef FUNC_AUDIO_VB_EFFECT_EN
						AudioVbPro((int16_t *)AduioBuf, MIXER_FORMAT_STERO, USB_DAC_FREQ, PacketLen/4);
#endif

#if (EQ_STYLE_MODE == 2) || (EQ_STYLE_MODE == 3)
						if(gSys.EqStatus == 1)
							AudioSwEQPro(AduioBuf, MIXER_FORMAT_STERO, PacketLen/4);
#endif
#ifdef FUNC_AUDIO_DRC_EFFECT_EN
						AudioDRCPro((int16_t *)AduioBuf, MIXER_FORMAT_STERO, USB_DAC_FREQ, PacketLen/4);
#endif
						MixerSetData(MIXER_SOURCE_USB, (void*)AduioBuf, PacketLen / 4);	
					}
				}
			}
		}
#endif		
		if((gSys.MicEnable) && (gSys.CurModuleID != MODULE_ID_I2SIN))	//�����ǰ��MIC����MICռ��һ·ADC��LINEIN��GUITAR��FMռ����һ·ADC
		{
			if(MixerIsDone(MIXER_SOURCE_MIC) && MixerIsDone(MIXER_SOURCE_ANA_MONO))
			{
				//ADC�������������ݷֿ��ţ�ǰSampleCnt����������ADCһ����������SampleCnt����������ADC��һ������
#ifdef CFG_WAV_REMINDSOUND_MIX_EN
				if(IsRmsPcmDataReminding())
				{
					SampleCnt = MIC_BUF_LEN;
					SampleCnt = RmsPcmDataRead((void*)AduioBuf, SampleCnt);
				}
				else
#endif
				{
					SampleCnt = AdcPcmDataRead((void*)AduioBuf, SampleCnt, 0x04);
				}	
#ifdef FUNC_DC_BLOCKER
				if(init_dcb_channel != 1)
				{
					init_dcb_channel = 1;
					memset(&dcb, 0, sizeof(DCBlocker));
					init_dc_blocker(&dcb, init_dcb_channel);
				}

				if(gSys.CurModuleID == MODULE_ID_LINEIN)
				{
					apply_dc_blocker(&dcb, (void*)AduioBuf, (void*)AduioBuf, SampleCnt);
				}
				
				#ifdef FUNC_SILENCE_DETECTOR
				if(init_sd_channel != 1)
				{
					init_sd_channel = 1;
					silence_counter = 0;
					memset(&sdct, 0, sizeof(SilenceDetectorContext));
					init_silence_detector(&sdct, init_sd_channel, 44100);
				}
				
				if(gSys.CurModuleID == MODULE_ID_LINEIN)
				{
					smoothed_pcm_amplitude = apply_silence_detector(&sdct, AduioBuf, SampleCnt);
					if(smoothed_pcm_amplitude < SILENCE_THRSHOLD)
					{
						silence_counter++; 
					}
					else
					{
						silence_counter = 0;
					}

					if(silence_counter > SILENCE_COUNT)
					{
						// go to sleep
					}
				}
				#endif
#endif
#ifdef FUNC_MIC_NOISE_GATE_EN
				noise_gate_apply(&ngct, AduioBuf, AduioBuf, SampleCnt);
#endif
#ifdef FUNC_MIC_ECHO_EN
#ifdef MIC_ECHO_MIX_BYPASS_EN
				memcpy(AduioBuf_MIC_Bypass, AduioBuf, SampleCnt * 2);
#endif
#endif
#ifdef FUNC_MIC_ECHO_EN
				if(!OldMicEnable)
				{
					memset(&ct, 0x00, sizeof(ct));
					echo_init(&ct, 1, DacAdcSampleRateGet(), CFG_ECHO_CUTOFF_FREQ, ECHO_DELAY_SAMPLE, echo_sbuf);
				}
				echo_apply(&ct, (void*)AduioBuf, (void*)AduioBuf, SampleCnt, EchoDepthStep * DEPTH_ONE_STEP, EchoDelayStep * DELAY_ONE_STEP * DacAdcSampleRateGet() / 44100, CFG_ECHO_DIRECT_SND_EN); 
				
#ifdef MIC_ECHO_MIX_BYPASS_EN
				{
					uint32_t Cnt;
					for(Cnt = 0; Cnt < SampleCnt; Cnt++)
					{
						AduioBuf[Cnt] = __ssat(((AduioBuf_MIC_Bypass[Cnt] * MIC_BYPASS_VOL) >> 12) + ((AduioBuf[Cnt] * MIC_ECHO_EFFECT_VOL) >> 12), 16);
					}
				}
#endif
#endif
#ifdef FUNC_RECORD_EN
				MixerForRecoder((void*)AduioBuf, (void*)(AduioBuf + SampleCnt), SampleCnt, 0x04);	
#endif
				MixerSetData(MIXER_SOURCE_MIC, (void*)AduioBuf, SampleCnt);		//������MIC����
#if defined(FUNC_USB_AUDIO_EN) || defined(FUNC_USB_READER_EN) || defined(FUNC_USB_AUDIO_READER_EN)
				DeviceAudioSetUpData((void*)AduioBuf, SampleCnt*2);
#endif

				if((gSys.CurModuleID == MODULE_ID_LINEIN) 
					|| (gSys.CurModuleID == MODULE_ID_RADIO) 
					|| (gSys.CurModuleID == MODULE_ID_RECORDER))
				{
#ifdef FUNC_SPEC_TRUM_EN
					SpecTrumProc((void*)(AduioBuf + SampleCnt), SampleCnt);
#endif
#if defined(FUNC_AMP_MUTE_EN) && defined(AMP_SILENCE_MUTE_EN)
					SilenceAmpMuteProc((void*)(AduioBuf + SampleCnt), SampleCnt);
#endif
				}
				
				MixerSetData(MIXER_SOURCE_ANA_MONO, (void*)(AduioBuf + SampleCnt), SampleCnt);	//������LINEIN��GUITAR��FM����
			}
		}
		else			//�����ǰ��MIC����LINEIN��FM, I2SINΪ˫��������
		{
			if(MixerIsDone(MIXER_SOURCE_ANA_STERO) && gSys.CurModuleID != MODULE_ID_BLUETOOTH)
			{
				if(gSys.CurModuleID == MODULE_ID_I2SIN)
                {
                    SampleCnt = I2sPcmDataRead((void*)AduioBuf, SampleCnt, 0x03); 
#ifdef DIGITAL_PER_GAIN_EN            
					//����Ԥ����
					DigitalGainSet((int32_t *)AduioBuf, SampleCnt, 2, DIGITAL_PER_GAIN_VAL);
#endif
                }
                else
                {
                    SampleCnt = AdcPcmDataRead((void*)AduioBuf, SampleCnt, 0x03);	
                }
                
#ifdef FUNC_DC_BLOCKER
				if(init_dcb_channel != 2)
				{
					init_dcb_channel = 2;
					memset(&dcb, 0, sizeof(DCBlocker));
					init_dc_blocker(&dcb, init_dcb_channel);
				}

				if(gSys.CurModuleID == MODULE_ID_LINEIN)
				{
					apply_dc_blocker(&dcb, (void*)AduioBuf, (void*)AduioBuf, SampleCnt);
				}

				#ifdef FUNC_SILENCE_DETECTOR
				if(init_sd_channel != 2)
				{
					init_sd_channel = 2;
					silence_counter = 0;
					memset(&sdct, 0, sizeof(SilenceDetectorContext));
					init_silence_detector(&sdct, init_sd_channel, 44100);
				}
				
				if(gSys.CurModuleID == MODULE_ID_LINEIN)
				{
					smoothed_pcm_amplitude = apply_silence_detector(&sdct, AduioBuf, SampleCnt);
					if(smoothed_pcm_amplitude < SILENCE_THRSHOLD)
					{
						silence_counter++; 
					}
					else
					{
						silence_counter = 0;
					}

					if(silence_counter > SILENCE_COUNT)
					{
						// go to sleep
					}
				}
				#endif
#endif

#ifdef FUNC_RECORD_EN
				MixerForRecoder((void*)AduioBuf, (void*)(AduioBuf + 1), SampleCnt, 0x03);
#endif
                if((gSys.CurModuleID == MODULE_ID_LINEIN) 
					|| (gSys.CurModuleID == MODULE_ID_RADIO) 
					|| (gSys.CurModuleID == MODULE_ID_RECORDER))
            	{
#if defined(FUNC_AMP_MUTE_EN) && defined(AMP_SILENCE_MUTE_EN)
					SilenceAmpMuteProc((void*)AduioBuf, SampleCnt);
#endif
#ifdef FUNC_SPEC_TRUM_EN
					SpecTrumProc((void*)AduioBuf, SampleCnt);
#endif
            	}

#if defined(FUNC_AUDIO_DRC_EFFECT_EN) || defined(FUNC_AUDIO_3D_EFFECT_EN) || defined(FUNC_AUDIO_VB_EFFECT_EN)||(EQ_STYLE_MODE == 2) || (EQ_STYLE_MODE == 3)
                if(((gSys.CurModuleID == MODULE_ID_LINEIN) 
					|| (gSys.CurModuleID == MODULE_ID_RADIO) 
					|| (gSys.CurModuleID == MODULE_ID_RECORDER)
                    || (gSys.CurModuleID == MODULE_ID_I2SIN)) 
#ifdef FUNC_SOUND_REMIND
				&& (!IsSoundRemindPlaying())
#endif
				)                 
                {
#ifdef FUNC_AUDIO_3D_EFFECT_EN   
                    Audio3dPro(AduioBuf, 2, 44100, SampleCnt);
#endif

#ifdef FUNC_AUDIO_VB_EFFECT_EN
                    AudioVbPro(AduioBuf, 2, 44100, SampleCnt);
#endif
#if (EQ_STYLE_MODE == 2) || (EQ_STYLE_MODE == 3)
					if(gSys.EqStatus == 1)
						AudioSwEQPro(AduioBuf, 2, SampleCnt);
#endif

#ifdef FUNC_AUDIO_DRC_EFFECT_EN
                    AudioDRCPro(AduioBuf, 2, 44100, SampleCnt);
#endif

#ifdef AUDIO_EFFECT_SWITCH_FADEOUT_FADEIN
					if(IsFadeOutEn)
					{
						DigitalFadeOut((int32_t*)AduioBuf, SampleCnt, 2);
					}
					else if(IsFadeInEn)
					{
						DigitalFadeIn((int32_t*)AduioBuf, SampleCnt, 2);
					}
#endif
                }
#endif
				MixerSetData(MIXER_SOURCE_ANA_STERO, (void*)AduioBuf, SampleCnt);	//˫����LINEIN��GUITAR��FM����
			}
		}

#ifdef FUNC_MIC_ECHO_EN
		OldMicEnable = gSys.MicEnable;
#endif
	}
}
