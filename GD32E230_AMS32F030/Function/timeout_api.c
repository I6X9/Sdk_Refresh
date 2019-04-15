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
#include "timeout_api.h"
#include "app_config.h"
#include <stdint.h>

volatile uint32_t CORET_Systick_Count;			//CORET Systick count

// ��õ�ǰTimerʵ������ʱ�䣬��λms
uint16_t GetPassTime(SW_TIMER* TimerHandle)
{
	uint64_t LastSystickVal;

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
	uint64_t LastSystickVal;

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

	if(LastSystickVal >= (uint64_t)(TimerHandle->LastTickets + TimerHandle->TimerPeriod))
	{
		TimerHandle->Callback();
		TimerHandle->LastTickets = CORET_Systick_Count;
	}
}

// ��ʼ��Timer
FlagStatus InitTimer(SW_TIMER* TimerHandle,uint32_t TimerPeriod, /*��ʱ���ms*/TIMER_CALLBACK CallbackFunc/*��ʱ�ص�����*/)
{
	TimerHandle->TimerPeriod = TimerPeriod;
	TimerHandle->IsRunning = RESET;
	TimerHandle->Callback = CallbackFunc;
	return SET;
}

// ����Timer
FlagStatus StartTimer(SW_TIMER* TimerHandle)
{
	TimerHandle->LastTickets = CORET_Systick_Count;
	TimerHandle->IsRunning = SET;
	return SET;
}

// ����Timer��ʱ���
FlagStatus ChangeTimerPeriod(SW_TIMER* TimerHandle, uint32_t TimerPeriod/*��ʱ���*/)
{
	TimerHandle->IsRunning = RESET;
	TimerHandle->TimerPeriod = TimerPeriod;
	TimerHandle->LastTickets = CORET_Systick_Count;
	TimerHandle->IsRunning = SET;
	return SET;
}

// Get time of some softtimer from setting to now.
uint16_t PastTimeGet(TIMER* timer)
{
	uint64_t LastSystickVal;

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
void TimeOutSet(TIMER* timer, uint32_t timeout)
{
	timer->TimeOutVal = timeout;
    timer->TickValCache = CORET_Systick_Count;
}

// Check whether time out.
FlagStatus IsTimeOut(TIMER* timer)
{
	uint64_t LastSystickVal;

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
		return SET;
	}

	return RESET;
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

