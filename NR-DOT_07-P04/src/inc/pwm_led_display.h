#ifndef __PWM_LED_DISP__
#define __PWM_LED_DISP__


#include "gpio.h"
#include "pwm.h"
#ifdef FUNC_ALEXA_PWM_LED_EN

 #define MAX_LED_PIN_NUM		12	//LED ������

/******************************************************************************/
//PWM ����ڶ��壬��ز����μ�pwm.h
/******************************************************************************/
#define   LED_RED_PWM_PORT_MASK        PWM_CH7_B9_B20
#define   LED_GREEN_PWM_PORT_MASK      PWM_CH6_B8_B21
#define   LED_BLUE_PWM_PORT_MASK       PWM_CH5_B7_B22	

/******************************************************************************/																																					
//����Ϊ���������ʾ��ɫ��������Ҫ�����޸�
//ע�⣺��ΪLED��ɫ�����ɫƫ���⣬��Ҫ���ݵ�ɫ�µ�����ɫ����
//����������PWM������Ӧ��ɫ��Ƶ�ʣ�10KHz����غ������ܣ��μ�pwm.h
/*****************************************************************************/
									
 #define  LED_RED_MODE_ON()    do{\
							                PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 1200);\
							                PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)
 
 #define  LED_RED_MODE_OFF()    do{\
							                PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 0);\
							                PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)
 
 #define  LED_GREEN_MODE_ON()    do{\
							                PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 1200);\
							                PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)	

 #define  LED_GREEN_MODE_OFF()    do{\
							                PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 0);\
							                PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)
 
 #define  LED_BLUE_MODE_ON()    do{\
							                PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 1200);\
							                PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)	

 #define  LED_BLUE_MODE_OFF()    do{\
							                PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 0);\
							                PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)															

 #define  LED_WHITE_MODE_ON()	  do{\
							                PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 1164);\
							                PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
							                PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 780);\
							                PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
							                PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 840);\
							                PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
									            }while(0)														
															
 #define  LED_WHITE_MODE_OFF()	  do{\
										          LED_RED_MODE_OFF();\
										          LED_GREEN_MODE_OFF();\
										          LED_BLUE_MODE_OFF();\
									            }while(0)
	
 #define  LED_ORANGE_MODE_ON()	   do{\
															 PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 1200);\
															 PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 180);\
															 PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 0);\
															 PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 }while(0)
 
#define  LED_YELLOW_MODE_ON()	   do{\
															 PwmConfig(LED_RED_PWM_PORT_MASK, 1200, 1200);\
															 PwmEnableChannel(LED_RED_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 PwmConfig(LED_GREEN_PWM_PORT_MASK, 1200, 600);\
															 PwmEnableChannel(LED_GREEN_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 PwmConfig(LED_BLUE_PWM_PORT_MASK, 1200, 0);\
															 PwmEnableChannel(LED_BLUE_PWM_PORT_MASK, PWM_IO_SEL1, PWM_MODE_OUT);\
															 }while(0)

 #define  LED_PURPLE_MODE_ON()	  do{\
										          LED_RED_MODE_ON();\
										          LED_GREEN_MODE_OFF();\
										          LED_BLUE_MODE_ON();\
									            }while(0)																
															
 #define  LED_CYAN_MODE_ON()	  do{\
										          LED_RED_MODE_OFF();\
										          LED_GREEN_MODE_ON();\
										          LED_BLUE_MODE_ON();\
									            }while(0)																
															
 #define  LED_ALL_MODE_OFF()  	LED_WHITE_MODE_OFF()														

#endif
#endif

