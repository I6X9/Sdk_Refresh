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
//// 铃声类型
//#define INTER_RING_TYPE 0 // 内置铃声
//#define USB_RING_TYPE   1 // U盘铃声
//#define SD_RING_TYPE    2 // SD卡铃声

//// 闹钟提醒方式
//#define PLAYER_REMIND 0// 来闹钟时，直接进入播放模式，并播放铃声
//#define FM_REMIND     1// 来闹钟时，直接进入FM模式
//#define POPUP_REMIND  2// 来闹钟时，弹出特定的闹钟提醒窗口

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
    RTC_SET_IDLE = 0,	// 空闲中
	RTC_SET_YEAR,    	// 设置年份
    RTC_SET_MON,     	// 设置月份
    RTC_SET_DAY,     	// 设置日期
	RTC_SET_WEEK,		// 设置周
    RTC_SET_HOUR,    	// 设置小时
    RTC_SET_MIN,     	// 设置分钟
	RTC_SET_ALARM_MODE,   //设置闹钟模式，单次、每天、工作日、自定义
	RTC_SET_ALARM_SOURCE, //设置闹钟铃声
	RTC_SET_ALARM_VOLUME, //设置闹钟音量
	RTC_SET_ALARM_ONOFF,  //设置闹钟开关
	RTC_SET_MAX, 		  //
	
} RTC_SUB_STATE;

typedef enum _RTC_RING_TYPE_
{
    RTC_RING_SOUND1_TYPE = 1,	// 内置铃声1
	RTC_RING_SOUND2_TYPE,    	// 内置铃声2
    RTC_RING_SOUND3_TYPE,     	// 内置铃声3
    RTC_RING_SOUND4_TYPE,     	// 内置铃声4
	RTC_RING_SOUND5_TYPE,		// 内置铃声5
	RTC_RING_SDCARD_TYPE,       // SD卡当前歌曲为铃声
	RTC_RING_TYPE_MAX, 		  //
	RTC_RING_WIFISD_TYPE,       // WiFi网络端闹钟，本地设置闹钟铃声不能进入
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
	uint8_t  		State;          	// RTC当前状态,IDLE, Set Time, Set Alarm
	uint8_t  		SubState;      		// RTC当前子状态,set year/mon/...
	
	RTC_DATE_TIME 	DataTime;  			// 时间日期	
	uint8_t	 		AlarmNum;			// 闹钟编号
	uint8_t	 		AlarmMode;			// 闹钟模式
	uint8_t         AlarmState;     	// 当前闹钟号闹钟状态，用于设置闹钟开关
	RTC_DATE_TIME 	AlarmTime; 			// 闹钟时间
	uint8_t  		AlarmData;			// 自定义闹钟模式下，闹钟时间选择，bit0：星期天，bit1：星期一，...,bit6：星期六,置位表示选择  
	uint8_t  		CurAlarmNum;		// 当前响应的闹钟编号
	uint16_t		CurRingDuration;	// 闹铃已经闹响持续时间

	bool			CurAlarmRun;    	// 是否正在闹铃中。
	SW_TIMER TimerHandle;
} RTC_CONTROL;

// 默认 自动闹铃3次、闹铃间隔5分钟
// 闹钟闹铃信息，这部分由客户自行完成

typedef struct _NVM_RTC_INFO_
{
	uint8_t 		AlarmVolume[MAX_ALARM_NUM+1];   // 闹钟铃声音量
	uint8_t			RingType[MAX_ALARM_NUM+1]; 		// 铃声类型 INTER_RING_TYPE - 内置铃声、USB_RING_TYPE - U盘铃声、SD_RING_TYPE - SD卡铃声
	ALARM_TIME_INFO AlarmTimeList[MAX_ALARM_NUM];	// 闹钟时间信息
	
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
#if (defined(FUNC_RTC_EN) && defined(FUNC_RTC_ALARM_SAVE2FLASH))
extern bool GetNearAlarmTimeFromBpInfo(ALARM_BP_INFO *AlarmInfo, uint8_t AlarmNum);
extern void SetWiFiAlarmTimeToBpInfo(ALARM_BP_INFO *AlarmInfo);
extern void SyncMcuAlarmTimeToBpInfo(ALARM_BP_INFO *AlarmInfo,uint8_t AlarmNum);
#endif
#ifdef __cplusplus
}
#endif//__cplusplus

#endif



