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
#include <stdlib.h>
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
void Usart_Debug_Init(uint32_t Baudrate)	{
	/* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_15);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_15);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_15);
    
	/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, Baudrate);
    //usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_invert_config(USART1, USART_SWAP_ENABLE);
    usart_interrupt_enable(USART1, USART_INT_TC);
	/* Enable USART */
	usart_enable(USART1);

	/*nvic interrupt enable*/
	nvic_irq_enable(USART1_IRQn, 3);
	
	printf("Usart debug init OK!\n");
}

bool Gd_Usart_Debug_Busy(int Word) {
	if ((SET == usart_flag_get(USART1, USART_FLAG_TBE)) && (FALSE == GetDebugDataLen())) {
    	usart_data_transmit(USART1, (uint32_t)Word);
    	return FALSE;
    }
	return TRUE;
}

/*****************************************************************************
 �� �� ��  : System_DetectInit
 ��������  : ϵͳ��������������ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��5��5��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void System_DetectInit(void) {
	printf("System is ");

	if (SET == rcu_flag_get(RCU_FLAG_LPRST)) {
		printf("LOW_POWER_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_PORRST)) {
		printf("POR_POWER_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_WWDGTRST)) {
		printf("WINDOWS_DOG_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_FWDGTRST)) {
		printf("FREE_DOG_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_EPRST)) {
		printf("EXTERNAL_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_SWRST)) {
		printf("SOFT_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_OBLRST)) {
		printf("OPTION_BYTE_RESET;\n");
	}
	else if (SET == rcu_flag_get(RCU_FLAG_V12RST)) {
		printf("VDD12_POWER_RESET;\n");
	}

	rcu_all_reset_flag_clear();
}
/*****************************************************************************
 �� �� ��  : Bsp_SystemInit
 ��������  : оƬƽ̨��ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��5��7��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void Bsp_SystemInit(void) {
	//USART��ʼ����115200������
	Usart_Debug_Init(115200);
	//ϵͳ�δ�ʱ����ʼ������λuS
	SysTick_Config(SystemCoreClock/1000);
	System_DetectInit();
}
/*****************************************************************************
 �� �� ��  : Bsp_FuncRuning
 ��������  : ���幦���߼�������������ѭ������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��5��7��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void Bsp_FuncRuning(void) {
	static uint32_t MemSize = 0;
	uint8_t* MemPoint = NULL;

	MemSize++;
	
	//MemPoint = malloc(MemSize);
	if (NULL == MemPoint) {
		printf ("malloc failed %d;\n", MemSize);
		while (1);
	}

	//free(MemPoint);
}


