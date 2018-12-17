#ifndef __PWM_LED_DISP__
#define __PWM_LED_DISP__


#include "gpio.h"
#include "adc.h"
#include "pwm.h"
#ifdef FUNC_ALEXA_PWM_LED_EN

#define MAX_LED_PIN_NUM				6	//LED ������
#define MAX_LED_COLOUR_NUM			8	//LED ������
#define PWM_LED_BLINK_FRQ			8	//LED ������

/******************************************************************************/
//PWM ����ڶ��壬��ز����μ�pwm.h
/******************************************************************************/
#define   LED_RED_PWM_PORT_MASK			PWM_CH7_B9_B20
#define   LED_GREEN_PWM_PORT_MASK		PWM_CH6_B8_B21
#define   LED_BLUE_PWM_PORT_MASK		PWM_CH5_B7_B22	

/******************************************************************************/																																					
//����Ϊ���������ʾ��ɫ��������Ҫ�����޸�
//ע�⣺��ΪLED��ɫ�����ɫƫ���⣬��Ҫ���ݵ�ɫ�µ�����ɫ����
//����������PWM������Ӧ��ɫ��Ƶ�ʣ�10KHz����غ������ܣ��μ�pwm.h
/*****************************************************************************/
									
 #define  LED_RED_MODE_ON(Duty)			do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
 #define  LED_GREEN_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)	
 
 #define  LED_BLUE_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)														

 #define  LED_WHITE_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 116*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 78*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 84*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)														
	
 #define  LED_ORANGE_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 18*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 0*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
 
#define  LED_YELLOW_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 60*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 0*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)

 #define  LED_PURPLE_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 0*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)																
															
 #define  LED_CYAN_MODE_ON(Duty)		do{\
										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 0*Duty/100);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 120*Duty/100);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										}while(0)
															
 #define  LED_ALL_MODE_OFF()			do{\
 										PwmConfig(LED_RED_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_GREEN_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
										PwmConfig(LED_BLUE_PWM_PORT_MASK, 120, 0);\
										PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
 										}while(0)

//�չ���Զ����ӿڶ���
#ifdef PWM_LED_SCAN_DISP_EN

//LED ��������GPIO ���Ӷ���,�����Ӳ������
//6ֻ�Ŷ���LED��ʾ��
#define LED_PIN1_PORT_PU 		GPIO_C_PU
#define LED_PIN1_PORT_PD 		GPIO_C_PD
#define LED_PIN1_PORT_OE 		GPIO_C_OE
#define LED_PIN1_PORT_IE 		GPIO_C_IE
#define LED_PIN1_PORT_OUTDS 	GPIO_C_OUTDS
#define LED_PIN1_PORT_OUT 		GPIO_C_OUT
#define LED_PIN1_PORT_BIT		GPIOC12

#define LED_PIN2_PORT_PU 		GPIO_C_PU
#define LED_PIN2_PORT_PD 		GPIO_C_PD
#define LED_PIN2_PORT_OE 		GPIO_C_OE
#define LED_PIN2_PORT_IE 		GPIO_C_IE
#define LED_PIN2_PORT_OUTDS 	GPIO_C_OUTDS
#define LED_PIN2_PORT_OUT 		GPIO_C_OUT
#define LED_PIN2_PORT_BIT		GPIOC11

#define LED_PIN3_PORT_PU 		GPIO_C_PU
#define LED_PIN3_PORT_PD 		GPIO_C_PD
#define LED_PIN3_PORT_OE 		GPIO_C_OE
#define LED_PIN3_PORT_IE 		GPIO_C_IE
#define LED_PIN3_PORT_OUTDS 	GPIO_C_OUTDS
#define LED_PIN3_PORT_OUT 		GPIO_C_OUT
#define LED_PIN3_PORT_BIT		GPIOC2

#define LED_PIN4_PORT_PU 		GPIO_A_PU
#define LED_PIN4_PORT_PD 		GPIO_A_PD
#define LED_PIN4_PORT_OE 		GPIO_A_OE
#define LED_PIN4_PORT_IE 		GPIO_A_IE
#define LED_PIN4_PORT_OUTDS 	GPIO_A_OUTDS
#define LED_PIN4_PORT_OUT 		GPIO_A_OUT
#define LED_PIN4_PORT_BIT		GPIOA19

#define LED_PIN5_PORT_PU 		GPIO_A_PU
#define LED_PIN5_PORT_PD 		GPIO_A_PD
#define LED_PIN5_PORT_OE 		GPIO_A_OE
#define LED_PIN5_PORT_IE 		GPIO_A_IE
#define LED_PIN5_PORT_OUTDS 	GPIO_A_OUTDS
#define LED_PIN5_PORT_OUT 		GPIO_A_OUT
#define LED_PIN5_PORT_BIT		GPIOA20

#define LED_PIN6_PORT_PU 		GPIO_A_PU
#define LED_PIN6_PORT_PD 		GPIO_A_PD
#define LED_PIN6_PORT_OE 		GPIO_A_OE
#define LED_PIN6_PORT_IE 		GPIO_A_IE
#define LED_PIN6_PORT_OUTDS 	GPIO_A_OUTDS
#define LED_PIN6_PORT_OUT 		GPIO_A_OUT
#define LED_PIN6_PORT_BIT		GPIOA21


//����LED ������������ߵ�ƽ

#define LED_PIN1_OUT_ON		 	GpioSetRegOneBit(LED_PIN1_PORT_OE,LED_PIN1_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN1_PORT_OUT,LED_PIN1_PORT_BIT)

#define LED_PIN2_OUT_ON			GpioSetRegOneBit(LED_PIN2_PORT_OE,LED_PIN2_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN2_PORT_OUT,LED_PIN2_PORT_BIT)

#define LED_PIN3_OUT_ON		 	GpioSetRegOneBit(LED_PIN3_PORT_OE,LED_PIN3_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN3_PORT_OUT,LED_PIN3_PORT_BIT)

#define LED_PIN4_OUT_ON		 	GpioSetRegOneBit(LED_PIN4_PORT_OE,LED_PIN4_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN4_PORT_OUT,LED_PIN4_PORT_BIT) 

#define LED_PIN5_OUT_ON		 	GpioSetRegOneBit(LED_PIN5_PORT_OE,LED_PIN5_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN5_PORT_OUT,LED_PIN5_PORT_BIT) 

#define LED_PIN6_OUT_ON			GpioSetRegOneBit(LED_PIN6_PORT_OE,LED_PIN6_PORT_BIT),\
								GpioSetRegOneBit(LED_PIN6_PORT_OUT,LED_PIN6_PORT_BIT)

//��ֹLED �������ŵ�������
#define LED_PIN1_OUT_OFF		GpioSetRegOneBit(LED_PIN1_PORT_OE,LED_PIN1_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN1_PORT_OUT,LED_PIN1_PORT_BIT) 
										   
#define LED_PIN2_OUT_OFF		GpioSetRegOneBit(LED_PIN2_PORT_OE,LED_PIN2_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN2_PORT_OUT,LED_PIN2_PORT_BIT) 
										   
#define LED_PIN3_OUT_OFF	 	GpioSetRegOneBit(LED_PIN3_PORT_OE,LED_PIN3_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN3_PORT_OUT,LED_PIN3_PORT_BIT) 
										   
#define LED_PIN4_OUT_OFF	 	GpioSetRegOneBit(LED_PIN4_PORT_OE,LED_PIN4_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN4_PORT_OUT,LED_PIN4_PORT_BIT)  
										   
#define LED_PIN5_OUT_OFF	 	GpioSetRegOneBit(LED_PIN5_PORT_OE,LED_PIN5_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN5_PORT_OUT,LED_PIN5_PORT_BIT)  
										   
#define LED_PIN6_OUT_OFF		GpioSetRegOneBit(LED_PIN6_PORT_OE,LED_PIN6_PORT_BIT),\
								GpioClrRegOneBit(LED_PIN6_PORT_OUT,LED_PIN6_PORT_BIT)  

//LED�������

#define LED_ALL_POWER_ON()				do{\
										LED_PIN1_OUT_ON;\
										LED_PIN2_OUT_ON;\
										LED_PIN3_OUT_ON;\
										LED_PIN4_OUT_ON;\
										LED_PIN5_OUT_ON;\
										LED_PIN6_OUT_ON;\
									}while(0)

#define LED_ALL_POWER_OFF()				do{\
										LED_PIN1_OUT_OFF;\
										LED_PIN2_OUT_OFF;\
										LED_PIN3_OUT_OFF;\
										LED_PIN4_OUT_OFF;\
										LED_PIN5_OUT_OFF;\
										LED_PIN6_OUT_OFF;\
									}while(0)


#endif

enum
{
	PWM_LED_COLOUR_IDLE = 0,
	PWM_LED_COLOUR_RED,
	PWM_LED_COLOUR_ORANGE,
	PWM_LED_COLOUR_YELLOW,
	PWM_LED_COLOUR_GREEN,
	PWM_LED_COLOUR_CYAN,
	PWM_LED_COLOUR_BLUE,
	PWM_LED_COLOUR_PURPLE,
	PWM_LED_COLOUR_WHITE,
	PWM_LED_COLOUR_COLORFUL,
	PWM_LED_COLOUR_MAX,
};

typedef struct _PWM_LED_STATE_
{
	uint8_t PwmLedColor;
	uint8_t PwmLedDuty;
}PWM_LED_STATE;

//��������
extern void PwmLedDisplayInit(void);
extern void PwmLedFlushDisp(void);
extern void PwmLedDisplayControlSetting(void);
#endif
#endif

