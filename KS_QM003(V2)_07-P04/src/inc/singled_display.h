/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : singled_display.h
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��1��23��
  ����޸�   :
  ��������   : singled_display.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��1��23��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "pwm.h"

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

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

#ifndef __SINGLED_DISPLAY_H__
#define __SINGLED_DISPLAY_H__

#define SINGLE_LED_DISP_SCAN_CNT		50
#define SINGLE_LED_MAX_DUTY				100
#define SINGLE_LED_MIN_DUTY				0

//LED��ʾģʽ
typedef enum _LED_MODE_TYPE_
{
	LED_DISPLAY_MODE_CHARGING = 0,	    //��ʾ�����
	LED_DISPLAY_MODE_LOWBATTER,	    	//��ʾ�͵�
	LED_DISPLAY_MODE_NIGHTLAMP,	    	//��ʾСҹ��
	LED_DISPLAY_MODE_WPSCONNECT,	    //��ʾ������
	LED_DISPLAY_MODE_TLAK_ON,	    	//��ʾ¼����
	LED_DISPLAY_MODE_POWER_ON,	    	//��ʾ������
	LED_DISPLAY_MODE_ALARM_CLOCK,	    //��ʾ����������
	LED_DISPLAY_MODE_BREATHING,			//��ʾ������
	LED_DISPLAY_MODE_OFF,				//�ر���ʾ��
}LED_MODE_TYPE;

typedef enum _LED_DISPLAY_STATUS_
{
	LED_DISPLAY_ONCE = 0,
	LED_DISPLAY_LOOP,
	LED_DISPLAY_KEEP
}LED_DISPLAY_STATUS;

typedef struct SINGLE_LED_DISP
{
	uint8_t CurDisplayMode;				//��ǰ��ʾģʽ
    uint8_t BledFlag;					//����ÿ��bit����LED��˸��������
    uint8_t RledFlag;					//����ÿ��bit����LED��˸��������
    uint8_t BledDuty;					//LED��ǰʵʱռ�ձ�
    uint8_t RledDuty;					//LED��ǰʵʱռ�ձ�
    uint8_t BledStepUpDuty;				//ɨ��ռ�ձȲ������������ʵ�ֵƹ����/��˸��Ч��
    uint8_t RledStepUpDuty;					//ɨ��ռ�ձȲ������������ʵ�ֵƹ����/��˸��Ч��
    uint8_t DispBitCnt;					//��ǰBITλָ��
    uint32_t DispChangeTime;			//ÿ����һ�����ȵȼ���ʱ�䣬��λ��ms
}SINGLE_LED_DISP_STRU;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern void LedFlushDisp(void);
extern void SingleLedFlushDispInit(void);
extern void SingleLedDisplayModeSet(uint8_t DisplayMode, bool IsOnOff);
extern bool GetSingleLedDispMode(LED_MODE_TYPE Mode);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SINGLED_DISPLAY_H__ */
