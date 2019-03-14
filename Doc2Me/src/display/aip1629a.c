//��������aip1629a.C��
//��д�ˣ�������
//��д���ڣ�2017.09.30
//ϵͳ���壺96MHz
//MCU���ͣ�M3
//ʹ��˵����
//�޸���־��
/*************************************************************************************/
//ͷ�ļ�����
#include "gpio.h"
#include "app_config.h"
#include "seg_panel.h"
#include "dev_detect_driver.h"
#include "sys_app.h"
#include "task_decoder.h"
#include "bt_app_func.h"
#include "bt_control_api.h"
#include "sd_card.h"
#include "eq.h"
#include "eq_params.h"
#include "clk.h"
#include "fsinfo.h"
#include "sys_vol.h"
#include "sound_remind.h"
#include "mixer.h"
#include "breakpoint.h"
#include "recorder_control.h"
#include "nvm.h"
#include "browser.h"
#include "lrc.h"
#include "timer_api.h"
#include "string_convert.h"
#include "player_control.h"
#include "rtc_control.h"
#include "radio_control.h"
#include "aip1629a.h"

#ifdef    FUNC_AIP1629A_LED_EN

/***********************************************************************************/
//��������������
/***********************************************************************************/
uint8_t LedDispPwmLevel = LED_DISP_PWM_14;

uint8_t const Led48IconSet[ICON_NUM] = {
	ICON48_USB,   ICON48_TF,    ICON48_AUX,    ICON48_FM,       ICON48_BLUE,    0xFF,
	ICON48_BAT1,  ICON48_BAT2,  ICON48_BAT3,   ICON48_BATFUL,   ICON48_DOTNO,   ICON48_COLON,
	0xFF, 		  0xFF, 		0xFF, 		   0xFF, 			ICON48_24H_12H, 0xFF,
	0xFF,		  0xFF,			0xFF,		   0xFF,			ICON48_A1MARK,	ICON48_A2MARK,
	ICON48_ALARM1,ICON48_ALARM2,ICON48_A1LOOP, ICON48_A2LOOP,
};

uint8_t const Led48IconTemp[ICON_NUM] = {
	ICON_COM_USB,     ICON_COM_TF,    ICON_COM_AUX,    ICON_COM_FM,       ICON_COM_BLUE,    0xFF,
	ICON48_BAT1,      ICON48_BAT2,    ICON48_BAT3,     ICON48_BATFUL,     ICON48_DOTNO,     ICON_COM_COLON,
	0xFF, 		      0xFF, 		  0xFF, 		   0xFF, 			  ICON_COM_24H_12H,	0xFF,
	0xFF,		      0xFF,			  0xFF,		       0xFF,			  ICON_COM_A1MARK,	ICON_COM_A2MARK,
	ICON48_ALARM1,    ICON48_ALARM2,  ICON48_A1LOOP,   ICON48_A2LOOP,
};


/*************************************************************************************/
//��ʱ����
/***********************************************************************************/

static void AIP1629ADisplayDelay(uint8_t Delay)
{
	uint16_t j;
	
	while(Delay)
		{
			j = 2;
			while(j)
			{
				j--;
			}
			Delay--;
		}
}

/*************************************************************************************/
//AIP1629A SPI��ʼ������
/***********************************************************************************/

static  void  Aip1629aSpiStart(void)
{
    AIP1629A_CS_SET();
    AIP1629A_CLK_SET();
    AIP1629A_DAT_SET();
    AIP1629ADisplayDelay(AIP1629A_DELAY_US);
    AIP1629A_CS_CLR();
    AIP1629ADisplayDelay(AIP1629A_DELAY_US);
    AIP1629A_CLK_CLR();
}

/*************************************************************************************/
//AIP1629A SPI��������
/***********************************************************************************/

static  void  Aip1629aSpiEnd(void)
{
    AIP1629A_CS_SET();
    AIP1629ADisplayDelay(AIP1629A_DELAY_US);
    AIP1629A_CLK_SET();
    AIP1629A_DAT_SET();
}

/*************************************************************************************/
//�����ֽ�д��AIP1629A����λ��ǰ
/***********************************************************************************/

static  void  SendOneCharToAip1629a(uint8_t Data)                
{
	uint8_t count;
	Aip1629aSpiStart();
	for (count = 8;count > 0;count--)
	{
		AIP1629A_CLK_CLR();
		if ((Data&0x01) == 0x00)
		   AIP1629A_DAT_CLR();
		else
		   AIP1629A_DAT_SET();
		AIP1629ADisplayDelay(AIP1629A_DELAY_US);
		AIP1629A_CLK_SET();
		AIP1629ADisplayDelay(AIP1629A_DELAY_US);
		Data >>= 1;
	}
	Aip1629aSpiEnd();
}

/*************************************************************************************/
//����д��AIP1629A����λ��ǰ
/***********************************************************************************/

static  void  SendCharToAip1629a(uint8_t dat)                
{
	uint8_t count;
	for (count = 8;count > 0;count--)
	{
		AIP1629A_CLK_CLR();
		if ((dat&0x01) == 0x00)
		   AIP1629A_DAT_CLR();
		else
		   AIP1629A_DAT_SET();
		AIP1629ADisplayDelay(AIP1629A_DELAY_US);
		AIP1629A_CLK_SET();
		AIP1629ADisplayDelay(AIP1629A_DELAY_US);
		dat >>= 1;
	}
}

/*************************************************************************************/
//����д��AIP1629A����λ��ǰ,��ַ�Զ�����ģʽ
/***********************************************************************************/

void  AutoCharToAip1629a(uint8_t Adder, uint8_t* DataBuf, uint8_t Len)                
{
	uint8_t count;

	Aip1629aSpiStart();
	SendCharToAip1629a(DATA_ADDR_AUTO);            //д����1������ģʽ����ַ�Զ�����
	Aip1629aSpiEnd();
	AIP1629ADisplayDelay(AIP1629A_DELAY_US);                        //�ʵ���ʱ
	Aip1629aSpiStart();
	SendCharToAip1629a(Adder);                     //д����2������д����׵�ַ
	for (count = 0;count < Len;count++)            //����д��Len���ֽ�
	{
		SendCharToAip1629a(DataBuf[count]);
	}
	Aip1629aSpiEnd();
}

/*************************************************************************************/
//����д��AIP1629A����λ��ǰ����ַ�̶�ģʽ
/***********************************************************************************/

void  FixedCharToAip1629a(uint8_t Adder, uint8_t Data)                
{
	Aip1629aSpiStart();
	SendCharToAip1629a(DATA_ADDR_FIXED);           //д����1������ģʽ����ַ�̶�ģʽ
	Aip1629aSpiEnd();
	AIP1629ADisplayDelay(AIP1629A_DELAY_US);                        //�ʵ���ʱ
	Aip1629aSpiStart();
	SendCharToAip1629a(Adder);                     //д����2������д����׵�ַ
	SendCharToAip1629a(Data);                      //д����
	Aip1629aSpiEnd();
}

/*************************************************************************************/
//���1629������ʾ�����������������
/***********************************************************************************/
void  AIP1629ADisplayClr(void)    
{
	uint8_t count = 0;

	Aip1629aSpiStart();
	SendCharToAip1629a(DATA_ADDR_AUTO);            //д��������1������ģʽ����ַ�Զ�����
	Aip1629aSpiEnd();
	AIP1629ADisplayDelay(AIP1629A_DELAY_US);                        //�ʵ���ʱ
	Aip1629aSpiStart();
	SendCharToAip1629a(ADDR_STA_OFFSET);           //д��ַ����2������д����׵�ַ
	for(count = 0; count < MAX_SEND_DATA_LEN; count++)
	{
		SendCharToAip1629a(0x00);
	}
	Aip1629aSpiEnd();
}

/************************************************************************************/
////��ʼ��AIP1629A������ʾ��ӭ��Ϣ
/***********************************************************************************/

void  AIP1629ADisplayInit(void)                        
{
    //memset(gDispBuff, 0, sizeof(gDispBuff));
	AIP1629ADisplayClr();
	SendOneCharToAip1629a(LedDispPwmLevel);
}

/*************************************************************************************/
//AIP1629A ������������
/*************************************************************************************/

void  AIP1629A_BackLightPwmSet(bool IsOnOff)    						//ˢ��AIP1629A��ʾ?
{
	uint8_t const PWM_buf[3] = {LED_DISP_PWM_01, LED_DISP_PWM_04,LED_DISP_PWM_14};
	static uint8_t PwmSetCnt = 2;

	if(IsOnOff == TRUE)
	{
		LedDispPwmLevel = (LedDispPwmLevel == LED_DISP_POWOFF)? PWM_buf[PwmSetCnt] : LED_DISP_POWOFF;                             //����ʾ��
	}
	else
	{
		PwmSetCnt--;
		if(PwmSetCnt > 2)
		{
			PwmSetCnt = 2;
		}
		LedDispPwmLevel = PWM_buf[PwmSetCnt];
	}
}

/*************************************************************************************/
//AIP1629A ��־λ��������Ϊ��Ŀʹ�ö��SING����ͬһ���ʶΣ�����Ҫ�����ݿ���
/*************************************************************************************/

void  AIP1629A_FlushIconCpy(void) 
{
	uint8_t IconVal,IconValTemp;
	uint8_t IconPos,IconPosTemp;
	uint8_t count;

	for(count = 0; count < ICON_NUM; count++)
	{
		IconVal = Led48IconSet[count];
		IconValTemp = Led48IconTemp[count];
	
		if(IconVal != 0xFF)
		{
			IconPos = IconVal / 8;
			IconVal = (1 << (IconVal % 8));
			IconVal = (gDispBuff[IconPos]&(DISP_TYPE)(IconVal));

			IconPosTemp = IconValTemp / 8;
			IconValTemp = (1 << (IconValTemp % 8));

			if(IconVal != FALSE)
			{
				gDispBuff[IconPosTemp] |= (DISP_TYPE)IconValTemp;
			}
			else
			{
				gDispBuff[IconPosTemp] &= (DISP_TYPE)(~IconValTemp);
			}
		}
	}
}

/*************************************************************************************/
//AIP1629A ���ݿ�������Ϊ��Ŀʹ�ö��SING����ͬһ���ʶΣ�����Ҫ�����ݿ���
/*************************************************************************************/

void  AIP1629A_FlushDataCpy(void) 
{
	uint8_t const DispOrd[4] = {0, 4, 8, 12};
	uint8_t const DispPos[4] = {2, 6, 10, 14};
	uint8_t count,CharMask;
	for(count = 0; count < 4; count++)
	{
		gDispBuff[DispPos[count]] = gDispBuff[DispPos[count]]&0x80;            //����ICONλ
		CharMask = gDispBuff[DispOrd[count]]&0x7F;                             //������Ҫ���ƶ���λ��
		gDispBuff[DispPos[count]] = CharMask | gDispBuff[DispPos[count]];      //ICON/����λ����
	}
}

/*************************************************************************************/
//AIP1629A ���ݿ�������Ϊ��Ŀʹ�ö��SING����ͬһ���ʶΣ�����Ҫ�����ݿ���
/*************************************************************************************/

void  AIP1629A_FlushDispData(void)    									//ˢ��AIP1629A��ʾ?
{
	AIP1629A_FlushDataCpy();          									//���ݿ�������ͬ�ʶ���������Ϊͬ����
	AIP1629A_FlushIconCpy();
	AutoCharToAip1629a(ADDR_STA_OFFSET, gDispBuff, MAX_SEND_DATA_LEN);  //ˢ����ʾ����
	SendOneCharToAip1629a(LedDispPwmLevel);                             //������ʾ���ȣ�����ʾ��
}

#endif
