///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: rtc_control.c
// maintainer: Sam
///////////////////////////////////////////////////////////////////////////////
#include "app_config.h"
#include "os.h"
#include "rtc.h"
#include "sys_app.h"
#include "recorder_control.h"
#include "dev_detect_driver.h"
#include "sound_remind.h"
#include "nvm.h"
#include "sys_vol.h"
#include "breakpoint.h"
#include "rtc_control.h"

#ifdef FUNC_WIFI_EN
#include "wifi_control.h"
#endif
#ifdef FUNC_DISP_EN
#include "led_display.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#endif

#ifdef FUNC_RTC_EN
RTC_CONTROL* sRtcControl = NULL;
NVM_RTC_INFO sNvmRtcInfo; 
static TIMER RtcAutoOutTimer;

//��ز����궨��
#define RTC_AUTO_OUT_TIME	         15000                 //�������Զ��˳�ʱ�䣺��λmS
#define RTC_ALARM_RING_INTERVAL      1                     //��������ʱ�����������λS
#define RTC_ALARM_RING_TIME          60                    //�����������ʱ�䣺 ��λS
#define RTC_SECOND_ALARM_TIME        9                     //����̰˯ʱ�䣺��λmin
#define RTC_ALARM_MAX_COUNT          3                     //����ѭ��������

////////////////////////////////////
const RTC_DATE_TIME gSysCurDate = {2018, 01, 01, 01, 00, 00, 00};
const uint8_t Alarm_Ring[7] = {0xff, SOUND_ALARM_RING1, SOUND_ALARM_RING2, SOUND_ALARM_RING3, SOUND_ALARM_RING4, SOUND_ALARM_RING5, 0xff};


#ifdef FUNC_RTC_LUNAR
static void DisplayLunarDate(void)
{
	//ũ���������
	const uint8_t LunarYearName[12][2] = {"��", "ţ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};
	
	//ũ���·�����
	const uint8_t LunarMonthName[12][2] = {"��", "��", "��", "��", "��", "��", "��", "��", "��", "ʮ", "��", "��"};
	
	//ũ����������
	const uint8_t LunarDateName[30][4] = {"��һ", "����", "����", "����", "����", "����", "����", "����", "����", "��ʮ", 
									  "ʮһ", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "��ʮ",
									  "إһ", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "��ʮ"};
	
	//ũ���������
	const uint8_t HeavenlyStemName[10][2] = {"��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};
	
	//ũ����֧����
	const uint8_t EarthlyBranchName[12][2] = {"��", "��", "��", "î", "��", "��", "��", "δ", "��", "��", "��", "��"};
	
	RTC_LUNAR_DATE LunarDate;

	SolarToLunar(&sRtcControl->DataTime, &LunarDate);	
	APP_DBG("ũ�� %d�� ", (uint32_t)LunarDate.Year);
	APP_DBG("%-.2s%-.2s�� ", HeavenlyStemName[GetHeavenlyStem(LunarDate.Year)], 
						  EarthlyBranchName[GetEarthlyBranch(LunarDate.Year)]);
	APP_DBG("%-.2s�� ", LunarYearName[GetEarthlyBranch(LunarDate.Year)]);
	if(LunarDate.IsLeapMonth)
	{
		APP_DBG("��");
	}
	APP_DBG("%-.2s��", LunarMonthName[LunarDate.Month - 1]);
	
	if(LunarDate.MonthDays == 29)
	{
		APP_DBG("(С)");
	}
	else
	{
		APP_DBG("(��)");
	}
	
	APP_DBG("%-.4s ", LunarDateName[LunarDate.Date - 1]);

	if((LunarDate.Month == 1) && (LunarDate.Date == 1))			//����
	{
		APP_DBG("����");
	}
	else if((LunarDate.Month == 1) && (LunarDate.Date == 15))	//Ԫ����
	{
		APP_DBG("Ԫ����");
	}
	else if((LunarDate.Month == 5) && (LunarDate.Date == 5))	//�����
	{
		APP_DBG("�����");
	}
	else if((LunarDate.Month == 7) && (LunarDate.Date == 7))	//��Ϧ���˽�
	{
		APP_DBG("��Ϧ���˽�");
	}
	else if((LunarDate.Month == 7) && (LunarDate.Date == 15))	//��Ԫ��
	{
		APP_DBG("��Ԫ��");
	}
	else if((LunarDate.Month == 8) && (LunarDate.Date == 15))	//�����
	{
		APP_DBG("�����");
	}
	else if((LunarDate.Month == 9) && (LunarDate.Date == 9))	//������
	{
   		APP_DBG("������");
	}
	else if((LunarDate.Month == 12) && (LunarDate.Date == 8))	//���˽�
	{
	 	APP_DBG("���˽�");
	}
	else if((LunarDate.Month == 12) && (LunarDate.Date == 23))	//С��
	{
		APP_DBG("С��");
	}
	else if((LunarDate.Month == 12) && (LunarDate.Date == LunarDate.MonthDays))	//��Ϧ
	{
		APP_DBG("��Ϧ");
	}
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RtcTimerCB(uint32_t unused)
{
	RtcGetCurrTime(&sRtcControl->DataTime);

	if(0)//(RTC_STATE_IDLE == sRtcControl->State) || (sRtcControl->SubState == RTC_SET_IDLE))
	{
		APP_DBG("RtcTime(%04d-%02d-%02d %02d:%02d:%02d) Week:%d ",
	        sRtcControl->DataTime.Year, sRtcControl->DataTime.Mon, sRtcControl->DataTime.Date,
	        sRtcControl->DataTime.Hour, sRtcControl->DataTime.Min, sRtcControl->DataTime.Sec,
	        sRtcControl->DataTime.WDay);
#ifdef FUNC_RTC_LUNAR
		DisplayLunarDate();
#endif
		APP_DBG("\n");
	}
	else if((RTC_STATE_SET_TIME == sRtcControl->State) && (sRtcControl->SubState != RTC_SET_IDLE))
	{
		APP_DBG("RTC set Time(%04d-%02d-%02d %02d:%02d:%02d) Week:%d ",
	        sRtcControl->DataTime.Year, sRtcControl->DataTime.Mon, sRtcControl->DataTime.Date,
	        sRtcControl->DataTime.Hour, sRtcControl->DataTime.Min, sRtcControl->DataTime.Sec,
	        sRtcControl->DataTime.WDay);
#ifdef FUNC_RTC_LUNAR
		DisplayLunarDate();
#endif
		APP_DBG("\n");
	}
#ifdef FUNC_RTC_ALARM	
	else if((RTC_STATE_SET_ALARM == sRtcControl->State) && (sRtcControl->SubState != RTC_SET_IDLE))
	{
		APP_DBG("���Ӻ�:%d ", sRtcControl->AlarmNum);
		switch(sRtcControl->AlarmMode)
		{
				case ALARM_MODE_ONCE_ONLY:
					APP_DBG("����ģʽ:����");
					APP_DBG("%04d-%02d-%02d (����%02d) ", 
					sRtcControl->AlarmTime.Year, 
					sRtcControl->AlarmTime.Mon, 
					sRtcControl->AlarmTime.Date, 
					sRtcControl->AlarmTime.WDay);
					break;
	
	
				case ALARM_MODE_PER_DAY:
					APP_DBG("����ģʽ:ÿ��;");
					break;
	
				case ALARM_MODE_PER_WEEK:
					APP_DBG("����ģʽ:ÿ��(%d);", sRtcControl->AlarmTime.WDay);
					break;
				case ALARM_MODE_WORKDAY:
					APP_DBG("����ģʽ:������;");
					break;
				case ALARM_MODE_USER_DEFINED:
					APP_DBG("����ģʽ:�ͻ��Զ���;");
					break;
	
				default:
					APP_DBG(("ģʽ����(mode error)\n"));
					break;
		}

		switch(sNvmRtcInfo.RingType[sRtcControl->AlarmNum])
		{
				case RTC_RING_SOUND1_TYPE:
					APP_DBG("��������:����һ;");
					break;
	
				case RTC_RING_SOUND2_TYPE:
					APP_DBG("��������:������;");
					break;
	
				case RTC_RING_SOUND3_TYPE:
					APP_DBG("��������:������;");
					break;
					
				case RTC_RING_SOUND4_TYPE:
					APP_DBG("��������:������;");
					break;
					
				case RTC_RING_SOUND5_TYPE:
					APP_DBG("��������:������;");
					break;

				case RTC_RING_SDCARD_TYPE:
					APP_DBG("��������:SD������;");
					break;
	
				default:
					APP_DBG(("��֧�ֵ���������;"));
					break;
		}

		APP_DBG("��������:%d;", sNvmRtcInfo.AlarmVolume);
		APP_DBG("����״̬:%s;", RtcGetAlarmStatus(sRtcControl->AlarmNum) == ALARM_STATUS_OPENED?"ON":"OFF");
		APP_DBG("AlarmTime(%02d:%02d:%02d)\n", sRtcControl->AlarmTime.Hour, sRtcControl->AlarmTime.Min, sRtcControl->AlarmTime.Sec);	
	}
#endif
}

// RTC��ʼ��
bool RtcInitialize(void)
{
	APP_DBG("RtcInitialize\n");
#ifdef FUNC_RTC_ALARM
	NvmRead(RTC_NVM_START_ADDR, (uint8_t*)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
	RtcInit((void *)(sNvmRtcInfo.AlarmTimeList), sizeof(ALARM_TIME_INFO) * MAX_ALARM_NUM);//Ӳ����ʼ������Ҫɾ��
#else
	RtcInit(NULL, 0); // �ú����������
#endif	
	if(sRtcControl != NULL)
	{
		APP_DBG("Init fail: RTC is running\n");
		return FALSE;
	}

	sRtcControl = APP_MMM_MALLOC(sizeof(RTC_CONTROL), MMM_ALLOC_NORMAL);
	ASSERT(sRtcControl != NULL);
	memset(sRtcControl, 0, sizeof(RTC_CONTROL));

	// ��ȡeprom��nvram�еģ�ʱ�����ڵ���Ϣ
	RtcGetCurrTime(&sRtcControl->DataTime);
	//�ж��Ƿ��һ���ϵ磬��һ���ϵ����ʼ��ϵͳʱ��
	if(sRtcControl->DataTime.Year < gSysCurDate.Year)
	{
		memcpy(&sRtcControl->DataTime, &gSysCurDate, sizeof(RTC_DATE_TIME));
		RtcSetCurrTime(&sRtcControl->DataTime);
	}
	
#ifdef FUNC_RTC_ALARM
	NVIC_EnableIRQ(RTC_IRQn);           // �����������ж�,������sRtcControl�ڴ����֮��
#endif
	
	// ��ʱ��
	TimeOutSet(&RtcAutoOutTimer, 0);
	InitTimer((SW_TIMER*)&sRtcControl->TimerHandle, 1000, (TIMER_CALLBACK)RtcTimerCB);
	StartTimer(&sRtcControl->TimerHandle);
#ifdef FUNC_RTC_ALARM	
	NvmWrite(RTC_NVM_START_ADDR, (uint8_t *)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
#ifdef FUNC_RTC_ALARM_SAVE2FLASH
	{
		BP_RTC_INFO *BpRtcInfo;
		
		BpRtcInfo = (BP_RTC_INFO *)BP_GetInfo(BP_RTC_INFO_TYPE);
		sNvmRtcInfo.AlarmVolume = BpRtcInfo->AlarmVolume;
		for(sRtcControl->AlarmNum = 1; sRtcControl->AlarmNum <= MAX_ALARM_NUM; sRtcControl->AlarmNum++)
		{
			sNvmRtcInfo.RingType[sRtcControl->AlarmNum] = BpRtcInfo->RingType[sRtcControl->AlarmNum-1];
			sRtcControl->AlarmMode = BpRtcInfo->AlarmMode[sRtcControl->AlarmNum-1]; 
			sRtcControl->AlarmTime = BpRtcInfo->AlarmTime[sRtcControl->AlarmNum-1];
			RtcSetAlarmTime(sRtcControl->AlarmNum, sRtcControl->AlarmMode, sRtcControl->AlarmData, &sRtcControl->AlarmTime);
			RtcAlarmSetStatus(sRtcControl->AlarmNum, BpRtcInfo->AlarmState[sRtcControl->AlarmNum-1]);
		}
		BP_SaveInfo(BP_SAVE2FLASH);
	}
#endif
#endif
	return TRUE;
}

// ȥ��ʼ��
bool RtcDeinitialize(void)
{
	
	APP_DBG("RtcDeinitialize\n");
	if(sRtcControl == NULL)
	{
		return FALSE;
	}

	DeinitTimer(&sRtcControl->TimerHandle);

	// ȡ��ʱ����������
	if(sRtcControl->State != RTC_STATE_IDLE)
	{

	}
	// ���������ӿڣ�ֹͣ��ع���

	// �ͷ���Դ
	APP_MMM_FREE(sRtcControl);
	sRtcControl = NULL;

	return TRUE;
}


void RtcSubStateDbg(void)
{
	switch(sRtcControl->SubState)
	{
		case RTC_SET_YEAR:
			APP_DBG("���á��ꡱ\n");
			break;
		case RTC_SET_MON:
			APP_DBG("���á��¡�\n");
			break;
		case RTC_SET_DAY:
			APP_DBG("���á��ա�\n");
			break;
#ifdef FUNC_RTC_ALARM
		case RTC_SET_WEEK:
			APP_DBG("���á��ܡ�\n");
			break;
#endif
		case RTC_SET_HOUR:
			APP_DBG("���á�ʱ��\n");
			break;
		case RTC_SET_MIN:
			APP_DBG("���á��֡�\n");
			break;
			
#ifdef FUNC_RTC_ALARM
		case RTC_SET_ALARM_MODE:
			APP_DBG("���á�����ģʽ��\n");
			break;

		case RTC_SET_ALARM_SOURCE:
			APP_DBG("���á������������͡�\n");
			break;

		case RTC_SET_ALARM_VOLUME:
			APP_DBG("���á���������������\n");
			break;

		case RTC_SET_ALARM_ONOFF:
			APP_DBG("���á�����״̬���ء�\n");
			break;
#endif
		default:
			break;
	}
}

// �л�ʱ��������
bool RtcSwitchNextSubState(void)
{
	switch(sRtcControl->State)
	{
		case RTC_STATE_IDLE:
			APP_DBG("Failed to switch focus\n");
			return FALSE;
		
		case RTC_STATE_SET_TIME:
			sRtcControl->SubState++;
			if(sRtcControl->SubState > RTC_SET_MIN)
			{
				sRtcControl->SubState = RTC_SET_YEAR;
			}
			else if(sRtcControl->SubState == RTC_SET_WEEK)//ʱ�����ò�֧����ʱ������
			{
				sRtcControl->SubState++;
			}
			break;
			
#ifdef FUNC_RTC_ALARM	
		case RTC_STATE_SET_ALARM:
			//���´��룬������������˳��Ϊ��ʱ���֡�ģʽ���ꡢ�¡��ա��ܡ����������������ء�
			if(sRtcControl->SubState == RTC_SET_MIN)
			{
				sRtcControl->SubState = RTC_SET_ALARM_MODE;
			}
			else if(sRtcControl->SubState == RTC_SET_ALARM_MODE)
			{
				sRtcControl->SubState = RTC_SET_YEAR;
			}
			else if(sRtcControl->SubState == RTC_SET_WEEK)
			{
				sRtcControl->SubState = RTC_SET_ALARM_SOURCE;
			}
			else
			{
				sRtcControl->SubState++;
			}
			
			//�����������ã���Сʱ��ʼRTC_SET_YEAR;		
			if(sRtcControl->SubState >= RTC_SET_MAX)
			{
				sRtcControl->SubState = RTC_SET_HOUR;             
			}

			//��������ģʽ������һ����������
			if(sRtcControl->AlarmMode == ALARM_MODE_PER_WEEK)      //�����Ӳ�֧���ꡢ�¡�������
			{
				if(sRtcControl->SubState < RTC_SET_WEEK)         
				{
					sRtcControl->SubState = RTC_SET_WEEK;
				}
			}
			else if(sRtcControl->AlarmMode == ALARM_MODE_ONCE_ONLY)//�������Ӳ�֧��������
			{
				if(sRtcControl->SubState == RTC_SET_WEEK)
				{
					sRtcControl->SubState = RTC_SET_ALARM_SOURCE;          
				}
			}
			else                                                   //�������Ӳ�֧��������������
			{
				if(sRtcControl->SubState <= RTC_SET_WEEK)
				{
					sRtcControl->SubState = RTC_SET_ALARM_SOURCE;  //����������������
				}
			}

			//��������״̬��������ز���
			if(sRtcControl->SubState == RTC_SET_ALARM_VOLUME)
			{
				if((sNvmRtcInfo.AlarmVolume > MAX_VOLUME) || (sNvmRtcInfo.AlarmVolume == 0))
				{
					sNvmRtcInfo.AlarmVolume = DEFAULT_VOLUME;
				}
			}
			else if(sRtcControl->SubState == RTC_SET_ALARM_SOURCE)
			{
				if((sNvmRtcInfo.RingType[sRtcControl->AlarmNum] >= RTC_RING_TYPE_MAX)
				|| (sNvmRtcInfo.RingType[sRtcControl->AlarmNum] < RTC_RING_SOUND1_TYPE))
				{
					sNvmRtcInfo.RingType[sRtcControl->AlarmNum] = RTC_RING_SOUND1_TYPE;
				}
			}
			break;
#endif
			
		default:
			break;
	}

	APP_DBG("sRtcControl->SubState:%d\n", sRtcControl->SubState);
	return TRUE;
}

void RtcControlEnterSetMode(void)
{
	if(sRtcControl->SubState == RTC_STATE_IDLE)
	{
#ifdef FUNC_RTC_ALARM
		if(sRtcControl->State == RTC_STATE_SET_ALARM)
		{
			APP_DBG("Set Alarm Time Mode\n");
			sRtcControl->SubState = RTC_SET_HOUR;                      //�����������ã���Сʱ��ʼRTC_SET_YEAR;
			sRtcControl->AlarmTime.Year = sRtcControl->DataTime.Year;
			sRtcControl->AlarmTime.Mon = sRtcControl->DataTime.Mon;
			sRtcControl->AlarmTime.Date = sRtcControl->DataTime.Date;  //��������������Ϊ��ǰʱ��;
		}
		else
#endif
		{
			APP_DBG("Set Rtc Time Mode\n");
			sRtcControl->State = RTC_STATE_SET_TIME;
			sRtcControl->SubState = RTC_SET_YEAR;
		}

		RtcCurAlarmSleepAndStop(RTC_ALARM_STATE_STOP);
	    TimeOutSet(&RtcAutoOutTimer, RTC_AUTO_OUT_TIME);
	    RtcSubStateDbg();
    }
    else                                                               //������ȷ�ϣ��˳�����ģʽ
    {
    	APP_DBG("Rtc Normer Mode\n");
		sRtcControl->State = RTC_STATE_IDLE;
		sRtcControl->SubState = RTC_SET_IDLE;
		sRtcControl->AlarmNum = 0;
    }
                                             		//�����������Ϣ
}

void RtcControlSetParm(void)
{
	if(sRtcControl->SubState == RTC_SET_IDLE)
	{
		if((sRtcControl->State == RTC_STATE_SET_ALARM) && (sRtcControl->AlarmNum >= MAX_ALARM_NUM))
		{
			sRtcControl->State = RTC_STATE_SET_TIME;
			sRtcControl->AlarmNum = 0;
		}
		else
		{
			if(sRtcControl->State != RTC_STATE_SET_ALARM)
			{
				sRtcControl->AlarmNum = 0;
			}
			sRtcControl->AlarmNum++;
			RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);
			sRtcControl->AlarmState = RtcGetAlarmStatus(sRtcControl->AlarmNum); 
			if((sRtcControl->AlarmMode < ALARM_MODE_ONCE_ONLY) || (sRtcControl->AlarmMode >= ALARM_MODE_USER_DEFINED))
			{
				sRtcControl->AlarmMode = ALARM_MODE_ONCE_ONLY;
			}

			sRtcControl->State = RTC_STATE_SET_ALARM;
		}

		sRtcControl->SubState = RTC_SET_IDLE;
		APP_DBG("Rtc set status change: %s\n", sRtcControl->State == RTC_STATE_SET_ALARM? "Set Alarm Mode": "Set Time Mode");
	}
	else if(sRtcControl->SubState != RTC_SET_IDLE)
	{
		RtcSwitchNextSubState();
	}

	TimeOutSet(&RtcAutoOutTimer, RTC_AUTO_OUT_TIME);
	RtcSubStateDbg();
}

void RtcTimeUp(RTC_DATE_TIME *Time)
{
	switch(sRtcControl->SubState)
	{
		case RTC_SET_YEAR:
			Time->Year++;
			if(Time->Year > 2099)
			{
				Time->Year = 1980;	
			}
			break;
		case RTC_SET_MON:
			Time->Mon++;
			if(Time->Mon > 12)
			{
				Time->Mon = 1;
			}
			if(Time->Date > RtcGetMonthDays(Time->Year, Time->Mon))
			{
				Time->Date = RtcGetMonthDays(Time->Year, Time->Mon);
			}
			break;
		case RTC_SET_WEEK:
			Time->WDay++;
			if(Time->WDay > 6)
			{
				Time->WDay = 0;	
			}
			break;
		case RTC_SET_DAY:	
			Time->Date++;
			if(Time->Date > RtcGetMonthDays(Time->Year, Time->Mon))
			{
				Time->Date = 1;
			}
			break;
		case RTC_SET_HOUR:
			Time->Hour++;
			Time->Sec = 0;
			if(Time->Hour > 23)
			{
				Time->Hour = 0;
			}
			break;
		case RTC_SET_MIN:
			Time->Min++;
			Time->Sec = 0;
			if(Time->Min > 59)
			{
				Time->Min = 0;
			}
			break;
		default:
			break;
	}
}

void RtcTimeDown(RTC_DATE_TIME *Time)
{
	switch(sRtcControl->SubState)
	{	
		case RTC_SET_YEAR:
			Time->Year--;
			if(Time->Year < 1980)
			{
				Time->Year = 2099;	
			}
			break;
		case RTC_SET_MON:
			Time->Mon--;
			if(Time->Mon == 0)
			{
				Time->Mon = 12;
			}
			if(Time->Date > RtcGetMonthDays(Time->Year, Time->Mon))
			{
				Time->Date = RtcGetMonthDays(Time->Year, Time->Mon);
			}
			break;
		case RTC_SET_WEEK:
			Time->WDay--;			
			if(Time->WDay > 6)
			{
				Time->WDay = 6;	
			}
			break;
		case RTC_SET_DAY:	
			Time->Date--;
			if(Time->Date == 0)
			{
				Time->Date = RtcGetMonthDays(Time->Year, Time->Mon);
			}
			break;
		case RTC_SET_HOUR:
			Time->Hour--;
			Time->Sec = 0;
			if(Time->Hour > 23)
			{
				Time->Hour = 23;
			}
			break;
		case RTC_SET_MIN:
			Time->Min--;
			Time->Sec = 0;
			if(Time->Min > 59)
			{
				Time->Min = 59;
			}
			break;
		default:
			break;
	}
}

// ϵͳʱ�䵱ǰ������ֵ����
void RtcSetTimeUp(void)
{
	if(RTC_SET_IDLE == sRtcControl->SubState)
	{
		//�ָ�����ʾʱ��
		APP_DBG("Rtc Normer Mode\n");
		sRtcControl->State = RTC_STATE_IDLE;
		sRtcControl->SubState = RTC_SET_IDLE;
		sRtcControl->AlarmNum = 0;
		return;
	}

	APP_DBG("RtcSetValueUp(state = %d, substate = %d)\n", sRtcControl->State, sRtcControl->SubState);
	if(RTC_STATE_SET_TIME == sRtcControl->State)
	{
		RtcTimeUp(&sRtcControl->DataTime);
		RtcSetCurrTime(&sRtcControl->DataTime);
	}
#ifdef FUNC_RTC_ALARM
	else if(RTC_STATE_SET_ALARM == sRtcControl->State)
	{
		if(RTC_SET_ALARM_MODE == sRtcControl->SubState)
		{
			sRtcControl->AlarmMode++;
			if(sRtcControl->AlarmMode >= ALARM_MODE_USER_DEFINED)
			{
				sRtcControl->AlarmMode = ALARM_MODE_ONCE_ONLY;
			}
			
			if(sRtcControl->AlarmMode == ALARM_MODE_USER_DEFINED)
			{
				sRtcControl->AlarmData = 0x7E; // �ͻ������Լ��޸�
			}
			else if(sRtcControl->AlarmMode == ALARM_MODE_WORKDAY)
			{
				sRtcControl->AlarmData = 0x3E;
			}
			else
			{
				sRtcControl->AlarmData = 0x0;
			}
			APP_DBG("+++sRtcControl->ModeData;%x\n", sRtcControl->AlarmData);
		}
		else if(RTC_SET_ALARM_SOURCE == sRtcControl->SubState)         //����������ʾ����Դ
		{
			sNvmRtcInfo.RingType[sRtcControl->AlarmNum]++;
			if(sNvmRtcInfo.RingType[sRtcControl->AlarmNum] >= RTC_RING_TYPE_MAX)
			{
			  sNvmRtcInfo.RingType[sRtcControl->AlarmNum] = RTC_RING_SOUND1_TYPE;
			}
		}
		else if(RTC_SET_ALARM_ONOFF == sRtcControl->SubState)          //�������ӿ���
		{
			if(RtcGetAlarmStatus(sRtcControl->AlarmNum) == ALARM_STATUS_OPENED)
			{
				sRtcControl->AlarmState = ALARM_STATUS_CLOSED;
			}
			else
			{
				sRtcControl->AlarmState = ALARM_STATUS_OPENED;
			}
		}
		else if(RTC_SET_ALARM_VOLUME == sRtcControl->SubState)         //����������ʾ������
		{
			sNvmRtcInfo.AlarmVolume++;
			if(sNvmRtcInfo.AlarmVolume >= MAX_VOLUME)
			{
			  sNvmRtcInfo.AlarmVolume = 0;
			}
		}
		else
		{
			RtcTimeUp(&sRtcControl->AlarmTime);
		}
		
		//�������Ӽ�����״̬����Ҫ���������ӣ������������״̬����������ʱ��ʱ��ǿ�ƴ�����״̬
		RtcSetAlarmTime(sRtcControl->AlarmNum, sRtcControl->AlarmMode, sRtcControl->AlarmData, &sRtcControl->AlarmTime);
		RtcAlarmSetStatus(sRtcControl->AlarmNum, sRtcControl->AlarmState);
		
#ifdef FUNC_RTC_ALARM		
		NvmWrite(RTC_NVM_START_ADDR, (uint8_t *)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
#ifdef FUNC_RTC_ALARM_SAVE2FLASH
		{
			BP_RTC_INFO *BpRtcInfo;
				
			BpRtcInfo = (BP_RTC_INFO *)BP_GetInfo(BP_RTC_INFO_TYPE);
			BpRtcInfo->AlarmMode[sRtcControl->AlarmNum-1] = sRtcControl->AlarmMode;
			BpRtcInfo->RingType[sRtcControl->AlarmNum-1] = sNvmRtcInfo.RingType[sRtcControl->AlarmNum];
			BpRtcInfo->AlarmState[sRtcControl->AlarmNum-1] = sRtcControl->AlarmState;
			BpRtcInfo->AlarmTime[sRtcControl->AlarmNum-1] = sRtcControl->AlarmTime;
			BP_SaveInfo(BP_SAVE2FLASH);
		}
#endif
#endif
	}
#endif
	TimeOutSet(&RtcAutoOutTimer, RTC_AUTO_OUT_TIME);
#ifdef FUNC_DISP_EN
	DispSendMessage(DEV_SYMBOL_INTERVAL, DISP_MSG_RTC_SET);
#endif
}

// ϵͳʱ�䵱ǰ������ֵ�ļ�
void RtcSetTimeDown(void)
{
	if(RTC_SET_IDLE == sRtcControl->SubState)
	{
		//�ָ�����ʾʱ��
		APP_DBG("Rtc Normer Mode\n");
		sRtcControl->State = RTC_STATE_IDLE;
		sRtcControl->SubState = RTC_SET_IDLE;
		sRtcControl->AlarmNum = 0;
		return;
	}

	APP_DBG("RtcSetValueDown(state = %d, substate = %d)\n", sRtcControl->State, sRtcControl->SubState);
	if(RTC_STATE_SET_TIME == sRtcControl->State)
	{
		RtcTimeDown(&sRtcControl->DataTime);
		RtcSetCurrTime(&sRtcControl->DataTime);
	}
#ifdef FUNC_RTC_ALARM
	else if(RTC_STATE_SET_ALARM == sRtcControl->State)
	{	
		if(RTC_SET_ALARM_MODE == sRtcControl->SubState)
		{
			sRtcControl->AlarmMode--;
			if((sRtcControl->AlarmMode < ALARM_MODE_ONCE_ONLY) || (sRtcControl->AlarmMode >= ALARM_MODE_USER_DEFINED))
			{
				sRtcControl->AlarmMode = (ALARM_MODE_USER_DEFINED - 1);
			}
			
			if(sRtcControl->AlarmMode == ALARM_MODE_USER_DEFINED)
			{
				sRtcControl->AlarmData = 0x7E; // �ͻ������Լ��޸�
			}
			else if(sRtcControl->AlarmMode == ALARM_MODE_WORKDAY)
			{
				sRtcControl->AlarmData = 0x3E;
			}
			else
			{
				sRtcControl->AlarmData = 0x0;
			}
			APP_DBG("---sRtcControl->ModeData;%x\n", sRtcControl->AlarmData);
		}
		else if(RTC_SET_ALARM_SOURCE == sRtcControl->SubState)         //����������ʾ����Դ
		{
			sNvmRtcInfo.RingType[sRtcControl->AlarmNum]--;
			if((sNvmRtcInfo.RingType[sRtcControl->AlarmNum] >= RTC_RING_TYPE_MAX)
			|| (sNvmRtcInfo.RingType[sRtcControl->AlarmNum] < RTC_RING_SOUND1_TYPE))
			{
			  sNvmRtcInfo.RingType[sRtcControl->AlarmNum] = (RTC_RING_TYPE_MAX-1);
			}
		}
		else if(RTC_SET_ALARM_ONOFF == sRtcControl->SubState)          //�������ӿ���
		{ 
			if(RtcGetAlarmStatus(sRtcControl->AlarmNum) == ALARM_STATUS_OPENED)
			{
				sRtcControl->AlarmState = ALARM_STATUS_CLOSED;
			}
			else
			{
				sRtcControl->AlarmState = ALARM_STATUS_OPENED;
			}
		}
		else if(RTC_SET_ALARM_VOLUME == sRtcControl->SubState)         //����������ʾ������
		{
			sNvmRtcInfo.AlarmVolume--;
			if(sNvmRtcInfo.AlarmVolume > MAX_VOLUME)
			{
			  sNvmRtcInfo.AlarmVolume = MAX_VOLUME;
			}
		}
		else
		{
			RtcTimeDown(&sRtcControl->AlarmTime);
		}

		//�������Ӽ�����״̬����Ҫ���������ӣ������������״̬����������ʱ��ʱ��ǿ�ƴ�����״̬
		RtcSetAlarmTime(sRtcControl->AlarmNum, sRtcControl->AlarmMode, sRtcControl->AlarmData, &sRtcControl->AlarmTime);
		RtcAlarmSetStatus(sRtcControl->AlarmNum, sRtcControl->AlarmState);
		
#ifdef FUNC_RTC_ALARM		
		NvmWrite(RTC_NVM_START_ADDR, (uint8_t *)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
#ifdef FUNC_RTC_ALARM_SAVE2FLASH
		{
			BP_RTC_INFO *BpRtcInfo;
				
			BpRtcInfo = (BP_RTC_INFO *)BP_GetInfo(BP_RTC_INFO_TYPE);
			BpRtcInfo->AlarmMode[sRtcControl->AlarmNum-1] = sRtcControl->AlarmMode;
			BpRtcInfo->RingType[sRtcControl->AlarmNum-1] = sNvmRtcInfo.RingType[sRtcControl->AlarmNum];
			BpRtcInfo->AlarmState[sRtcControl->AlarmNum-1] = sRtcControl->AlarmState;
			BpRtcInfo->AlarmTime[sRtcControl->AlarmNum-1] = sRtcControl->AlarmTime;
			BP_SaveInfo(BP_SAVE2FLASH);
		}
#endif
#endif
	}
#endif
	TimeOutSet(&RtcAutoOutTimer, RTC_AUTO_OUT_TIME);
#ifdef FUNC_DISP_EN
	DispSendMessage(DEV_SYMBOL_INTERVAL, DISP_MSG_RTC_SET);
#endif
}

//����Ƿ���������������
bool IsRtcAlarmRingPlaying(uint16_t CheckMsg)
{
	if(sRtcControl == NULL)
	{
		APP_DBG("fail: RTC is not running\n");
		return FALSE;
	}
	else if(CheckMsg == MSG_RTC_SET_PARM)
	{
		return sRtcControl->CurAlarmRun;
	}
	else
	{
		return sRtcControl->CurRingDuration;
	}
}

//��ǰ���ӽ���̰˯ģʽ
//Parma��TRUE ��������̰˯��FLSE����ʱ����̰˯
bool RtcCurAlarmSleepAndStop(RTC_ALARM_STATE state)
{
	bool Ret = FALSE;
	RTC_DATE_TIME  CurDateTime;
	uint32_t AlarmTemp,DateTemp;
	static uint8_t AlarmRingCnt,CurVolumeBackUp;

	if(sRtcControl == NULL)
	{
		return FALSE;
	}
	
	//���ص�ǰRTCʱ��,����ʱ��
	RtcGetCurrTime(&CurDateTime); 

	AlarmTemp = sRtcControl->AlarmTime.Hour*10000;
	AlarmTemp += sRtcControl->AlarmTime.Min*100;
	AlarmTemp += sRtcControl->AlarmTime.Sec;

	DateTemp = CurDateTime.Hour*10000;
	DateTemp += CurDateTime.Min*100;
	DateTemp += CurDateTime.Sec;

	if(state != RTC_ALARM_STATE_STOP)
	{
		if((AlarmTemp == DateTemp) && (state == RTC_ALARM_STATE_IDLE))
		{
			Ret = TRUE;
			//��һ�ο�ʼ���ӣ���ʼ�����ӻ�����
			if(sRtcControl->CurRingDuration == FALSE)
			{
				CurVolumeBackUp = gSys.Volume;
				gSys.Volume = sNvmRtcInfo.AlarmVolume;
				SetSysVol();
			}
		}
		else if(((sRtcControl->CurRingDuration != FALSE) && (state == RTC_ALARM_STATE_SLEEP))
			|| (sRtcControl->CurRingDuration >= RTC_ALARM_RING_TIME))
		{
			sRtcControl->AlarmTime.Min += RTC_SECOND_ALARM_TIME;
			sRtcControl->AlarmTime.Sec = 0;
			sRtcControl->CurRingDuration = 0;
			state = RTC_ALARM_STATE_SLEEP;
			AlarmRingCnt++;

			//�ж�����ʱ�����Ƿ���Ҫ��λ��
			sRtcControl->AlarmTime.Hour += (sRtcControl->AlarmTime.Min/60);
			sRtcControl->AlarmTime.Hour = (sRtcControl->AlarmTime.Hour%24);
			sRtcControl->AlarmTime.Min = (sRtcControl->AlarmTime.Min%60);

			APP_DBG("��������̰˯ģʽ��%02d-%02d-%02d; CurRingDuration��%d;AlarmRingCnt :%d;\n", 
			sRtcControl->AlarmTime.Hour, sRtcControl->AlarmTime.Min, sRtcControl->AlarmTime.Sec,
			sRtcControl->CurRingDuration, AlarmRingCnt);
			
			Ret = FALSE;
		}
		else if(((AlarmTemp < DateTemp) && ((CurDateTime.Hour-sRtcControl->AlarmTime.Hour) < 2))
			|| ((AlarmTemp > DateTemp) && ((sRtcControl->AlarmTime.Hour - CurDateTime.Hour) >= 2)))
		{ 	
			//���㵱ǰ�����Ѿ������ʱ��
			DateTemp = (sRtcControl->CurRingDuration%60);
			sRtcControl->CurRingDuration += ((CurDateTime.Sec + 60) - DateTemp)%60;
				
			sRtcControl->AlarmTime.Min += ((CurDateTime.Sec+RTC_ALARM_RING_INTERVAL)/60);
			sRtcControl->AlarmTime.Sec = ((CurDateTime.Sec+RTC_ALARM_RING_INTERVAL)%60);
			state = RTC_ALARM_STATE_IDLE;

			//�ж�����ʱ�����Ƿ���Ҫ��λ��
			sRtcControl->AlarmTime.Hour += (sRtcControl->AlarmTime.Min/60);
			sRtcControl->AlarmTime.Hour = (sRtcControl->AlarmTime.Hour%24);
			sRtcControl->AlarmTime.Min = (sRtcControl->AlarmTime.Min%60);

			APP_DBG("��������̰˯ģʽ��%02d-%02d-%02d; CurRingDuration��%d; AlarmRingCnt: %d\n", 
			sRtcControl->AlarmTime.Hour, sRtcControl->AlarmTime.Min, sRtcControl->AlarmTime.Sec,
			sRtcControl->CurRingDuration, AlarmRingCnt);
			
			Ret = FALSE;
		}
	}

	if(state != RTC_ALARM_STATE_IDLE)
	{
		//�ָ��ֳ�
		if(CurVolumeBackUp != FALSE)
		{
			gSys.Volume = CurVolumeBackUp;
			CurVolumeBackUp = FALSE;
		}
		SetSysVol();
		
		if(state == RTC_ALARM_STATE_STOP || (AlarmRingCnt >= RTC_ALARM_MAX_COUNT))
		{
			sRtcControl->CurAlarmNum = 0;
			sRtcControl->CurAlarmRun = 0;
			sRtcControl->CurRingDuration = 0;
			AlarmRingCnt = 0;
			APP_DBG("Rtc alarm ring is stop!!!!\n");
		}
	}
	
	return Ret;
}

bool RtcControl(void)
{
	uint16_t Msg  = 0;

	APP_DBG("Enter RTC\n");
#ifndef FUNC_MIXER_SRC_EN
	AudioSampleRateSet(44100);
#endif

#ifndef FUNC_WIFI_SUPPORT_RTC_EN 
	//��ʼ��
	if(!RtcInitialize())
	{
		return FALSE;
	}
#endif

	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	while(Msg != MSG_COMMON_CLOSE)
	{
	
		RtcControlProcess();
		Msg = MsgRecv(20);

		switch(Msg)
		{
			case MSG_POWER:
				APP_DBG("[RTC] MSG_POWER\n");
			#ifdef FUNC_WIFI_POWER_KEEP_ON
				WiFiRequestMcuPowerOff();
			#else
				gSys.NextModuleID = MODULE_ID_STANDBY;
				Msg = MSG_COMMON_CLOSE;
			#endif
				break;
			
			case MSG_MODE:
				APP_DBG("Exit RTC\n");
				Msg = MSG_COMMON_CLOSE;
				break;

			case MSG_RTC_SET_PARM:
				RtcControlSetParm();
				break;

            case MSG_WIFI_WPS:
            	RtcControlEnterSetMode();
            	break;
            	
			case MSG_RTC_SET_TIME:					
			case MSG_RTC_SET_ALARM:
				break;
				
				// �������ֵ����
			case MSG_VOL_UP:
			case MSG_RTC_SET_UP:
				RtcSetTimeUp();
				break;
			
				// �������ֵ����
			case MSG_VOL_DW:
			case MSG_RTC_SET_DW:
				RtcSetTimeDown();
				break;
			
#ifdef FUNC_RECORD_EN
			case MSG_REC:  //¼��
				gSys.ModuleIDBackUp = gSys.CurModuleID;
				gSys.NextModuleID = MODULE_ID_RECORDER;
				gSys.RecordState = RECORD_STATE_REC;
				gSys.RecordSource = RECORD_SOURCE_LINEIN;
				Msg = MSG_COMMON_CLOSE;
				break;
#endif
           
#ifdef FUNC_REC_PLAYBACK_EN
			case MSG_REC_PLAYBACK:
				EnterRecBackPlay(gSys.CurModuleID, IsUDiskLink() ? DEV_ID_USB : DEV_ID_SD, TRUE, 0, 0);
				break;
#endif			

			default:
				CommonMsgProccess(Msg);
				break;
		}

		if(((RTC_STATE_IDLE != sRtcControl->State)||(RTC_SET_IDLE != sRtcControl->SubState)) && IsTimeOut(&RtcAutoOutTimer))
		{
			//�ָ�����ʾʱ��
			APP_DBG("Rtc Normer Mode\n");
			sRtcControl->State = RTC_STATE_IDLE;
			sRtcControl->SubState = RTC_SET_IDLE;
			sRtcControl->AlarmNum = 0;
		}
	}
	
#ifndef FUNC_WIFI_SUPPORT_RTC_EN 
	RtcDeinitialize();//ģʽ�л����˳�RTC
#endif
    //�˳�RTCʱ�������������״̬��
	APP_DBG("Exit Rtc Mode\n");
	sRtcControl->State = RTC_STATE_IDLE;
	sRtcControl->SubState = RTC_SET_IDLE;

	return TRUE;
}

#ifdef FUNC_RTC_EN
bool RtcControlProcess(void)
{
	
	CheckTimer(&sRtcControl->TimerHandle);

#ifdef FUNC_RTC_ALARM	
	if(RTC_STATE_IDLE == sRtcControl->State)
	{		
		if(sRtcControl->CurAlarmNum)
		{
			if(!sRtcControl->CurAlarmRun)
			{
				RtcGetAlarmTime(sRtcControl->CurAlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);
				if(sRtcControl->AlarmMode == ALARM_MODE_ONCE_ONLY)
				{
					//�������ӣ���������ӹر�
					RtcAlarmSetStatus(sRtcControl->CurAlarmNum, ALARM_STATUS_CLOSED);
				}
				sRtcControl->AlarmState = RtcGetAlarmStatus(sRtcControl->AlarmNum);
				//ˢ�µ�ǰʵʱʱ��
				RtcGetCurrTime(&sRtcControl->DataTime);
				APP_DBG("RTC ALARM(%d) COME! ����ʱ��%02d-%02d-%02d\n",sRtcControl->CurAlarmNum, 
				sRtcControl->AlarmTime.Hour, sRtcControl->AlarmTime.Min, sRtcControl->AlarmTime.Sec);
				RtcAlarmArrivedProcess();
				//�����������ΪWiFi SD�������Զ��л�SDģʽ
				if(IsWiFiCardLink() && (sNvmRtcInfo.RingType[sRtcControl->CurAlarmNum] == RTC_RING_SDCARD_TYPE))
				{
					gSys.NextModuleID = MODULE_ID_PLAYER_WIFI_SD;	
					MsgSend(MSG_MODE);	
					sRtcControl->CurAlarmRun = FALSE;
					sRtcControl->CurAlarmNum = FALSE;
				}
				//�������ΪWiFI�����ӣ���ǰ�����������������ز����쵱ǰ����
				else if((sNvmRtcInfo.RingType[sRtcControl->CurAlarmNum] == RTC_RING_WIFISD_TYPE)
				&& (WiFiWwwStateGet() == WIFI_STATUS_WWW_ENABLE)
				&& (WiFiLoginStateGet() == WIFI_STATUS_LOGIN_SUCCESSFUL))
				{
					sRtcControl->CurAlarmRun = FALSE;
					sRtcControl->CurAlarmNum = FALSE;
				}
				else
				{
					if(sNvmRtcInfo.RingType[sRtcControl->CurAlarmNum] >= RTC_RING_SDCARD_TYPE)
					{
						sNvmRtcInfo.RingType[sRtcControl->CurAlarmNum] = RTC_RING_SOUND1_TYPE;
					}
					sRtcControl->CurAlarmRun = TRUE;
				}
#ifdef FUNC_RTC_ALARM			
				NvmWrite(RTC_NVM_START_ADDR, (uint8_t *)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
#ifdef FUNC_RTC_ALARM_SAVE2FLASH
				{
					BP_RTC_INFO *BpRtcInfo;
						
					BpRtcInfo = (BP_RTC_INFO *)BP_GetInfo(BP_RTC_INFO_TYPE);
					BpRtcInfo->AlarmMode[sRtcControl->AlarmNum-1] = sRtcControl->AlarmMode;
					BpRtcInfo->RingType[sRtcControl->AlarmNum-1] = sNvmRtcInfo.RingType[sRtcControl->AlarmNum];
					BpRtcInfo->AlarmState[sRtcControl->AlarmNum-1] = sRtcControl->AlarmState;
					BpRtcInfo->AlarmTime[sRtcControl->AlarmNum-1] = sRtcControl->AlarmTime;
					BP_SaveInfo(BP_SAVE2FLASH);
				}
#endif
#endif
			}
			else if(sRtcControl->CurAlarmRun && !IsSoundRemindPlaying())
			{
				if(RtcCurAlarmSleepAndStop(RTC_ALARM_STATE_IDLE))
				{
					//����������ʾ����
					SoundRemind(Alarm_Ring[sNvmRtcInfo.RingType[sRtcControl->CurAlarmNum]]);
				}
			}
		}
	}
	
	if((RTC_STATE_SET_ALARM == sRtcControl->State) && !IsSoundRemindPlaying())
	{
		if((RTC_SET_ALARM_SOURCE == sRtcControl->SubState)
		&& (sNvmRtcInfo.RingType[sRtcControl->AlarmNum] < RTC_RING_SDCARD_TYPE))
		{
			SoundRemind(Alarm_Ring[sNvmRtcInfo.RingType[sRtcControl->AlarmNum]]);
		}
	}
#endif
	return TRUE;
}
#endif

#ifdef FUNC_RTC_ALARM
__attribute__((section(".driver.isr"), weak)) void RtcInterrupt(void)
{
	sRtcControl->CurAlarmRun = FALSE;
	sRtcControl->CurAlarmNum = RtcCheckAlarmFlag();
	RtcAlarmIntClear();// ������������ж�
}
#endif
#endif
