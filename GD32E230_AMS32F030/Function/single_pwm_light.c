/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : func_led.c
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��18��
  ����޸�   :
  ��������   : LED��ʾʵ��
  �����б�   :
              Led_DispInit
              Led_DispScan
  �޸���ʷ   :
  1.��    ��   : 2019��4��18��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "app_config.h"
/*----------------------------------------------*
 * ����˵��                                 *
 *----------------------------------------------*/
typedef struct _LIGHT_DISP_ARR_ {
	DISP_COLOR_TYPEDEF DispColor;
	uint8_t DispDuty;
}LIGHT_DISP_ARR;

typedef struct _LIGHT_PWM_CHANNEL_ {
	uint16_t Pwm_OutChSel;
	uint8_t Pwm_OutIoSel;
	uint8_t Pwm_OutLevel;
}LIGHT_PWM_CHANNEL;
 
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
static LIGHT_DISP_ARR Light_ScanParam;
static SW_TIMER Light_DispChangeTime;
static DISP_STATE_TYPEDEF CurDispState;
static uint32_t CurDispTimer;

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
const LIGHT_PWM_CHANNEL Light_Define[MAX_LIGHT_PWM_CHANNEL] = {
	{PWM_LIGHT_CHANNEL_RED, PWM_LIGHT_OUTIO_RED, PWM_LIGHT_LEVEL_RED},
	{PWM_LIGHT_CHANNEL_GREEN, PWM_LIGHT_OUTIO_GREEN, PWM_LIGHT_LEVEL_GREEN},
	{PWM_LIGHT_CHANNEL_BLUE, PWM_LIGHT_OUTIO_BLUE, PWM_LIGHT_LEVEL_BLUE},
};

const uint16_t LightColorTable[DISP_COLOR_MAX+1][3] =
{
	{0,			0,			0},								//����
	{100,		0,			0},								//��
	{100,		15,		0}, 							//��
	{100,		50,		0}, 							//��
	{0,			100,		0}, 							//��
	{0,			100,		100},							//��
	{0,			0,			100}, 							//��
	{100,		0,			100}, 							//��
	{100,		100,		100}, 							//��
};


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
static void Light_DispAutoChange(void) {
	static uint8_t Duty = 0, CurParam = 0;
	static FlagStatus Flip = RESET;
	
	switch (CurDispState) {
		case DISP_STATE_SINGLE_COLOR:
			Light_ScanParam.DispDuty = PWM_LIGHT_MAX_DUTY;
			
			ChangeTimerPeriod(&Light_DispChangeTime, CurDispTimer);
			break;

		case DISP_STATE_CUR_BLINK:
			Duty = 0;
			Flip = (FlagStatus)!Flip;
			if (SET == Flip) {
				Duty = PWM_LIGHT_MAX_DUTY;
			}
			Light_ScanParam.DispDuty = Duty;
				
			ChangeTimerPeriod(&Light_DispChangeTime, CurDispTimer);
			break;

		case DISP_STATE_FULL_COLOR:
		case DISP_STATE_LOOP_CHANGE:
			if (Duty >= PWM_LIGHT_MAX_DUTY) {
				Flip = RESET;
				Duty = PWM_LIGHT_MAX_DUTY;
			}
			else if (Duty <= PWM_LIGHT_MIN_DUTY) {
				Flip = SET;
				Duty = PWM_LIGHT_MIN_DUTY;
				CurParam = 0xFF;
			}
		
			if (SET == Flip) {
				Duty += (PWM_LIGHT_MAX_DUTY/USE_LIGHT_SCAN_RATIO);
			}
			else {
				Duty -= (PWM_LIGHT_MAX_DUTY/USE_LIGHT_SCAN_RATIO);
			}

			if (DISP_STATE_FULL_COLOR == CurDispState) {
				if ((0xFF == CurParam) && (SET == Flip)) {
					CurParam = 0;
					Light_ScanParam.DispColor++;
				}
				Light_ScanParam.DispColor %= DISP_COLOR_MAX;
				if (DISP_COLOR_OFF == Light_ScanParam.DispColor) {
					Light_ScanParam.DispColor = DISP_COLOR_RED;
				}
			}
			Light_ScanParam.DispDuty = Duty;
			
			ChangeTimerPeriod(&Light_DispChangeTime, CurDispTimer/USE_LIGHT_SCAN_RATIO);
			break;

		default:
			memset(&Light_ScanParam, 0, sizeof(Light_ScanParam));
			ChangeTimerPeriod(&Light_DispChangeTime, CurDispTimer);
			break;	
	}
}


static void Light_ScanPwmConfig(void) {
	uint8_t PwmInitIndex = 0;
	for (PwmInitIndex = 0; PwmInitIndex < USE_LIGHT_SCAN_CHANNEL; PwmInitIndex++) {
		PwmConfigChannel(Light_Define[PwmInitIndex].Pwm_OutChSel, PWM_LIGHT_SCAN_FREQ, 0);
		PwmEnableChannel(Light_Define[PwmInitIndex].Pwm_OutChSel, Light_Define[PwmInitIndex].Pwm_OutIoSel, Light_Define[PwmInitIndex].Pwm_OutLevel);
	}
}

void Light_DispInit(void) {
	Light_ScanPwmConfig();
	
	InitTimer(&Light_DispChangeTime, 0, Light_DispAutoChange);
	StartTimer(&Light_DispChangeTime);
	
	memset(&Light_ScanParam, 0, sizeof(Light_ScanParam));

	Light_DispStateSet(DISP_STATE_IDLE, DISP_COLOR_OFF, 0);

	printf ("Light disp init OK!!!\n");
}

void Light_Display(void) {
	CheckTimer(&Light_DispChangeTime);
}

void Light_DispScan(void) {
	uint8_t ScanNum = 0;
	uint16_t Duty;

	for (ScanNum = 0; ScanNum < USE_LIGHT_SCAN_CHANNEL; ScanNum++) {
		Duty = LightColorTable[Light_ScanParam.DispColor][ScanNum];				//��ȡ��ǰ��ɫ����������PWMͨ����ռ�ձ�
		Duty = (Duty*Light_ScanParam.DispDuty/100);								//����ʵ����Ҫ���õ�ռ�ձ�
		PwmConfigChannel(Light_Define[ScanNum].Pwm_OutChSel, PWM_LIGHT_SCAN_FREQ, Duty);
	}
}

void Light_DispStateSet(DISP_STATE_TYPEDEF State, DISP_COLOR_TYPEDEF Color, uint32_t Timer) {
	CurDispState = State;
	Light_ScanParam.DispColor = Color;
	CurDispTimer = Timer;
}

