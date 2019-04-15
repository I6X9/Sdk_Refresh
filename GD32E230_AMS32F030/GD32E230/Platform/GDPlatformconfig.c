/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : GDPlatformconfig.c
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��15��
  ����޸�   :
  ��������   : GD32E230оƬƽ̨�����ļ�
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
#include "app_config.h"
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
	/* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_14);

    /* connect port to USARTx_Rx */
    //gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_15);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_14);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_14);

    /* configure USART Rx as alternate function push-pull */
    //gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_15);
    //gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_15);
    
	/* USARTx configured as follow:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- Stop Bit = 1 Stop Bit
	- Parity = No Parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, Baudrate);
    //usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

	/* Enable USART */
	usart_enable(USART1);
	
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
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    
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


