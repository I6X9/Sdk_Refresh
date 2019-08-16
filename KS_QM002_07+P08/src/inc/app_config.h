///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: app_config.h
//  maintainer: Halley
///////////////////////////////////////////////////////////////////////////////
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "type.h"
#include "app_msg.h"
#include "timer_api.h"
#include "drc.h"
#include "three_d.h"	
#include "virtual_bass.h"
#include "rtc.h"
#include "bt_device_type.h"
#include "timeout.h"
#include "delay.h"
#include "chip_info.h"
#include "os.h"
#include "eq.h"
#include "eq_params.h"
#include "gpio.h"

//****************************************************************************************
//WAV��ʾ�������CODE��CONST�����ź����
//�˹����������ֲ�����MIX��ʾ����
//****************************************************************************************
#define CFG_WAV_REMINDSOUND_MIX_EN

#ifdef CFG_WAV_REMINDSOUND_MIX_EN
	#include "audio_file.h"
	#define CFG_WAV_REMIND_IN_CODE_AREA			(0)				//���CODE��
	#define CFG_WAV_REMIND_IN_CONST_AREA		(1)				//��ų�����		
	#define CFG_WAV_REMIND_DET_ADC_EN							//ADC�����ʾ������
	//#define CFG_WAV_REMIND_DET_GPIO_EN							//GPIO�����ʾ������

	#ifdef CFG_WAV_REMIND_DET_ADC_EN
		#define ADC_REMIND_DET_PORT		ADC_CHANNEL_B6			//ADC���ͨ��
	#endif

	#ifdef CFG_WAV_REMIND_DET_GPIO_EN
	//GPIO KEY1
		#define GPIO_KEY1_DETECT_PORT_IN			GPIO_B_IN	
		#define GPIO_KEY1_DETECT_PORT_OE			GPIO_B_OE	
		#define GPIO_KEY1_DETECT_PORT_PU			GPIO_B_PU	
		#define GPIO_KEY1_DETECT_PORT_PD			GPIO_B_PD	
		#define GPIO_KEY1_DETECT_PORT_IE  			GPIO_B_IE
		#define GPIO_KEY1_DETECT_PORT_INT  			GPIO_B_INT
		#define GPIO_KEY1_DETECT_BIT_MASK			GPIOB6

	//GPIO KEY2
		#define GPIO_KEY2_DETECT_PORT_IN			GPIO_B_IN	
		#define GPIO_KEY2_DETECT_PORT_OE			GPIO_B_OE	
		#define GPIO_KEY2_DETECT_PORT_PU			GPIO_B_PU	
		#define GPIO_KEY2_DETECT_PORT_PD			GPIO_B_PD	
		#define GPIO_KEY2_DETECT_PORT_IE  			GPIO_B_IE
		#define GPIO_KEY2_DETECT_PORT_INT  			GPIO_B_INT
		#define GPIO_KEY2_DETECT_BIT_MASK			GPIOB24

	//GPIO KEY3
		#define GPIO_KEY3_DETECT_PORT_IN			GPIO_B_IN	
		#define GPIO_KEY3_DETECT_PORT_OE			GPIO_B_OE	
		#define GPIO_KEY3_DETECT_PORT_PU			GPIO_B_PU	
		#define GPIO_KEY3_DETECT_PORT_PD			GPIO_B_PD	
		#define GPIO_KEY3_DETECT_PORT_IE  			GPIO_B_IE
		#define GPIO_KEY3_DETECT_PORT_INT  			GPIO_B_INT
		#define GPIO_KEY3_DETECT_BIT_MASK			GPIOB25

	//GPIO KEY4
		#define GPIO_KEY4_DETECT_PORT_IN			GPIO_B_IN	
		#define GPIO_KEY4_DETECT_PORT_OE			GPIO_B_OE	
		#define GPIO_KEY4_DETECT_PORT_PU			GPIO_B_PU	
		#define GPIO_KEY4_DETECT_PORT_PD			GPIO_B_PD	
		#define GPIO_KEY4_DETECT_PORT_IE  			GPIO_B_IE
		#define GPIO_KEY4_DETECT_PORT_INT  			GPIO_B_INT
		#define GPIO_KEY4_DETECT_BIT_MASK			GPIOB26

	//GPIO KEY5
		#define GPIO_KEY5_DETECT_PORT_IN			GPIO_B_IN	
		#define GPIO_KEY5_DETECT_PORT_OE			GPIO_B_OE	
		#define GPIO_KEY5_DETECT_PORT_PU			GPIO_B_PU	
		#define GPIO_KEY5_DETECT_PORT_PD			GPIO_B_PD	
		#define GPIO_KEY5_DETECT_PORT_IE  			GPIO_B_IE
		#define GPIO_KEY5_DETECT_PORT_INT  			GPIO_B_INT
		#define GPIO_KEY5_DETECT_BIT_MASK			GPIOB27

	#endif
	
	#if (CFG_WAV_REMIND_IN_CODE_AREA) && (CFG_WAV_REMIND_IN_CONST_AREA)
		#error	"Cannot define CFG_WAV_REMIND_IN_CODE_AREA and CFG_WAV_REMIND_IN_CONST_AREA at the same time"
	#endif
#endif

//****************************************************************************************
//��ȡһ���������ԭ����player_control.c�ж���ģ�Ϊ�˷���ʹ�ã���Ϊ������app_config.h��
//****************************************************************************************

    #define GetRandNum(MaxValue) ((OSSysTickGet()%MaxValue) + 3)
	
//****************************************************************************************
//                  ϵͳĬ����������         
//****************************************************************************************
	#define MAX_VOLUME  32
	#define DEFAULT_VOLUME  24
	#define DEC_FADEIN_TIME					1000	//fadeinʱ��	

	#define ADC_DIGITAL_VOL				VOLUME_ADC_DIGITAL
	#define DAC_DIGITAL_VOL				VOLUME_DAC_DIGITAL
	
//****************************************************************************************
//                  ���ͨ��ѡ��       
//****************************************************************************************
    //ͬһʱ��ֻ��ʹ�����е�һ��
    #define OUTPUT_CHANNEL_DAC                      //DAC���
//    #define OUTPUT_CHANNEL_I2S                    //��׼I2S���
//    #define OUTPUT_CHANNEL_DAC_I2S                //��׼I2S�Լ�DACͬʱ���
//    #define OUTPUT_CHANNEL_CLASSD                 //�����STϵ��CLASSD��I2S���
//    #define OUTPUT_CHANNEL_DAC_CLASSD             //�����STϵ��CLASSD��I2S�Լ�DAC���:��������ܲ��Ų����ʵ���32KHZ,�����Ҫȫ��֧�֣����Կ���ת��������
                                                   
    
//****************************************************************************************
//					��������
//****************************************************************************************
	#define FUNC_SPI_UPDATE_EN
	
	#ifdef 	FUNC_SPI_UPDATE_EN
		//#define FUNC_UPDATE_CONTROL   			//�����������̿���
		#define FUNC_UPDATE_DONE_DELETE_FILE		//�������ɾ��������
	#endif

//****************************************************************************************
//                  Ƶ�ײɼ�����        
//****************************************************************************************
	#define FUNC_SPEC_TRUM_EN                      	//��Ƶ�׹��ܿ���
	
//****************************************************************************************
//                 ������ʾ����������        
//****************************************************************************************
	#define FUNC_SOUND_REMIND		//������ʾ

//****************************************************************************************
//                  ϵͳ����������            
//****************************************************************************************
//ʹ�ܸú��ʾMIXERģ���Զ������ֲ�����ͳһת��Ϊ44.1KHZ
//��������OK���Ӧ��ʱ��ǿ�ҽ���ʹ�ܸú�
//ʹ�ܸú��MIXERģ��Է�44.1KHZ�����ʵ�������������ת������Ƶ�źŻ�����˥��
//	#define FUNC_MIXER_SRC_EN   					
													
//****************************************************************************************
//                  ���΢������       
//****************************************************************************************
//ʹ�ܸú꣬�����Normalģʽ���ʱ���������΢��
//	#define FUNC_SOFT_ADJUST_EN

//****************************************************************************************
//                  �ļ�ϵͳ����
//****************************************************************************************
	#define FS_EXFAT_EN

//****************************************************************************************
//                  Decoder ��Ӧ��ʱ����
//****************************************************************************************
	#define DECODER_TASK_WAIT_TIMEOUT		10*1000	// 10S

	#define FUNC_DECODER_ENCRYPT_FILE               //�����û��Զ�������ļ�

//****************************************************************************************
//                  ����ƵԴʹ�õ�MIXERģ�������ͨ��������             
//****************************************************************************************
	#define MIXER_SOURCE_DEC				0		//U��/SD������
	#define MIXER_SOURCE_BT					0		//��������
	#define MIXER_SOURCE_USB				0		//USB��������
	#define MIXER_SOURCE_MIC				1		//MIC
	#define MIXER_SOURCE_ANA_MONO			2		//������ģ�����룬������GUITAR��������GUITAR+LINEIN
	#define MIXER_SOURCE_ANA_STERO			3		//˫����ģ�����룬˫����LINEIN��˫����FM����ʱMIC������


//****************************************************************************************
//                  BASS/TREB��������                        
//****************************************************************************************
//	#define FUNC_TREB_BASS_EN					//�ߵ������ڹ���
	#ifdef FUNC_TREB_BASS_EN
		#define MAX_TREB_VAL 			10		//�������ڵļ�����0--10
		#define MAX_BASS_VAL 			10		//�������ڵļ�����0--10
		#define DEFAULT_TREB_VAL		5		//�������ڵ�Ĭ�ϼ���
		#define DEFAULT_BASS_VAL		5 		//�������ڵ�Ĭ�ϼ���
	#endif
//****************************************************************************************
//                 EQ��������        
//****************************************************************************************
	#define EQ_STYLE_MODE				EQ_MODE
	
//****************************************************************************************
//                 DC Blocker��������        
//****************************************************************************************
	#define FUNC_DC_BLOCKER

//****************************************************************************************
//                 Silence detector ��������        
//****************************************************************************************
	#ifdef FUNC_DC_BLOCKER
//		#define FUNC_SILENCE_DETECTOR

		#define SILENCE_THRSHOLD				100
		#define SILENCE_COUNT					200
	#endif

//****************************************************************************************
//                  MIC��������  
//****************************************************************************************
	#define FUNC_MIC_EN								//MIC����
	#ifdef FUNC_MIC_EN
		#define FUNC_MIC_ALC_EN							//MIC ALC���ܿ���
		//#define FUNC_MIC_NOISE_GATE_EN					//MIC Noise Gate���ܿ���
		#define MAX_MICIN_VOLUME				16		//MIC�����ɵ��ļ�����0--16
		#define DEFAULT_MICIN_VOLUME			12		//MIC����Ĭ��ֵ
		#define MICIN_ANA_GAIN_VAL				VOLUME_ADC_ANA_MIC		//MIC����ͨ·ģ������(22.9dB),GIAN ��Ӧ��ϵ�� sys_vol.h
//		#define FUNC_MIC_DET_EN  						//MIC��μ�ⶨ��
		#ifdef FUNC_MIC_DET_EN
			#define	MICIN_DETECT_PORT_IN		GPIO_A_IN
			#define	MICIN_DETECT_PORT_IE		GPIO_A_IE
			#define	MICIN_DETECT_PORT_OE		GPIO_A_OE
			#define	MICIN_DETECT_PORT_PU		GPIO_A_PU
			#define	MICIN_DETECT_PORT_PD		GPIO_A_PD
			#define MICIN_DETECT_BIT			(1 << 0)
		#endif

//		#define FUNC_MIC_ECHO_EN						//MIC ECHO����
		#ifdef FUNC_MIC_ECHO_EN
			#define MAX_ECHO_DELAY_STEP			30		//MIC������ʱ�ɵ��ļ�����0--30
			#define DEFAULT_DELAY_STEP			22		//MIC������ʱĬ��ֵ
			#define DELAY_ONE_STEP				400		//MIC������ʱÿ��һ����Ӧ�Ĳ�����������
			#define MAX_ECHO_DEPTH_STEP			16		//MIC����ǿ�ȿɵ��ļ�����0--16
			#define DEFAULT_DEPTH_STEP			12		//MIC����ǿ��Ĭ��ֵ
			#define DEPTH_ONE_STEP				1024	//MIC����ǿ��ÿ��һ����Ӧ��ǿ�Ȳ���	
			
			#define MIC_ECHO_MIX_BYPASS_EN				//MIC�����Ƿ����ԭʼ����
			#define MIC_BYPASS_VOL				4095	//MIC������������
			#define MIC_ECHO_EFFECT_VOL			4095	//MICԭʼ��������
		#endif
		
		#define CFG_ECHO_DELAY                         (17640)  //400 ms
		#define CFG_ECHO_ATTENUATION                   (11626)  //-9 dB
		#define CFG_ECHO_DIRECT_SND_EN                 (    1)
		#define CFG_ECHO_CUTOFF_FREQ                   (    0)  //Disabled

	#endif

//****************************************************************************************
//					String format convert
//****************************************************************************************
	#define FUNC_STRING_CONVERT_EN 			// �ַ�������ת��

//****************************************************************************************
//                  ����ģʽ����                        
//****************************************************************************************
//	#define FUNC_USB_EN						// U�̲��Ź���
	#ifdef FUNC_USB_EN
		#define UDISK_PORT_NUM		        2		// USB�˿ڶ���
	#endif
	#define FUNC_CARD_EN						// SD�����Ź���	
	#ifdef FUNC_CARD_EN
		#define	SD_PORT_NUM                 1		// SD���˿ڶ���
	#endif

	#if (SD_PORT_NUM == 1)
		//SD_CLK����ΪӲ�����ţ�A20
		#define CARD_DETECT_PORT_IN			GPIO_A_IN	
		#define CARD_DETECT_PORT_OE			GPIO_A_OE	
		#define CARD_DETECT_PORT_PU			GPIO_A_PU	
		#define CARD_DETECT_PORT_PD			GPIO_A_PD	
		#define CARD_DETECT_PORT_IE  		GPIO_A_IE
		#define CARD_DETECT_BIT_MASK		(1 << 20)
	#else
		#define CARD_DETECT_PORT_IN			GPIO_B_IN	
		#define CARD_DETECT_PORT_OE			GPIO_B_OE	
		#define CARD_DETECT_PORT_PU			GPIO_B_PU	
		#define CARD_DETECT_PORT_PD			GPIO_B_PD	
		#define CARD_DETECT_PORT_IE  		GPIO_B_IE
		#define CARD_DETECT_BIT_MASK		(GPIOB4)
	#endif

	#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
		#define FUNC_LRC_EN							// LRC����ļ�����
		//#define FUNC_FFFB_WITHOUT_SOUND     		//��ѡ���������Ƿ񲥷�������Ĭ�ϳ�����
		//#define FUNC_FFFB_END_WITHOUT_AUTO_SWITCH	//��ѡ������β���߿��˵������Ƿ�ֹͣseek��Ĭ�ϱ��ֿ������
	#endif

//****************************************************************************************
//                 �ļ����ģʽ����        
//****************************************************************************************
	#define FUNC_BROWSER_EN  				//�ļ��������

//****************************************************************************************
//                 FM������ģʽ���� 
//****************************************************************************************
//	#define FUNC_RADIO_EN						//Fm Radio
	#ifdef FUNC_RADIO_EN
		#define FMIN_ANA_GAIN_VAL			VOLUME_ADC_ANA_LIN23	//FM����ͨ����ģ������(10.8dB)�������ΪLineInʹ����Ҫ����Ϊ18(-3.65dB)������ʧ��
												//GIAN ��Ӧ��ϵ�� sys_vol.h
		#define FUNC_RADIO_DIV_CLK_EN			//FM��Ƶ����

		#define FUNC_RADIO_RDA5807_EN
//		#define FUNC_RADIO_QN8035_EN

		#define MAX_RADIO_CHANNEL_NUM    50
		#define RADIO_SEEK_PREVIEW_SUPPORT		//֧����̨ʱ��ÿ�ѵ�һ����̨���Զ����ż�����
		#define RADIO_DELAY_SWITCH_CHANNEL		//�ӳٴ����̨�л����ϲ����������л���̨���������
		#define FM_CHL_SEARCH_TIME_UNIT 100		//��̨ʱƵ��ɨ����ms
		#define FM_PERVIEW_TIMEOUT 1000			//FM��̨Ԥ��ʱ�䣬��ֵ����С��MIN_TIMER_PERIOD������ȷ�Ȼ�������(unit ms)
	#endif


//****************************************************************************************
//                 LINEINģʽ���� 
//****************************************************************************************
//	#define FUNC_LINEIN_EN   					// Linein
	#ifdef FUNC_LINEIN_EN
		#define LINEIN_ANA_GAIN_VAL			VOLUME_ADC_ANA_LIN1	//LINEIN����ͨ����ģ������(-3.83),GIAN ��Ӧ��ϵ�� sys_vol.h
		#define LINEIN_DETECT_PORT_IN		GPIO_A_IN
		#define LINEIN_DETECT_PORT_OE		GPIO_A_OE
		#define LINEIN_DETECT_PORT_IE		GPIO_A_IE
		#define LINEIN_DETECT_PORT_PU		GPIO_A_PU
		#define LINEIN_DETECT_PORT_PD		GPIO_A_PD
		#define LINEIN_DETECT_BIT_MASK		GPIOA24
	#endif
        
//****************************************************************************************
//                 I2Sģʽ����        
//****************************************************************************************
//  #define FUNC_I2SIN_EN

	#define FUNC_I2S_MASTER_EN				//enbale:master mode, disable: slave mode
        #define I2SIN_MCLK_IO_PORT      1   //mclk port: 0--B2, 1--C8, 0XFF--NO USE
        #define I2SIN_I2S_IO_PORT       1   //i2s port: 0-- lrck: B3, bclk: B4, din: B6, do: B5;
                                            //          1-- lrck: B24, bclk: B25, din: B27, do: B26
                                            //          2-- lrck: C9, bclk: C10, din: C12, do: C11  
                                            //          0xff--NO USE

//****************************************************************************************
//                 USBһ��ͨģʽ����        
//****************************************************************************************
//	#define FUNC_USB_AUDIO_EN				//������USB����ģʽ
	#define FUNC_USB_READER_EN				//������USB������ģʽ
//	#define FUNC_USB_AUDIO_READER_EN		//һ��ͨģʽ
		#define PC_PORT_NUM			        2

//****************************************************************************************
//                 ¼��ģʽ����        
//****************************************************************************************
	#if (defined(FUNC_USB_EN) || defined(FUNC_CARD_EN))
//		#define FUNC_PLAY_RECORD_FOLDER_EN		//����ģʽ����record�ļ���
//		#define FUNC_RECORD_EN   				//¼������
		#ifdef FUNC_RECORD_EN
			#define FUNC_RECORD_FILE_DELETE_EN 		//֧��¼��ɾ������
			#define FUNC_REC_FILE_REFRESH_EN 		//��ʱ�����ļ�����������¼�����ݲ��ܱ�������⣩
			#define FUNC_REC_PLAYBACK_EN 			//¼���طŹ���

			#define RECORD_FORMAT		MP3_FORMAT	// MP3_FORMAT , WAV_FORMAT
			#define RECORD_BITRATE		96			// ˫�������֧��320kbps�����������֧��192kbps
			#define RECORD_CHANNEL		1			// ������ 1 ���� 2������2ʱ����MEM��������Ҫȥ��һЩ����
		#endif
	#endif

//****************************************************************************************
//                 RTC��������        
//****************************************************************************************
//	#define FUNC_RTC_EN						//rtc����
	#ifdef FUNC_RTC_EN
		#define FUNC_RTC_ALARM 				//alarm����
		#define FUNC_RTC_LUNAR
	#endif


//****************************************************************************************
//                 ����ģʽ����        
//****************************************************************************************
	#define FUNC_BT_EN											//��������
		#define BT_DEVICE_TYPE		BTUartDeviceRTL8761//BTUartDeviceMTK662X//BTUartDeviceRTL8761 		// bt chip type

		#define BLE_STACK										// ʹ��BLE

		#define BT_LDOEN_RST_GPIO_PORT				GPIO_PORT_B
		#define BT_LDOEN_RST_GPIO_PIN				(22)

		#define BT_UART_RTS_PORT					(1)			// 0 --- A12; 1 --- B31; 2 --- C1

		#define AP80_DEV_BOARD									// AP80_DEV_BOARD : ΪMV��AP80�����壬��������������MVs�Ĵ���Ʊ��

		#ifdef AP80_DEV_BOARD
			#define MTK_BT_PWR_EN					GPIOB22
			#define MTK_BT_RESET					GPIOB31
		#else
			#define MTK_BT_PWR_EN					GPIOC1
			#define MTK_BT_RESET					GPIOB31
		#endif

	#ifdef FUNC_BT_EN
//		#define FUNC_BT_HF_EN									//bt hf call module
		#ifdef FUNC_BT_HF_EN

//			#define FUNC_BT_HF_UART								// �����绰����ͨ��UART����(��RTK8761֧�ִ˹���)

			#define MAX_BTHF_VOLUME					(15)		//������������ȼ�
			#ifdef FUNC_RECORD_EN
				#define BT_HF_RECORD_FUNC						// ����ͨ��¼������
			#endif

			#ifdef FUNC_SOUND_REMIND
//				#define BT_HF_NUMBER_REMIND						// �����������Ź���
			#endif

			#define NON_LINEAR_PROCESS				(1)			// "�����Դ���"(AEC֮��ĺ���ģ�飬�𵽽�һ��ѹ�ƻ������Լ��������Ƶ�)�򿪻��߹رգ�0:�رգ�1:��
			#define NOISE_SUPPRESSION_LEVEL			(0)			// �������Ƶȼ���ȡֵ��Χ(1 ~ 5)��0Ϊ�رմ˹���
		#endif

		#define BT_RECONNECTION_FUNC							// �����Զ���������
		#ifdef BT_RECONNECTION_FUNC
			#define BT_POWERON_RECONNECTION						// �����Զ�����
			#ifdef BT_POWERON_RECONNECTION
				#define BT_POR_TRY_COUNTS			(7)			// �����������Դ���
				#define BT_POR_INTERNAL_TIME		(3)			// ��������ÿ���μ��ʱ��(in seconds)
			#endif

			#define BT_BB_LOST_RECONNECTION						// BB Lost֮���Զ�����
			#ifdef BT_BB_LOST_RECONNECTION
				#define BT_BLR_TRY_COUNTS			(90)		// BB Lost ������������
				#define BT_BLR_INTERNAL_TIME		(5)			// BB Lost ����ÿ���μ��ʱ��(in seconds)
			#endif

		#endif

		#define BT_ADVANCED_AVRCP_SUPPORT						// �߼�AVRCP֧��
		#ifdef BT_ADVANCED_AVRCP_SUPPORT
			#define BT_VOLUME_SYNC								// ����ͬ��
			#define BT_SONG_TRACK_INFOR							// ����ID3��Ϣ����
			#define BT_SONG_PLAY_STATUS							// ��������״̬
		#endif

		//#define FUNC_BT_DISABLE_SIMPLE_PARING					//ȡ��simple pairing���ܣ��Զ���PIN CODE

		//#define CONFIG_INQUIRY_SCAN_FUNC						// ����INQUIRY_SCAN��ز���

		//#define FUNC_BT_PBAP_EN									//PBAP����

		#define TEST_CLEAR_BT_PAIRED_INFO						//ǿ�ƶϿ���������
		
//		#define MASS_PRODUCTION_TEST_FUNC						// �������Թ���

//		#define FAST_POWER_ON_OFF_BT							// ���ٴ�/�ر���������

//		#define BT_RF_POWER_ON_OFF								// BT RF Ӳ������

//		#define AUTO_MV_BT_TEST_FUNC							// auto scan and test bt device, 
																// one must be initialized with 
																// [MvBtAutoTestFlag = 1] in this case,
																// please refer to the related files.
		#if defined(FAST_POWER_ON_OFF_BT) && defined(BT_RF_POWER_ON_OFF)
			#error	"Cannot define FAST_POWER_ON_OFF_BT and BT_RF_POWER_ON_OFF at the same time"
		#endif
	#endif
//****************************************************************************************
//                 UART DEBUG��������        
//****************************************************************************************
	#define FUNC_DEBUG_EN
	#ifdef FUNC_DEBUG_EN
		#define FUART_RX_PORT  0xFF	//rx port  0--A1��1--B6��2--C4��0xFF--NO USE
		#define FUART_TX_PORT  1	//tx port  0--A0��1--B7��2--C3��0xFF--NO USE
	#endif

//****************************************************************************************
//                 WATCH DOG��������        
//****************************************************************************************
	#define FUNC_WATCHDOG_EN			//WATCHDOG����


//****************************************************************************************		
//                 ��Ч���ܿ���(3D, VB, DRC)
//****************************************************************************************
	#define FUNC_AUDIO_EFFECT_EN
	#ifdef FUNC_AUDIO_EFFECT_EN
		#include "audio_effect_codec.h"
		#define FUNC_AUDIO_EQ_EN						//EQ���ܿ���
		#define DIGITAL_PER_GAIN_EN						//��������Ԥ����
		#define DIGITAL_PER_GAIN_VAL       1850         //��������Ԥ����ֵ
	#endif

//****************************************************************************************		
//				��Ч�����л�ʱ���뵭����������������pop��(3D, VB, DRC��EQ)
//				Ŀǰ��׼ֻ�������EQ�л�ʱ����pop����������������Ҫ��һ���ĸĶ�
//****************************************************************************************
	//#define AUDIO_EFFECT_SWITCH_FADEOUT_FADEIN
	
//****************************************************************************************
//                 �ϵ���书������        
//****************************************************************************************
	#define FUNC_BREAKPOINT_EN			// ϵͳ�ϵ���Ϣ����
	#ifdef FUNC_BREAKPOINT_EN
		#define BP_SAVE_TO_NVM				//���ϵ���Ϣ���浽NVM ��
		#define BP_SAVE_TO_FLASH			// ������书�ܣ�֧�ֽ��ϵ���Ϣ���浽FLASH�У���ֹ���綪ʧ��
		//#define BP_SAVE_TO_EEPROM			// ������书�ܣ�֧�ֽ��ϵ���Ϣ���浽EEPROM�У���ֹ���綪ʧ��		
		#define FUNC_MATCH_PLAYER_BP		// ��ȡFSɨ����벥��ģʽ�ϵ���Ϣ��ƥ����ļ����ļ���ID��
	#endif


//****************************************************************************************
//                 VIN����5V�ٴ�Trim LDO3V3��������        
//****************************************************************************************
//	#define FUNC_5VIN_TRIM_LDO3V3			//LDO3V3_TRIM 5VIN����	


//****************************************************************************************
//                 POWER MONITOR��������        
//****************************************************************************************
// ������ܼ���(�����ڴ����ϵͳ)�Ĺ��ܺ��ѡ���
// ���ܼ��Ӱ�������ص�ѹ��⼰�͵�ѹ���ϵͳ��Ϊ�Լ����ָʾ��
// ��ص�ѹ��⣬��ָLDOIN����˵ĵ�ѹ��⹦��(���ϵͳһ�㶼�ǵ��ֱ�Ӹ�LDOIN�ܽŹ���)
// �ù��ܺ�򿪺�Ĭ�ϰ�����ص�ѹ��⹦�ܣ��йص�ص�ѹ���������ɶ�������������power_monitor.c�ļ�

//USE_POWERKEY_SLIDE_SWITCH ��USE_POWERKEY_SOFT_PUSH_BUTTON �����겻Ҫͬʱ����
//	#define USE_POWERKEY_SLIDE_SWITCH     		//for slide switch case ONLY
	#define USE_POWERKEY_SOFT_PUSH_BUTTON		//for soft push button case ONLY

//	#define POWERON_DETECT_VOLTAGE        		//��������ص�ѹ�����ڹػ���ѹ��ֱ�ӽ���ػ�����
	#define FUNC_POWER_MONITOR_EN
//	#define	OPTION_CHARGER_DETECT				//�򿪸ú궨�壬֧��GPIO������豸���빦��
			//�����˿�����
	#ifdef OPTION_CHARGER_DETECT
		#define CHARGE_DETECT_PORT_PU			GPIO_A_PU
		#define CHARGE_DETECT_PORT_PD			GPIO_A_PD
		#define CHARGE_DETECT_PORT_IN			GPIO_A_IN
		#define CHARGE_DETECT_PORT_IE			GPIO_A_IE
		#define CHARGE_DETECT_PORT_OE			GPIO_A_OE
		#define CHARGE_DETECT_BIT				(1 << 3)
	#endif

//	#define FUNC_POWERKEY_SOFT_POWERON_EN 		//������BYBASSģʽʵ��
	#define FUNC_GPIO_POWER_ON_EN         		//GPIO����ϵͳ�ϵ�

	#ifdef FUNC_GPIO_POWER_ON_EN          		//GPIO����ϵͳ�ϵ�
		#define POWERON_GPIO_PORT_OUT			GPIO_A_OUT
		#define POWERON_GPIO_PORT_BIT			GPIOA24

		#define POWERON_GPIO_PORT_IE			(POWERON_GPIO_PORT_OUT + 1)
		#define POWERON_GPIO_PORT_OE			(POWERON_GPIO_PORT_OUT + 2)
		#define POWERON_GPIO_PORT_PU			(POWERON_GPIO_PORT_OUT + 5)
		#define POWERON_GPIO_PORT_PD			(POWERON_GPIO_PORT_OUT + 6)

		#define SysPowerOnControlEnable()		do{\
												GpioSetRegOneBit(POWERON_GPIO_PORT_PU, POWERON_GPIO_PORT_BIT);\
												GpioClrRegOneBit(POWERON_GPIO_PORT_PD, POWERON_GPIO_PORT_BIT);\
												GpioClrRegOneBit(POWERON_GPIO_PORT_IE, POWERON_GPIO_PORT_BIT);\
												GpioSetRegOneBit(POWERON_GPIO_PORT_OE, POWERON_GPIO_PORT_BIT);\
												GpioSetRegOneBit(POWERON_GPIO_PORT_OUT, POWERON_GPIO_PORT_BIT);\
												DBG("GpioControlSysPowerOn.........................\n");\
												}while(0)
								
		#define SysPowerOnControlDisable()		do{\
												GpioSetRegOneBit(POWERON_GPIO_PORT_PU, POWERON_GPIO_PORT_BIT);\
												GpioClrRegOneBit(POWERON_GPIO_PORT_PD, POWERON_GPIO_PORT_BIT);\
												GpioClrRegOneBit(POWERON_GPIO_PORT_IE, POWERON_GPIO_PORT_BIT);\
												GpioSetRegOneBit(POWERON_GPIO_PORT_OE, POWERON_GPIO_PORT_BIT);\
												GpioClrRegOneBit(POWERON_GPIO_PORT_OUT, POWERON_GPIO_PORT_BIT);\
												DBG("GpioControlSysPowerOff.........................\n");\
												}while(0)
		
	#endif

//****************************************************************************************
//                 ������������        
//****************************************************************************************
//	#define FUNC_KEY_BEEP_SOUND_EN    						//����beep������

	//ADC KEY����
	#define FUNC_ADC_KEY_EN								//ADC KEY�꿪��  									
		#define ADC_KEY_PORT_CH1	ADC_CHANNEL_B5			//���ֻʹ��1·ADC��������������
		//#define	ADC_KEY_PORT_CH2	ADC_CHANNEL_B5			//���ֻʹ��2·ADC��������������

	//IR KEY����
//	#define FUNC_IR_KEY_EN   							//IR KEY�꿪��
		#define IR_KEY_PORT			IR_USE_GPIOC2			//IRʹ�õĶ˿ڣ�IR_USE_GPIOA10, IR_USE_GPIOB7 or IR_USE_GPIOC2

	//CODING KEY����
//	#define FUNC_CODING_KEY_EN							//CODING KEY�꿪��			
		#define 	CODING_KEY_A_PORT_IN	GPIO_B_IN	
		#define		CODING_KEY_A_PORT_OE	GPIO_B_OE 			
		#define		CODING_KEY_A_PORT_PU	GPIO_B_PU 
		#define		CODING_KEY_A_PORT_PD	GPIO_B_PD 					
		#define		CODING_KEY_A_PORT_INT	GPIO_B_INT 				
		#define		CODING_KEY_A_BIT		(1 << 21)	//GPIO_B[21] for signal A 
	
		#define		CODING_KEY_B_PORT_IN	GPIO_B_IN 				
		#define		CODING_KEY_B_PORT_OE	GPIO_B_OE 			
		#define		CODING_KEY_B_PORT_PU	GPIO_B_PU 
		#define		CODING_KEY_B_PORT_PD	GPIO_B_PD 					
		#define		CODING_KEY_B_BIT		(1 << 20)	//GPIO_B[20] for signal B
	
	#define FUNC_POWER_KEY_EN							//POWER KEY�꿪�أ����ظ��ù���
														//��Ҫͬʱ������USE_POWERKEY_SOFT_PUSH_BUTTON �ù��ܲ�������Ч

	#define FUNC_KEY_HOLD_EN							//�����������֡�

//****************************************************************************************
//                 ��������        
//****************************************************************************************
#define FUNC_AMP_MUTE_EN         //����Mute��������		
#define FUNC_AMP_POWER_EN									
								
#ifdef FUNC_AMP_MUTE_EN
															
	#define AMP_SILENCE_MUTE_EN     //�򿪸ú֧꣬����DAC�������ʱMUTE����
	#define AMP_GPIO_MUTE_EN        //�򿪸ú֧꣬��GPIO���ƹ���MUTE
	#ifdef  AMP_SILENCE_MUTE_EN
		#define SILENCE_MUTE_TIMER        500     //���ź�MUTE���ʱ��
		#define SILENCE_UNMUTE_TIMER      30      //��MUTE���ʱ��
		#define DEC_SILENCE_MUTE_VALUE    1       //DEC����MUTE��ֵ
		#define ANA_SILENCE_MUTE_VALUE    16      //ģ������MUTE��ֵ
		#define SILENCE_MUTE_DET_CNT      85      //�ٷֱȣ���200ms�ڵ��ڼ�ֵⷧ�İٷֱȴﵽ��ֵ����������MUTE
	#endif
	
	#ifdef AMP_GPIO_MUTE_EN		
		#define AMP_MUTE_PORT_OUT				GPIO_A_OUT
		#define AMP_MUTE_PORT_BIT				  GPIOA25

		#define AMP_MUTE_PORT_IE				(AMP_MUTE_PORT_OUT + 1)
		#define AMP_MUTE_PORT_OE				(AMP_MUTE_PORT_OUT + 2)
		#define AMP_MUTE_PORT_PU				(AMP_MUTE_PORT_OUT + 5)
		#define AMP_MUTE_PORT_PD				(AMP_MUTE_PORT_OUT + 6)

		#define GpioAmpMuteEnable()				do{\
												GpioSetRegOneBit(AMP_MUTE_PORT_PU, AMP_MUTE_PORT_BIT);\
												GpioClrRegOneBit(AMP_MUTE_PORT_PD, AMP_MUTE_PORT_BIT);\
												GpioClrRegOneBit(AMP_MUTE_PORT_IE, AMP_MUTE_PORT_BIT);\
												GpioSetRegOneBit(AMP_MUTE_PORT_OE, AMP_MUTE_PORT_BIT);\
												GpioClrRegOneBit(AMP_MUTE_PORT_OUT, AMP_MUTE_PORT_BIT);\
												}while(0)
							
		#define GpioAmpMuteDisable()			do{\
												GpioSetRegOneBit(AMP_MUTE_PORT_PU, AMP_MUTE_PORT_BIT);\
												GpioClrRegOneBit(AMP_MUTE_PORT_PD, AMP_MUTE_PORT_BIT);\
												GpioClrRegOneBit(AMP_MUTE_PORT_IE, AMP_MUTE_PORT_BIT);\
												GpioSetRegOneBit(AMP_MUTE_PORT_OE, AMP_MUTE_PORT_BIT);\
												GpioSetRegOneBit(AMP_MUTE_PORT_OUT, AMP_MUTE_PORT_BIT);\
												}while(0)
	#endif
#endif
								
#ifdef FUNC_AMP_POWER_EN
																					
	#define AMP_POWER_PORT_OUT				GPIO_B_OUT
	#define AMP_POWER_PORT_BIT				GPIOB8

	#define AMP_POWER_PORT_IE				(AMP_POWER_PORT_OUT + 1)
	#define AMP_POWER_PORT_OE				(AMP_POWER_PORT_OUT + 2)
	#define AMP_POWER_PORT_PU				(AMP_POWER_PORT_OUT + 5)
	#define AMP_POWER_PORT_PD				(AMP_POWER_PORT_OUT + 6)

	#define AmpPowerEnable()				do{\
											GpioSetRegOneBit(AMP_POWER_PORT_PU, AMP_POWER_PORT_BIT);\
											GpioClrRegOneBit(AMP_POWER_PORT_PD, AMP_POWER_PORT_BIT);\
											GpioClrRegOneBit(AMP_POWER_PORT_IE, AMP_POWER_PORT_BIT);\
											GpioSetRegOneBit(AMP_POWER_PORT_OE, AMP_POWER_PORT_BIT);\
											GpioSetRegOneBit(AMP_POWER_PORT_OUT, AMP_POWER_PORT_BIT);\
											}while(0)
								
	#define AmpPowerDisable()				do{\
											GpioSetRegOneBit(AMP_POWER_PORT_PU, AMP_POWER_PORT_BIT);\
											GpioClrRegOneBit(AMP_POWER_PORT_PD, AMP_POWER_PORT_BIT);\
											GpioClrRegOneBit(AMP_POWER_PORT_IE, AMP_POWER_PORT_BIT);\
											GpioSetRegOneBit(AMP_POWER_PORT_OE, AMP_POWER_PORT_BIT);\
											GpioClrRegOneBit(AMP_POWER_PORT_OUT, AMP_POWER_PORT_BIT);\
											}while(0)

#endif				
					
/**********************************************************************************************************
 *               LED DISPLAY DEFINE                          *
 *********************************************************************************************************/
 
// ��ʾģ�鶨�壬����ʵ��ѡ���֧��LED,LCD,����LED��
// FUNC_DISP_EN���Ƿ�����ʾ����
// FUNC_SEG_LCD_EN, FUNC_SEG_LED_EN,	FUNC_SINGLE_LED_EN������Ҫѡ��һ����������������ϸ����Ϣ��segPanel.h
// ���⻹��Ҫ����gDisplayMode����Ϊ��Ҫ��ģʽ,��ʾģʽ��������.
// DISP_DEV_SLED	-> ����LED��
// DISP_DEV_LED1888	-> ��λ�����LED��
// DISP_DEV_LED8888	-> ��λ����LED��
// DISP_DEV_LED57	-> 5*7����LED��
// DISP_DEV_LCD58	-> 5*8����LCD��
//ע����ʾ���ƺ���gDisplayMode�����Ķ�Ӧ��ϵ������Ҫѡ��LED57��ʾ�豸������Ҫ����FUNC_SEG_LED_EN�겢��gDisplayMode��ֵΪDISP_DEV_LED57��

#define FUNC_DISP_EN					//��ʾ����

#ifdef FUNC_DISP_EN						              
	//#define FUNC_SEG_LED_EN				    //����LED��
	//#define FUNC_SEG_LCD_EN				    //����LCD��
	#define FUNC_SINGLE_LED_EN			    //����LED��
	//#define FUNC_ALEXA_PWM_LED_EN			  //PWM LED��
#endif

//�������LED ��ѡ��
#ifdef	FUNC_SEG_LED_EN
	//#define FUNC_7PIN_SEG_LED_EN		//7ֻ�ŵĶ���LED ��
	//#define FUNC_6PIN_SEG_LED_EN		  //6ֻ�ŵĶ���LED ��
	//#define FUNC_AIP1629A_LED_EN            //AIP1629LED��������IC
#endif

#ifdef FUNC_SINGLE_LED_EN
//����
	#define LED_BLUE_PORT_OUT				GPIO_A_OUT
	#define LED_BLUE_PORT_BIT				  GPIOA0

	#define LED_BLUE_PORT_IE				(LED_BLUE_PORT_OUT + 1)
	#define LED_BLUE_PORT_OE				(LED_BLUE_PORT_OUT + 2)
	#define LED_BLUE_PORT_PU				(LED_BLUE_PORT_OUT + 5)
	#define LED_BLUE_PORT_PD				(LED_BLUE_PORT_OUT + 6)

	#define BlueLedOn()						do{\
											GpioSetRegOneBit(LED_BLUE_PORT_PU, LED_BLUE_PORT_BIT);\
											GpioClrRegOneBit(LED_BLUE_PORT_PD, LED_BLUE_PORT_BIT);\
											GpioClrRegOneBit(LED_BLUE_PORT_IE, LED_BLUE_PORT_BIT);\
											GpioSetRegOneBit(LED_BLUE_PORT_OE, LED_BLUE_PORT_BIT);\
											GpioSetRegOneBit(LED_BLUE_PORT_OUT, LED_BLUE_PORT_BIT);\
										}while(0)
									
	#define BlueLedOff()					do{\
											GpioSetRegOneBit(LED_BLUE_PORT_PU, LED_BLUE_PORT_BIT);\
											GpioClrRegOneBit(LED_BLUE_PORT_PD, LED_BLUE_PORT_BIT);\
											GpioClrRegOneBit(LED_BLUE_PORT_IE, LED_BLUE_PORT_BIT);\
											GpioSetRegOneBit(LED_BLUE_PORT_OE, LED_BLUE_PORT_BIT);\
											GpioClrRegOneBit(LED_BLUE_PORT_OUT, LED_BLUE_PORT_BIT);\
										}while(0)

//���
	#define LED_RED_PORT_OUT				GPIO_B_OUT
	#define LED_RED_PORT_BIT				  GPIOB8

	#define LED_RED_PORT_IE				(LED_RED_PORT_OUT + 1)
	#define LED_RED_PORT_OE				(LED_RED_PORT_OUT + 2)
	#define LED_RED_PORT_PU				(LED_RED_PORT_OUT + 5)
	#define LED_RED_PORT_PD				(LED_RED_PORT_OUT + 6)

	#define RedLedOn()						do{\
											GpioSetRegOneBit(LED_RED_PORT_PU, LED_RED_PORT_BIT);\
											GpioClrRegOneBit(LED_RED_PORT_PD, LED_RED_PORT_BIT);\
											GpioClrRegOneBit(LED_RED_PORT_IE, LED_RED_PORT_BIT);\
											GpioSetRegOneBit(LED_RED_PORT_OE, LED_RED_PORT_BIT);\
											GpioClrRegOneBit(LED_RED_PORT_OUT, LED_RED_PORT_BIT);\
										}while(0)
									
	#define RedLedOff()						do{\
											GpioSetRegOneBit(LED_RED_PORT_PU, LED_RED_PORT_BIT);\
											GpioClrRegOneBit(LED_RED_PORT_PD, LED_RED_PORT_BIT);\
											GpioClrRegOneBit(LED_RED_PORT_IE, LED_RED_PORT_BIT);\
											GpioSetRegOneBit(LED_RED_PORT_OE, LED_RED_PORT_BIT);\
											GpioSetRegOneBit(LED_RED_PORT_OUT, LED_RED_PORT_BIT);\
										}while(0)
#endif

//****************************************************************************************
//                 ��Ƶ�˵�����        
//****************************************************************************************
	#define FUNC_AUDIO_MENU_EN						//���������ܸ���ѡ��MIC������������ʱ������ǿ�ȡ�BASS���ڡ�TREB����
		#define		MENU_TIMEOUT		5000		//����˵����ں����5��û�а�MENU��VOL_UP��VOL_DW�����Զ��˳��˵�ģʽ


//****************************************************************************************
//                 ��ʱ�ػ���������        
//****************************************************************************************
//	#define FUNC_SLEEP_EN
	#define FUNC_SLEEP_LEDOFF_EN
															
	#ifdef FUNC_SLEEP_EN
		#define SLEEP_POWEROFF_TMR   30   //��λ������
	#endif
	#ifdef FUNC_SLEEP_LEDOFF_EN
		#define SLEEP_LED_OFF_TMR    1
	#endif

//****************************************************************************************
//                 ϵͳģʽֵ����        
//****************************************************************************************
enum EnumModuleID
{
    MODULE_ID_UNKNOWN     = 0,	// idle

    MODULE_ID_PLAYER_SD,		// player
    MODULE_ID_PLAYER_USB,		// player
    MODULE_ID_BLUETOOTH,		// module bt
    MODULE_ID_RECORDER,			// recorder
    MODULE_ID_RADIO,			// FM/AM radio
    MODULE_ID_LINEIN,			// line in
    MODULE_ID_I2SIN,            // I2s in
    MODULE_ID_USB_AUDIO,		// usb audio
    MODULE_ID_USB_READER,		// usb audio
    MODULE_ID_USB_AUDIO_READER,	// usb audio

    //add more herr
    MODULE_ID_RTC,				// rtc

    MODULE_ID_BT_HF,
	MODULE_ID_IDLE,
    // ����Ӧ��ģʽ������ͨ��mode��ѭ���л�����
    //////////////////////////////////////////////////////////////
    // DO NOT MODIFY FOLLOWINGS
    MODULE_ID_END,				// end of moduleID
    // ����Ӧ��ģʽ����ͨ��mode���л����룬���Ǹ���ʵ����; ͨ����ݼ���������������ʱ�Ż����

    //MODULE_ID_ALARM_REMIND,		// alarm remind
    MODULE_ID_POWEROFF,			// faked module ID for Power OFF
    MODULE_ID_STANDBY,			// faked module ID for StandBy mode
#ifdef FUNC_REC_PLAYBACK_EN
    MODULE_ID_REC_BACK_PLAY,	// ¼���ط�ģʽ
#endif
    MODULE_ID_USER_PLUS,		// �û���չ��ʼID
};


//****************************************************************************************
//                 MEM����        
//****************************************************************************************
//DEC
#define DEC_MEM_ADDR              		(VMEM_ADDR + 0 * 1024)		//decoder 28KB
#define DEC_MEM_MAX_SIZE          		(28 * 1024)            

//ENC
#define ENC_MEM_ADDR					(VMEM_ADDR + 1 * 1024)		//encoder 17KB mp3���� ���巽ʽ ��ʼ1k��pcm_sync.c�ж����ͻ  mp3ֻ��16k  
#define ENC_MEM_SIZE					(17 * 1024)

//ENC BUF
#define MP3_OUT_SAMPLES_BUF             (VMEM_ADDR + 18 * 1024)		//record buf 19KB
#define MP3_OUT_SAMPLES_BUF_SIZE        (19 * 1024)

//ADPCM - ENCODE
#define ADPCM_ENC_MEM_ADDR				(VMEM_ADDR + 28 * 1024)		// adpcm for bt phone call recording
#define ADPCM_ENC_MEM_SIZE				(3*1024)

//ADPCM - ENCODE BUF
#define ADPCM_OUT_SAMPLES_BUF             (VMEM_ADDR + 31 * 1024)	//record buf 6KB
#define ADPCM_OUT_SAMPLES_BUF_SIZE        (6 * 1024)

//���漸��FIFO������PMEM�У�������(VMEM_ADDR + 32 * 1024)
//PCM FIFO
#define PCM_FIFO_ADDR             		(VMEM_ADDR + 37 * 1024)		//pcm fifo 16KB
#define PCM_FIFO_LEN              		(16 * 1024)

//ADC FIFO
#define ADC_FIFO_ADDR         			(VMEM_ADDR + 53 * 1024)		//adc fifo 4KB
#define ADC_FIFO_LEN             		(4 * 1024)    

//I2S FIFO
#define I2SIN_FIFO_ADDR                 (VMEM_ADDR + 57 * 1024)
#define I2SIN_FIFO_LEN                  (3 * 1024)

//BUART
#define BUART_RX_TX_FIFO_ADDR  			(VMEM_ADDR + 60 * 1024)		//buart fifo 7KB��RX��TX��������
#define BUART_RX_FIFO_SIZE             	(3 * 1024)         
#define BUART_TX_FIFO_SIZE             	(1 * 1024)         


////////////////////�ڴ�������/////////////////////////
#define APP_MMM_MALLOC(MemSize, MemType)          OSMalloc(MemSize, MemType)
#define APP_MMM_REALLOC(MemPtr, MemSize, MemType) OSRealloc(MemPtr, MemSize, MemType)
#define APP_MMM_FREE(MemPtr)                      OSFree(MemPtr)

////////////////////OTP�û�����˵��//////////////////////
// ������ַ(�û���¼,ȫ���ʾΪ��Ч��ַ)
// ��ַ��0x7F6C~0x7F71
// ��С: 6Byte
// 
// �û��Զ�������(�û���¼���Լ�˽��)
// ��ַ��0x7F10~0x7F1F
// ��С��16Byte
//
// ע�⣺
// ����OTPд����¼��Ҫ��MV���׵Ĺ��ߺ���λ�����
// ��ȡOTP�ռ�����code Flash�������Ҫƫ��0x7C0000,���ܻ�ȡ����ȷ������
// ����OTP��ζ�ȡ���Ķ�bt_app_func.c��GetBtAddr() ����
//////////////////////////////////////////////////////////

/////////////////////FLASH ����/////////////////////////

//#define FLASH_ERASE_IN_TCM			//FLASH�����������TCM��,Ŀǰֻ������GD��FLASH

#include "debug.h"

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif/*_APP_CONFIG_H_*/
