///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: line_in_control.c
///////////////////////////////////////////////////////////////////////////////
#include "app_config.h"
#include "breakpoint.h"
#include "i2s.h"
#include "audio_path.h"
#include "pcm_fifo.h"
#include "task_decoder.h"
#include "bt_app_func.h"
#include "bt_control_api.h"
#include "dev_detect_driver.h"
#include "sd_card.h"
#include "eq.h"
#include "eq_params.h"
#include "clk.h"
#include "fsinfo.h"
#include "sys_vol.h"
#include "sound_remind.h"
#include "mixer.h"
#include "dac.h"
#include "breakpoint.h"
#include "recorder_control.h"
#include "nvm.h"
#include "sys_app.h"
#include "browser.h"
#include "lrc.h"
#include "eq.h"
#include "eq_params.h"
#include "timer_api.h"
#include "string_convert.h"
#include "i2s_in_control.h"
#include "player_control.h"
#include "type.h"
#include "gpio.h"
#include "uart.h"
#include "watchdog.h"
#include "get_bootup_info.h"
#include "rtc_control.h"
#include "wifi_uart_com.h"
#include "wifi_control.h"
#include "wifi_init_setting.h"
#include "wifi_mcu_command.h"
#include "wifi_wifi_command.h"
#ifdef FUNC_DISP_EN
#include "led_display.h"
#include "seg_panel.h"
#include "seg_led_disp.h"
#endif

#ifdef	OPTION_CHARGER_DETECT
extern bool IsInCharge(void);
#endif

#ifdef FUNC_WIFI_EN
#ifdef FUNC_WIFI_POWER_KEEP_ON
#define WIFI_POWER_ON_INIT_TIME		400	//WiFiģ���ϵ��ĳ�ʼ������ʱ��(��λ: 10ms)
#else
#define WIFI_POWER_ON_INIT_TIME		60	//6s(WiFiģ���ϵ��ĳ�ʼ������ʱ��)
#endif
#define WIFI_LED_CB_TIME			50	 //����WiFiLedCb()��������ʱ����(��λ:ms)
#define WIFI_LED_SLOW_FLASH_TIME	1000 //����LED����˸ʱ��(��λ:ms)
#define WIFI_LED_FAST_FLASH_TIME	100 //����LED����˸ʱ��(��λ:ms)
#define WIFI_POWERON_RECONNECTION_TIME		600	//60s(WiFiģ���ϵ����Զ�����APʱ��)

uint16_t McuSoftSdkVerNum = 0;	//MCU����汾��

WIFI_WORK_STATE gWiFi;
TIMER WiFiCmdRespTimer;	//WiFiģ��������Ӧʱ��
TIMER WiFiPowerOffTimer;
TIMER WiFiLedBlinkTimer;
TIMER WiFiSoundRemindTimer;

static bool WiFiPowerOnInitEndFlag;
static uint16_t WiFiPowerOnInitTimeCnt;
uint8_t gCurNumberKeyNum = 0;
bool WiFiTalkOnFlag;
uint8_t McuCurPlayMode = 0;	//Mcu��ǰ����ģʽ
uint8_t WiFiLedFlashCnt; //WiFi LEDָʾ����˸����
uint16_t PassThroughDataLen = 0;  //͸�����ݳ���,����ֵ��Ϊ0��ʾ��͸�����ݽ���
uint8_t PassThroughDataState = 0; //͸������״̬

#ifdef FUNC_POWER_MONITOR_EN	
uint8_t GetCurBatterLevelAverage(void);
#endif

#ifdef	FUNC_WIFI_UART_UPGRADE
#define UPGRADE_CODE_BANK_ADDR	0x100000	//����������Spi Flash�д�ŵ���ʼ��ַ
#define UPGRADE_NVM_ADDR        (176) 	//boot upgrade information at NVRAM address

#define UPGRADE_DATA_BUF_LEN		1024

uint8_t Upgrade_Data_Buf[UPGRADE_DATA_BUF_LEN];
uint8_t UpgradeSoftProjectName[32];   //WiFi ���͵�MCU���������Ŀ��
uint16_t UpgradeSoftVerMsg;	//��������汾
uint32_t UpgradeSoftTotalSize;	//������������С
uint16_t SoftBankIndex;
uint32_t UpgradeSoftRecvLen;
static bool UpgradeDataStartEnFlag = FALSE;

static const uint16_t CrcCCITTTable[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

static uint16_t CRC16(const uint8_t* Buf, uint32_t BufLen, uint16_t CRC)
{
	uint32_t i;

	for (i = 0 ; i < BufLen ; i++) 
	{
		CRC = (CRC << 8) ^ CrcCCITTTable[((CRC >> 8) ^ *(uint8_t*)Buf ++) & 0x00FF];
	}
	return CRC;
}

bool DualBankDataSavenCRC(uint32_t DataLen)
{
	uint32_t Remine_Len, Recv_Len = 0;
	uint32_t Addr;
	uint16_t Crc = 0;

	//flash data crc check
	Remine_Len = DataLen;
	Addr = UPGRADE_CODE_BANK_ADDR;	

	while(Remine_Len > 0)
	{
		Recv_Len = (Remine_Len > UPGRADE_DATA_BUF_LEN)? UPGRADE_DATA_BUF_LEN : Remine_Len;
		
		SpiFlashRead(Addr, Upgrade_Data_Buf, Recv_Len); 
		if(Remine_Len <= UPGRADE_DATA_BUF_LEN)
		{
			Crc = CRC16((const uint8_t*)Upgrade_Data_Buf, Recv_Len - 4, Crc);
		}
		else
		{
			Crc = CRC16((const uint8_t*)Upgrade_Data_Buf, Recv_Len, Crc);
		}
		Addr += Recv_Len;
		Remine_Len -= Recv_Len;
	}	
	
	//compare the crc calculated with CRC in upgarde ball(last 4bytes)
	if(Crc != (*(uint32_t*)(UPGRADE_CODE_BANK_ADDR + UpgradeSoftTotalSize - 4)))
	{
		DBG("CRC error!\r\n");
		return FALSE;
	}
	
	DBG("Dual Bank Update data received successed, CRC16 OK!\r\n");	

	return TRUE;
}

void DualBankDataSaveFlash(uint16_t DataLen)
{
	uint32_t Addr;

	Addr = UPGRADE_CODE_BANK_ADDR + UpgradeSoftRecvLen - DataLen;
	
	SpiFlashWrite(Addr, Upgrade_Data_Buf, DataLen);
}

bool DualBankFlashErase(void)
{
#ifdef FUNC_WATCHDOG_EN
	WdgDis();				// disable watch dog
#endif
#ifdef FUNC_AMP_MUTE_EN
	WiFiSoundRemindStateSet(TRUE);
	gSys.MuteFlag = TRUE;
	AmpMuteControl(TRUE);
#endif

	DBG("DualBankFlashErase Start.........!\r\n");
	//erase dual bank update flash area
	if(SpiFlashErase(UPGRADE_CODE_BANK_ADDR, UPGRADE_CODE_BANK_ADDR) != FLASH_NONE_ERR)
	{
		DBG("Dual Bank Update: Flash erase FAILED!\r\n");
		return FALSE;
	}
    DBG("DualBankFlashErase OK.........!\r\n");
#ifdef FUNC_AMP_MUTE_EN
	gSys.MuteFlag = FALSE;
#endif
	return TRUE;
}

void DualBankDataUpgrade(uint32_t DataLen)
{	
	uint32_t BootNvmInfo;
	
	if(1)//DualBankDataSavenCRC(DataLen)) //���������ļ���CRCУ�鲻ͨ����ͬʱBOOT����ʱ����CRCУ�飬���Ӧ�ò�ȡ��
	{
#ifdef FUNC_AMP_MUTE_EN
		WiFiSoundRemindStateSet(TRUE);
		gSys.MuteFlag = TRUE;
		AmpMuteControl(TRUE);
#endif
#ifdef FUNC_BREAKPOINT_EN	
        {
            BP_SYS_INFO *pSysInfo;

            pSysInfo = (BP_SYS_INFO *)BP_GetInfo(BP_SYS_INFO_TYPE);
            gSys.UpgradeFileSource = UPGRADE_SOURCE_WIFI;
            BP_SET_ELEMENT(pSysInfo->UpgradeFileSource, gSys.UpgradeFileSource);
            BP_SaveInfo(BP_SAVE2NVM);
#ifdef BP_SAVE_TO_FLASH // �������
            BP_SaveInfo(BP_SAVE2FLASH);
#endif
        }
#endif
		APP_DBG("[UPGRADE]: dual bank flash prepare to boot upgrade\n");
		//write upgrade ball offset address to NVM(176), and then reset system
		BootNvmInfo = UPGRADE_CODE_BANK_ADDR;
		NvmWrite(UPGRADE_NVM_ADDR, (uint8_t*)&BootNvmInfo, 4);
	    //if you want PORRESET to reset GPIO only,uncomment it
        GpioPorSysReset(GPIO_RSTSRC_PORREST);
		NVIC_SystemReset();
		while(1);		
	}
}

//WiFi����MCU�����Ϣ
void WiFiUpgradeMcuSoftMsg(uint8_t* MsgData)
{
	static uint8_t MsgPos, RcvCnt, Temp;
	static bool SoftVerRcvStartFlag;
	static bool SoftSizeRcvStartFlag;
	static bool SoftNameRcvStartFlag;
	uint8_t i;

	if(WiFiDataRcvStartFlag == FALSE)
	{
		MsgPos = 0;
		RcvCnt = 0;
		Temp = 0;
		SoftVerRcvStartFlag = TRUE;
		SoftSizeRcvStartFlag = FALSE;
		SoftNameRcvStartFlag = FALSE;
		WiFiDataRcvStartFlag = TRUE;
		UpgradeSoftVerMsg = 0;
		UpgradeSoftTotalSize = 0;
		return;
	}

//����汾��Ϣ����
	if(SoftVerRcvStartFlag == TRUE)
	{	
		if(MsgData[Temp] == ':')
		{
			SoftVerRcvStartFlag = FALSE;
			SoftSizeRcvStartFlag = TRUE;
			RcvCnt = 0;	
			Temp += 1;
			MsgPos = Temp;
			return;
		}
		else
		{
			if(RcvCnt == 0)
			{
				UpgradeSoftVerMsg += (MsgData[Temp++] - 0x30) * 1000;
			}
			else if(RcvCnt == 1)
			{
				UpgradeSoftVerMsg += (MsgData[Temp++] - 0x30) * 100;
			}
			else if(RcvCnt == 2)
			{
				UpgradeSoftVerMsg += (MsgData[Temp++] - 0x30) * 10;
			}
			else if(RcvCnt == 3)
			{
				UpgradeSoftVerMsg += MsgData[Temp++] - 0x30;
			}
			RcvCnt++;
		}		
	}

//��������ļ���С����
	if(SoftSizeRcvStartFlag == TRUE)
	{			
		if(MsgData[Temp++] == ':')
		{			
			for(i = 0; i < RcvCnt; i++)
			{
				if(i == (RcvCnt - 1))
				{
					UpgradeSoftTotalSize += MsgData[MsgPos++] - 0x30;
				}
				else if(i == (RcvCnt - 2))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 10;
				}
				else if(i == (RcvCnt - 3))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 100;
				}
				else if(i == (RcvCnt - 4))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 1000;
				}
				else if(i == (RcvCnt - 5))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 10000;
				}
				else if(i == (RcvCnt - 6))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 100000;
				}
				else if(i == (RcvCnt - 7))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 1000000;
				}
				else if(i == (RcvCnt - 8))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 10000000;
				}
				else if(i == (RcvCnt - 9))
				{
					UpgradeSoftTotalSize += (MsgData[MsgPos++] - 0x30) * 100000000;
				}
			}	

			RcvCnt = 0;
			SoftSizeRcvStartFlag = FALSE;
			SoftNameRcvStartFlag = TRUE;	
			return;
		}
		else
		{
			RcvCnt++;
		}
	}
	
//��Ŀ��������
	if(SoftNameRcvStartFlag == TRUE)
	{
		if(MsgData[Temp] == '&')
		{
			RcvCnt = 0;
			SoftNameRcvStartFlag = FALSE;			
			//MCUͬ������
			UpgradeSoftRecvLen = 0;
			UpgradeDataStartEnFlag = FALSE;
			WiFiDataRcvStartFlag = FALSE;	
			WaitMs(500);
			RxDataEnableFlag = FALSE;
			WaitMs(2000);
			DualBankFlashErase();
#ifdef FUNC_WATCHDOG_EN
			WdgEn(WDG_STEP_4S);				
#endif
			WaitMs(100);
			ClearRxQueue();
			RxDataEnableFlag = TRUE;
			SlaveRxIndex = -3;		
			WaitMs(100);
			Mcu_SendCmdToWiFi(MCU_UPG_STA, NULL);	
		}
		else
		{
			UpgradeSoftProjectName[RcvCnt++] = MsgData[Temp++];
		}
	}
}

//WiFi��������MCU���
void WiFiUpgradeMcuSoftRunning(uint8_t* UpdateData)
{
	static uint16_t DataLen, RcvCnt, Temp;
	static uint16_t DataPos;
	static bool SoftBankIndexRcvStartFlag;
	static bool SoftBankLenRcvStartFlag;
	static bool SoftBankDataRcvStartFlag;
	static bool SoftBankUpgradeStartFlag;		
	static uint32_t RevCheckSum;
	static uint8_t CheckSumErrCnt;
	uint16_t i, j;	
	uint32_t* Pstr;
	uint32_t CheckSum;	
	
	if(WiFiDataRcvStartFlag == FALSE)
	{
		DataPos = 0;
		Temp = 0;
		RcvCnt = 0;
		DataLen = 0;
		RevCheckSum = 0;
		SoftBankIndex = 0;
		CheckSumErrCnt = 0;
		SoftBankIndexRcvStartFlag = TRUE;
		SoftBankLenRcvStartFlag = FALSE;
		SoftBankDataRcvStartFlag = FALSE;
		SoftBankUpgradeStartFlag = FALSE;
		WiFiDataRcvStartFlag = TRUE;
		return;
	}

//����������ݰ���
	if(SoftBankIndexRcvStartFlag == TRUE)
	{
		if(UpdateData[Temp++] == ':')
		{
			for(i = 0; i < RcvCnt; i++)
			{
				if(i == (RcvCnt - 1))
				{
					SoftBankIndex += UpdateData[DataPos++] - 0x30;
				}
				else if(i == (RcvCnt - 2))
				{
					SoftBankIndex += (UpdateData[DataPos++] - 0x30) * 10;
				}
				else if(i == (RcvCnt - 3))
				{
					SoftBankIndex += (UpdateData[DataPos++] - 0x30) * 100;
				}	
				else if(i == (RcvCnt - 4))
				{
					SoftBankIndex += (UpdateData[DataPos++] - 0x30) * 1000;
				}	
			}	
			SoftBankIndexRcvStartFlag = FALSE;
			SoftBankLenRcvStartFlag = TRUE;
			RcvCnt = 0;
			DataPos = Temp;
			return;
		}
		else
		{			
			RcvCnt++;
		}
	}

//����������ݰ�����	
	if(SoftBankLenRcvStartFlag == TRUE)
	{
		if(UpdateData[Temp++] == ':')
		{			
			for(i = 0; i < RcvCnt; i++)
			{
				if(i == (RcvCnt - 1))
				{
					DataLen += UpdateData[DataPos++] - 0x30;
				}
				else if(i == (RcvCnt - 2))
				{
					DataLen += (UpdateData[DataPos++] - 0x30) * 10;
				}
				else if(i == (RcvCnt - 3))
				{
					DataLen += (UpdateData[DataPos++] - 0x30) * 100;
				}	
				else if(i == (RcvCnt - 4))
				{
					DataLen += (UpdateData[DataPos++] - 0x30) * 1000;
				}	
			}	
			
			RcvCnt = 0;
			DataPos = Temp;			
			SoftBankLenRcvStartFlag = FALSE;
			SoftBankDataRcvStartFlag = TRUE;
			return;
		}
		else
		{
			RcvCnt++;
		}
	}

//�����������
	if(SoftBankDataRcvStartFlag == TRUE)
	{		
		if(UpdateData[Temp++] == '&')
		{
			//�������ֽڵ�Checksum
			Temp = 0;
			for(i = 0; i < 8; i++)
			{				
				if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '0'))
				{   
					Upgrade_Data_Buf[Temp++] = '0'; 
					i++; 
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '1'))
				{ 
					Upgrade_Data_Buf[Temp++] = 0; 
					i++;
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '2'))
				{ 
					Upgrade_Data_Buf[Temp++] = '&'; 
					i++; 
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '3'))
				{ 
					Upgrade_Data_Buf[Temp++] = '+'; 
					i++; 
				}
				else 
				{
					Upgrade_Data_Buf[Temp++] = UpdateData[i + DataPos];
				}					
				
				if(Temp >= 4)
				{
					break;
				}				
				
			}
			Pstr = (uint32_t*)&Upgrade_Data_Buf[0];
			RevCheckSum = *Pstr;
			Temp = i + 1;
			memset(Upgrade_Data_Buf, 0, sizeof(Upgrade_Data_Buf));
			j = 0;

			for(i = Temp; i < RcvCnt; i++)
			{				
				if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '0'))
				{   
					Upgrade_Data_Buf[j++] = '0'; 
					i++; 
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '1'))
				{ 
					Upgrade_Data_Buf[j++] = 0; 
					i++;
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '2'))
				{ 
					Upgrade_Data_Buf[j++] = '&'; 
					i++; 
				}
				else if((UpdateData[i + DataPos] == '0') && (UpdateData[i + DataPos + 1] == '3'))
				{ 
					Upgrade_Data_Buf[j++] = '+'; 
					i++; 
				}
				else 
				{
					Upgrade_Data_Buf[j++] = UpdateData[i + DataPos];
				}	
			}	
			
			SoftBankUpgradeStartFlag = TRUE;
			SoftBankDataRcvStartFlag = FALSE;
		}
		else
		{
			RcvCnt++;			
		}
	}
	
//���������������У���
	if(SoftBankUpgradeStartFlag == TRUE)
	{	
		CheckSum = 0;
		for(i = 0; i < DataLen; i+= 4)
		{
			Pstr = (uint32_t*)&Upgrade_Data_Buf[i];
			CheckSum += *Pstr;
		}

		if(RevCheckSum == CheckSum)
		{
			CheckSumErrCnt = 0;
			UpgradeSoftRecvLen += DataLen;
			DualBankDataSaveFlash(DataLen);
			if(UpgradeSoftRecvLen >= UpgradeSoftTotalSize)
			{
			  APP_DBG("UpgradeSoftRecvLen = %d, UpgradeSoftTotalSize = %d\n", UpgradeSoftRecvLen,UpgradeSoftTotalSize);	
				//�����Ƿ�����ж�
				//Flash bin�ļ�: ��ַ0x000000FC��ֵ��ͬ, δ������0xFFFFFFFF; ������0xFFFFFF55 
				//MVA�����ļ�:	��ַ0x00000112��ֵ��ͬ, δ������0xFFFFFFFF;  ������0xFFFFFF55	
				SpiFlashRead(0xFC, (uint8_t*)&CheckSum, 4); 
				SpiFlashRead(UPGRADE_CODE_BANK_ADDR + 0x112, (uint8_t*)&RevCheckSum, 4);
				if(CheckSum != RevCheckSum) //��ǰ�̼��������̼��Ƿ���ܲ�һ��
				{
					DBG("Upgrade Firmware error!\r\n");
					RcvCnt = 2; //���ܱ�ǲ�ƥ��
					Mcu_SendCmdToWiFi(MCU_UPG_FAL,(uint8_t *)&RcvCnt);	
				}
				else	
				{
					Mcu_SendCmdToWiFi(MCU_UPG_END, NULL);
					UpgradeDataStartEnFlag = TRUE;
				}
			}
			else
			{
				Mcu_SendCmdToWiFi(MCU_UPG_RUN, UpdateData);			
			}
		}
		else
		{
			CheckSumErrCnt++;
			if(CheckSumErrCnt > 5)	//һ�����ݰ��ط���������5����ֹ����
			{
				RcvCnt = 1; //checksum����ȷ
				Mcu_SendCmdToWiFi(MCU_UPG_FAL,(uint8_t *)&RcvCnt);	
			}
			else
			{
				if(SoftBankIndex)
				{
					SoftBankIndex--;
				}
				else
				{
					SoftBankIndex = 0x0FFF;
				}
				Mcu_SendCmdToWiFi(MCU_UPG_RUN, UpdateData);		
			}
		}
		
		SoftBankUpgradeStartFlag = FALSE;
		WiFiDataRcvStartFlag = FALSE;
	}
}
#endif

//��ȡWiFiģ���ϵ��ʼ��ʱ�����״̬
bool WiFiPowerOnInitStateGet(void)
{
	return WiFiPowerOnInitEndFlag; 
}

//WiFiģ�鹤��״̬����
void WiFiWorkStateSet(uint8_t State)
{
	switch(State)
	{
		case WIFI_STATUS_INITING:
			gWiFi.InitState = 0;
			break;
			
		case WIFI_STATUS_INIT_END:
			gWiFi.InitState = 1;
#ifdef	FUNC_WIFI_UART_UPGRADE
			if(UpgradeDataStartEnFlag == TRUE)
			{
				UpgradeDataStartEnFlag = FALSE;
				DualBankDataUpgrade(UpgradeSoftTotalSize);	
			}
#endif
			break;

		case WIFI_STATUS_REBOOT_MCU:
			WiFiFirmwareUpgradeStateSet(0);
#ifdef	FUNC_WIFI_UART_UPGRADE
			if(UpgradeDataStartEnFlag == TRUE)
			{
				UpgradeDataStartEnFlag = FALSE;
				DualBankDataUpgrade(UpgradeSoftTotalSize);	
			}
#endif
			break;			
			
		case WIFI_STATUS_PLAY_PAUSE:
			gWiFi.PlayState = 0;
			break;
			
		case WIFI_STATUS_PLAY_PLAYING:
			gWiFi.PlayState = 1;
			break;
	}
}

//WiFi ������»���ͯ������
void WiFiKaiShuChildLockStateSet(bool State)
{
	gWiFi.KaiShuChildLock = State;
}

//WiFi ��ȡ������»���ͯ��״̬
bool WiFiKaiShuChildLockStateGet(void)
{
	return gWiFi.KaiShuChildLock;
}

//WiFi ������»���ͯ������
void WiFiKaiShuVolumeMaxSet(  uint8_t       Vol)
{
	gWiFi.KaiShuVolumeMax = Vol;
	if(gWiFi.KaiShuVolumeMax < ((100 / MAX_VOLUME) * gSys.Volume))
	{
		gSys.Volume = (MAX_VOLUME * gWiFi.KaiShuVolumeMax) / 100;
		SetSysVol();
		Mcu_SendCmdToWiFi(MCU_CUR_VOL, &gWiFi.KaiShuVolumeMax);
	}
}

//WiFi ��ȡ������»���ͯ��״̬
uint8_t WiFiKaiShuVolumeMaxGet(void)
{
	return gWiFi.KaiShuVolumeMax;
}

//WiFi ������»���̨����
void WiFiKaiShuRadioSet(  uint8_t val)
{
	gWiFi.KaiShuRadio = val;
}

//WiFiģ��WPS����״̬����
void WiFiWpsStateSet(uint8_t State)
{
	gWiFi.WPSState = State;
}

//WiFiģ������·������״̬����
void WiFiStationStateSet(uint8_t State)
{
	gWiFi.StationState = State;
}

//WiFiģ����̫������״̬����
void WiFiEthStateSet(uint8_t State)
{
	gWiFi.ETHState = State;
}

//WiFiģ�黥����������״̬����
void WiFiWwwStateSet(uint8_t State)
{
	gWiFi.WWWState = State;
}

uint8_t WiFiWwwStateGet(void)
{
	return gWiFi.WWWState;
}

//WiFiģ��APģʽ�ȵ�ı�����״̬����
void WiFiRa0StateSet(uint8_t State)
{
	gWiFi.APState = State;
}

//WiFiģ��APģʽ�ȵ�ı�����״̬��ȡ
uint8_t WiFiRa0StateGet()
{
	return gWiFi.APState;
}


//WiFi�ָ���������״̬
void WiFiFactoryStateSet(bool State)
{
	gWiFi.FactoryState = State;
}

//��ȡWiFi�ָ���������״̬
bool WiFiFactoryStateGet(void)
{
	return gWiFi.FactoryState;
}

//WiFi�̼�����״̬����
void WiFiFirmwareUpgradeStateSet(uint8_t State)
{
	gWiFi.FirmwareState = State;
}

//��ȡWiFi�̼���������״̬
uint8_t WiFiFirmwareUpgradeStateGet(void)
{
	return gWiFi.FirmwareState;
}

//WiFiģ�����������״̬����
void WiFiLoginStateSet(uint8_t State)
{
	gWiFi.LoginState = State;
}

uint8_t WiFiLoginStateGet(void)
{
	return gWiFi.LoginState;
}

//WiFi����MCU �ػ�
void WiFiRequestMcuPowerOff(void)
{
	static uint8_t PowerOffCnt = 0;
	
	if(WiFiFirmwareUpgradeStateGet() != TRUE)
	{	
		//�ϵ���Ϣ����
		AudioSysInfoSetBreakPoint();
		PowerOffCnt++;        
        gWiFi.WiFiPowerOffRequestFlag = TRUE;
		if((PowerOffCnt == 2) || (gWiFi.InitState == WIFI_STATUS_IDLE))
		{
			APP_DBG("McuWillPowerOff.......\n");
			PowerOffCnt = FALSE;

		#ifdef USE_POWERKEY_SOFT_PUSH_BUTTON
			gSys.NextModuleID = MODULE_ID_POWEROFF;
		#else
			gSys.NextModuleID = MODULE_ID_IDLE;
		#endif
            if(GetCurBatterLevelAverage() <= 5)
            {
                gSys.NextModuleID = MODULE_ID_POWEROFF;
            }
			MsgSend(MSG_COMMON_CLOSE);
        #ifdef FUNC_WIFI_POWER_KEEP_ON
			WiFiPowerOff();
		#endif
		}
		else
		{
			APP_DBG("McuRequestWiFiPowerOff.......\n");
			PowerOffCnt = 1;
			TimeOutSet(&WiFiPowerOffTimer, 5000);	
			Mcu_SendCmdToWiFi(MCU_POW_OFF, NULL);
		}
	}
}

//WiFi ���ϵ�״̬����[000�� ����  001�� ʡ��ģʽ(RF�ر�)    002���̼������� 003���豸������]
void WiFiPowerStateSet(uint8_t* State)
{
	gWiFi.PowerState = State[2] - 0x30;
}

//WiFi ����ģʽ״̬����
void WiFiTestModeStateSet(void)
{
	gWiFi.TestModeState = 1;
}

//��ǰWiFiģ�����ӵ�������������
void WiFiSlaveSounBoxCntSet(uint16_t SoundBoxCnt)
{
#ifdef FUNC_WIFI_MULTIROOM_PLAY_EN
	if((gWiFi.SlaveSoundBoxCnt && (SoundBoxCnt == 0)) || ((gWiFi.SlaveSoundBoxCnt == 0) && SoundBoxCnt))
	{
		if((gSys.CurModuleID != MODULE_ID_WIFI) && (gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_SD) && (gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_USB))
		{
			gSys.NextModuleID = gSys.CurModuleID;
			MsgSend(MSG_MODE);
		}
	}
	
	gWiFi.SlaveSoundBoxCnt = SoundBoxCnt;
#endif
}

//��ȡ��ǰWiFiģ�����ӵ���������
uint8_t WiFiSlaveSounBoxCntGet(void)
{
	return gWiFi.SlaveSoundBoxCnt;
}

//WiFi �෿��������״̬����
void WiFiMutliRoomStateSet(bool State)
{
#ifdef FUNC_WIFI_MULTIROOM_PLAY_EN
	if(State && (gSys.CurModuleID != MODULE_ID_WIFI))
	{
		gSys.NextModuleID = MODULE_ID_WIFI;
		MsgSend(MSG_MODE);
	}
	gWiFi.MutliRoomState = State;
#endif
}

//��ȡ��ǰWiFi ģ���Ƿ���������״̬
bool WiFiMutliRoomStateGet(void)
{
	return gWiFi.MutliRoomState;
}

//WiFi ��USB����״̬����(����Ͱγ�����������������״̬)
void WiFiUSBStateSet(bool State)
{
	
	if(State == WIFI_STATUS_USB_PLUGIN)
	{
		if(gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_USB)
		{
			gSys.NextModuleID = MODULE_ID_PLAYER_WIFI_USB;	
			MsgSend(MSG_MODE);	//�γ�TF��WiFiģ�鲻�ᷢ���л�ģʽ�����ҪMCU�Զ��л�
		}
	}
	else if(State == WIFI_STATUS_USB_PLUGOUT)
	{	
		if(gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_USB)
		{
			MsgSend(MSG_MODE);	//�γ�USB�豸WiFiģ�鲻�ᷢ���л�ģʽ�����ҪMCU�Զ��л�
		}
	}
	
  gWiFi.USBInsertState = State;	
}

bool IsWiFiUSBLink(void)
{
	if(gWiFi.USBInsertState == WIFI_STATUS_USB_PLUGIN)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//WiFi ��TF������״̬����(����Ͱγ�����������������״̬)
void WiFiCardStateSet(bool State)
{
	static bool IsPowerCardState = FALSE;
	if(gWiFi.CardInsertState != State)
	{
		if(State == WIFI_STATUS_SD_PLUGIN)
		{
			if(IsPowerCardState && gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_SD)
			{
				gSys.NextModuleID = MODULE_ID_PLAYER_WIFI_SD;	
				MsgSend(MSG_MODE);	//�γ�TF��WiFiģ�鲻�ᷢ���л�ģʽ�����ҪMCU�Զ��л�
			}
		}
		else if(State == WIFI_STATUS_SD_PLUGOUT)
		{
			if(gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_SD)
			{
				MsgSend(MSG_MODE);	//�γ�TF��WiFiģ�鲻�ᷢ���л�ģʽ�����ҪMCU�Զ��л�
			}
		}
	}

	IsPowerCardState = TRUE;
	gWiFi.CardInsertState = State;	
}

bool IsWiFiCardLink(void)
{
	if(gWiFi.CardInsertState == WIFI_STATUS_SD_PLUGIN)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//WiFi �˵�������ʱͬ��MCU������
void WiFiSetMcuVolume(uint8_t Vol)
{
	if(Vol >= ((100 / MAX_VOLUME) * MAX_VOLUME))
	{
		Vol = MAX_VOLUME;
	}
	else
	{
		Vol = Vol / (100 / MAX_VOLUME);
	}
	gSys.Volume = Vol;
	SetSysVol();
}

//WiFi �˵�������ʱͬ��MCU������
void WiFiGetMcuVolume(void)
{
	uint8_t Temp;
	
	if(gSys.Volume >= MAX_VOLUME)
	{
		Temp = 100;						
	}
	else
	{
		Temp = gSys.Volume * (100 / MAX_VOLUME);
	}
	Mcu_SendCmdToWiFi(MCU_CUR_VOL, &Temp);
}

//WiFi �˹���Mute״̬����
void WiFiMuteStateSet(uint8_t State)
{
	if((gSys.CurModuleID == MODULE_ID_WIFI) || (gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_USB) 
	|| (gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_SD))
	{
		if(State)
		{					
//			DacSoftMuteSet(TRUE, TRUE);	
		}
		else
		{			
#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
#ifndef FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN		
			if(WiFiTalkOnFlag == TRUE)
			{
				gSys.MicEnable = FALSE;
				MixerMute(MIXER_SOURCE_MIC);
			}
#endif
#endif
			DacSoftMuteSet(FALSE, FALSE);		
		}		
		gSys.MuteFlag = State;
		
#ifdef FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN	
		DacVolumeSet(DAC_DIGITAL_VOL, DAC_DIGITAL_VOL);
#endif
	}
}

//WiFiģ���ѯ��ǰMCU ������ʾ����
void WiFiGetMcuSoundRemindLanguage(void)
{	
#if (MCU_CAP_LAU_ENGLISH == 1)
	gSys.LanguageMode = 0;
#elif (MCU_CAP_LAU_CHINESE == 1)
	gSys.LanguageMode = 1;
#elif (MCU_CAP_LAU_FRENCH == 1)
	gSys.LanguageMode = 6;
#elif (MCU_CAP_LAU_PORTUGUESE == 1)
	gSys.LanguageMode = 7;
#elif (MCU_CAP_LAU_ITALIAN == 1)
	gSys.LanguageMode = 8;
#elif (MCU_CAP_LAU_GERMANY == 1)
	gSys.LanguageMode = 4;
#elif (MCU_CAP_LAU_SPANISH == 1)
	gSys.LanguageMode = 5;
#endif

	Mcu_SendCmdToWiFi(MCU_LAU_XXX, &gSys.LanguageMode);
}

//MCU��ѯWiFi ��������ʾ����
void McuGetWiFiSoundRemindLanguage(void)
{
	Mcu_SendCmdToWiFi(MCU_LAU_GET, NULL);
}

//WiFi ��������ʾ��������[TRUE���̶�����������ʾ����FALSE�����ص�ǰϵͳ����]
void WiFiSoundRemindVolSet(bool WorkFlag)
{	
	extern const uint16_t gAnaVolArr[MAX_VOLUME + 1];
	
#ifdef 	WIFI_SOUND_REMIND_VOL	
	if(WorkFlag)
	{
		MixerConfigVolume(MIXER_SOURCE_ANA_MONO, gAnaVolArr[WIFI_SOUND_REMIND_VOL], gAnaVolArr[WIFI_SOUND_REMIND_VOL]);
		MixerConfigVolume(MIXER_SOURCE_ANA_STERO, gAnaVolArr[WIFI_SOUND_REMIND_VOL], gAnaVolArr[WIFI_SOUND_REMIND_VOL]);		
		APP_DBG("WiFiSoundRemindVolFixed = %d\n", WIFI_SOUND_REMIND_VOL);
	}
	else
	{		
		SetSysVol();
	}
#else
	if(gSys.Volume >= 7)
		return;
	
	if(WorkFlag)
	{
		MixerConfigVolume(MIXER_SOURCE_ANA_MONO, gAnaVolArr[7], gAnaVolArr[7]);
		MixerConfigVolume(MIXER_SOURCE_ANA_STERO, gAnaVolArr[7], gAnaVolArr[7]);
		APP_DBG(" WiFiSoundRemindVolSet = 7!\n");
	}
	else
	{
		SetSysVol();
	}
#endif
}

//����ģʽ�²���WiFi��ʾ��
#ifdef FUNC_WIFI_POWER_KEEP_ON
void OtherModulePlayWiFiSoundRemind(bool State)
{
	static bool AudioChannelState = FALSE;
	static uint8_t BtPlayState = PLAYER_STATE_IDLE;
	
	if(AudioChannelState != State)
	{
		if(gSys.CurModuleID != MODULE_ID_WIFI 
#ifdef FUNC_WIFI_PLAY_CARD_EN
		&& gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_SD
#endif
#ifdef FUNC_WIFI_PLAY_USB_EN
		&& gSys.CurModuleID != MODULE_ID_PLAYER_WIFI_USB
#endif
		&& (gWiFi.WiFiAppPlayMode != WIFI_PLAY_MODE_AVS))
		{
			AudioAnaSetChannel(AUDIO_CH_NONE);
#ifndef FUNC_MIXER_SRC_EN
			AudioSampleRateSet(44100);
#endif
			APP_DBG("OtherModulePlayWiFiSoundRemind Switch = %d\n", State);
			if(!State)
			{
				gWiFi.OtherModuleWiFiAudioEn = FALSE;
				switch(gSys.CurModuleID)
				{
					case MODULE_ID_LINEIN:
						AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC_LINEIN : AUDIO_CH_LINEIN);
						break;
				
					case MODULE_ID_RADIO:
						AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC_FM : AUDIO_CH_FM);
						break;
				
					case MODULE_ID_PLAYER_SD:  // enter into module player
					case MODULE_ID_PLAYER_USB: // enter into module player		
					case MODULE_ID_BLUETOOTH:  // BT shares the same entry func.
						AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC : AUDIO_CH_NONE);
						if((BtPlayState == PLAYER_STATE_PLAYING) && (gSys.CurModuleID == MODULE_ID_BLUETOOTH))
						{
							BTAudioPlayStart();
							BtPlayState = PLAYER_STATE_IDLE;
						}
						break;
				
					default:
						AudioAnaSetChannel(gSys.MicEnable ? AUDIO_CH_MIC : AUDIO_CH_NONE);	
						break;
				}
			}
			else
			{
				switch(gSys.CurModuleID)
				{	
					case MODULE_ID_PLAYER_SD:  // enter into module player
					case MODULE_ID_PLAYER_USB: // enter into module player					
					case MODULE_ID_BLUETOOTH:  // BT shares the same entry func.
						if((GetPlayState() == PLAYER_STATE_PLAYING) && (gSys.CurModuleID == MODULE_ID_BLUETOOTH))
						{
							BtPlayState = PLAYER_STATE_PLAYING;
							BTAudioPlayStop();
							WaitMs(500);
						}
						break;
				
					default:
						break;
				}
				gWiFi.OtherModuleWiFiAudioEn = TRUE;
				AudioAnaSetChannel(AUDIO_CH_I2SIN);
			}
		}
		else
		{
			gWiFi.OtherModuleWiFiAudioEn = FALSE;
		}
		
		AudioChannelState = State;
		WiFiSoundRemindVolSet(State);
  }
}
#endif

//WiFi ��������ʾ״̬����[000 ��ʼ, 001 ����. 002 ����, 003 ����MCU��������]
void WiFiSoundRemindStateSet(uint16_t State)
{
	gWiFi.WiFiSoundRemindState = State;
	if(!State && gSys.MuteFlag)
	{
		gSys.MuteFlag = FALSE;
		APP_DBG("WiFi SoundRemind UnMute........\n");
	}

	if(State)
	{
		TimeOutSet(&WiFiSoundRemindTimer, 1000);
	}
}

//WiFi����������״̬
bool IsWiFiSoundRemindPlaying(void)
{
	if(!WiFiPowerOnInitStateGet())
	{
		//�ȼ�鶨ʱ������Ϊ��һ�μ�飬����ʱ���Ƿ񵽣����᷵��FALSE
		IsTimeOut(&WiFiSoundRemindTimer);
		return FALSE;
	}
	else
	{
		return (!gWiFi.WiFiSoundRemindState || !IsTimeOut(&WiFiSoundRemindTimer));
	}
}

//MCU��ѯWiFi ��������ʾ״̬
void McuGetWiFiSoundRemindState(void)
{
	Mcu_SendCmdToWiFi(MCU_LAU_XXX, NULL);
}

//MCU�˵�������ʱͬ��WiFi������
void McuSyncWiFiVolume(uint8_t Vol)
{	
	uint8_t TempVol;
	
	if(Vol >= MAX_VOLUME)
	{
		TempVol = 100;						
	}
	else
	{
		TempVol = Vol * (100 / MAX_VOLUME);
	}
	Mcu_SendCmdToWiFi(MCU_CUR_VOL, &TempVol);
}

//ͨ��App���ò���ģʽ
void WiFiAppSetPlayMode(uint8_t Cmd,uint8_t Mode)
{
	uint8_t SetMcuPlayMode;

	SetMcuPlayMode = 0;

#ifdef FUNC_WIFI_BT_CONTROL_EN
	if((gWiFi.WiFiAppPlayMode == WIFI_PLAY_MODE_AVS) && GetCurBtConnectedFlag() && (Cmd != AXX_PLM_AVS)
	&& ((Mode < WIFI_PLAY_MODE_END_PLAY) || (Mode > WIFI_PLAY_MODE_HIGHT_PLAY)))
	{
		BTDisConnect();
		WaitMs(500);
	}
	
	gWiFi.WiFiAppPlayMode = (Cmd == AXX_PLM_AVS)? WIFI_PLAY_MODE_AVS : Mode;
#endif

	if(Mode == WIFI_PLAY_MODE_AUX)
	{
		SetMcuPlayMode = MODULE_ID_LINEIN;	
	}
	else if(Mode == WIFI_PLAY_MODE_BT)
	{
		SetMcuPlayMode = MODULE_ID_BLUETOOTH;	
	}
	else if(Mode == WIFI_PLAY_MODE_EXT_USBTF)
	{
		SetMcuPlayMode = MODULE_ID_PLAYER_SD;	
	}
	else if(Mode == WIFI_PLAY_MODE_MENU_PLAY_USB)
	{
		SetMcuPlayMode = MODULE_ID_PLAYER_WIFI_USB;	
	}	
	else if(Mode == WIFI_PLAY_MODE_MENU_PLAY_SD)
	{
		SetMcuPlayMode = MODULE_ID_PLAYER_WIFI_SD;	
	}	
	else if((Mode == WIFI_PLAY_MODE_MENU_PLAY_WIFI) || (Mode == WIFI_PLAY_MODE_SLAVE) 
		 || (Mode == WIFI_PLAY_MODE_SPOTIFY_PLAY) || (Mode == WIFI_PLAY_MODE_ALEXA)
		 || (Mode == WIFI_PLAY_MODE_AIRPLAY) || (Mode == WIFI_PLAY_MODE_DLNA))
	{
		SetMcuPlayMode = MODULE_ID_WIFI;
	}	
	
	if(SetMcuPlayMode && (gSys.CurModuleID != SetMcuPlayMode))
	{
		gSys.NextModuleID = SetMcuPlayMode;
		gWiFi.WiFiNotifyChangeModeFlag = TRUE;
		MsgSend(MSG_MODE);
	}
}

//MCU�л�����ģʽ
void McuSetPlayMode(uint8_t ChangeMode)
{
	uint16_t Cmd;
	uint8_t Mode;
	
	Cmd = 0;
	Mode = 0xFF;

	if(WiFiSlaveSounBoxCntGet())  //����MCU+PLM-xxx�������⣬��ʱȫ������MCU+PLM+xxx
	{
		Cmd = MCU_PLM_ADD;
	}
	else
	{
		Cmd = MCU_PLM_SUB;	
	}
	
	if(ChangeMode == MODULE_ID_LINEIN)
	{
		Mode = MCU_PLAY_MODE_AUX;		
	}
	else if((ChangeMode == MODULE_ID_BLUETOOTH) || (ChangeMode == MODULE_ID_BT_HF))
	{
		Mode = MCU_PLAY_MODE_BT;		
	}
	else if(ChangeMode == MODULE_ID_PLAYER_USB)
	{
		Mode = MCU_PLAY_MODE_USB;		
	}
	else if(ChangeMode == MODULE_ID_PLAYER_SD)
	{
		Mode = MCU_PLAY_MODE_SD;		
	}
#ifdef FUNC_WIFI_PLAY_CARD_EN
	else if(ChangeMode == MODULE_ID_PLAYER_WIFI_SD)
	{
		Mode = MCU_PLAY_MODE_SD;	
		Cmd = MCU_PLM_ADD;
	}
#endif
#ifdef FUNC_WIFI_PLAY_USB_EN
	else if(ChangeMode == MODULE_ID_PLAYER_WIFI_USB)
	{
		Mode = MCU_PLAY_MODE_USB;	
		Cmd = MCU_PLM_ADD;
	}
#endif	
	else if((ChangeMode == MODULE_ID_WIFI)
		|| (ChangeMode == MODULE_ID_RTC))
	{
		Mode = MCU_PLAY_MODE_WIFI;	
		Cmd = MCU_PLM_ADD;
	}

	if(Mode == 0xFF)	
	{
		return; //WiFi��APP��֧�ֵ�ģʽ
	}	
	
	McuCurPlayMode = Mode;
	if(gWiFi.WiFiNotifyChangeModeFlag == FALSE)
	{
		Mcu_SendCmdToWiFi(Cmd, &Mode);
		gWiFi.WiFiAppPlayMode = WIFI_PLAY_MODE_IDLE;
		WaitMs(200); //�л�̫������WiFi ģ���Զ��л�ģʽ����
	}
}

//WiFiģ���ѯ��ǰMCU ����ģʽ
void WiFiGetMcuCurPlayMode(void)
{
	uint16_t Cmd;
	
	Cmd = 0;	
	if(WiFiSlaveSounBoxCntGet())
	{
		Cmd = MCU_PLM_ADD;
	}
	else
	{
		Cmd = MCU_PLM_SUB;	
	}
	Mcu_SendCmdToWiFi(Cmd, &McuCurPlayMode);
}

//WiFģ�����ò����ظ�ģʽ
void WiFiSetRepeatMode(uint8_t RepeatMode)
{
	if(RepeatMode == 0x00)
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_REPEAT_ALL;
	}
	else if(RepeatMode == 0x01)
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_REPEAT_ONE;
	}
	else if(RepeatMode == 0x02)
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_REPEAT_SHUFFLE;
	}
	else if(RepeatMode == 0x03)
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_ONCE_SHFFLE;
	}
	else 
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_NO_REPEAT;
	}	
}

//MCU����WiFi�˵Ĳ����ظ�ģʽ
void McuSetWiFiRepeatMode(void)
{
	uint8_t RepMode;
	
	gWiFi.RepeatMode++;
	if(gWiFi.RepeatMode >=  WIFI_PLAY_MODE_TOTAL_NUM)
	{
		gWiFi.RepeatMode = WIFI_PLAY_MODE_REPEAT_ALL;
	}
	RepMode = gWiFi.RepeatMode;
	Mcu_SendCmdToWiFi(MCU_PLP_XXX, &RepMode);
}

//MCU��ȡWiFi�˵Ĳ����ظ�ģʽ
void McuGetWiFiRepeatMode(void)
{	
	Mcu_SendCmdToWiFi(MCU_PLP_GET, NULL);
	WaitMs(50);
}

//WiFi Talk״̬����
void WiFiTalkStateSet(bool State)
{
	WiFiTalkOnFlag = State;

#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
	if(State)
	{	
#ifdef FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN		
		//��ǰ��MIC
		DacDigitalMuteSet(TRUE, TRUE);	
		WaitMs(100);
		MixerMute(MIXER_SOURCE_ANA_STERO);	
		MixerMute(MIXER_SOURCE_MIC);		
		if(gSys.Volume > DEFAULT_VOLUME)
		{			
			DacVolumeSet(gDecVolArr[DEFAULT_VOLUME], gDecVolArr[DEFAULT_VOLUME]); 
		}
		else
		{
			DacVolumeSet(gDecVolArr[gSys.Volume], gDecVolArr[gSys.Volume]); 
		}	
		PcmFifoClear();
		PhubPathClose(ALL_PATH_CLOSE); 
		PhubPathSel(ADCIN2PMEM_IISIN2DACOUT_PCMFIFO2IISOUT);				
		MixerUnmute(MIXER_SOURCE_MIC);
		MixerUnmute(MIXER_SOURCE_ANA_STERO);
		gSys.MicEnable = TRUE;		
		DacDigitalMuteSet(FALSE, FALSE);
#else
		DacSoftMuteSet(TRUE, TRUE);	
		gSys.MicEnable = TRUE;
		MixerUnmute(MIXER_SOURCE_MIC);
#endif
	}
	else
	{		
		MixerMute(MIXER_SOURCE_MIC);
#ifdef FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN		
		DacSoftMuteSet(TRUE, TRUE);	
		PcmFifoClear();
		PhubPathClose(ALL_PATH_CLOSE);				
		PhubPathSel(ADCIN2PMEM_PCMFIFO2DACOUT);
#endif
		gSys.MicEnable = FALSE;
#ifdef FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN	
		DacVolumeSet(DAC_DIGITAL_VOL, DAC_DIGITAL_VOL);
#endif
		DacSoftMuteSet(FALSE, FALSE);	
	}
#endif
}

//MCU У׼ʱ����ģ���·���ʱ����UTCʱ�䣬��Ҫ����ʱ��У׼��
void McuChangeSystemTimeZone(RTC_DATE_TIME *SystemTime)
{
#ifdef FUNC_RTC_EN		
	#define TIMEZONE			(-7)

	int8_t TimeHour;
	uint8_t MonDate;
	
	TimeHour = (int8_t)SystemTime->Hour;
	TimeHour += (int8_t)TIMEZONE;

	if(TimeHour >= 24)
	{	
		SystemTime->Hour = (TimeHour%24);
		SystemTime->Date++;
		MonDate = RtcGetMonthDays(SystemTime->Year, SystemTime->Mon);
		if(SystemTime->Date > MonDate)
		{
			SystemTime->Date = (SystemTime->Date - MonDate);
			SystemTime->Mon++;
			if(SystemTime->Mon > 12)
			{
				SystemTime->Mon -= 12;
				SystemTime->Year++;
			}
		}
		ChangeSystemTimeZoneFlag = 1;
	}
	else if(TimeHour < 0)
	{
		SystemTime->Hour = (uint8_t)(24+TimeHour);
		SystemTime->Date--;
		if(SystemTime->Date == 0)
		{
			SystemTime->Mon--;
			if(SystemTime->Mon == 0)
			{
				SystemTime->Mon = 12;
				SystemTime->Year--;
			}
			SystemTime->Date = RtcGetMonthDays(SystemTime->Year, SystemTime->Mon);
		}
		ChangeSystemTimeZoneFlag = -1;
	}
	else
	{
		SystemTime->Hour = (TimeHour%24);
		ChangeSystemTimeZoneFlag = 0;
	}
#endif
}

//WiFi������MCU��ǰϵͳʱ��
void WiFiSetMcuSystemTime(uint8_t* DateData)
{
#ifdef FUNC_WIFI_SUPPORT_RTC_EN
#ifdef FUNC_RTC_EN		
	RTC_DATE_TIME SystemTime;
	static uint8_t RcvCnt;
	
	if(WiFiDataRcvStartFlag == FALSE)
	{
		RcvCnt = 0;
		WiFiDataRcvStartFlag = TRUE;
		return;
	}

	if(DateData[RcvCnt++] == '&')
	{
		APP_DBG("WiFiNoticeMcuSystemTime:%s\n", &DateData[0]);
		SystemTime.Year = (DateData[0] - 0x30) * 1000 + (DateData[1] - 0x30) * 100 + (DateData[2] - 0x30) * 10 + (DateData[3] - 0x30);
		SystemTime.Mon = (DateData[4] - 0x30) * 10 + (DateData[5] - 0x30);
		SystemTime.Date = (DateData[6] - 0x30) * 10 + (DateData[7] - 0x30);
		SystemTime.Hour = (DateData[8] - 0x30) * 10 + (DateData[9] - 0x30);
		SystemTime.Min = (DateData[10] - 0x30) * 10 + (DateData[11] - 0x30);
		SystemTime.Sec = (DateData[12] - 0x30) * 10 + (DateData[13] - 0x30);
		SystemTime.WDay = sRtcControl->DataTime.WDay;
		McuChangeSystemTimeZone(&SystemTime);
		RtcSetCurrTime(&SystemTime);
		WiFiDataRcvStartFlag = FALSE;
		DateData[RcvCnt] = '\0';
		APP_DBG("WiFiSetMcuSystemTime:(%04d-%02d-%02d %02d:%02d:%02d)\n",
		SystemTime.Year, SystemTime.Mon, SystemTime.Date, SystemTime.Hour, SystemTime.Min, SystemTime.Sec);
	}	
#endif
#endif
}

//WiFi������MCU��ǰ���ں�
void WiFiSetMcuWeekDay(uint8_t* DayData)
{
#ifdef FUNC_WIFI_SUPPORT_RTC_EN
#ifdef FUNC_RTC_EN		
	RTC_DATE_TIME SystemTime;
	static uint8_t RcvCnt;
	
	if(WiFiDataRcvStartFlag == FALSE)
	{
		RcvCnt = 0;
		WiFiDataRcvStartFlag = TRUE;
		return;
	}

	if(DayData[RcvCnt++] == '&')
	{		
		RtcGetCurrTime(&sRtcControl->DataTime);
		if(ChangeSystemTimeZoneFlag > 0)
		{
			SystemTime.WDay = ((DayData[0] - 0x30)+1);
			if(SystemTime.WDay > 6)
			{
				SystemTime.WDay = 0;	
			}
		}
		else if(ChangeSystemTimeZoneFlag < 0)
		{
			if((DayData[0] - 0x30) == 0)
			{
				SystemTime.WDay = 6;	
			}
			else
			{
				SystemTime.WDay = ((DayData[0] - 0x30)-1);
			}
		}
		else
		{
			SystemTime.WDay = DayData[0] - 0x30;
		
}
		ChangeSystemTimeZoneFlag = 0;
		SystemTime.Year = sRtcControl->DataTime.Year;
		SystemTime.Mon = sRtcControl->DataTime.Mon;
		SystemTime.Date = sRtcControl->DataTime.Date;
		SystemTime.Hour = sRtcControl->DataTime.Hour;
		SystemTime.Min = sRtcControl->DataTime.Min;
		SystemTime.Sec = sRtcControl->DataTime.Sec;
		RtcSetCurrTime(&SystemTime);
		WiFiDataRcvStartFlag = FALSE;
		DayData[RcvCnt] = '\0';
		APP_DBG("WiFiSetMcuWeekDay:%s\n", &DayData[0]);
	}	
#endif
#endif
}

//WiFiģ���֪MCU��һ�����ӵ�ʱ��
//����: SecondData ��ʾΪ�뵱ǰʱ���������
void WiFiNoticeMcuNextAlarmTime(uint8_t* SecondData)
{
#ifdef FUNC_WIFI_SUPPORT_ALARM_EN
#ifdef FUNC_WIFI_SUPPORT_RTC_EN
#ifdef FUNC_RTC_EN
	uint32_t TimeSecond;	
	//uint32_t  CurRtcTimeSecond;	
	static uint8_t RcvCnt;
	//RTC_DATE_TIME AlarmTime;
	
	if(WiFiDataRcvStartFlag == FALSE)
	{
		RcvCnt = 0;
		WiFiDataRcvStartFlag = TRUE;
		return;
	}
	
	if(SecondData[RcvCnt++] == '&')
	{	
		TimeSecond = 0;
		RcvCnt -= 1;
		
		switch(RcvCnt)
		{
			case 5:
				TimeSecond += SecondData[4] - 0x30;
				
			case 4:
				TimeSecond += (SecondData[3] - 0x30) * 10;

			case 3:
				TimeSecond += (SecondData[2] - 0x30) * 100;

			case 2:
				TimeSecond += (SecondData[1] - 0x30) * 1000;

			case 1:
				TimeSecond += (SecondData[0] - 0x30) * 10000;
				break;
		}
		
		WiFiDataRcvStartFlag = FALSE;
		sRtcControl->AlarmNum = 1;
		sRtcControl->AlarmMode = ALARM_MODE_PER_DAY;

		SecondData[RcvCnt] = '\0';
		APP_DBG("WiFiNoticeMcuNextAlarmTime:%s\n", &SecondData[0]);
		
		//���ӿ���״̬����
		if((SecondData[0] == '-') && (SecondData[1] == '1'))	
		{
			if(RtcGetAlarmStatus(1) != ALARM_STATUS_CLOSED)
			{
				//RtcAlarmSetStatus(1, ALARM_STATUS_CLOSED);	//�ر�����
			}
			return;
		}
		else
		{
			if(RtcGetAlarmStatus(1) != ALARM_STATUS_OPENED)
			{
				RtcAlarmSetStatus(1, ALARM_STATUS_OPENED); //������
			}
		}
#if 0		
		RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);

		AlarmTime.WDay = sRtcControl->AlarmTime.WDay;
		AlarmTime.Year = sRtcControl->AlarmTime.Year;
		AlarmTime.Mon = sRtcControl->AlarmTime.Mon;
		AlarmTime.Date = sRtcControl->AlarmTime.Date;

		CurRtcTimeSecond = (sRtcControl->DataTime.Hour * 3600) + (sRtcControl->DataTime.Min * 60) + sRtcControl->DataTime.Sec;
		//��������ʱ��
		AlarmTime.Hour = (TimeSecond  + CurRtcTimeSecond) / 3600;
		if(AlarmTime.Hour >= 24)
		{
			AlarmTime.Hour -= 24;
		}
		AlarmTime.Min = ((TimeSecond - (AlarmTime.Hour * 3600))  + (CurRtcTimeSecond - (sRtcControl->DataTime.Hour * 3600))) / 60 ;
		if(AlarmTime.Min >= 60)
		{
			AlarmTime.Hour += 1;
			AlarmTime.Min -= 60;
		}		
		AlarmTime.Sec = (TimeSecond - (AlarmTime.Hour * 3600) - (AlarmTime.Min * 60)) + sRtcControl->DataTime.Sec;
		if(AlarmTime.Sec >= 60)
		{
			AlarmTime.Min += 1;
			AlarmTime.Sec -= 60;
		}
		if(AlarmTime.Min >= 60)
		{
			AlarmTime.Hour += 1;
			AlarmTime.Min -= 60;
		}	
		if(AlarmTime.Hour >= 24)
		{
			AlarmTime.Hour -= 24;
		}
		RtcSetAlarmTime(sRtcControl->AlarmNum, sRtcControl->AlarmMode, sRtcControl->AlarmData, &AlarmTime);
		RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);
#endif

	}
#endif
#endif
#endif
}

//WiFiģ������MCU���ӵ�ʱ��
//����: AlarmTimeData ��ʾ����ʱ��
void WiFiSetMcuAlarmTime(uint8_t* AlarmTimeData)
{
#ifdef FUNC_WIFI_SUPPORT_RTC_EN
#ifdef FUNC_RTC_EN
#ifdef FUNC_WIFI_SUPPORT_ALARM_EN
	static uint8_t RcvCnt;
	RTC_DATE_TIME AlarmTime;
	
	if(WiFiDataRcvStartFlag == FALSE)
	{
		RcvCnt = 0;
		WiFiDataRcvStartFlag = TRUE;
		return;
	}
	
	if(AlarmTimeData[RcvCnt++] == '&')
	{
		APP_DBG("WiFiNoticeMcuNextAlarmTime:%s\n", &AlarmTimeData[0]);
		RcvCnt -= 1;		
		AlarmTime.Year = (AlarmTimeData[0] - 0x30) * 1000 + (AlarmTimeData[1] - 0x30) * 100 + (AlarmTimeData[2] - 0x30) * 10 + (AlarmTimeData[3] - 0x30);
		AlarmTime.Mon = (AlarmTimeData[4] - 0x30) * 10 + (AlarmTimeData[5] - 0x30);
		AlarmTime.Date= (AlarmTimeData[6] - 0x30) * 10 + (AlarmTimeData[7] - 0x30);
		AlarmTime.Hour= (AlarmTimeData[8] - 0x30) * 10 + (AlarmTimeData[9] - 0x30);
		AlarmTime.Min= (AlarmTimeData[10] - 0x30) * 10 + (AlarmTimeData[11] - 0x30);
		AlarmTime.Sec= (AlarmTimeData[12] - 0x30) * 10 + (AlarmTimeData[13] - 0x30);
		//Ϊ�˷���Ƚϣ���ʵ������
		AlarmTime.WDay = sRtcControl->DataTime.WDay;
		WiFiDataRcvStartFlag = FALSE;

		McuChangeSystemTimeZone(&AlarmTime);
		//ֹͣ�����������������
		if(sRtcControl->CurAlarmRun)
		{
			RtcCurAlarmSleepAndStop(RTC_ALARM_STATE_STOP);
		}

		//��ѯ�����������Ƿ�����ͬ�����ӣ������滻
		for(sRtcControl->AlarmNum = 1; sRtcControl->AlarmNum <= MAX_ALARM_NUM; sRtcControl->AlarmNum++)
		{
			RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);
			sRtcControl->AlarmTime.WDay = sRtcControl->DataTime.WDay;			//�������ݱȽϣ���ʵ������
			if(memcmp((void *)&AlarmTime, (void *)&sRtcControl->AlarmTime, sizeof(RTC_DATE_TIME)) == 0)
			{
				break;
			}
		}

		//��ѯ�����������Ƿ����Ѿ���ʱ�����ӣ����н����滻
		if(sRtcControl->AlarmNum > MAX_ALARM_NUM)
		{
			for(sRtcControl->AlarmNum = 1; sRtcControl->AlarmNum <= MAX_ALARM_NUM; sRtcControl->AlarmNum++)
			{
				RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);	
				sRtcControl->AlarmTime.WDay = sRtcControl->DataTime.WDay;			//�������ݱȽϣ���ʵ������
				if(memcmp((void *)&sRtcControl->AlarmTime, (void *)&sRtcControl->DataTime, sizeof(RTC_DATE_TIME)) <= 0)
				{
					break;
				}
			}
		}

		if(sRtcControl->AlarmNum > MAX_ALARM_NUM)
		{
			//��ѯ�����������뵱ǰʱ��Ƚϳ���һ�����������滻
			for(sRtcControl->AlarmNum = 1; sRtcControl->AlarmNum <= MAX_ALARM_NUM; sRtcControl->AlarmNum++)
			{
				uint16_t      TimeCmpBuf[MAX_ALARM_NUM+1];
				
				RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);	
				TimeCmpBuf[sRtcControl->AlarmNum] = 0;
				sRtcControl->AlarmTime.WDay = sRtcControl->DataTime.WDay;			//�������ݱȽϣ���ʵ������

				if(RtcGetAlarmStatus(sRtcControl->AlarmNum) != ALARM_STATUS_OPENED)
				{
					break;
				}
				
				//���õ������ڵ�ǰ����֮ǰ
				if(memcmp((void *)&AlarmTime, (void *)&sRtcControl->AlarmTime, sizeof(RTC_DATE_TIME)) < 0)
				{		
					if(sRtcControl->AlarmTime.Year > AlarmTime.Year)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 1<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Year-AlarmTime.Year);
					}
					else if(sRtcControl->AlarmTime.Mon > AlarmTime.Mon)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 2<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Mon-AlarmTime.Mon);
					}
					else if(sRtcControl->AlarmTime.Date > AlarmTime.Date)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 3<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Date-AlarmTime.Date);
					}
					else if(sRtcControl->AlarmTime.Hour > AlarmTime.Hour)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 4<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Hour-AlarmTime.Hour);
					}
					else if(sRtcControl->AlarmTime.Min > AlarmTime.Min)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 5<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Min-AlarmTime.Min);
					}
					else if(sRtcControl->AlarmTime.Sec > AlarmTime.Sec)
					{
						TimeCmpBuf[sRtcControl->AlarmNum] = 6<<8;
						TimeCmpBuf[sRtcControl->AlarmNum] += (sRtcControl->AlarmTime.Sec-AlarmTime.Sec);
					}
				}
				else
				{
					TimeCmpBuf[sRtcControl->AlarmNum] = 0xFFFF;
				}
				
				if(sRtcControl->AlarmNum == MAX_ALARM_NUM)
				{
					uint16_t Temp = 0;

					for(RcvCnt = 1; RcvCnt < MAX_ALARM_NUM; RcvCnt++)
					{
						if((TimeCmpBuf[RcvCnt]>>8) != (TimeCmpBuf[RcvCnt+1]>>8))
						{
							Temp = ((TimeCmpBuf[RcvCnt]>>8) < (TimeCmpBuf[RcvCnt+1]>>8))? RcvCnt: (RcvCnt+1);
						}
					}
					
					if(Temp == 0)
					{
						for(RcvCnt = 1; RcvCnt < MAX_ALARM_NUM; RcvCnt++)
						{
							if(TimeCmpBuf[RcvCnt] != TimeCmpBuf[RcvCnt+1])
							{
								Temp = (TimeCmpBuf[RcvCnt] > TimeCmpBuf[RcvCnt+1])? RcvCnt: (RcvCnt+1);
							}
						}
					}

					if(Temp == 0)
					{
						if(TimeCmpBuf[sRtcControl->AlarmNum] == 0xFFFF)
						{
							sRtcControl->AlarmNum = 0;	
						}
						else
						{
							sRtcControl->AlarmNum = 1;
						}
					}
					else
					{
						sRtcControl->AlarmNum = Temp;
					}
					break;
				}
			}
		}
		
		APP_DBG("WiFiSetMcuNextAlarmTime:(%04d-%02d-%02d %02d:%02d:%02d)\n",
		AlarmTime.Year, AlarmTime.Mon, AlarmTime.Date, AlarmTime.Hour, AlarmTime.Min, AlarmTime.Sec);
		
		if(sRtcControl->AlarmNum != 0)
		{
			sRtcControl->AlarmMode = ALARM_MODE_ONCE_ONLY;
			//��������
			RtcSetAlarmTime(sRtcControl->AlarmNum, sRtcControl->AlarmMode, sRtcControl->AlarmData, &AlarmTime);
			RtcGetAlarmTime(sRtcControl->AlarmNum, &sRtcControl->AlarmMode, &sRtcControl->AlarmData, &sRtcControl->AlarmTime);
			if(RtcGetAlarmStatus(sRtcControl->AlarmNum) != ALARM_STATUS_OPENED)
			{
				RtcAlarmSetStatus(sRtcControl->AlarmNum, ALARM_STATUS_OPENED); //������
			}
			//�洢�����������뱾���������֡�
			sNvmRtcInfo.RingType[sRtcControl->AlarmNum] = RTC_RING_WIFISD_TYPE;
#ifdef FUNC_RTC_ALARM_SAVE2FLASH
			{
				BP_RTC_INFO *BpRtcInfo;
				
				BpRtcInfo = (BP_RTC_INFO *)BP_GetInfo(BP_RTC_INFO_TYPE);
				BpRtcInfo->AlarmMode[sRtcControl->AlarmNum-1] = sRtcControl->AlarmMode;
				BpRtcInfo->RingType[sRtcControl->AlarmNum-1] = sNvmRtcInfo.RingType[sRtcControl->AlarmNum];
				BpRtcInfo->AlarmState[sRtcControl->AlarmNum-1] = ALARM_STATUS_OPENED;
				BpRtcInfo->AlarmTime[sRtcControl->AlarmNum-1] = AlarmTime;
				BP_SaveInfo(BP_SAVE2FLASH);
			}
#endif
		}
		else
		{
			sRtcControl->AlarmTime = AlarmTime;
		}

#ifdef FUNC_RTC_ALARM		
		NvmWrite(RTC_NVM_START_ADDR, (uint8_t *)&sNvmRtcInfo, sizeof(NVM_RTC_INFO));
#endif

#ifdef FUNC_DISP_EN
		DispSendMessage(SET_INTERVL, DISP_MSG_WIFI_SET_ALARM);
#endif
	}
#endif
#endif
#endif
}

//MCU��ȡWiFiģ����һ�����ӵ�ʱ��
void McuGetWiFiNextAlarmTime(void)
{
	Mcu_SendCmdToWiFi(MCU_ALM_NXT, NULL);
}
/*****************************************************************************
 �� �� ��  : WiFiSetAlarmRemindState
 ��������  : ����WiFi��ʾ��״̬
 �������  : bool State  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��1��28��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void WiFiSetAlarmRemindState(bool State)
{
	gWiFi.AlarmRemindState = State;
}

/*****************************************************************************
 �� �� ��  : GetWiFiAlarmRemindState
 ��������  : ��ȡWiFi��ʾ��״̬
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��1��28��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
bool GetWiFiAlarmRemindState(void)
{
	return gWiFi.AlarmRemindState;
}


//Master�豸ͨ��͸����ʽ����ͬһ���෿���MCU����ָ��
//����:Cmdֵ��Χ000 ~ 999
void MasterMcuSendPassThroughCmd(uint16_t Cmd)
{
	if(Cmd <= 999)
	{
		Mcu_SendCmdToWiFi(MCU_M2S_NNN, (uint8_t*)&Cmd);
	}
}

//Master�豸����ͨ��͸����ʽ���ݵ�ָ��
//����:Cmdֵ��Χ000 ~ 999
void MasterMcuRevPassThroughCmd(uint16_t Cmd)
{
//�û������ڴ˺����н������յ���ָ��
	switch(Cmd)
	{
		default:
			break;
	}
}

//Slave�豸ͨ��͸����ʽ����ͬһ���෿���MCU����ָ��
//����:Cmdֵ��Χ000 ~ 999
void SlaveMcuSendPassThroughCmd(uint16_t Cmd)
{
	if(Cmd <= 999)
	{
		Mcu_SendCmdToWiFi(MCU_S2M_NNN, (uint8_t*)&Cmd);
	}
}

//Slave�豸����ͨ��͸����ʽ���ݵ�ָ��
//����:Cmdֵ��Χ000 ~ 999
void SlaveMcuRevPassThroughCmd(uint16_t Cmd)
{
//�û������ڴ˺����н������յ���ָ��
	switch(Cmd)
	{
		default:
			break;
	}
}

//MCU���͵�ǰEq Treble ֵ��WiFi
void McuSendWiFiEqTrebleVal(uint8_t TrebleVal)
{	
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
	PassThroughDataLen = 0;
	memcpy((void*)&Upgrade_Data_Buf[PassThroughDataLen], (void*)"MCU+PAS+EQ:treble:", 18);
	PassThroughDataLen = 18;
	if(TrebleVal / 10)
	{
		Upgrade_Data_Buf[PassThroughDataLen++] = (TrebleVal / 10) + 0x30;
		Upgrade_Data_Buf[PassThroughDataLen++] = (TrebleVal % 10) + 0x30;				
	}
	else
	{
		Upgrade_Data_Buf[PassThroughDataLen++] = TrebleVal + 0x30;	
	}	
	Upgrade_Data_Buf[PassThroughDataLen++] = '&';
	McuSendPassThroughData(Upgrade_Data_Buf, PassThroughDataLen);
#endif
}

//MCU���͵�ǰEq Bass ֵ��WiFi
void McuSendWiFiEqBassVal(uint8_t BassVal)
{	
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
	PassThroughDataLen = 0;
	memcpy((void*)&Upgrade_Data_Buf[PassThroughDataLen], (void*)"MCU+PAS+EQ:bass:", 16);
	PassThroughDataLen = 16;
	if(BassVal / 10)
	{
		Upgrade_Data_Buf[PassThroughDataLen++] = (BassVal / 10) + 0x30;
		Upgrade_Data_Buf[PassThroughDataLen++] = (BassVal % 10) + 0x30;				
	}
	else
	{
		Upgrade_Data_Buf[PassThroughDataLen++] = BassVal + 0x30;	
	}	
	Upgrade_Data_Buf[PassThroughDataLen++] = '&';
	McuSendPassThroughData(Upgrade_Data_Buf, PassThroughDataLen);
#endif
}

//MCU����WiFi �˷��͵�͸������
void McuRevPassThroughData(uint8_t* RevData)
{
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
	static uint16_t RcvCnt;
	uint16_t Temp = 0;
	uint16_t AsciiIndex = 0;

	if(WiFiDataRcvStartFlag == FALSE)
	{
	    RcvCnt = 0;
		WiFiDataRcvStartFlag = TRUE;
		PassThroughDataLen = 0;
		PassThroughDataState = 1;
		memset(Upgrade_Data_Buf, 0, sizeof(Upgrade_Data_Buf));
		return;
	}

	if(RevData[RcvCnt++] == '&')
	{
		PassThroughDataLen = RcvCnt - 1;
		if(memcmp(RevData, "DAT", 3) == 0)
		{
			for(Temp = 3; Temp < (PassThroughDataLen-1);)
			{
				if(RevData[Temp] < 0x3a)							//����0-9
				{
					Upgrade_Data_Buf[AsciiIndex] = (RevData[Temp++]-0x30);
				}
				else if(RevData[Temp] < 0x47)					//��д�ַ�A-F
				{
					Upgrade_Data_Buf[AsciiIndex] = (RevData[Temp++]-0x41+10);
				}
				else												//Сд�ַ�a-f
				{
					Upgrade_Data_Buf[AsciiIndex] = (RevData[Temp++]-0x61+10);
				}

				Upgrade_Data_Buf[AsciiIndex] <<= 4;
				
				if(RevData[Temp] < 0x3a)							//����0-9
				{
					Upgrade_Data_Buf[AsciiIndex] += (RevData[Temp++]-0x30);
				}
				else if(RevData[Temp] < 0x47)					//��д�ַ�A-F
				{
					Upgrade_Data_Buf[AsciiIndex] += (RevData[Temp++]-0x41+10);
				}
				else												//Сд�ַ�a-f
				{
					Upgrade_Data_Buf[AsciiIndex] += (RevData[Temp++]-0x61+10);
				}

				AsciiIndex++;
			}
		}
		else if(memcmp(RevData, "IOT", 3) == 0)
		{
			Temp = 3;
			RcvCnt = 0;
			if(RevData[Temp] != (gWiFi.IotSlaveDeviceID+0x30))
				return;

			for(Temp = 4; Temp < PassThroughDataLen; Temp++)
			{
				Upgrade_Data_Buf[RcvCnt++] = RevData[Temp];
			}
		}
		else
		{
			for(Temp = 0; Temp < PassThroughDataLen; Temp++)
			{
				Upgrade_Data_Buf[Temp] = RevData[Temp];
			}
		}
#ifdef FUNC_LIGHT_PWM_LED_EN
#if defined(FUNC_WIFI_DUMI_PROJECT_EN)
		LightLedDisplayAvsCmdProcess(Upgrade_Data_Buf, AsciiIndex);
#elif defined(FUNC_WIFI_ALEXA_PROJECT_EN)
		WiFi_CtrlPwmLight(Upgrade_Data_Buf, TRUE);
#endif
#endif
		APP_DBG("PAS rcv Cmd = %s;\n", Upgrade_Data_Buf);
		WiFiDataRcvStartFlag = FALSE;
		PassThroughDataState = 0;
	}
#endif
}

//MCU����WiFi �˷��͵�͸������״̬
//����ֵ: 1--���ڽ�������   0--�ѽ������0xFF-- ����ʧ��
uint8_t McuRevPassThroughDataState(void)
{
	return PassThroughDataState;
}

//MCU�Խ��յ���͸�����ݽ�������
void McuRevPassThroughDataProcess(void)
{
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
	if(PassThroughDataLen && (McuRevPassThroughDataState() == 0))
	{
#ifdef FUNC_TREB_BASS_EN
		uint8_t Temp = 0xFF;
		if(memcmp(Upgrade_Data_Buf, "EQGet", 5) == 0)
		{
			McuSendWiFiEqTrebleVal(gSys.TrebVal);
			WaitMs(10);
			McuSendWiFiEqBassVal(gSys.BassVal);			
		}
		else if(memcmp(Upgrade_Data_Buf, "EQSet:treble:", 13) == 0)
		{
			if(Upgrade_Data_Buf[14])
			{
				Temp = (Upgrade_Data_Buf[13]- 0x30) * 10 + (Upgrade_Data_Buf[14]- 0x30);
			}
			else 
			{
				Temp = Upgrade_Data_Buf[13]- 0x30;
			}

			if(Temp < MAX_TREB_VAL)
			{
				gSys.TrebVal = Temp;
			}
			TrebBassSet(gSys.TrebVal, gSys.BassVal);
			gSys.EqStatus = 0;
		}
		else if(memcmp(Upgrade_Data_Buf, "EQSet:bass:", 11) == 0)
		{
			if(Upgrade_Data_Buf[12])
			{
				Temp = (Upgrade_Data_Buf[11]- 0x30) * 10 + (Upgrade_Data_Buf[12]- 0x30);
			}
			else
			{
				Temp = Upgrade_Data_Buf[11]- 0x30;
			}
			
			if(Temp < MAX_BASS_VAL)
			{
				gSys.BassVal= Temp;
			}
			
			TrebBassSet(gSys.TrebVal, gSys.BassVal);
			gSys.EqStatus = 0;
		}
#endif
		PassThroughDataLen = 0;
	}
#endif
}

//MCU����͸�����ݵ�WiFi ��
void McuSendPassThroughData(uint8_t* SendData, uint16_t DataLen)
{
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
	int32_t Temp = -1;
		
	PassThroughDataState = 1;
	if(DataLen <= 1024)
	{		
#ifdef WIFI_SELECT_BUART_COM
		Temp = BuartSend(SendData, DataLen);
#else
		Temp = FuartSend(SendData, DataLen);
#endif
	}

	if(Temp == DataLen)
	{
		PassThroughDataState = 0;
	}
	else
	{
		PassThroughDataState = 0xFF;
	}
#endif
}

//MCU����͸�����ݵ�WiFi ��
//����ֵ: 1--���ڷ�������   0--�ѷ������ 0xFF--����ʧ��
uint8_t McuSendPassThroughDataState(void)
{
	return PassThroughDataState;
}

//WiFi����״̬��ѯ
void WiFiStateCheck(void)
{
#define WIFI_STATE_CHECK_TIME	30000	//WiFi״̬��ѯʱ����(10s)
	
#ifdef FUNC_WIFI_POWER_KEEP_ON
	WiFiPowerOnInitProcess();          //Ϊ������WiFi���ڡ�
#endif	

	if(!gWiFi.InitState || WiFiFactoryStateGet() || WiFiFirmwareUpgradeStateGet())
	{
		return;
	}
	
	if(gWiFi.WiFiPowerOffRequestFlag && (IsTimeOut(&WiFiPowerOffTimer)) && (gSys.CurModuleID != MODULE_ID_IDLE))
	{
		WiFiRequestMcuPowerOff();	
	}		
	
#ifdef FUNC_POWER_MONITOR_EN
	if((WiFiWwwStateGet() != WIFI_STATUS_WWW_ENABLE) && (WiFiRa0StateGet() == WIFI_STATUS_AP_NO_CONNECTED))
	{
		return;
	}
	
#ifdef FUNC_WIFI_BATTERY_POWER_CHECK_EN
#ifdef OPTION_CHARGER_DETECT
	if(gWiFi.ChargeStatusFlag != IsInCharge())
	{
		gWiFi.ChargeStatusFlag = IsInCharge();
		if(gWiFi.ChargeStatusFlag)
		{
			Mcu_SendCmdToWiFi(MCU_BAT_ON, NULL);
		}
		else
		{
			Mcu_SendCmdToWiFi(MCU_BAT_OFF, NULL);
		}
		WaitMs(10);
	}
#endif
	
	if(IsTimeOut(&WiFiCmdRespTimer))
	{
		if(!(WiFiFactoryStateGet() || WiFiFirmwareUpgradeStateGet()))
		{
			
			gWiFi.BatPowerPercent = GetCurBatterLevelAverage();
			if(gWiFi.BatPowerPercentBak == 0)
			{
				gWiFi.BatPowerPercentBak = gWiFi.BatPowerPercent;
			}
			APP_DBG("Get Cur Batter Level Average = %d\n", gWiFi.BatPowerPercent);

			if(gWiFi.ChargeStatusFlag && (gWiFi.BatPowerPercentBak < gWiFi.BatPowerPercent)) 
			{
				gWiFi.BatPowerPercentBak++;
			}
			else if(!gWiFi.ChargeStatusFlag && (gWiFi.BatPowerPercentBak > gWiFi.BatPowerPercent)) 
			{
				gWiFi.BatPowerPercentBak--;
			}
			
			if(gWiFi.BatPowerPercentBak <= 1)
			{
				gWiFi.BatPowerPercentBak = 1;
				Mcu_SendCmdToWiFi(MCU_BAT_LOW, NULL);
			}
			WaitMs(10);
			Mcu_SendCmdToWiFi(MCU_BAT_VAL, &gWiFi.BatPowerPercentBak);
		}
		TimeOutSet(&WiFiCmdRespTimer, WIFI_STATE_CHECK_TIME);
	}
#endif
#endif
}

//WiFi������MCU�˿��Ƶ�LED ��ʾ״̬
void WiFiSetMcuLedState(uint16_t State)
{
	gWiFi.LedState = State;
}

//WiFi��MIC״̬
void WiFiSetMicState(WIFI_AVS_STATUS State)
{
	gWiFi.MicState = State;
}

//MCU��ȡWiFi��ǰ����״̬��ز���(ע:�����ڶϵ�����л�رմ����ж�,
//�ڷ����л�ģʽ�п��ܵ���WiFi���ʧ, �˺��������ڶϵ����������
//��ֹ����״̬����)
void McuGetWiFiPlayStateParams(void)
{
	Mcu_SendCmdToWiFi(MCU_MUT_GET, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_PLY_GET, NULL);
	WaitMs(10);	
}

//WiFi��ȡ��Ʒ��ز���������Ϣ
void WiFiGetProjectParams(void)
{	
	WiFiFactoryStateSet(0);
	WiFiFirmwareUpgradeStateSet(0);
		
	Mcu_SendCmdToWiFi(MCU_CAP_PRJ, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_PTV_XXX, NULL);
	WaitMs(10);
#ifdef FUNC_WIFI_ALI_PROJECT_EN
	Mcu_SendCmdToWiFi(MCU_ALI_PID, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_ALV_NUM, NULL);
	WaitMs(10);
#endif
#ifdef FUNC_WIFI_SPOTIFY_NEED_EN
	Mcu_SendCmdToWiFi(MCU_SPY_NAM, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SPY_BRN, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SPY_MOD, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SPY_BRD, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SPY_CID, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SPY_TYP, NULL);
	WaitMs(10);
#endif
#ifdef FUNC_LIGHT_PWM_LED_EN
#ifdef FUNC_WIFI_ALEXA_PROJECT_EN
	Mcu_SendCmdToWiFi(MCU_IOTN_LBC_NNN, NULL);
	WaitMs(10);
#endif
#endif
	Mcu_SendCmdToWiFi(MCU_CAP_001, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_CAP_002, NULL);
	WaitMs(10);	
	Mcu_SendCmdToWiFi(MCU_CAP_LAU, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_LAULIST, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_CAP_STM, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_CAP_PLM, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_CAP_EXT, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_PAR_XXX, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_PRESETN, NULL);
	WaitMs(10);
#ifdef FUNC_MCU_SET_WIFI_PASS_WORD_EN
	Mcu_SendCmdToWiFi(MCU_SET_PWD, NULL);
	WaitMs(10);
#endif
	Mcu_SendCmdToWiFi(MCU_VMX_XXX, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_FMT_XXX, NULL);
	WaitMs(10);
	Mcu_SendCmdToWiFi(MCU_SID_VAL, NULL);
	WaitMs(10);	
}

//WiFi���GPIO�ϵ��ʼ��(��ֹ©�絼��WiFiģ�鲻��������ʼ��)
void WiFiControlGpioInit(void)
{
#ifdef WIFI_SELECT_BUART_COM
	GpioBuartRxIoConfig(0xFF);	
	GpioBuartTxIoConfig(0xFF);	
#endif
#ifdef WIFI_SELECT_FUART_COM
	GpioFuartRxIoConfig(0xFF);	
	GpioFuartTxIoConfig(0xFF);	
#endif
	WiFiUartTxdOff();
	WiFiUartRxdOff();
	GpioI2sIoConfig(0xFF);    
	GpioMclkIoConfig(0xFF); 
	WiFiI2sLrckOff();
	WiFiI2sBclkOff();
	WiFiI2sDinOff();
	WiFiI2sDoutOff();
	WiFiI2sMclkOff();
	WiFiResetOff();	
	WiFiPowerOff();
	WiFiMicOn();
	TimeOutSet(&WiFiLedBlinkTimer, 0);
	TimeOutSet(&WiFiSoundRemindTimer, 0);
	
#ifdef FUNC_WIFI_EN
	memset(&gWiFi, 0, sizeof(gWiFi));
	memcpy((void*)&gWiFi, (void*)&InitgWiFi, sizeof(WIFI_WORK_STATE));
#ifdef FUNC_WIFI_BT_CONTROL_EN
	memset(&BtCtrl, 0, sizeof(BtCtrl));
#endif
#endif	
#ifdef FUNC_WIFI_POWER_KEEP_ON
	WiFiPowerOnInitTimeCnt = WIFI_POWER_ON_INIT_TIME;
	WiFiPowerOnInitEndFlag = FALSE;
#endif
}

//WiFiģʽ�µ�LED��ʾ����(100ms����һ�Σ���ʱ����WIFI_LED_CB_TIME �����û��Լ�����)
void WiFiLedCb(void* Param)
{	
#ifndef FUNC_WIFI_POWER_KEEP_ON
	if(WiFiPowerOnInitTimeCnt)
	{
		WiFiPowerOnInitTimeCnt--;
	}
#endif
}

//WiFi �ϵ�󲿷�Ӳ����ʼ������
void WiFiPowerOnInitProcess(void)
{
#ifdef FUNC_WIFI_POWER_KEEP_ON
	if(WiFiPowerOnInitEndFlag == FALSE)
	{
		WiFiPowerOnInitTimeCnt--;
		if(WiFiPowerOnInitTimeCnt < 1)
		{
			WiFi_UartInit();
			WiFiPowerOnInitEndFlag = TRUE;
			APP_DBG("wifi uart init.............\n");
		}
	}
#endif
}

uint32_t WiFiControl(void)
{
	uint16_t Msg = 0;
	SW_TIMER WiFiInTimer;
#ifndef FUNC_WIFI_POWER_KEEP_ON
	uint8_t WiFiInitStep;
#endif
	static bool FirstPowerOnFlag = TRUE;
	
#ifndef FUNC_MIXER_SRC_EN
	AudioSampleRateSet(44100);
#endif

	// ��Ϣע��
	if(gSys.MuteFlag)
	{
		gSys.MuteFlag = 0;
		AudioPlayerMute(gSys.MuteFlag);
	}
	
#ifdef FUNC_TREB_BASS_EN
	if(gSys.EqStatus == 0)
	{
		TrebBassSet(gSys.TrebVal, gSys.BassVal);
	}
#endif
	TimeOutSet(&WiFiCmdRespTimer, 0);
	TimeOutSet(&WiFiPowerOffTimer, 0);
	InitTimer(&WiFiInTimer, WIFI_LED_CB_TIME, WiFiLedCb);
	StartTimer(&WiFiInTimer);    

	if(McuSoftSdkVerNum == 0)
	{
		McuSoftSdkVerNum = GetSdkVer() >> 24;
	}

	if(gWiFi.WiFiNotifyChangeModeFlag == FALSE)
	{
		if(gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_USB)
	    {
		   SoundRemind(SOUND_UDISC_PLAYBACK); 
		   APP_DBG("enter MODULE_ID_PLAYER_WIFI_USB player\n");		
	    }
		else if(gSys.CurModuleID == MODULE_ID_PLAYER_WIFI_SD)
		{
		   SoundRemind(SOUND_CARD_PLAYBACK); 
		   APP_DBG("enter MODULE_ID_PLAYER_WIFI_SD player\n");
		}	
		else
		{
		   SoundRemind(SOUND_WIFI_MODE); 
		   APP_DBG("enter  WiFi player\n");
		}
	}

	gWiFi.WiFiNotifyChangeModeFlag = FALSE;
	
	AudioSysInfoSetBreakPoint();
	SetModeSwitchState(MODE_SWITCH_STATE_DONE);
	WiFiTalkOnFlag = FALSE;
	gWiFi.OtherModuleWiFiAudioEn = FALSE;
	
#ifdef FUNC_WIFI_POWER_KEEP_ON
	if(FirstPowerOnFlag == FALSE)
	{
		APP_DBG("AudioAnaSetChannel\n");	
#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
		AudioAnaSetChannel(AUDIO_CH_MIC_I2SIN);
#else
		AudioAnaSetChannel(AUDIO_CH_I2SIN); 
#endif
	}
#else
	gCurNumberKeyNum = 0;
	WiFiPowerOnInitEndFlag = FALSE;
	WiFiControlGpioInit();		
	WiFiInitStep = 2;
	WiFiPowerOnInitTimeCnt = 10; //WiFi power on delay time
#endif

	while(Msg != MSG_COMMON_CLOSE)
	{
		CheckTimer(&WiFiInTimer);
#ifdef FUNC_WIFI_POWER_KEEP_ON
		if((FirstPowerOnFlag == TRUE) && (WiFiPowerOnInitEndFlag == TRUE))
		{	
			APP_DBG("AudioAnaSetChannel init\n");					
#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
			AudioAnaSetChannel(AUDIO_CH_MIC_I2SIN); 
#else
			AudioAnaSetChannel(AUDIO_CH_I2SIN); 
#endif
			FirstPowerOnFlag = FALSE;
		}
#else
		if(WiFiPowerOnInitEndFlag == FALSE)
		{
			if(WiFiPowerOnInitTimeCnt < 1)
			{		
				if(WiFiInitStep == 2)
				{
					WiFiPowerOn(); 
					WiFiInitStep = 1;
					WiFiPowerOnInitTimeCnt = 5; //WiFi reset delay time
				}
				else if(WiFiInitStep == 1)
				{
					WiFiResetOn(); 
					WiFiInitStep = 0;
					WiFiPowerOnInitTimeCnt = WIFI_POWER_ON_INIT_TIME;
				}
				else
				{
					WiFi_UartInit();
					WiFiPowerOnInitProcess();
#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
					AudioAnaSetChannel(AUDIO_CH_MIC_I2SIN); 
#else
					AudioAnaSetChannel(AUDIO_CH_I2SIN);	
#endif
					WiFiPowerOnInitEndFlag = TRUE;
					WiFiPowerOnInitTimeCnt = WIFI_POWERON_RECONNECTION_TIME;
				}
			}
		}
#endif
		
#ifdef FUNC_WIFI_PASS_THROUGH_DATA_EN
		McuRevPassThroughDataProcess();
#endif
		
		if(gSys.OtherModuleTalkOn)
		{
			gSys.OtherModuleTalkOn = FALSE;
			Mcu_SendCmdToWiFi(MCU_TALK_ON, NULL);
		}

		Msg = MsgRecv(5);

		switch(Msg)
		{	
			case MSG_COMMON_CLOSE:  //WiFiģʽֻ���յ������Ϣ�Ż��˳�
			case MSG_MODE:        //working mode changing
				if(WiFiFirmwareUpgradeStateGet() == TRUE)
				{
					gSys.NextModuleID = MODULE_ID_UNKNOWN;
				  Msg = MSG_NONE;
				}
				else
				{
				  Msg = MSG_COMMON_CLOSE;
				}
				break;

			case MSG_POWER:       //Standy mode
				APP_DBG("[WIFI] MSG_POWER\n");
			#ifdef FUNC_WIFI_POWER_KEEP_ON
				WiFiRequestMcuPowerOff();
			#else
				gSys.NextModuleID = MODULE_ID_STANDBY;
				Msg = MSG_COMMON_CLOSE;
			#endif
				break;

			case MSG_PLAY_PAUSE:
				Mcu_SendCmdToWiFi(MCU_PLY_PAU, NULL);
				break;

			case MSG_NEXT:
				Mcu_SendCmdToWiFi(MCU_PLY_NXT, NULL);
				break;

			case MSG_PRE:
				Mcu_SendCmdToWiFi(MCU_PLY_PRV, NULL);
				break;	

			/*case MSG_REPEAT:
				McuSetWiFiRepeatMode();				
				break;*/

			case MSG_WIFI_NEXT_CH:	//������һ������Ԥ�ø赥	
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					Mcu_SendCmdToWiFi(MCU_KEY_NXT, NULL);
				}
				break;

			case MSG_WIFI_PREV_CH:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					Mcu_SendCmdToWiFi(MCU_KEY_PRE, NULL);
				}
				break;

			case MSG_WIFI_GROUP:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					Mcu_SendCmdToWiFi(MCU_JNGROUP, NULL);
				}
				break;

			case MSG_WIFI_UNGROUP:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					Mcu_SendCmdToWiFi(MCU_UNGROUP, NULL);
				}
				break;

			case MSG_REPEAT:
				break;

			case MSG_NUM_1:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					if((gWiFi.StationState == WIFI_STATUS_STATION_CONNECTED) || (gWiFi.WPSState == WIFI_STATUS_WPS_SCAN_STOP) 
					|| (gWiFi.WWWState == WIFI_STATUS_WWW_ENABLE))
					{		
						gCurNumberKeyNum = 1;	
						Mcu_SendCmdToWiFi(MCU_KEY_XNN, &gCurNumberKeyNum);
					}
				}
				break;

			case MSG_WIFI_PRESET1:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					if((gWiFi.StationState == WIFI_STATUS_STATION_CONNECTED) || (gWiFi.WPSState == WIFI_STATUS_WPS_SCAN_STOP) 
					|| (gWiFi.WWWState == WIFI_STATUS_WWW_ENABLE))
					{
						gCurNumberKeyNum = 1;
						Mcu_SendCmdToWiFi(MCU_PRE_NNN, &gCurNumberKeyNum);
					}
				}
				break;
				
			case MSG_WIFI_PRESET2:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					if((gWiFi.StationState == WIFI_STATUS_STATION_CONNECTED) || (gWiFi.WPSState == WIFI_STATUS_WPS_SCAN_STOP) 
					|| (gWiFi.WWWState == WIFI_STATUS_WWW_ENABLE))
					{
						gCurNumberKeyNum = 2;
						Mcu_SendCmdToWiFi(MCU_KEY_XNN, &gCurNumberKeyNum);
					}
				}
				break;

			case MSG_WIFI_SAVE:
				if(gSys.CurModuleID == MODULE_ID_WIFI)
				{
					Mcu_SendCmdToWiFi(MCU_PLY_LKE, NULL);
				}
				break;

			default:
				CommonMsgProccess(Msg);
				break;
		}
	}
	
#if (defined(FUNC_BREAKPOINT_EN) && (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN)))
	{
		BP_SYS_INFO *pBpSysInfo;
		pBpSysInfo = (BP_SYS_INFO *)BP_GetInfo(BP_SYS_INFO_TYPE);
		BP_SET_ELEMENT(pBpSysInfo->Volume, gSys.Volume);
		BP_SET_ELEMENT(pBpSysInfo->Eq, gSys.Eq);
		BP_SaveInfo(BP_SAVE2NVM);
	}
#endif

#ifndef FUNC_OUTPUT_CHANNEL_AUTO_CHANGE
#if (!defined (OUTPUT_CHANNEL_CLASSD)) && (!defined (OUTPUT_CHANNEL_DAC_CLASSD))//usb ģʽ��΢��
	DacSampRateAdjust(FALSE, 0);
#endif
#endif
#ifdef FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN
	gSys.MicEnable = FALSE;
#endif
#ifndef FUNC_WIFI_POWER_KEEP_ON
	WiFiPowerOff();
	WiFiControlGpioInit();	
	WaitMs(200);          //�ʵ���ʱ����ֹWiFi��Դ������������
#endif
	
#ifdef FUNC_BT_HF_EN
	GpioPcmSyncIoConfig(1);
#endif

	AudioAnaSetChannel(AUDIO_CH_NONE);	
	APP_DBG("leave WiFiControl...\n");
	return TRUE;           
}

#endif

