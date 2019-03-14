#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "app_config.h"

#ifdef FUNC_WIFI_EN
//WiFiģ�鹤��״̬����
enum
{
    WIFI_STATUS_IDLE = 0, 
    WIFI_STATUS_SD_PLUGOUT,  //SD���γ�  
    WIFI_STATUS_SD_PLUGIN,  //SD������
    WIFI_STATUS_USB_PLUGOUT,  //USB�γ�
    WIFI_STATUS_USB_PLUGIN,  //USB����  	
    WIFI_STATUS_INITING,  //ģ�����ڳ�ʼ��    
    WIFI_STATUS_INIT_END,  //ģ���ʼ������        
    WIFI_STATUS_REBOOT_MCU,  //ģ��֪ͨMCU����  
    WIFI_STATUS_STATION_DISCONNECTED, //û����·����
    WIFI_STATUS_STATION_CONNECTING,  //��������·����
    WIFI_STATUS_STATION_CONNECTED,  //������·����
    WIFI_STATUS_AP_NO_CONNECTED,	//��������
    WIFI_STATUS_AP_CONNECTED,	//����������
    WIFI_STATUS_AP_DISCONNECTED,	//���豸�Ͽ����ӣ�����Ȼ�������豸������
    WIFI_STATUS_AP_HIDDEN,	//AP�Ѿ�����
    WIFI_STATUS_ETH_PLUGOUT,  //��̫��δ����    
    WIFI_STATUS_ETH_PLUGIN,  //��̫���Ѳ���
    WIFI_STATUS_ETH_PERMITED,  //��̫���Ѿ�����
    WIFI_STATUS_WWW_DISABLE,  //��������
    WIFI_STATUS_WWW_ENABLE,  //������    
    WIFI_STATUS_WPS_SCAN_STOP,  //δ��������״̬
    WIFI_STATUS_WPS_SCANNING,  //��������
    WIFI_STATUS_PLAY_PAUSE,	//��ͣ����
    WIFI_STATUS_PLAY_PLAYING, //���ڲ���
    WIFI_STATUS_LOGIN_SUCCESSFUL, //��¼�����̷������ɹ�
    WIFI_STATUS_LOGIN_FAILED, //��¼�����̷�����ʧ��
};

//MCU�˲���ģʽ����
enum
{
	MCU_PLAY_MODE_WIFI = 0,	//�л�WiFiģʽ�����Զ���ͷ�����ϴεĸ赥
	MCU_PLAY_MODE_USB = 4,	//U�̱��أ����U�̽���ģ���ϣ���ͬʱ�л���WiFiģʽ
	MCU_PLAY_MODE_AUX = 5,	//AUX��ĿǰApp��ʾΪline-in
	MCU_PLAY_MODE_BT = 6,	//Bluetooth
	MCU_PLAY_MODE_MIRROR = 7,	// Mirror, ����Mirror��������룬ͬʱ�л���WiFiģʽ
	MCU_PLAY_MODE_OPTICAL = 8,	//Optical
	MCU_PLAY_MODE_RCA = 9,	//RCA, APP��֧��
	MCU_PLAY_MODE_COAXIAL = 10,	//ͬ��, APP��֧��
	MCU_PLAY_MODE_SD = 14,	//T�����أ����T������ģ���ϣ���ͬʱ�л���WiFiģʽ��2016��İ汾֧�֣�
};

//WiFi�˲���ģʽ����
enum
{	
	WIFI_PLAY_MODE_IDLE = 0,
	WIFI_PLAY_MODE_AIRPLAY = 1,
	WIFI_PLAY_MODE_DLNA = 2,			//������DLNA	
	WIFI_PLAY_MODE_ALEXA = 3,			//Alexa��Ƶ	
	WIFI_PLAY_MODE_MENU_PLAY_WIFI = 10,	//�赥��ʽ����  010 ~ 019������ 011 ��ʾ�赥��ʽ����ģ��˵�U��
	WIFI_PLAY_MODE_MENU_PLAY_USB = 11,	//�赥��ʽ����  010 ~ 019������ 011 ��ʾ�赥��ʽ����ģ��˵�U��
	WIFI_PLAY_MODE_MENU_PLAY_SD = 16,	//�赥��ʽ����  010 ~ 019������ 016 ��ʾ�赥��ʽ����ģ��˵�U��
	WIFI_PLAY_MODE_API_PLAY = 20,		//������http api����    020 ~ 029�� ���� 021 ��ʾ������http api����ģ��˵�U��
	WIFI_PLAY_MODE_SPOTIFY_PLAY = 31,
	WIFI_PLAY_MODE_END_PLAY = 39,
	WIFI_PLAY_MODE_AUX = 40,
	WIFI_PLAY_MODE_BT = 41,
	WIFI_PLAY_MODE_EXT_USBTF = 42,	//�ⲿTF��USB����	
	WIFI_PLAY_MODE_OPTICAL = 43,
	WIFI_PLAY_MODE_RCA = 44,
	WIFI_PLAY_MODE_COAXIAL = 45,	//ͬ��, APP��֧��
	WIFI_PLAY_MODE_HIGHT_PLAY = 49,
	WIFI_PLAY_MODE_MIRROR = 50,
	WIFI_PLAY_MODE_SLAVE = 99,	//�����䲥��
	WIFI_PLAY_MODE_AVS = 0xff,				//����AVS������Ƶ	
};

//WiFi֧�ֵĲ����ظ�ģʽ����
enum 
{
	WIFI_PLAY_MODE_REPEAT_ALL = 0,    // ˳��ѭ������
	WIFI_PLAY_MODE_REPEAT_ONE,     // ����ѭ������
	WIFI_PLAY_MODE_REPEAT_SHUFFLE, // ���ѭ������
	WIFI_PLAY_MODE_ONCE_SHFFLE,	//�������һ��
	WIFI_PLAY_MODE_NO_REPEAT,	//��ѭ������
	WIFI_PLAY_MODE_TOTAL_NUM
};

typedef enum _WIFI_AVS_STATUS_
{
	WIFI_AVS_STATUS_IDLE = 0,
	WIFI_AVS_STATUS_LSN,
	WIFI_AVS_STATUS_THK,
	WIFI_AVS_STATUS_TTS,
	WIFI_AVS_STATUS_ERR,
	WIFI_AVS_STATUS_DIS,
	WIFI_AVS_STATUS_DIALOGUE_END,
	WIFI_AVS_STATUS_CAO,
	WIFI_AVS_STATUS_CAC,
	WIFI_AVS_STATUS_CAI,
	WIFI_AVS_STATUS_CAA,
	WIFI_AVS_STATUS_CAN,
	WIFI_AVS_STATUS_COMMS_END,
}WIFI_AVS_STATUS;

//�������ܹ��»���̨ģʽ
enum 
{
	WIFI_PLAY_KAISHU_RADIO_DEFAULT = 1,    	//�����̨
	WIFI_PLAY_KAISHU_RADIO_SLEEP,			//��˯ģʽ
	WIFI_PLAY_KAISHU_RADIO_TIPS,			//С֪ʶ
	WIFI_PLAY_KAISHU_RADIO_CUSTOM,			//�Զ���
	WIFI_PLAY_KAISHU_RADIO_FAVORITE,		//�ղؼ�
	WIFI_PLAY_KAISHU_RADIO_MAX
};

//WiFi����״̬��������
typedef struct _WIFI_WORK_STATE_
{	
	uint32_t InitState			:2;	//ģ���ʼ��״̬	
	uint32_t PlayState			:2;	//����״̬
	uint32_t GroupState			:2; //WPS����״̬	
	uint32_t FactoryState		:2; //�����ָ�״̬	
	uint32_t FirmwareState		:2; //WiFiģ��̼�����״̬	
	uint32_t TestModeState		:2; //����ģʽ״̬	
	uint32_t MutliRoomState		:2; //�෿��״̬
	uint32_t CardInsertState	:2; //������״̬
	uint32_t USBInsertState		:2; //USB����״̬
	uint32_t PowerState			:2; //WiFi�ϵ�״̬(000 loop all 001 single loop 002 shuffle loop 003 shuffle once Others: no loop)
	uint32_t RepeatMode			:4;	//WiFi�����ظ�ģʽ
	uint16_t WPSState; 				//WPS����״̬
	uint16_t StationState;			//·��������״̬(0-û������ 1-�Ѿ�����   2-�������� )
	uint16_t APState;				//APģʽ�ȵ㱻����״̬
	uint16_t ETHState; 				//��̫��״̬
	uint16_t WWWState; 				//��������״̬
	uint16_t WiFiAppPlayMode;		//WiFi֪ͨMCU����ģʽ
	uint16_t SlaveSoundBoxCnt;		//WiFi���������	
	uint8_t  IotSlaveDeviceID;		//IOT�豸ID��	
	uint16_t WiFiSoundRemindState;	//WiFi������ʾ״̬
	uint8_t  LoginState;			//��������¼״̬
	uint16_t LedState; 				//LED״̬(000 ��ʾ�ر�001 ��ʾ����002 ��ʾ����003 ��ʾ����)
	uint8_t  KaiShuRadio;			//������»���ǰ��̨
	uint8_t  KaiShuVolumeMax;		//������������;
	bool	 KaiShuChildLock;		//�����ͯ��״̬��
	WIFI_AVS_STATUS MicState; 		//MIC״̬��000 ���� 001 ¼����  002 ����ʶ����  003 �����ظ��� 004 MIC���� ��
	bool     VisInfState; 			//WiFi��Ϣ֪ͨ״̬
	bool	 WiFiNotifyChangeModeFlag;
	bool	 AlarmRemindState;		//WiFi���Ӳ���״̬
#ifdef FUNC_POWER_MONITOR_EN
#ifdef FUNC_WIFI_BATTERY_POWER_CHECK_EN	
	uint8_t BatPowerPercentBak;
	bool ChargeStatusFlag;
	uint8_t BatPowerPercent;
#endif
#endif
#ifdef FUNC_WIFI_POWER_KEEP_ON
	bool OtherModuleWiFiAudioEn;  //����ģʽ�´�WiFi���� 
	bool WiFiPowerOffRequestFlag;	
#endif
} WIFI_WORK_STATE;

// WiFi������WiFi��ز�����ʼ����ֵ���ɸ�����Ҫ�޸�
const static WIFI_WORK_STATE InitgWiFi =
{
	WIFI_STATUS_IDLE,            	    //ģ���ʼ��״̬	
	FALSE,  	                        //����״̬
	0,                                //����״̬	
	0,                                //�����ָ�״̬	
	0,                                //WiFiģ��̼�����״̬	
	0,                                //����ģʽ״̬	
	0,                                //�෿��״̬
	0,                                //������״̬
	0,                                //USB����״̬
	0,                                //WiFi�ϵ�״̬(000 loop all 001 single loop 002 shuffle loop 003 shuffle once Others: no loop)
	WIFI_PLAY_MODE_REPEAT_ALL,  	    //WiFi�����ظ�ģʽ
	WIFI_STATUS_WPS_SCAN_STOP,        //WPS����״̬
	WIFI_STATUS_STATION_DISCONNECTED,	//·��������״̬(0-û������ 1-�Ѿ�����   2-�������� )
	WIFI_STATUS_AP_NO_CONNECTED,    	//APģʽ�ȵ㱻����״̬
	WIFI_STATUS_ETH_PLUGOUT,          //��̫��״̬
	WIFI_STATUS_WWW_DISABLE,          //��������״̬
	MCU_PLAY_MODE_WIFI,         			//WiFi����ģʽ
	0,                            		//WiFi���������	
	1,                          	    //IOT�豸ID��	
	2,                              	//WiFi������ʾ״̬
	WIFI_STATUS_LOGIN_FAILED,
	0,                                //LED״̬(000 ��ʾ�ر�001 ��ʾ����002 ��ʾ����003 ��ʾ����)
	WIFI_PLAY_KAISHU_RADIO_DEFAULT,   //Ĭ�Ͽ����̨
	100,							  //����Ĭ������������
	FALSE,							  //�����ͯ��
	WIFI_AVS_STATUS_IDLE,             //MIC״̬��000 ���� 001 ¼����  002 ����ʶ����  003 �����ظ��� 004 MIC���� ��
	0,                                //WiFi��Ϣ֪ͨ״̬
	0,
	FALSE,							  //WiFi���Ӳ���״̬
#ifdef FUNC_POWER_MONITOR_EN
#ifdef FUNC_WIFI_BATTERY_POWER_CHECK_EN	
	0,
	FALSE,
	0,
#endif
#endif
#ifdef FUNC_WIFI_POWER_KEEP_ON
	0,                                //����ģʽ�´�WiFi���� 
	FALSE,
#endif
};

extern WIFI_WORK_STATE gWiFi;
extern uint8_t gCurNumberKeyNum;
extern uint16_t McuSoftSdkVerNum;	
extern TIMER WiFiSoundRemindTimer;

extern void WiFiUpgradeMcuSoftMsg(uint8_t* MsgData);
extern void WiFiUpgradeMcuSoftRunning(uint8_t* UpdateData);
extern bool WiFiPowerOnInitStateGet(void);
extern void WiFiWorkStateSet(uint8_t State);
extern void WiFiWpsStateSet(uint8_t State);
extern void WiFiStationStateSet(uint8_t State);
extern void WiFiEthStateSet(uint8_t State);
extern void WiFiWwwStateSet(uint8_t State);
extern uint8_t WiFiWwwStateGet(void);
extern void WiFiRa0StateSet(uint8_t State);
extern uint8_t WiFiRa0StateGet(void);
extern void WiFiFactoryStateSet(bool State);
extern bool WiFiFactoryStateGet(void);
extern void WiFiFirmwareUpgradeStateSet(uint8_t State);
extern uint8_t WiFiFirmwareUpgradeStateGet(void);
extern void WiFiLoginStateSet(uint8_t State);
extern uint8_t WiFiLoginStateGet(void);
extern void WiFiRequestMcuPowerOff(void);
extern void WiFiPowerStateSet(uint8_t* State);
extern void WiFiTestModeStateSet(void);
extern void WiFiSlaveSounBoxCntSet(uint16_t SoundBoxCnt);
extern uint8_t WiFiSlaveSounBoxCntGet(void);
extern void WiFiMutliRoomStateSet(bool State);
extern bool WiFiMutliRoomStateGet(void);
extern void WiFiUSBStateSet(uint8_t State);
extern bool IsWiFiUSBLink(void);
extern void WiFiCardStateSet(uint8_t State);
extern bool IsWiFiCardLink(void);
extern void WiFiSetMcuVolume(uint8_t Vol);
extern void WiFiGetMcuVolume(void);
extern void WiFiMuteStateSet(uint8_t State);
extern void WiFiGetMcuSoundRemindLanguage(void);
#ifdef FUNC_WIFI_POWER_KEEP_ON
extern void OtherModulePlayWiFiSoundRemind(bool State);
#endif
extern void WiFiSoundRemindStateSet(uint16_t State);
extern bool IsWiFiSoundRemindPlaying(void);
extern void McuSyncWiFiVolume(uint8_t Vol);
extern void McuGetWiFiPlayStateParams(void);
extern void WiFiGetProjectParams(void);
extern void WiFiControlGpioInit(void);
extern void WiFiAppSetPlayMode(uint8_t Cmd,uint8_t Mode);
extern void McuSetPlayMode(uint8_t ChangeMode);
extern void WiFiGetMcuCurPlayMode(void);
extern void WiFiSetRepeatMode(uint8_t RepeatMode);
extern void WiFiTalkStateSet(bool State);
extern void WiFiSetMcuSystemTime(uint8_t* DateData);
extern void WiFiSetMcuWeekDay(uint8_t* DayData);
extern void WiFiNoticeMcuNextAlarmTime(uint8_t* SecondData);
extern void WiFiSetMcuAlarmTime(uint8_t* AlarmTimeData);
extern void WiFiSetAlarmRemindState(bool State);
extern bool GetWiFiAlarmRemindState(void);
extern void MasterMcuRevPassThroughCmd(uint16_t Cmd);
extern void SlaveMcuRevPassThroughCmd(uint16_t Cmd);
extern void McuRevPassThroughData(uint8_t* RevData);
extern void McuSendPassThroughData(uint8_t* SendData, uint16_t DataLen);
extern void WiFiSetMicState(WIFI_AVS_STATUS State);
extern void WiFiStateCheck(void);
extern void WiFiSetMcuLedState(uint16_t State);
extern void WiFiPowerOnInitProcess(void);
extern uint32_t WiFiControl(void);
extern void WiFiKaiShuChildLockStateSet(bool State);
extern bool WiFiKaiShuChildLockStateGet(void);
extern void WiFiKaiShuVolumeMaxSet(uint8_t Vol);
extern uint8_t WiFiKaiShuVolumeMaxGet(void);
extern void WiFiKaiShuRadioSet(  uint8_t val);
extern void GpioRedWhiteLedControl(void);
#ifdef __cplusplus
}
#endif//__cplusplus

#endif

typedef uint8_t BtLinkedProfile;

//�������
extern uint32_t FlshUpdatePairedProfile(uint8_t *addr, BtLinkedProfile profile);
extern uint32_t FlshUpdatePairedName(uint8_t *addr, uint8_t *name);
extern uint8_t ClearPairedList(uint8_t *addr);
extern void BtTryToConnect(void);

#ifdef FUNC_WIFI_BT_CONTROL_EN
typedef enum _BT_CTRL_STATUS_
{
	BT_CTRL_STATE_IDLE = 0,
	BT_CTRL_STATE_SCAN,
	BT_CTRL_STATE_PAIR,
	BT_CTRL_STATE_UNPR,
	BT_CTRL_STATE_CTDI,
	BT_CTRL_STATE_CTPL,
	BT_CTRL_STATE_DSCT,
}BT_CTRL_STATUS;

typedef struct
{
	uint8_t 		index;			//1byte
	uint8_t         bdAddr[6];		//6bytes
	uint8_t			deviceName[60];	//60bytes
    BtLinkedProfile linkedProfile;	//1byte  //note:Ϊ�˽�ʡ��FLASH�ռ�Ĳ�д����,PROFILE��Ӧ�ı�������ݱ�־λ,0=��Ч,1=��Ч

}BT_PAIRED_LIST_INFO;  //68byts

typedef struct _WIFI_CTRL_BT_
{	
	BT_CTRL_STATUS State;
	uint8_t ScanBtDevice[67];
	uint32_t ScanEnable_Timer;						//��������ɷ���ʱ�䣬��λmS	
	bool    IsScanEnableDone;
	bool    Scan_Enable;
} WIFI_CTRL_BT;

#ifndef BT_DEVICE_NUM
#define BT_DEVICE_NUM				8 //֧�����8�������豸��Ϣ����
#endif
extern uint8_t BtDevName[50];
extern WIFI_CTRL_BT BtCtrl;
extern BT_PAIRED_LIST_INFO btPairedList[BT_DEVICE_NUM];

extern void WiFi_SendBtCmdToMcu(uint16_t WiFiCmd, uint8_t* CmdData);
extern void Mcu_SendBtCmdToWiFi(uint16_t McuCmd, uint8_t* DataBuf);
extern void WiFi_CtrlBtCmdProcess(uint8_t* gCtrlCmd);

#endif
#endif


