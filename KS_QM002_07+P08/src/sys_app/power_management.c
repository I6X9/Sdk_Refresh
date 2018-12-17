///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: power_management.c
//  maintainer: lujiangang
///////////////////////////////////////////////////////////////////////////////

#include "type.h"
#include "app_config.h"
#include "uart.h"
#include "adc.h"
#include "clk.h"
#include "gpio.h"
#include "wakeup.h"
#include "sys_app.h"
#include "breakpoint.h"

#ifdef FUNC_POWER_MONITOR_EN

#define LDOIN_SAMPLE_COUNT			10		//��ȡLDOIN����ʱ����ƽ���Ĳ�������
#define LDOIN_SAMPLE_PERIOD			50		//��ȡLDOIN����ʱ��ȡ����ֵ�ļ��(ms)
#define LOW_POWEROFF_DET_TMR		600		//�͵���ػ��������ʱ��(��λs)
#define LOW_POWER_SOUND_CNT 		300		//�͵籨����ʾ�����ʱ��(��λs)

//��ȡLDOIN����ʱ��ȡ����ֵ����ʱ����(ms)
#define LDOIN_SAMPLE_TIMER			(LDOIN_SAMPLE_COUNT*LDOIN_SAMPLE_PERIOD)
//�͵�����ʱʱ�������������������ʾ�����š�
#define LOW_POWEROFF_COUNT			5000/LDOIN_SAMPLE_TIMER

//���¶��岻ͬ�ĵ�ѹ����¼��Ĵ�����ѹ(��λmV)���û���������ϵͳ��ص��ص�������
#define LDOIN_VOLTAGE_FULL			4200
#define LDOIN_VOLTAGE_HIGH			3800
#define LDOIN_VOLTAGE_MID			3600
#define LDOIN_VOLTAGE_LOW			3500
#define LDOIN_VOLTAGE_OFF			3400	//���ڴ˵�ѹֵ����ػ�powerdown״̬

//��ѹ���ʱ��ͬ����ʾ����
typedef enum _PWR_MNT_DISP
{
	PWR_MNT_DISP_NONE = 0,
	PWR_MNT_DISP_CHARGE,		 
	PWR_MNT_DISP_HIGH_V, 
	PWR_MNT_DISP_MID_V, 
	PWR_MNT_DISP_LOW_V, 
	PWR_MNT_DISP_EMPTY_V, 
	PWR_MNT_DISP_SYS_OFF
	 
} PWR_MNT_DISP;

//��Ҫ���ڵ�ѹ״̬��ʾ�ı���
static TIMER 		BlinkTimer;
static uint16_t     LowPowerDetCnt = ((LOW_POWEROFF_DET_TMR*1000)/LDOIN_SAMPLE_TIMER)+(LOW_POWEROFF_COUNT*2);
#ifdef	OPTION_CHARGER_DETECT
static PWR_MNT_DISP PwrMntDisp;
#endif


//���ڵ�ѹ���ı���
TIMER PowerMonitorTimer;
uint32_t LdoinSampleSum = 0; 
uint16_t  LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
uint32_t LdoinLevelAverage = 0;		//��ǰLDOIN��ѹƽ��ֵ


#ifdef	OPTION_CHARGER_DETECT
//Ӳ�����PC ����������״̬
//ʹ���ڲ���������PC����������ʱ������Ϊ�͵�ƽ����ʱ����Ϊ�ߵ�ƽ
bool IsInCharge(void)
{
//��Ϊ���룬��������

	GpioSetRegBits(CHARGE_DETECT_PORT_PU, CHARGE_DETECT_BIT);
	GpioSetRegBits(CHARGE_DETECT_PORT_PD, CHARGE_DETECT_BIT);
	GpioClrRegBits(CHARGE_DETECT_PORT_OE, CHARGE_DETECT_BIT);	   
	GpioSetRegBits(CHARGE_DETECT_PORT_IE, CHARGE_DETECT_BIT);
	WaitUs(2);
	if(GpioGetReg(CHARGE_DETECT_PORT_IN) & CHARGE_DETECT_BIT)
	{
		return TRUE;
	}   	

	return FALSE;
}
#endif
void PowerMonitorDisp(void)
{
#if (defined(FUNC_SEG_LED_EN) || defined(FUNC_SEG_LCD_EN) ||defined(FUNC_TM1628_LED_EN))
	static uint8_t  ShowStep = 0;
	static bool IsToShow = FALSE;

	switch(PwrMntDisp)
	{			
		case PWR_MNT_DISP_CHARGE:
			//��˸���ICON,��ʾ���ڳ��
			if(IsTimeOut(&BlinkTimer))
			{
				TimeOutSet(&BlinkTimer, 500);
				switch(ShowStep)
				{

					case 0:
						DispIcon(ICON_BAT1, FALSE);
						DispIcon(ICON_BAT2, FALSE);
						DispIcon(ICON_BAT3, FALSE);
						break;
					case 1:
						DispIcon(ICON_BAT1, TRUE);
						DispIcon(ICON_BAT2, FALSE);
						DispIcon(ICON_BAT3, FALSE);
						break;
					case 2:
						DispIcon(ICON_BAT1, TRUE);
						DispIcon(ICON_BAT2, TRUE);
						DispIcon(ICON_BAT3, FALSE);
						break;
					case 3:
						DispIcon(ICON_BAT1, TRUE);
						DispIcon(ICON_BAT2, TRUE);
						DispIcon(ICON_BAT3, TRUE);
						break;
				}		
				if(ShowStep < 3)
				{
					ShowStep++;
				}
				else
				{
					ShowStep = 0;
				}
			}
			
			break;

		case PWR_MNT_DISP_HIGH_V:
			//DBG("BAT FULL\n");			
			DispIcon(ICON_BATFUL, TRUE);
			DispIcon(ICON_BATHAF, FALSE);
			//������ʾ����������������ʾ����
			break;
			
		case PWR_MNT_DISP_MID_V:
			//DBG("BAT HALF\n");		
			DispIcon(ICON_BATFUL, FALSE);
			DispIcon(ICON_BATHAF, TRUE);
			//������ʾ2��������������ʾ����
			break;

		case PWR_MNT_DISP_LOW_V:
			DispIcon(ICON_BATFUL, FALSE);
			DispIcon(ICON_BATHAF, TRUE);
			//������ʾ1��������������ʾ����
			break;
			
		case PWR_MNT_DISP_EMPTY_V:
			//DBG("BAT EMPTY\n");				
			DispIcon(ICON_BATFUL, FALSE);

			if(IsTimeOut(&BlinkTimer))
			{
				TimeOutSet(&BlinkTimer, 300);
				if(IsToShow)
				{
					DispIcon(ICON_BATHAF, TRUE);
				}
				else
				{
					DispIcon(ICON_BATHAF, FALSE);
				}
				IsToShow = !IsToShow;
			}
			//������ʾ0��������������ʾ����
			break;
		
		case PWR_MNT_DISP_SYS_OFF:
			//DBG("BAT OFF\n");
			//ClearScreen();			//�����ʾ				
			//DispString(" LO ");
			break;
			
		default:
			break;
	}
#endif	
}
//���LDOIN�ĵ�ѹֵ��ִ�ж�Ӧ����Ĵ���
//PowerOnInitFlag: TRUE--��һ���ϵ�ִ�е�Դ��ؼ��
static void PowerLdoinLevelMonitor(bool PowerOnInitFlag)
{	
	//bool PowerOffFlag = FALSE;
	//uint16_t AdcVal;	
	if(LdoinSampleCnt > 0)
	{
		LdoinSampleSum += SarAdcGetLdoinVoltage();		
		LdoinSampleCnt--;
	}

	//������LDOIN_SAMPLE_COUNT��������LDOINƽ��ֵ
	if(LdoinSampleCnt == 0)
	{
		LdoinLevelAverage = LdoinSampleSum / LDOIN_SAMPLE_COUNT;

//		DBG("LDOin 5V Volt: %lu\n", (uint32_t)LdoinLevelAverage);

		//Ϊ�������LDOIN����ʼ������
		LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
		LdoinSampleSum = 0;

#ifdef	OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{		
			PowerMonitorDisp();
			return;
		}

		
		if(LdoinLevelAverage > LDOIN_VOLTAGE_HIGH)	  
		{
			//������ʾ�������������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_HIGH_V);
			PwrMntDisp = PWR_MNT_DISP_HIGH_V;
			//DBG("bat full\n");
		}

		else if(LdoinLevelAverage > LDOIN_VOLTAGE_MID)
		{
			//������ʾ2�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_MID_V);
			PwrMntDisp = PWR_MNT_DISP_MID_V;
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_LOW)
		{
			//������ʾ1�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_LOW_V);
			PwrMntDisp = PWR_MNT_DISP_LOW_V;
			//DBG("bat LOW\n");
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_OFF)
		{
			//������ʾ0�����������PowerMonitorDisp�������ʾ����
			//PowerMonitorDisp(PWR_MNT_DISP_EMPTY_V);
			PwrMntDisp = PWR_MNT_DISP_EMPTY_V;
		}
		else if(LdoinLevelAverage <= LDOIN_VOLTAGE_OFF)
		{
			//���ڹػ���ѹ������ػ�����
			//����������ʾ�͵�ѹ��ʾ��Ȼ��ִ�йػ�����
			PwrMntDisp = PWR_MNT_DISP_SYS_OFF;
		}
#endif
		

		if(LdoinLevelAverage <= LDOIN_VOLTAGE_OFF)
		{
			//ֹͣ�����������̣���������ʾ����DAC���ع��ŵ�Դ�ȶ���
			DBG("PowerMonitor, Low Voltage detect LowPowerDetCnt = %d;\n", LowPowerDetCnt);	

			//low level voltage detect in power on sequence, power down system directly
			if(PowerOnInitFlag == TRUE)
			{
				//��ʾ�͵磬Ȼ��ػ���
				LowPowerDetCnt = LOW_POWEROFF_COUNT+1;
				//SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
				//SysGotoPowerDown(); 
			}

			LowPowerDetCnt--;
			if(LowPowerDetCnt%(LOW_POWER_SOUND_CNT*1000/LDOIN_SAMPLE_TIMER) == LOW_POWEROFF_COUNT)
			{
				APP_DBG("PowerMonitor, Low Voltage sound remind!!!!");
				MsgSend(MSG_BAT_LOW_PWR);
			}
			else if(LowPowerDetCnt == FALSE)	// PowerOnInitFlag == FALSE
			{
				if(gSys.NextModuleID < MODULE_ID_IDLE)	/*MSG_COMMON_CLOSE only need send once*/
				{
					/*if use push button, send message, for task's saving info.*/
					APP_DBG("Low level voltage detected->send message common close\n");
					//gSys.NextModuleID = MODULE_ID_POWEROFF;
					gSys.NextModuleID = MODULE_ID_IDLE;
					MsgSend(MSG_COMMON_CLOSE);
				}
			}
		}	
		else if(LdoinLevelAverage >= LDOIN_VOLTAGE_MID)
		{
			LowPowerDetCnt = ((LOW_POWEROFF_DET_TMR*1000)/LDOIN_SAMPLE_TIMER)+(LOW_POWEROFF_COUNT*2);
		}
	}
	PowerMonitorDisp();
}


//���ܼ��ӳ�ʼ��
//ʵ��ϵͳ���������еĵ͵�ѹ��⴦���Լ���ʼ������豸������IO��
void PowerMonitorInit(void)
{
	TimeOutSet(&PowerMonitorTimer, 0);	
	TimeOutSet(&BlinkTimer, 0);	
#ifdef OPTION_CHARGER_DETECT
	//���ϵͳ����ʱ������豸�Ѿ����룬�Ͳ�ִ��������ε͵�ѹ���ʹ������
	if(!IsInCharge())
#endif
	{
#ifdef POWERON_DETECT_VOLTAGE	// ��������ѹ
		//ϵͳ���������еĵ͵�ѹ���
		//����ʱ��ѹ��⣬���С�ڿ�����ѹ���������豸���Ͳ������̣�ֱ�ӹػ�
		//������Ϊʱ50ms�������ж�Ӧ�Ĵ���
		while(LdoinSampleCnt)
		{
			LdoinSampleCnt--;
			LdoinSampleSum += SarAdcGetLdoinVoltage();
			WaitMs(5);
		}		
		//Ϊ��ߵ�LDOIN����ʼ������
		PowerLdoinLevelMonitor(TRUE);
#endif
	}
}

//ϵͳ��Դ״̬��غʹ���
//ϵͳ���������LDOIN���ڿ�����ѹ������ϵͳ���������м��LDOIN
void PowerMonitor(void)
{
	if(IsTimeOut(&PowerMonitorTimer))
	{
		TimeOutSet(&PowerMonitorTimer, LDOIN_SAMPLE_PERIOD);

#ifdef OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{
			if(LdoinLevelAverage >= LDOIN_VOLTAGE_FULL) 
			{
				//PowerMonitorDisp(PWR_MNT_DISP_HIGH_V);		//��ʾ���״̬
				PwrMntDisp = PWR_MNT_DISP_HIGH_V;
				//DBG("charger full\n");
			}
			else
			{
				//PowerMonitorDisp(PWR_MNT_DISP_CHARGE);		//��ʾ���״̬
				PwrMntDisp = PWR_MNT_DISP_CHARGE;
				//DBG("charger.....\n");
			}
		}
#endif		
		//û�в�����LDOIN_SAMPLE_COUNT��������������
		PowerLdoinLevelMonitor(FALSE);
	}
}
#ifdef LOW_POWER_SAVE_PLAYTIME_TO_FLASH
extern uint32_t AudioPlaySavePlayTimeToFlash(void);

void LowPowerDetProc(void)
{
	uint16_t RetVal;

	RetVal = SarAdcGetLdoinVoltage();

	if((RetVal < 4200) && (RetVal > 3400))	
   	{
	   	AudioPlaySavePlayTimeToFlash();
   	}
}
#endif
#endif	//end of FUNC_POWER_MONITOR_EN

/**
 * @brief 		system power off detect function
 *			this function should be called in timer INT function or systick function for periodically detecting
 *			when USE_POWERKEY_SLIDE_SWITCH, the system will be powered off directly
 *			when USE_POWERKEY_SOFT_PUSH_BUTTON, it wil send message:MSG_COMMON_CLOSE, for task's saving information, 
* 			and then power off system
 * @param	None
 * @return	None
 */
void SystemPowerOffDetect(void)
{

#ifdef USE_POWERKEY_SLIDE_SWITCH
#define SLIDE_SWITCH_ONTIME 500
	static uint16_t  slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;//����ʱ��0.5s����PowerKeyDetect()����
	if(PowerKeyDetect())
	{
		if(slide_switch_pd_cnt-- == 0)
		{
			/*if slide switch, power down system directly*/
			APP_DBG("PowerKeyDetect->go to PowerDown\n");
			SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
			SysGotoPowerDown(); 
			while(1);
		}
	}
	else
	{
		slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;
	}
#endif 
	
#ifdef USE_POWERKEY_SOFT_PUSH_BUTTON
#ifdef FUNC_POWERKEY_SOFT_POWERON_EN
	static uint16_t  powerkey_switch_pd_cnt = 500;//����ʱ��0.5s
	
	if(PowerkeyGetOnkeyReg())
	{
		if(powerkey_switch_pd_cnt-- == FALSE) /*MSG_COMMON_CLOSE only need send once*/
		{
			if(gSys.PowerOnInitFlag == FALSE)
			{
				APP_DBG("PowerKeyDetect->send message common poweron\n");
				gSys.PowerOnInitFlag = TRUE;
			#ifdef FUNC_BREAKPOINT_EN
                {
                    BP_SYS_INFO *pBpSysInfo;
                    pBpSysInfo = (BP_SYS_INFO *)BP_GetInfo(BP_SYS_INFO_TYPE);
                    gSys.NextModuleID = BP_GET_ELEMENT(pBpSysInfo->CurModuleId);
                    if(gSys.NextModuleID >= MODULE_ID_IDLE)
                    {
                        gSys.NextModuleID = MODULE_ID_BLUETOOTH;
                    }
                }
			#else
				gSys.NextModuleID = MODULE_ID_BLUETOOTH;
			#endif
				MsgSend(MSG_COMMON_CLOSE);
			}
			else
			{
				/*if use push button, send message, for task's saving info.*/
				APP_DBG("PowerKeyDetect->send message common close\n");
				//��Idle control�����Դ�ϵ��־��
				//gSys.PowerOnInitFlag = FALSE;
				gSys.NextModuleID = MODULE_ID_IDLE;
				MsgSend(MSG_COMMON_CLOSE);
			}
		}
	}
	else
	{
		powerkey_switch_pd_cnt = 2000;
	}

#else
	if(PowerKeyDetect())
	{
		//if(gSys.NextModuleID != MODULE_ID_POWEROFF) 	/*MSG_COMMON_CLOSE only need send once*/
		if((gSys.NextModuleID != MODULE_ID_IDLE) && (gSys.PowerOnInitFlag == TRUE) && (gSys.CurModuleID != MODULE_ID_IDLE))
		{
			/*if use push button, send message, for task's saving info.*/
			APP_DBG("PowerKeyDetect->send message common close\n");
			//gSys.NextModuleID = MODULE_ID_POWEROFF;
			gSys.NextModuleID = MODULE_ID_IDLE;
			MsgSend(MSG_COMMON_CLOSE);
		}
	}
#endif
#endif	
	
}

// ����deep sleep��Ϊ�˽��͹��ģ�Ĭ������Ϊ����������
// �ͻ����Ը�����Ҫ����һЩ���ƻ�����
void SysGotoDeepSleepGpioCfg(void)
{
	GpioSetRegBits(GPIO_A_IE, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_A_OE, 0xFFFFFFFF);
	GpioSetRegBits(GPIO_A_PU, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_A_PD, 0xFFFFFFFF);

	GpioSetRegBits(GPIO_B_IE, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_B_OE, 0xFFFFFFFF);
	GpioSetRegBits(GPIO_B_PU, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_B_PD, 0xFFFFFFFF);

	GpioSetRegBits(GPIO_C_IE, 0x7FFF);
	GpioClrRegBits(GPIO_C_OE, 0x7FFF);
	GpioSetRegBits(GPIO_C_PU, 0x7FFF);
	GpioClrRegBits(GPIO_C_PD, 0x7FFF);

	// BT close, not arbitrarily modify
	BTDevicePowerOff();
}


/**
 * @brief 		system power off flow
 *			user can add other functions before SysGotoPowerDown()
 *			
 * @param	None
 * @return	None
 */
void SystemPowerOffControl(void)
{
	APP_DBG("SystemPowerOffControl->system will power off soon!\n");
	
	SysVarDeinit();
	//APP_DBG("REG_LP_WAKEUP = %d, s_onoff = %d\n", (*(volatile unsigned long*)0x4002200C), PowerkeyGetSOnOff());
	//SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
	SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_RTC);
	SysGotoPowerDown(); 

	/*Never reach here,  expect power down fail*/
	APP_DBG("System Power Down Fail!");
	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	while(1);
}

/**
 * @brief 		system standby flow
 *			user can add other functions before SysGotoDeepSleep()
 *			
 * @param	None
 * @return	None
 */
void SystemStandbyControl(void)
{
	uint32_t Port = GPIOC7;
	APP_DBG("StandBy Mode\n");
	SysVarDeinit();
	
	SysGotoDeepSleepGpioCfg();

	//set GPIOB24 input enable, pull down
	GpioSetRegOneBit(GPIO_C_IE, Port);
	GpioSetRegOneBit(GPIO_C_PU, Port);
	GpioSetRegOneBit(GPIO_C_PD, Port);  

	//set wakeup source GPIOB24, Wakeup Polarith: High wakeup, pin wakeup valid minimal time is 1ms
	SysSetWakeUpSrcInDeepSleep(WAKEUP_SRC_SLEEP_C7, WAKEUP_POLAR_C7_HI, WAKEUP_TMODE_1MS);
	SysGotoDeepSleep();

	/*Never reach here,  expect go to deepsleep fail*/
	APP_DBG("System Go to Deep Sleep Fail!");
	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	while(1);
}
