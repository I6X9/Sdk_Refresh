/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : gpio_key.c
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��4��20��
  ����޸�   :
  ��������   : 
  �����б�   :
              GpioKeyInit
              GpioKeyScan
  �޸���ʷ   :
  1.��    ��   : 2019��4��20��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

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

/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : power_key.c
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2018��9��13��
  ����޸�   :
  ��������   : GPIO��Դ���ƹ���
  �����б�   :
              PowerKeyScan
              SysPowerOnControl
              SystemGotoDeepSleepPowerDown
              SystemPowerControlInit
              SystemPowerOnDetect
  �޸���ʷ   :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "app_config.h"

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


#define 	POWER_KEY_JITTER_TIME		20			//����ʱ�䣬��ʱ������ؿ��ػ�Ӳ��ʱ���й�
#define 	POWER_KEY_CP_JITTER_TIME	500	
#define 	POWER_KEY_CP_HOLD_TIME		1000			

typedef enum _POWER_KEY_STATE
{
	POWER_KEY_STATE_IDLE,
	POWER_KEY_STATE_JITTER,
	POWER_KEY_STATE_PRESS_DOWN,
	POWER_KEY_STATE_CP,
	POWER_KEY_STATE_CP_UP,
} POWER_KEY_STATE;

static TIMER	PowerKeyWaitTimer;
POWER_KEY_STATE	PowerKeyState;

										/*������ʼ*/			/*�̰��ɿ�*/			/*������ʼ*/		/*��������*/		/*�����ɿ�*/
static const uint16_t PowerKeyEvent[5] = {MSG_NONE, 		MSG_NEXT, 			MSG_NONE,		MSG_NONE,		MSG_NONE,};

// POWER_KEY����ͨ�İ�����ͬ�����Ӱ�ť���أ����أ�ʱ����Ҫ���û���ϵͳ���ػ�����Ȼ��Ҳ�����ö̰����ܡ�
// �̰�����ʱ�����Ͷ̰���Ϣ�������̰����䣬�˴������κδ�����ϵͳӲ�����ػ���
uint16_t GpioKeyScan(void)							
{
	switch(PowerKeyState)
	{
		case POWER_KEY_STATE_IDLE:
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				return MSG_NONE;
			}
			else
			{	
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_JITTER_TIME);
				PowerKeyState = POWER_KEY_STATE_JITTER;
			}
			break;
            
		case POWER_KEY_STATE_JITTER:
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_PRESS_DOWN;
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CP_JITTER_TIME);
				return PowerKeyEvent[0];			//return key sp value
			}
			break;
			
		case POWER_KEY_STATE_PRESS_DOWN:
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
                /* �����̰���Ϣ */
				return PowerKeyEvent[1];
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				PowerKeyState = POWER_KEY_STATE_CP;
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CP_HOLD_TIME);
				return PowerKeyEvent[2];
			}
			break;
			
		case POWER_KEY_STATE_CP:
			//�˴�����֤һ�ΰ���������Ӧ��ζ̰�
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{
				PowerKeyState = POWER_KEY_STATE_IDLE;
				return PowerKeyEvent[4];//return MSG_NONE;				
			}
			else if(IsTimeOut(&PowerKeyWaitTimer))
			{
				/* ��⵽�ػ�����,ϵͳ����ػ������᷵��*/
				PowerKeyState = POWER_KEY_STATE_CP;
				TimeOutSet(&PowerKeyWaitTimer, POWER_KEY_CP_HOLD_TIME);
       			return PowerKeyEvent[3];//return MSG_NONE;
			}
			break;              
			
		default:
			PowerKeyState = POWER_KEY_STATE_IDLE;
			break;
	}
	return MSG_NONE;
}
/*****************************************************************************
 �� �� ��  : SystemPowerControlInit
 ��������  : ϵͳ��Դ���Ƴ�ʼ����
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��13��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void GpioKeyInit(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

