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
#include "sys_vol.h"
#include "power_management.h"
#ifdef FUNC_WIFI_EN
#include "wifi_control.h"
#include "wifi_uart_com.h"
#include "wifi_mcu_command.h"
#endif
#ifdef FUNC_DISP_EN
#include "led_display.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#include "singled_display.h"
#endif

#ifdef BT_APPLE_BATTERY_STATUS 
#define BT_BATTERY_LEVEL_CNT    9
static uint8_t BtBatteryLevel = BT_BATTERY_LEVEL_CNT;
TIMER BtBatteryDetTim;
#endif

#ifdef FUNC_POWER_MONITOR_EN

//#define POWER_MONITOR_DISP_EN

#define LDOIN_SAMPLE_COUNT			20		//��ȡLDOIN����ʱ����ƽ���Ĳ�������
#define LDOIN_SAMPLE_PERIOD			50		//��ȡLDOIN����ʱ��ȡ����ֵ�ļ��(ms)
#define LOW_POWER_DET_COUNT 		3		//�͵��ⱨ������
#define LOW_POWER_SOUND_CNT 		300		//�͵籨����ʾ�����ʱ��(��λs)
#define LOW_POWEROFF_VOLTAGE		3		//�͵�ػ���ص����ٷֱ�
#define LOW_POWER_RESET_VOLTAGE		30		//�͵���ָ���ص����ٷֱ�

//�͵�����ʱʱ�������������������ʾ�����š�
#define LOW_POWEROFF_DELAY			(500)	//��λ��10ms
#define POWER_OFF_JITTER_TIMER		500

//���¶��岻ͬ�ĵ�ѹ����¼��Ĵ�����ѹ(��λmV)���û���������ϵͳ��ص��ص�������
#ifdef ADC_POWER_MONITOR_EN 
   #define LDOIN_VOLTAGE_FULL			3100
   #define LDOIN_VOLTAGE_HIGHT          3000
   #define LDOIN_VOLTAGE_HIGH			2800
   #define LDOIN_VOLTAGE_MID			2600
   #define LDOIN_VOLTAGE_LOW			2500
   #define LDOIN_VOLTAGE_OFF			2400	//ע�⣺������ADCֵ��Vol=log/4095*3.41,��λV
#else
   #define LDOIN_VOLTAGE_FULL			4150
   #define LDOIN_VOLTAGE_HIGH			3900
   #define LDOIN_VOLTAGE_MID			3650
   #define LDOIN_VOLTAGE_LOW			3500
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

#ifdef POWER_MONITOR_DISP_EN
//��Ҫ���ڵ�ѹ״̬��ʾ�ı���
static TIMER BlinkTimer;
static PWR_MNT_DISP PwrMntDisp;
#endif

//���ڵ�ѹ���ı���
TIMER PowerMonitorTimer;
uint32_t LdoinSampleSum = 0; 
uint16_t  LdoinSampleCnt = LDOIN_SAMPLE_COUNT;
uint32_t LdoinLevelAverage = 0;		//��ǰLDOIN��ѹƽ��ֵ
//���ڹػ�������
TIMER PowerOffDetectTimer;

#ifdef FUNC_SOUND_REMIND
#include "sound_remind.h"
static bool 	IsSystemPowerLow = FALSE;
static uint8_t	LowPowerSoundCnt = LOW_POWER_DET_COUNT;
static uint32_t LowPowerDetCnt = 0;
#endif

#ifdef	OPTION_CHARGER_DETECT
//Ӳ�����PC ����������״̬
//ʹ���ڲ���������PC����������ʱ������Ϊ�͵�ƽ����ʱ����Ϊ�ߵ�ƽ
bool IsInCharge(void)
{
	GpioSetRegBits(CHARGE_DETECT_PORT_PU, CHARGE_DETECT_BIT);
	GpioClrRegBits(CHARGE_DETECT_PORT_PD, CHARGE_DETECT_BIT);
	GpioClrRegBits(CHARGE_DETECT_PORT_OE, CHARGE_DETECT_BIT);	   
	GpioSetRegBits(CHARGE_DETECT_PORT_IE, CHARGE_DETECT_BIT);
	WaitUs(2);

	if(GpioGetReg(CHARGE_DETECT_PORT_IN)&CHARGE_DETECT_BIT) {
		return TRUE;
	} 

	return FALSE;
}
#endif
#ifdef	OPTION_CHAR_FUL_DETECT
/*****************************************************************************
 �� �� ��  : IsChargeFul
 ��������  : ��س�������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��29��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
bool IsChargeFull(void)
{
//��Ϊ���룬��������

	GpioSetRegBits(CHAR_FUL_DETECT_PORT_PU, CHAR_FUL_DETECT_BIT);
	GpioClrRegBits(CHAR_FUL_DETECT_PORT_PD, CHAR_FUL_DETECT_BIT);
	GpioClrRegBits(CHAR_FUL_DETECT_PORT_OE, CHAR_FUL_DETECT_BIT);	   
	GpioSetRegBits(CHAR_FUL_DETECT_PORT_IE, CHAR_FUL_DETECT_BIT);
	WaitUs(2);
	if(!(GpioGetReg(CHAR_FUL_DETECT_PORT_IN) & CHAR_FUL_DETECT_BIT))
	{
		return TRUE;
	}   	

	return FALSE;
}
#endif

void PowerMonitorDisp(void)
{
#if (defined(FUNC_SEG_LED_EN) || defined(FUNC_SEG_LCD_EN))
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
//#else
		//BtDisableBatteryState();
#endif	

#ifdef	OPTION_CHARGER_DETECT
		if(IsInCharge())		//������Ѿ�����Ĵ���
		{	
			//DBG("IsInCharge\n");
			IsSystemPowerLow = FALSE;
			LowPowerSoundCnt = LOW_POWER_DET_COUNT;
			LowPowerDetCnt = (LOW_POWER_SOUND_CNT*100-LOW_POWEROFF_DELAY);
#ifdef FUNC_SINGLE_LED_EN
			SingleLedDisplayModeSet(LED_DISPLAY_MODE_LOWBATTER, FALSE);
#endif
			return;
		}
#endif

#ifdef POWER_MONITOR_DISP_EN		
		if(LdoinLevelAverage > LDOIN_VOLTAGE_HIGH)	  
		{
			//������ʾ�������������PowerMonitorDisp�������ʾ����
			PwrMntDisp = PWR_MNT_DISP_HIGH_V;
			//DBG("bat full\n");
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_MID)
		{
			//������ʾ2�����������PowerMonitorDisp�������ʾ����
			PwrMntDisp = PWR_MNT_DISP_MID_V;
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_LOW)
		{
			//������ʾ1�����������PowerMonitorDisp�������ʾ����
			PwrMntDisp = PWR_MNT_DISP_LOW_V;
			//DBG("bat LOW\n");
		}
		else if(LdoinLevelAverage > LDOIN_VOLTAGE_OFF)
		{
			//������ʾ0�����������PowerMonitorDisp�������ʾ����
			PwrMntDisp = PWR_MNT_DISP_EMPTY_V;
		}
		else
		{
		    PwrMntDisp = PWR_MNT_DISP_SYS_OFF;
		}
#endif

		if (LOW_POWEROFF_VOLTAGE >= GetCurBatterLevelAverage())
		{
			//���ڹػ���ѹ������ػ�����
			//ֹͣ�����������̣���������ʾ����DAC���ع��ŵ�Դ�ȶ���
			DBG("PowerMonitor, Low Voltage!PD. %d:%d\n", LowPowerDetCnt, LowPowerSoundCnt);
			
			//low level voltage detect in power on sequence, power down system directly
			if(PowerOnInitFlag == TRUE)
			{
				APP_DBG("Power on Low level voltage detected->Power Off\n");
				LowPowerSoundCnt = 1;
				LowPowerDetCnt = (LOW_POWER_SOUND_CNT*100-LOW_POWEROFF_DELAY);
				IsSystemPowerLow = TRUE; 
			}
			    
			if(LowPowerDetCnt >= (LOW_POWER_SOUND_CNT*100))
			{
				if (LowPowerSoundCnt) {
					APP_DBG("PowerMonitor, Low Voltage sound remind!!!!");
					MsgSend(MSG_BAT_LOW_PWR);
					LowPowerSoundCnt--;
			#ifdef FUNC_SINGLE_LED_EN
					SingleLedDisplayModeSet(LED_DISPLAY_MODE_LOWBATTER, TRUE);
			#endif
				}

				IsSystemPowerLow = TRUE;
				LowPowerDetCnt = 0;
			}
			
			if(!LowPowerSoundCnt && (LowPowerDetCnt >= LOW_POWEROFF_DELAY*2)
#ifdef FUNC_WIFI_EN
			&& (!WiFiFirmwareUpgradeStateGet())
#endif
			) {
				if((MODULE_ID_POWEROFF != gSys.CurModuleID) && IsTimeOut(&PowerOffDetectTimer))	/*MSG_COMMON_CLOSE only need send once*/
				{
					/*if use push button, send message, for task's saving info.*/
					APP_DBG("Low level voltage detected->send message common close\n");
					TimeOutSet(&PowerOffDetectTimer, POWER_OFF_JITTER_TIMER);
				#if 0 //def FUNC_WIFI_POWER_KEEP_ON
					WiFiRequestMcuPowerOff();
				#else
					gSys.NextModuleID = MODULE_ID_POWEROFF;
					MsgSend(MSG_COMMON_CLOSE);
				#endif
				}
			}
		}
		else if (!IsSystemPowerLow || (LOW_POWER_RESET_VOLTAGE <= GetCurBatterLevelAverage())){
			IsSystemPowerLow = FALSE;
			LowPowerSoundCnt = LOW_POWER_DET_COUNT;
			LowPowerDetCnt = (LOW_POWER_SOUND_CNT*100-LOW_POWEROFF_DELAY);
		#ifdef FUNC_SINGLE_LED_EN
			SingleLedDisplayModeSet(LED_DISPLAY_MODE_LOWBATTER, FALSE);
		#endif
		}
	}
	PowerMonitorDisp();
}


//���ܼ��ӳ�ʼ��
//ʵ��ϵͳ���������еĵ͵�ѹ��⴦���Լ���ʼ������豸������IO��
void PowerMonitorInit(void)
{
	TimeOutSet(&PowerMonitorTimer, 0);
	TimeOutSet(&PowerOffDetectTimer, 0);
	
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

/*****************************************************************************
 �� �� ��  : GetCurBatterLevelAverage
 ��������  : ��ȡ��ǰ��ص����İٷֱ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��7��9��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
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
	
	//DBG("Battery Level Average: %lu\n", (uint32_t)LdoinLevelAverage);
	return AdcBatVol;
}

//ϵͳ��Դ״̬��غʹ���
//ϵͳ���������LDOIN���ڿ�����ѹ������ϵͳ���������м��LDOIN
void PowerMonitor(void)
{	
	if(IsTimeOut(&PowerMonitorTimer))
	{
		static bool IsBatterFull = FALSE;
		static bool IsFristCharge = FALSE;
		
		TimeOutSet(&PowerMonitorTimer, LDOIN_SAMPLE_PERIOD);
#ifdef OPTION_CHARGER_DETECT
		if(IsInCharge()) {		//������Ѿ�����Ĵ���	
			if (IS_CUR_WORK_MODULE()
			//&& !IS_RTC_WAKEUP()
			&& !WiFiFirmwareUpgradeStateGet()
			&& (MODULE_ID_UNKNOWN == gSys.NextModuleID)) {
				gSys.NextModuleID = MODULE_ID_IDLE;
				MsgSend(MSG_COMMON_CLOSE);
			}

			if (IsFristCharge) {
				IsFristCharge = FALSE;
				if (IS_RTC_WAKEUP()) {
					RTC_WAKEUP_FLAG_CLR();
					APP_DBG ("Charge clr RTC wakeup flag;\n");
				}
			}
#ifdef OPTION_CHAR_FUL_DETECT
			if (IsChargeFull() && !IsBatterFull) {
				//��ʾ������
				IsBatterFull = TRUE;
				MsgSend(MSG_BAT_FUL_PWR);
			}
#endif
		}
		else {
			IsBatterFull = FALSE;
			IsFristCharge = TRUE;
			if (!IS_CUR_WORK_MODULE() && (MODULE_ID_UNKNOWN == gSys.NextModuleID)) {
				gSys.NextModuleID = MODULE_ID_WIFI;
				MsgSend(MSG_COMMON_CLOSE);
			}
		}
#endif
		IsBatterFull = IsBatterFull;	//ȥ�����뾯�档
		//û�в�����LDOIN_SAMPLE_COUNT��������������
		PowerLdoinLevelMonitor(FALSE);
	}
#ifdef FUNC_SOUND_REMIND
	LowPowerDetCnt++;			//�͵���ʾ����ʱ���㣻ÿ10MSһ�Σ�
#endif
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

/*****************************************************************************
 �� �� ��  : powerkey�����⺯�������ʵ��Ӳ����
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/

#ifdef FUNC_POWERKEY_SOFT_POWERON_EN
#define SOFT_SWITCH_ONTIME				500

bool SoftPowerKeyDetect(void)
{
	static uint16_t PowerKeyLinkState = 0, IsStateChange = 0xffff;
	static bool PrevPowerKeyState = FALSE;
	bool CurPowerKeyState = FALSE, ret = FALSE;

	//��Ϊ���룬������
	GpioClrRegBits(POWERKEY_DETECT_PORT_OE, POWERKEY_DETECT_PORT_BIT);
	GpioSetRegBits(POWERKEY_DETECT_PORT_IE, POWERKEY_DETECT_PORT_BIT);
	GpioClrRegBits(POWERKEY_DETECT_PORT_PU, POWERKEY_DETECT_PORT_BIT);
	GpioClrRegBits(POWERKEY_DETECT_PORT_PD, POWERKEY_DETECT_PORT_BIT);

	if (GpioGetReg(POWERKEY_DETECT_PORT_IN)&POWERKEY_DETECT_PORT_BIT) {
		CurPowerKeyState = TRUE;
	}

	if (CurPowerKeyState != PrevPowerKeyState) {
		PowerKeyLinkState = 0;
		PrevPowerKeyState = CurPowerKeyState;
		if (0xffff != IsStateChange) {
			IsStateChange = 0x00FF;
		}
	}

	if (PowerKeyLinkState < SOFT_SWITCH_ONTIME)
	{
		PowerKeyLinkState++;
	}
	else {	
		if (IS_RTC_WAKEUP()) {
			if (0x00FF == IsStateChange) {
				RTC_WAKEUP_FLAG_CLR();
				APP_DBG ("Switch change clr RTC wakeup flag;\n");
			}
		}
		else {
			ret = CurPowerKeyState;
		}
		IsStateChange = 0;
	}

	return ret;
}
#endif

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

// ����PowerDown��Ϊ��оƬ�ܽ��룬Ĭ������Ϊ���������
void SysGotoPowerDownGpioCfg(void)
{
	GpioSetRegBits(GPIO_A_OE, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_A_OUT, 0xFFFFFFFF);

	GpioSetRegBits(GPIO_B_OE, 0xFFFFFFFF);
	GpioClrRegBits(GPIO_B_OUT, 0xFFFFFFFF);

	GpioSetRegBits(GPIO_C_OE, 0x7FFF);
	GpioClrRegBits(GPIO_C_OUT, 0x7FFF);
	// BT close, not arbitrarily modify
	BTDevicePowerOff();
}

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
#ifdef FUNC_POWERKEY_SOFT_POWERON_EN
	#define SLIDE_SWITCH_ONTIME 2
	static uint16_t  slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;//����ʱ��0.5s����PowerKeyDetect()����
	if(SoftPowerKeyDetect() && IsTimeOut(&PowerOffDetectTimer))
#else
	#define SLIDE_SWITCH_ONTIME 500
	static uint16_t  slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;//����ʱ��0.5s����PowerKeyDetect()����
	if(PowerKeyDetect())
#endif
	{
		if((slide_switch_pd_cnt-- == 0)
#ifdef FUNC_WIFI_EN                             //WiFi�����н�ֹ�ػ�
		&& (!WiFiFirmwareUpgradeStateGet())
#endif
		)
		{
			/*if slide switch, power down system directly*/
			APP_DBG("PowerKeyDetect->go to PowerDown\n");
#ifdef FUNC_GPIO_POWER_ON_EN
			TimeOutSet(&PowerOffDetectTimer, POWER_OFF_JITTER_TIMER);
			SysPowerOnControl(FALSE);
			SysGotoPowerDownGpioCfg();
#else
			SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
			SysGotoPowerDown(); 
			while(1);
#endif
		}
	}
	else
	{
		slide_switch_pd_cnt = SLIDE_SWITCH_ONTIME;
	}
#endif 
	
#ifdef USE_POWERKEY_SOFT_PUSH_BUTTON
#ifdef FUNC_POWERKEY_SOFT_POWERON_EN
	if(SoftPowerKeyDetect()
#else
	if(PowerKeyDetect()
#endif
#ifdef FUNC_WIFI_EN
	//WiFi�����н�ֹ�ػ�
	&& (!WiFiFirmwareUpgradeStateGet())
#endif
	)
	{
		if((MODULE_ID_POWEROFF != gSys.CurModuleID) && IsTimeOut(&PowerOffDetectTimer))	/*MSG_COMMON_CLOSE only need send once*/
		{
			/*if use push button, send message, for task's saving info.*/
			APP_DBG("PowerKeyDetect->send message common close;\n");
			TimeOutSet(&PowerOffDetectTimer, POWER_OFF_JITTER_TIMER);
		#if 0//def FUNC_WIFI_POWER_KEEP_ON
			if (!IsPowerOff) {
				IsPowerOff = TRUE;
				WiFiRequestMcuPowerOff();
			}
		#else
			gSys.NextModuleID = MODULE_ID_POWEROFF;
			MsgSend(MSG_COMMON_CLOSE);
		#endif
		}
	}
#endif	
	
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
#ifdef FUNC_AMP_MUTE_EN
	gSys.MuteFlag = TRUE;
	GpioAmpMuteEnable();
#endif
	WaitMs(50);
	APP_DBG("SystemPowerOffControl->system will power off soon!\n");
#ifdef FUNC_WATCHDOG_EN
	WdgDis();				// disable watch dog
#endif

	SysGotoPowerDownGpioCfg();
	SysVarDeinit();
	//APP_DBG("REG_LP_WAKEUP = %d, s_onoff = %d\n", (*(volatile unsigned long*)0x4002200C), PowerkeyGetSOnOff());
	//SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_POWERKEY);
	SysSetWakeUpSrcInPowerDown(WAKEUP_SRC_PD_RTC);
	SysGotoPowerDown(); 

	/*Never reach here,  expect power down fail*/
	APP_DBG("System Power Down Fail!");
	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	//�ػ�ʧ�ܣ��������оƬ��20190722
	//NVIC_SystemReset();
#ifdef FUNC_WATCHDOG_EN
	WdgEn(WDG_STEP_3S);
#endif
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
