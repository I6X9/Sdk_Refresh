///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: idle_control.c
//  maintainer: Halley
///////////////////////////////////////////////////////////////////////////////
#include "app_config.h"
#include "idle_control.h"
#include "sys_app.h"
#include "sys_vol.h"
#include "gpio.h"
#include "wifi_control.h"
#include "sound_remind.h"
#include "bt_app_func.h"

TIMER ChargeLedTmr;

void SysPowerOnControl(bool Flag);

void IdleControl(void)
{
	uint16_t Msg = 0;

	APP_DBG("Enter Idle\n");

#ifndef FUNC_MIXER_SRC_EN
	AudioSampleRateSet(44100);
#endif

#ifdef FUNC_AMP_MUTE_EN
//	AmpMuteControl(TRUE);
#endif	
	
#ifdef FUNC_GPIO_POWER_ON_EN 
	SysPowerOnControl(FALSE);
#endif	
#ifdef FUNC_BT_EN
    //�ر�����
    BtHwPowerOff();
#endif /* FUNC_BT_EN */
#if (defined(FUNC_LIGHT_PWM_LED_EN) || defined(FUNC_ALEXA_PWM_LED_EN))
	//����ʾ
	PwmLedDisplayScanSet(FALSE);
	PwmDisplayLightOnOff();
#endif
#ifdef FUNC_WIFI_POWER_KEEP_ON
	WiFiControlGpioInit();	
	WiFiPowerOff();
#endif
	
	TimeOutSet(&ChargeLedTmr, 0);
	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	while(Msg != MSG_COMMON_CLOSE)
	{
		Msg = MsgRecv(20);// ��Ϣ���գ�����Ϣ����20ms������Ϣ��������
		
		if(IsTimeOut(&ChargeLedTmr))
		{
			static bool LedFlag = FALSE;
			TimeOutSet(&ChargeLedTmr, 500);
			LedFlag = !LedFlag;
		}
		
		switch(Msg)
		{
			case MSG_POWER:        //working mode changing
				gSys.NextModuleID = MODULE_ID_WIFI;
				Msg = MSG_COMMON_CLOSE;
				break;
			
			default:
				//CommonMsgProccess(Msg);
				break;
		}
	}
	
	APP_DBG("Exit Idle\n");
	//����ǰ�ж�����
#ifndef POWERON_DEFAULT_VOLUME_EN
	if(gSys.Volume > MAX_VOLUME || gSys.Volume < 7)
#endif
	{
		gSys.Volume = 7;
	}

#if (defined(FUNC_LIGHT_PWM_LED_EN) || defined(FUNC_ALEXA_PWM_LED_EN))
	//����ʾ
	PwmLedDisplayScanSet(TRUE);
#endif
#ifdef FUNC_WIFI_POWER_KEEP_ON
    WiFiControlGpioInit();	
	WaitMs(500);
	WiFiPowerOn();	
	SoundRemind(SOUND_PWR_ON);
#endif
#ifdef FUNC_BT_EN
    //������
    BtHwPowerOn();
#endif /* FUNC_BT_EN */	
}
