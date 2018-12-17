#ifndef __SEG_LED_DISP__
#define __SEG_LED_DISP__


#include "gpio.h"
#ifdef FUNC_SEG_LED_EN

//7��6��LED������������ɨ���㷨���£�

//7��LED����ϰ���Ͻ�7���ܽŵ���ų�Ϊ1��--7��
//Ϊ�˷���д���룬��ģ���н�7������Ÿ�Ϊ0-6
//COM�׶ηֱ�ΪCOM0--COM6
//SEG����ֱ�ΪSEG0--SEG6

//ɨ���㷨

//COM��ѡ��: ����ߵ�ƽ
//COM��ѡ��: ����̬

//SEG��: ʹ����������Դ
//SEG��: ����̬

//COM0: 1����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM1: 2����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM2: 3����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM3: 4����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM4: 5����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM5: 6����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬
//COM6: 7����COM����ߵ�ƽ, ����6������SEG, ���ıʻ�SEGʹ����������Դ����ıʻ�SEG����Ϊ����̬


#if (defined(FUNC_7PIN_SEG_LED_EN) || defined(FUNC_6PIN_SEG_LED_EN))

#ifdef FUNC_6PIN_SEG_LED_EN
#define MAX_LED_PIN_NUM		6	//LED ������
#else
#define MAX_LED_PIN_NUM		7	//LED ������
#endif


//LED ��������GPIO ���Ӷ���,�����Ӳ������
#ifdef FUNC_6PIN_SEG_LED_EN
//6ֻ�Ŷ���LED��ʾ��
#define LED_PIN1_PORT_PU 		GPIO_A_PU
#define LED_PIN1_PORT_PD 		GPIO_A_PD
#define LED_PIN1_PORT_OE 		GPIO_A_OE
#define LED_PIN1_PORT_IE 		GPIO_A_IE
#define LED_PIN1_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN1_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN1_PORT_OUT 		GPIO_A_OUT
#define LED_PIN1_BIT			GPIOA26

#define LED_PIN2_PORT_PU 		GPIO_A_PU
#define LED_PIN2_PORT_PD 		GPIO_A_PD
#define LED_PIN2_PORT_OE 		GPIO_A_OE
#define LED_PIN2_PORT_IE 		GPIO_A_IE
#define LED_PIN2_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN2_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN2_PORT_OUT 		GPIO_A_OUT
#define LED_PIN2_BIT			GPIOA27

#define LED_PIN3_PORT_PU 		GPIO_A_PU
#define LED_PIN3_PORT_PD 		GPIO_A_PD
#define LED_PIN3_PORT_OE 		GPIO_A_OE
#define LED_PIN3_PORT_IE 		GPIO_A_IE
#define LED_PIN3_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN3_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN3_PORT_OUT 		GPIO_A_OUT
#define LED_PIN3_BIT			GPIOA28

#define LED_PIN4_PORT_PU 		GPIO_A_PU
#define LED_PIN4_PORT_PD 		GPIO_A_PD
#define LED_PIN4_PORT_OE 		GPIO_A_OE
#define LED_PIN4_PORT_IE 		GPIO_A_IE
#define LED_PIN4_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN4_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN4_PORT_OUT 		GPIO_A_OUT
#define LED_PIN4_BIT			GPIOA29

#define LED_PIN5_PORT_PU 		GPIO_A_PU
#define LED_PIN5_PORT_PD 		GPIO_A_PD
#define LED_PIN5_PORT_OE 		GPIO_A_OE
#define LED_PIN5_PORT_IE 		GPIO_A_IE
#define LED_PIN5_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN5_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN5_PORT_OUT 		GPIO_A_OUT
#define LED_PIN5_BIT			GPIOA30

#define LED_PIN6_PORT_PU 		GPIO_A_PU
#define LED_PIN6_PORT_PD 		GPIO_A_PD
#define LED_PIN6_PORT_OE 		GPIO_A_OE
#define LED_PIN6_PORT_IE 		GPIO_A_IE
#define LED_PIN6_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN6_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN6_PORT_OUT 		GPIO_A_OUT
#define LED_PIN6_BIT			GPIOA31

#elif defined(FUNC_7PIN_SEG_LED_EN)
//7ֻ�Ŷ���LED��ʾ��
#define LED_PIN1_PORT_PU 		GPIO_A_PU
#define LED_PIN1_PORT_PD 		GPIO_A_PD
#define LED_PIN1_PORT_OE 		GPIO_A_OE
#define LED_PIN1_PORT_IE 		GPIO_A_IE
#define LED_PIN1_PORT_ICS_1MA7 	GPIO_A_PULLDOWN1
#define LED_PIN1_PORT_ICS_2MA4 	GPIO_A_PULLDOWN2
#define LED_PIN1_PORT_OUT 		GPIO_A_OUT
#define LED_PIN1_BIT			GPIOA25

#define LED_PIN2_PORT_PU 		GPIO_B_PU
#define LED_PIN2_PORT_PD 		GPIO_B_PD
#define LED_PIN2_PORT_OE 		GPIO_B_OE
#define LED_PIN2_PORT_IE 		GPIO_B_IE
#define LED_PIN2_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN2_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN2_PORT_OUT 		GPIO_B_OUT
#define LED_PIN2_BIT			GPIOB2

#define LED_PIN3_PORT_PU 		GPIO_B_PU
#define LED_PIN3_PORT_PD 		GPIO_B_PD
#define LED_PIN3_PORT_OE 		GPIO_B_OE
#define LED_PIN3_PORT_IE 		GPIO_B_IE
#define LED_PIN3_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN3_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN3_PORT_OUT 		GPIO_B_OUT
#define LED_PIN3_BIT			GPIOB3

#define LED_PIN4_PORT_PU 		GPIO_B_PU
#define LED_PIN4_PORT_PD 		GPIO_B_PD
#define LED_PIN4_PORT_OE 		GPIO_B_OE
#define LED_PIN4_PORT_IE 		GPIO_B_IE
#define LED_PIN4_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN4_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN4_PORT_OUT 		GPIO_B_OUT
#define LED_PIN4_BIT			GPIOB4

#define LED_PIN5_PORT_PU 		GPIO_B_PU
#define LED_PIN5_PORT_PD 		GPIO_B_PD
#define LED_PIN5_PORT_OE 		GPIO_B_OE
#define LED_PIN5_PORT_IE 		GPIO_B_IE
#define LED_PIN5_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN5_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN5_PORT_OUT 		GPIO_B_OUT
#define LED_PIN5_BIT			GPIOB5

#define LED_PIN6_PORT_PU 		GPIO_B_PU
#define LED_PIN6_PORT_PD 		GPIO_B_PD
#define LED_PIN6_PORT_OE 		GPIO_B_OE
#define LED_PIN6_PORT_IE 		GPIO_B_IE
#define LED_PIN6_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN6_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN6_PORT_OUT 		GPIO_B_OUT
#define LED_PIN6_BIT			GPIOB6

#define LED_PIN7_PORT_PU 		GPIO_B_PU
#define LED_PIN7_PORT_PD 		GPIO_B_PD
#define LED_PIN7_PORT_OE 		GPIO_B_OE
#define LED_PIN7_PORT_IE 		GPIO_B_IE
#define LED_PIN7_PORT_ICS_1MA7 	GPIO_B_PULLDOWN1
#define LED_PIN7_PORT_ICS_2MA4 	GPIO_B_PULLDOWN2
#define LED_PIN7_PORT_OUT 		GPIO_B_OUT
#define LED_PIN7_BIT			GPIOB7

#endif


//����LED ������������ߵ�ƽ
#define LED_PIN1_OUT_HIGH	GpioSetRegOneBit(LED_PIN1_PORT_OE,LED_PIN1_BIT),\
                                                 GpioSetRegOneBit(LED_PIN1_PORT_OUT,LED_PIN1_BIT)

#define LED_PIN2_OUT_HIGH	GpioSetRegOneBit(LED_PIN2_PORT_OE,LED_PIN2_BIT),\
                                                 GpioSetRegOneBit(LED_PIN2_PORT_OUT,LED_PIN2_BIT)

#define LED_PIN3_OUT_HIGH	GpioSetRegOneBit(LED_PIN3_PORT_OE,LED_PIN3_BIT),\
                                                 GpioSetRegOneBit(LED_PIN3_PORT_OUT,LED_PIN3_BIT)

#define LED_PIN4_OUT_HIGH	GpioSetRegOneBit(LED_PIN4_PORT_OE,LED_PIN4_BIT),\
                                                 GpioSetRegOneBit(LED_PIN4_PORT_OUT,LED_PIN4_BIT)

#define LED_PIN5_OUT_HIGH	GpioSetRegOneBit(LED_PIN5_PORT_OE,LED_PIN5_BIT),\
                                                 GpioSetRegOneBit(LED_PIN5_PORT_OUT,LED_PIN5_BIT)

#define LED_PIN6_OUT_HIGH	GpioSetRegOneBit(LED_PIN6_PORT_OE,LED_PIN6_BIT),\
                                                 GpioSetRegOneBit(LED_PIN6_PORT_OUT,LED_PIN6_BIT)

#ifdef FUNC_7PIN_SEG_LED_EN
#define LED_PIN7_OUT_HIGH	GpioSetRegOneBit(LED_PIN7_PORT_OE,LED_PIN7_BIT),\
                                                 GpioSetRegOneBit(LED_PIN7_PORT_OUT,LED_PIN7_BIT)
#endif


#define LED_PIN1_IN_ON	 GpioSetRegOneBit(LED_PIN1_PORT_ICS_2MA4,LED_PIN1_BIT),\
                                           GpioSetRegOneBit(LED_PIN1_PORT_ICS_1MA7,LED_PIN1_BIT)

#define LED_PIN2_IN_ON	 GpioSetRegOneBit(LED_PIN2_PORT_ICS_2MA4,LED_PIN2_BIT),\
                                           GpioSetRegOneBit(LED_PIN2_PORT_ICS_1MA7,LED_PIN2_BIT)

#define LED_PIN3_IN_ON	 GpioSetRegOneBit(LED_PIN3_PORT_ICS_2MA4,LED_PIN3_BIT),\
                                           GpioSetRegOneBit(LED_PIN3_PORT_ICS_1MA7,LED_PIN3_BIT)

#define LED_PIN4_IN_ON	 GpioSetRegOneBit(LED_PIN4_PORT_ICS_2MA4,LED_PIN4_BIT),\
                                           GpioSetRegOneBit(LED_PIN4_PORT_ICS_1MA7,LED_PIN4_BIT)

#define LED_PIN5_IN_ON	 GpioSetRegOneBit(LED_PIN5_PORT_ICS_2MA4,LED_PIN5_BIT),\
                                           GpioSetRegOneBit(LED_PIN5_PORT_ICS_1MA7,LED_PIN5_BIT)

#define LED_PIN6_IN_ON	GpioSetRegOneBit(LED_PIN6_PORT_ICS_2MA4,LED_PIN6_BIT),\
                                           GpioSetRegOneBit(LED_PIN6_PORT_ICS_1MA7,LED_PIN6_BIT)

#ifdef FUNC_7PIN_SEG_LED_EN
#define LED_PIN7_IN_ON	GpioSetRegOneBit(LED_PIN7_PORT_ICS_2MA4,LED_PIN7_BIT),\
                                           GpioSetRegOneBit(LED_PIN7_PORT_ICS_1MA7,LED_PIN7_BIT)
#endif

//��ֹLED �������ŵ�������
#define LED_PIN1_IN_OFF	 GpioClrRegOneBit(LED_PIN1_PORT_ICS_2MA4,LED_PIN1_BIT),\
                                           GpioClrRegOneBit(LED_PIN1_PORT_ICS_1MA7,LED_PIN1_BIT)

#define LED_PIN2_IN_OFF	 GpioClrRegOneBit(LED_PIN2_PORT_ICS_2MA4,LED_PIN2_BIT),\
                                           GpioClrRegOneBit(LED_PIN2_PORT_ICS_1MA7,LED_PIN2_BIT)

#define LED_PIN3_IN_OFF	 GpioClrRegOneBit(LED_PIN3_PORT_ICS_2MA4,LED_PIN3_BIT),\
                                           GpioClrRegOneBit(LED_PIN3_PORT_ICS_1MA7,LED_PIN3_BIT)

#define LED_PIN4_IN_OFF	 GpioClrRegOneBit(LED_PIN4_PORT_ICS_2MA4,LED_PIN4_BIT),\
                                           GpioClrRegOneBit(LED_PIN4_PORT_ICS_1MA7,LED_PIN4_BIT)

#define LED_PIN5_IN_OFF	 GpioClrRegOneBit(LED_PIN5_PORT_ICS_2MA4,LED_PIN5_BIT),\
                                           GpioClrRegOneBit(LED_PIN5_PORT_ICS_1MA7,LED_PIN5_BIT)

#define LED_PIN6_IN_OFF	 GpioClrRegOneBit(LED_PIN6_PORT_ICS_2MA4,LED_PIN6_BIT),\
                                           GpioClrRegOneBit(LED_PIN6_PORT_ICS_1MA7,LED_PIN6_BIT)

#ifdef FUNC_7PIN_SEG_LED_EN
#define LED_PIN7_IN_OFF	 GpioClrRegOneBit(LED_PIN7_PORT_ICS_2MA4,LED_PIN7_BIT),\
                                           GpioClrRegOneBit(LED_PIN7_PORT_ICS_1MA7,LED_PIN7_BIT)
#endif

#endif


void DispLedIcon(ICON_IDX Icon, ICON_STATU Light);
void LedDispDevSymbol(void);
void LedDispRepeat(void);
void LedDispRepeatAB(void);
void LedDispPlayState(void);

void LedAllPinGpioInput(void);
void LedPinGpioDeInit(void);
void LedPinGpioInit(void);
#endif

#ifdef FUNC_SINGLE_LED_EN
void SLedInit(void);
void SLedDisplay(void);
void SLedDeinit(void);
#endif
#endif



