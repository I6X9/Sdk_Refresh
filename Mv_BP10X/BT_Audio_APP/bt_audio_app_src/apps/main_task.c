/**
 **************************************************************************************
 * @file    main_task.c
 * @brief   Program Entry 
 *
 * @author  Sam
 * @version V1.0.0
 *
 * $Created: 2016-6-29 13:06:47$
 *
 * @Copyright (C) 2016, Shanghai Mountain View Silicon Co.,Ltd. All rights reserved.
 **************************************************************************************
 */

#include <string.h>
#include "type.h"
#include "app_config.h"
#include "timeout.h"
#include "rtos_api.h"
#include "app_message.h"
#include "debug.h"
#include "dma.h"
#include "clk.h"
#include "main_task.h"
#include "timer.h"
#include "otg_detect.h"
#include "irqn.h"
#include "watchdog.h"
#include "dac.h"
#include "dac_interface.h"
#include "audio_adc.h"
#include "adc_interface.h"
#ifdef CFG_RES_FLASHFS_EN
#include "file.h"
#endif
#include "breakpoint.h"
#include "audio_vol.h"
#include "audio_core_api.h"
#include "audio_core_service.h"
#include "device_service.h"
#include "device_detect.h" 
#include "bt_stack_service.h"
#ifdef CFG_FUNC_RECORDER_FLASHFS
#include "playback_service.h"
#endif
#include "remind_sound_service.h"
#include "shell.h"
#include "ctrlvars.h"
#include "communication.h"
#include "mode_switch_api.h"
#include "media_play_mode.h"
#include "flash_boot.h"
#include "breakpoint.h"
#include "sadc_interface.h"
#include "adc.h"
#include "otg_device_hcd.h"
#include "spdif.h"
#include "otg_device_standard_request.h"
#include "powercontroller.h"
#include "deepsleep.h"
#include "adc_key.h"
#include "delay.h"
#include "ir_key.h"
#include "rtc_alarm.h"
#include "rtc.h"
#include "i2s.h"
#include "i2s_interface.h"
#include "uarts_interface.h"
#include "bt_ddb_flash.h"
#include "bb_api.h"
#include "bt_config.h"

#ifdef CFG_FUNC_DISPLAY_EN
#include "display_service.h"
#endif
#if FLASH_BOOT_EN
void start_up_grate(uint32_t UpdateResource);
bool IsUpdataFileExist(void);
#endif
void SystemGotoPowerDown(void);
#ifdef CFG_FUNC_AUDIO_EFFECT_EN
extern void LineInModeEffectConfig(void);
extern void AudioEffectsInit_LineIn(void);
#endif
void SysVarInit(void);
inline void SleepMainAppTask(void);
inline void WakeupMainAppTask(void);
inline void StartModeExit(void);
static void SelectModeBeforeEnter(void);

#define MAIN_APP_TASK_STACK_SIZE		512//1024
#define MAIN_APP_TASK_PRIO				4
#define MAIN_NUM_MESSAGE_QUEUE			10

#define SHELL_TASK_STACK_SIZE			512//1024
#define SHELL_TASK_PRIO					2
#define	MODE_JITTER_TIME				50
#define	MODE_ENTER_TIME					5000
#define	MODE_EXIT_TIME					3000
#define	MODE_WORK_TIME					0xfffffff 
#define MAIN_APP_MSG_TIMEOUT			50	/* 50 ms */

//void mv_speex_init(void);
/**����appconfigȱʡ����:DMA 8��ͨ������**/
/*1��cec��PERIPHERAL_ID_TIMER3*/
/*2�����ߴ��ڵ�����PERIPHERAL_ID_UART1 RX/TX,����ʹ��USB HID����ʡDMA��Դ*/
/*3��Mic������PERIPHERAL_ID_AUDIO_ADC1_RX��mode֮��ͨ������һ��*/
/*4��Dac0������PERIPHERAL_ID_AUDIO_DAC0_TX mode֮��ͨ������һ��*/
/*5��DacX�迪��PERIPHERAL_ID_AUDIO_DAC1_TX mode֮��ͨ������һ��*/
/*ע��DMA 8��ͨ�����ó�ͻ:*/
/*a��UART���ߵ�����DAC-X�г�ͻ,Ĭ�����ߵ���ʹ��USB HID*/
/*b��UART���ߵ�����HDMI/SPDIFģʽ��ͻ*/

static const uint8_t DmaChannelMap[29] = {
	255,//PERIPHERAL_ID_SPIS_RX = 0,	//0
	255,//PERIPHERAL_ID_SPIS_TX,		//1
	5,//PERIPHERAL_ID_TIMER3,			//2
	4,//PERIPHERAL_ID_SDIO_RX,			//3
	4,//PERIPHERAL_ID_SDIO_TX,			//4
	255,//PERIPHERAL_ID_UART0_RX,		//5
	255,//PERIPHERAL_ID_TIMER1,			//6
	255,//PERIPHERAL_ID_TIMER2,			//7
	255,//PERIPHERAL_ID_SDPIF_RX,		//8 SPDIF_RX /TX same chanell
	255,//PERIPHERAL_ID_SDPIF_TX,		//8 SPDIF_RX /TX same chanell
	255,//PERIPHERAL_ID_SPIM_RX,		//9
	255,//PERIPHERAL_ID_SPIM_TX,		//10
	255,//PERIPHERAL_ID_UART0_TX,		//11
	
#ifdef CFG_COMMUNICATION_BY_UART	
	7,//PERIPHERAL_ID_UART1_RX,			//12
	6,//PERIPHERAL_ID_UART1_TX,			//13
#else
	255,//PERIPHERAL_ID_UART1_RX,		//12
	255,//PERIPHERAL_ID_UART1_TX,		//13
#endif

	255,//PERIPHERAL_ID_TIMER4,			//14
	255,//PERIPHERAL_ID_TIMER5,			//15
	255,//PERIPHERAL_ID_TIMER6,			//16
	0,//PERIPHERAL_ID_AUDIO_ADC0_RX,	//17
	1,//PERIPHERAL_ID_AUDIO_ADC1_RX,	//18
	2,//PERIPHERAL_ID_AUDIO_DAC0_TX,	//19
	3,//PERIPHERAL_ID_AUDIO_DAC1_TX,	//20
	255,//PERIPHERAL_ID_I2S0_RX,		//21
#if	(defined(CFG_RES_AUDIO_I2SOUT_EN )&&(CFG_RES_I2S_PORT==0))
	7,//PERIPHERAL_ID_I2S0_TX,			//22
#else	
	255,//PERIPHERAL_ID_I2S0_TX,		//22
#endif	
	255,//PERIPHERAL_ID_I2S1_RX,		//23
#if	(defined(CFG_RES_AUDIO_I2SOUT_EN )&&(CFG_RES_I2S_PORT==1))
	7,	//PERIPHERAL_ID_I2S1_TX,		//24
#else
	255,//PERIPHERAL_ID_I2S1_TX,		//24
#endif
	255,//PERIPHERAL_ID_PPWM,			//25
	255,//PERIPHERAL_ID_ADC,     		//26
	255,//PERIPHERAL_ID_SOFTWARE,		//27
};

//������ �ǼǱ�
uint32_t		SoftwareFlag;

static uint8_t gEnterBtHfMode = 0;

MainAppContext	mainAppCt;

TIMER ModeTimer;//����ģʽ�л�Ƶ�����Ա��ֽ��գ����У���Ϣ

static AppMode NextModeProcess(AppMode StartMode);

#ifdef CFG_COMMUNICATION_BY_UART
uint8_t UartRxBuf[1024] = {0};
uint8_t UartTxBuf[1024] = {0};
#endif


#ifdef CFG_RES_RTC_EN
__attribute__((section(".driver.isr")))void RtcInterrupt(void)
{
	if(RTC_IntFlagGet() == TRUE)
	{
#ifdef CFG_FUNC_ALARM_EN
		mainAppCt.AlarmFlag = TRUE;
		mainAppCt.AlarmID = RTC_IntAlarmIDGet();//��ǰֻ��rtcģʽ������Ч����flag��ϡ�
#endif
		RTC_IntFlagClear();
	}
}
#endif


static int32_t MainAppInit(void)
{
	memset(&mainAppCt, 0, sizeof(MainAppContext));

	mainAppCt.msgHandle = MessageRegister(MAIN_NUM_MESSAGE_QUEUE);
	mainAppCt.state = TaskStateCreating;
	mainAppCt.MState = ModeStateNone;
	mainAppCt.appBackupMode = AppModeIdle;
	mainAppCt.appCurrentMode = AppModeIdle;
	mainAppCt.appTargetMode = AppModeIdle;
	return 0;
}


//�����²�service created��Ϣ����Ϻ�start��Щservcie
static void MainAppServiceCreating(uint16_t msgId)
{
	if(msgId == MSG_AUDIO_CORE_SERVICE_CREATED)
	{
		DBG("MainApp:AudioCore service created\n");
		mainAppCt.AudioCoreSync = TRUE;
	}
	else if(msgId == MSG_DEVICE_SERVICE_CREATED)
	{
		DBG("MainApp:Device service created\n");
		mainAppCt.DeviceSync = TRUE;
	}
#ifdef CFG_FUNC_DISPLAY_EN
	if(msgId == MSG_DISPLAY_SERVICE_CREATED)
	{
		DBG("MainApp:Display service created\n");
		mainAppCt.DisplaySync = TRUE;
	}
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
	if(msgId == MSG_REMIND_SOUND_SERVICE_CREATED)
	{
		DBG("MainApp:Remind service created\n");
		mainAppCt.RemindSoundSync = TRUE;
	}
#endif

	if(mainAppCt.AudioCoreSync
		&& mainAppCt.DeviceSync
#ifdef CFG_FUNC_DISPLAY_EN
		&& mainAppCt.DisplaySync
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
		&& mainAppCt.RemindSoundSync
#endif
		)
	{
		DeviceServiceStart();
		mainAppCt.DeviceSync = FALSE;
		AudioCoreServiceStart();
		mainAppCt.AudioCoreSync = FALSE;
#ifdef CFG_FUNC_DISPLAY_EN
		DisplayServiceStart();
		mainAppCt.DisplaySync = FALSE;
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
		RemindSoundServiceStart();
		mainAppCt.RemindSoundSync = FALSE;
#endif
		mainAppCt.state = TaskStateReady;
	}
}


//�����²�service started����Ϻ�׼��ģʽ�л���
static void MainAppServiceStarting(uint16_t msgId)
{
	if(msgId == MSG_AUDIO_CORE_SERVICE_STARTED)
	{
		DBG("MainApp:AudioCore service started\n");
		mainAppCt.AudioCoreSync = TRUE;
	}
	else if(msgId == MSG_DEVICE_SERVICE_STARTED)
	{
		DBG("MainApp:Device service started\n");
		mainAppCt.DeviceSync = TRUE;
	}
#ifdef CFG_FUNC_DISPLAY_EN
	else if(msgId == MSG_DISPLAY_SERVICE_STARTED)
	{
		DBG("MainApp:Display service started\n");
		mainAppCt.DisplaySync = TRUE;
	}
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
	else if(msgId == MSG_REMIND_SOUND_SERVICE_STARTED)
	{
		DBG("MainApp:Remind service started\n");
		mainAppCt.RemindSoundSync = TRUE;
	}
#endif

	if(mainAppCt.AudioCoreSync
		&& mainAppCt.DeviceSync
#ifdef CFG_FUNC_DISPLAY_EN
		&& mainAppCt.DisplaySync
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
		&& mainAppCt.RemindSoundSync
#endif
		)
	{
		mainAppCt.DeviceSync = FALSE;
		mainAppCt.AudioCoreSync = FALSE;
#ifdef CFG_FUNC_DISPLAY_EN
		mainAppCt.DisplaySync = FALSE;
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
		mainAppCt.RemindSoundSync = FALSE;
#endif

		//�ȴ�������ʾ����deviceɨ��ʱ�Ρ���standbyģʽ��ʵʩ��
		SoftFlagRegister(SoftFlagFirstMode);//�˱���ڼ�target���ı䡣����Ӧdeepsleep���˳�/���롣

		mainAppCt.appCurrentMode = AppModeStandbyPlay;
		mainAppCt.appTargetMode = AppModeStandbyPlay;
		;//����Standbyģʽ������ʾ���������������׸���Чģʽ-���ϵ����
		//ModeStateSet(ModeCreate);
		if(ModeStateSet(ModeCreate))
		{
			mainAppCt.MState = ModeStateEnter;
			TimeOutSet(&ModeTimer, MODE_ENTER_TIME);//����ģʽ�� ��ʱ����
			mainAppCt.state = TaskStateRunning;
		}
		else
		{
			MessageContext		msgSend;
			mainAppCt.MState = ModeStateExit;
			msgSend.msgId		= MSG_MAINAPP_NEXT_MODE;
			//DBG("enter mode err, go to next mode\n");
			MessageSend(mainAppCt.msgHandle, &msgSend);
		}
	}
}

#ifdef CFG_FUNC_DEEPSLEEP_EN
static void MainAppServicePause(void)
{
	if(mainAppCt.state == TaskStateRunning)//����״̬����Ӧ
	{
#if (defined(CFG_APP_BT_MODE_EN) && defined(CFG_BT_BACKGROUND_RUN_EN))
		if(GetBtStackCt())
		{
			if(GetBtDeviceConnMode() == BT_DEVICE_CONNECTION_MODE_NONE)
			{
				BTDisconnect();
			}
			
			if(GetBtManager()->btReconnectTimer.timerFlag)
			{
				BtStopReconnect();
			}
		}

#endif
		DeviceServicePause();
		mainAppCt.DeviceSync = FALSE;
//		AudioCoreServicePause();
//		mainAppCt.AudioCoreSync = FALSE;
#ifdef CFG_FUNC_DISPLAY_EN
		DisplayServicePause();
		mainAppCt.DisplaySync = FALSE;
#endif

#ifdef CFG_FUNC_REMIND_SOUND_EN
		RemindSoundServicePause();
		mainAppCt.RemindSoundSync = FALSE;
#endif
		mainAppCt.state = TaskStatePausing;
		//Work̬�跢���˳���Noneֱ����ͣ��Enter��~ed��Ϣ��ʱִ��exit��Exit��~ed��Ϣ��ʱ����Pause̬
		if(mainAppCt.MState == ModeStateWork)
		{
			StartModeExit();
		}
	}
}

//�����²�service Pausing����Ϻ�׼��ģʽ�л���
static void MainAppServicePausing(uint16_t msgId)
{
//	if(msgId == MSG_AUDIO_CORE_SERVICE_PAUSED)
//	{
//		DBG("MainApp:AudioCore service paused\n");
//		mainAppCt.AudioCoreSync = TRUE;
//	}
//	else
	if(msgId == MSG_DEVICE_SERVICE_PAUSED)
	{
		DBG("MainApp:Device service paused\n");
		mainAppCt.DeviceSync = TRUE;
	}

#ifdef CFG_FUNC_DISPLAY_EN
	else if(msgId == MSG_DISPLAY_SERVICE_PAUSED)
	{
		DBG("MainApp:Display service paused\n");
		mainAppCt.DisplaySync = TRUE;
	}
#endif

#ifdef CFG_FUNC_REMIND_SOUND_EN
	else if(msgId == MSG_REMIND_SOUND_SERVICE_PAUSED)
	{
		DBG("MainApp:Remind service paused\n");
		mainAppCt.RemindSoundSync = TRUE;
	}
#endif

	if(mainAppCt.DeviceSync //mainAppCt.AudioCoreSync &&
#ifdef CFG_FUNC_REMIND_SOUND_EN
		&& mainAppCt.RemindSoundSync
#endif
#ifdef CFG_FUNC_DISPLAY_EN
		&& mainAppCt.DisplaySync
#endif
		&& (mainAppCt.MState == ModeStateNone || mainAppCt.MState == ModeStatePause)
		)
	{
#if (defined(CFG_APP_BT_MODE_EN) && defined(CFG_BT_BACKGROUND_RUN_EN))
		{
			uint8_t btDisconnectTimeout = 0;
			DBG("confirm bt disconnect\n");
			while(GetBtDeviceConnMode() == BT_DEVICE_CONNECTION_MODE_NONE)
			{
				//2s timeout
				vTaskDelay(50);
				btDisconnectTimeout++;
				if(btDisconnectTimeout>=40)
					break;
			}
			DBG("kill bt\n");
			//bb reset
			rwip_reset();
			BT_IntDisable();
			//Kill bt stack service
			BtStackServiceKill();
			vTaskDelay(10);
		}
#endif
		DBG("MainApp:Sleep\n");
		AudioCoreSinkMute(0,TRUE,TRUE);
		AudioCoreSinkDisable(0);
		vTaskPrioritySet(mainAppCt.taskHandle, 5);//�趨������ȼ�
		DMA_ChannelAllocTableSet((uint8_t*)DmaChannelMap);
		SleepMainAppTask();
		DeepSleeping();
#ifdef CFG_FUNC_WAKEUP_MCU_RESET
	Reset_McuSystem();
#endif
		WakeupMainAppTask();
#if (defined(CFG_APP_BT_MODE_EN) && defined(CFG_BT_BACKGROUND_RUN_EN))
		WDG_Feed();
		BtStackServiceStart();
		WDG_Feed();
#endif
		
		vTaskPrioritySet(mainAppCt.taskHandle, MAIN_APP_TASK_PRIO);
		DeviceServiceResume();
//		AudioCoreServiceResume();
#ifdef CFG_FUNC_DISPLAY_EN
		DisplayServiceResume();
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
		RemindSoundServiceResume();
#endif
		if(mainAppCt.appCurrentMode == AppModeUDiskPlayBack 
			|| mainAppCt.appCurrentMode == AppModeCardPlayBack
			|| mainAppCt.appCurrentMode == AppModeFlashFsPlayBack)
		{
			mainAppCt.appCurrentMode = NextModeProcess(mainAppCt.appCurrentMode);
			mainAppCt.appTargetMode = mainAppCt.appCurrentMode;
			mainAppCt.appBackupMode = AppModeIdle;//�����ģʽ
			SoftFlagDeregister(SoftFlagPlayback);//��ȫ������
		}
		else
		{
			mainAppCt.appTargetMode = mainAppCt.appCurrentMode;
		}
		ModeStateSet(ModeCreate);
		mainAppCt.MState = ModeStateEnter;
		TimeOutSet(&ModeTimer, MODE_ENTER_TIME);//����ģʽ�� ��ʱ����
		mainAppCt.state = TaskStateRunning;
		WDG_Feed();
		DelayMs(200);//ּ������һ��'po'������mic�йأ��ɸ���ʵ��Ч��΢����ʱ��
		AudioCoreSinkEnable(0);
		AudioCoreSinkUnmute(0,TRUE,TRUE);
		SystemVolSet();
	}
}
#endif

//����ģʽ��ѡ����һ����Чģʽ����ģʽ��ʱ�������ȡ�
//backup������ǰ������backup�����£���һ��ģʽ��backup��backup����һ��ģʽ��ֻ�ڷ���Enterʱ����
static AppMode NextModeProcess(AppMode StartMode)
{
	AppMode FindMode;

	if(mainAppCt.appBackupMode != AppModeIdle)
	{
		if(CheckModeResource(mainAppCt.appBackupMode))
		{
			FindMode = mainAppCt.appBackupMode;
		}
		else
		{
			FindMode = NextAppMode(mainAppCt.appBackupMode);
		}
	}
	else
	{
		FindMode = NextAppMode(mainAppCt.appCurrentMode);
	}
	return FindMode;
}

/**
 * @func        MsgProcessModeResource
 * @brief       ����ģʽ��Դ����¼���Ϣ�������ı�ģʽҲ��˴���������Ϣ���������ģʽƵ���л�
 * @param       uint16_t msgId  
 * @Output      None
 * @return      bool TRUE��appTargetMode���ģ�FALSE��appTargetMode���䡣
 * @Others      �˺�������targetmode���ص���ģʽѡ��Ĳ���Ԥ���塣�����Ƿ��л�currentModeȡ����MStateģʽ״̬��
 * Record
 */
static bool MsgProcessModeResource(uint16_t msgId)
{
	AppMode AppModes = AppModeStandbyPlay;
	switch(msgId)
	{
		case MSG_DEVICE_SERVICE_LINE_IN:
		case MSG_DEVICE_SERVICE_CARD_IN:
		case MSG_DEVICE_SERVICE_DISK_IN:
#ifndef CFG_COMMUNICATION_BY_USB //USB HID���ߵ�����USB device ��������Ȳ�����
		case MSG_DEVICE_SERVICE_USB_DEVICE_IN:
#endif
			if(!SoftFlagGet(SoftFlagRecording | SoftFlagPlayback) //�طŻ�¼���ڼ䣬��������
#ifdef CFG_APP_BT_MODE_EN
#if (BT_HFP_SUPPORT == ENABLE)
				&& (mainAppCt.appCurrentMode != AppModeBtHfPlay) //����ͨ�������У�������
#endif
#endif
				)
			{
				if(!SoftFlagGet(SoftFlagFirstMode))//�������׶�
				{
#ifndef CFG_FUNC_APP_MODE_AUTO
					AppModes = FindModeByPlugInMsg(msgId);
#else
					if(mainAppCt.appTargetMode == AppModeStandbyPlay)//����Դʱ�����κ�����Դ��Ҫ����ģʽ���ͺ���Ȳ��޹ء�
					{
						AppModes = FindModeByPlugInMsg(msgId);
					}
#endif
				}
				if(AppModes != mainAppCt.appTargetMode && AppModes != AppModeStandbyPlay)
				{
					mainAppCt.appTargetMode = AppModes;
					if(mainAppCt.appCurrentMode != AppModeStandbyPlay)//standby������ˡ�
					{
						mainAppCt.appBackupMode = mainAppCt.appCurrentMode;//����Ȳ�ֻ��һ��ģʽ����
					}
					return TRUE;
				}
			}
			break;
		case MSG_DEVICE_SERVICE_CARD_OUT:
		case MSG_DEVICE_SERVICE_LINE_OUT:
		case MSG_DEVICE_SERVICE_DISK_OUT:
#ifndef CFG_COMMUNICATION_BY_USB //USB���ߵ�����USB device ��������Ȳ�����
		case MSG_DEVICE_SERVICE_USB_DEVICE_OUT:	
#endif
			if(!CheckModeResource(mainAppCt.appCurrentMode))
			{
				AppModes  = NextModeProcess(mainAppCt.appCurrentMode);
				if(AppModes != mainAppCt.appTargetMode)
				{
					mainAppCt.appTargetMode = AppModes;
					DBG("\nMainApp:Find New:%s @:%s  DevOut:%d ",ModeNameStr(AppModes), ModeNameStr(mainAppCt.appTargetMode), msgId);
					return TRUE;
				}
			}
			break;
			
#ifdef CFG_APP_BT_MODE_EN
#ifdef BT_AUTO_ENTER_PLAY_MODE
		case MSG_DEVICE_SERVICE_BTPLAY_IN:
			DBG("MSG: MSG_DEVICE_SERVICE_BTPLAY_IN\n");
			if(mainAppCt.appCurrentMode != AppModeBtAudioPlay)
			{
				mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				mainAppCt.appTargetMode = AppModeBtAudioPlay;
				return TRUE;
			}
			break;
#endif

#if (BT_HFP_SUPPORT == ENABLE)
		case MSG_DEVICE_SERVICE_BTHF_IN:
			DBG("MSG: MSG_DEVICE_SERVICE_BTHF_IN\n");
			if((mainAppCt.appCurrentMode != AppModeBtHfPlay)&&(!gEnterBtHfMode))
			{
				mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				mainAppCt.appTargetMode = AppModeBtHfPlay;
				gEnterBtHfMode = 1;
				return TRUE;
			}
			break;
			
		case MSG_DEVICE_SERVICE_BTHF_OUT:
			DBG("MSG: MSG_DEVICE_SERVICE_BTHF_OUT\n");
			if((mainAppCt.appCurrentMode == AppModeBtHfPlay)&&(gEnterBtHfMode))
			{
				gEnterBtHfMode = 0;
				if((mainAppCt.appBackupMode != AppModeIdle)&&(mainAppCt.appBackupMode != AppModeBtHfPlay))
				{
					mainAppCt.appTargetMode = mainAppCt.appBackupMode;
				}
				else
				{
					mainAppCt.appTargetMode = AppModeBtAudioPlay;
				}
				return TRUE;
			}
			break;
#endif
#endif

		case MSG_DEVICE_SERVICE_RECORDER://switch to line recorde only for usb_play and card_play mode
			
		if(mainAppCt.appCurrentMode==AppModeCardAudioPlay||mainAppCt.appCurrentMode==AppModeUDiskAudioPlay)
			msgId=MSG_DEVICE_SERVICE_LINE_IN;// 
		else
			break;	
		if(!SoftFlagGet(SoftFlagRecording | SoftFlagPlayback))//�طŻ�¼���ڼ䣬��������
			{
				if(!SoftFlagGet(SoftFlagFirstMode))//�������׶�
				{
#ifndef CFG_FUNC_APP_MODE_AUTO
					AppModes = FindModeByPlugInMsg(msgId);
#else
					if(mainAppCt.appTargetMode == AppModeStandbyPlay)//����Դʱ�����κ�����Դ��Ҫ����ģʽ���ͺ���Ȳ��޹ء�
					{
						AppModes = FindModeByPlugInMsg(msgId);
					}
#endif
				}
				if(AppModes != mainAppCt.appTargetMode && AppModes != AppModeStandbyPlay)
				{
					mainAppCt.appTargetMode = AppModes;
					if(mainAppCt.appCurrentMode != AppModeStandbyPlay)//standby������ˡ�
					{
						mainAppCt.appBackupMode = mainAppCt.appCurrentMode;//����Ȳ�ֻ��һ��ģʽ����
					}
				SoftFlagRegister(SoftFlagMediaToLineRecMode);

					return TRUE;
				}
			}

			
			break;

		case MSG_DEVICE_SERVICE_SWITCH_MODE:
#ifdef CFG_APP_BT_MODE_EN
#if (BT_HFP_SUPPORT == ENABLE)
			if(mainAppCt.appCurrentMode == AppModeBtHfPlay) //����ͨ�������У�����ǿ���л�ģʽ
			{
				return FALSE;
			}
#endif
#endif
			AppModes = NextModeProcess(mainAppCt.appCurrentMode);
			if(AppModes != mainAppCt.appTargetMode)
			{
				mainAppCt.appTargetMode = AppModes;
				return TRUE;
			}
			break;
#ifdef CFG_FUNC_RECORDER_EN
		case MSG_DEVICE_SERVICE_PLAYBACK:
			if((mainAppCt.appCurrentMode == AppModeUDiskPlayBack 
				|| mainAppCt.appCurrentMode == AppModeCardPlayBack
				|| mainAppCt.appCurrentMode == AppModeFlashFsPlayBack)
			   && mainAppCt.appBackupMode != AppModeIdle)
			{
				mainAppCt.appTargetMode = NextModeProcess(mainAppCt.appBackupMode);
				return TRUE;
			}
#ifdef CFG_FUNC_RECORD_SD_UDISK
#ifdef CFG_FUNC_RECORD_UDISK_FIRST
			if(CheckModeResource(AppModeUDiskPlayBack))
			{

				mainAppCt.appTargetMode = AppModeUDiskPlayBack;
				if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
				{
					mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				}
				SoftFlagRegister(SoftFlagPlayback);
				DBG("MainApp:target:%s\n", "AppModeUDiskPlayBack");
				return TRUE;
			}
			else if( CheckModeResource(AppModeCardPlayBack))
			{
				mainAppCt.appTargetMode = AppModeCardPlayBack;
				if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
				{
					mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				}
				SoftFlagRegister(SoftFlagPlayback);
				DBG("MainApp:target:%s\n", "AppModeCardPlayBack");
				return TRUE;
			}
			break;
#else //CFG_FUNC_RECORD_UDISK_FIRST
			if(CheckModeResource(AppModeCardPlayBack))
			{

				mainAppCt.appTargetMode = AppModeCardPlayBack;
				if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
				{
					mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				}
				SoftFlagRegister(SoftFlagPlayback);
				DBG("MainApp:target:%s\n", "AppModeCardPlayBack");
				return TRUE;
			}
			else if(CheckModeResource(AppModeUDiskPlayBack))
			{
				mainAppCt.appTargetMode = AppModeUDiskPlayBack;
				if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
				{
					mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
				}
				SoftFlagRegister(SoftFlagPlayback);
				DBG("MainApp:target:%s\n", "AppModeUDiskPlayBack");
				return TRUE; 
			}
			break;
#endif//CFG_FUNC_RECORD_UDISK_FIRST
#elif defined(CFG_FUNC_RECORD_FLASHFS)
		if(CheckModeResource(AppModeFlashFsPlayBack))
		{
			mainAppCt.appTargetMode = AppModeFlashFsPlayBack;
			if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
			{
				mainAppCt.appBackupMode = mainAppCt.appCurrentMode;
			}
			SoftFlagRegister(SoftFlagPlayback);
			DBG("MainApp:target:%s\n", "AppModeFlashFsPlayBack");
			return TRUE;
		}
#endif //CFG_FUNC_RECORD_SD_UDISK
#endif//CFG_FUNC_RECORDER_EN
		default:
			break;	
	}
	return FALSE;
}

static void SelectFirstMode(void)
{
	mainAppCt.appTargetMode = AppModeStandbyPlay ;//AppModeBtAudioPlay;//���ʼֵ
	if(mainAppCt.appBackupMode != AppModeIdle)//�жϵ����
	{
		mainAppCt.appTargetMode = NextModeProcess(mainAppCt.appBackupMode);
		DBG("\n MainApp:first select 0:%s\n", ModeNameStr(mainAppCt.appTargetMode));
	}
	else
	{
		mainAppCt.appTargetMode = FindPriorityMode();
		DBG("\n MainApp:first select 1:%s\n", ModeNameStr(mainAppCt.appTargetMode));
	}
	if(mainAppCt.appTargetMode != AppModeStandbyPlay)//ע�⣺�˺�ϵ����ģʽ������ռ
	{
		ModeStateSet(ModeStop);
		mainAppCt.MState = ModeStateExit;
		TimeOutSet(&ModeTimer, MODE_EXIT_TIME);//����ģʽ�� ��ʱ����
	}
	//û�к���ģʽʱ������standby�����˳���
}

//���exit��ʵʩEnterǰģʽ�жϡ�
static void SelectModeBeforeEnter(void)
{
	if(!CheckModeResource(mainAppCt.appTargetMode))//�ٴ���Դȷ�ϣ��Է���Ϣ��ʧ��������Ч��ģʽ����
	{
		mainAppCt.appTargetMode = NextModeProcess(mainAppCt.appCurrentMode);
		mainAppCt.appBackupMode = AppModeIdle;//�����ģʽ
		SoftFlagDeregister(SoftFlagPlayback);//��ȫ������
	}
	else if(mainAppCt.appBackupMode != AppModeIdle && (!CheckModeResource(mainAppCt.appBackupMode) || mainAppCt.appBackupMode == mainAppCt.appTargetMode))
	{//�������������طţ��ο�����ʱҪ����������һ��ģʽ�����backup����
		mainAppCt.appBackupMode = AppModeIdle;
		SoftFlagDeregister(SoftFlagPlayback);//��ȫ������
	}
	if(mainAppCt.appTargetMode == AppModeStandbyPlay)
	{
	
		mainAppCt.appCurrentMode = mainAppCt.appTargetMode = AppModeStandbyPlay;//ǿ����������ģʽ
		vTaskDelay(10);//������ּ����idle�������У���ɾ��task�ͷ���Դ��������Ƭ��
		
		DBG("\nMainApp:Start:%s\n", ModeNameStr(mainAppCt.appCurrentMode));
		ModeStateSet(ModeCreate);//�˴���Ӧ���ǳ�ʼ��ʧ�ܺ��ģʽ������
		mainAppCt.MState = ModeStateEnter;
		TimeOutSet(&ModeTimer, MODE_ENTER_TIME);//����ģʽ�� ��ʱ����
	}
	else
	{
		mainAppCt.appCurrentMode = mainAppCt.appTargetMode;
		vTaskDelay(10);//������ּ����idle�������У���ɾ��task�ͷ���Դ��������Ƭ��
		
		DBG("\nMainApp:Start:%s\n", ModeNameStr(mainAppCt.appCurrentMode));
		//ModeStateSet(ModeCreate);//�˴���Ӧ���ǳ�ʼ��ʧ�ܺ��ģʽ������
		if(ModeStateSet(ModeCreate))
		{
			mainAppCt.MState = ModeStateEnter;
			TimeOutSet(&ModeTimer, MODE_ENTER_TIME);//����ģʽ�� ��ʱ����
		}
		else
		{
			MessageContext		msgSend;
			mainAppCt.MState = ModeStateExit;
			msgSend.msgId		= MSG_MAINAPP_NEXT_MODE;
			//DBG("enter mode err, go to next mode\n");
			MessageSend(mainAppCt.msgHandle, &msgSend);	
		}
	}
}

inline void StartModeExit(void)
{
	ModeStateSet(ModeStop);
	mainAppCt.MState = ModeStateExit;
	TimeOutSet(&ModeTimer, MODE_EXIT_TIME);//��ʱά��	
}


inline void EndModeExit(void)
{
	SoftFlagDeregister(SoftFlagDecoderMask | SoftFlagDecoderSwitch);//��ȫ������
	ModeStateSet(ModeKill);
	//vTaskDelay(10);//������ּ����idle�������У���ɾ��task�ͷ���Դ��������Ƭ��
	SoftFlagDeregister(SoftFlagRecording | SoftFlagPlayback);//����
	MessageContext		MsgSend;
	MsgSend.msgId = MSG_APP_EXIT;
	MessageSend(GetDeviceMessageHandle(), &MsgSend);//֪ͨdevice����keymaptable
#ifdef CFG_FUNC_DEEPSLEEP_EN
	if(mainAppCt.state == TaskStatePausing)//deepsleep����׶Σ�mode������̬ͣ��
	{
		vTaskDelay(10);//������ּ����idle�������У���ɾ��task�ͷ���Դ��������Ƭ��
		mainAppCt.MState = ModeStatePause;
		MainAppServicePausing(MSG_NONE);
	}
	else
#endif
	{
		SelectModeBeforeEnter();
	#if (defined(CFG_APP_USB_AUDIO_MODE_EN))&&(defined(CFG_COMMUNICATION_BY_USB))
		if(GetUSBAudioExitFlag())
		{
			SetUSBAudioExitFlag(FALSE);
			SetDeviceDetectVarReset();
		}
	#endif
	}
}

//��Ƶģʽ����ʱ����ģʽ�����Ϣ���жϺ�����
static void StateAtAppRunning(bool Change)
{
	if(Change && !SoftFlagGet(SoftFlagFirstMode))//app���Ž׶���ģʽ������
	{
		DBG("MainApp:Work change Jitter!\n");
		TimeOutSet(&ModeTimer, MODE_JITTER_TIME);
	}

	if(IsTimeOut(&ModeTimer) && (mainAppCt.appCurrentMode != mainAppCt.appTargetMode))
	{
		StartModeExit();
	}
}

/**
 * @func        MainAppModeProcess
 * @brief       ģʽ�Ľ��롢�˳�������
 * @param       uint16_t Msg  
 * @Output      None
 * @return      None
 * @Others      �˺�����Ҫ��mode��״̬������϶�ʱ�������ͱ�����ʵ������app,created/started ����stopped,kill Msgͬ����
 * @Others      Ҫ�㣺work״̬�ڼ���Ϣ������ת��
 * Record
 */
static void MainAppModeProcess(uint16_t Msg)
{
	MessageContext		msgSend;
	bool 				TargetModeChange = FALSE;

	if(!SoftFlagGet(SoftFlagFirstMode))//�����׶Σ����Զ����ģʽ�仯��
	{
		TargetModeChange = MsgProcessModeResource(Msg);
	}
	switch(mainAppCt.MState)
	{
//			//���� ��һ��ѡģʽapp
//		case ModeStateNone:
//			BeforeEnterFirstMode();
//			break;

			//����̬�������µ�TargetMode��������
		case ModeStateEnter: 
			if(IsTimeOut(&ModeTimer))
			{
				DBG("\nMainApp:Enter %s timeout��care for ram.\n", ModeNameStr(mainAppCt.appCurrentMode));
				StartModeExit();
			}
			else
			{
				switch(Msg)//�������޹ص���Ϣ������
				{	
						//ģʽ�Ѵ��� ����
					case MSG_RTC_MODE_CREATED:
					case MSG_STANDBY_PLAY_MODE_CREATED:
					case MSG_LINE_AUDIO_MODE_CREATED:
					case MSG_MEDIA_PLAY_MODE_CREATED:
					case MSG_BT_PLAY_MODE_CREATED:
					case MSG_BT_HF_MODE_CREATED:
					case MSG_HDMI_AUDIO_MODE_CREATED:
					case MSG_USB_DEVICE_MODE_CREATED:
					case MSG_RADIO_AUDIO_MODE_CREATED:
					case MSG_SPDIF_AUDIO_MODE_CREATED:
					case MSG_I2SIN_AUDIO_MODE_CREATED:
						if(mainAppCt.state == TaskStatePausing)
						{
							StartModeExit();
						}
						else if(mainAppCt.appCurrentMode == mainAppCt.appTargetMode || SoftFlagGet(SoftFlagFirstMode))
						{
							ModeStateSet(ModeStart);
						}
						else
						{
							StartModeExit();
							DBG("\nMainApp:exiting %s to %s After Created", ModeNameStr(mainAppCt.appCurrentMode), ModeNameStr(mainAppCt.appTargetMode));
						}
						break;

						//ģʽ������ ����
					case MSG_RTC_MODE_STARTED:
					case MSG_STANDBY_PLAY_MODE_STARTED:
					case MSG_LINE_AUDIO_MODE_STARTED:
					case MSG_MEDIA_PLAY_MODE_STARTED:
					case MSG_BT_PLAY_MODE_STARTED:
					case MSG_BT_HF_MODE_STARTED:
					case MSG_HDMI_AUDIO_MODE_STARTED:
					case MSG_USB_DEVICE_MODE_STARTED:
					case MSG_RADIO_AUDIO_MODE_STARTED:
					case MSG_SPDIF_AUDIO_MODE_STARTED:
					case MSG_I2SIN_AUDIO_MODE_STARTED:
						if(mainAppCt.state == TaskStatePausing)
						{
							StartModeExit();
						}
						else if(mainAppCt.appCurrentMode == mainAppCt.appTargetMode || SoftFlagGet(SoftFlagFirstMode))//˵�����������targetyû�б仯,�ϵ�ģʽ����
						{
							DBG("MainApp:Mode Started\n");
							mainAppCt.MState = ModeStateWork;
							TimeOutSet(&ModeTimer, MODE_WORK_TIME);
							msgSend.msgId = Msg;
							MessageSend(GetDeviceMessageHandle(), &msgSend);//֪ͨdevice����keymaptable	
                            #ifdef CFG_FUNC_DISPLAY_EN
							msgSend.msgId = MSG_DISPLAY_SERVICE_DEV;
							MessageSend(GetDisplayMessageHandle(), &msgSend);
                            #endif
#ifdef	CFG_FUNC_AUDIO_EFFECT_EN
							if(mainAppCt.appCurrentMode != AppModeStandbyPlay)
							{
								void EffectChange(void);
								EffectChange();
							}
#endif
						}
						else//target�б仯��������˳�����
						{
							StartModeExit();
						}
						break;
				}
			}
			break;

			//�ȶ�����״̬��ϵͳ��Ϣ�����ڴ˴���������Ϣת��ǰmode App��
		case ModeStateWork:
			AudioPlayerMenuCheck();
			switch(Msg)
			{
					//ģʽ���ϴ���
				case MSG_MEDIA_PLAY_MODE_FAILURE://����������������stop����;like:SD/Udisk�ա���ʼ��ʧ��
					mainAppCt.appTargetMode = NextModeProcess(mainAppCt.appCurrentMode);
#ifdef	CFG_FUNC_DEV_ERROR_MODE
					//�����豸���ϵ�ģʽ�����������ʧ���˳����ر���ֻ��һ������ģʽʱ��
					if(mainAppCt.appCurrentMode == AppModeCardAudioPlay)
					{
						ResourceDeregister(AppResourceCardForPlay);
					}
					else if(mainAppCt.appCurrentMode == AppModeUDiskAudioPlay)
					{
						ResourceDeregister(AppResourceUDiskForPlay);
					}
#endif
					StartModeExit();
					DBG("MainApp:MEDIA_PLAY_MODE_FAILURE\n");
					break;

				case MSG_STANDY_PERMISSION_MODE:
					if(SoftFlagGet(SoftFlagFirstMode))//�����Լ�⣬������Ϣ�ط��������
					{
						SoftFlagDeregister(SoftFlagFirstMode);
						SelectFirstMode();
					}
					break;

				default: //��ϵͳ��Ϣת����app
					//AudioPlayerMenuCheck();
					if(Msg != MSG_NONE && GetAppMessageHandle() != NULL)
					{
					
					//	DBG("MSG=%x\n",Msg);
						msgSend.msgId = Msg;
						MessageSend(GetAppMessageHandle(), &msgSend);
					}
					else if(SoftFlagGet(SoftFlagMediaToLineRecMode))
						{
						
							if(mainAppCt.appCurrentMode==AppModeLineAudioPlay)
							{
							SoftFlagDeregister(SoftFlagMediaToLineRecMode);
							msgSend.msgId = MSG_DEVICE_SERVICE_RECORDER;
							MessageSend(GetAppMessageHandle(), &msgSend);
							}

						}


					break;
			}
			StateAtAppRunning(TargetModeChange);
			break;

				//����̬�������µ�TargetMode��������
		case ModeStateExit:
			if(IsTimeOut(&ModeTimer))
			{
				DBG("\nMainApp:Exit %s timeout��care for task.\n", ModeNameStr(mainAppCt.appCurrentMode));
				EndModeExit();
			}
			else 
			{
				switch(Msg)
				{
					//ģʽ��ֹͣ���Ĵ���
					case MSG_MAINAPP_NEXT_MODE://main taskǿ���˳�mode
					case MSG_RTC_MODE_STOPPED:
					case MSG_STANDBY_PLAY_MODE_STOPPED:
					case MSG_MEDIA_PLAY_MODE_STOPPED:
					case MSG_HDMI_AUDIO_MODE_STOPPED:
					case MSG_LINE_AUDIO_MODE_STOPPED:
					case MSG_BT_PLAY_MODE_STOPPED:
					case MSG_BT_HF_MODE_STOPPED:
					case MSG_USB_DEVICE_MODE_STOPPED:
					case MSG_RADIO_AUDIO_MODE_STOPPED:
					case MSG_SPDIF_AUDIO_MODE_STOPPED:
					case MSG_I2SIN_AUDIO_MODE_STOPPED:
						DBG("MainApp:exit stopped OK !\n");
						EndModeExit();
						break;
				}
				
			}
			break;
		default://case ModeStatePause:
//			if(mainAppCt.appTargetMode != AppModeIdle && CheckModeResource(mainAppCt.appTargetMode))
//			{
//				mainAppCt.appCurrentMode = mainAppCt.appTargetMode;
//				DBG("\nMainApp:Enter:%s\n", ModeNameStr(mainAppCt.appCurrentMode));
//				ModeStateSet(ModeCreate);//�˴���Ӧ���ǳ�ʼ��ʧ�ܺ��ģʽ������
//				mainAppCt.MState = ModeStateEnter;
//				TimeOutSet(&ModeTimer, MODE_ENTER_TIME);//����ģʽ�� ��ʱ����
//			}
			break;
	}
}


extern void rwip_reset(void);

//test, sam 20190702
void AudioResRelease(void);
bool AudioResMalloc(uint16_t SampleLen);
void AudioResInit(void);
void SamplesFrameUpdata(void);
static bool IsEffectProcessDone = 1;
void EffectProcessFlagSet(bool flag)
{
	IsEffectProcessDone = flag;
}

bool EffectProcessFlagGet(void)
{
	return IsEffectProcessDone;
}
static void MainAppTaskEntrance(void * param)
{
	int16_t i;
	MessageContext		msg;

	DMA_ChannelAllocTableSet((uint8_t*)DmaChannelMap);
#ifdef CFG_FUNC_MAIN_DEEPSLEEP_EN


	WDG_Feed();
	vTaskDelay(3000); //���Խ׶���ر��ִ���ʱ���Է�оƬ����SW���޷��ٴ����أ�������flashboot
	WDG_Feed();
	vTaskDelay(2000);
	DBG("Main:DeepSleep\n");
//#ifdef CFG_PARA_WAKEUP_SOURCE_IR
//	IRKeyInit();//�˴�ΪIR���ó�ʼ�� 
////#endif
	DeepSleeping();
#endif
	SarADC_Init();

	//For OTG check
 	OTG_PortSetDetectMode(1,1);
 	Timer_Config(TIMER2,1000,0);
 	Timer_Start(TIMER2);
 	NVIC_EnableIRQ(Timer2_IRQn);

#ifdef CFG_FUNC_BREAKPOINT_EN
 	BP_LoadInfo();
#endif
	SoftFlagDeregister(SoftFlagMask);

	SysVarInit();
	
	CtrlVarsInit();//��Ч��ʼ��������������Ҫ����	

	///////////////////////////////AudioCore/////////////////////////////////////////
	mainAppCt.AudioCore =  (AudioCoreContext*)&AudioCore;
	memset(mainAppCt.AudioCore, 0, sizeof(AudioCoreContext));
	mainAppCt.SamplesPreFrame = CFG_PARA_SAMPLES_PER_FRAME;//Ĭ������֡��С
	mainAppCt.SampleRate = CFG_PARA_SAMPLE_RATE;
	if(!AudioResMalloc(mainAppCt.SamplesPreFrame))
	{
		DBG("Res Error!\n");
		return;
	}
	
	//DAC init
#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_Init(ALL, mainAppCt.SampleRate, (void*)mainAppCt.DACFIFO, mainAppCt.SamplesPreFrame * 2 * 2 *2, (void*)mainAppCt.DACXFIFO, mainAppCt.SamplesPreFrame * 2 * 2);
#else
	AudioDAC_Init(DAC0, mainAppCt.SampleRate, (void*)mainAppCt.DACFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2, NULL, 0);
#endif

#ifdef CFG_RES_AUDIO_I2SOUT_EN
{
	I2SParamCt i2s_set;
	i2s_set.IsMasterMode=!CFG_RES_I2S_MODE;// 1:master 0:slave
	i2s_set.SampleRate=mainAppCt.SampleRate;
	i2s_set.I2sFormat=I2S_FORMAT_I2S;
	i2s_set.I2sBits=I2S_LENGTH_16BITS;
	i2s_set.I2sTxRxEnable=1;
#if (CFG_RES_I2S_PORT == 0)
	i2s_set.TxPeripheralID=PERIPHERAL_ID_I2S0_TX;
#else
	i2s_set.TxPeripheralID=PERIPHERAL_ID_I2S1_TX;
#endif
	i2s_set.TxBuf=(void*)mainAppCt.I2SFIFO;
	i2s_set.TxLen=mainAppCt.SamplesPreFrame * 2 * 2 * 2;
#if (CFG_RES_I2S_IO_PORT==0)
//i2s0  group_gpio0
	GPIO_PortAModeSet(GPIOA0, 9);// mclk out
	GPIO_PortAModeSet(GPIOA1, 6);// lrclk
	GPIO_PortAModeSet(GPIOA2, 5);// bclk
	GPIO_PortAModeSet(GPIOA3, 7);// dout
	GPIO_PortAModeSet(GPIOA4, 1);// din
//i2s0  group_gpio0
#else //lif (CFG_RES_I2S_IO_PORT==1)
//i2s1  group_gpio1 
	GPIO_PortAModeSet(GPIOA7, 5);//mclk out
	GPIO_PortAModeSet(GPIOA8, 1);//lrclk
	GPIO_PortAModeSet(GPIOA9, 2);//bclk
	GPIO_PortAModeSet(GPIOA10, 4);//do
	GPIO_PortAModeSet(GPIOA11, 2);//di
//i2s1  group_gpio1 
#endif
#if (CFG_RES_I2S_PORT == 0)
	AudioI2S_Init(I2S0_MODULE, &i2s_set);
#else
	AudioI2S_Init(I2S1_MODULE, &i2s_set);
#endif
}

#endif

	//Mic1 analog  = Soure0.
	AudioADC_AnaInit();
	//AudioADC_VcomConfig(1);//MicBias en
	AudioADC_MicBias1Enable(1);

	AudioADC_DynamicElementMatch(ADC1_MODULE, TRUE, TRUE);
	AudioADC_PGASel(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1);
	AudioADC_PGASel(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2);
	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1, 15, 4);//0db bypass
	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2, 15, 4);

	//Mic1   digital
	AudioADC_DigitalInit(ADC1_MODULE, mainAppCt.SampleRate, (void*)mainAppCt.ADCFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2);

	//Soure0.
	#if CFG_RES_MIC_SELECT
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].Enable = 0;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].FuncDataGet = AudioADC1DataGet;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].FuncDataGetLen = AudioADC1DataLenGet;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].IsSreamData = TRUE;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].PcmFormat = 2;//test mic audio effect
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].PcmInBuf = (int16_t *)mainAppCt.Source0Buf_ADC1;
	AudioCoreSourceEnable(MIC_SOURCE_NUM);
    #endif
	
	//sink0
	mainAppCt.AudioCore->AudioSink[AUDIO_DAC0_SINK_NUM].Enable = 0;
	mainAppCt.AudioCore->AudioSink[AUDIO_DAC0_SINK_NUM].FuncDataSet = AudioDAC0DataSet;
	mainAppCt.AudioCore->AudioSink[AUDIO_DAC0_SINK_NUM].FuncDataSpaceLenSet = AudioDAC0DataSpaceLenGet;
	mainAppCt.AudioCore->AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink0Buf_DAC0; //sink audiocore ��buf 1 frame��
	AudioCoreSinkEnable(AUDIO_DAC0_SINK_NUM);
#ifdef CFG_RES_AUDIO_DACX_EN
	//���ı�ԭ���ṹ��sink2 ����DAC-X�����Ը���ʵ���������
	mainAppCt.AudioCore->AudioSink[AUDIO_DACX_SINK_NUM].Enable = 0;
	mainAppCt.AudioCore->AudioSink[AUDIO_DACX_SINK_NUM].FuncDataSet = AudioDAC1DataSet;
	mainAppCt.AudioCore->AudioSink[AUDIO_DACX_SINK_NUM].FuncDataSpaceLenSet = AudioDAC1DataSpaceLenGet;
	mainAppCt.AudioCore->AudioSink[AUDIO_DACX_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink2Buf_DACX; //sink audiocore ��buf 1 frame��
	AudioCoreSinkEnable(AUDIO_DACX_SINK_NUM);
#endif

#ifdef CFG_RES_AUDIO_I2SOUT_EN
	//���ı�ԭ���ṹ��sink2 ����I2S OUTs�����Ը���ʵ���������
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].Enable = 0;
#if (CFG_RES_I2S_PORT == 0)
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].FuncDataSet = AudioI2S0_DataSet;
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].FuncDataSpaceLenSet = AudioI2S0_DataSpaceLenGet;
#else
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].FuncDataSet = AudioI2S1_DataSet;
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].FuncDataSpaceLenSet = AudioI2S1_DataSpaceLenGet;
#endif
	mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink2Buf_I2S; //sink audiocore ��buf 1 frame��
	AudioCoreSinkEnable(AUDIO_I2SOUT_SINK_NUM);
#endif

	////Audio Core��������
	SystemVolSet();
	
	for( i = 0; i < AUDIO_CORE_SOURCE_MAX_MUN; i++)
	{
	   mainAppCt.AudioCore->AudioSource[i].PreGain = 4095;//Ĭ��ʹ��4095�� 0dB
	}

	AudioCoreServiceCreate(mainAppCt.msgHandle);
	mainAppCt.AudioCoreSync = FALSE;
	DeviceServiceCreate(mainAppCt.msgHandle);
	mainAppCt.DeviceSync = FALSE;
#ifdef CFG_FUNC_DISPLAY_EN
	DisplayServiceCreate(mainAppCt.msgHandle);
	mainAppCt.DisplaySync = FALSE;
#endif
#ifdef CFG_FUNC_REMIND_SOUND_EN
	RemindSoundServiceCreate(mainAppCt.msgHandle);
	mainAppCt.RemindSoundSync = FALSE;
#endif


#ifdef CFG_FUNC_ALARM_EN
	mainAppCt.AlarmFlag = FALSE;
#endif

#if (defined(CFG_APP_BT_MODE_EN) && defined(CFG_BT_BACKGROUND_RUN_EN))
	//���������񴴽������˴�,�Ա���������Э��ջʹ�õ��ڴ�ռ�,��Ӱ������������; ����˯��ʱ������stack�ٴο��������ϵ������˳���
	BtStackServiceStart();//�����豸����serivce ����ʧ��ʱ��Ŀǰ�ǹ�����ͬ����Ϣ��
//	IRKeyInit();//clkԴ���ģ�
#endif

	DBG("MainApp:run\n");


	while(1)
	{
		WDG_Feed();
		MessageRecv(mainAppCt.msgHandle, &msg, MAIN_APP_MSG_TIMEOUT);

#ifdef CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN
        extern uint32_t  Silence_Power_Off_Time;
        if(!Silence_Power_Off_Time)
		{
			Silence_Power_Off_Time = SILENCE_POWER_OFF_DELAY_TIME;
			DBG("power off\n");
			#ifdef	CFG_FUNC_POWERKEY_EN
			msg.msgId =  MSG_DEVICE_SERVICE_POWERDOWN;
			#endif

			#ifdef	CFG_FUNC_DEEPSLEEP_EN
			msg.msgId =  MSG_DEVICE_SERVICE_DEEPSLEEP;
			#endif
        }
#endif
		
		switch(msg.msgId)
		{
#ifdef CFG_FUNC_REMIND_SOUND_EN
			case MSG_REMIND_SOUND_SERVICE_CREATED:
#endif
#ifdef CFG_FUNC_DISPLAY_EN
			case MSG_DISPLAY_SERVICE_CREATED:
#endif
			case MSG_DEVICE_SERVICE_CREATED:
			case MSG_AUDIO_CORE_SERVICE_CREATED:	
				MainAppServiceCreating(msg.msgId);
				break;
			
#ifdef CFG_FUNC_REMIND_SOUND_EN
			case MSG_REMIND_SOUND_SERVICE_STARTED:
#endif
#ifdef CFG_FUNC_DISPLAY_EN
			case MSG_DISPLAY_SERVICE_STARTED:
#endif
			case MSG_DEVICE_SERVICE_STARTED:
			case MSG_AUDIO_CORE_SERVICE_STARTED:
				MainAppServiceStarting(msg.msgId);
				break;
#if FLASH_BOOT_EN
			case MSG_DEVICE_SERVICE_CARD_OUT:
				SoftFlagDeregister(SoftFlagMvaInCard);//����mva�����
				break;
			
			case MSG_DEVICE_SERVICE_DISK_OUT:
				SoftFlagDeregister(SoftFlagMvaInUDisk);//����mva�����
				break;
			
			case MSG_DEVICE_SERVICE_UPDATE:
				DBG("MainApp:UPDATE MSG\n");
				//�豸�������ţ�Ԥ����mva���Ǽǣ����������γ�ʱȡ���Ǽǡ�
				if(SoftFlagGet(SoftFlagMvaInCard) && GetSystemMode() == AppModeCardAudioPlay)
				{
					DBG("MainApp:updata file exist in Card\n");
					start_up_grate(AppResourceCard);
				}
				else if(SoftFlagGet(SoftFlagMvaInUDisk) && GetSystemMode() == AppModeUDiskAudioPlay)
				{
					DBG("MainApp:updata file exist in Udisk\n");
					start_up_grate(AppResourceUDisk);
				}
				break;
#endif

#ifdef	CFG_FUNC_POWERKEY_EN
			case MSG_DEVICE_SERVICE_POWERDOWN:
				DBG("MainApp:POWERDOWN\n");
				//��Ϣת������ǰAPP��APP�����ֳ�������Stopģʽ������Ϣ����main
				//SystemPowerDown();
				SystemGotoPowerDown();
				break;
#endif
#ifdef CFG_FUNC_DEEPSLEEP_EN
			case MSG_DEVICE_SERVICE_DEEPSLEEP:
				MainAppServicePause();
				break;
			case MSG_AUDIO_CORE_SERVICE_PAUSED:
			case MSG_DEVICE_SERVICE_PAUSED:
			case MSG_REMIND_SOUND_SERVICE_PAUSED:
			case MSG_DISPLAY_SERVICE_PAUSED:
				if(mainAppCt.state == TaskStatePausing)// ״̬����
				{
					MainAppServicePausing(msg.msgId);
				}
				break;
#endif

#ifdef CFG_SOFT_POWER_KEY_EN
			case MSG_DEVICE_SERVICE_SOFT_POWER:
				SoftKeyPowerOff();
				break;
#endif

#ifdef CFG_APP_BT_MODE_EN
			case MSG_BTSTACK_BB_ERROR:
				DBG("bb and bt stack reset\n");
				RF_PowerDownBySw();
				WDG_Feed();
				//reset bb and bt stack
				rwip_reset();
				BtStackServiceKill();
				WDG_Feed();
				vTaskDelay(50);
				RF_PowerDownByHw();
				WDG_Feed();
				//reset bb and bt stack
				BtStackServiceStart();
				break;

			//�������ӶϿ���Ϣ,������ʾ��
			case MSG_BT_STATE_CONNECTED:
				DBG("[BT_STATE]:BT Connected...\n");
				break;
			
			case MSG_BT_STATE_DISCONNECT:
				DBG("[BT_STATE]:BT Disconnected...\n");
				break;

#ifdef BT_AUTO_ENTER_PLAY_MODE
			case MSG_BT_A2DP_STREAMING:
				//���Ÿ���ʱ,��ģʽ�л�����,���ڴ���Ϣ�п�ʼ����ģʽ�л�����
				if((mainAppCt.appCurrentMode != AppModeBtAudioPlay)&&(mainAppCt.appCurrentMode != AppModeBtHfPlay))
				{
					MessageContext		msgSend;
					
					DBG("Enter Bt Audio Play Mode...\n");
					ResourceRegister(AppResourceBtPlay);
					
					// Send message to main app
					msgSend.msgId		= MSG_DEVICE_SERVICE_BTPLAY_IN;
					MessageSend(GetMainMessageHandle(), &msgSend);
				}
				break;
#endif
#endif
		}
		//����̬ �� deepsleepǰ���˳�����
		if(mainAppCt.state == TaskStateRunning ||(mainAppCt.state == TaskStatePausing && (mainAppCt.MState != ModeStatePause || mainAppCt.MState != ModeStateNone)))
		{
			if(msg.msgId == MSG_AUDIO_CORE_EFFECT_CHANGE)
			{
				DBG("msg.msgId == MSG_AUDIO_CORE_EFFECT_CHANGE\n");
				#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				//osMutexLock(AudioEffectChangeDoneMutex);
				#endif
				gCtrlVars.audio_effect_init_flag = 1;
				AudioEffectsDeInit();//�ͷ���Ч�ڴ�
				AudioEffectsInit();//���·�����Ч�ڴ棬��������������λ���ᵥ������ĳ����Ч
				if(GetAudioCoreServiceState() == TaskStatePaused)
				{
					AudioCoreServiceResume();
				}
				gCtrlVars.audio_effect_init_flag = 0;
				#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				//osMutexUnlock(AudioEffectChangeDoneMutex);
				#endif
			}
			//��̬�ڴ��С����
			if(msg.msgId == MSG_AUDIO_CORE_FRAME_SIZE_CHANGE)
			{
				DBG("msg.msgId == MSG_AUDIO_CORE_FRAME_SIZE_CHANGE\n");
				if(EffectProcessFlagGet() == TRUE)
			    {
			    	#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				    //osMutexLock(AudioEffectChangeDoneMutex);
					#endif
				    EffectProcessFlagSet(0);
					mainAppCt.SourcesMuteState = 0;
					
					for(i = 0; i<AUDIO_CORE_SOURCE_MAX_MUN; i++)
					{
						mainAppCt.SourcesMuteState <<= 1;
						DBG("mainAppCt.AudioCore->AudioSource[%d].LeftMuteFlag =%d\n", i, mainAppCt.AudioCore->AudioSource[i].LeftMuteFlag);
						if(mainAppCt.AudioCore->AudioSource[i].LeftMuteFlag == TRUE)
						{
							mainAppCt.SourcesMuteState |= 0x01;
						}
						//DBG("mainAppCt.SourcesMuteState = %x\n", mainAppCt.SourcesMuteState);
						AudioCoreSourceMute(i, 1, 1);
					}
					vTaskDelay(20);

					AudioDAC_DigitalMute(DAC0, TRUE, TRUE);
#ifdef CFG_RES_AUDIO_DACX_EN
					AudioDAC_DigitalMute(DAC1, TRUE, TRUE);
#endif
					vTaskDelay(10);
			    }
				if(GetAudioCoreServiceState() == TaskStatePaused)
				{
					MessageContext		msgSend;
					msgSend.msgId		= MSG_AUDIO_CORE_SERVICE_PAUSED;
					MessageSend(GetMainMessageHandle(), &msgSend);
				}
				else
				{
					AudioCoreServicePause();
				}
			}
			if(msg.msgId == MSG_AUDIO_CORE_SERVICE_PAUSED)
			{
				AudioResRelease();
				//SamplesFrameUpdata();//AudioCore Pause ס֮������л�ϵͳ��֡��С

#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				AudioEffectsDeInit();
#endif			
				{
					MessageContext		msgSend;
					if(GetAppMessageHandle() != NULL)
					{
						msgSend.msgId = MSG_APP_RES_RELEASE;
						MessageSend(GetAppMessageHandle(), &msgSend);
					}
				}
			}
			if(msg.msgId == MSG_APP_RES_RELEASE_SUC)
			{
				DBG("MSG_APP_RES_RELEASE_SUC\n");
				SamplesFrameUpdata();//AudioCore Pause ס֮������л�ϵͳ��֡��С
				if(!AudioResMalloc(mainAppCt.SamplesPreFrame))
				{
					DBG("Res Error!\n");
					return;
				}
				{
					MessageContext		msgSend;
					if(GetAppMessageHandle() != NULL)
					{
						msgSend.msgId = MSG_APP_RES_MALLOC;
						MessageSend(GetAppMessageHandle(), &msgSend);
					}
				}
			}
			if(msg.msgId == MSG_APP_RES_MALLOC_SUC)
			{
				//DBG("MSG_APP_RES_MALLOC_SUC\n");
				AudioResInit();
#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				gCtrlVars.audio_effect_init_flag = 1;
				AudioEffectsInit();
				gCtrlVars.audio_effect_init_flag = 0;
#endif				
				{
					MessageContext		msgSend;
					if(GetAppMessageHandle() != NULL)
					{
						msgSend.msgId = MSG_APP_RES_INIT;
						MessageSend(GetAppMessageHandle(), &msgSend);
					}
				}
			}
			if(msg.msgId == MSG_APP_RES_INIT_SUC)
			{
				//DBG("(msg.msgId == MSG_APP_RES_INIT_SUC)\n");
				AudioCoreDeinit();
				AudioCoreServiceResume();
				EffectProcessFlagSet(1);
				#ifdef CFG_FUNC_AUDIO_EFFECT_EN
				//osMutexUnlock(AudioEffectChangeDoneMutex);
				#endif
				AudioDAC_DigitalMute(DAC0, FALSE, FALSE);
#ifdef CFG_RES_AUDIO_DACX_EN
				AudioDAC_DigitalMute(DAC1, FALSE, FALSE);
#endif
				for(i = 0; i<AUDIO_CORE_SOURCE_MAX_MUN; i++)
				{
					if((mainAppCt.SourcesMuteState & 0x01) == 0x00)//sourcr ��ԭ������unmute״̬����ԭsource״̬
					{
						AudioCoreSourceUnmute(AUDIO_CORE_SOURCE_MAX_MUN - 1 - i, 1, 1);
						//DBG("i = %d, mainAppCt.SourcesMuteState = %x\n", i, mainAppCt.SourcesMuteState);
					}
					mainAppCt.SourcesMuteState >>= 1;
				}
			}
			
			MainAppModeProcess(msg.msgId);
		}
	}
}

/***************************************************************************************
 *
 * APIs
 *
 */

int32_t MainAppTaskStart(void)
{
	MainAppInit();
#ifdef CFG_FUNC_SHELL_EN
	shell_init();
	xTaskCreate(mv_shell_task, "SHELL", SHELL_TASK_STACK_SIZE, NULL, SHELL_TASK_PRIO, NULL);
#endif
	xTaskCreate(MainAppTaskEntrance, "MainApp", MAIN_APP_TASK_STACK_SIZE, NULL, MAIN_APP_TASK_PRIO, &mainAppCt.taskHandle);
	return 0;
}

MessageHandle GetMainMessageHandle(void)
{
	return mainAppCt.msgHandle;
}


uint32_t GetSystemMode(void)
{
	return mainAppCt.appCurrentMode;
}

void SoftFlagRegister(uint32_t SoftEvent)
{
	SoftwareFlag |= SoftEvent;
	return ;
}

void SoftFlagDeregister(uint32_t SoftEvent)
{
	SoftwareFlag &= ~SoftEvent;
	return ;
}

bool SoftFlagGet(uint32_t SoftEvent)
{
	return SoftwareFlag & SoftEvent ? TRUE : FALSE;
}

void SysVarInit(void)
{
	int16_t i;
#ifdef CFG_FUNC_BREAKPOINT_EN
	BP_SYS_INFO *pBpSysInfo;

	pBpSysInfo = (BP_SYS_INFO *)BP_GetInfo(BP_SYS_INFO_TYPE);

	if(pBpSysInfo->CurModuleId != AppModeUDiskPlayBack
		&& pBpSysInfo->CurModuleId != AppModeCardPlayBack
		&& pBpSysInfo->CurModuleId != AppModeFlashFsPlayBack)//����¼���ط�
	{
		mainAppCt.appBackupMode = pBpSysInfo->CurModuleId;//�ϵ����standby���ϵ����ģʽʹ��backup�ָ���
		if((mainAppCt.appBackupMode < AppModeStandbyPlay) || (mainAppCt.appBackupMode > AppModeFlashFsPlayBack))
		{
			mainAppCt.appBackupMode = AppModeBtAudioPlay;
		}
	}
	//mainAppCt.appBackupMode = AppModeBtAudioPlay;
	DBG("MainApp:%d,%d\n", mainAppCt.appCurrentMode, pBpSysInfo->CurModuleId);
    
	mainAppCt.MusicVolume = pBpSysInfo->MusicVolume;
	if((mainAppCt.MusicVolume > CFG_PARA_MAX_VOLUME_NUM) || (mainAppCt.MusicVolume <= 0))
	{
		mainAppCt.MusicVolume = CFG_PARA_MAX_VOLUME_NUM;
	}
	DBG("MusicVolume:%d,%d\n", mainAppCt.MusicVolume, pBpSysInfo->MusicVolume);	
	
	#ifdef CFG_FUNC_MIC_KARAOKE_EN
	mainAppCt.EffectMode = pBpSysInfo->EffectMode;
	if((mainAppCt.EffectMode > EFFECT_MODE_NvBianNan) || (mainAppCt.EffectMode <= 10))
	{
		mainAppCt.EffectMode = EFFECT_MODE_HunXiang;
	}
	#else
	mainAppCt.EffectMode = EFFECT_MODE_NORMAL;
	#endif
	DBG("EffectMode:%d,%d\n", mainAppCt.EffectMode, pBpSysInfo->EffectMode);
	
	mainAppCt.MicVolume = pBpSysInfo->MicVolume;
	if((mainAppCt.MicVolume > CFG_PARA_MAX_VOLUME_NUM) || (mainAppCt.MicVolume <= 0))
	{
		mainAppCt.MicVolume = CFG_PARA_MAX_VOLUME_NUM;
	}
	DBG("MicVolume:%d,%d\n", mainAppCt.MicVolume, pBpSysInfo->MicVolume);
	
#ifdef CFG_FUNC_EQ_MODE_EN
	mainAppCt.EqMode = pBpSysInfo->EqMode;
	if(mainAppCt.EqMode > EQ_MODE_VOCAL_BOOST)
	{
		mainAppCt.EqMode = EQ_MODE_FLAT;
	}
	DBG("EqMode:%d,%d\n", mainAppCt.EqMode, pBpSysInfo->EqMode);
#endif

	mainAppCt.ReverbStep = pBpSysInfo->ReverbStep;
    if((mainAppCt.ReverbStep > MAX_MIC_DIG_STEP) || (mainAppCt.ReverbStep <= 0))
	{
		mainAppCt.ReverbStep = MAX_MIC_DIG_STEP;
	}
	DBG("ReverbStep:%d,%d\n", mainAppCt.ReverbStep, pBpSysInfo->ReverbStep);
	
#ifdef CFG_FUNC_TREB_BASS_EN	
    mainAppCt.BassStep = pBpSysInfo->BassStep;
    if((mainAppCt.BassStep > MAX_MIC_DIG_STEP) || (mainAppCt.BassStep <= 0))
	{
		mainAppCt.BassStep = 7;
	}
    mainAppCt.TrebStep = pBpSysInfo->TrebStep;
    if((mainAppCt.TrebStep > MAX_MIC_DIG_STEP) || (mainAppCt.TrebStep <= 0))
	{
		mainAppCt.TrebStep = 7;
	}
	DBG("TrebStep:%d,%d\n", mainAppCt.TrebStep, pBpSysInfo->TrebStep);
	DBG("BassStep:%d,%d\n", mainAppCt.BassStep, pBpSysInfo->BassStep);
#endif

#else
	mainAppCt.appBackupMode = AppModeBtAudioPlay;		  
	mainAppCt.MusicVolume = CFG_PARA_MAX_VOLUME_NUM;
	
	#ifdef CFG_FUNC_MIC_EN
	mainAppCt.EffectMode = EFFECT_MODE_HunXiang;
	#else
	mainAppCt.EffectMode = EFFECT_MODE_NORMAL;
	#endif
	mainAppCt.MicVolume = CFG_PARA_MAX_VOLUME_NUM;
	
#ifdef CFG_FUNC_EQ_MODE_EN
	mainAppCt.EqMode = EQ_MODE_FLAT;
#endif
	mainAppCt.ReverbStep = MAX_MIC_DIG_STEP;

#ifdef CFG_FUNC_TREB_BASS_EN	
	mainAppCt.BassStep = 7;
	mainAppCt.TrebStep = 7;
#endif		
#endif

#ifdef CFG_APP_HDMIIN_MODE_EN
	mainAppCt.hdmiArcOnFlg = 1;
#endif
    for(i = 0; i < AUDIO_CORE_SINK_MAX_NUM; i++)
	{
		mainAppCt.gSysVol.AudioSinkVol[i] = CFG_PARA_MAX_VOLUME_NUM;
	}

	for(i = 0; i < AUDIO_CORE_SOURCE_MAX_MUN; i++)
	{
		if(i == 0)
		{
			mainAppCt.gSysVol.AudioSourceVol[i] = mainAppCt.MicVolume;
		}
		else if(i == 1)
		{
			mainAppCt.gSysVol.AudioSourceVol[i] = mainAppCt.MusicVolume;
		}
		else if(i == 2)
		{
			#if CFG_PARAM_FIXED_REMIND_VOL
			mainAppCt.gSysVol.AudioSourceVol[i] = CFG_PARAM_FIXED_REMIND_VOL;
			#else
			mainAppCt.gSysVol.AudioSourceVol[i] = mainAppCt.MusicVolume;
			#endif
		}
		else
		{
			mainAppCt.gSysVol.AudioSourceVol[i] = CFG_PARA_MAX_VOLUME_NUM;		
		}
	}
	mainAppCt.gSysVol.MuteFlag = FALSE;	
}

#ifdef	CFG_FUNC_POWERKEY_EN
void SystemGotoPowerDown(void)
{
	MessageContext		msgSend;

	msgSend.msgId = MSG_TASK_POWERDOWN;
	MessageSend(GetAppMessageHandle(), &msgSend);
}
#endif

#if defined(CFG_FUNC_DEEPSLEEP_EN) || defined(CFG_FUNC_MAIN_DEEPSLEEP_EN)
inline void SleepMainAppTask(void)
{
	AudioCoreSourceDisable(MIC_SOURCE_NUM);
//	OTG_PortSetDetectMode(0,0);//����usb/device ɨ������

#ifdef DISP_DEV_7_LED
	NVIC_DisableIRQ(Timer6_IRQn);
#endif
	NVIC_DisableIRQ(Timer2_IRQn);
	WDG_Disable();
//	OTG_PowerDown(); //usb device ���� �� ��ֹ��
	OTG_DeepSleepBackup();
	ADC_Disable();//����ɨ��
	AudioADC_PowerDown();
	AudioADC_VcomConfig(2);//ע�⣬VCOM���DAC�����ص�������2��PowerDown VCOM

	SPDIF_AnalogModuleDisable();//spdif,HDMI

	AudioDAC_PowerDown(DAC0);
#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_PowerDown(DAC1);
#endif
}


inline void WakeupMainAppTask(void)
{
	//��Ӧ VIC_DisableIRQ(Timer2_IRQn);
//	OTG_PortSetDetectMode(1,1);
	NVIC_EnableIRQ(Timer2_IRQn);
#ifdef DISP_DEV_7_LED
	NVIC_EnableIRQ(Timer6_IRQn);
#endif
//	OTG_DeviceInit();	//	OTG_PowerDown(); //usb device ����, pc������� ��

	//��Ӧ AudioDAC_PowerDown(DAC0);

#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_Init(ALL, mainAppCt.SampleRate, (void*)mainAppCt.DACFIFO, mainAppCt.SamplesPreFrame * 2 * 2 *2, (void*)mainAppCt.DACXFIFO, mainAppCt.SamplesPreFrame * 2 * 2);
#else
	AudioDAC_Init(DAC0, mainAppCt.SampleRate, (void*)mainAppCt.DACFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2, NULL, 0);
#endif

	//Mic1  analog ��ӦAudioADC_PowerDown(); ע��dacҪ���䡣
	AudioADC_AnaInit();
	AudioADC_VcomConfig(1);//MicBias en
	AudioADC_MicBias1Enable(1);

	AudioADC_PGASel(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1);
	AudioADC_PGASel(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2);

	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1, 15, 4);//0db bypass
	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2, 15, 4);

	//Mic1   digital
	AudioADC_DigitalInit(ADC1_MODULE, mainAppCt.SampleRate, (void*)mainAppCt.ADCFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2);
#if CFG_RES_MIC_SELECT
	AudioCoreSourceEnable(MIC_SOURCE_NUM);
#endif
//	SPDIF_AnalogModuleDisable();//spdif,HDMI
	
#ifdef CFG_RES_ADC_KEY_SCAN
	SarADC_Init();
	AdcKeyInit();
#endif
#ifdef	CFG_PARA_WAKEUP_SOURCE_IR
	IRKeyInit();
#endif
	OTG_WakeupResume();

}

#endif

//mic channel: 
void AudioADC1ParamsSet(uint32_t sampleRate, uint16_t gain, uint8_t gainBoostSel)
{
	AudioCoreSourceDisable(MIC_SOURCE_NUM);
	AudioADC_AnaInit();
	//AudioADC_VcomConfig(1);//MicBias en
	AudioADC_MicBias1Enable(1);
	AudioADC_PGASel(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1);
	AudioADC_PGASel(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2);
	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_LEFT, LINEIN3_LEFT_OR_MIC1, gain, gainBoostSel);
	AudioADC_PGAGainSet(ADC1_MODULE, CHANNEL_RIGHT, LINEIN3_RIGHT_OR_MIC2, gain, gainBoostSel);

	//Mic1	 digital
	memset(mainAppCt.ADCFIFO, 0, mainAppCt.SamplesPreFrame * 2 * 2 * 2);
	AudioADC_DigitalInit(ADC1_MODULE, sampleRate, (void*)mainAppCt.ADCFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2);
}

//��������HFͨ��ʱ,MIC�����ϴ�,�˳���״̬,�ָ���֮ǰ״̬
void ResumeAudioCoreMicSource(void)
{
	AudioADC1ParamsSet(mainAppCt.SampleRate, 15, 4);
	
#if CFG_RES_MIC_SELECT
	//Soure0.
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].Enable = 1;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].FuncDataGet = AudioADC1DataGet;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].FuncDataGetLen = AudioADC1DataLenGet;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].IsSreamData = TRUE;
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].PcmFormat = 2;//test mic audio effect
	mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].PcmInBuf = (int16_t *)mainAppCt.Source0Buf_ADC1;
	AudioCoreSourceEnable(MIC_SOURCE_NUM);
#endif

#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_SampleRateChange(ALL, CFG_PARA_SAMPLE_RATE);//�ָ�
#else
	AudioDAC_SampleRateChange(DAC0, CFG_PARA_SAMPLE_RATE);//�ָ�
#endif
}


void AudioDACSinkBufClear(void)
{
	uint16_t AudioCoreSinkBufLen = mainAppCt.SamplesPreFrame * 2 * 2;
	memset(mainAppCt.Sink0Buf_DAC0, 0, AudioCoreSinkBufLen);
#ifdef CFG_RES_AUDIO_DACX_EN
	memset(mainAppCt.Sink2Buf_DACX, 0, AudioCoreSinkBufLen);
#endif
}

//֡��С�л�����Ҫ�ͷ�������Ƶͨ·��ص��ڴ���Դ
//AudioCore��Ҫ��ͣ
//�������������Ƶͨ·����Դ
void AudioResRelease(void)
{
	uint32_t i;
    DBG("AudioResRelease\n");

	AudioDAC_Disable(DAC0);
#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_Disable(DAC1);
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	//I2S hold
#endif
	AudioADC_Disable(ADC1_MODULE);

	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_DAC0_TX);
#ifdef CFG_RES_AUDIO_DACX_EN
	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_DAC1_TX);
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	//I2S DMA Disable
#endif
	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_ADC1_RX);
	
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_ADC1_RX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_ADC1_RX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_ADC1_RX, DMA_ERROR_INT);
	

#ifdef CFG_RES_AUDIO_I2SOUT_EN
	//I2S DMA Disable
#endif
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC0_TX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC0_TX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC0_TX, DMA_ERROR_INT);

#ifdef CFG_RES_AUDIO_DACX_EN
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC1_TX, DMA_DONE_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC1_TX, DMA_THRESHOLD_INT);
	DMA_InterruptFlagClear(PERIPHERAL_ID_AUDIO_DAC1_TX, DMA_ERROR_INT);
#endif
	AudioDAC_Reset(DAC0);
#ifdef CFG_RES_AUDIO_DACX_EN
	AudioDAC_Reset(DAC1);
#endif

#ifdef CFG_RES_AUDIO_DACX_EN
	if(mainAppCt.Sink2Buf_DACX != NULL)
	{
		DBG("Sink2Buf_DACX\n");
		osPortFree(mainAppCt.Sink2Buf_DACX);
		mainAppCt.Sink2Buf_DACX = NULL;
	}
#endif
	if(mainAppCt.Sink0Buf_DAC0 != NULL)
	{
		DBG("Sink0Buf_DAC0\n");
		osPortFree(mainAppCt.Sink0Buf_DAC0);
		mainAppCt.Sink0Buf_DAC0 = NULL;
	}
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	//I2S Sink2Buf_I2S
	if(mainAppCt.Sink2Buf_I2S != NULL)
	{
		DBG("Sink2Buf_I2S\n");
		osPortFree(mainAppCt.Sink2Buf_I2S);
		mainAppCt.Sink2Buf_I2S = NULL;
	}
#endif
	if(mainAppCt.Source0Buf_ADC1 != NULL)
	{
		DBG("Source0Buf_ADC1\n");
		osPortFree(mainAppCt.Source0Buf_ADC1);
		mainAppCt.Source0Buf_ADC1 = NULL;
	}

	if(mainAppCt.ADCFIFO != NULL)
	{
		DBG("ADC1FIFO\n");
		osPortFree(mainAppCt.ADCFIFO);
		mainAppCt.ADCFIFO = NULL;
	}


#ifdef CFG_RES_AUDIO_I2SOUT_EN
	if(mainAppCt.I2SFIFO != NULL)
	{
		DBG("I2SFIFO\n");
		osPortFree(mainAppCt.I2SFIFO);
		mainAppCt.I2SFIFO = NULL;
	}
#endif
#ifdef CFG_RES_AUDIO_DACX_EN
	if(mainAppCt.DACXFIFO != NULL)
	{
		DBG("DACXFIFO\n");
		osPortFree(mainAppCt.DACXFIFO);
		mainAppCt.DACXFIFO = NULL;
	}
#endif
	if(mainAppCt.DACFIFO != NULL)
	{
		DBG("DACFIFO\n");
		osPortFree(mainAppCt.DACFIFO);
		mainAppCt.DACFIFO = NULL;
	}
	DBG("AudioResRelease ok\n");
	DBG(" AudioResRelease remain mem size: %d\n",(320 * 1024 - osPortRemainMem())/1024);
}

bool AudioResMalloc(uint16_t SampleLen)
{
	uint16_t FifoLenStereo = SampleLen * 2 * 2 * 2;//������8����С��֡������λbyte
	uint16_t FifoLenMono = SampleLen * 2 * 2;//������4����С��֡������λbyte
	uint16_t AudioCoreBufLen = SampleLen * 2 * 2;//AudioCore�ӿھ���������������
	DBG("AudioResMalloc\n");
	DBG("mainAppCt.SamplesPreFrame = %d\n", SampleLen);
	//////////����DMA fifo
	DBG("Audio Res Malloc from End\n");
	mainAppCt.DACFIFO = (uint32_t*)osPortMallocFromEnd(FifoLenStereo);//DAC fifo
	if(mainAppCt.DACFIFO != NULL)
	{
		memset(mainAppCt.DACFIFO, 0, FifoLenStereo);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}

#ifdef CFG_RES_AUDIO_DACX_EN
	mainAppCt.DACXFIFO = (uint32_t*)osPortMallocFromEnd(FifoLenMono);//DACX fifo
	if(mainAppCt.DACXFIFO != NULL)
	{
		memset(mainAppCt.DACXFIFO, 0, FifoLenMono);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	mainAppCt.I2SFIFO = (uint32_t*)osPortMallocFromEnd(FifoLenStereo);//I2S fifo
	if(mainAppCt.I2SFIFO != NULL)
	{
		memset(mainAppCt.I2SFIFO, 0, FifoLenStereo);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}
#endif

	mainAppCt.ADCFIFO = (uint32_t*)osPortMallocFromEnd(FifoLenStereo);//ADC fifo
	if(mainAppCt.ADCFIFO != NULL)
	{
		memset(mainAppCt.ADCFIFO, 0, FifoLenStereo);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}

///////////////////////////////AudioCore/////////////////////////////////////////
	//mic
	mainAppCt.Source0Buf_ADC1 = (uint16_t*)osPortMallocFromEnd(AudioCoreBufLen);//stereo
	if(mainAppCt.Source0Buf_ADC1 != NULL)
	{
		memset(mainAppCt.Source0Buf_ADC1, 0, AudioCoreBufLen);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}

	mainAppCt.Sink0Buf_DAC0 = (uint16_t*)osPortMallocFromEnd(AudioCoreBufLen);//stereo
	if(mainAppCt.Sink0Buf_DAC0 != NULL)
	{
		memset(mainAppCt.Sink0Buf_DAC0, 0, AudioCoreBufLen);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}

#ifdef CFG_RES_AUDIO_DACX_EN
	mainAppCt.Sink2Buf_DACX = (uint16_t*)osPortMallocFromEnd(AudioCoreBufLen);//mono
	if(mainAppCt.Sink2Buf_DACX != NULL)
	{
		memset(mainAppCt.Sink2Buf_DACX, 0, AudioCoreBufLen);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	mainAppCt.Sink2Buf_I2S = (uint16_t*)osPortMallocFromEnd(AudioCoreBufLen);//stereo
	if(mainAppCt.Sink2Buf_I2S != NULL)
	{
		memset(mainAppCt.Sink2Buf_I2S, 0, AudioCoreBufLen);
	}
	else
	{
		DBG("malloc error\n");
		return FALSE;
	}
#endif

	DBG("\n");
	return TRUE;
}

//��Ƶͨ·��Դ��ʼ��
void AudioResInit(void)
{
	DBG("AudioResInit\n");
	if(mainAppCt.Source0Buf_ADC1 != NULL)
	{
		mainAppCt.AudioCore->AudioSource[MIC_SOURCE_NUM].PcmInBuf = (int16_t *)mainAppCt.Source0Buf_ADC1;
	}

	if(mainAppCt.Sink0Buf_DAC0 != NULL)
	{
		mainAppCt.AudioCore->AudioSink[AUDIO_DAC0_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink0Buf_DAC0;
	}
#ifdef CFG_RES_AUDIO_DACX_EN
	if(mainAppCt.Sink2Buf_DACX != NULL)
	{
		mainAppCt.AudioCore->AudioSink[AUDIO_DACX_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink2Buf_DACX;
	}
#endif
#ifdef CFG_RES_AUDIO_I2SOUT_EN
	if(mainAppCt.Sink2Buf_I2S != NULL)
	{
		mainAppCt.AudioCore->AudioSink[AUDIO_I2SOUT_SINK_NUM].PcmOutBuf = (int16_t*)mainAppCt.Sink2Buf_I2S;
	}
#endif

	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_DAC0_TX);
	DMA_CircularConfig(PERIPHERAL_ID_AUDIO_DAC0_TX, mainAppCt.SamplesPreFrame * 2 * 2, (void*)mainAppCt.DACFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2);
	DMA_ChannelEnable(PERIPHERAL_ID_AUDIO_DAC0_TX);
	AudioDAC_Enable(DAC0);

#ifdef CFG_RES_AUDIO_DACX_EN
	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_DAC1_TX);
	DMA_CircularConfig(PERIPHERAL_ID_AUDIO_DAC1_TX, mainAppCt.SamplesPreFrame * 2, (void*)mainAppCt.DACXFIFO, mainAppCt.SamplesPreFrame * 2 * 2);
	DMA_ChannelEnable(PERIPHERAL_ID_AUDIO_DAC1_TX);
	AudioDAC_Enable(DAC1);
#endif

#ifdef CFG_RES_AUDIO_I2SOUT_EN
{
	I2SParamCt i2s_set;
	i2s_set.IsMasterMode=!CFG_RES_I2S_MODE;// 1:master 0:slave
	i2s_set.SampleRate=mainAppCt.SampleRate;
	i2s_set.I2sFormat=I2S_FORMAT_I2S;
	i2s_set.I2sBits=I2S_LENGTH_16BITS;
	i2s_set.I2sTxRxEnable=1;
#if (CFG_RES_I2S_PORT == 0)
	i2s_set.TxPeripheralID=PERIPHERAL_ID_I2S0_TX;
#else
	i2s_set.TxPeripheralID=PERIPHERAL_ID_I2S1_TX;
#endif
	i2s_set.TxBuf=(void*)mainAppCt.I2SFIFO;
	i2s_set.TxLen=mainAppCt.SamplesPreFrame * 2 * 2 * 2;
#if (CFG_RES_I2S_IO_PORT==0)
//i2s0  group_gpio0
	GPIO_PortAModeSet(GPIOA0, 9);// mclk out
	GPIO_PortAModeSet(GPIOA1, 6);// lrclk
	GPIO_PortAModeSet(GPIOA2, 5);// bclk
	GPIO_PortAModeSet(GPIOA3, 7);// dout
	GPIO_PortAModeSet(GPIOA4, 1);// din
//i2s0  group_gpio0
#else //lif (CFG_RES_I2S_IO_PORT==1)
//i2s1  group_gpio1 
	GPIO_PortAModeSet(GPIOA7, 5);//mclk out
	GPIO_PortAModeSet(GPIOA8, 1);//lrclk
	GPIO_PortAModeSet(GPIOA9, 2);//bclk
	GPIO_PortAModeSet(GPIOA10, 4);//do
	GPIO_PortAModeSet(GPIOA11, 2);//di
//i2s1  group_gpio1 
#endif
#if (CFG_RES_I2S_PORT == 0)
	AudioI2S_Init(I2S0_MODULE, &i2s_set);
#else
	AudioI2S_Init(I2S1_MODULE, &i2s_set);
#endif
}
#endif
	DMA_ChannelDisable(PERIPHERAL_ID_AUDIO_ADC1_RX);
	DMA_CircularConfig(PERIPHERAL_ID_AUDIO_ADC1_RX, mainAppCt.SamplesPreFrame * 2 * 2, (void*)mainAppCt.ADCFIFO, mainAppCt.SamplesPreFrame * 2 * 2 * 2);
	if(AudioADC_IsOverflow(ADC1_MODULE))
	{
		AudioADC_OverflowClear(ADC1_MODULE);
	}
	AudioADC_Clear(ADC1_MODULE);
    DMA_ChannelEnable(PERIPHERAL_ID_AUDIO_ADC1_RX);
    AudioADC_LREnable(ADC1_MODULE, 1, 1);
    AudioADC_Enable(ADC1_MODULE);
}

void SamplesFrameUpdataMsg(void)//����֡�仯��������Ϣ
{
	//if(gCtrlVars.SamplesPerFrame != mainAppCt.SamplesPreFrame)
	{
		MessageContext		msgSend;
		DBG("SamplesFrameUpdataMsg\n");
		//mainAppCt.SamplesPreFrame = gCtrlVars.SamplesPerFrame;

		msgSend.msgId		= MSG_AUDIO_CORE_FRAME_SIZE_CHANGE;
	    MessageSend(mainAppCt.msgHandle, &msgSend);
	}
}

void SamplesFrameUpdata(void)
{
	mainAppCt.SamplesPreFrame = gCtrlVars.SamplesPerFrame;
}

void EffectUpdataMsg(void)
{
	MessageContext		msgSend;
	DBG("EffectUpdataMsg\n");

	msgSend.msgId		= MSG_AUDIO_CORE_EFFECT_CHANGE;
	MessageSend(mainAppCt.msgHandle, &msgSend);
}
