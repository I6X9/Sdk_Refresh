///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: slave_bt_control.c
//  maintainer: qing
///////////////////////////////////////////////////////////////////////////////

#include "app_config.h"
#include "dac.h"
#include "gpio.h"
#include "beep.h"
#include "echo.h"
#include "audio_adc.h"
#include "audio_path.h"
#include "clk.h"
#include "pmem.h"
#include "pcm_transfer.h"
#include "pcm_fifo.h"
#include "line_in_control.h"
#include "sys_vol.h"
#include "sound_remind.h"
#include "dev_detect_driver.h"
#include "recorder_control.h"
#include "breakpoint.h"
#include "sys_app.h"
#include "nvm.h"
#include "eq_params.h"
#include "bt_app_func.h"
#include "mixer.h"
#include "wifi_mcu_command.h"
#include "wifi_control.h" 
#include "wifi_uart_com.h"

#ifdef	OPTION_CHARGER_DETECT
extern bool IsInCharge(void);
#endif

bool BluetoothSoundFlag = FALSE;
TIMER BtStatusDetTimer;
uint8_t CurSlaveBtStatus = 0;
uint16_t TempSlaveBtStatus = 0;
uint8_t PrevSlaveBtStatus = 0;

#define BLUE_LED_CB_TIME   150       //LED显示时间

#ifdef FUNC_SLAVE_BLUETOOTH_EN

void SlaveBtGpioInit(void)
{
#ifdef SLAVE_BTDETE_EN
	//设为输入，带上拉
	GpioClrRegBits(SLAVE_BTDETE_PORT_OE, SLAVE_BTDETE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTDETE_PORT_PU, SLAVE_BTDETE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTDETE_PORT_PD, SLAVE_BTDETE_PORT_BIT);
	
	TimeOutSet(&BtStatusDetTimer, 0);
	GpioIntEn(SLAVE_BTDETE_PORT_INT, SLAVE_BTDETE_PORT_BIT, GPIO_NEG_EDGE_TRIGGER);
	//enable gpio irqc
	NVIC_EnableIRQ(GPIO_IRQn);
#endif
#ifdef SLAVE_BTMUTE_EN
	//设为输入，带上拉
	GpioClrRegBits(SLAVE_BTMUTE_PORT_OE, SLAVE_BTMUTE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTMUTE_PORT_PU, SLAVE_BTMUTE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTMUTE_PORT_PD, SLAVE_BTMUTE_PORT_BIT);
	GpioSetRegBits(SLAVE_BTMUTE_PORT_IE, SLAVE_BTMUTE_PORT_BIT);
#endif
#ifdef SLAVE_BTPLAY_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTNEXT_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
#ifdef SLAVE_BTPREV_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif
#ifdef SLAVE_BTPOWER_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPOWER_PORT_PU, SLAVE_BTPOWER_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPOWER_PORT_PD, SLAVE_BTPOWER_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPOWER_PORT_IE, SLAVE_BTPOWER_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPOWER_PORT_OE, SLAVE_BTPOWER_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPOWER_PORT_OUT, SLAVE_BTPOWER_PORT_BIT);
#endif
}

// Slave Bt Status detect interrupt.
__attribute__((section(".driver.isr"))) void GpioInterrupt(void)
{
	if(GpioIntFlagGet(SLAVE_BTDETE_PORT_INT) == SLAVE_BTDETE_PORT_BIT)
	{
		GpioIntClr(SLAVE_BTDETE_PORT_INT, SLAVE_BTDETE_PORT_BIT);
		TempSlaveBtStatus = PastTimeGet(&BtStatusDetTimer);
		TimeOutSet(&BtStatusDetTimer, 2000);
	}
}

#if 0
static bool SlaveBtMuteDetect(void)
{
#ifdef SLAVE_BTMUTE_EN
	//设为输入，带上拉
	GpioClrRegBits(SLAVE_BTMUTE_PORT_OE, SLAVE_BTMUTE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTMUTE_PORT_PU, SLAVE_BTMUTE_PORT_BIT);
	GpioClrRegBits(SLAVE_BTMUTE_PORT_PD, SLAVE_BTMUTE_PORT_BIT);

	GpioSetRegBits(SLAVE_BTMUTE_PORT_IE, SLAVE_BTMUTE_PORT_BIT);

	if(GpioGetReg(SLAVE_BTMUTE_PORT_IN) & SLAVE_BTMUTE_PORT_BIT)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#endif
}	
#endif

static void SlaveBtPlaypause(void)
{
#ifdef SLAVE_BTPLAY_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
	WaitMs(120);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
	APP_DBG("SlaveBtPlaypause ........\n");
}

static void SlaveBtPrevSong(void)
{
#ifdef SLAVE_BTPREV_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
	WaitMs(120);
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif
}

static void SlaveBtNextSong(void)
{
#ifdef SLAVE_BTNEXT_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
	WaitMs(120);
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
}

void SlaveBtVolUp(void)
{
#ifdef SLAVE_BTPLAY_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTNEXT_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
	WaitMs(120);
#ifdef SLAVE_BTPLAY_EN
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTNEXT_EN
	GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
}

void SlaveBtVolDw(void)
{
#ifdef SLAVE_BTPLAY_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTPREV_EN
	//设置为输出，带下拉；
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
	GpioSetRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif
	WaitMs(120);
#ifdef SLAVE_BTPLAY_EN
	GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTPREV_EN
	GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif
}

void SlaveBtEnterPairedAndReject(void)
{
	if(CurSlaveBtStatus != 5)
	{
#ifdef SLAVE_BTPLAY_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTNEXT_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
#ifdef SLAVE_BTPREV_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif	
	 
	  WaitMs(120);
		GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
		APP_DBG("SlaveBtEnterPaired..................\n");
	}
	else
	{
	#ifdef SLAVE_BTNEXT_EN
		//设置为输出，带下拉；
		GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
		GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
	#endif
	#ifdef SLAVE_BTPREV_EN
		//设置为输出，带下拉；
		GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
		GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
		GpioSetRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
	#endif	
		 
		WaitMs(120);
		GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
		GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
	}
}

void SlaveBtClearPaired(void)
{
	if(CurSlaveBtStatus != 5)
	{
#ifdef SLAVE_BTPLAY_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PU, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_PD, SLAVE_BTPLAY_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPLAY_PORT_IE, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OE, SLAVE_BTPLAY_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
#endif
#ifdef SLAVE_BTNEXT_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PU, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_PD, SLAVE_BTNEXT_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTNEXT_PORT_IE, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OE, SLAVE_BTNEXT_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
#endif
#ifdef SLAVE_BTPREV_EN
	  //设置为输出，带下拉；
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_PU, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_PD, SLAVE_BTPREV_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPREV_PORT_IE, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_OE, SLAVE_BTPREV_PORT_BIT);
	  GpioSetRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
#endif	
	 
	  WaitMs(1100);
		GpioClrRegOneBit(SLAVE_BTPLAY_PORT_OUT, SLAVE_BTPLAY_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTPREV_PORT_OUT, SLAVE_BTPREV_PORT_BIT);
	  GpioClrRegOneBit(SLAVE_BTNEXT_PORT_OUT, SLAVE_BTNEXT_PORT_BIT);
		APP_DBG("SlaveBtClearPaired..................\n");
	}
}

//蓝牙模式下提示音播放
void SlaveBluetoothStateDet(void)
{
	static uint8_t TempState;
	static uint8_t SlaveBtStatusJitterCnt;
	
	if(TempSlaveBtStatus == FALSE)
	{
		return;
	}
		
	if(CurSlaveBtStatus != (TempSlaveBtStatus+25)/100)
	{
		if(TempState == (TempSlaveBtStatus+25)/100)
		{
			PrevSlaveBtStatus = CurSlaveBtStatus;
			CurSlaveBtStatus = TempState;
		  SlaveBtStatusJitterCnt = 0;
			TempState = 0;
		  APP_DBG("CurSlaveBtStatus = %d\n", CurSlaveBtStatus);
		}
		else
		{
			TempState = (TempSlaveBtStatus+25)/100;
		}
	}
	TempSlaveBtStatus = 0;
	
	if(SlaveBtStatusJitterCnt <= 15)
	{
	  SlaveBtStatusJitterCnt++;
	}
	else
	{
		if(PrevSlaveBtStatus != CurSlaveBtStatus)
		{
			PrevSlaveBtStatus = CurSlaveBtStatus;
			APP_DBG("Slave BT status detect TimeOut!!!\n");
		}
		return;
	}
	
	if(gSys.CurModuleID == MODULE_ID_SLAVE_BLUETOOTH)
	{
	  if(SlaveBtStatusJitterCnt == 4)
		{
			BluetoothSoundFlag = TRUE;
			if(CurSlaveBtStatus == 5)
			{
				SlaveBtStatusJitterCnt = 0;
			}
		}
	}
	else if((CurSlaveBtStatus == 7) && (PrevSlaveBtStatus != CurSlaveBtStatus))
	{
		if(!gWiFi.PlayState && (SlaveBtStatusJitterCnt >= 6))
		{
			APP_DBG("Slave BT status detect enter SlaveBluetoothModule\n");
			APP_DBG("SlaveBtStatusJitterCnt = %d\n", SlaveBtStatusJitterCnt);
			PrevSlaveBtStatus = CurSlaveBtStatus;
			gSys.NextModuleID = MODULE_ID_SLAVE_BLUETOOTH;
			MsgSend(MSG_COMMON_CLOSE);
		}
	}
}

//蓝牙模式下提示音播放
void SlaveBluetoothSoundRemind(void)
{
	APP_DBG("PrevSlaveBtStatus = %d\n", PrevSlaveBtStatus);
	if((CurSlaveBtStatus == 2) && (PrevSlaveBtStatus != CurSlaveBtStatus))
	{
		SoundRemind(SOUND_WAIT_PAIR);
	}
	else if(CurSlaveBtStatus == 5)
	{
	  SoundRemind(SOUND_CALL_INCOM);
	}
	else if((CurSlaveBtStatus >= 3) && (PrevSlaveBtStatus < 3))
	{
		SoundRemind(SOUND_BT_CONNECT);
	}
}

//蓝牙模式下的LED显示处理(200ms调用一次，此时间间隔WIFI_LED_CB_TIME 可以用户自己定义)
void BlueLedCb(void* Param)
{
	static uint8_t BlueLedDispCnt;
	
	BlueLedDispCnt++;
	
	if(gWiFi.VisInfState)
	{
		if(BlueLedDispCnt >= 2)
		{
			LED_YELLOW_MODE_ON();
			BlueLedDispCnt = 0;
		}
		else
		{
			LED_WHITE_MODE_OFF();
		}
		return;
	}
	
	if(!gSys.SleepLedOffFlag)	
	{
		if(CurSlaveBtStatus < 3)
		{
			LED_WHITE_MODE_OFF();
			if(BlueLedDispCnt >= 2 )
			{
				LED_RED_MODE_ON();
				BlueLedDispCnt = 0;
			}
			else
			{
				LED_BLUE_MODE_ON();
			}
			return;
		}
		else 
		{
			LED_RED_MODE_OFF();
			LED_BLUE_MODE_ON();
			LED_GREEN_MODE_OFF();
		}
	}
	else 
	{
		LED_ALL_MODE_OFF();
	}
}

//蓝牙从机控制
bool SlaveBluetoothControl(void)
{
	uint16_t Msg = 0;
	SW_TIMER BlueInTimer;

	APP_DBG("Enter SlaveBluetoothControl...\n");
	
#ifndef FUNC_MIXER_SRC_EN
	AudioSampleRateSet(44100);
#endif	
	
	SoundRemind(SOUND_BT_MODE);
	InitTimer(&BlueInTimer, BLUE_LED_CB_TIME, BlueLedCb);
	StartTimer(&BlueInTimer); 
	
	if(gSys.MicEnable)
	{
		MixerMute(MIXER_SOURCE_ANA_MONO);	
		MixerMute(MIXER_SOURCE_MIC);
	}
	else
	{
		MixerMute(MIXER_SOURCE_ANA_STERO);
	}
	
	WaitMs(20);
	AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC_FM : AUDIO_CH_FM);
	
	if(CurSlaveBtStatus != 7)
	{
		CurSlaveBtStatus = FALSE;
	}
	
	if(gSys.MicEnable)
	{
		MixerUnmute(MIXER_SOURCE_ANA_MONO);	
		MixerUnmute(MIXER_SOURCE_MIC);
	}
	else
	{
		MixerUnmute(MIXER_SOURCE_ANA_STERO);
	}
	
	// 消息注册
    if(gSys.MuteFlag)
	{
    	gSys.MuteFlag = 0;
		AudioPlayerMute(gSys.MuteFlag);
	} 

	AudioSysInfoSetBreakPoint();

	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	
	while(Msg != MSG_COMMON_CLOSE)
	{	
		CheckTimer(&BlueInTimer);
		
		if((CurSlaveBtStatus < 3) && !IsWiFiSoundRemindPlaying())
		{	
			gSys.MuteFlag = TRUE;
		}
		else
		{
			gSys.MuteFlag = FALSE;//SlaveBtMuteDetect();
		}
		
		Msg = MsgRecv(5);

		switch(Msg)
		{			
			case MSG_MODE:        //working mode changing
				Msg = MSG_COMMON_CLOSE;
				break;
			
			case MSG_POWER:
				gSys.NextModuleID = MODULE_ID_STANDBY;
				Msg = MSG_COMMON_CLOSE;
				break;
			
			case MSG_PLAY_PAUSE:
			  if(CurSlaveBtStatus == 5)
	      {
					BluetoothSoundFlag = FALSE;
				}
				SlaveBtPlaypause();
				break;
			
      case MSG_NEXT:
				SlaveBtNextSong();
				break;
			
			case MSG_PRE:
				SlaveBtPrevSong();
				break;
			
			case MSG_STOP:
				SlaveBtEnterPairedAndReject();
			  WaitMs(200);
			  SlaveBtClearPaired();
				break;
			
			case MSG_REPEAT:
				//SlaveBtRedial();
				break;
			
			case MSG_WIFI_WPS:
				if(WiFiPowerOnInitStateGet() == TRUE)
				{
					WiFiWorkStateSet(WIFI_STATUS_STATION_DISCONNECTED);
					Mcu_SendCmdToWiFi(MCU_WIF_WPS, NULL);
				}
				break;
			
			case MSG_WIFI_FACTORY: //WiFi模组恢复出厂设置 
				if(WiFiPowerOnInitStateGet() == TRUE)
				{
					if(gWiFi.TestModeState)
					{
						Mcu_SendCmdToWiFi(MCU_FCRYPOW, NULL);
					}
					else
					{
						Mcu_SendCmdToWiFi(MCU_FACTORY, NULL);
					}
				}
				break;
			
			default:
				CommonMsgProccess(Msg);
				break;

		}
		
		if(BluetoothSoundFlag)
		{
			BluetoothSoundFlag = FALSE;
			SlaveBluetoothSoundRemind();
		}
	}
#if (defined(FUNC_BREAKPOINT_EN) && (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN)))
	{
		BP_SYS_INFO *pBpSysInfo;
		pBpSysInfo = (BP_SYS_INFO *)BP_GetInfo(BP_SYS_INFO_TYPE);
		BP_SET_ELEMENT(pBpSysInfo->Volume, gSys.Volume);
    BP_SET_ELEMENT(pBpSysInfo->Eq, gSys.Eq);
		BP_SaveInfo(BP_SAVE2NVM);
	}
#endif
	
	if(gSys.OtherModuleTalkOn && (CurSlaveBtStatus == 7))
	{
		SlaveBtPlaypause();
	}
			
	AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC : AUDIO_CH_NONE);	
	PrevSlaveBtStatus = CurSlaveBtStatus;
	APP_DBG("leave SlaveBluetoothControl...\n");
	return TRUE;//OK
}
#endif

