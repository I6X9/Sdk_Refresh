/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : func_led.h
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��18��
  ����޸�   :
  ��������   : func_led.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��4��18��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __FUNC_LED_H__
#define __FUNC_LED_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "stm32f0xx_conf.h"

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
//ģ��궨��
	
#define MAX_PWM_SCAN_CHANNEL				3 
#define MAX_LED_SCAN_PIN					12

//ʵ��ʹ�ù��ܺ궨��
#define PWM_LED_SCAN_FREQ					10000
#define USE_LED_SCAN_PIN					12	
#define USE_PWM_SCAN_CHANNEL				1

#define MAX_DISP_DUTY						100
#define MIN_DISP_DUTY						5
#define USE_SCAN_RATIO						20
#define SCAN_LOOP_SPIN_NUM					5


//ɨ��GPIO���壬����Ӳ�����Ӷ���
#define LED_SCAN_GPIO_PORT1					GPIOB
#define LED_SCAN_GPIO_CLK1					RCC_AHBPeriph_GPIOB
#define LED_SCAN_GPIO_PIN1					GPIO_Pin_4

#define LED_SCAN_GPIO_PORT2					GPIOB
#define LED_SCAN_GPIO_CLK2					RCC_AHBPeriph_GPIOB
#define LED_SCAN_GPIO_PIN2					GPIO_Pin_5

#define LED_SCAN_GPIO_PORT3					GPIOB
#define LED_SCAN_GPIO_CLK3					RCC_AHBPeriph_GPIOB
#define LED_SCAN_GPIO_PIN3					GPIO_Pin_6

#define LED_SCAN_GPIO_PORT4					GPIOB
#define LED_SCAN_GPIO_CLK4					RCC_AHBPeriph_GPIOB
#define LED_SCAN_GPIO_PIN4					GPIO_Pin_7

#define LED_SCAN_GPIO_PORT5					GPIOA
#define LED_SCAN_GPIO_CLK5					RCC_AHBPeriph_GPIOB
#define LED_SCAN_GPIO_PIN5					GPIO_Pin_7

#define LED_SCAN_GPIO_PORT6					GPIOA
#define LED_SCAN_GPIO_CLK6					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN6					GPIO_Pin_6

#define LED_SCAN_GPIO_PORT7					GPIOA
#define LED_SCAN_GPIO_CLK7					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN7					GPIO_Pin_5

#define LED_SCAN_GPIO_PORT8					GPIOA
#define LED_SCAN_GPIO_CLK8					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN8					GPIO_Pin_4

#define LED_SCAN_GPIO_PORT9					GPIOA
#define LED_SCAN_GPIO_CLK9					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN9					GPIO_Pin_3

#define LED_SCAN_GPIO_PORT10				GPIOA
#define LED_SCAN_GPIO_CLK10					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN10					GPIO_Pin_11

#define LED_SCAN_GPIO_PORT11				GPIOA
#define LED_SCAN_GPIO_CLK11					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN11					GPIO_Pin_12

#define LED_SCAN_GPIO_PORT12				GPIOB
#define LED_SCAN_GPIO_CLK12					RCC_AHBPeriph_GPIOA
#define LED_SCAN_GPIO_PIN12					GPIO_Pin_3

//PWM���ƽӿڶ��壬����Ӳ���޸�
#define PWM_SCAN_CHANNEL_SEL1				PWM3_CH3_B0
#define PWM_SCAN_OUTIO_SEL1					PWM_IO_SEL0
#define PWM_SCAN_LEVEL_SEL1					PWM_OUTPUT_LOW

#define PWM_SCAN_CHANNEL_SEL2				PWM3_CH2_A7_B5
#define PWM_SCAN_OUTIO_SEL2					PWM_IO_SEL0
#define PWM_SCAN_LEVEL_SEL2					PWM_OUTPUT_LOW

#define PWM_SCAN_CHANNEL_SEL3				PWM3_CH1_A6_B4
#define PWM_SCAN_OUTIO_SEL3					PWM_IO_SEL0
#define PWM_SCAN_LEVEL_SEL3					PWM_OUTPUT_LOW

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

typedef enum _DISP_STATE_ {
	DISP_STATE_IDLE = 0,
	DISP_STATE_POWER_ON,
	DISP_STATE_SINGLE_COLOR,
	DISP_STATE_CUR_BLINK,
	DISP_STATE_FULL_COLOR,
	DISP_STATE_LOOP_CHANGE,
	DISP_STATE_LOOP_SPIN,
	DISP_STATE_MAX,
}DISP_STATE_TYPEDEF;

typedef enum _DISP_COLOR_ {
	DISP_COLOR_OFF = 0,
	DISP_COLOR_RED,
	DISP_COLOR_ORANGE,
	DISP_COLOR_YELLOW,
	DISP_COLOR_GREEN,
	DISP_COLOR_CYAN,
	DISP_COLOR_BLUE,
	DISP_COLOR_PURPLE,
	DISP_COLOR_WHITE,
	DISP_COLOR_MAX,
}DISP_COLOR_TYPEDEF;

extern void Led_Display(void);
extern void Led_DispInit(void);
extern void Led_DispScan(void);
extern void Led_DispStateSet(DISP_STATE_TYPEDEF State, DISP_COLOR_TYPEDEF Color, uint32_t Timer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __FUNC_LED_H__ */
