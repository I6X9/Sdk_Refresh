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
#include "app_config.h"
#include "asm32s003_types_local.h"

extern volatile U32_T CORET_Systick_Count;			//CORET Systick count

// ��õ�ǰTimerʵ������ʱ�䣬��λms
U16_T GetPassTime(SW_TIMER* TimerHandle)
{
	U64_T LastSystickVal;

	if(CORET_Systick_Count < TimerHandle->LastTickets)
	{
		LastSystickVal = (MaxSystickCount + CORET_Systick_Count);
	}
	else
	{
		LastSystickVal = CORET_Systick_Count;
	}

	return (LastSystickVal - TimerHandle->LastTickets);
}

// Ӧ��ģʽ��ѭ���У����øú�������ѯTimer����ʱ����CallBack��
void CheckTimer(SW_TIMER* TimerHandle)
{
	U64_T LastSystickVal;

	if(!TimerHandle->IsRunning)
	{
		return;
	}

	if(CORET_Systick_Count < TimerHandle->LastTickets)
	{
		LastSystickVal = (MaxSystickCount + CORET_Systick_Count);
	}
	else
	{
		LastSystickVal = CORET_Systick_Count;
	}

	if(LastSystickVal >= (U64_T)(TimerHandle->LastTickets + TimerHandle->TimerPeriod))
	{
		TimerHandle->Callback();
		TimerHandle->LastTickets = CORET_Systick_Count;
	}
}

// ��ʼ��Timer
B_T InitTimer(SW_TIMER* TimerHandle,U32_T TimerPeriod, /*��ʱ���ms*/TIMER_CALLBACK CallbackFunc/*��ʱ�ص�����*/)
{
	TimerHandle->TimerPeriod = TimerPeriod;
	TimerHandle->IsRunning = FALSE;
	TimerHandle->Callback = CallbackFunc;
	return TRUE;
}

// ����Timer
B_T StartTimer(SW_TIMER* TimerHandle)
{
	TimerHandle->LastTickets = CORET_Systick_Count;
	TimerHandle->IsRunning = TRUE;
	return TRUE;
}

// ����Timer��ʱ���
B_T ChangeTimerPeriod(SW_TIMER* TimerHandle, U32_T TimerPeriod/*��ʱ���*/)
{
	TimerHandle->IsRunning = FALSE;
	TimerHandle->TimerPeriod = TimerPeriod;
	TimerHandle->LastTickets = CORET_Systick_Count;
	TimerHandle->IsRunning = TRUE;
	return TRUE;
}

// Get time of some softtimer from setting to now.
U16_T PastTimeGet(TIMER* timer)
{
	U64_T LastSystickVal;

	if(CORET_Systick_Count < timer->TickValCache)
	{
		LastSystickVal = (MaxSystickCount + CORET_Systick_Count);
	}
	else
	{
		LastSystickVal = CORET_Systick_Count;
	}

	return (LastSystickVal - timer->TickValCache);
}

// Set time out value of softtimer.
void TimeOutSet(TIMER* timer, U32_T timeout)
{
	timer->TimeOutVal = timeout;
    timer->TickValCache = CORET_Systick_Count;
}

// Check whether time out.
B_T IsTimeOut(TIMER* timer)
{
	U64_T LastSystickVal;

	if(CORET_Systick_Count < timer->TickValCache)
	{
		LastSystickVal = (MaxSystickCount + CORET_Systick_Count);
	}
	else
	{
		LastSystickVal = CORET_Systick_Count;
	}

	if(LastSystickVal >= (timer->TickValCache +timer->TimeOutVal))
	{
		timer->TickValCache = CORET_Systick_Count;
		return TRUE;
	}

	return FALSE;
}
/*****************************************************************************
 �� �� ��  : WaitMs
 ��������  : CPU�յ�һ��ʱ�䣬��λMs
 �������  : uint32_t Time  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��17��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
void WaitMs(uint32_t Time)
{
    Time = (Time+CORET_Systick_Count)%MaxSystickCount;
    while(CORET_Systick_Count != Time);    
}

