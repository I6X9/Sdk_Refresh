///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: rtc_control.h
// maintainer: Sam
///////////////////////////////////////////////////////////////////////////////

#ifndef __RTC_CONTROL_H__
#define __RTC_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "app_config.h"
#include "rtc.h"
#include "breakpoint.h"
//// ��������
//#define INTER_RING_TYPE 0 // ��������
//#define USB_RING_TYPE   1 // U������
//#define SD_RING_TYPE    2 // SD������

//// �������ѷ�ʽ
//#define PLAYER_REMIND 0// ������ʱ��ֱ�ӽ��벥��ģʽ������������
//#define FM_REMIND     1// ������ʱ��ֱ�ӽ���FMģʽ
//#define POPUP_REMIND  2// ������ʱ�������ض����������Ѵ���

////////////////////////////////////////////////////

typedef enum _RTC_STATE_
{
	RTC_STATE_IDLE = 0,
	RTC_STATE_SET_TIME,
	RTC_STATE_SET_ALARM,
	
} RTC_STATE;

typedef enum _RTC_ALARM_STATE_
{
	RTC_ALARM_STATE_IDLE = 0,
	RTC_ALARM_STATE_SLEEP,
	RTC_ALARM_STATE_STOP,
	
} RTC_ALARM_STATE;


typedef enum _RTC_SUB_STATE_
{
    RTC_SET_IDLE = 0,	// ������
	RTC_SET_YEAR,    	// �������
    RTC_SET_MON,     	// �����·�
    RTC_SET_DAY,     	// ��������
	RTC_SET_WEEK,		// ������
    RTC_SET_HOUR,    	// ����Сʱ
    RTC_SET_MIN,     	// ���÷���
	RTC_SET_ALARM_MODE,   //��������ģʽ�����Ρ�ÿ�졢�����ա��Զ���
	RTC_SET_ALARM_SOURCE, //������������
	RTC_SET_ALARM_VOLUME, //������������
	RTC_SET_ALARM_ONOFF,  //�������ӿ���
	RTC_SET_MAX, 		  //
	
} RTC_SUB_STATE;

typedef enum _RTC_RING_TYPE_
{
    RTC_RING_SOUND1_TYPE = 1,	// ��������1
	RTC_RING_SOUND2_TYPE,    	// ��������2
    RTC_RING_SOUND3_TYPE,     	// ��������3
    RTC_RING_SOUND4_TYPE,     	// ��������4
	RTC_RING_SOUND5_TYPE,		// ��������5
	RTC_RING_SDCARD_TYPE,       // SD����ǰ����Ϊ����
	RTC_RING_TYPE_MAX, 		  //
	RTC_RING_WIFISD_TYPE,       // WiFi��������ӣ��������������������ܽ���
} RTC_RING_TYPE;

typedef enum _RTC_WEEK_
{
    SUNDAY,  
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,

} RTC_WEEK;

typedef struct _RTC_CONTROL_
{
	uint8_t  		State;          	// RTC��ǰ״̬,IDLE, Set Time, Set Alarm
	uint8_t  		SubState;      		// RTC��ǰ��״̬,set year/mon/...
	
	RTC_DATE_TIME 	DataTime;  			// ʱ������	
	uint8_t	 		AlarmNum;			// ���ӱ��
	uint8_t	 		AlarmMode;			// ����ģʽ
	uint8_t         AlarmState;     	// ��ǰ���Ӻ�����״̬�������������ӿ���
	RTC_DATE_TIME 	AlarmTime; 			// ����ʱ��
	uint8_t  		AlarmData;			// �Զ�������ģʽ�£�����ʱ��ѡ��bit0�������죬bit1������һ��...,bit6��������,��λ��ʾѡ��  
	uint8_t  		CurAlarmNum;		// ��ǰ��Ӧ�����ӱ��
	uint16_t		CurRingDuration;	// �����Ѿ��������ʱ��

	bool			CurAlarmRun;    	// �Ƿ����������С�
	SW_TIMER TimerHandle;
} RTC_CONTROL;

// Ĭ�� �Զ�����3�Ρ�������5����
// ����������Ϣ���ⲿ���ɿͻ��������

typedef struct _NVM_RTC_INFO_
{
	uint8_t 		AlarmVolume;        			// ������������
	uint8_t			RingType[MAX_ALARM_NUM+1]; 		// �������� INTER_RING_TYPE - ����������USB_RING_TYPE - U��������SD_RING_TYPE - SD������
	ALARM_TIME_INFO AlarmTimeList[MAX_ALARM_NUM];	// ����ʱ����Ϣ
	
} NVM_RTC_INFO;


#ifdef FUNC_BREAKPOINT_EN
#define RTC_NVM_START_ADDR		(sizeof(BP_INFO))
#else 
#define RTC_NVM_START_ADDR		0
#endif
///////////////////////////////////////////

#ifdef FUNC_RTC_EN
extern RTC_CONTROL* sRtcControl;
extern NVM_RTC_INFO sNvmRtcInfo; 
#endif

bool RtcControl(void);
bool RtcControlProcess(void);
bool RtcInitialize(void);
bool IsRtcAlarmRingPlaying(uint16_t CheckMsg);
void SoundRemindAlarmRing(uint8_t RingId);
bool RtcCurAlarmSleepAndStop(RTC_ALARM_STATE state);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif



