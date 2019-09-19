/******************************************************************************
 * @file    app_config.h
 * @author
 * @version V0.0.7_test2(o26:test10)
 * @date    2019-04-15
 * @maintainer
 * @brief
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "type.h"
#include "i2c_host.h"
#include "flash_boot.h"

//************************************************************************************************************
//    �̼��汾FIRMWARE_VERSION ��flash_config.h����
// ***********************************************************************************************************

//************************************************************************************************************
//    FLASH_BOOT��������ʹ�� ����flash_boot.h��ѡ�����á�
// 1��flash bootռ���ʼ��64K Byte */
// 2��Ĭ������flash boot���ܣ�FLASH_BOOT_EN = 1 */
// 3��flashbootʵʩmva������������ȷ��void stub(void)ָ����0xB8~0xBB�汾�Ų�ͬ��ͬ�汾������*/
// 4��ʹ��pctool mva����usercode���ܣ��迪��usbdeviceģʽ����⣩*/
//************************************************************************************************************

//************************************************************************************************************
//    ��ϵͳĬ�Ͽ���2��ϵͳȫ�ֺ꣬��IDE��������(Build Settings-Compiler-Symbols)���˴���������
//*CFG_APP_CONFIG �� FUNC_OS_EN*/
//************************************************************************************************************

//************************************************************************************************************
//    ���ܿ���˵����
// *CFG_APP_*  : ���ϵͳӦ��ģʽ����������USB U�̲��Ÿ���Ӧ��ģʽ��ѡ��
// *CFG_FUNC_* : ������ܿ���
// *CFG_PARA_* : ϵͳ�����������
// *CFG_RES_*  : ϵͳӲ����Դ����
// ************************************************************************************************************

//****************************************************************************************
//       оƬ�ͺ�ѡ������
// Ӧ�ÿ���ʱ��һ��Ҫѡ���ͺ�,��ֹ��λ����ʾ��mic��linein�����Ӳ���ӿڲ�һ��
//****************************************************************************************
//#define  CFG_CHIP_BP1064     //64pin
#define  CFG_CHIP_BP1048       //48pin
//#define  CFG_CHIP_BP1032     //32pin


	#if  defined(CFG_CHIP_BP1048) || defined(CFG_CHIP_BP1064)
	//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
	#define CFG_RES_MIC_SELECT      (3)
	#endif

	#ifdef CFG_CHIP_BP1032
	//0=NO MIC, 1= MIC1, 2= MIC2, 3 = MCI1+MIC2
	#define CFG_RES_MIC_SELECT      (1)
	#endif	

//****************************************************************************************
// ϵͳApp����ģʽѡ��
//****************************************************************************************

/**UDisk����ģʽ**/
//#define	CFG_APP_USB_PLAY_MODE_EN

/**SD Card����ģʽ**/
#define	CFG_APP_CARD_PLAY_MODE_EN

/**��·����ģʽ**/
#define	CFG_APP_LINEIN_MODE_EN

#ifdef CFG_APP_LINEIN_MODE_EN
#define CFG_LINEIN_DET_EN
#ifdef	CFG_LINEIN_DET_EN
	#define LINEIN_DET_GPIO					GPIOB6
	#define LINEIN_DET_GPIO_IN 				GPIO_B_IN
	#define LINEIN_DET_BIT_MASK				GPIO_INDEX6
	#define LINEIN_DET_GPIO_IE 				GPIO_B_IE
	#define LINEIN_DET_GPIO_OE 				GPIO_B_OE
	#define LINEIN_DET_GPIO_PU 				GPIO_B_PU
	#define LINEIN_DET_GPIO_PD 				GPIO_B_PD
#endif
#endif

/**HDMI INģʽ**/
//#define CFG_APP_HDMIIN_MODE_EN

/**��������**/
#define CFG_APP_BT_MODE_EN

/**����������**/
//#define CFG_APP_RADIOIN_MODE_EN
#ifdef CFG_APP_RADIOIN_MODE_EN
	#define	CFG_RADIO_CLK_M12
	//#define CFG_RADIO_I2C_SD_SAME_PIN_EN//fm��sd����data/clk
	
	//#define CFG_RADIO_CLK_ENABLE()				GPIO_PortAModeSet(GPIOA16, 0x4)
	#define CFG_RADIO_IIC_ENABLE()				GPIO_PortAModeSet(GPIOA21, 0x0),GPIO_PortAModeSet(GPIOA22, 0x0)
	#define CFG_RADIO_IIC_HANDLER()				I2cMasterCreate(PORT_A, 22, PORT_A, 21)

    #define FUNC_RADIO_RDA5807_EN
    //#define FUNC_RADIO_QN8035_EN  //оƬioʱ��ֻ֧��12M��qn8035������Ҿ���

#if defined(FUNC_RADIO_RDA5807_EN) && defined(FUNC_RADIO_QN8035_EN)
   #error Conflict: radio type error //����ͬʱѡ��������ʾģʽ
#endif

#endif

/**SPDIF ����ģʽ**/
//#define	CFG_APP_OPTICAL_MODE_EN
#ifdef CFG_APP_OPTICAL_MODE_EN
#define SPDIF_OPTICAL_PORT_ANA_INPUT	SPDIF_ANA_INPUT_A30
#endif

/**SPDIF ͬ��ģʽ**/
//#define CFG_APP_COAXIAL_MODE_EN

/**USB��������������һ��ͨ����**/
//#define CFG_APP_USB_AUDIO_MODE_EN

/**I2SIN����ģʽ**/
//#define CFG_APP_I2SIN_MODE_EN

//****************************************************************************************
//                 ��Ƶ���ͨ���������
//˵��:
//    �������Դ��ͬʱ�����
//****************************************************************************************

/**I2SOUT��DAC-Xͨ����ѡһ�����Ҫͬʱ֧����Ҫ�޸Ĵ���**/
/**DAC-Xͨ������ѡ��**/
#define CFG_RES_AUDIO_DACX_EN


/**I2S��Ƶ���ͨ������ѡ��**/
//#define CFG_RES_AUDIO_I2SOUT_EN
#ifdef CFG_RES_AUDIO_I2SOUT_EN
#ifdef CFG_RES_AUDIO_DACX_EN
#undef CFG_RES_AUDIO_DACX_EN
#endif
#endif

//****************************************************************************************
//     I2S�������ѡ��
//˵��:
//    1.I2S���Ҳʹ��ʱ�˿�ѡ���ģʽ��Ҫע�Ᵽ��һ��;
//    2.i2s�����Ϊslave����Ҫע��MCLKѡ��Ͳ��������⣬�Լ��첽ʱ������;
//****************************************************************************************
#if defined(CFG_APP_I2SIN_MODE_EN) || defined(CFG_RES_AUDIO_I2SOUT_EN)
#define	CFG_RES_I2S_PORT  					1//1:I2S1����; 0:I2S0����
#define	CFG_RES_I2S_MODE					0//0:master mode ;1:slave mode
#if (CFG_RES_I2S_PORT==0)
	#define	CFG_RES_I2S_IO_PORT				0//  0:i2S0-A0-1-2-3-4; 1:I2S1-A7-8-9-10-11
#else
	#define	CFG_RES_I2S_IO_PORT				1//  0:i2S0-A0-1-2-3-4; 1:I2S1-A7-8-9-10-11
#endif
#endif

//****************************************************************************************
//                 ����������ѡ��
//˵��:
//    ���½���������ѡ���Ӱ��code size;
//****************************************************************************************
#define USE_MP3_DECODER
#define USE_WMA_DECODER
#define USE_SBC_DECODER
#define USE_WAV_DECODER
//#define USE_FLAC_DECODER
//#define USE_AAC_DECODER
//#define USE_AIF_DECODER
//#define USE_AMR_DECODER
//#define USE_APE_DECODER

//****************************************************************************************
//                 ����Ч��������
//****************************************************************************************
//�ߵ������ڹ�������˵��:
//    1.�˹����ǻ���MIC OUT EQ�����ֶ����õģ���Ҫ�ڵ���������ʹ�ܴ�EQ��
//    2.Ĭ��f5��Ӧbass gain,f6��Ӧtreb gain,�������������޸Ĵ�EQ filter��Ŀ����Ҫ��Ӧ�޸�BassTrebAjust()�ж�Ӧ��ţ�
//EQģʽ��������˵��:
//    1.�˹����ǻ���MUSIC EQ�����ֶ����õģ���Ҫ�ڵ���������ʹ�ܴ�EQ��
//    2.����flat/classic/pop/jazz/pop/vocal boost֮��ͨ��KEY���л�   
#define CFG_FUNC_AUDIO_EFFECT_EN //����Чʹ�ܿ���
#ifdef CFG_FUNC_AUDIO_EFFECT_EN

 	//#define CFG_FUNC_EQ_MODE_EN     //EQģʽ��������               

    //#define CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN     //���ź��Զ��ػ����ܣ�
    #ifdef CFG_FUNC_SILENCE_AUTO_POWER_OFF_EN      

		#define  SILENCE_THRESHOLD                 120        //�����źż�����ޣ�С�����ֵ��Ϊ���ź�
		#define  SILENCE_POWER_OFF_DELAY_TIME      60*1000     //���źŹػ���ʱʱ�䣬��λ��ms

    #endif

	#if CFG_RES_MIC_SELECT
		#define	CFG_FUNC_MIC_KARAOKE_EN      //MIC karaoke����ѡ��
	#endif
	
	#ifdef CFG_FUNC_MIC_KARAOKE_EN
        
		//#define CFG_FUNC_DETECT_MIC_EN //mic��μ�⹦��

        #define CFG_FUNC_TREB_BASS_EN   //�ߵ������ڹ�������            
        
		//���ܹ���ѡ������
		//ע:����Ҫ������Ƶ��������౸��������⣬��Ҫ�����˹���(����MIC�źż��ӿڴ���)
		#define  CFG_FUNC_SHUNNING_EN                                

				#define SHNNIN_VALID_DATA                            500  ////MIC������ֵ
				#define SHNNIN_STEP                                  256  /////���ε��ڵĲ���
    			#define SHNNIN_THRESHOLD                             SHNNIN_STEP*2  ////threshold
    			#define SHNNIN_VOL_RECOVER_TIME                      50////���������ָ�ʱ����100*20ms = 2s
    			#define SHNNIN_UP_DLY                                3/////��������ʱ��
    			#define SHNNIN_DOWN_DLY                              1/////�����½�ʱ��

	#endif	

	//**��ƵSDK�汾��,��Ҫ�޸�**/
	#define  CFG_EFFECT_MAJOR_VERSION						(CFG_SDK_MAJOR_VERSION)
	#define  CFG_EFFECT_MINOR_VERSION						(CFG_SDK_MINOR_VERSION)
	#define  CFG_EFFECT_USER_VERSION						(CFG_SDK_PATCH_VERSION)		

	/**���ߵ���Ӳ���ӿ���USB HID�ӿڣ�����UART�ӿ�*/
	/**����ʹ��USB HID�ӿڣ��շ�buf��512Byte*/
	/**UART�ӿ�ռ��2·DMA���շ�Buf��2k Byte*/
	/**����ʹ��USB HID��Ϊ�����ӿڣ�DMA��Դ����*/
	/**���ʹ��UART��Ϊ�����ӿ�������DMA�ӿڲ���֤DMA��Դ����*/
	#define  CFG_COMMUNICATION_BY_USB		// usb or uart 					
	//#define  CFG_COMMUNICATION_BY_UART		// usb or uart 				
	
	#define	 CFG_UART_COMMUNICATION_TX_PIN					GPIOA10
	#define  CFG_UART_COMMUNICATION_TX_PIN_MUX_SEL			(3)
	#define  CFG_UART_COMMUNICATION_RX_PIN					GPIOA9
	#define  CFG_UART_COMMUNICATION_RX_PIN_MUX_SEL			(1)
	
	#define  CFG_COMMUNICATION_CRYPTO						(0)////����ͨѶ����=1 ����ͨѶ������=0
	#define  CFG_COMMUNICATION_PASSWORD                     0x11223344//////���ֽڵĳ�������
#endif

//****************************************************************************************
//     ��Ƶ������ò���
//****************************************************************************************
#define	CFG_PARA_SAMPLE_RATE				(44100)//(48000)//
#define CFG_BTHF_PARA_SAMPLE_RATE			(16000)//����ģʽ��ͳһ������Ϊ16K
#define	CFG_PARA_SAMPLES_PER_FRAME          (512)//(512)	//ϵͳĬ�ϵ�֡����С
#define	CFG_BTHF_PARA_SAMPLES_PER_FRAME     (256)			//����ͨ��ģʽ��֡����С
#define CFG_PARA_MIN_SAMPLES_PER_FRAME		(128)//         //ϵͳ��С֡����С����֤mic delay��С
#define CFG_PARA_MAX_SAMPLES_PER_FRAME		(512)//(512)

#define CFG_PARA_MAX_VOLUME_NUM		        (32)	//SDK 32 ������
#define CFG_PARA_SYS_VOLUME_DEFAULT			(25)	//SDKĬ��������Source�ˣ�Sink����25



//****************************************************************************************
//     ת��������ѡ��
//˵��:
//    1.ʹ�ܸú��ʾϵͳ�Ὣ����ͳһ�Ĳ����������Ĭ��ʹ��44.1KHz;
//    2.�˰汾Ĭ�ϴ򿪣�����ر�!!!!!!!!!!
//****************************************************************************************	
#define	CFG_FUNC_MIXER_SRC_EN

//****************************************************************************************
//     ������΢������ѡ��
//˵��:
//    ����첽��Դ����������ˮλ����Ӳ����΢��ϵͳ��Ƶʱ�ӣ���Ƶ��,������Դ(ʱ��)
//****************************************************************************************	
#define	CFG_FUNC_FREQ_ADJUST

//****************************************************************************************
//                 ¼����������
//˵��:
//    1.¼��֧��2�������һ��д�뵽SD��/U�̣�����д�뵽Flash��
//    2.����2���������ͬʱ֧�֣�������ʱע��һ��
//****************************************************************************************
//#define CFG_FUNC_RECORDER_EN
#ifdef CFG_FUNC_RECORDER_EN
	#define CFG_FUNC_RECORD_SD_UDISK	//¼����SD������U��
	//#define CFG_FUNC_RECORD_FLASHFS 	//����ͬʱ���� CFG_FUNC_RECORD_SD_UDISK
	
	#ifdef CFG_FUNC_RECORD_SD_UDISK
		#define CFG_FUNC_RECORD_UDISK_FIRST				//U�̺Ϳ�ͬʱ����ʱ��¼���豸����ѡ��U�̣���������ѡ��¼����SD����
		#define CFG_PARA_RECORDS_FOLDER 		"REC"	//¼��¼U��ʱ��Ŀ¼�ļ��С�ע��ffpresearch_init ʹ�ûص����������ַ�����
		#define CFG_FUNC_RECORDS_MIN_TIME		1000	//��λms�������˺��С��������ȵ��Զ�ɾ����
	#endif
	#define DEL_REC_FILE_EN
	/*ע��flash�ռ䣬�����ͻ   middleware/flashfs/file.h FLASH_BASE*/
	#ifdef CFG_FUNC_RECORD_FLASHFS
		#define CFG_PARA_FLASHFS_FILE_NAME		"REC.MP3"//RECORDER_FILE_NAME
	#endif

	#define MEDIA_RECORDER_CHANNEL				2
	#define MEDIA_RECORDER_BITRATE				96
#endif //CFG_FUNC_RECORDER_EN

//****************************************************************************************
//                 U�̻�SD��ģʽ��ع�������
//    
//****************************************************************************************
#if(defined(CFG_APP_USB_PLAY_MODE_EN) || defined(CFG_APP_CARD_PLAY_MODE_EN))
/**LRC��ʹ��� **/
#define CFG_FUNC_LRC_EN			 	// LRC����ļ�����


/**�ַ���������ת�� **/
/**Ŀǰ֧��Unicode     ==> Simplified Chinese (DBCS)**/
/**�ַ�ת������fatfs�ṩ������Ҫ�����ļ�ϵͳ**/
/**���֧��ת���������ԣ���Ҫ�޸�fatfs���ñ�**/
#define CFG_FUNC_STRING_CONVERT_EN	// ֧���ַ�����ת��


/**����Ȳ����ܣ�������رպ���Ȳ���**/
//#define CFG_FUNC_APP_MODE_AUTO

/**ȡ��AA55�ж�**/
/*fatfs�ڴ���ϵͳMBR��DBR������β�д˱�Ǽ�⣬Ϊ��߷Ǳ������̼����ԣ��ɿ�������, Ϊ��Ч������Ч�̣�����Ĭ�Ϲر�*/
//#define	CANCEL_COMMON_SIGNATURE_JUDGMENT

#endif

//****************************************************************************************
//           TF����USB�豸��⹦��
//****************************************************************************************
#if (defined(CFG_APP_CARD_PLAY_MODE_EN) )|| defined(CFG_FUNC_RECORD_SD_UDISK)
#define	CFG_RES_CARD_USE  //����SD��·
#ifdef CFG_RES_CARD_USE
#include "sd_card.h"

#define	CFG_RES_CARD_GPIO				SDIO_A20_A21_A22//SDIO_A15_A16_A17 //ע�������ļ������޸ġ�
#define	CFG_FUNC_CARD_DETECT //���sd�����뵯������·���
#endif //CFG_RES_CARD_USE
#endif

#if CFG_RES_CARD_GPIO == SDIO_A15_A16_A17
	#define SDIO_Clk_Disable				SDIO_ClkDisable
	#define SDIO_Clk_Eable					SDIO_ClkEnable
	#define CARD_DETECT_GPIO				GPIOA16
	#define CARD_DETECT_GPIO_IN				GPIO_A_IN
	#define CARD_DETECT_BIT_MASK			GPIOA16
	#define CARD_DETECT_GPIO_IE				GPIO_A_IE
	#define CARD_DETECT_GPIO_OE				GPIO_A_OE
	#define CARD_DETECT_GPIO_PU				GPIO_A_PU
	#define CARD_DETECT_GPIO_PD				GPIO_A_PD
	#define CARD_DETECT_GPIO_OUT			GPIO_A_OUT
	
#elif CFG_RES_CARD_GPIO == SDIO_A20_A21_A22
	#define SDIO_Clk_Disable				SDIO_ClkDisable
	#define SDIO_Clk_Eable					SDIO_ClkEnable
	#define CARD_DETECT_GPIO				GPIOA21
	#define CARD_DETECT_GPIO_IN				GPIO_A_IN
	#define CARD_DETECT_BIT_MASK			GPIOA21
	#define CARD_DETECT_GPIO_IE				GPIO_A_IE
	#define CARD_DETECT_GPIO_OE				GPIO_A_OE
	#define CARD_DETECT_GPIO_PU				GPIO_A_PU
	#define CARD_DETECT_GPIO_PD				GPIO_A_PD

#endif

/**USB Host��⹦��**/
#if(defined(CFG_APP_USB_PLAY_MODE_EN) || defined(CFG_FUNC_RECORD_SD_UDISK))
#define CFG_RES_UDISK_USE
#define CFG_FUNC_UDISK_DETECT
#endif

/**USB Device��⹦��**/
#if (defined (CFG_APP_USB_AUDIO_MODE_EN)) || (defined(CFG_COMMUNICATION_BY_USB))
	#define CFG_FUNC_USB_DEVICE_EN
#endif


//****************************************************************************************
//                 BT��������
//˵��:
//    1.������ع�������ϸ������bt_config.h�ж���!!!!!!!!!!
//    
//****************************************************************************************
#ifdef CFG_APP_BT_MODE_EN
#include "bt_config.h"

/*ĳЩ����:�ڷ�����ģʽ��,�������ͷ�Э��ջ/EM/BB��ص���Դ*/
/*�翪����HFP,��ÿ���������̨���й���*/
#define CFG_BT_BACKGROUND_RUN_EN /*������̨���п���*/

//-------------------------------//
//       AI����ѡ��
//˵��:
//1����Ҫ����СQͬѧ
//2. CFG_RES_MIC_SELECT����Ϊ0
//3. ������Ϣ��Ҫ��СQͬѧ�������ṩ
//     const char *APP_ID = "";
//     const char *SECRET = "";
//4. AI��Ҫͬʱ����BLE��SPPЭ��
//-------------------------------//
#if ((BLE_SUPPORT == ENABLE) && (BT_SPP_SUPPORT == ENABLE))
#define CFG_FUNC_AI
#endif

/**BB EM�������� **/
//ע����Ҫ���MPUConfigTable��SRAM_END_ADDRʹ��
//����OS��ص�RAM������ַ�Ѿ���BB_MPU_START_ADDR����
//��Ӧ��ϵ 0x40:20010000; 0x80:20020000; 0xc0:20030000 ;0x100: 0x20040000; 0x120:0x20048000; 0x130:0x2004c000
//default: addr: 0x20040000;  size: 0x10000;  em_start_addr: 0x100
//EM_SIZE˵��:ֻʹ��A2DP+AVRCP+SPP+BLE,EM_SIZE����Ϊ16K����������;
//ʹ��HFP+A2DP+AVRCP+SPP+BLE,EM_SIZE������ҪΪ32K;
#define BB_EM_MAP_ADDR			0x80000000
#define BB_EM_START_PARAMS		0x120//0x130//
#define BB_EM_SIZE				(32*1024)//(16*1024)
#define BB_MPU_START_ADDR		(0x20050000 - BB_EM_SIZE)
#else
#define BB_EM_MAP_ADDR			0x80000000
#define BB_EM_START_PARAMS		0
#define BB_EM_SIZE				0
#define BB_MPU_START_ADDR		(0x20050000 - BB_EM_SIZE)
#endif

//****************************************************************************************
//                            HDMI����������ò���
//						CEC�ܽŶ�ARC�ܽ���Ӱ�죬��Ҫע��PCB����
//****************************************************************************************
#ifdef  CFG_APP_HDMIIN_MODE_EN
	/**ARC**/
	#define HDMI_ARC_RECV_IO_OE				GPIO_A_OE
	#define HDMI_ARC_RECV_IO_IE				GPIO_A_IE
	#define HDMI_ARC_RECV_IO_ANA			GPIO_A_ANA_EN
	#define HDMI_ARC_RECV_IO_PIN			GPIOA29
	#define HDMI_ARC_RECV_ANA_PIN           SPDIF_ANA_INPUT_A29

	/**CEC**/
	//timer3����ʹ��rcʱ�ӻ���ϵͳʱ�ӣ�
	//PWC���Ը�������GPIO�ڣ���PWMֻ��ָ��GPIO�ڿ����ã����Զ��߽ӿڲ����÷���һ����CEC����IO��������RECV_IO��RECV_IO_PIN
	#define HDMI_CEC_CLK_MODE				RC_CLK_MODE//SYSTEM_CLK_MODE//RC_CLK_MODE /*����cec���ѹ���ʱ��ֻ����rc*/
	#define HDMI_CEC_IO_TYPE				IO_TYPE_A
	#define HDMI_CEC_IO_INDEX				27
	//#define HDMI_CEC_RECV_IO	            28
	//#define HDMI_CEC_RECV_IO_PIN	    	28
	#define HDMI_CEC_SEND_IO	            TIMER3_PWM_A0_A8_A22_A27 /*dma table���*/
	#define HDMI_CEC_SEND_IO_PIN			(3)
    #define HDMI_CEC_RECV_DATA_ADDR	    	TIMER3_PWC_DATA_ADDR
    #define HDMI_CEC_SEND_DATA_ADDR         TIMER3_PWM_DUTY_ADDR


	#define CFG_PARA_WAKEUP_GPIO_CEC		WAKEUP_GPIOA27

	#define	TIMER3_PWC_DATA_ADDR			(0x4002C034)
	#define TIMER3_PWM_DUTY_ADDR			(0x4002C024)

	#define	TIMER4_PWC_DATA_ADDR			(0x4002C834)
	#define TIMER4_PWM_DUTY_ADDR			(0x4002C824)

	#define	TIMER5_PWC_DATA_ADDR			(0x4002F034)
	#define TIMER5_PWM_DUTY_ADDR			(0x4002F024)

	#define	TIMER6_PWC_DATA_ADDR			(0x4002F834)
	#define TIMER6_PWM_DUTY_ADDR			(0x4002F824)

	#define HDMI_CEC_RECV_TIMER_ID	        TIMER3
	#define HDMI_CEC_RECV_DMA_ID	        PERIPHERAL_ID_TIMER3
	#define HDMI_CEC_RECV_DMA_ADDR	        TIMER3_PWC_DATA_ADDR

	#define HDMI_CEC_SEND_TIMER_ID	        TIMER3
	#define HDMI_CEC_SEND_DMA_ID	        PERIPHERAL_ID_TIMER3
	#define HDMI_CEC_SEND_DMA_ADDR	        TIMER3_PWM_DUTY_ADDR

	#define HDMI_CEC_ARBITRATION_TIMER_ID   TIMER5
	#define HDMI_CEC_ARBITRATION_TIMER_IRQ  Timer5_IRQn
	#define HDMI_CEC_ARBITRATION_TIMER_FUNC Timer5Interrupt

	/**HPD**/
	#define HDMI_HPD_CHECK_STATUS_IO	   	GPIO_A_IN
	#define HDMI_HPD_CHECK_INI_IO		   	GPIO_A_INT
	#define HDMI_HPD_CHECK_STATUS_IO_PIN   	GPIOA24
	#define HDMI_HPD_CHECK_IO_INIT()	   	GPIO_RegOneBitClear(GPIO_A_PU, GPIOA24),\
										   	GPIO_RegOneBitSet(GPIO_A_PD, GPIOA24),\
										   	GPIO_RegOneBitClear(GPIO_A_OE, GPIOA24),\
										   	GPIO_RegOneBitSet(GPIO_A_IE, GPIOA24),\
										   	GPIO_INTEnable(GPIO_A_INT, GPIOA24, GPIO_EDGE_TRIGGER),\
										   	GPIO_INTFlagClear(GPIO_A_INT, GPIOA24)

	/**DDC**/
	#define DDC_USE_SW_I2C
	#ifdef DDC_USE_SW_I2C
		#define DDCSclPortSel               PORT_B
		#define SclIndex                    (5)
		#define DDCSdaPortSel               PORT_B
		#define SdaIndex                    (4)
	#else
	   #define HDMI_DDC_DATA_IO_INIT()		GPIO_RegOneBitSet(GPIO_B_PU, GPIOB3),\
										    GPIO_RegOneBitClear(GPIO_B_PD, GPIOB3)
	   #define HDMI_DDC_CLK_IO_INIT()		GPIO_RegOneBitSet(GPIO_B_PU, GPIOB2),\
										    GPIO_RegOneBitClear(GPIO_B_PD, GPIOB2)
	   #define HDMI_DDC_IO_PIN				I2C_PORT_B2_B3
    #endif
#endif

//****************************************************************************************
//						SPDIF����ͬ�Ṧ��������ò���
//****************************************************************************************
#if (defined (CFG_APP_OPTICAL_MODE_EN)) || (defined (CFG_APP_COAXIAL_MODE_EN))
	#define	CFG_FUNC_SPDIF_EN

	#ifdef CFG_APP_OPTICAL_MODE_EN

		//#define PORT_B_INPUT_DIGATAL
		#ifndef PORT_B_INPUT_DIGATAL
		#define SPDIF_OPTICAL_INDEX				GPIOA30
		//#define SPDIF_PORT_OE					GPIO_A_OE
		//#define SPDIF_PORT_IE					GPIO_A_IE
		#define SPDIF_OPTICAL_PORT_MODE			8
		#define SPDIF_OPTICAL_PORT_ANA_INPUT	SPDIF_ANA_INPUT_A30
		#else
		#define SPDIF_OPTICAL_INDEX				GPIOB1
		//#define SPDIF_PORT_OE					GPIO_A_OE
		//#define SPDIF_PORT_IE					GPIO_A_IE
		#define SPDIF_OPTICAL_PORT_MODE			1//(digital)
		//#define SPDIF_OPTICAL_PORT_ANA_INPUT	SPDIF_ANA_INPUT_A31
		#endif
	#endif
	#ifdef CFG_APP_COAXIAL_MODE_EN
		#define SPDIF_COAXIAL_INDEX				GPIOA31
		//#define SPDIF_PORT_OE					GPIO_A_OE
		//#define SPDIF_PORT_IE					GPIO_A_IE
		#define SPDIF_COAXIAL_PORT_MODE			8
		#define SPDIF_COAXIAL_PORT_ANA_INPUT	SPDIF_ANA_INPUT_A31
	#endif
#endif


/**OS����ϵͳ����IDLEʱ��core��������״̬���Դﵽ���͹���Ŀ��**/
/*ע�⣬����OS���ȵ�IDLE������Ӧ�ò�APPMODE��Ӧ�ò��������*/
#define CFG_FUNC_IDLE_TASK_LOW_POWER
#ifdef	CFG_FUNC_IDLE_TASK_LOW_POWER
	#define	CFG_GOTO_SLEEP_USE
#endif

//****************************************************************************************
//                 ������߹�������
//****************************************************************************************
//#define 	CFG_FUNC_MAIN_DEEPSLEEP_EN	//ϵͳ������˯�ߣ��Ȼ��ѡ�
#define	CFG_FUNC_DEEPSLEEP_EN //appģʽ����ʱ �ɷ���˯��/���ѡ�
#if defined(CFG_FUNC_DEEPSLEEP_EN) || defined(CFG_FUNC_MAIN_DEEPSLEEP_EN)
	/**����һ������Դ**/ //Source(ͨ��) �����ظ�,����IO�͹���IO������һ�¡� �μ�:deepsleep.c
	/*���ⰴ������,ע��CFG_PARA_WAKEUP_GPIO_IR�� ���Ѽ�IR_KEY_POWER����*/
//	#define CFG_PARA_WAKEUP_SOURCE_IR		SYSWAKEUP_SOURCE9_IR//�̶�source9
	/*ADCKey���� ���CFG_PARA_WAKEUP_GPIO_ADCKEY ���Ѽ�ADCKEYWAKEUP���ü����ƽ*/
	#define CFG_PARA_WAKEUP_SOURCE_ADCKEY	SYSWAKEUP_SOURCE1_GPIO//
	#define CFG_PARA_WAKEUP_SOURCE_CEC		SYSWAKEUP_SOURCE2_GPIO//HDMIר�ã�CFG_PARA_WAKEUP_GPIO_CEC
//	#define CFG_PARA_WAKEUP_SOURCE_RTC		SYSWAKEUP_SOURCE7_RTC//
	/*ע��:RTC����Ĭ������ʱ���ѣ�Ӱ��rtcģʽ�����õ����ӣ�����ָ��ʱ�̻��ѹ��ܣ���������һ�У���rtcmode�������ӡ�*/
	#define CFG_PARA_WAKEUP_TIME_RTC	25//˯��ʱ�� ��	-

//	#define CFG_FUNC_WAKEUP_MCU_RESET		//���Ѻ�mcu��λ
#endif



//****************************************************************************************
//                 SHELL��������
//˵��:
//    1.shell���������Լ������뵽shell.c������;
//    2.shell����Ĭ�Ϲرգ�Ĭ��ʹ��UART1;
//****************************************************************************************
//#define CFG_FUNC_SHELL_EN

//****************************************************************************************
//                 UART DEBUG��������
//ע�⣺DEBUG�򿪺󣬻�����micͨ·��delay������ҪDEBUG���Դ���ʱ������رյ���
//****************************************************************************************
#define CFG_FUNC_DEBUG_EN
#ifdef CFG_FUNC_DEBUG_EN
	//#define CFG_USE_SW_UART
	
	#ifdef CFG_USE_SW_UART
		#define SW_UART_IO_PORT				    SWUART_GPIO_PORT_B//SWUART_GPIO_PORT_B
		#define SW_UART_IO_PORT_PIN_INDEX	    6//bit num
		#define  CFG_UART_BANDRATE   			256000//DEBUG UART����������
	#else
		#ifdef CFG_COMMUNICATION_BY_UART
		#define CFG_UART_TX_PORT  				(1)//tx port  0--A6��1--A10, 2--A25
		#else
		#define CFG_UART_TX_PORT 				(0)//tx port  0--A6��1--A10, 2--A25
		#endif
	#endif

	#define  CFG_UART_BANDRATE   			256000//DEBUG UART����������
#endif


/**RTC ���ӹ��ܣ�Ĭ�ϲ�����**/
//#define CFG_FUNC_ALARM_EN


//****************************************************************************************
//                 ��ʾ����������
//˵��:
//    1.��¼���߲μ�MVs26_SDK\tools\script��
//    2.��ʾ�����ܿ�����ע��flash��const data��ʾ��������ҪԤ����¼�����򲻻Ქ��;
//    3.const data���ݿ�����飬Ӱ�쿪���ٶȣ���Ҫ������֤��
//****************************************************************************************
#define CFG_FUNC_REMIND_SOUND_EN
#ifdef CFG_FUNC_REMIND_SOUND_EN
	#define CFG_FUNC_REMIND_CHECK_EN
	//#define CFG_FUNC_REMIND_SBC
	#define CFG_PARAM_FIXED_REMIND_VOL    15//�̶���ʾ������ֵ,0��ʾ��music volͬ������
#endif


//****************************************************************************************
//                 �ϵ���书������        
//****************************************************************************************
#define CFG_FUNC_BREAKPOINT_EN
#ifdef CFG_FUNC_BREAKPOINT_EN
	#define BP_PART_SAVE_TO_NVM			// �ϵ���Ϣ���浽NVM
	#define BP_SAVE_TO_FLASH			// �ϵ���Ϣ���浽Flash

	#define FUNC_MATCH_PLAYER_BP		// ��ȡFSɨ����벥��ģʽ�ϵ���Ϣƥ����ļ����ļ��к�ID��
#endif

//****************************************************************************************
//                 PowerKey���ܣ�NVM�洢���͹���RTC��
//PowerKey���ؽ���POWERKEY_CNT: 2000��Ӳ���ؽ���POWERKEY_CNT < 100,��������Ϊ8
//****************************************************************************************
#define	CFG_FUNC_BACKUP_EN
#ifdef CFG_FUNC_BACKUP_EN
	#define	CFG_FUNC_POWERKEY_EN
	#define POWERKEY_MODE					POWERKEY_MODE_PUSH_BUTTON//POWERKEY_MODE_SLIDE_SWITCH_LPD
	#define POWERKEY_CNT					2000
#endif

//****************************************************************************************
//                 POWER MONITOR��������
// ������ܼ���(�����ڴ����ϵͳ)�Ĺ��ܺ��ѡ���
// ���ܼ��Ӱ�������ص�ѹ��⼰�͵�ѹ���ϵͳ��Ϊ�Լ����ָʾ��
// ��ص�ѹ��⣬��ָLDOIN����˵ĵ�ѹ��⹦��
// �ù��ܺ�򿪺�Ĭ�ϰ�����ص�ѹ��⹦��
//****************************************************************************************
//#define CFG_FUNC_POWER_MONITOR_EN
	//#define	CFG_FUNC_OPTION_CHARGER_DETECT		//�򿪸ú궨�壬֧��GPIO������豸���빦��
	//�����˿�����
	#define CHARGE_DETECT_PORT_PU			GPIO_A_PU
	#define CHARGE_DETECT_PORT_PD			GPIO_A_PD
	#define CHARGE_DETECT_PORT_IN			GPIO_A_IN
	#define CHARGE_DETECT_PORT_IE			GPIO_A_IE
	#define CHARGE_DETECT_PORT_OE			GPIO_A_OE
	#define CHARGE_DETECT_GPIO				GPIOA31


//****************************************************************************************
//                 RTC��������
//ע��:��ǰ������rtc��rcʱ���¾��ȵͣ����Ƽ���32K���񷽰�����оƬ��֧��;�Ƽ�12M����ģʽ��deepsleep��������̧��*/
//****************************************************************************************	
#if defined(CFG_PARA_WAKEUP_SOURCE_RTC) || defined(CFG_FUNC_ALARM_EN)
#define CFG_RES_RTC_EN
#endif


//****************************************************************************************
//                            Key �����������
//****************************************************************************************

/**����beep������**/
//#define  CFG_FUNC_BEEP_EN

/**����˫������**/
//#define  CFG_FUNC_DBCLICK_MSG_EN
#ifdef CFG_FUNC_DBCLICK_MSG_EN
	#define  CFG_PARA_CLICK_MSG             MSG_DEVICE_SERVICE_SWITCH_MODE
	#define  CFG_PARA_DBCLICK_MSG           MSG_DEVICE_SERVICE_EQ
#endif

/**ADC����**/
#define CFG_RES_ADC_KEY_SCAN				//��device service ������Keyɨ��ADCKEY
#if defined(CFG_RES_ADC_KEY_SCAN) || CFG_PARA_WAKEUP_SOURCE_ADCKEY
	#define CFG_RES_ADC_KEY_USE				//ADC�������� ����
#endif
#define CFG_PARA_ADC_KEY_COUNT				11  //key count per adc channel ��·���
#ifdef CFG_RES_ADC_KEY_USE

	#define CFG_RES_POWERKEY_ADC_EN         //power key����adc key����ʹ�ܣ���5��key

    //#define CFG_RES_ADC_KEY_PORT_CH1		ADC_CHANNEL_GPIOA20_A23
	#define CFG_RES_ADC_KEY_CH1_ANA_EN		GPIO_A_ANA_EN
	#define CFG_RES_ADC_KEY_CH1_ANA_MASK	GPIO_INDEX23
	#define CFG_PARA_WAKEUP_GPIO_ADCKEY		WAKEUP_GPIOA23 //ͬ�����û��Ѷ˿�

	//#define CFG_RES_ADC_KEY_PORT_CH2		ADC_CHANNEL_GPIOA26
	#define CFG_RES_ADC_KEY_CH2_ANA_EN		GPIO_A_ANA_EN
	#define CFG_RES_ADC_KEY_CH2_ANA_MASK	GPIO_INDEX26

#endif //CFG_RES_ADC_KEY_USE

/**IR����**/
//#define CFG_RES_IR_KEY_SCAN				//����device service Keyɨ��IRKey
#if defined(CFG_RES_IR_KEY_SCAN) || defined(CFG_PARA_WAKEUP_SOURCE_IR)
#define	CFG_RES_IR_KEY_USE

	#define CFG_RES_IR_NUMBERKEY //���ּ���������

	#define CFG_RES_IR_PIN                  IR_GPIOB6//IR_GPIOB6,IR_GPIOB7,IR_GPIOA29
	#define CFG_PARA_IR_SEL					IR_MODE_NEC
	#define CFG_PARA_IR_BIT					IR_NEC_32BITS
	#define IR_MANU_ID						0x7F80//0xFF00//��ͬң�����᲻ͬ,��ֵ��ir_key.c\gIrVal[]
//	#define	IR_KEY_TABLE	0xBA,/*POWER*/		0xB9,/*MODE*/		0xB8,/*MUTE*/	\
//							0xBB,/*PLAY/PAUSE*/	0xBF,/*PRE*/		0xBC,/*NEXT*/	\
//							0xF8,/*EQ*/			0xEA,/*VOL-*/		0xF6,/*VOL+*/	\
//							0xE9,/*0*/			0xE6,/*REPEAT*/		0xF2,/*SCN*/	\
//							0xF3,/*1*/			0xE7,/*2*/			0xA1,/*3*/		\
//							0xF7,/*4*/			0xE3,/*5*/			0xA5,/*6*/		\
//							0xBD,/*7*/			0xAD,/*8*/			0xB5/*9*/
	#define	IR_KEY_TABLE	0xED,/*POWER*/		0xE5,/*MODE*/		0xE1,/*MUTE*/	\
							0xFE,/*PLAY/PAUSE*/	0xFD,/*PRE*/		0xFC,/*NEXT*/	\
							0xFB,/*EQ*/			0xFA,/*VOL-*/		0xF9,/*VOL+*/	\
							0xF8,/*0*/			0xF7,/*REPEAT*/		0xF6,/*SCN*/	\
							0xF5,/*1*/			0xE4,/*2*/			0xE0,/*3*/		\
							0xF3,/*4*/			0xF2,/*5*/			0xF1,/*6*/		\
							0xFF,/*7*/			0xF0,/*8*/			0xE6/*9*/
#endif

/**������ť����**/
//#define	CFG_RES_CODE_KEY_USE
#ifdef CFG_RES_CODE_KEY_USE
	#define CFG_CODE_KEY1P_BANK				'A'
	#define CFG_CODE_KEY1P_PIN				(8)
	#define CFG_CODE_KEY1N_BANK				'A'
	#define CFG_CODE_KEY1N_PIN				(9)
#endif

/**GPIO����**/
//#define CFG_RES_IO_KEY_SCAN	
#ifdef CFG_RES_IO_KEY_SCAN
	#define  CFG_SOFT_POWER_KEY_EN                          //��ΧӲ������ʽ���غ꿪��,����������ͨGPIO KEY0ʹ�ã�    
    #define  CFG_GPIO_KEY1_EN                               //GPIO KEY1ʹ��
    #define  CFG_GPIO_KEY2_EN                               //GPIO KEY2ʹ��   
#endif

/**��λ������ѡ��**/
//#define CFG_ADC_LEVEL_KEY_EN 
#ifdef CFG_ADC_LEVEL_KEY_EN
    #define  ADCLEVL_CHANNEL_MAP            (ADC_GPIOA20|ADC_GPIOA21|ADC_GPIOA22)//ѡ��GPIOA20��GPIOA21,GPIOA22����3·ADC��λ��

	#define MAX_ADCLEVL_LEVEL_VAL 	        4096//��λ������ѹֵ:4096��Ӧ3.3v
	#define MAX_ADCLEVL_STEP_NUMBER 	    CFG_PARA_MAX_VOLUME_NUM//��λ���������������Χ:0-31
    #define DISTANCE_BETWEEN_STEP 		    5//��λ���˲�����ֵ://5//15//25 

#endif


//****************************************************************************************
//                            Display ��ʾ����
//****************************************************************************************

//#define  CFG_FUNC_DISPLAY_EN
#ifdef CFG_FUNC_DISPLAY_EN

//  #define  DISP_DEV_SLED
  #define  DISP_DEV_7_LED
/**8��LED��ʾ����**/
/*LED�Դ�ˢ����Ҫ��Timer1ms�жϽ��У���дflash����ʱ��ر��ж�*/
/*������Ҫ�����⴦�����ע�ú�����Ĵ����*/
/*ע��timer�жϷ������͵��õ���API�������TCM�������õ�����api���⺯������ѯ֧��*/
/*�����˺꣬Ҫ��ע����ʹ��NVIC_SetPriority ����Ϊ0�Ĵ��룬�����Ӧ�жϵ��÷�TCM��������������λ*/
#ifdef DISP_DEV_7_LED
  #define	CFG_FUNC_LED_REFRESH
#endif
#ifdef DISP_DEV_SLED
	#define DISP_LED_INIT()  	    GPIO_PortAModeSet(GPIOA10, 0),\
                                    GPIO_RegOneBitClear(GPIO_A_PU, GPIOA10),\
							        GPIO_RegOneBitClear(GPIO_A_PD, GPIOA10),\
							        GPIO_RegOneBitSet(GPIO_A_OE, GPIOA10),\
							        GPIO_RegOneBitClear(GPIO_A_IE, GPIOA10),\
									GPIO_RegOneBitSet(GPIO_A_OUT, GPIOA10)
   #define DISP_LED_ON()            GPIO_RegOneBitSet(GPIO_A_OUT, GPIOA10)
   #define DISP_LED_OFF()           GPIO_RegOneBitClear(GPIO_A_OUT, GPIOA10)
#endif

#if defined(DISP_DEV_SLED) && defined(DISP_DEV_7_LED)
   #error Conflict: display setting error //����ͬʱѡ��������ʾģʽ
#endif
#endif

//****************************************************************************************
//				   ������μ�⹦��ѡ������
//****************************************************************************************
//#define  CFG_FUNC_DETECT_PHONE_EN                            

//****************************************************************************************
//				   3�ߣ�4�߶������ͼ�⹦��ѡ������
//****************************************************************************************
//#define  CFG_FUNC_DETECT_MIC_SEG_EN  


//****************************************************************************************
//                            FLASH�������
// 2M��Сflash����ʾ����flash¼������ͬʱ���ڣ�flash¼���Ŀռ��С��flashfs������
//****************************************************************************************
/*���ע����ɨ���������⡣*/
#ifdef CFG_FUNC_RECORD_FLASHFS
#define CFG_RES_FLASHFS_EN
#endif

/*��ע��init-defalut.c��stub()��file.h��FLASH_BASE��FLASH_FS_SIZE ��������flash_config.h����*/

#define REMIND_FLASH_STORE_BASE				CONST_DATA_ADDR //ϵͳflash�ռ�������
/*//��ʾ�����ݿռ������ַ */
#ifdef CFG_RES_FLASHFS_EN
#define REMIND_FLASH_STORE_OVERFLOW			FLASHFS_ADDR //flashfs����ȱʡ���á�
#else
#define REMIND_FLASH_STORE_OVERFLOW			BP_DATA_ADDR
#endif                        


//****************************************************************************************
//                            ���ó�ͻ ���뾯��
//****************************************************************************************

//#if defined(CFG_PARA_SHELL_COM0) && defined(CFG_RES_BT_COM0)
//#error	Conflict: Com0  used by Bt / Shell.
//#endif

//#if defined(CFG_PARA_SHELL_COM1) && defined(CFG_PARA_AUDIO_EFFECT_ON_LINE_COM1)
//#error Conflict: Com1  used by effect / shell. //��ȷ�϶˿ڲ��ص�������
//#endif

#endif /* APP_CONFIG_H_ */
