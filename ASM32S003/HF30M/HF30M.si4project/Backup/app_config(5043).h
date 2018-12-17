//Í¨ÓÃ¹¦ÄÜºê¶¨Òå¼°º¯ÊýÉùÃ÷

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#ifdef _APP_CONFIG_H_
#include "asm32s003.h"
#include "asm32s003_i2c.h"
#include "asm32s003_uart.h"
#include "asm32s003_syscon.h"
#include "asm32s003_gtc.h"
#include "asm32s003_pwm.h"
#include "asm32s003_adc.h"
#include "asm32s003_led.h"
#include "asm32s003_gpio.h"
#include "asm32s003_ifc.h"
#include "stdio.h"
#include "timeout.h"
#include "app_msg.h"
#include "key.h"

/////////////////////////////////////////////////////////////////////////////////////
//ϵͳ����Ƶ��
/////////////////////////////////////////////////////////////////////////////////////
#define  WorkFreq						20000000ul

/////////////////////////////////////////////////////////////////////////////////////
//UART ���ڴ�ӡ��������
/////////////////////////////////////////////////////////////////////////////////////
//#define  FUNC_APP_DEBUG_EN

#ifdef FUNC_APP_DEBUG_EN
	#define APP_DEBUG_UART_PROT_SEL		UART0
	//UART gpio�˿�ѡ��UART0��0=A12��A13��1=A5��A4;UART1:0=B1��B2��1=C2��C3��
	#define APP_DEBUG_UART_GPIO_SEL		0				    
	#define APP_DEBUG_UART_BAUDRATE		115200

	extern void APP_DBG(const char *format,...);
#endif

/////////////////////////////////////////////////////////////////////////////////////
//��Դ���ƹ�������
/////////////////////////////////////////////////////////////////////////////////////
#define FUNC_GPIO_POWER_CONTROL_EN					//GPIO����ϵͳ��Դ

#ifdef FUNC_GPIO_POWER_CONTROL_EN
    //GPIO�������ػ�����
	#define FUNC_GPIO_ON_OFF_EN   					//GPIO���ػ�����
	#ifdef FUNC_GPIO_ON_OFF_EN
		#define 	GPIO_ON_OFF_PORT_OUT			GPIOA0		
		#define 	GPIO_ON_OFF_PORT_BIT			4	
	#endif

    //GPIO����ϵͳ��Դ
	#define FUNC_GPIO_POWERON_EN   					//GPIOϵͳ��Դ����
	#ifdef FUNC_GPIO_POWERON_EN
		#define 	GPIO_POWERON_PORT_OUT			GPIOC0		
		#define 	GPIO_POWERON_PORT_BIT			3	
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////////////
//������⹦������
/////////////////////////////////////////////////////////////////////////////////////
#define  FUNC_KEY_EN

#ifdef FUNC_KEY_EN
//ADC KEY����
//	#define FUNC_ADC_KEY_EN							//ADC KEY�꿪��
	#ifdef FUNC_ADC_KEY_EN	
		#define		ADC_KEY_PORT_CH1	ADC_CHANNEL_B7		//���ֻʹ��1·ADC��������������
//		#define		ADC_KEY_PORT_CH2	ADC_CHANNEL_B5		//���ֻʹ��2·ADC��������������
	#endif
	
//IR KEY����
//	#define FUNC_IR_KEY_EN   						//IR KEY�꿪��
	#ifdef FUNC_IR_KEY_EN 
		#define 	IR_KEY_PORT						IR_USE_GPIOB7		//IRʹ�õĶ˿ڣ�IR_USE_GPIOA10, IR_USE_GPIOB7 or IR_USE_GPIOC2
	#endif
	
//IIC�ӻ���������
//	#define FUNC_IIC_KEY_EN   						//IIC �ӻ�����
	#ifdef FUNC_IIC_KEY_EN
		#define 	I2C_KEY_SCL_PORT_OUT			GPIOA0		
		#define 	I2C_KEY_SCL_PORT_BIT			1		
		#define 	I2C_KEY_SDA_PORT_OUT			GPIOA0	    
		#define 	I2C_KEY_SDA_PORT_BIT			2	     	
	#endif

//GPIO��������
	#define FUNC_GPIO_KEY_EN   						//GPIO����
	#ifdef FUNC_GPIO_KEY_EN
		#define 	GPIO_KEY1_PORT_OUT				GPIOA0		
		#define 	GPIO_KEY1_PORT_BIT				5	

		#define 	GPIO_KEY2_PORT_OUT				GPIOA0		
		#define 	GPIO_KEY2_PORT_BIT				6	

        #define 	GPIO_KEY3_PORT_OUT				GPIOB0		
		#define 	GPIO_KEY3_PORT_BIT				2	

		#define 	GPIO_KEY4_PORT_OUT				GPIOB0		
		#define 	GPIO_KEY4_PORT_BIT				3

        #define 	GPIO_KEY5_PORT_OUT				GPIOA0		
		#define 	GPIO_KEY5_PORT_BIT				8
	#endif

//CODING KEY����
//	#define FUNC_CODING_KEY_EN						//CODING KEY�꿪��
	#ifdef FUNC_CODING_KEY_EN
		#define 	CODING_KEY_A_PORT_OUT			GPIOA0	
		#define		CODING_KEY_A_PORT_BIT			6			

		#define 	CODING_KEY_B_PORT_OUT			GPIOA0	
		#define		CODING_KEY_B_PORT_BIT			7	
	#endif
												
//  #define FUNC_KEY_CPH_EN							    //�����������ֹ���
//  #define FUNC_KEY_DBCK_EN							//����˫��
//  #define FUNC_KEY_SCAN_DELAY_EN				  		//����ɨ����ʱ����

#endif

/////////////////////////////////////////////////////////////////////////////////////
//������⹦������
/////////////////////////////////////////////////////////////////////////////////////
#define FUNC_AUDIO_MENU_EN							//�������л�Ϊ��ͬ����

#ifdef FUNC_AUDIO_MENU_EN
	#define MENU_TIMEOUT 							500
#endif

/////////////////////////////////////////////////////////////////////////////////////
//PWM��������
/////////////////////////////////////////////////////////////////////////////////////
#define  FUNC_PWM_EN

#ifdef FUNC_PWM_EN
//PWM0 ����
	#define FUNC_PWM0_OUT_EN								//PWM0�꿪��
	#ifdef FUNC_PWM0_OUT_EN	
		#define		FUNC_PWM0_GPIO_PORT	GTC0_PWMPIN_GPIOA11	//���ֻʹ��0·PWM����������
		//PWM����������ã����ء�PWMʹ�ܡ������Ч״̬
		#define		FUNC_PWM0_CONFIGURE	(GTC_ControlSet_REPEAT|GTC_ControlSet_PWMEN|GTC_ControlSet_OUTST)
		//PWM�ж�����
		#define		FUNC_PWM0_INT_CONFIG	0
	#endif
	
//PWM1 ����
	#define FUNC_PWM1_OUT_EN								//PWM1�꿪��
	#ifdef FUNC_PWM1_OUT_EN
		#define		FUNC_PWM1_GPIO_PORT	GTC1_PWMPIN_GPIOA9	//���ֻʹ��1·PWM����������
		//PWM����������ã����ء�PWMʹ�ܡ������Ч״̬
		#define		FUNC_PWM1_CONFIGURE	(GTC_ControlSet_REPEAT|GTC_ControlSet_PWMEN|GTC_ControlSet_OUTST)
		//PWM�ж�����
		#define		FUNC_PWM1_INT_CONFIG	0
	#endif
	
//PWM2 ����
	#define FUNC_PWM2_OUT_EN								//PWM2�꿪��
	#ifdef FUNC_PWM2_OUT_EN	
		#define		FUNC_PWM2_GPIO_PORT	GTC2_PWMPIN_GPIOA7	//���ֻʹ��2·PWM����������
		//PWM����������ã����ء�PWMʹ�ܡ������Ч״̬
		#define		FUNC_PWM2_CONFIGURE	(GTC_ControlSet_REPEAT|GTC_ControlSet_PWMEN|GTC_ControlSet_OUTST)
		//PWM�ж�����
		#define		FUNC_PWM2_INT_CONFIG	0
	#endif

//PWM3 ����
//	#define FUNC_PWM3_OUT_EN								//PWM3�꿪��
	#ifdef FUNC_PWM3_OUT_EN	
		#define		FUNC_PWM3_GPIO_PORT	GTC0_PWMPIN_GPIOC3	//���ֻʹ��3·PWM����������
		//PWM����������ã����ء�PWMʹ�ܡ������Ч״̬
		#define		FUNC_PWM3_CONFIGURE	(GTC_ControlSet_REPEAT|GTC_ControlSet_PWMEN|GTC_ControlSet_OUTST)
		//PWM�ж�����
		#define		FUNC_PWM3_INT_CONFIG	0

	#endif
#endif

/////////////////////////////////////////////////////////////////////////////////////
#endif
#endif //_APP_CONFIG_H_


