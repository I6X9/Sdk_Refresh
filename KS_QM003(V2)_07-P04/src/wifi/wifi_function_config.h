#ifndef _WIFI_FUNCTION_CONFIG_H_
#define _WIFI_FUNCTION_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

//****************************************************************************************
//					WiFi ģ�鹦��ģ��ѡ������
//****************************************************************************************
#define FUNC_WIFI_EN   //�򿪴˺궨�壬֧�� WiFi ����

#ifdef FUNC_WIFI_EN
//************************************************
//	���º궨����뱣�ִ�
//************************************************
#define FUNC_OUTPUT_CHANNEL_AUTO_CHANGE		//�򿪴˺궨�壬֧�����ģʽ�Զ��л����ܿ���

//************************************************
//	���º궨����Ը��ݹ�����Ҫ����
//************************************************
#define FUNC_WIFI_POWER_KEEP_ON						//�򿪴˺궨�壬WiFiģ�鿪�����Դһֱ����
#define FUNC_WIFI_PLAY_CARD_EN						//�򿪴˺궨�壬֧�ֿ��˿ڽ���WiFi�˲���
//#define FUNC_WIFI_PLAY_USB_EN						//�򿪴˺궨�壬USB�˿ڽ���WiFi�˲���
//#define FUNC_WIFI_BT_CONTROL_EN						//�򿪴˺궨�壬WiFi������������
#define FUNC_WIFI_UART_UPGRADE						//ѡ��ͨ��WiFi ��������MCU�������
#define FUNC_WIFI_BATTERY_POWER_CHECK_EN			//�򿪴˺궨�壬֧�ֵ�ص�����⹦��
#define FUNC_WIFI_MIC_TALK_EN						//�򿪴˺궨�壬֧������ʶ����
#ifdef FUNC_WIFI_MIC_TALK_EN
	#define FUNC_WIFI_TALK_AND_AUDIO_EFFECT_EN			//�򿪴˺궨�壬WiFi��Mic��������ͬʱ��Ч��I2S In��ԴҲ��Ч
//	#define FUNC_KEEP_ON_MIC_IN_I2S_OUT_EN				//�򿪴˺궨�壬��WiFiģʽ����ʱ����ʵ���������ܲ���Ҫ����������I2S In�����ź�û��VB/3D����Ч����
	#define FUNC_WIFI_TALK_QUICK_OPEN_MIC_EN 			//�򿪴˺궨�壬����ʵ��WiFi talk���ٴ򿪹ر�MICͨ��,�Ӷ�ʵ��ͬʱ¼�Ź���
#endif
//#define FUNC_WIFI_MULTIROOM_PLAY_EN					//�򿪴˺궨�壬֧�ֶ෿�䲥�Ź��ܣ�������Դͨ��ͨ��I2S �����WiFi
#define FUNC_WIFI_SUPPORT_RTC_EN					//�򿪴˺궨�壬֧��RTC����
#define FUNC_WIFI_SUPPORT_ALARM_EN				//�򿪴˺궨�壬֧�����ӹ���
//#define FUNC_WIFI_ALI_PROJECT_EN	 				//�򿪴˺궨�壬֧�ְ�����Ŀ���⹦��
//#define FUNC_WIFI_ALEXA_PROJECT_EN	 			//�򿪴˺궨�壬֧������ѷ��Ŀ
#define FUNC_WIFI_DUMI_PROJECT_EN	 				//�򿪴˺궨�壬֧�ְٶȶ�����Ŀ
//#define FUNC_WIFI_SPOTIFY_NEED_EN 					//�򿪴˺궨�壬֧��Spotify��֤
//#define FUNC_WIFI_I2S_BYPASS_NEED_EN 				//�򿪴˺궨�壬֧��I2SBypassģʽ
#define FUNC_WIFI_PASS_THROUGH_DATA_EN				//�򿪴˺궨�壬֧��WiFi ͸�����ݹ���

#endif
#endif


