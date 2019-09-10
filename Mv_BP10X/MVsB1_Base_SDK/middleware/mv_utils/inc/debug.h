////////////////////////////////////////////////////////////////////////////////
//                   Mountain View Silicon Tech. Inc.
//		Copyright 2011, Mountain View Silicon Tech. Inc., ShangHai, China
//                   All rights reserved.
//
//		Filename	:debug.h
//
//		Description	:
//					Define debug ordinary print & debug routine
//
//		Changelog	:
///////////////////////////////////////////////////////////////////////////////

#ifndef __DEBUG_H__
#define __DEBUG_H__

/**
 * @addtogroup mv_utils
 * @{
 * @defgroup debug debug.h
 * @{
 */
 
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include "type.h"
#include "gpio.h"
#define	DBG(format, ...)			printf(format, ##__VA_ARGS__)

#define	APP_DBG(format, ...)		printf(format, ##__VA_ARGS__)

#define	OTG_DBG(format, ...)		//printf(format, ##__VA_ARGS__)

#define	BT_DBG(format, ...)			printf(format, ##__VA_ARGS__)//do{printf("[BT] "); printf(format, ##__VA_ARGS__);}while(0)

int DbgUartInit(int Which, unsigned int BaudRate, unsigned char DatumBits, unsigned char Parity, unsigned char StopBits);



#define PORT_BANK_A				(GPIO_A_START)
#define PORT_BANK_B				(GPIO_B_START)
#define PORT_IN_REG(X)		(GPIO_A_IN - GPIO_A_START + X)
#define PORT_OUT_REG(X)		(GPIO_A_OUT - GPIO_A_START + X)
#define PORT_SET_REG(X)		(GPIO_A_SET - GPIO_A_START + X)
#define PORT_CLR_REG(X)		(GPIO_A_CLR - GPIO_A_START + X)
#define PORT_TGL_REG(X)		(GPIO_A_TGL - GPIO_A_START + X)
#define PORT_IE_REG(X)		(GPIO_A_IE - GPIO_A_START + X)
#define PORT_OE_REG(X)		(GPIO_A_OE - GPIO_A_START + X)
#define PORT_DS_REG(X)		(GPIO_A_DS - GPIO_A_START + X)
#define PORT_PU_REG(X)		(GPIO_A_PU - GPIO_A_START + X)
#define PORT_PD_REG(X)		(GPIO_A_PD - GPIO_A_START + X)
#define PORT_ANA_REG(X)		(GPIO_A_ANA_EN - GPIO_A_START + X)
#define PORT_PULLDOWN_REG(X)	(GPIO_A_PULLDOWN - GPIO_A_START + X)
#define PORT_CORE_REG(X)	(GPIO_A_CORE_OUT_MASK - GPIO_A_START + X)
#define PORT_DMA_REG(X)		(GPIO_A_DMA_OUT_MASK - GPIO_A_START + X)


//����IO,�����ڳ���ִ��ʱ����ӻ�����׼SDK��ʹ��
//����ߵ͵�ƽ�������أ��½��أ���ת��led�ȣ�1~4�˿������ã������Σ�led��������Ч��ƽ��
//����LedPortInit��ʼ���˿����á�
#define LED_ON_LEVEL	0//�����ߵ͵�ƽ �趨��
/**������Ҫ������Ӧ�˿ں����**/
//#define LED1_PORT			PORT_BANK_A		//ע�ʹ��У��ɹر�LED1���ж˿ڲ���
#define LED1_INDEX		GPIO_INDEX7
#define LED2_PORT			PORT_BANK_A		//ע�ʹ��У��ɹر�LED2
#define LED2_INDEX		GPIO_INDEX8
#define LED3_PORT			PORT_BANK_A		//ע�ʹ��У��ɹر�LED3
#define LED3_INDEX		GPIO_INDEX9
#define LED4_PORT			PORT_BANK_A		//ע�ʹ��У��ɹر�LED4
#define LED4_INDEX		GPIO_INDEX10

void LedPortInit(void);
void LedOn(uint8_t Index);//1~4 �Ƿ���Чȡ����LEDx_PORT�Ƿ���
void LedOff(uint8_t Index);
void LedToggle(uint8_t Index);
void LedPortRise(uint8_t Index);
void LedPortDown(uint8_t Index);
bool LedPortGet(uint8_t Index);//TRUE:�ߵ�ƽ��FALSE:�͵�ƽ��index��Чʱ Ĭ��FALSE

#ifdef __cplusplus
}
#endif//__cplusplus

/**
 * @}
 * @}
 */
 
#endif //__DBG_H__ 

