/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : rtc_at8563.h
  Version       : Initial Draft
  Author        : qing
  Created       : 2019/8/21
  Last Modified :
  Description   : rtc_at8563.c header file
  Function List :
  History       :
  1.Date        : 2019/8/21
    Author      : qing
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#ifndef __RTC_AT8563_H__
#define __RTC_AT8563_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern bool RtcAt8563tAlarmCome(void);
extern bool RtcAt8563tInit(void);
extern void WiFiGetAt8563tSystemTime(RTC_DATE_TIME* SystemTime);
extern void WiFiSetAt8563tAlarmTime(bool IsOnOff, RTC_DATE_TIME* AlarmTime);
extern void WiFiSetAt8563tSystemTime(RTC_DATE_TIME* CurTime);
extern void SetAt8563tSecondAlarm(bool SecondAlarm);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __RTC_AT8563_H__ */
