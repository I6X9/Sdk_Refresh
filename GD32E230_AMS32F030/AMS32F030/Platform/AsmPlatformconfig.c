/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : AsmPlatformconfig.c
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��15��
  ����޸�   :
  ��������   : ASM32оƬƽ̨�����ļ�
  �����б�   :
              fputc
              Usart_Debug_Init
  �޸���ʷ   :
  1.��    ��   : 2019��4��15��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include "stm32f0xx_conf.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/
extern volatile uint32_t CORET_Systick_Count;			//CORET Systick count
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

/*****************************************************************************
 �� �� ��  : Usart_Debug_Init
 ��������  : ���ڴ�ӡ��ʼ��
 �������  : uint32_t Baudrate  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��15��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void Usart_Debug_Init(uint32_t Baudrate){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);

	/* Connect PXx to USARTx_Rx */
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_pin_15;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configured as follow:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- Stop Bit = 1 Stop Bit
	- Parity = No Parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	//USART_DeInit(USART1);
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	printf("Usart debug init OK!\n");
}
/*****************************************************************************
 �� �� ��  : fputc
 ��������  : ��ӡ����ض���USART TX
 �������  : int ch   
             FILE *f  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��15��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint16_t)ch);
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));

    return ch;
}
/*****************************************************************************
 �� �� ��  : System_Systick_Init
 ��������  : ϵͳ�δ�ʱ����ʼ������
 �������  : uint32_t ticks  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��15��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void System_Systick_Init(uint32_t ticks)
{
	if (SysTick_Config(SystemCoreClock / ticks)){
	/* capture error */
		printf("SysTick_Config ERRO!!!\n");
		return;
	}
	/* configure the systick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x00U);
	printf("SysTick_Config %d uS OK.\n", ticks);
}


