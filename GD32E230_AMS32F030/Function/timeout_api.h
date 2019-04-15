/*
*****************************************************************************
*					Mountain View Silicon Tech. Inc.
*	Copyright 2012, Mountain View Silicon Tech. Inc., ShangHai, China
*					All rights reserved.
*
* Filename:			timeout.h
* maintainer        Halley
* Description:		declare	varable and function here
*
******************************************************************************/
#ifndef __TIMEROUT_H__
#define __TIMEROUT_H__

#include <stdint.h>
#include "app_config.h"

/*
 * define timer check for init task period event
 */
 #define MaxSystickCount   0xffffffff
 
typedef void (*TIMER_CALLBACK)(void);

typedef struct _SW_TIMER_
{
	uint16_t TimerPeriod;  // ms,��ע�⡿��С��ʱ�����Ӧ��ģʽ��ѭ����������������ʱ���йأ����鲻Ҫ������ѭ�����������ʱ�䣻
	uint32_t LastTickets;  // ����timeout�ж�
	TIMER_CALLBACK Callback;
	FlagStatus  IsRunning;
} SW_TIMER;

// define softtimer structure.
typedef struct	_TIMER
{
	uint16_t	TimeOutVal; 								//time out value
	uint32_t	TickValCache;			  					//soft timer setting value
} TIMER;

// ��õ�ǰTimerʵ������ʱ�䣬��λms
uint16_t GetPassTime(SW_TIMER* TimerHandle);

// Ӧ��ģʽ��ѭ���У����øú�������ѯTimer����ʱ����CallBack��
void CheckTimer(SW_TIMER* TimerHandle);

// ��ʼ��Timer
FlagStatus InitTimer(SW_TIMER* TimerHandle,
	               uint32_t TimerPeriod, /*��ʱ���ms*/
	               TIMER_CALLBACK CallbackFunc/*��ʱ�ص�����*/);

// ����Timer
FlagStatus StartTimer(SW_TIMER* TimerHandle);


// ����Timer��ʱ���
FlagStatus ChangeTimerPeriod(SW_TIMER* TimerHandle, uint32_t TimerPeriod/*��ʱ���*/);

// Get time of some softtimer from setting to now.
uint16_t PastTimeGet(TIMER* timer);

// Set time out value of softtimer.
void TimeOutSet(TIMER* timer, uint32_t timeout);

// Check whether time out.
FlagStatus IsTimeOut(TIMER* timer);
void WaitMs(uint32_t Time);
#endif

