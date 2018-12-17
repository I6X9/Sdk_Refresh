#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__
#include "seg_panel.h"
//#include "display/led_tm162x.h"
typedef enum _DISP_DEV
{
    DISP_DEV_NONE = 0,
    DISP_DEV_SLED,
    DISP_DEV_LED1888,
    DISP_DEV_LED8888,
    DISP_DEV_LED57,
    DISP_DEV_LCD58

} DISP_DEV;

typedef enum _DISP_MSG_ID
{
    DISP_MSG_NONE = 0,
    DISP_MSG_PWR_ON,
    DISP_MSG_PWR_OFF,
    DISP_MSG_READ,
    DISP_MSG_DEV,
    DISP_MSG_FILE,
    DISP_MSG_ERR,
    DISP_MSG_MVOL,
    DISP_MSG_EQ,
    DISP_MSG_PLAY_MODE,
    DISP_MSG_REP_AB,
    DISP_MSG_MIC_VOL,
    DISP_MSG_GUITAR_VOL,
    DISP_MSG_TREB,
    DISP_MSG_BASS,
    DISP_MSG_DEC_MIX,
#ifdef FUNC_MULTI_UDISK_PORT_EN
    DISP_MSG_UDISK_PORT,
#endif
#ifdef FUNC_RADIO_EN
    DISP_MSG_RADIO_CH,
#endif
#ifdef FUNC_SOUND_REMIND
    DISP_MSG_ALARM_REMIND1,
    DISP_MSG_ALARM_REMIND2,
#endif
    DISP_MSG_ECHO_DEPTH,
    DISP_MSG_ECHO_DELAY,
    DISP_MSG_SEL_NUM,
#ifdef FUNC_RECORD_FILE_DELETE_EN
    DISP_MSG_FILE_DEL,
#endif
#ifdef FUNC_MIC_FIRST_EN
    DISP_MSG_MIC_FIRST,
#endif
#ifdef FUNC_AUDIO_3D_EFFECT_EN
    DISP_MSG_3D,
#endif
#ifdef FUNC_PROGRAM_EN
    DISP_MSG_PROG_FULL,
    DISP_MSG_PROG_NUM,
#endif
#ifdef FUNC_RTC_EN
	DISP_MSG_RTC_SET,
	DISP_MSG_WIFI_SET_ALARM,
#endif
#ifdef FUNC_BT_HF_EN
	DISP_MSG_BTHF_VOL,
#endif	

    DISP_MSG_VER,
    DISP_MSG_NO,
    DISP_MSG_MUTE

} DISP_MSG_ID;


typedef enum _DISP_STATE
{
    DISP_STATE_IDLE = 0,	//��״̬��������ʾ����ʱ�䣬RadioƵ�ʵ�
    DISP_STATE_DEV,
    DISP_STATE_FILE	//MP3���д�״̬

} DISP_STATE;

typedef union _UN_BLINK
{
	uint8_t 	FalshBlinkFlag;
	uint8_t	    SlowBlinkFlag;
	//����Լ���Ҫ����˸��־

} UN_BLINK;


#define AMPLITUDE_INTERVAL		100
#define FFFB_BLINK_INTERVAL		300
#define PLAY_BLINK_INTERVAL		500
#define MUTE_BLINK_INTERVAL		1000
#define SCAN_BLINK_INTERVAL		300
#define DEV_SYMBOL_INTERVAL		1000
#define NORMAL_INTERVAL			3000//1000
#define FM_NORMAL_INTERVAR              1500
#define FM_CH_INTERVAR                       500
#define INTERIM_INTERVAL		2000	// EQ��Repeat��Volume��ʾ����״̬
#define SET_INTERVL				5000	// RTC���õ���ʾ״̬����ʱ��
#define POWERON_INTERVL				5000

extern uint8_t const Led48IconSet[ICON_NUM];
extern DISP_TYPE	gDispBuff[];
extern UN_BLINK gBlink;
extern TIMER	DispTmr;
extern TIMER		DispMsgTmr;
//extern TIMER            LedFlashTmr;
//extern bool             LedFlashOn;
extern uint8_t gDispPrevRep;
extern uint8_t gDispPrevEQ;
extern uint8_t gDispCurVol;
#if 1//def FUNC_KARAOKE_EN
extern uint8_t 		gDispCurMicVol;
extern uint8_t 		gDispPrevMicVol;
#endif
extern uint8_t gDispPrevPlayState;
extern DISP_DEV	gDisplayMode;
extern bool gLedDispRefreshFlag;
extern uint8_t             gDispMsgId;

void DispSendMessage(uint16_t DispMsgTime, uint8_t DispMsgId);
// Clear Screen
void ClearScreen(void);
void ClearAllIcon(void);
// Display initilization
// Indicate whether the system in "STAND-BY" state or not.
void DispInit(bool IsStandBy);

// Display de-initialization.
// Indicate whether the system in "STAND-BY" state or not.
void DispDeInit(void);

// ��ʾˢ��.
// Call by display refresh ISR.
void LedFlushDisp(void);

// ��ʾˢ��.
// Lcd display refresh ISR.
void LcdFlushDisp(void);

// ����Repeatģʽʱ����.
void DispRepeat(bool IsForced);

// ����EQʱ����.
void DispEQ(bool IsForced);

// ����״̬�仯ʱ����(Play/Pause/Stop/FF/FB).
void DispPlayState(void);

// �ļ��й��ܴ�/�ر�ʱ����.
void DispFoldState(void);
#ifdef FUNC_KARAOKE_EN
void DispEchoDelayStep(bool IsForced);
// MIC��������ʱ����.
void DispMicVolume(bool IsForced);
void DispDecoderMix(bool IsForced);
#endif

#ifdef FUNC_TREB_BASS_EN
void DispTreb(bool IsForced);
void DispBass(bool IsForced);
#endif

// ��������ʱ����.
void DispVolume(bool IsForced);
#ifdef FUNC_PT231X_EN
// ��������ʱ����.
void DispTreb(bool IsForced);

// ��������ʱ����.
void DispBass(bool IsForced);
#endif
// Mute״̬�仯ʱ����.
void DispMute(void);

//" LOD"
void DispLoad(void);

// �ļ��л�ʱ����.
void DispDev(void);

#ifdef FUNC_RTC_EN
void DispRtc(bool DispState);
#endif

void DispNum(uint16_t Num);

// �豸�л�ʱ����.
void DispFileNum(void);

#define DIS_PLAYTIME_PALY    0
#define DIS_PLAYTIME_PAUSE   1
void DispPlayTime(uint8_t Type);
void DispResume(void);

// ����������.
void Display(void);

//��ʾPower on��Ϣ
void DispPowerOn(void);
void DispPowerOff(void);
// ����Audio��ʾʱ����
void DispAudio(void);
#ifdef FUNC_SEG_LCD_EN
void LCD_Drv_Init(void);
#endif
#ifdef FUNC_SLEEP_EN
void DispSleepRemainTime(void);
void DispSleepTime(void);
#endif
#endif
