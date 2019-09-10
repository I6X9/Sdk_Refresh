/**
 **************************************************************************************
 * @file    audio_core_api.h
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

#ifndef __AUDIO_CORE_API_H__
#define __AUDIO_CORE_API_H__

typedef uint16_t (*AudioCoreDataGetFunc)(void* Buf, uint16_t Samples);
typedef uint16_t (*AudioCoreDataLenGetFunc)(void);
typedef uint16_t (*AudioCoreDataSetFunc)(void* Buf, uint16_t Samples);
typedef uint16_t (*AudioCoreDataSpaceLenSetFunc)(void);

typedef void (*AudioCoreProcessFunc)(void*);//��Ч����������

#include "app_config.h"

#define AUDIO_CORE_SOURCE_MAX_MUN	4
#if ((defined(CFG_RES_AUDIO_DACX_EN ) || defined(CFG_RES_AUDIO_I2SOUT_EN)) && (defined(CFG_APP_BT_MODE_EN) && (BT_HFP_SUPPORT == ENABLE)))
#define AUDIO_CORE_SINK_MAX_NUM		4
#elif (defined(CFG_RES_AUDIO_DACX_EN) || defined(CFG_RES_AUDIO_I2SOUT_EN) || (defined(CFG_APP_BT_MODE_EN) && (BT_HFP_SUPPORT == ENABLE)))
#define AUDIO_CORE_SINK_MAX_NUM		3
#else
#define AUDIO_CORE_SINK_MAX_NUM		2
#endif

//SINK2,DACX��I2SOUT ����ϻ��⣬���ͬʱ���ڣ��ͻ������޸Ĵ���
#define MIC_SOURCE_NUM			0 //��˷���audiocore sourceͨ��,��ЧԤ���� ����Ϊ0
#define APP_SOURCE_NUM			1 //app��Ҫ��Դͨ��,������Ч
#define REMIND_SOURCE_NUM		2 //��ʾ��ʹ�ù̶�����ͨ�� ����Ч
#define PLAYBACK_SOURCE_NUM		3//flashfs ¼���ط�ͨ��		����Ч
#define AUDIO_DAC0_SINK_NUM		0//����Ƶ�����audiocore Sink�е�ͨ�����������ã�audiocore���ô�ͨ��buf��������
#define AUDIO_RECORDER_SINK_NUM	1//¼��ר��ͨ��		 ��������ʾ����Դ��		
#define AUDIO_HF_SCO_SINK_NUM	2//����sco��������ͨ��
#if (defined(CFG_APP_BT_MODE_EN) && (BT_HFP_SUPPORT == ENABLE))
#define AUDIO_PLUS_SINK_NUM		3//��Ƶ�������ͨ�����û����䡣
#else
#define AUDIO_PLUS_SINK_NUM		2//��Ƶ�������ͨ�����û����䡣
#endif
//����ͨ·��Ҫͬʱʹ��
#define AUDIO_DACX_SINK_NUM		AUDIO_PLUS_SINK_NUM//dacxʹ��ͨ��2
#define AUDIO_I2SOUT_SINK_NUM	AUDIO_PLUS_SINK_NUM//I2SOUTʹ��ͨ��2��


typedef enum
{
	SOURCE_DECODE,
	SOURCE_MIC_ADC0,
	SOURCE_LINEIN_ADC0,
	SOURCE_I2S0,
}AUDIO_SOURCE_Type;

typedef enum
{
	SINK_DAC,
	SINK_I2S0,
	SINK_I2S1,
}AUDIO_SINK_Type;

typedef struct _AudioCoreSource
{
	AUDIO_SOURCE_Type			SrcType;
	uint8_t						Index;
	uint16_t					PcmFormat;	//���ݸ�ʽ,sign or stereo
	bool						Enable; 	//
	bool						IsSreamData;//�Ƿ�������
	AudioCoreDataGetFunc		FuncDataGet;//****��������ں���
	AudioCoreDataLenGetFunc		FuncDataGetLen;
	int16_t						*PcmInBuf;	//��������buf
	int16_t						PreGain;
	int16_t						LeftVol;	//����
	int16_t						RightVol;	//����
	int16_t						LeftCurVol;	//��ǰ����
	int16_t						RightCurVol;//��ǰ����
	bool						LeftMuteFlag;//������־
	bool						RightMuteFlag;//������־
}AudioCoreSource;


typedef struct _AudioCoreSink
{
	AUDIO_SINK_Type					SinkType;
	uint8_t							Index;
	uint16_t						PcmFormat;	//���ݸ�ʽ��1:���������; 2:���������
	bool							Enable;
	AudioCoreDataSetFunc			FuncDataSet;//****������� ��buf->�⻺����ƺ���
	AudioCoreDataSpaceLenSetFunc	FuncDataSpaceLenSet;	
	int16_t							*PcmOutBuf;
	int16_t							LeftVol;	//����
	int16_t							RightVol;	//����
	int16_t							LeftCurVol;	//��ǰ����
	int16_t							RightCurVol;//��ǰ����
	bool							LeftMuteFlag;//������־
	bool							RightMuteFlag;//������־

}AudioCoreSink;

typedef struct _AudioCoreContext
{
	AudioCoreSource AudioSource[AUDIO_CORE_SOURCE_MAX_MUN];
	AudioCoreProcessFunc AudioEffectProcess;			//****���������
	AudioCoreSink   AudioSink[AUDIO_CORE_SINK_MAX_NUM];

}AudioCoreContext;

extern AudioCoreContext		AudioCore;

//typedef void (*AudioCoreProcessFunc)(AudioCoreContext *AudioCore);
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
void AudioCoreSourceFreqAdjustEnable(uint8_t AsyncIndex, uint16_t LevelLow, uint16_t LevelHigh);


void AudioCoreSourceFreqAdjustDisable(void);


void AudioCoreSourcePcmFormatConfig(uint8_t Index, uint16_t Format);

void AudioCoreSourceEnable(uint8_t Index);

void AudioCoreSourceDisable(uint8_t Index);

void AudioCoreSourceMute(uint8_t Index, bool IsLeftMute, bool IsRightMute);

void AudioCoreSourceUnmute(uint8_t Index, bool IsLeftUnmute, bool IsRightUnmute);

void AudioCoreSourceVolSet(uint8_t Index, uint16_t LeftVol, uint16_t RightVol);

void AudioCoreSourceVolGet(uint8_t Index, uint16_t* LeftVol, uint16_t* RightVol);

void AudioCoreSourceConfig(uint8_t Index, AudioCoreSource* Source);

void AudioCoreSinkEnable(uint8_t Index);

void AudioCoreSinkDisable(uint8_t Index);

void AudioCoreSinkMute(uint8_t Index, bool IsLeftMute, bool IsRightMute);

void AudioCoreSinkUnmute(uint8_t Index, bool IsLeftUnmute, bool IsRightUnmute);

void AudioCoreSinkVolSet(uint8_t Index, uint16_t LeftVol, uint16_t RightVol);

void AudioCoreSinkVolGet(uint8_t Index, uint16_t* LeftVol, uint16_t* RightVol);

void AudioCoreSinkConfig(uint8_t Index, AudioCoreSink* Sink);

void AudioCoreProcessConfig(AudioCoreProcessFunc AudioEffectProcess);

void AudioCoreConfig(AudioCoreContext *AudioCoreConfig);

bool AudioCoreInit(void);

void AudioCoreDeinit(void);

void AudioCoreRun(void);

//��������
void AduioCoreSourceVolSet(void);
void AduioCoreSinkVolSet(void);


#endif //__AUDIO_CORE_API_H__
