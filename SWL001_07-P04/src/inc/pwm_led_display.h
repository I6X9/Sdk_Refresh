#ifndef __PWM_LED_DISP__
#define __PWM_LED_DISP__


#include "gpio.h"
#include "adc.h"
#include "pwm.h"

typedef enum _PWM_LED_COLOUR_
{
	PWM_LED_COLOUR_IDLE = 0,
	PWM_LED_COLOUR_WHITE,
	PWM_LED_COLOUR_RED,
	PWM_LED_COLOUR_ORANGE,
	PWM_LED_COLOUR_YELLOW,
	PWM_LED_COLOUR_GREEN,
	PWM_LED_COLOUR_CYAN,
	PWM_LED_COLOUR_BLUE,
	PWM_LED_COLOUR_PURPLE,
	PWM_LED_COLOUR_COLORFUL,
	PWM_LED_COLOUR_MAX,
}
PWM_LED_COLOUR;

typedef struct _PWM_LED_STATE_
{
	PWM_LED_COLOUR	PwmLedColor;
	uint8_t			PwmLedDuty;
}PWM_LED_STATE;

#ifdef FUNC_ALEXA_PWM_LED_EN

#define    PWM_LED_SCAN_DISP_EN				//��ʾɨ�蹦��

/******************************************************************************/
//PWM ����ڶ��壬��ز����μ�pwm.h
/******************************************************************************/
#define   UXLED_RED_PWM_PORT_MASK		PWM_CH2_A9_B25
#define   UXLED_GREEN_PWM_PORT_MASK		PWM_CH1_A1_B26
#define   UXLED_BLUE_PWM_PORT_MASK		PWM_CH0_A0_B27	

/******************************************************************************/																																					
//����Ϊ���������ʾ��ɫ��������Ҫ�����޸�
//ע�⣺��ΪUXLED��ɫ�����ɫƫ���⣬��Ҫ���ݵ�ɫ�µ�����ɫ����
//����������PWM������Ӧ��ɫ��Ƶ�ʣ�10KHz����غ������ܣ��μ�pwm.h
/*****************************************************************************/
									
#define  UXLED_RED_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
#define  UXLED_GREEN_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)		

#define  UXLED_BLUE_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)															

#define  UXLED_WHITE_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 1164*Duty/100);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 780*Duty/100);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 840*Duty/100);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)													
	
#define  UXLED_ORANGE_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 180*Duty/100);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)

#define  UXLED_YELLOW_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 600*Duty/100);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)

#define  UXLED_PURPLE_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)																
															
#define  UXLED_CYAN_MODE_ON(Duty)		do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
															
#define  UXLED_ALL_MODE_OFF()			do{\
										PwmConfig(UXLED_RED_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_GREEN_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(UXLED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(UXLED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)


//LEDɨ��ӿڶ���
#ifdef PWM_LED_SCAN_DISP_EN

	#define PWM_LED_SCAN_MAX_NUM		6				//��ʾɨ�����LED����
//LED ��������GPIO ���Ӷ���,�����Ӳ������
//6ֻ�Ŷ���LED��ʾ��
	#define LED_PIN1_PORT_PU 			GPIO_B_PU
	#define LED_PIN1_PORT_PD 			GPIO_B_PD
	#define LED_PIN1_PORT_OE 			GPIO_B_OE
	#define LED_PIN1_PORT_IE 			GPIO_B_IE
	#define LED_PIN1_PORT_OUTDS 		GPIO_B_OUTDS
	#define LED_PIN1_PORT_OUT 			GPIO_B_OUT
	#define LED_PIN1_PORT_BIT			GPIOB23

	#define LED_PIN2_PORT_PU 			GPIO_B_PU
	#define LED_PIN2_PORT_PD 			GPIO_B_PD
	#define LED_PIN2_PORT_OE 			GPIO_B_OE
	#define LED_PIN2_PORT_IE 			GPIO_B_IE
	#define LED_PIN2_PORT_OUTDS 		GPIO_B_OUTDS
	#define LED_PIN2_PORT_OUT 			GPIO_B_OUT
	#define LED_PIN2_PORT_BIT			GPIOB24

	#define LED_PIN3_PORT_PU 			GPIO_C_PU
	#define LED_PIN3_PORT_PD 			GPIO_C_PD
	#define LED_PIN3_PORT_OE 			GPIO_C_OE
	#define LED_PIN3_PORT_IE 			GPIO_C_IE
	#define LED_PIN3_PORT_OUTDS 		GPIO_C_OUTDS
	#define LED_PIN3_PORT_OUT 			GPIO_C_OUT
	#define LED_PIN3_PORT_BIT			GPIOC14

	#define LED_PIN4_PORT_PU 			GPIO_C_PU
	#define LED_PIN4_PORT_PD 			GPIO_C_PD
	#define LED_PIN4_PORT_OE 			GPIO_C_OE
	#define LED_PIN4_PORT_IE 			GPIO_C_IE
	#define LED_PIN4_PORT_OUTDS 		GPIO_C_OUTDS
	#define LED_PIN4_PORT_OUT 			GPIO_C_OUT
	#define LED_PIN4_PORT_BIT			GPIOC13

	#define LED_PIN5_PORT_PU 			GPIO_C_PU
	#define LED_PIN5_PORT_PD 			GPIO_C_PD
	#define LED_PIN5_PORT_OE 			GPIO_C_OE
	#define LED_PIN5_PORT_IE 			GPIO_C_IE
	#define LED_PIN5_PORT_OUTDS 		GPIO_C_OUTDS
	#define LED_PIN5_PORT_OUT 			GPIO_C_OUT
	#define LED_PIN5_PORT_BIT			GPIOC12

	#define LED_PIN6_PORT_PU 			GPIO_C_PU
	#define LED_PIN6_PORT_PD 			GPIO_C_PD
	#define LED_PIN6_PORT_OE 			GPIO_C_OE
	#define LED_PIN6_PORT_IE 			GPIO_C_IE
	#define LED_PIN6_PORT_OUTDS 		GPIO_C_OUTDS
	#define LED_PIN6_PORT_OUT 			GPIO_C_OUT
	#define LED_PIN6_PORT_BIT			GPIOC11


//����LED ������������ߵ�ƽ

	#define LED_PIN1_OUT_ON		 		GpioSetRegOneBit(LED_PIN1_PORT_OE,LED_PIN1_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN1_PORT_OUT,LED_PIN1_PORT_BIT)

	#define LED_PIN2_OUT_ON				GpioSetRegOneBit(LED_PIN2_PORT_OE,LED_PIN2_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN2_PORT_OUT,LED_PIN2_PORT_BIT)

	#define LED_PIN3_OUT_ON		 		GpioSetRegOneBit(LED_PIN3_PORT_OE,LED_PIN3_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN3_PORT_OUT,LED_PIN3_PORT_BIT)

	#define LED_PIN4_OUT_ON		 		GpioSetRegOneBit(LED_PIN4_PORT_OE,LED_PIN4_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN4_PORT_OUT,LED_PIN4_PORT_BIT) 

	#define LED_PIN5_OUT_ON		 		GpioSetRegOneBit(LED_PIN5_PORT_OE,LED_PIN5_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN5_PORT_OUT,LED_PIN5_PORT_BIT) 

	#define LED_PIN6_OUT_ON				GpioSetRegOneBit(LED_PIN6_PORT_OE,LED_PIN6_PORT_BIT),\
										GpioSetRegOneBit(LED_PIN6_PORT_OUT,LED_PIN6_PORT_BIT)

//��ֹLED �������ŵ�������
	#define LED_PIN1_OUT_OFF			GpioSetRegOneBit(LED_PIN1_PORT_OE,LED_PIN1_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN1_PORT_OUT,LED_PIN1_PORT_BIT) 
										   
	#define LED_PIN2_OUT_OFF			GpioSetRegOneBit(LED_PIN2_PORT_OE,LED_PIN2_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN2_PORT_OUT,LED_PIN2_PORT_BIT) 
										   
	#define LED_PIN3_OUT_OFF	 		GpioSetRegOneBit(LED_PIN3_PORT_OE,LED_PIN3_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN3_PORT_OUT,LED_PIN3_PORT_BIT) 
										   
	#define LED_PIN4_OUT_OFF	 		GpioSetRegOneBit(LED_PIN4_PORT_OE,LED_PIN4_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN4_PORT_OUT,LED_PIN4_PORT_BIT)  
										   
	#define LED_PIN5_OUT_OFF	 		GpioSetRegOneBit(LED_PIN5_PORT_OE,LED_PIN5_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN5_PORT_OUT,LED_PIN5_PORT_BIT)  
										   
	#define LED_PIN6_OUT_OFF			GpioSetRegOneBit(LED_PIN6_PORT_OE,LED_PIN6_PORT_BIT),\
										GpioClrRegOneBit(LED_PIN6_PORT_OUT,LED_PIN6_PORT_BIT)  

//LED�������

	#define LED_ALL_POWER_ON()			do{\
										LED_PIN1_OUT_ON;\
										LED_PIN2_OUT_ON;\
										LED_PIN3_OUT_ON;\
										LED_PIN4_OUT_ON;\
										LED_PIN5_OUT_ON;\
										LED_PIN6_OUT_ON;\
										}while(0)

	#define LED_ALL_POWER_OFF()			do{\
										LED_PIN1_OUT_OFF;\
										LED_PIN2_OUT_OFF;\
										LED_PIN3_OUT_OFF;\
										LED_PIN4_OUT_OFF;\
										LED_PIN5_OUT_OFF;\
										LED_PIN6_OUT_OFF;\
										}while(0)


#endif

//��������
extern void PwmLedDisplayControlSetting(void);
#endif


#ifdef FUNC_LIGHT_PWM_LED_EN

#define   MAX_LIGHT_NUM					4	//LED ������
#define   PWM_LED_BLINK_FRQ				8	//LED ������


/******************************************************************************/
//PWM ����ڶ��壬��ز����μ�pwm.h
/******************************************************************************/
#define   LED_RED_PWM_PORT_MASK			PWM_CH5_B7_B22
#define   LED_GREEN_PWM_PORT_MASK		PWM_CH7_B9_B20
#define   LED_BLUE_PWM_PORT_MASK		PWM_CH6_B8_B21
#define   LED_LIGHT_PWM_PORT_MASK		PWM_CH3_A10_B24	

/******************************************************************************/																																					
//����Ϊ���������ʾ��ɫ��������Ҫ�����޸�
//ע�⣺��ΪLED��ɫ�����ɫƫ���⣬��Ҫ���ݵ�ɫ�µ�����ɫ����
//����������PWM������Ӧ��ɫ��Ƶ�ʣ�10KHz����غ������ܣ��μ�pwm.h
/*****************************************************************************/
									
 #define  LED_RED_MODE_ON(Duty)			do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
 #define  LED_RED_MODE_OFF()			do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
 #define  LED_GREEN_MODE_ON(Duty)		do{\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)	

 #define  LED_GREEN_MODE_OFF()			do{\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
 #define  LED_BLUE_MODE_ON(Duty)		do{\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)	

 #define  LED_BLUE_MODE_OFF()			do{\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)															


 #define  LED_LIGHT_MODE_ON(Duty)		do{\
										PwmConfig(LED_LIGHT_PWM_PORT_MASK, 1200, 1200*Duty/100);\
										PwmEnableChannel(LED_LIGHT_PWM_PORT_MASK, PWM_IO_SEL0, PWM_MODE_OUT);\
										}while(0)
 
 #define  LED_LIGHT_MODE_OFF()			do{\
										PwmConfig(LED_LIGHT_PWM_PORT_MASK, 1200, 0);\
										PwmEnableChannel(LED_LIGHT_PWM_PORT_MASK, PWM_IO_SEL0, PWM_MODE_OUT);\
										}while(0)
															
 #define  LED_ALL_MODE_OFF()			do{\
 										LED_RED_MODE_OFF();\
 										LED_GREEN_MODE_OFF();\
 										LED_BLUE_MODE_OFF();\
 										LED_LIGHT_MODE_OFF();\
 										}while(0)

//�չ���Զ����ӿڶ���
#ifdef LIGHT_DETECT_AUTO_CONTROL_EN
	#ifdef LIGHT_DETECT_GPIO1_MASK_BIT
		#define LIGHT_DETECT_GPIO1_PORT_IN				  (LIGHT_DETECT_GPIO1_PORT_OUT - 1)
		#define LIGHT_DETECT_GPIO1_PORT_IE				  (LIGHT_DETECT_GPIO1_PORT_OUT + 1)
		#define LIGHT_DETECT_GPIO1_PORT_OE				  (LIGHT_DETECT_GPIO1_PORT_OUT + 2)
		#define LIGHT_DETECT_GPIO1_PORT_PU				  (LIGHT_DETECT_GPIO1_PORT_OUT + 5)
		#define LIGHT_DETECT_GPIO1_PORT_PD				  (LIGHT_DETECT_GPIO1_PORT_OUT + 6)
	#endif

	#ifdef LIGHT_DETECT_GPIO2_MASK_BIT
		#define LIGHT_DETECT_GPIO2_PORT_IN				  (LIGHT_DETECT_GPIO2_PORT_OUT - 1)
		#define LIGHT_DETECT_GPIO2_PORT_IE				  (LIGHT_DETECT_GPIO2_PORT_OUT + 1)
		#define LIGHT_DETECT_GPIO2_PORT_OE				  (LIGHT_DETECT_GPIO2_PORT_OUT + 2)
		#define LIGHT_DETECT_GPIO2_PORT_PU				  (LIGHT_DETECT_GPIO2_PORT_OUT + 5)
		#define LIGHT_DETECT_GPIO2_PORT_PD				  (LIGHT_DETECT_GPIO2_PORT_OUT + 6)
	#endif

	#ifdef LIGHT_DETECT_GPIO3_MASK_BIT
		#define LIGHT_DETECT_GPIO3_PORT_IN				  (LIGHT_DETECT_GPIO3_PORT_OUT - 1)
		#define LIGHT_DETECT_GPIO3_PORT_IE				  (LIGHT_DETECT_GPIO3_PORT_OUT + 1)
		#define LIGHT_DETECT_GPIO3_PORT_OE				  (LIGHT_DETECT_GPIO3_PORT_OUT + 2)
		#define LIGHT_DETECT_GPIO3_PORT_PU				  (LIGHT_DETECT_GPIO3_PORT_OUT + 5)
		#define LIGHT_DETECT_GPIO3_PORT_PD				  (LIGHT_DETECT_GPIO3_PORT_OUT + 6)
	#endif
#endif

//********************************************************************************************//
typedef struct _PWM_LED_DUTY_
{
	uint8_t		PwmScanDuty;
	uint8_t		PwmSetDuty;
}PWM_LED_DUTY;

typedef enum _LIGHT_DETECT_STATE
{
	LIGHT_DETECT_STATE_IDLE,
	LIGHT_DETECT_STATE_JITTER,
	LIGHT_DETECT_STATE_PRESS_DOWN,
	LIGHT_DETECT_STATE_PRESS_UP,

} LIGHT_DETECT_STATE;

#endif

//��������
extern void PwmLedDisplayScanSet(bool State);
extern void PwmLedDisplayInit(void);
extern void PwmLedFlushDisp(void);
extern void WiFi_CtrlPwmLight(uint8_t* Cmdbuf, bool IsPasCmd);
extern void PwmDisplayLightOnOff(void);
extern void PwmLightBrightnessSetting(uint16_t Msg);
extern void PwmLightDisplayControlSetting(void);
extern void PwmRandomColourTimerBlink(void);
extern void PwmLedDisplayControlSuspend(bool status);
extern void LightLedDisplayAvsCmdProcess(uint8_t* AvsCmdBuf,uint16_t Len);
#endif

