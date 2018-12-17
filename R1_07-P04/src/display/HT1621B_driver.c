//��������HT1621B_driver.C��
//��д�ˣ�������
//��д���ڣ�2015.09.30
//ϵͳ���壺96MHz
//MCU���ͣ�M3
//�ӿڶ��壺HT_dat-1621��data�����ߣ�HT_wr-1621��дʱ��ͬ���ߣ�HT_cs-1621��ʹ��/��λ�ߣ�
//          HT_rd-1621�Ķ�ʱ��ͬ���ߡ�
//ʹ��˵����
//�޸���־��
/*************************************************************************************/
//ͷ�ļ�����
#include "gpio.h"
#include "HT1621B_driver.h"
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

#ifdef    FUNC_HT1621_DISPLAY_EN
/***********************************************************************************/
//���ÿ�������
#define   SYS_dis    0x00   //�ر�ϵͳʱ��
#define   SYS_on     0x01   //��ϵͳʱ��
#define   LCD_off    0x02   //�ر�LCD��ʾ
#define   LCD_on     0x03   //��LCD��ʾ
#define   timer_dis  0x04   //�ر�ʱ�����
#define   WDT_dis    0x05   //�رտ��Ź�
#define   tone_off   0x08   //�ر��������
#define   XTAL       0x18   //ϵͳ����ѡ��
#define   BIAS       0x29   //ƫѹ��ռ�ձ�����

TIMER  ColDispTimer;
TIMER  PrevDispTimer;
TIMER  UpdataDispTimer;
DISP_MODULE_ID    CurDisplayId  =  DISP_MODULE_NONE;              //��ǰ��Ҫ��ʾ������
 //��һ����ʾ�����ݣ����ڵ�ĳ��������Ҫ��ʾ�̶�ʱ�䣬����ʾ�����в���ˢ�µ�ǰ��ʾ���ݵ�����
static DISP_MODULE_ID    PrevDisplayId = DISP_MODULE_NONE;
static bool ColDispFlag;

static uint8_t const NumberTab [10] = {0xFD,0x0D,0xDB,0x9F,0x2F,0xBE,0xFE,0x1D,0xFF,0xBF};
//.......................................0.....1...2....3....4....5....6.....7....8....9.//
//Ϊ����ˢ����ʾ����ʱ��Ӱ��������־����ʾ�����Խ�����BITΪ1����ˢ������ʱ�á��߼��롱

uint8_t  DisplayBuffer [13] = {0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x67,0x00,0x00,0x00,0x00,0x00}; 	

/*************************************************************************************/
#ifdef FUNC_HT1621_DISPLAY_EN
static __INLINE void HT1621DisplayDelay(void)
{
	__nop();
}
#endif
/*************************************************************************************/
static  void  Init_1621(void)
{
      HT1621_CS_SET();
	    HT1621_CLK_SET();
	    HT1621_DAT_SET();
	    HT1621DisplayDelay();
	    HT1621_CS_CLR();
	    HT1621_CS_CLR();
	    HT1621_CLK_CLR();
	    HT1621_CLK_CLR();
}
/*************************************************************************************/
static  void  Sendchar_h(uint8_t dat,uint8_t count)                 //dat��countλд��1621����λ��ǰ
{
      for (;count > 0;count--)
	    {
          if ((dat&0x80) == 0x00)
	           HT1621_DAT_CLR();
	        else
	           HT1621_DAT_SET();
	       HT1621_CLK_CLR();
	       HT1621DisplayDelay();
	       HT1621_CLK_SET();
	       dat <<= 1;
      }
}
/*************************************************************************************/
static  void  Sendchar_l(uint8_t dat,uint8_t count)                 //dat��countλд��1621����λ��ǰ
{
      for (;count > 0;count--)
	    {
          if ((dat&0x01) == 0x00)
	           HT1621_DAT_CLR();
	        else
	           HT1621_DAT_SET();
	       HT1621_CLK_CLR();
	       HT1621DisplayDelay();
	       HT1621_CLK_SET();
	       dat >>= 1;
      }
}
/*************************************************************************************/
static  void  write_cmd(uint8_t command)                          //д1621��������
{   
      HT1621_CS_CLR();
	    Init_1621();
      Sendchar_h(0x80,3);
      Sendchar_h(command,9);
      HT1621_CS_SET();
}
/*************************************************************************************/
void  HT1621DisplayClr(void)    //���1621������ʾ������
{
	    uint8_t count = 0;
	    for(count = 0; count < 13; count++)
			{
				DisplayBuffer[count] = 0x00;
			}
}
/*************************************************************************************/
void  SendHt1621DispMsg(DISP_MODULE_ID  DispId)               //������ʾ����
{
	    CurDisplayId = DispId;
}
/*************************************************************************************/
static void  DisplayModuleFlag(void)    //ˢ��ģʽ�����־
{
	     //���������ģʽͼ�ꡣ
	    DISP_MODEFLAG_OFF();
	    if(gSys.CurModuleID != MODULE_ID_RTC)
			//���������RTCģʽ������ͼ��رա�
			{
				 DISP_COL_OFF();
			}
	    if(gSys.CurModuleID == MODULE_ID_PLAYER_USB)
				//��ʾUSBͼ��
			{
				 DISP_USB_ON();
			}
			else if(gSys.CurModuleID == MODULE_ID_PLAYER_SD)
			{
				 if((GetCurReadCard() == TFTOSD_CARD_TYPE) || \
					   (GetCurReadCard() == SD_CARD_TYPE))
				 //��SD��ģʽ����ʾSDͼ��
				 {
					   DISP_SDCARD_ON();
				 }
				 else
					//��TF��ģʽ����ʾTFͼ�ꡣ
				 {
					   DISP_TFCARD_ON();
				 }
			}
			else if (gSys.CurModuleID == MODULE_ID_RADIO)
			//��ʾFMͼ��
			{
				  DISP_FM_ON();
			}
}
/************************************************************************************/
static void DisplayWelcome(void)        //��ʾ��ӭ��Ϣ
{
	    uint8_t  count = 0;
			HT1621_CS_CLR();
	    Init_1621();
      Sendchar_h(0xa0,3);
      Sendchar_h(0x18,6);
      for (count = 0;count < 13 ;count++)
	    {
			  if(count == 6)
				{
           Sendchar_l(0x05,8);
				}
				else if(count == 7)
				{
					 Sendchar_l(0x67,8);
				}
				else
				{
					 Sendchar_l(0x00,8);
				}
      }
      HT1621_CS_SET();
			TimeOutSet(&PrevDispTimer, 3000);
}
/************************************************************************************/
void  HT1621DisplayInit(void)                         //��ʼ��HT1621������ʾ��ӭ��Ϣ
{
      write_cmd(BIAS);                                        //LCDƫѹ����
      write_cmd(XTAL);                                      //ʱ��Դѡ��
	    write_cmd(timer_dis);                               //�ر�ʱ�����
	    write_cmd(WDT_dis);                                //�رտ��Ź�
	    write_cmd(tone_off);                                  //�ر��������
      write_cmd(SYS_on);                                  //��ϵͳʱ��
      HT1621DisplayDelay();                              //�ʵ���ʱ
	    TimeOutSet(&ColDispTimer, 0);
	    TimeOutSet(&PrevDispTimer, 0);           //������ʾ��ض�ʱ��
      write_cmd(LCD_on);                                  //��LCD��ʾ
	    DisplayWelcome();                                  //��ʾ��ӭ��Ϣ
}
/************************************************************************************/
static void DisplayTimer(void)        //��ʾ����ʱ��
{
	        DISP_COL_ON();                       //����ų�������ʾ���ڲ���
	        DisplayBuffer[8] |= 0xF7;
				  DisplayBuffer[8] &= NumberTab[gPlayContrl->CurPlayTime / 1000 / 60 / 10];
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &= NumberTab[gPlayContrl->CurPlayTime/ 1000 / 60 % 10];
				  DisplayBuffer[6] |= 0xF7;
	        DisplayBuffer[6] &= NumberTab[gPlayContrl->CurPlayTime / 1000 % 60 / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[gPlayContrl->CurPlayTime / 1000 % 60 % 10];
}
/************************************************************************************/
static void DispSongNumber(void)        //��ʾ������Ŀ
{
	        DisplayBuffer[8] |= 0xF7;
					DisplayBuffer[8] &= NumberTab[gPlayContrl->CurFileIndex / 1000 ];
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &= NumberTab[gPlayContrl->CurFileIndex % 1000 / 100];
				  DisplayBuffer[6] |= 0xF7;
	        DisplayBuffer[6] &= NumberTab[gPlayContrl->CurFileIndex % 100  / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[gPlayContrl->CurFileIndex % 10];
	        TimeOutSet(&PrevDispTimer, 2000);
}
/************************************************************************************/
static void DispAudioVol(void)        //������
{
	        DisplayBuffer[8] |= 0xF7;
					DisplayBuffer[8] &= 0xED;    // ��ĸ"U"
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &= 0xE8;     //��ĸ��L��
				  DisplayBuffer[6] |= 0xF7;
	        DisplayBuffer[6] &= NumberTab[(uint8_t)gSys.Volume  / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[(uint8_t)gSys.Volume % 10];
	        TimeOutSet(&PrevDispTimer, 2000);
}
/************************************************************************************/
static void DispRtcTimer(void)        //��ʾʵʱʱ����Ϣ�
{
	     //��ʾ����Ϣ
	     if((GetRtcControlState() != RTC_STATE_IDLE) && (GetRtcControlSubState() == RTC_SET_YEAR)\
				   && (ColDispFlag == FALSE))
			 //��ǰ����������ݣ���˸��ݱ�־����������
			 {
				  DisplayBuffer[12] |= 0xD7;
					DisplayBuffer[12] &= DISP_HIDDEN | 0x20;
					DisplayBuffer[11] |= 0xF7;
					DisplayBuffer[11] &= DISP_HIDDEN;
				  DisplayBuffer[10] |= 0xF7;
					DisplayBuffer[10] &= DISP_HIDDEN;
				  DisplayBuffer[9] |= 0xF7;
					DisplayBuffer[9] &= DISP_HIDDEN;
			 }
			 else
			 {
					DisplayBuffer[12] |= 0xD7;
					DisplayBuffer[12] &= NumberTab[sRtcControl->DataTime.Year / 1000] | 0x20;
					DisplayBuffer[11] |= 0xF7;
					DisplayBuffer[11] &= NumberTab[sRtcControl->DataTime.Year % 1000 / 100];
				  DisplayBuffer[10] |= 0xF7;
					DisplayBuffer[10] &= NumberTab[sRtcControl->DataTime.Year % 100 / 10];
				  DisplayBuffer[9] |= 0xF7;
					DisplayBuffer[9] &= NumberTab[sRtcControl->DataTime.Year % 10];
			 }
			  //��ʾ����Ϣ
			 if((GetRtcControlState() != RTC_STATE_IDLE) && (GetRtcControlSubState() == RTC_SET_MON)\
				   && (ColDispFlag == FALSE))
			 //�����·���ʾ
			 {
				  DISP_MONDEC_OFF();
				  DisplayBuffer[4] |= 0x0F;
					DisplayBuffer[4] &= ((DISP_HIDDEN >> 4) | 0xF0);
					DisplayBuffer[3] |= 0x70;
					DisplayBuffer[3] &= ((DISP_HIDDEN << 4) | 0x0F);
					DISP_MON_OFF();
			 }
			 else
			 //��ʾ����Ϣ
			 {
				  if((sRtcControl->DataTime.Mon / 10) > 0)
					{
						DISP_MONDEC_ON();
					}
					else
					{
						DISP_MONDEC_OFF();
					}
					DisplayBuffer[4] |= 0x0F;
					DisplayBuffer[4] &= ((NumberTab[sRtcControl->DataTime.Mon % 10] >> 4) | 0xF0);
					DisplayBuffer[3] |= 0x70;
					DisplayBuffer[3] &= ((NumberTab[sRtcControl->DataTime.Mon % 10] << 4) | 0x0F);
					DISP_MON_ON();
			 }
			 //��ʾ����Ϣ
			 if((GetRtcControlState() != RTC_STATE_IDLE) && (GetRtcControlSubState() == RTC_SET_DAY)\
				   && (ColDispFlag == FALSE))
			 //��������ʾ��Ϣ
			 {
				  DisplayBuffer[2] |= 0xF7;
					DisplayBuffer[2] &= DISP_HIDDEN;
					DisplayBuffer[1] |= 0xF7;
					DisplayBuffer[1] &= DISP_HIDDEN;
					DISP_DATE_OFF();
			 }
			 else
			 //��ʾ����Ϣ
			 {
					DisplayBuffer[2] |= 0xF7;
					DisplayBuffer[2] &= ((sRtcControl->DataTime.Date / 10) == 0)? DISP_HIDDEN : (NumberTab[sRtcControl->DataTime.Date / 10]);
					DisplayBuffer[1] |= 0xF7;
					DisplayBuffer[1] &= NumberTab[sRtcControl->DataTime.Date % 10];
					DISP_DATE_ON();
			 }
			 //��ʾСʱ��Ϣ
			 if((GetRtcControlState() != RTC_STATE_IDLE) && (GetRtcControlSubState() == RTC_SET_HOUR)\
				   && (ColDispFlag == FALSE))
			 //����Сʱ
			 {
					DisplayBuffer[8] &= DISP_HIDDEN;
	        DisplayBuffer[7] &= DISP_HIDDEN;
			 }
			 else
			 //��ʾСʱ
			 {
					DisplayBuffer[8] |= 0xF7;
					DisplayBuffer[8] &= NumberTab[sRtcControl->DataTime.Hour / 10];
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &= NumberTab[sRtcControl->DataTime.Hour % 10];
			 }
			 //��ʾ������Ϣ
			 if((GetRtcControlState() != RTC_STATE_IDLE) && (GetRtcControlSubState() == RTC_SET_MIN)\
				   && (ColDispFlag == FALSE))
			 //��������
			 {
				  DisplayBuffer[6] &= DISP_HIDDEN;
	        DisplayBuffer[5] &= DISP_HIDDEN;
			 }
			 else
			 //��ʾ����
			 {
				  DisplayBuffer[6] |= 0xF7;
	        DisplayBuffer[6] &= NumberTab[sRtcControl->DataTime.Min / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[sRtcControl->DataTime.Min % 10];
			 }
}
/************************************************************************************/
static void DisplayRadioFreq(void)   //��ʾFMƵ��
{
#ifdef FUNC_RADIO_EN
	        DISP_DOT_ON();                           //��С����                 
				  DISP_MHz_ON();                           //��Ƶ�ʵ�λ��ʾ       
	        DisplayBuffer[8] |= 0xF7;
	        DisplayBuffer[8] &= (sRadioControl->Freq / 1000 == 0)?DISP_HIDDEN : NumberTab[sRadioControl->Freq / 1000 ];
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &=  (sRadioControl->Freq / 100 == 0)?DISP_HIDDEN : NumberTab[sRadioControl->Freq % 1000 / 100];
				  DisplayBuffer[6] |= 0xF7;
	        DisplayBuffer[6] &=  (sRadioControl->Freq / 10 == 0)?DISP_HIDDEN : NumberTab[sRadioControl->Freq % 100  / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[sRadioControl->Freq % 10];   
#endif	
}
/************************************************************************************/
static void DisplayRadioNumber(void)   //��ʾFM��̨Ŀ¼
{
#ifdef FUNC_RADIO_EN
	        DisplayBuffer[8] |= 0xF7;
	        DisplayBuffer[8] &= DISP_HIDDEN;   //����
				  DisplayBuffer[7] |= 0xF7;
	        DisplayBuffer[7] &=  0x7B;                   //��ĸ��P��
				  DisplayBuffer[6] |= 0xF7; 
	        DisplayBuffer[6] &=  NumberTab[((uint8_t)sRadioControl->CurStaIdx +1) / 10];
				  DisplayBuffer[5] |= 0xF7;
	        DisplayBuffer[5] &= NumberTab[((uint8_t)sRadioControl->CurStaIdx+1)  % 10]; 
          TimeOutSet(&PrevDispTimer, 1000);	
#endif
}
/************************************************************************************/
static void DisplayString(void)   //��ʾ���ģʽ�µ��ַ�����Ϣ
{
	        if(gSys.CurModuleID == MODULE_ID_BLUETOOTH)
					{
	            DisplayBuffer[8] |= 0xF7;
	            DisplayBuffer[8] &= 0xEE;                    //��ĸ��b��
				      DisplayBuffer[7] |= 0xF7;
	            DisplayBuffer[7] &=  0xE8;                   //��ĸ��L��
				      DisplayBuffer[6] |= 0xF7; 
	            DisplayBuffer[6] &=  0xED;                   //��ĸ��U��
				      DisplayBuffer[5] |= 0xF7;
	            DisplayBuffer[5] &= 0xFA;                    //��ĸ��E��
					}
					else if(gSys.CurModuleID == MODULE_ID_PLAYER_SD || gSys.CurModuleID == MODULE_ID_PLAYER_USB)
					{
						  DisplayBuffer[8] |= 0xF7;
	            DisplayBuffer[8] &= 0x7B;                    //��ĸ��p��
				      DisplayBuffer[7] |= 0xF7;
	            DisplayBuffer[7] &=  0x7F;                   //��ĸ��A��
				      DisplayBuffer[6] |= 0xF7; 
	            DisplayBuffer[6] &=  0xED;                   //��ĸ��U��
				      DisplayBuffer[5] |= 0xF7;
	            DisplayBuffer[5] &= 0xBE;                    //��ĸ��S��
					}
					else if(gSys.CurModuleID == MODULE_ID_LINEIN)
					{
						  DisplayBuffer[8] |= 0xF7;
	            DisplayBuffer[8] &= DISP_HIDDEN;    //����
				      DisplayBuffer[7] |= 0xF7;
	            DisplayBuffer[7] &=  0x7F;                   //��ĸ��A��
				      DisplayBuffer[6] |= 0xF7; 
	            DisplayBuffer[6] &=  0xED;                  //��ĸ��U��
				      DisplayBuffer[5] |= 0xF7;
	            DisplayBuffer[5] &= 0x6F;                    //��ĸ��H��
					}
}
/************************************************************************************/
static void  HT1621DisplaySet(void)
{
	   //��ʾͣ����ʱ�䵽�ˣ����ߵ�ǰ������ʾ��������Ҫˢ��
	  if(IsTimeOut(&PrevDispTimer) || CurDisplayId == PrevDisplayId)
		{
			  DISP_RTCDATE_OFF();                                      //���������Ϣ
			  DISP_MAINFLAG_OFF();                                    //�ر�����Ļ������ͼ��
			  switch(CurDisplayId)
				{
					case  DISP_MODULE_PLAYTIMER:              //��ʾ����ʱ��
						 DisplayTimer();
					   break;
					case  DISP_MODULE_SONGNUM:                //��ʾ������Ŀ
						DispSongNumber();
					  break;
					case  DISP_MODULE_AUDIOVOL:                //��ʾ����������Ϣ
						DispAudioVol();
					  break;
					case  DISP_MODULE_RTCTIMER:                //��ʾʵʱʱ����Ϣ
						DispRtcTimer();
						break;
					case  DISP_MODULE_RADIOFREQ:             //��ʾFM��̨Ƶ��
						DisplayRadioFreq();
            break;	
          case  DISP_MODULE_RADIONUM:              //��ʾFM��̨Ŀ¼
						DisplayRadioNumber();
            break;					
          case  DISP_MODULE_WELCOME:               //��ʾ��ӭ��Ϣ
						//DisplayWelcome();
            break;
          case  DISP_MODULE_STRING:                     //��ʾ���ģʽ�µ��ַ���
						DisplayString();
            break;					
					default:
						break;
				}
				PrevDisplayId = CurDisplayId;
				CurDisplayId = DISP_MODULE_NONE;
		}
}
/*************************************************************************************/
void  HT1621UpdataDisplay(void)    //ˢ��HT1621��ʾ�
{
	    uint8_t  count = 0;
	    static bool DisplayState;
	    //�����ж��Ƿ�����Ч����Ϣ��Ҫ��ʾ
	    if(CurDisplayId  !=  DISP_MODULE_NONE && CurDisplayId  !=  DISP_MODULE_WELCOME)
			{
				  DisplayModuleFlag();
	        HT1621DisplaySet();
			}
			//�ж��Ƿ���Ҫ��ʾ��ͼ�ꡣ
		 if(IsTimeOut(&ColDispTimer) && IsTimeOut(&PrevDispTimer) && (gSys.CurModuleID == MODULE_ID_RTC))
		 {
			    //���ﲻ�ϴ�����ʾʵʱʱ�������Ϊ��������״̬ʱ��Ӧ����λ��˸���������ȥ��
			    SendHt1621DispMsg(DISP_MODULE_RTCTIMER);
				  TimeOutSet(&ColDispTimer, 500);
				  ColDispFlag = !ColDispFlag;
				  if(ColDispFlag )
					{
						 DISP_COL_ON();
						//��������״̬������ͼ����˸
						 if(GetRtcControlState() == RTC_STATE_SET_ALARM)
						 {
							 DISP_ALARM_ON();
						 }
					}
					else
					{
						 DISP_COL_OFF();
						//��������״̬������ͼ����˸
						 if(GetRtcControlState() == RTC_STATE_SET_ALARM)
						 {
							 DISP_ALARM_OFF();
						 }
					}
			}
			//�����ֹдHT1621ʱ�����������⣬ʱ�ӻ��Ҷ�����ʾ
	    if(DisplayState == FALSE)                        
		  {
				  DisplayState = TRUE;
				  HT1621_CS_CLR();
	        Init_1621();
          Sendchar_h(0xa0,3);
          Sendchar_h(0x18,6);
          for (count = 0;count < 13 ;count++)
	        {
                Sendchar_l(DisplayBuffer[count],8);
          }
          HT1621_CS_SET();
				  DisplayState = FALSE;
			}
}

#endif
