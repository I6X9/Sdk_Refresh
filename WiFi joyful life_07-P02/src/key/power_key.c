////////////////////////////////////////////////////////////////////////////////
//                   Mountain View Silicon Tech. Inc.
//		Copyright 2014, Mountain View Silicon Tech. Inc., ShangHai, China
//                   All rights reserved.
//
//		Filename	:power_key_driver.c
//
//		Description	:
//					power keyboard driver(soft push botton)
//
//             maintainer: lujiangang
//
//		Changelog	:
//					2014-09-25	Created Source File
///////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "adc.h"
#include "app_config.h"
#include "os.h"
#include "gpio.h"
#include "irqs.h"
#include "msgq.h"
#include "app_msg.h"
#include "wakeup.h"
#include "bt_app_func.h"
#include "bt_control_api.h"
#include "sys_app.h"

#define 	POWER_KEY_JITTER_TIME		20			//����ʱ�䣬��ʱ������ؿ��ػ�Ӳ��ʱ���й�
#define 	POWER_KEY_CP_TIME			 1000		
#define 	POWER_KEY_PWRON_TIME			4000	


//extern bool PowerkeyGetOnkeyReg(void);
extern bool power_on_reconnection_flag;


typedef enum _POWER_KEY_STATE
{
	POWER_KEY_STATE_IDLE,
	POWER_KEY_STATE_JITTER,
	POWER_KEY_STATE_PRESS_DOWN,
	POWER_KEY_STATE_CP,

} POWER_KEY_STATE;

bool PowerKeyInitFlag = TRUE;
TIMER			PowerKeyWaitTimer;
POWER_KEY_STATE	PowerKeyState;
										
static const uint16_t PowerKeyEvent[2] = {MSG_NONE/*������ʼ*/, MSG_WIFI_TALK,/*�̰��ɿ�*/};	


#if (defined(FUNC_POWER_KEY_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))

	#warning	"MV: PUSH BUTTON POWER KEY ENABLE!"

// Initialize POWER_KEY scan operation.
void PowerKeyScanInit(void)
{
	DBG("PowerKeyScanInit*******\n");
	PowerKeyState = POWER_KEY_STATE_IDLE;
}


// POWER_KEY����ͨ�İ�����ͬ�����Ӱ�ť���أ����أ�ʱ����Ҫ���û���ϵͳ���ػ�����Ȼ��Ҳ�����ö̰����ܡ�
// �̰�����ʱ�����Ͷ̰���Ϣ�������̰����䣬�˴������κδ�����ϵͳӲ�����ػ���
uint16_t PowerKeyScan(void)							
{
	if(PowerKeyInitFlag)  
	//��ֹ����ʱ����POWER�������̰���Ϣ������ϵͳ������ֻ�м�⵽�����ɿ��󣬲ŻῪʼ���̰���Ϣ
	{
		if(PowerkeyGetOnkeyReg())
		{
			return MSG_NONE;
		}
		else if(!PowerkeyGetOnkeyReg())
		{
			PowerKeyInitFlag = FALSE;
		}
	}
	switch(PowerKeyState)
	{
		case POWER_KEY_STATE_IDLE:
			if(!PowerkeyGetOnkeyReg())
			{
				return MSG_NONE;
			}
			else
			{	
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_JITTER_TIME);
				PowerKeyState = POWER_KEY_STATE_JITTER;
			}
			break;
		case POWER_KEY_STATE_JITTER:
			if(!PowerkeyGetOnkeyReg())
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_PRESS_DOWN;
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CP_TIME);
				return PowerKeyEvent[0];			//return key sp value
			}
			break;
			
		case POWER_KEY_STATE_PRESS_DOWN:
			if(!PowerkeyGetOnkeyReg() && !IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
				return PowerKeyEvent[1];
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_CP;
				return PowerKeyEvent[0];
			}
			break;
			
		case POWER_KEY_STATE_CP:
			//�˴�����֤һ�ΰ���������Ӧ��ζ̰�
			if(!PowerkeyGetOnkeyReg())
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
				return PowerKeyEvent[0];//return MSG_NONE;				
			}
			else
			{
				//do no thing
			}
			break;
			
		default:
			PowerKeyState = POWER_KEY_STATE_IDLE;
			break;
	}
	return MSG_NONE;
}

#ifdef FUNC_GPIO_POWER_ON_EN 
void SysPowerOnControl(bool Flag)
{
	DBG("GpioControlSysPower:%d*******\n", Flag);
	if(Flag)
	{
		GpioClrRegOneBit(POWERON_GPIO_PORT_PU, POWERON_GPIO_PORT_BIT);
	  GpioClrRegOneBit(POWERON_GPIO_PORT_PD, POWERON_GPIO_PORT_BIT);
	  GpioClrRegOneBit(POWERON_GPIO_PORT_IE, POWERON_GPIO_PORT_BIT);
	  GpioSetRegOneBit(POWERON_GPIO_PORT_OE, POWERON_GPIO_PORT_BIT);
		GpioSetRegOneBit(POWERON_GPIO_PORT_OUTDS, POWERON_GPIO_PORT_BIT);
	  GpioSetRegOneBit(POWERON_GPIO_PORT_OUT, POWERON_GPIO_PORT_BIT);
	}
	else
	{
		GpioSetRegOneBit(POWERON_GPIO_PORT_PU, POWERON_GPIO_PORT_BIT);
	  GpioSetRegOneBit(POWERON_GPIO_PORT_PD, POWERON_GPIO_PORT_BIT);
	  GpioClrRegOneBit(POWERON_GPIO_PORT_IE, POWERON_GPIO_PORT_BIT);
	  GpioSetRegOneBit(POWERON_GPIO_PORT_OE, POWERON_GPIO_PORT_BIT);
	  GpioClrRegOneBit(POWERON_GPIO_PORT_OUT, POWERON_GPIO_PORT_BIT);
	}
}
#endif
#endif
//end #if (defined(FUNC_POWER_KEY_EN) && defined(USE_POWERKEY_SOFT_PUSH_BUTTON))
