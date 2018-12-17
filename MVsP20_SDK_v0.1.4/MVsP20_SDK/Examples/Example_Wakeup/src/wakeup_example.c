#include <stdio.h>
#include <nds32_intrinsic.h>
#include "cache_tcm.h"
#include "gpio.h"
#include "uarts.h"
#include "uarts_interface.h"
#include "type.h"
#include "debug.h"
#include "spi_flash.h"
#include "timeout.h"
#include "clk.h"
#include "pwm.h"
#include "sleep.h"
#include "delay.h"
#include "rtc.h"
#include "irqn.h"
#include "watchdog.h"


#define GPIO_WAKEUP  1
#define RTC_WAKEUP   0
#define MIX_WAKEUP   0


//wakeup�ж���Ӧ�������ڵ���NVIC_EnableIRQ(Wakeup_IRQn)��Sleep_EnWakeUpInt()�Լ�ȫ���ж�ʹ�ܿ�������ж����̱���Ӧ
__attribute__((section(".driver.isr")))void WakeupInterrupt(void)
{
	uint32_t flag1, flag2,flag3;

	Sleep_WakeUpSrcFlagGet(&flag1, &flag2,&flag3);
	//SREG_WAKEUP_INT_CTRL.WKUP_INT_EN = 0;
	DBG("\n[Interrupt]WakeUpFlags:FLag0 %08X, Flag1 %08X, Flag2 %08X\n ", flag1, flag2, flag3);
	WakeUpFlagClear();
	Sleep_WakeUpSrcFlagGet(&flag1, &flag2,&flag3);
	WaitMs(1);
	DBG("[Interrupt]after clear WakeUpFlag:FLag0 %08X, Flag1 %08X, Flag2 %08X \n", flag1, flag2, flag3);

}

int main()
{

	 uint32_t  flag1,flag2,flag3;


	 //Disable watchDog
	 WDG_Disable();

	 CacheTcmInitFast(PHYMEM_16KBPM0 /*cache*/, PHYMEM_16KBPM1/*tcm r0*/, 0/*tcm r0 offset*/, 4/*tcm r0 size*/, PHYMEM_NONE/*tcm r1*/, 0/*tcm r1 offset*/, 0/*tcm r1 size*/);

	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB30);
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB30);
	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB31);
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB31);
	Clock_Config(1, 12000000);
	Clock_PllLock(480000);
	Clock_SysClkSelect(PLL_CLK_MODE);
	Clock_UARTClkSelect(1);

    __nds32__mtsr(1, NDS32_SR_INT_PEND2);
    LP_EfuseReadDisable();
    Clock_DynamicClockGateEnable();

	SpiFlashInit(80000000, MODE_4BIT, 0, 1);

	/********************GPIOC3---->RXD***************/
	/********************GPIOC2---->TXD***************/
	GPIO_UartTxIoConfig(0, 1);
	GPIO_UartRxIoConfig(0, 1);

	UARTS_Init(0,  115200, 8,  0,  1);

	GIE_ENABLE();



	DBG("****************************************************************\n");
	DBG("               P2A CHIP Wakeup Module TEST 2016 MVSilicon       \n");
	DBG("****************************************************************\n");



	Sleep_WakeUpSrcFlagGet(&flag1, &flag2,&flag3);
	DBG("[main]WakeUpFlag:FLag0 %08X, Flag1 %08X, Flag2 %08X\n", flag1, flag2,flag3);
	WakeUpFlagClear();

	WaitMs(1000);//��ͣ1000ms������δ��ȷ���û���Դ����Sleep״̬ͨ���ϵ縴λ�Ĳ�������JLINK��������¼

	Sleep_WakeUpSrcFlagGet(&flag1, &flag2,&flag3);
	DBG("[main]after clear WakeUpFlag:FLag0 %08X, Flag1 %08X,Flag2 %08X\n", flag1, flag2,flag3);

	NVIC_EnableIRQ(Wakeup_IRQn);
	Sleep_WakeUpIntEnable();

#if GPIO_WAKEUP
	//����GPIOC0/C1��Ϊ����Դ��C0�½��ػ���,C1�����ػ���
	Sleep_WakeUpSrcSet(0,     0,        WAKEUP_SRC2_SLEEP_C0|WAKEUP_SRC2_SLEEP_C1,\
				       0,     0,        (~WAKEUP_POLAR3_C0_HI)|(WAKEUP_POLAR3_C0_HI),\
				       0);//Ϊ0�����õ�ƽ���
	//Sleep_WakeUpTrgTimeSet(5);��������˵�ƽ��ȣ��˺�����Ҫ�����ã������ֵ��Ҫ����15
#endif


#if RTC_WAKEUP
	//����RTCΪ����Դ�����ú���Ҫ����RTC������ʱ��
	Sleep_WakeUpSrcSet(  0,     0,        WAKEUP_SRC2_SLEEP_RTC,\
					   0,     0,        0,\
					   0);
	SetNSecondsAlarm(5);

#endif

#if MIX_WAKEUP
	Sleep_WakeUpSrcSet(  0,     0,        WAKEUP_SRC2_SLEEP_C0 | WAKEUP_SRC2_SLEEP_C1 | WAKEUP_SRC2_SLEEP_RTC,\
					   0,     0,        0,\
					   0);
	SetNSecondsAlarm(5);
#endif

	//Sleep_WkUpTrgTime(5);
	DBG("MCU go to sleep now\n");

	GIE_DISABLE();
    Clock_SysClkSelect(RC_CLK_MODE);
    Clock_PllClose();
    Clock_OSC12MDisable();

    Sleep_GoToSleep();

	Clock_Config(1, 12000000);
	Clock_PllLock(480000);
	Clock_SysClkSelect(PLL_CLK_MODE);
	GIE_ENABLE();

	Sleep_WakeUpSrcFlagGet(&flag1, &flag2,&flag3);

	DBG("[main]after clear WakeUpFlag:FLag0 %08X, Flag1 %08X,Flag2 %08X\n", flag1, flag2,flag3);
	WakeUpFlagClear();

	while(1){

	}

}

