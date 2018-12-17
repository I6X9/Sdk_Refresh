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
#include "watchdog.h"
#include "bt_control_api.h"
#include "breakpoint.h"
#include "dev_detect_driver.h" 
#ifdef FUNC_WIFI_EN
#include "wifi_control.h"
#include "wifi_uart_com.h"
#include "wifi_mcu_command.h"
#endif
#ifdef FUNC_SINGLE_LED_EN
#include "single_led_disp.h"
#endif

#ifdef BT_APPLE_BATTERY_STATUS 
#define BT_BATTERY_LEVEL_CNT    9
static uint8_t BtBatteryLevel = BT_BATTERY_LEVEL_CNT;
TIMER BtBatteryDetTim;
#endif

#ifdef FUNC_SLEEP_EN
TIMER		PowerOffDelTmr;
#endif

extern void SysPowerOnControl(bool Flag);

#ifdef FUNC_POWER_MONITOR_EN

//#define POWER_MONITOR_DISP_EN
#define LDOIN_SAMPLE_COUNT			20		//��ȡLDOIN����ʱ����ƽ���Ĳ�������
#define LDOIN_SAMPLE_PERIOD			50		//��ȡLDOIN����ʱ��ȡ����ֵ�ļ��(ms)
#define POWER_LOW_OFF_COUNT			200		//�͵���ػ��������ʱ��(ms)
#define POWER_OFF_OFF_COUNT			10		//�͵���ػ��������ʱ��(ms)

//���¶��岻ͬ�ĵ�ѹ����¼��Ĵ�����ѹ(��λmV)���û���������ϵͳ��ص��ص�������
#ifdef ADC_POWER_MONITOR_EN 
   #define LDOIN_VOLTAGE_FULL			3100
   #define LDOIN_VOLTAGE_HIGHT    3000
   #define LDOIN_VOLTAGE_HIGH			2800
   #define LDOIN_VOLTAGE_MID			2600
   #define LDOIN_VOLTAGE_LOW			2500
   #define LDOIN_VOLTAGE_OFF			2400	//ע�⣺������ADCֵ��Vol=log/4095*3.41,��λV
#else
   #define LDOIN_VOLTAGE_FULL			4200
   #define LDOIN_VOLTAGE_HIGH			4000
   #define LDOIN_VOLTAGE_MID			3700
   #define LDOIN_VOLTAGE_LOW			3400
   #define LDOIN_VOLTAGE_OFF			3300	//���ڴ˵�ѹֵ����ػ�powerdown״̬
#endif

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
static TIMER BlinkTimer;
#ifdef POWER_MONITOR_DISP_EN
static PWR_MNT_DISP PwrMntDisp;
#endif

//���ڵ�ѹ���ı���
TIMER PowerMonitorTimer;
uint32_t LdoinSampleSum = 0; 
uint16_t  LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
uint32_t LdoinLevelAverage = 0;		//��ǰLDOIN��ѹƽ��ֵ

#ifdef FUNC_SOUND_REMIND
#include "sound_remind.h"
uint16_t PowLowSoundCnt = POWER_LOW_OFF_COUNT;
uint16_t PowOffPowerCnt = POWER_OFF_OFF_COUNT;
#endif

#ifdef	OPTION_CHARGER_DETECT
//Ӳ�����PC ����������״̬
//ʹ���ڲ���������PC����������ʱ������Ϊ�͵�ƽ����ʱ����Ϊ�ߵ�ƽ
bool IsInCharge(void)
{
//��Ϊ���룬��������

	GpioSetRegBits(CHARGE_DETECT_PORT_PU, CHARGE_DETECT_BIT);
	GpioClrRegBits(CHARGE_DETECT_PORT_PD, CHARGE_DETECT_BIT);
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
	#ifdef ADC_POWER_MONITOR_EN
		LdoinSampleSum += (SarAdcChannelGetValue(ADC_POWER_MONITOR_PORT)*3400/4095);
	#else
		LdoinSampleSum += SarAdcGetLdoinVoltage();		
	#endif		
		LdoinSampleCnt--;
	}

	//������LDOIN_SAMPLE_COUNT��������LDOINƽ��ֵ
	if(LdoinSampleCnt == 0)
	{
		LdoinLevelAverage = (LdoinSampleSum / LDOIN_SAMPLE_COUNT);

		//DBG("Battery Level Average: %lu\n", (uint32_t)LdoinLevelAverage);

		//Ϊ�������LDOIN����ʼ������
		LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
		LdoinSampleSum = 0;

#ifdef BT_APPLE_BATTERY_STATUS 
		{
			if(IsTimeOut(&BtBatteryDetTim) && GetCurBtConnectedFlag())
			{
				TimeOutSet(&BtBatteryDetTim, 5000);
				BtBatteryLevel = (LdoinLevelAverage - LDOIN_VOLTAGE_OFF)/100;
				if(BtBatteryLevel > BT_BATTERY_LEVEL_CNT)
				{
					BtBatteryLevel = BT_BATTERY_LEVEL_CNT;
				}
		    BTSetBatteryState(BtBatteryLevel, TRUE);
				DBG("Bt Battery Volt: %lu\n", (uint32_t)LdoinLevelAverage);
			}
		}
//#else
		//BtDisableBatteryState();
#endif		

#ifdef	OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{		
			PowerMonitorDisp();
			//DBG("IsInCharge\n");
			return;
		}
#endif

#ifdef POWER_MONITOR_DISP_EN		
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
#endif
    //��ص����ͱ���	
#ifdef FUNC_SOUND_REMIND
		if((LdoinLevelAverage <= LDOIN_VOLTAGE_LOW) && !IsInCharge())
		{
			if(LdoinLevelAverage <= LDOIN_VOLTAGE_OFF)
			{
				if(PowOffPowerCnt > 0)
				{
					PowOffPowerCnt--;
					if(PowOffPowerCnt == FALSE)
					{
						PowLowSoundCnt = 5;
					}
				}
			}
			else
			{
				PowOffPowerCnt = POWER_OFF_OFF_COUNT;
			}

            if(PowLowSoundCnt > 0)
			    PowLowSoundCnt--;
            
			if(PowLowSoundCnt%60 == 3)
			{
				//���﷢�͵͵���Ϣ������������ʾ��
				MsgSend(MSG_BAT_LOW_PWR); 
			}
			DBG("PowerMonitor, Low Voltage: %lu : %d\n", (uint32_t)LdoinLevelAverage, PowLowSoundCnt);	
		}
		else if(LdoinLevelAverage >= LDOIN_VOLTAGE_MID)
		{
			PowLowSoundCnt = POWER_LOW_OFF_COUNT;
			PowOffPowerCnt = POWER_OFF_OFF_COUNT;
		}
#endif	

		if(((LdoinLevelAverage <= LDOIN_VOLTAGE_LOW && PowerOnInitFlag == TRUE)
#ifdef FUNC_SOUND_REMIND
		 || PowLowSoundCnt == FALSE
#endif
		 )
#ifdef FUNC_WIFI_EN
		 && (WiFiFirmwareUpgradeStateGet() != TRUE)
#endif
			)
		{
			//���ڹػ���ѹ������ػ�����
			//����������ʾ�͵�ѹ��ʾ��Ȼ��ִ�йػ�����
#ifdef POWER_MONITOR_DISP_EN			
			PwrMntDisp = PWR_MNT_DISP_SYS_OFF;
			PowerMonitorDisp();
#endif		
			//WaitMs(1000);
			//ֹͣ�����������̣���������ʾ����DAC���ع��ŵ�Դ�ȶ���
			DBG("PowerMonitor, Low Voltage!PD.\n");		
			
			//low level voltage detect in power on sequence, power down system directly
			if(PowerOnInitFlag == TRUE)
			{
				APP_DBG("Power on Low level voltage detected->Power Off\n");
				PowLowSoundCnt = 5;
                /* ������ڹػ���ѹ��ֱ�ӽ���ػ����� */
                if(LdoinLevelAverage < LDOIN_VOLTAGE_OFF)
                {
                    APP_DBG("Low level voltage detected->Power Off now!!!\n");  
				    SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
				    SysGotoPowerDown();
                    while(1);
                }
			}
			else	// PowerOnInitFlag == FALSE
			{
                bool IsPowerOff = TRUE;
                
				if((gSys.CurModuleID < MODULE_ID_END) && (TRUE == IsPowerOff))	/*MSG_COMMON_CLOSE only need send once*/
				{
					/*if use push button, send message, for task's saving info.*/
					APP_DBG("Low level voltage detected->send message common close\n");
                    IsPowerOff = FALSE;
				#ifdef FUNC_WIFI_POWER_KEEP_ON
					WiFiRequestMcuPowerOff();
				#else
                    SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
				    SysGotoPowerDown();
                    while(1);
				#endif
				}
			}
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
	
#ifdef ADC_POWER_MONITOR_EN
	SarAdcGpioSel(ADC_POWER_MONITOR_PORT);
#endif	
	
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
    #ifdef ADC_POWER_MONITOR_EN
		  LdoinSampleSum += (SarAdcChannelGetValue(ADC_POWER_MONITOR_PORT)*3400/4095);
	  #else
	 	  LdoinSampleSum += SarAdcGetLdoinVoltage();		
	  #endif	
			WaitMs(5);
		}		
		//Ϊ��ߵ�LDOIN����ʼ������
		PowerLdoinLevelMonitor(TRUE);
#endif
	}
#ifdef BT_APPLE_BATTERY_STATUS
	TimeOutSet(&BtBatteryDetTim, 0);	
#endif
}

//��ص�ǰ�������
uint8_t GetCurBatterLevelAverage(void)
{
	uint8_t AdcBatVol = 0;

	if(LdoinLevelAverage > LDOIN_VOLTAGE_FULL)
	{
		AdcBatVol = 100;
	}
	else if(LdoinLevelAverage < LDOIN_VOLTAGE_LOW)
	{
		AdcBatVol = 1;
	}
	else
	{
		AdcBatVol = ((LdoinLevelAverage - LDOIN_VOLTAGE_LOW)*100/(LDOIN_VOLTAGE_FULL - LDOIN_VOLTAGE_LOW));
	}
	
	DBG("Battery Level Average: %lu\n", (uint32_t)LdoinLevelAverage);
	return AdcBatVol;
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
				//SingLedDispStateSet(LED_DISP_STATE_CHREGERED, TRUE);
				//DBG("charger.....\n");
			}
			else
			{
				//SingLedDispStateSet(LED_DISP_STATE_CHREGERED, FALSE);
				//DBG("charger full\n");
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
#define SLIDE_SWITCH_ONTIME 200
	static uint16_t  slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;//����ʱ��0.5s����PowerKeyDetect()����
	if(PowerKeyDetect())
	{
		if((slide_switch_pd_cnt-- == 0)
#ifdef FUNC_WIFI_EN                             //WiFi�����н�ֹ�ػ�
		 && (WiFiFirmwareUpgradeStateGet() != TRUE)
#endif
			)
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
	static uint16_t  soft_switch_pd_cnt = 500;
	
	if(PowerkeyGetOnkeyReg() 
#ifdef FUNC_WIFI_EN
	//WiFi�����н�ֹ�ػ�
	&& (WiFiFirmwareUpgradeStateGet() != TRUE)
#endif
	)
	{
		if(soft_switch_pd_cnt > 0)	
		{	
			soft_switch_pd_cnt--;
			if(soft_switch_pd_cnt == FALSE)
			{
				if(gSys.CurModuleID >= MODULE_ID_END)
				{
					APP_DBG("PowerKeyDetect->send message power on\n");
				#ifdef FUNC_GPIO_POWER_ON_EN
					SysPowerOnControl(TRUE);
				#endif	
					if(IsLineInLink())
					{
						gSys.NextModuleID = MODULE_ID_LINEIN;
					}
					else
					{
						gSys.NextModuleID = MODULE_ID_WIFI;
					}
					MsgSend(MSG_COMMON_CLOSE);
				}
				else if((gSys.CurModuleID < MODULE_ID_END) && (gSys.NextModuleID < MODULE_ID_END))
				{
					/*if use push button, send message, for task's saving info.*/
					APP_DBG("PowerKeyDetect->send message common close\n"); 
				#ifdef FUNC_WIFI_POWER_KEEP_ON
					WiFiRequestMcuPowerOff();
				#else
					gSys.NextModuleID = MODULE_ID_POWEROFF;
					MsgSend(MSG_COMMON_CLOSE);
				#endif
				}
			}
		}
	}
	else
	{
		soft_switch_pd_cnt = 2000;
	}
#else
	if(PowerKeyDetect()
#ifdef FUNC_WIFI_EN
	//WiFi�����н�ֹ�ػ�
	&& (WiFiFirmwareUpgradeStateGet() != TRUE)
#endif
	)
	{
		if((gSys.CurModuleID < MODULE_ID_END) && (gSys.NextModuleID < MODULE_ID_END))
		{
			/*if use push button, send message, for task's saving info.*/
			APP_DBG("PowerKeyDetect->send message common close\n"); 
	#ifdef FUNC_WIFI_POWER_KEEP_ON
			WiFiRequestMcuPowerOff();
	#else
			gSys.NextModuleID = MODULE_ID_POWEROFF;
			MsgSend(MSG_COMMON_CLOSE);
	#endif
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
	
#ifdef FUNC_SLEEP_EN
	if(gSys.SleepStartPowerOff)
	{
	    gSys.SleepStartPowerOff = 0;
		TimeOutSet(&PowerOffDelTmr, 3000);
		while(!IsTimeOut(&PowerOffDelTmr))
		{
  #ifdef FUNC_WATCHDOG_EN
			WdgFeed();
  #endif
		}
	}
#endif
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
