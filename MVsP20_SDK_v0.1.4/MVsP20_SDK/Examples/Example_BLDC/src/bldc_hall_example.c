/**
 **************************************************************************************
 * @file    BLDCHallExample.c
 * @brief   BLDC Hall operation example
 * 
 * @author  Grayson
 * @version V1.0.0 	initial release
 * 
 * $Id$
 * $Created: 2016-11-28 17:31:10$
 *
 * &copy; Shanghai Mountain View Silicon Technology Co.,Ltd. All rights reserved.
 **************************************************************************************
 */


#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "uarts.h"
#include "uarts_interface.h"
#include "clk.h"
#include "gpio.h"
#include "timeout.h"
#include "watchdog.h"
#include "debug.h"
#include "spi_flash.h"
#include "timeout.h"
#include "cache_tcm.h"
#include "timer.h"
#include "pwm.h"
#include "pwc.h"
#include "irqn.h"
#include "spi_flash.h"



#define APP_DBG          DBG              //������Ϣ���
#define MOTOR_NUM        1                //1=���1��2=���2


#if   (MOTOR_NUM == 1)
#define PWC_TIM_IDX      TIMER5           //PWC�Ķ�ʱ����ţ�����HALL����
#define PWC_IRQ_IDX      TMR5_IRQn        //PWC�жϺ�
#define PWM_TIM_IDX      TIMER6           //PWM�Ķ�ʱ�����
#define PWM_IRQ_IDX      TMR6_IRQn        //PWM���жϺ�

#else
#define PWC_TIM_IDX      TIMER7           //PWC�Ķ�ʱ����ţ�����HALL����
#define PWC_IRQ_IDX      TMR7_IRQn        //PWC�жϺ�
#define PWM_TIM_IDX      TIMER8           //PWM�Ķ�ʱ�����
#define PWM_IRQ_IDX      TMR8_IRQn        //PWM���жϺ�

#endif



#define SYS_CLK          120000000        //ϵͳ��ʱ��120Mhz

#define PWM_CARRIER_FRQ  16000            //PWM��Ƶ 16Khz
#define PWC_CLK          1000000		  //PWCʱ��Ƶ�� 1M



uint8_t g_ucRun = 0;//1=���У�0=ͣ��
uint8_t g_ucFault = 0;//1=�й��ϣ�0=�޹���
uint8_t g_ucPWMDuty = 0;//ռ�ձ�:0~100%


static uint8_t GetHallVal(void)//��HALLֵ
{
#if   (MOTOR_NUM == 1)
	return ((GPIO_RegGet(GPIO_A_IN) >> 16) & 0x07);
#else
	return ((GPIO_RegGet(GPIO_C_IN) >> 4) & 0x07);
#endif
}
static bool  IsRSStatus_Run(void)
{
	return g_ucRun;
}
static void  SetRSStatus_Run(bool val)
{
	g_ucRun = val;
}
static bool  IsRSStatus_Fault(void)
{
	return g_ucFault;
}
static void  SetRSStatus_Fault(bool val)
{
	g_ucFault = val;
}
static void  SetPWM_Duty(bool val)
{
	g_ucPWMDuty = val;
}
static uint8_t GetPWM_Duty(void)
{
	return g_ucPWMDuty;
}
/*********************************************************************************************************
** Func name:               PWM_GpioConfigInit()
** Param:                   TimerIdx---PWM��ʱ������
**                          PWMIdx---PWM�˿�����,PWM1~3
** Return:
** Description:             PWM��GPIO����
*********************************************************************************************************/
static void PWM_GpioConfigInit(TIMER_INDEX TimerIdx, PWM_INDEX PWMIdx)
{
    if(TimerIdx == TIMER6)
    {
    	APP_DBG("PWM TIMER6 GPIO CONFIG:\n");
        if(PWMIdx == PWM1)
        {
            PWM_GpioConfig(TIMER6_PWM1_N_B5, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER6_PWM1_P_B6, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM1 OUTPUT: B5_N, B6_P\n");
        }

        if(PWMIdx == PWM2)
        {
            PWM_GpioConfig(TIMER6_PWM2_N_B3, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER6_PWM2_P_B4, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM2 OUTPUT: B3_N, B4_P\n");
        }

        if(PWMIdx == PWM3)
        {
            PWM_GpioConfig(TIMER6_PWM3_N_B1, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER6_PWM3_P_B2, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM3 OUTPUT: B1_N, B2_P\n");
			PWM_BreakGpioConfig(TIMER6_BREAK_B0,1);//BREAK IN ��IO����
			APP_DBG("BREAK IN: B0\n");
        }
    }
    if(TimerIdx == TIMER8)
    {
    	APP_DBG("PWM TIMER8 GPIO CONFIG:\n");
        if(PWMIdx == PWM1)
        {
            PWM_GpioConfig(TIMER8_PWM1_N_B8, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER8_PWM1_P_B9, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM1 OUTPUT: B8_N, B9_P\n");
        }

        if(PWMIdx == PWM2)
        {
            PWM_GpioConfig(TIMER8_PWM2_N_B10, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER8_PWM2_P_B11, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM2 OUTPUT: B10_N, B11_P\n");
        }

        if(PWMIdx == PWM3)
        {
            PWM_GpioConfig(TIMER8_PWM3_N_B12, 0, PWM_IO_MODE_OUT);
            PWM_GpioConfig(TIMER8_PWM3_P_B13, 0, PWM_IO_MODE_OUT);
            APP_DBG("PWM3 OUTPUT: B12_N, B13_P\n");
			PWM_BreakGpioConfig(TIMER8_BREAK_B7,1);//BREAK IN ��IO����
			APP_DBG("BREAK IN: B7\n");
        }
    }


}
/*********************************************************************************************************
** Func name:               PWC_GpioConfigInit()
** Param:                   TimerIdx---PWM��ʱ������
**                          PwcIdx---PWC������PWC1~3
** Return:
** Description:             PWC��GPIO����
*********************************************************************************************************/
static void PWC_GpioConfigInit(TIMER_INDEX TimerIdx, PWC_INDEX PwcIdx)
{
    if(TimerIdx == TIMER5)
    {
        if(PwcIdx == PWC1)
        {
            PWC_GpioConfig(TIMER5_PWC1_A16, 0, 1);
            APP_DBG("PWC1 INPUT: A16\n");
        }

        if(PwcIdx == PWC2)
        {
            PWC_GpioConfig(TIMER5_PWC2_A17, 0, 1);
            APP_DBG("PWC2 INPUT: A17\n");
        }

        if(PwcIdx == PWC3)
        {
            PWC_GpioConfig(TIMER5_PWC3_A18, 0, 1);
            APP_DBG("PWC3 INPUT: A18\n");
        }
    }
    if(TimerIdx == TIMER7)
    {
        if(PwcIdx == PWC1)
        {
            PWC_GpioConfig(TIMER7_PWC1_C4, 0, 1);
            APP_DBG("PWC1 INPUT: C4\n");
        }

        if(PwcIdx == PWC2)
        {
            PWC_GpioConfig(TIMER7_PWC2_C5, 0, 1);
            APP_DBG("PWC2 INPUT: C5\n");
        }

        if(PwcIdx == PWC3)
        {
            PWC_GpioConfig(TIMER7_PWC3_C6, 0, 1);
            APP_DBG("PWC3 INPUT: C6\n");
        }
    }
}
/*********************************************************************************************************
** Func name:               PWM_OutPutEnable()
** Param:                   TimerIdx---PWM��ʱ������
**                          blEnable---0:�����ֹ;1:���ʹ��
** Return:
** Description:             PWM����ź�ʹ��
*********************************************************************************************************/
void PWM_OutPutEnable(TIMER_INDEX  TimerIdx,bool blEnable)
{
	PWM_IOCTRL_ARG	Arg;

	if(blEnable)//��PWM
	{
		Arg.OutEnable = OUTP_AND_OUTN_ENABLE;//������
	}
	else//��PWM
	{
		Arg.OutEnable = OUTP_AND_OUTN_DISABLE;//�����
	}
	PWM_IOCTRl(TimerIdx, PWM1, OUTPUT_ENABLE_UPDATE, &Arg);
	PWM_IOCTRl(TimerIdx, PWM2, OUTPUT_ENABLE_UPDATE, &Arg);
	PWM_IOCTRl(TimerIdx, PWM3, OUTPUT_ENABLE_UPDATE, &Arg);
}
/*********************************************************************************************************
** Func name:               PWM_DutySet()
** Param:                   TimerIdx---PWM��ʱ������
**                          usDutyA��usDutyB��usDutyC---PWM1~3ռ�ձȣ�0%~100%
** Return:
** Description:             PWMռ�ձ�����
*********************************************************************************************************/
void PWM_DutySet(TIMER_INDEX  TimerIdx,uint16_t usDutyA,uint16_t usDutyB,uint16_t usDutyC)
{
	PWM_IOCTRL_ARG	Arg;

	Arg.Duty = usDutyA;
	PWM_IOCTRl(TimerIdx, PWM1, OUTPUT_DUTY_UPDATE, &Arg);
	Arg.Duty = usDutyB;
	PWM_IOCTRl(TimerIdx, PWM2, OUTPUT_DUTY_UPDATE, &Arg);
	Arg.Duty = usDutyC;
	PWM_IOCTRl(TimerIdx, PWM3, OUTPUT_DUTY_UPDATE, &Arg);
}
/*********************************************************************************************************
** Func name:               PWC_TimerConfig()
** Param:
**
** Return:
** Description:             PWC��ʱ������
*********************************************************************************************************/
void PWC_TimerConfig(void)
{
	TIMER_INDEX     TimerIdx;
	PWC_StructInit  PWCParam;

	TimerIdx = PWC_TIM_IDX;//

	//PWC����GPIO����
	PWC_GpioConfigInit(TimerIdx, PWC1);
	PWC_GpioConfigInit(TimerIdx, PWC2);
	PWC_GpioConfigInit(TimerIdx, PWC3);

	//PWC��������
	memset(&PWCParam,0,sizeof(PWC_StructInit));
	Timer_InterrputSrcEnable(TimerIdx,PWC1_CAP_DATA_INTERRUPT_SRC /*| PWC_OVER_RUN_INTERRUPT_SRC*/);//����PWC�жϼ�����ж�
	PWCParam.Polarity[PWC1]      = PWC_POLARITY_BOTH;
	PWCParam.SingleGet           = PWC_CAPTURE_CONTINUES;
	PWCParam.DMAReqEnable        = PWC_REQ_INTERRUPT_MODE;
	PWCParam.TimeScale           = SYS_CLK / PWC_CLK;//��Ƶ��1M
	PWCParam.FilterTime          = 30;
	PWCParam.PWC1Source          = PWC1_INPUT_PWC1_XOR_PWC2PWC3;// PWC1�� PWC2�� PWC3�������Ϊ����Դ
	PWC_Config(TimerIdx, &PWCParam,PWC_INPUT_CHANNEL_PWC1);

	Timer_InputTriggerSelect(TimerIdx, NONE_INTPUT_TRIGGER);
	PWC_Enable(TimerIdx);
	NVIC_SetPriority(PWC_IRQ_IDX,1);//priority=0,1,2,3 0=highest 3=lowest
	NVIC_EnableIRQ(PWC_IRQ_IDX);
}
/*********************************************************************************************************
** Func name:               PWM_TimerConfig()
** Param:
**
** Return:
** Description:             PWM��ʱ������
*********************************************************************************************************/
void PWM_TimerConfig(void)
{
    TIMER_INDEX  TimerIdx;
    PWM_INDEX  PWMIndex;
    PWM_StructInit PWMParam;
    BDTInfo BDTParam;

	TimerIdx = PWM_TIM_IDX;//��ʱ������

	for(PWMIndex = PWM1;PWMIndex <= PWM3;PWMIndex++)//
	{
		PWM_GpioConfigInit(TimerIdx, PWMIndex);//GPIO����

		//PWM��������
		PWMParam.CounterMode   			= PWM_COUNTER_MODE_UP;
		PWMParam.OutputType    			= PWM_OUTPUT_SINGLE_1;
		PWMParam.FreqDiv       			= SYS_CLK / (PWM_CARRIER_FRQ);//
		PWMParam.DMAReqEnable  			= PWM_REQ_INTERRUPT_MODE;
		PWMParam.Duty[PWMIndex]			= 0;
		PWMParam.OutPolarity[PWMIndex]  = 0;//0 - ����˺͸���˾�Ϊ�ߵ�ƽ��Ч   1 - ����Ϊ�͵�ƽ��Ч���͵�ƽΪ�ߵ�ƽ��Ч   2 - �����Ϊ�ߵ�ƽ��Ч�������Ϊ�͵�ƽ��Ч  3 - ����˺͸���˾�Ϊ�͵�ƽΪ��Ч��ƽ
		PWMParam.OutIdleState[PWMIndex]	= IDLE_P_LOW_AND_N_LOW; //idle״̬ʱ��P������͵�ƽ��N������͵�ƽ
		//��ʼ��ʱ��PWM������Ϊ�ر�״̬
		PWMParam.OutEnable[PWMIndex]    = 0; //0 - ����� 1 - ֻ�����ֶ˵������  2 - ֻ�����ֶ˵ĸ����  3 -- ������
		PWM_Config(TimerIdx, PWMIndex, &PWMParam);//PWM��������
	}


	//����ʱ�䡢BREAK IN ��������
    BDTParam.DTFreqDiv     = SYS_CLK_DIV_4;//4��Ƶ
    BDTParam.DTCycle       = 20*(SYS_CLK/10000000/4);//����ʱ�䣺20 = 2.0us
    PWM_DeadTimeConfig(TimerIdx, &BDTParam);
    PWM_DeadTimeEnable(TimerIdx);

	Timer_InterrputSrcEnable(TimerIdx,PWM_BREAK_IN_INTERRUPT_SRC/*|UPDATE_INTERRUPT_SRC*/);//�ж�Դbreak in
	Timer_InterruptFlagClear(TimerIdx,PWM_BREAK_IN_INTERRUPT_SRC);
	BDTParam.BreakPolar    = BREAK_POLARITY_LOW;//break in�͵�ƽ��Ч
	BDTParam.FilterTime    = 3;//break in �˲�ʱ��
	BDTParam.AutoExitIdleEnable = 1;//1��ͨ�����update���Զ��˳�break in״̬
	BDTParam.BreakOffStateSel = 1;  //1���Ƚ�����Ч��ƽ״̬��Ȼ��������ʱ��֮�����IDLE״̬
	PWM_BreakInConfig(TimerIdx, &BDTParam);
	PWM_BreakInEnable(TimerIdx);
	PWM_Enable(TimerIdx);//PWM��ʱ��ʹ��

	NVIC_SetPriority(PWM_IRQ_IDX,0);//�ж����ȼ�=0,1,2,3;0=highest,3=lowest
	NVIC_EnableIRQ(PWM_IRQ_IDX);
}

/*********************************************************************************************************
** Func name:               PWM_3PhaseSet()
** Param:                   TimerIdx---��ʱ��������,��TIMER5
** 							PWMx_P---PWM��P�����������ˣ�,������ΪPWM1�����ʾPWM1��P�����
**                          PWMx_P---PWM��N�����������ˣ�
**                          PWMx_Off---PWM�ر�
** Return:
** Description:             ������PWM��ͨ����
*********************************************************************************************************/
void PWM_3PhaseSet(TIMER_INDEX TimerIdx,uint8_t PWMx_P,uint8_t PWMx_N,uint8_t PWMx_Off)
{
	PWM_IOCTRL_ARG	Arg;

	Arg.OutputType = PWM_OUTPUT_SINGLE_1;
	Arg.OutEnable  = OUTP_ENABLE_AND_OUTN_DISABLE;//ֻ��������
	PWM_IOCTRl(TimerIdx, PWMx_P, OUTPUT_TYPE_UPDATE | OUTPUT_ENABLE_UPDATE, &Arg);

	Arg.OutputType = PWM_OUTPUT_SINGLE_FORCE_NOACTIVE;
	Arg.OutEnable  = OUTP_AND_OUTN_ENABLE;//���,�¹ܳ���
	PWM_IOCTRl(TimerIdx, PWMx_N, OUTPUT_TYPE_UPDATE | OUTPUT_ENABLE_UPDATE, &Arg);

	Arg.OutputType = PWM_OUTPUT_SINGLE_1;
	Arg.OutEnable  = OUTP_AND_OUTN_DISABLE;//�����
	PWM_IOCTRl(TimerIdx, PWMx_Off, OUTPUT_TYPE_UPDATE | OUTPUT_ENABLE_UPDATE, &Arg);
}
/*********************************************************************************************************
** Func name:               PWM_HallPhaseChange()
** Param:                   TimerIdx---PWM��ʱ������
**                          ucHall---����״ֵ̬
** Return:
** Description:             HALL����
**
** A��B��C���࣬��+����ʾ���űۿ�����-����ʾ���űۿ�����Z����ʾ�����ű۾��ر�
**     ---------+---+---+---+---+---+---+
**     Phase A  | + | z | - | - | z | + |
**     Phase B  | - | - | z | + | + | z |
**     Phase C  | Z | + | + | Z | - | - |
**     ---------+---+---+---+---+---+---+
**     Hall A   | 1 | 0 | 0 | 0 | 1 | 1 |
**     Hall B   | 0 | 0 | 1 | 1 | 1 | 0 |
**     Hall C   | 1 | 1 | 1 | 0 | 0 | 0 |
**     ---------+---+---+---+---+---+---+
**     0xCBA----+-5-+-4-+-6-+-2-+-3-+-1-+
**����ʱ������HALLλ�ò���ҳ��űۿ������״̬�������תʱHALL�źŰ�λȡ���ٲ����
**120��:1-3-2-6-4-5
*************************************************************************************/
void PWM_HallPhaseChange(TIMER_INDEX TimerIdx,uint8_t ucHall)//HALL����
{

	if(TimerIdx < TIMER5)
	{
		return;
	}

	/*ע�⣺PWM 1 2 3 ��P/N���ŷֱ��ӦMOS�����ű� A B C �� H/L*/
	switch(ucHall)	//����HALLλ�ã��л������
	{
			//case 0x00://60��HALL
			case 0x05:{
							//1,4	                 A+  B-
							PWM_3PhaseSet(TimerIdx,PWM1,PWM2,PWM3);
					  };
			break;
			case 0x04:{
							//4,5	                 C+  B-
						    PWM_3PhaseSet(TimerIdx,PWM3,PWM2,PWM1);
					  };
			break;
			case 0x06:{
							//2,5	                 C+   A-
				            PWM_3PhaseSet(TimerIdx,PWM3,PWM1,PWM2);
					  };
			break;
			//case 0x07: //60��HALL
			case 0x02:{
							//2,3	                 B+   A-
							PWM_3PhaseSet(TimerIdx,PWM2,PWM1,PWM3);
					  };
			break;
			case 0x03:{
							//3,6	                 B+   C-
				            PWM_3PhaseSet(TimerIdx,PWM2,PWM3,PWM1);
					  };
			break;
			case 0x01:{
						    //1,6	                 A+   C-
				            PWM_3PhaseSet(TimerIdx,PWM1,PWM3,PWM2);
					  };
			break;
			default:

					break;
	}
}
/*********************************************************************************************************
** Func name:               Timer5Interrupt()
** Param:
**
** Return:
** Description:             TIMER5(PWC)�жϷ����������HALL����
*********************************************************************************************************/
#if   (MOTOR_NUM == 1)
__attribute__((section(".driver.isr")))void Timer5Interrupt(void)
#else
__attribute__((section(".driver.isr")))void Timer7Interrupt(void)
#endif
{
	uint8_t ucHall;
	uint32_t PWCValue;
    TIMER_INDEX TimerIdx = PWC_TIM_IDX;

	if(Timer_InterruptFlagGet(TimerIdx,PWC_OVER_RUN_INTERRUPT_SRC))//����ʱ�����,65535us
	{
		Timer_InterruptFlagClear(TimerIdx,PWC_OVER_RUN_INTERRUPT_SRC);	  //���жϱ�־
		//�жϲ���ʱ������жϣ������ڶ�ת�ж�
	}

	if(Timer_InterruptFlagGet(TimerIdx,PWC1_CAP_DATA_INTERRUPT_SRC))//cap�ж�
	{
		PWCValue = PWC_IOCTRL(TimerIdx, PWC1, PWC_DATA_GET, NULL);//���Ƚ�ֵ���Զ���CAP�ж�
		ucHall  = GetHallVal();	//��HALLλ��
	    {
			 if(IsRSStatus_Run())//����ʱ����
			 {
				PWM_HallPhaseChange(PWM_TIM_IDX,ucHall);//����HALLֵ���л���
			 }
			/*************************************���¼���ʵʱ�ٶ�ֵ************************************************/
	         //����PWC����ֵ��������ת��
	    }
	}
}
/*********************************************************************************************************
** Func name:               Timer6Interrupt()
** Param:
**
** Return:
** Description:             PWM�жϷ����������BREAK IN����
*********************************************************************************************************/
#if   (MOTOR_NUM == 1)
__attribute__((section(".driver.isr")))void Timer6Interrupt(void)
#else
__attribute__((section(".driver.isr")))void Timer8Interrupt(void)
#endif
{
	static uint16_t usOCCnt;
    TIMER_INDEX TimerIdx = PWM_TIM_IDX;

	if(Timer_InterruptFlagGet(TimerIdx,PWM_BREAK_IN_INTERRUPT_SRC))//
	{
		Timer_InterruptFlagClear(TimerIdx,PWM_BREAK_IN_INTERRUPT_SRC);	  //���ж�
		if(++usOCCnt > 1)//Ӳ��������������������������
		{
			usOCCnt = 0;

			SetRSStatus_Run(0);//��ͣ��״̬
			SetPWM_Duty(0);//ռ�ձ���Ϊ0
    		SetRSStatus_Fault(1);//�ù���״̬
    		PWM_OutPutEnable(PWM_TIM_IDX,0);//��ֹPWM���
    		APP_DBG("ERR:OC\n");//��OC��������
		}
	}
	if(Timer_InterruptFlagGet(TimerIdx,UPDATE_INTERRUPT_SRC))//update interrupt
	{
		Timer_InterruptFlagClear(TimerIdx,UPDATE_INTERRUPT_SRC);
	}
}
/*********************************************************************************************************
** Func name:               BLDC_CmdHelp()
** Param:
**
** Return:
** Description:             ��������ʾ��Ϣ
*********************************************************************************************************/
void BLDC_CmdHelp(void)
{
	APP_DBG("\n------------------------------------------------------\n");
	APP_DBG("Comand line:\n");
	APP_DBG("BLDC Run:      R   \n");
	APP_DBG("BLDC STOP:     S   \n");
	APP_DBG("Fault Reset:   F	\n");
	APP_DBG("PWM Duty Set:  0~9 \n");
	APP_DBG("Help:          ?   \n");
	APP_DBG("------------------------------------------------------\n");
}
/*********************************************************************************************************
** Func name:               BLDC_DebugInfo()
** Param:
**
** Return:
** Description:             ������Ϣ
*********************************************************************************************************/
void BLDC_DebugInfo(void)
{
    static	uint32_t timerCnt = 0;

	if(timerCnt++ > 1000000)//��ʱ��ӡ
	{
		timerCnt = 0;//��ӡһ����Ϣ

		 if(IsRSStatus_Run())
		 {
			 DBG("PWM=%d%%,Hall=%d,Break_in=%d\n",GetPWM_Duty(), GetHallVal(),PWM_IOCTRl(PWM_TIM_IDX, PWM1, BREAKIN_STATUS_GET, NULL));
		 }
	}
}
/*********************************************************************************************************
** Func name:               main()
** Param:
**
** Return:
** Description:             ������
*********************************************************************************************************/
int main(void)
{	
    uint8_t rx = 0;

	//Disable watchDog
	WDG_Disable();
	CacheTcmInitFast(PHYMEM_16KBPM0 /*cache*/, PHYMEM_16KBPM1/*tcm r0*/, 0/*tcm r0 offset*/, 4/*tcm r0 size*/, PHYMEM_NONE/*tcm r1*/, 0/*tcm r1 offset*/, 0/*tcm r1 size*/);

#if  1    //OSC_12M
	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB30);	//PU=0,PD=1:ģ��
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB30);
	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB31);	//PU=0,PD=1:ģ��
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB31);

	Clock_Config(1, 12000000);//��12Mʱ����Ҫ��B30��B31��Ϊģ������
#else//32K
	Clock_Config(1, 32768);
#endif
	Clock_PllLock(480000);
	Clock_SysClkSelect(PLL_CLK_MODE);
	Clock_UARTClkSelect(0);

	//	SpiFlashInit(80000000, MODE_4BIT, 0, 1);

	GPIO_UartTxIoConfig(0, 1);//GPIO_C2
	GPIO_UartRxIoConfig(0, 1);//GPIO_C3

	UARTS_Init(0,  115200, 8,  0,  1);
	GIE_ENABLE();

	//������Ϣ
	APP_DBG("------------------------------------------------------\n");
	APP_DBG("             BLDC HALL Operation Example                      \n");
	APP_DBG("       Mountain View Silicon Technology Co.,Ltd.               \n");
	APP_DBG("------------------------------------------------------\r\n\r\n");
	
	PWM_TimerConfig();//��ʼ��PWM��PWC
	PWC_TimerConfig();
	BLDC_CmdHelp();//����������ʾ

	while(1)
	{
		UART0_RecvByte(&rx);
	    switch(rx)
	    {
			case 'R': //RUN
				if(!IsRSStatus_Run() && !IsRSStatus_Fault())
				{
					SetRSStatus_Run(1);  //����
					PWM_HallPhaseChange(PWM_TIM_IDX,GetHallVal());//��������
					PWM_Enable(PWM_TIM_IDX);//PWM��ʱ��ʹ��
					APP_DBG("Cmd:Run,PWM duty=%d%%\n",GetPWM_Duty());
				}
				else
				{
					APP_DBG("Cmd:Invalid\n");
				}
				break;
			case 'S': //STOP
				SetRSStatus_Run(0);  //ͣ��
				//SetPWM_Duty(0);
				PWM_OutPutEnable(PWM_TIM_IDX,0);//��ֹPWM���
				APP_DBG("Cmd:Stop\n");
				break;
			case 'F': //FAULT RESET
				if(IsRSStatus_Fault())//�жϵ�ǰ�Ƿ��й���
				{
					SetRSStatus_Fault(0);//������0
					APP_DBG("Cmd:Fault reset\n");
				}
				else
				{
					APP_DBG("Cmd:Invalid\n");
				}
				break;
			case '0':// PWM ռ�ձ�����
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				SetPWM_Duty((rx - '0')*10 + 5);//0~9��ӦPWMռ�ձ�5%~95%
				PWM_DutySet(PWM_TIM_IDX,GetPWM_Duty(),GetPWM_Duty(),GetPWM_Duty());//����PWMֵ
				APP_DBG("PWM duty set:%d%%\n",((rx - '0')*10 + 5));
				break;
			case '?':
				BLDC_CmdHelp();//������ʾ
				break;
			default:

				break;
	    }
	    rx = 0;

	    BLDC_DebugInfo();//��ӡ������Ϣ
	}
	return -1;
}

