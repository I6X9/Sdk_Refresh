/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : power_key.c
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2018��9��13��
  ����޸�   :
  ��������   : GPIO��Դ���ƹ���
  �����б�   :
              PowerKeyScan
              SysPowerOnControl
              SystemGotoDeepSleepPowerDown
              SystemPowerControlInit
              SystemPowerOnDetect
  �޸���ʷ   :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#include "app_config.h"

#ifdef FUNC_GPIO_POWER_CONTROL_EN

#define 	POWER_KEY_JITTER_TIME		20			//����ʱ�䣬��ʱ������ؿ��ػ�Ӳ��ʱ���й�
#define 	POWER_KEY_CP_TIME			1000		
#define 	POWER_KEY_PWRON_TIME		GPIO_POWER_DETECT_TIME	


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
 �� �� ��  : SystemGotoDeepSleepPowerDown
 ��������  : ����˯��ģʽ�Ļ���Դ��������deepsleepģʽ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void SystemGotoDeepSleepPowerDown(void)
{
    APP_DBG("system will enter deep sleep mode!!!\n");
    
    GPIO_PullHigh_Init(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT);
	if(GPIO_ON_OFF_PORT_OUT == GPIOA0)
	{
		GPIOGRP->IGRP = GPIOGRP->IGRP & (~(0x00000003 << (GPIO_ON_OFF_PORT_BIT*2)));
		GPIOA0_EXI_Init(GPIO_ON_OFF_PORT_BIT); 
	}
	else if(GPIO_ON_OFF_PORT_OUT == GPIOB0)
	{
		GPIOGRP->IGRP = GPIOGRP->IGRP | (0x00000001 << (GPIO_ON_OFF_PORT_BIT*2));
		GPIOB0_EXI_Init(GPIO_ON_OFF_PORT_BIT); 
	}
	else if(GPIO_ON_OFF_PORT_OUT == GPIOC0)
	{
		GPIOGRP->IGRP = GPIOGRP->IGRP | (0x00000002 << (GPIO_ON_OFF_PORT_BIT*2));
		GPIOC0_EXI_Init(GPIO_ON_OFF_PORT_BIT); 
	}
	EXTI_trigger_CMD(ENABLE,(CSP_REGISTER_T)(0X01ul<<GPIO_ON_OFF_PORT_BIT),_EXIFT); 
	EXTI_interrupt_CMD(ENABLE,(CSP_REGISTER_T)(0X01ul<<GPIO_ON_OFF_PORT_BIT));
	GPIO_EXTI_interrupt(GPIO_ON_OFF_PORT_OUT,(CSP_REGISTER_T)(0X01ul<<GPIO_ON_OFF_PORT_BIT));

	if(GPIO_ON_OFF_PORT_BIT == 0)
	{
    	EXI0_Int_Enable();                                          //EXI0 INT Vector
        EXI0_WakeUp_Enable();
    }
    else if(GPIO_ON_OFF_PORT_BIT == 1)
    {
    	EXI1_Int_Enable();                                          //EXI1 INT Vector
        EXI1_WakeUp_Enable();
    }
    else if((GPIO_ON_OFF_PORT_BIT >= 2) && (GPIO_ON_OFF_PORT_BIT <= 3))
    {
    	EXI2_Int_Enable();                                          //EXI2~EXI3 INT Vector
        EXI2_WakeUp_Enable();
    }
    else if((GPIO_ON_OFF_PORT_BIT >= 4) && (GPIO_ON_OFF_PORT_BIT <= 8))
    {
    	EXI3_Int_Enable();                                          //EXI4~EXI8 INT Vector
        EXI3_WakeUp_Enable();
    }
    else if((GPIO_ON_OFF_PORT_BIT >= 9) && (GPIO_ON_OFF_PORT_BIT <= 13))
    {
    	EXI4_Int_Enable();                                          //EXI9~EXI13 INT Vector
        EXI4_WakeUp_Enable();
    } 

    while(GetDebugDataLen());
    /* ����˯��ģʽ�Ĺ��ģ�������ϵͳ */
    //SYSCON_PWRCR_CONFIG(POWER_DRIVE_IDLE_LOW);
    PCLK_goto_deepsleep_mode();
    APP_DBG("From deep sleep mode WakeUp!!!\n");
    /* ��˯��ģʽ���ѣ�ִ�������λ */
    SYSCON_Soft_Reset();
}
#endif
//end #ifdef FUNC_GPIO_ON_OFF_EN

#ifdef FUNC_GPIO_POWERON_EN
/*****************************************************************************
 �� �� ��  : SysPowerOnControl
 ��������  : GPIO����ϵͳ���µ�Դ
 �������  : bool Flag  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void SysPowerOnControl(B_T Flag)
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
    TIMER PowerOnWiatTime;

    TimeOutSet(&PowerOnWiatTime, GPIO_POWER_DETECT_TIME);

    while(!IsTimeOut(&PowerOnWiatTime))
    {
#ifdef FUNC_GPIO_ON_OFF_EN
        if(GPIO_Read_Status(GPIO_ON_OFF_PORT_OUT,GPIO_ON_OFF_PORT_BIT))
        {
            SystemGotoDeepSleepPowerDown();
        }
#endif
    }
    
    APP_DBG("System power on!!!!\n");
}

/*****************************************************************************
 �� �� ��  : SystemPowerControlInit
 ��������  : ϵͳ��Դ���Ƴ�ʼ����
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
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

