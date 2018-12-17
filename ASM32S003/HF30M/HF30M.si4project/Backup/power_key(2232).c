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
#include "app_config.h"

#ifdef FUNC_GPIO_POWER_CONTROL_EN

#define 	POWER_KEY_JITTER_TIME		20			//����ʱ�䣬��ʱ������ؿ��ػ�Ӳ��ʱ���й�
#define 	POWER_KEY_CP_TIME			1000		
#define 	POWER_KEY_PWRON_TIME		3000	


typedef enum _POWER_KEY_STATE
{
	POWER_KEY_STATE_IDLE,
	POWER_KEY_STATE_JITTER,
	POWER_KEY_STATE_PRESS_DOWN,
	POWER_KEY_STATE_CP,

} POWER_KEY_STATE;

B_T PowerKeyInitFlag = TRUE;
TIMER			PowerKeyWaitTimer;
POWER_KEY_STATE	PowerKeyState;
										

#ifdef FUNC_GPIO_ON_OFF_EN
#ifdef FUNC_POWER_KEY_EN
    #warning    "MV: PUSH BUTTON POWER KEY ENABLE!"
#endif /* FUNC_POWER_KEY_EN */

static const uint16_t PowerKeyEvent[2] = {MSG_NONE/*������ʼ*/, MSG_MODE/*�̰��ɿ�*/};

// POWER_KEY����ͨ�İ�����ͬ�����Ӱ�ť���أ����أ�ʱ����Ҫ���û���ϵͳ���ػ�����Ȼ��Ҳ�����ö̰����ܡ�
// �̰�����ʱ�����Ͷ̰���Ϣ�������̰����䣬�˴������κδ�����ϵͳӲ�����ػ���
uint16_t PowerKeyScan(void)							
{
	if(PowerKeyInitFlag)  
	//��ֹ����ʱ����POWER�������̰���Ϣ������ϵͳ������ֻ�м�⵽�����ɿ��󣬲ŻῪʼ���̰���Ϣ
	{
		if(!GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
		{
			return MSG_NONE;
		}
		else if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
		{
			PowerKeyInitFlag = FALSE;
		}
	}
    
	switch(PowerKeyState)
	{
		case POWER_KEY_STATE_IDLE:
			if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
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
			if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
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
			if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
#ifdef FUNC_POWER_KEY_EN
                /* �����̰���Ϣ */
				return PowerKeyEvent[1];
#else
				return MSG_NONE;
#endif
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_CP;
                TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_PWRON_TIME);
				return PowerKeyEvent[0];
			}
			break;
			
		case POWER_KEY_STATE_CP:
			//�˴�����֤һ�ΰ���������Ӧ��ζ̰�
			if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
				return PowerKeyEvent[0];//return MSG_NONE;				
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				/* ��⵽�ػ�����,ϵͳ����ػ������᷵��*/
                APP_DBG("Detect power off key,System will enter power down!!!!\n");
                SystemGotoDeepSleepPowerDown();
			}
			break;
			
		default:
			PowerKeyState = POWER_KEY_STATE_IDLE;
			break;
	}
	return MSG_NONE;
}

/*****************************************************************************
 �� �� ��  : SystemPowerOnDetect
 ��������  : ϵͳ����������⣬������㿪��������ϵͳ��������ϵͳ����˯��ģʽ��
            ���᷵��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��12��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void SystemPowerOnDetect(void)
{
    
}
#endif
//end #ifdef FUNC_GPIO_ON_OFF_EN

#ifdef FUNC_GPIO_POWERON_EN 
void SysPowerOnControl(bool Flag)
{
	APP_DBG("GpioControlSysPower:%d*******\n", Flag);
	if(Flag)
	{
        GPIO_Write_High(GPIO_POWERON_PORT_OUT,GPIO_POWERON_PORT_BIT);
	}
	else
	{
        GPIO_Write_Low(GPIO_POWERON_PORT_OUT,GPIO_POWERON_PORT_BIT);
	}
}
#endif

// Initialize POWER_KEY scan operation.
void SystemPowerControlInit(void)
{
	APP_DBG("System power control Init*******\n");
    
#ifdef FUNC_GPIO_ON_OFF_EN
    PowerKeyState = POWER_KEY_STATE_IDLE;
	GPIO_PullHigh_Init(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT);
	GPIO_Init(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT,Intput);       
#endif
#ifdef FUNC_GPIO_POWERON_EN
	GPIO_PullLow_Init(GPIO_POWERON_PORT_OUT,GPIO_POWERON_PORT_BIT);
	GPIO_Init(GPIO_POWERON_PORT_OUT,GPIO_POWERON_PORT_BIT,Output); 
    GPIO_Write_Low(GPIO_POWERON_PORT_OUT,GPIO_POWERON_PORT_BIT);
#endif /* FUNC_GPIO_POWERON_EN */
}
#endif //FUNC_GPIO_POWER_CONTROL_EN

