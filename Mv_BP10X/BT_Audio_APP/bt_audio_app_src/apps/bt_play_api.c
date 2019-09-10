/**
 **************************************************************************************
 * @file    bt_play_api.c
 * @brief   
 *
 * @author  kk
 * @version V1.0.0
 *
 * $Created: 2017-3-17 13:06:47$
 * 
 * @Copyright (C) 2016, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */

//�й�����A2DP���ŵ���ش��������ô��ļ�

#include "string.h"
#include "type.h"
#include "app_config.h"
#include "app_message.h"
#include "gpio.h"
#include "irqn.h"
#include "gpio.h"
#include "clk.h"
#include "dac.h"
#include "rtos_api.h"
#include "freertos.h"
#include "debug.h"
#include "bt_play_api.h"
#include "bt_play_mode.h"
#include "audio_core_api.h"
#include "decoder_service.h"
#include "device_detect.h"
#include "typedefine.h"
#include "audio_decoder_api.h"
#include "main_task.h"
#include "mode_switch_api.h"
#include "bt_app_interface.h"
#include "bt_avrcp_api.h"
#include "bt_manager.h"

#if (BT_AVRCP_SONG_TRACK_INFOR == ENABLE)
#include "string_convert.h"
#endif

#ifdef CFG_APP_BT_MODE_EN

typedef SongInfo		SbcSongInfo;

//#define SBC_DECODER_INPUT_LEN			(5*1024)//(10*1024)

uint8_t *sbcBuf = NULL;

static bool	sbcDecoderInitFlag = FALSE;
static bool	sbcDecoderStarted = FALSE;

//static MemHandle SBC_MemHandle;
MemHandle SBC_MemHandle;


typedef struct _SbcDecoderContext
{
	AudioDecoderContext		*audioDecoderCt;
	SbcSongInfo				*sbcSongInfo;

//	BT_PLAYER_STATE			curPlayState;
//#ifdef CFG_FUNC_REMIND_SOUND_EN
//	DecoderShare		PlayerDecoderShare;
//#endif
}SbcDecoderContext;

static SbcDecoderContext	sbcDecoderCt = {0};

static int16_t SaveBtA2dpStreamData(uint8_t * data, uint16_t dataLen);
void GetBtMediaInfo(void *params);

static bool IsSbcDecoderInitialized(void)
{
	return sbcDecoderInitFlag;
}

static MemHandle * GetSbcDecoderMemHandle(void)
{
	if(IsSbcDecoderInitialized())
	{
		return &SBC_MemHandle;
	}
	return NULL;
}

SbcSongInfo * GetSbcSongInfo(void)
{
	if(IsSbcDecoderInitialized())
		return sbcDecoderCt.sbcSongInfo;
	return NULL;
}

int32_t SbcDecoderInit(void)
{
	sbcBuf = osPortMalloc(BT_SBC_DECODER_INPUT_LEN);
	if(sbcBuf == NULL)
		return -1;
	memset(sbcBuf, 0, BT_SBC_DECODER_INPUT_LEN);
	
	SBC_MemHandle.addr = sbcBuf;
	SBC_MemHandle.mem_capacity = BT_SBC_DECODER_INPUT_LEN;
	SBC_MemHandle.mem_len = 0;
	SBC_MemHandle.p = 0;
	
	sbcDecoderInitFlag = TRUE;
	SetSbcDecoderStarted(FALSE);

	//register function
	BtAppiFunc_SaveA2dpData(SaveBtA2dpStreamData);
	BtAppiFunc_RefreshSbcDecoder(BtSbcDecoderRefresh);
#if (BT_AVRCP_SONG_TRACK_INFOR == ENABLE)
	BtAppiFunc_GetMediaInfo(GetBtMediaInfo);
#else
	BtAppiFunc_GetMediaInfo(NULL);
#endif

//#ifdef CFG_FUNC_REMIND_SOUND_EN
//	sbcDecoderCt.PlayerDecoderShare = SHARE_OWN_USING;
//#endif

	return 0;
}

//������ ֹͣ�������
void BtPlayerDecoderStop(void)
{//��App�⣬����������û�и��ã��ҽ�������ֹ̬ͣ
	if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
	{
		DecoderStop();
	}
}


void SbcDecoderRefresh(void)
{
	if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
	{
		SBC_MemHandle.addr = sbcBuf;
		SBC_MemHandle.mem_capacity = BT_SBC_DECODER_INPUT_LEN;
		SBC_MemHandle.mem_len = 0;
		SBC_MemHandle.p = 0;

		sbcDecoderInitFlag = TRUE;
		SetSbcDecoderStarted(FALSE);
	}
}

void BtSbcDecoderRefresh(void)
{
	BtPlayerDecoderStop();
	SbcDecoderRefresh();
}

int32_t SbcDecoderDeinit(void)
{
	//deregister function
	BtAppiFunc_SaveA2dpData(NULL);
	BtAppiFunc_GetMediaInfo(NULL);
	BtAppiFunc_RefreshSbcDecoder(NULL);
	
	SBC_MemHandle.addr = NULL;
	SBC_MemHandle.mem_capacity = 0;
	SBC_MemHandle.mem_len = 0;
	SBC_MemHandle.p = 0;
	
	sbcDecoderInitFlag = FALSE;
	SetSbcDecoderStarted(FALSE);

	osPortFree(sbcBuf);
	sbcBuf = NULL;
	return 0;
}

uint32_t InsertDataToSbcBuffer(uint8_t * data, uint16_t dataLen)
{
	uint32_t	insertLen = 0;
	int32_t		remainLen = 0;

	if(sbcDecoderInitFlag)
	{
		remainLen = mv_mremain(&SBC_MemHandle);
		if(BT_SBC_DECODER_INPUT_LEN - remainLen > BT_SBC_LEVEL_LOW //ˮλ���ż���
			&& GetBtPlayState() == BT_PLAYER_STATE_PLAYING //��ǰ����̬
			&& GetBtMuteState()) //bt play ���������ھ��������ݻ���
		{

			SetBtMuteState(FALSE);
			BtPlayerPlay();
		}
		if(remainLen <= (dataLen+8))
		{
			BT_DBG("F");//sbc fifo full
			//���Ӷ�ָ��
			//
			return 0;
		}
		insertLen = mv_mwrite(data, dataLen, 1,&SBC_MemHandle);
		if(BT_SBC_DECODER_INPUT_LEN - remainLen < BT_SBC_DECODER_INPUT_LEN >> 3)
		{//fifo���ݲ���ʱ���������ݼ�ʱ֪ͨdecoder
			SbcDataNotify();
		}
		if(insertLen != dataLen)
		{
			BT_DBG("insert data len err! i:%ld,d:%d\n", insertLen, dataLen);
		}
	}
	return insertLen;
}

uint32_t GetValidSbcDataSize(void)
{
	uint32_t	dataSize = 0;
	if(sbcDecoderInitFlag)
	{
		//osMutexLock(SbcDecoderMutex);
		dataSize = mv_msize(&SBC_MemHandle);
		//osMutexUnlock(SbcDecoderMutex);
	}
	return dataSize;
}

int32_t SbcDecoderStart(void)
{
	int32_t 		ret = 0;
	ret = DecoderInit(GetSbcDecoderMemHandle(), (int32_t)IO_TYPE_MEMORY, SBC_DECODER);
	//BT_DBG("audio_decoder_initialize ret:%d\n",ret);
	if(ret != RT_SUCCESS)
	{
		BT_DBG("[BT]<SBC>: audio_decoder_initialize error code:%ld!\n", audio_decoder_get_error_code());
		SetSbcDecoderStarted(FALSE);
		return -1;
	}
	DecoderPlay();
	audio_decoder->song_info = audio_decoder_get_song_info();

#ifndef CFG_FUNC_MIXER_SRC_EN
#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_SampleRateChange(ALL, audio_decoder->song_info->sampling_rate);
#else
	AudioDAC_SampleRateChange(DAC0, audio_decoder->song_info->sampling_rate);
#endif
	BT_DBG("DAC Sample rate = %d\n", (int)AudioDAC_SampleRateGet(DAC0));
#endif	
	//BT_DBG("decoder channel number = %d\n",(int)audio_decoder->song_info->num_channels);
	//BT_DBG("decoder SampleRate = %d\n",(int)audio_decoder->song_info->sampling_rate);

//	AudioCoreSourcePcmFormatConfig(DecoderSourceNumGet(), (int)audio_decoder->song_info->num_channels);//only testĬ��ʹ��0 source

	BT_DBG("Decoder Service Start...\n");
	AudioCoreSourceEnable(DecoderSourceNumGet());
//	DecoderServiceStart();
DBG("En Channel\n");
	SetSbcDecoderStarted(TRUE);
#ifdef CFG_FUNC_REMIND_SOUND_EN
	SoftFlagDeregister(SoftFlagDecoderMask & ~SoftFlagDecoderApp);//��ȫ������
	switch(GetBtPlayState())
	{
		case BT_PLAYER_STATE_PLAYING://��play
			break;
		
		case BT_PLAYER_STATE_PAUSED:
			DecoderPause();
			break;
		
		case BT_PLAYER_STATE_STOP:
			DecoderStop();
			break;
		
		default:
			break;
	}
#endif	
	return 0;
}

void SetSbcDecoderStarted(bool flag)
{
	sbcDecoderStarted = flag;
}

bool GetSbcDecoderStarted(void)
{
	return sbcDecoderStarted;
}


void BtPlayerPlay(void)
{

#ifdef CFG_FUNC_REMIND_SOUND_EN
	if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))//������û�б�ռ�ã�Ҳ���г�ʱ
#endif
	{
#ifdef CFG_FUNC_FREQ_ADJUST
		AudioCoreSourceFreqAdjustEnable(DecoderSourceNumGet(), BT_SBC_LEVEL_LOW, BT_SBC_LEVEL_HIGH);
#endif
		AudioCoreSourceUnmute(DecoderSourceNumGet(), 1, 1);
		AudioCoreSourceEnable(DecoderSourceNumGet());
		DecoderResume();  //
	}
	SetBtPlayState(BT_PLAYER_STATE_PLAYING);//״̬�ٵǼǡ�
}

void BtPlayerPause(void)
{
	/*if(GetBtPlayState() == BT_PLAYER_STATE_PLAYING)
    {
        DecoderPause();
        SetBtPlayState(BT_PLAYER_STATE_PAUSED);
	}*/
#ifdef CFG_FUNC_REMIND_SOUND_EN
	if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
#endif
	{
		//DecoderPause();//ȡ���������ԭ���ǣ�sbc���ݲ��㣬�������Զ������룬Ϊ������ʱ���и費������������ֻmute�Ϳ���ͨ��
	}
	SetBtPlayState(BT_PLAYER_STATE_PAUSED);
}
//����&��ͣ
void BtPlayerPlayPause(void)
{
    if(GetBtPlayState() == BT_PLAYER_STATE_PLAYING)
    {
#ifdef CFG_FUNC_REMIND_SOUND_EN
		if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
#endif
		{
        	DecoderPause();
		}
        SetBtPlayState(BT_PLAYER_STATE_PAUSED);
	}
	else if(GetBtPlayState() == BT_PLAYER_STATE_STOP)
	{
	    SetBtPlayState(BT_PLAYER_STATE_PLAYING);
	}
	else
	{
#ifdef CFG_FUNC_REMIND_SOUND_EN
		if(!SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
#endif
		{
	    	DecoderResume();
		}
	    SetBtPlayState(BT_PLAYER_STATE_PLAYING);
	}
}


static int16_t SaveBtA2dpStreamData(uint8_t * data, uint16_t dataLen)
{
	if(AppModeBtAudioPlay != GetSystemMode() 
#ifdef CFG_FUNC_REMIND_SOUND_EN
		||(SoftFlagGet(SoftFlagDecoderMask & ~SoftFlagDecoderApp))
#endif
		)
	{
		return -1;
	}
	//save a2dp stream data to sbc buffer
	if(GetA2dpState() == BT_A2DP_STATE_STREAMING)
	{
		InsertDataToSbcBuffer(data, dataLen);
	}
	
	if(!GetSbcDecoderStarted() && !SoftFlagGet(SoftFlagDecoderRemind))
	{
		//if(GetValidSbcDataSize() > BT_SBC_LEVEL_HIGH)//�����������Э��ջ�������ȼ����˴�Ҫ��LOW�����������������ʧ���ݡ�
		if(GetValidSbcDataSize() > BT_SBC_LEVEL_LOW)//�����������ȼ�Ϊ4���˴�Ҫ��LOW�����������������ʧ���ݡ�
		{
			MessageHandle		msgHandle;
			MessageContext		msgSend;
			
			msgHandle = GetBtPlayMessageHandle();
			if(msgHandle == NULL)
				return -2;

			// Send message to bt play mode
			msgSend.msgId		= MSG_BT_PLAY_DECODER_START;
			MessageSend(msgHandle, &msgSend);
			//if(GetBtPlayState() == BT_PLAYER_STATE_STOP)
			{
				SetBtPlayState(BT_PLAYER_STATE_PLAYING);//for first state change add by pi
			}
			SetSbcDecoderStarted(TRUE);
		}
	}

	return 0;
}

#if (BT_AVRCP_SONG_TRACK_INFOR == ENABLE)
void GetBtMediaInfo(void *params)
{
	#define StringMaxLen 60
	AvrcpAdvMediaInfo	*CurMediaInfo;
	uint8_t i;
	uint8_t StringData[StringMaxLen];
	uint8_t ConvertStringData[StringMaxLen];
	CurMediaInfo = (AvrcpAdvMediaInfo*)params;

	if((CurMediaInfo)&&(CurMediaInfo->numIds))
	{
		for(i=0;i<CurMediaInfo->numIds;i++)
		{
			memset(StringData, 0, StringMaxLen);
			memset(ConvertStringData, 0, StringMaxLen);
			
			//Attribute ID
			switch(CurMediaInfo->property[i].attrId)
			{
				case 1:
					BT_DBG("Title of the media\n");
					break;

				case 2:
					BT_DBG("Name of the artist\n");
					break;

				case 3:
					BT_DBG("Name of the Album\n");
					break;

				case 4:
					BT_DBG("Number of the media\n");
					break;

				case 5:
					BT_DBG("Totle number of the media\n");
					break;

				case 6:
					BT_DBG("Genre\n");
					break;

				case 7:
					BT_DBG("Playing time in millisecond\n");
					break;
				
				case 8:
					BT_DBG("Default cover art\n");
					break;

				default:
					break;
			}

			if(CurMediaInfo->property[i].length)
			{
				if(CurMediaInfo->property[i].charSet == 0x006a)
				{
					BT_DBG("Character Set Id: UTF-8\n");
					
					if(CurMediaInfo->property[i].length > StringMaxLen)
					{
						memcpy(StringData, CurMediaInfo->property[i].string, StringMaxLen);
					#ifdef CFG_FUNC_STRING_CONVERT_EN
						StringConvert(ConvertStringData, 60, StringData, StringMaxLen ,UTF8_TO_GBK);
					#endif
					}
					else
					{
						memcpy(StringData, CurMediaInfo->property[i].string, CurMediaInfo->property[i].length);
					#ifdef CFG_FUNC_STRING_CONVERT_EN
						StringConvert(ConvertStringData, 60, StringData, CurMediaInfo->property[i].length ,UTF8_TO_GBK);
					#endif
					}

					#ifdef CFG_FUNC_STRING_CONVERT_EN
						BT_DBG("%s\n", ConvertStringData);
					#endif
				}
				else
				{
					;//BT_DBG("Other Character Set Id: 0x%x\n", CurMediaInfo->property[i].charSet);
				}
			}
		}
	}
}
#endif

void BtAutoPlayMusic(void)
{
	if(GetSystemMode() == AppModeBtAudioPlay)
	{
		BTCtrlPlay();
	}
}

#else

void BtSbcDecoderRefresh(void)
{
}


#endif//#ifdef CFG_APP_BT_MODE_EN
