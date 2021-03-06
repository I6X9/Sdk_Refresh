#include "app_config.h"
#ifdef FUNC_SINGLE_LED_EN
#include "folder.h"
#include "Fat_file.h"
#include "audio_decoder.h"
#include "player_control.h"
#include "radio_control.h"
#include "line_in_control.h"
#include "sys_app.h"
#include "recorder_control.h"
#include "idle_control.h"
#include "eq.h"
#include "eq_params.h"
#include "sys_vol.h"
#include "wifi_control.h"

#ifdef FUNC_POWER_MONITOR_EN
#include "power_management.h"
#endif
#include "led_display.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#include "aip1629a.h"
#include "singled_display.h"

#define SINGLE_BLED_INIT()				do{\
										PwmConfig(PWM_CH3_A10_B24, 1200, 100);\
										PwmEnableChannel(PWM_CH3_A10_B24, PWM_IO_SEL0, PWM_MODE_OUT);\
										}while(0)
#define SINGLE_RLED_INIT()				do{\
										PwmConfig(PWM_CH0_A0_B27, 1200, 0);\
										PwmEnableChannel(PWM_CH0_A0_B27, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
										
#define SINGLE_BLED_SETTING(Duty)		do{\
										PwmConfig(PWM_CH3_A10_B24, 1200, 1200*Duty/100);\
										PwmEnableChannel(PWM_CH3_A10_B24, PWM_IO_SEL0, PWM_MODE_OUT);\
										}while(0)
#define SINGLE_RLED_SETTING(Duty)		do{\
										PwmConfig(PWM_CH0_A0_B27, 1200, 1200*Duty/100);\
										PwmEnableChannel(PWM_CH0_A0_B27, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)

//==============================================================================
static SINGLE_LED_DISP_STRU Led;
static TIMER SingleLedChangeTime;

static void SingleLedChangeDutyFunc(void)
{
	if(Led.BledFlag&(1<<Led.DispBitCnt))
	{
		Led.BledDuty += Led.BledStepUpDuty;
	
		if(SINGLE_LED_MAX_DUTY <= Led.BledDuty)
		{
			Led.BledDuty = SINGLE_LED_MAX_DUTY;
		}
	}
	else if(!(Led.BledFlag&(1<<Led.DispBitCnt)))
	{
		if(Led.BledDuty >= Led.BledStepUpDuty)
		{
			Led.BledDuty -= Led.BledStepUpDuty;
		}
	}
	//APP_DBG ("bled display duty = %d;\n", Led.BledDuty);
	SINGLE_BLED_SETTING(Led.BledDuty);

	if(Led.RledFlag&(1<<Led.DispBitCnt))
	{
		Led.RledDuty += Led.RledStepUpDuty;
		
		if(SINGLE_LED_MAX_DUTY <= Led.RledDuty)
		{
			Led.RledDuty = SINGLE_LED_MAX_DUTY;
		}
	}
	else if(!(Led.RledFlag&(1<<Led.DispBitCnt)))
	{
		if(Led.RledDuty >= Led.RledStepUpDuty)
		{
			Led.RledDuty -= Led.RledStepUpDuty;
		}
	}
	//APP_DBG ("rled display duty = %d;\n", Led.RledDuty);
	SINGLE_RLED_SETTING(Led.RledDuty);
}

void SingleLedDisplayModeSet(LED_MODE_TYPE DisplayMode, bool IsOnOff)
{	
	if(gDisplayMode == DISP_DEV_SLED)
	{
		if((Led.CurDisplayMode&(1<<DisplayMode)) == IsOnOff)
		{
			return;
		}

		if(IsOnOff)
		{
			Led.CurDisplayMode |= (1<<DisplayMode);
		}
		else
		{
			Led.CurDisplayMode &= ~(1<<DisplayMode);
		}

		if (Led.CurDisplayMode&(1<<LED_DISPLAY_MODE_LOWBATTER)) {
			Led.RledFlag = 0xAA;
			Led.RledStepUpDuty = (SINGLE_LED_MAX_DUTY/SINGLE_LED_DISP_SCAN_CNT);
			Led.DispBitCnt = 0;
			Led.DispChangeTime = (1500/SINGLE_LED_DISP_SCAN_CNT);
		}
		else {
			Led.RledFlag = 0x00;
		}

		if (Led.CurDisplayMode&(1<<LED_DISPLAY_MODE_WPSCONNECT)) {
			Led.BledFlag = 0xAA;
			Led.BledStepUpDuty = (SINGLE_LED_MAX_DUTY/SINGLE_LED_DISP_SCAN_CNT);
			Led.DispBitCnt = 0;
			Led.DispChangeTime = (1500/SINGLE_LED_DISP_SCAN_CNT);
		}
		else if (Led.CurDisplayMode&(1<<LED_DISPLAY_MODE_NIGHTLAMP)) {
			Led.BledFlag = 0xFF;
			Led.BledStepUpDuty = SINGLE_LED_MAX_DUTY;
			Led.DispBitCnt = 0;
			Led.DispChangeTime = (1500/SINGLE_LED_DISP_SCAN_CNT);
		}
		else {
			Led.BledFlag = 0x00;
		}
	}
}

void LedFlushDisp(void)
{
	if ((!IS_CUR_WORK_MODULE())
#ifdef FUNC_SLEEP_LEDOFF_EN
	|| (gSys.SleepLedOffFlag) 
#endif
	) {
		SINGLE_RLED_SETTING(0);
		SINGLE_BLED_SETTING(0);
		return;
	}
	//LED����ģʽ��
	if(gWiFi.TestModeState) {
		static bool IsLedOn = FALSE;
		if (IsTimeOut(&SingleLedChangeTime)) {
			TimeOutSet(&SingleLedChangeTime, 500);
			IsLedOn = !IsLedOn;
			if(IsLedOn) {
				SINGLE_RLED_SETTING(100);
				SINGLE_BLED_SETTING(100);
			}
			else {
				SINGLE_RLED_SETTING(0);
				SINGLE_BLED_SETTING(0);
			}
		}
		return;
	}

	if(IsTimeOut(&SingleLedChangeTime))
	{
		TimeOutSet(&SingleLedChangeTime, Led.DispChangeTime);
		if(((0 == Led.RledDuty) || (SINGLE_LED_MAX_DUTY == Led.RledDuty))
		&& ((0 == Led.BledDuty) || (SINGLE_LED_MAX_DUTY == Led.BledDuty))) {
			Led.DispBitCnt++;
			if(Led.DispBitCnt >= 8)
			{
				Led.DispBitCnt = 0;
			}
		}
		SingleLedChangeDutyFunc();
	}
}

void SingleLedFlushDispInit(void)
{
	memset(&Led, 0, sizeof(SINGLE_LED_DISP_STRU));

	TimeOutSet(&SingleLedChangeTime, 0);
	SINGLE_BLED_INIT();
	SINGLE_RLED_INIT();
}

bool GetSingleLedDispMode(LED_MODE_TYPE Mode)
{
	return (Led.CurDisplayMode&(1<<Mode));
}
#endif

