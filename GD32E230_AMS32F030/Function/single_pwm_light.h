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
#ifndef __FUNC_LIGHT_H__
#define __FUNC_LIGHT_H__

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
	
#define MAX_LIGHT_PWM_CHANNEL				3 

//ʵ��ʹ�ù��ܺ궨��
#define PWM_LIGHT_SCAN_FREQ					500
#define USE_LIGHT_SCAN_RATIO				20
#define USE_LIGHT_SCAN_CHANNEL				3 
#define PWM_LIGHT_MAX_DUTY					60
#define PWM_LIGHT_MIN_DUTY					(PWM_LIGHT_MAX_DUTY/USE_LIGHT_SCAN_RATIO)

//PWM���ƽӿڶ��壬����Ӳ���޸�
#define PWM_LIGHT_CHANNEL_RED				PWM1_CH3_A10
#define PWM_LIGHT_OUTIO_RED					PWM_IO_SEL0
#define PWM_LIGHT_LEVEL_RED					PWM_OUTPUT_HIGH

#define PWM_LIGHT_CHANNEL_GREEN				PWM1_CH2_A9
#define PWM_LIGHT_OUTIO_GREEN				PWM_IO_SEL0
#define PWM_LIGHT_LEVEL_GREEN				PWM_OUTPUT_HIGH

#define PWM_LIGHT_CHANNEL_BLUE				PWM1_CH1_A8
#define PWM_LIGHT_OUTIO_BLUE				PWM_IO_SEL0
#define PWM_LIGHT_LEVEL_BLUE				PWM_OUTPUT_HIGH

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

extern void Light_Display(void);
extern void Light_DispInit(void);
extern void Light_DispScan(void);
extern void Light_DispStateSet(DISP_STATE_TYPEDEF State, DISP_COLOR_TYPEDEF Color, uint32_t Timer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __FUNC_LED_H__ */
