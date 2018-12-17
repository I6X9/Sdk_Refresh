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

#include "asm32s003_types_local.h"

/*
 * define timer check for init task period event
 */
 #define MaxSystickCount   0xffffffff
 
typedef void (*TIMER_CALLBACK)(void);

typedef struct _SW_TIMER_
{
	U16_T TimerPeriod;  // ms,��ע�⡿��С��ʱ�����Ӧ��ģʽ��ѭ����������������ʱ���йأ����鲻Ҫ������ѭ�����������ʱ�䣻
	U32_T LastTickets;  // ����timeout�ж�
	TIMER_CALLBACK Callback;
	B_T  IsRunning;
} SW_TIMER;

// define softtimer structure.
typedef struct	_TIMER
{
	U16_T	TimeOutVal; 								//time out value
	U32_T	TickValCache;			  					//soft timer setting value
} TIMER;

// ��õ�ǰTimerʵ������ʱ�䣬��λms
U16_T GetPassTime(SW_TIMER* TimerHandle);

// Ӧ��ģʽ��ѭ���У����øú�������ѯTimer����ʱ����CallBack��
void CheckTimer(SW_TIMER* TimerHandle);

// ��ʼ��Timer
B_T InitTimer(SW_TIMER* TimerHandle,
	               U32_T TimerPeriod, /*��ʱ���ms*/
	               TIMER_CALLBACK CallbackFunc/*��ʱ�ص�����*/);

// ����Timer
B_T StartTimer(SW_TIMER* TimerHandle);


// ����Timer��ʱ���
B_T ChangeTimerPeriod(SW_TIMER* TimerHandle, U32_T TimerPeriod/*��ʱ���*/);

// Get time of some softtimer from setting to now.
U16_T PastTimeGet(TIMER* timer);

// Set time out value of softtimer.
void TimeOutSet(TIMER* timer, U32_T timeout);

// Check whether time out.
B_T IsTimeOut(TIMER* timer);
void WaitMs(uint32_t Time);
#endif

