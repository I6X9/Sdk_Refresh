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
#include <stdint.h>
#include "stm32f0xx_conf.h"
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
static uint8_t CurWorkMode = 0;
static TIMER WaveAutoSwitchTimer;

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define	WAVE_MIN_FREQ					490			//���������СƵ��
#define WAVE_MAX_FREQ					510			//����������Ƶ��
#define WAVE_STILL_FREQ					495			//ˮ�龲ֹ״̬Ƶ��		
#define WAVE_PWM_DUTY					500			//��������ź�ռ�ձ�
#define WAVE_MAX_OPERATION				42			//�������������������
#define PWM_OUTPUT_OFF					0
#define	LIGHT_MAX_DISP_NUM				4			//LED�����ƵĹ���״̬

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
typedef struct _LED_SET_PARAM_ {
	DISP_STATE_TYPEDEF CurDispStateNum;
	DISP_COLOR_TYPEDEF CurDispColorNum;
	uint32_t ScanTimerNum;
}LED_SET;

typedef struct _WAVE_SET_PARAM_ {
	uint32_t Timer;
	uint16_t Freq;
}WAVE_SET;

const LED_SET Light_WorkState[LIGHT_MAX_DISP_NUM] = {
	{DISP_STATE_IDLE, DISP_COLOR_OFF, 0},
	{DISP_STATE_SINGLE_COLOR, DISP_COLOR_WHITE, 1500},
	{DISP_STATE_SINGLE_COLOR, DISP_COLOR_BLUE, 1500},
	{DISP_STATE_FULL_COLOR, DISP_COLOR_RED, 1500},
};


const WAVE_SET WAVE_OPERATION_DATA[WAVE_MAX_OPERATION] = {
	{5000*5, WAVE_MIN_FREQ},
	{5000*5, WAVE_MIN_FREQ},
	{5000*5, WAVE_MIN_FREQ},
	
	{1000, WAVE_MIN_FREQ+1},
	{1000, WAVE_MIN_FREQ+2},        
	{1000, WAVE_MIN_FREQ+3},
	{1000, WAVE_MIN_FREQ+4},
	{1000, WAVE_MIN_FREQ+5},
	{1000, WAVE_MIN_FREQ+6},
	{1000, WAVE_MIN_FREQ+7},        
	{1000, WAVE_MIN_FREQ+8},
	{1000, WAVE_MIN_FREQ+9},
	
	{3000*5, WAVE_STILL_FREQ},        
	{1000, WAVE_MAX_FREQ-9},
	{1000, WAVE_MAX_FREQ-8},
	{1000, WAVE_MAX_FREQ-7},
	{1000, WAVE_MAX_FREQ-6},
	{1000, WAVE_MAX_FREQ-5},        
	{1000, WAVE_MAX_FREQ-4},
	{1000, WAVE_MAX_FREQ-3},
	{1000, WAVE_MAX_FREQ-2},
	{1000, WAVE_MAX_FREQ-1},  

	{4000*5, WAVE_MAX_FREQ},        
	{1000, WAVE_MAX_FREQ-1},
	{1000, WAVE_MAX_FREQ-2},
	{1000, WAVE_MAX_FREQ-3},
	{1000, WAVE_MAX_FREQ-4},
	{1000, WAVE_MAX_FREQ-5},
	{1000, WAVE_MAX_FREQ-6},
	{1000, WAVE_MAX_FREQ-7},
	{1000, WAVE_MAX_FREQ-8},
	{1000, WAVE_MAX_FREQ-9},

	{3000*5, WAVE_STILL_FREQ},
	{1000, WAVE_MIN_FREQ+9},
	{1000, WAVE_MIN_FREQ+8},
	{1000, WAVE_MIN_FREQ+7},
	{1000, WAVE_MIN_FREQ+6},
	{1000, WAVE_MIN_FREQ+5},
	{1000, WAVE_MIN_FREQ+4},
	{1000, WAVE_MIN_FREQ+3},   
	{1000, WAVE_MIN_FREQ+2},        
	{1000, WAVE_MIN_FREQ+1},
};


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
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configure USART Tx as alternate function push-pull */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	/* Enable USART clock */
	USART_DeInit(USART1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	/* USARTx configured as 115200,8 Bits,1 Stop Bit,No Parity*/
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_SWAPPinCmd(USART1,ENABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable and set USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	printf("Usart debug init OK!\n");
}

bool Asm_Usart_Debug_Busy(int Word) {
	if ((SET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) && (FALSE == GetDebugDataLen())) {
    	USART_SendData(USART1, (uint16_t)Word);
    	return FALSE;
    }
	return TRUE;
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
/*****************************************************************************
 �� �� ��  : System_WaterPump_Init
 ��������  : ˮ�ÿ��ƺͼ����ؽӿڳ�ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void System_WaterPump_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);

	/* Configure ADC Channel1 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC Change config
	ADC_InitTypeDef 	ADC_InitStructure;
	
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADCs DeInit */  
	ADC_DeInit(ADC1);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure); 

	/* Convert the ADC1 Channel 1 with 239.5 Cycles as sampling time */ 
	ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC1, ENABLE);     

	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 

	/* ADC1 regular Software Start Conv */ 
	ADC_StartOfConversion(ADC1);
}
/*****************************************************************************
 �� �� ��  : Pwm_SaveConfig
 ��������  : ��������ź�����
 �������  : uint32_t Freq  
             uint16_t Duty  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void Pwm_WaveConfig(uint32_t Freq, uint16_t Duty) {
	PwmConfigChannel(PWM14_CH1_A4_B1, Freq, Duty);
	PwmEnableChannel(PWM14_CH1_A4_B1, PWM_IO_SEL1, PWM_OUTPUT_HIGH);
}
/*****************************************************************************
 �� �� ��  : System_WaterPump_Control
 ��������  : ˮ�ù���״̬����
 �������  : FlagStatus NewState  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void System_WaterPump_Control(bool NewState) {
	if (TRUE == NewState) {
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}

/*****************************************************************************
 �� �� ��  : WaterPump_WorkStateDetect
 ��������  : ˮ�ù���״̬���
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��22��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
#define WATER_PUMP_DET_JITTER			5000
#define WATER_PUMP_DETECT_CNT			1000
#define WATER_PUMP_DETECT_NULL			(70*4095/3300)
#define WATER_PUMP_DETECT_SORT			(500*4095/3300)

const uint16_t WaterPumpParam[3] = {WATER_PUMP_DETECT_NULL, WATER_PUMP_DETECT_SORT, 4095};
const uint16_t WaterPumpDetMsg[3] = {MSG_VAL_LOW, MSG_VAL_NORMAL, MSG_VAL_HIGHT};

/*****************************************************************************
 �� �� ��  : WaterPump_WorkStateDetect
 ��������  : ˮ�ù���������⣬ͨ����������С�ж�?-
                 ??λ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��22��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
uint16_t WaterPump_WorkStateDetect(void) {
	static uint32_t SampleWaterPuapDetVal = 0, AverageWaterPampDetVal = 0;
	static uint16_t	WaterPampDetCnt = WATER_PUMP_DETECT_CNT,PrevIndex = 0xFF;
	static TIMER WaterPumpDetTimer;
	
	if (SET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
		SampleWaterPuapDetVal += ADC_GetConversionValue(ADC1);
		WaterPampDetCnt--;
	}

	if (0 == WaterPampDetCnt) {
		WaterPampDetCnt = WATER_PUMP_DETECT_CNT;
		AverageWaterPampDetVal = (SampleWaterPuapDetVal/WATER_PUMP_DETECT_CNT);
		SampleWaterPuapDetVal = 0;

		//printf("WaterPump_WorkStateDetect ADC_Value=%d;\n", AverageWaterPampDetVal);

		uint8_t CurIndex = 0;
		for (CurIndex = 0; CurIndex < 3; CurIndex++) {
			if (AverageWaterPampDetVal < WaterPumpParam[CurIndex]) {
				break;
			}
		}

		if (PrevIndex != CurIndex) {
			TimeOutSet(&WaterPumpDetTimer, WATER_PUMP_DET_JITTER);
			PrevIndex = CurIndex;
		}
		else if (IsTimeOut(&WaterPumpDetTimer)) {
			TimeOutSet(&WaterPumpDetTimer, WATER_PUMP_DET_JITTER/2);
			return WaterPumpDetMsg[CurIndex];
		}

	}

	return MSG_NONE;
}
/*****************************************************************************
 �� �� ��  : Display
 ��������  : ��ʾ����ˢ�£���main��������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void Display(void) {
	Led_Display();
	Light_Display();
}
/*****************************************************************************
 �� �� ��  : DisplayScan
 ��������  : ��ʾɨ�躯�����ɶ�ʱ���жϵ���
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void DisplayScan(void) {
	Led_DispScan();
	Light_DispScan();
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
	System_Systick_Init(1000);
	/* ���������ź������ʼ�� */
	Pwm_WaveConfig(WAVE_MIN_FREQ, PWM_OUTPUT_OFF);
	TimeOutSet(&WaveAutoSwitchTimer, 0);
	/* GUI LED��ʾ��ʼ�� */
	Led_DispInit();
	Led_DispStateSet(DISP_STATE_LOOP_CHANGE, DISP_COLOR_RED, 5000);
	
	/* ����ɨ���ʼ�� */
	GpioKeyInit();
	/* ˮ�ÿ��Ƽ�����ʼ�� */
	System_WaterPump_Init();
	/* �����Ƴ�ʼ�� */
	Light_DispInit();
	Light_DispStateSet(DISP_STATE_IDLE, DISP_COLOR_OFF, 0);
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
	static uint8_t WaveAutoIndex = 0;
		
	Display();
	
	if (MSG_NEXT == GpioKeyScan()) {
		CurWorkMode++;
		CurWorkMode %= LIGHT_MAX_DISP_NUM;
		WaveAutoIndex = 0;

		if (FALSE == CurWorkMode) {
			System_WaterPump_Control(FALSE);
			Pwm_WaveConfig(WAVE_MIN_FREQ, PWM_OUTPUT_OFF);
			Led_DispStateSet(DISP_STATE_LOOP_CHANGE, DISP_COLOR_WHITE, 5000);
		}
		else if (1 == CurWorkMode) {
			System_WaterPump_Control(TRUE);
			WaitMs(1000);
			TimeOutSet(&WaveAutoSwitchTimer, WAVE_OPERATION_DATA[WaveAutoIndex].Timer);
			Pwm_WaveConfig(WAVE_OPERATION_DATA[WaveAutoIndex].Freq, WAVE_PWM_DUTY);
			Led_DispStateSet(DISP_STATE_LOOP_SPIN, DISP_COLOR_RED, 1000);
		}

		Light_DispStateSet(Light_WorkState[CurWorkMode].CurDispStateNum,
		Light_WorkState[CurWorkMode].CurDispColorNum, Light_WorkState[CurWorkMode].ScanTimerNum);
	}

	uint16_t Msg = WaterPump_WorkStateDetect();
	if (MSG_NONE != Msg) {
		printf("WaterPump_WorkStateDetect Msg = %x;\n", Msg);
	}

	if (0 == CurWorkMode) {
		return;
	}

	if (MSG_VAL_NORMAL == Msg) {
		if (IsTimeOut(&WaveAutoSwitchTimer)) {
			WaveAutoIndex++;
			WaveAutoIndex %= WAVE_MAX_OPERATION;

			TimeOutSet(&WaveAutoSwitchTimer, WAVE_OPERATION_DATA[WaveAutoIndex].Timer);
			Pwm_WaveConfig(WAVE_OPERATION_DATA[WaveAutoIndex].Freq, WAVE_PWM_DUTY);
			Led_DispStateSet(DISP_STATE_LOOP_SPIN, DISP_COLOR_RED, 5000);
		}
	}
	else if (MSG_VAL_LOW == Msg){
		Led_DispStateSet(DISP_STATE_LOOP_CHANGE, DISP_COLOR_RED, 1000);
		CurWorkMode = 0;
		Light_DispStateSet(DISP_STATE_IDLE, DISP_COLOR_OFF, 0);
		Pwm_WaveConfig(WAVE_MIN_FREQ, PWM_OUTPUT_OFF);
		System_WaterPump_Control(FALSE);
	}
	else if (MSG_VAL_HIGHT == Msg) {	
		Led_DispStateSet(DISP_STATE_LOOP_CHANGE, DISP_COLOR_RED, 500);
		CurWorkMode = 0;
		Light_DispStateSet(DISP_STATE_IDLE, DISP_COLOR_OFF, 0);
		Pwm_WaveConfig(WAVE_MIN_FREQ, PWM_OUTPUT_OFF);
		System_WaterPump_Control(FALSE);
	}
}


