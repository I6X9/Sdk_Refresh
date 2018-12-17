#ifndef _WIFI_INIT_SETTING_H_
#define _WIFI_INIT_SETTING_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "app_config.h"

//****************************************************************************************
//					WiFi��Ʒ����ض��Ʋ�������
//****************************************************************************************
//**MCU+CAP+PRJproject_name&:��Ŀ���ƶ���
#define MCU_CAP_PRJ_PROJECT_NAME	"RHYMEET_R1"

//**MCU+SID+name&:�����豸Ĭ��SSID����
#define MCU_SSID_NAME	"RHYMEET_R1"

//**MCU+PTV+nnn:ʹ�õ�MCU Э��İ汾������
#define MCU_PTV_NUM		002		//nnnΪ0~999�����֣���ʾʹ�õ�MCU Э��İ汾�ţ�Ĭ��000�������趨��capacity ��λ������������ϵ?
								//����MCU+CAP+001�ĵ�8λ��ʾģ���I2S master/slave ģʽ����MCUЭ��001�汾����֧�֣�����Ҫʹ������cap1 bit8�ͱ���Ҫ�趨��nnn >=001��֧�֡������������Ϊ��000�ˡ�

#ifdef FUNC_WIFI_ALI_PROJECT_EN
	//**MCU+ALI+PIDproductid&: ����С�ǵ���Ŀ����
	#define MCU_ALI_PID_PRODUCTID	"SUNCHAN_ENTERTAINMENT_ATALK_R1"	//�������һ������С�ǵ���Ŀ����ظ����productid���԰����ṩ��prodconf.json�ļ�����model�ֶΣ����磺EDIFIER_ENTERTAINMENT_ATALK_MA1

	//**MCU+ALV+nnn:����С�ǵ���Ŀ Э��İ汾������
	#define MCU_ALV_NUMBER 		001		//nnnΪ0~999�����֣���ʾʹ�õİ�����ĿЭ��汾�š�
#endif

//**MCU+SPY+NAMmodelname&:Spotify��֤������modename
#define MCU_SPY_NAM_MODE_NAME	"MV_WiFiDemo" //�����ҪSpotify��֤���ظ���������modename��������ظ�Ĭ����project_nameһ��

//**MCU+SPY+BRNbrandname&:Spotify��֤������brandname
#define	MCU_SPY_BRN_BRAND_NAME	"MV_WiFiDemo" //�����ҪSpotify��֤���ظ���������brandname��������ظ�Ĭ��Ĭ����project_nameһ��

//**MCU+SPY+MODmodedisplaylname&:Spotify��֤������modedisplayname
#define MCU_SPY_MOD_DISP_NAME	"MV_WiFiDemo" //�����ҪSpotify��֤���ظ���������modedisplayname��������ظ�Ĭ��Ĭ����modenameһ��

//**MCU+SPY+BRDbarnddisplayname&:Spotify��֤������branddisplayname
#define MCU_SPY_BRD_DISP_NAME	"MV_WiFiDemo" //�����ҪSpotify��֤���ظ���������branddisplayname��������ظ�Ĭ��Ĭ����brandnameһ��

//**MCU+SPY+TYPdevicetype&:Spotify��֤����������
#define	MCU_SPY_TYP_DEVICE_TYPE		1	//�����ҪSpotify��֤���ظ������������ͣ�����Ϊ1������Ϊ0��Ĭ��Ϊ1

//**MCU+CAP+001xxxxxxxx&:���ܿ��ر�������
#define MCU_CAP_001_BIT0	0	//����
#define MCU_CAP_001_BIT1	0	//�Ƿ��ֹ�������֣�Ĭ��0����ֹ
#define MCU_CAP_001_BIT2	0	//���������Ƿ�ʹ��user������Ƶ��Ĭ��0����ͳһΪ��User����ʹ��user�������򲥷�Ĭ������
#define MCU_CAP_001_BIT3	0	//�Ƿ��ֹ��ֹ������ʾ��Ĭ��0����ֹ
#define MCU_CAP_001_BIT4	0	//����
#define MCU_CAP_001_BIT5	0	//������ʾ�Ƿ�ʹ��user������Ƶ��Ĭ��0����ͳһΪ��User����ʹ��user�������򲥷�Ĭ��������
#define MCU_CAP_001_BIT6	0	//����U��/T���Ƿ�UDisk/T������Ƶ��Ĭ��0���Զ�
#define MCU_CAP_001_BIT7	0	//�����Զ�����U��/T���Ƿ��Զ����ţ�Ĭ��0���Զ�
#define MCU_CAP_001_BIT8	0	//ģ��I2S ģʽ��Ĭ��0 ��ģʽ��1 ��ģʽ��Ҫʹ����һλ�趨������Ҫͬʱ�趨MCU+PTV+nnn��nnn>=001 ����Ч��ֻ��A31/A28֧��������ܣ����ҹ̼���Ҫ�Ǳ�׼�̼���
#define MCU_CAP_001_BIT9	1	//�����Ƿ���MCU���ƣ�Ĭ����MCU��Ϊ1������Ϊ0,  ���������MCU���ƣ�������WiFi���ᷢ��������MCU��ֻ���ȡ, ���򣬿���ʱWIFi�ᷢ��������MCU
#define MCU_CAP_001_BIT10	0	//�����Ƿ��Զ����浽WiFi�ˣ�Ĭ����MCU��Ϊ0������Ϊ1
#define MCU_CAP_001_BIT11	0	//������Ϣmedadata�Ƿ�Ĭ����GBK���룬Ĭ��Ϊ0 UTF8���룬����Ϊ1 GBK����
#define MCU_CAP_001_BIT12	1	//���MCU�л�ģʽʱ����WiFi����AXX+PLM��ָ������1λ����λ1��Ĭ��Ϊ0
#define MCU_CAP_001_BIT13	0	//����
#define MCU_CAP_001_BIT14	0	//EQ�Ƿ���MCU���ƣ�Ĭ����MCU��Ϊ1������Ϊ0
#define MCU_CAP_001_BIT15	0	//EQ�Ƿ��Զ����棬Ĭ����MCU��Ϊ0������Ϊ1
#define MCU_CAP_001_BIT16	0	//MCU�Ƿ���RTC��Ĭ��Ϊ0��ʾû��
#define MCU_CAP_001_BIT17	0	//����
#define MCU_CAP_001_BIT18	0	//����
#define MCU_CAP_001_BIT19	0	//����
#define MCU_CAP_001_BIT20	0	//�����Ƿ���MCU���ƣ�Ĭ����MCU��Ϊ1������Ϊ0
#define MCU_CAP_001_BIT21	0	//�����Ƿ��Զ����棬����Ϊ0
#define MCU_CAP_001_BIT22	0	//����
#define MCU_CAP_001_BIT23	1	//WiFiģ���Ƿ��н�USB��˷磬Ĭ��Ϊû�н�0������Ϊ1��Ҫʹ����һλ�趨������Ҫͬʱ�趨MCU+PTV+nnn��nnn>=002 ����Ч��ֻ��A31/A28֧��������ܣ����ҹ̼���Ҫ�Ǳ�׼�̼���
#define MCU_CAP_001_BIT24	0	//WiFiģ���Ƿ��н�USB����ң������Ĭ��Ϊû�н�0������Ϊ1��Ҫʹ����һλ�趨������Ҫͬʱ�趨MCU+PTV+nnn��nnn>=002 ����Ч��ֻ��A31/A28֧��������ܣ����ҹ̼���Ҫ�Ǳ�׼�̼���
#define MCU_CAP_001_BIT25	0	//WiFiģ���Ƿ��MCU��ȡʱ�䣬Ĭ��Ϊ0��0��WiFiģ��ͨ��SNTP��ȡʱ�䣬1��WiFiģ���MCU����APP��ȡʱ��
#define MCU_CAP_001_BIT26	0	//�Ƿ�MONO�����Ĭ��0��˫����
#define MCU_CAP_001_BIT27	0	//�����찴ungroup����ֻ�Ǵ����������������䣬Ĭ��Ϊ0��0����ɢ�����飬1��ֻ�ǽ���������죡?
#define MCU_CAP_001_BIT28	0	//��group������ɢ���������䣬Ĭ��0��0����ɢ��·�����µ������飬1������ɢ������
#define MCU_CAP_001_BIT29	0	//����
#define MCU_CAP_001_BIT30	0	//����
#define MCU_CAP_001_BIT31	0	//����

//**MCU+CAP+002xxxxxxxx&:���ܿ��ر�������
#define MCU_CAP_002_BIT0	0	//����
#define MCU_CAP_002_BIT1	0	//����
#define MCU_CAP_002_BIT2	0	//����
#define MCU_CAP_002_BIT3	0	//����
#define MCU_CAP_002_BIT4	0	//����
#define MCU_CAP_002_BIT5	0	//����
#define MCU_CAP_002_BIT6	0	//����
#define MCU_CAP_002_BIT7	0	//�෿���Ƿ�����·����ת��������ע�������������������ͬһ������ʱ������Ҫֱ������Ĭ��0��֧��
#define MCU_CAP_002_BIT8	0	//�෿���Ƿ����·��������������ź��Զ��ж��Ƿ���Ҫֱ�������Զ��ж��������Ƿ���Ҫ���������䣩��Ĭ��0��֧�֣�Ŀǰ�������
#define MCU_CAP_002_BIT9	0	//�Ƿ�֧�ֿ������·����(��ͨ�������������豸��ȡ·�������ֺ����룬���û����������䣬����һ�������Ѿ�����·��������һ��������Բ���Ҫapp��������·�����ˡ�)��Ĭ��Ϊ0��֧��
#define MCU_CAP_002_BIT10	0	//�Ƿ�֧���Զ����루������·����ʱ���豸��������·���������룬ֱ���û��ֶ����ù̶����룩��Ĭ��Ϊ0��֧��
#define MCU_CAP_002_BIT11	1	//����·�������Ƿ��Զ�����AP��Ĭ��Ϊ0��������
#define MCU_CAP_002_BIT12	0	//USB�ǽ���MCU�ϻ���WiFi�ϣ�Ĭ��Ϊ0��ʾ��WiFi�ϻ���û��USB
#if (defined(FUNC_WIFI_PLAY_CARD_EN) || defined(FUNC_WIFI_PLAY_USB_EN))
#define MCU_CAP_002_BIT13	1	//�Ƿ���T��ģʽ��Ĭ��Ϊ0����ʾ����νT��ģʽ���Ǳ�ʾģʽ�л��費��Ҫ�ر���һ��T��ģʽ��ʵ����App����ֱ��ѡ��T�����������Դ󲿷�����²���Ҫѡ��
#else
#define MCU_CAP_002_BIT13	0
#endif
#define MCU_CAP_002_BIT14	1	//�Ƿ�������ʶ���ܣ�Ĭ��0�ޣ�����Ϊ1��ʾ��
#define MCU_CAP_002_BIT15	0	//�Ƿ�ر�intercom���ܣ�Ĭ��0���رգ�����Ϊ1�ر�
#ifdef FUNC_WIFI_I2S_BYPASS_NEED_EN
#define MCU_CAP_002_BIT16	1
#else
#define MCU_CAP_002_BIT16	0	//�Ƿ��I2S Bypass Auto���ܣ�����򿪣�����Aux-In, BT���ⲿ����ģʽʱ�����û�������䣬���bypass��I2S in������ֱ��I2s out������������䣬��ر�bypass��WiFi��I2S in������ת���������䣨������1.5��Ļ��壩��
#endif
#define MCU_CAP_002_BIT17	0	//�Ƿ�ر�multiroom�����书�ܣ�Ĭ��0���ر�
#define MCU_CAP_002_BIT18	0	//MCU�Ƿ�֧��ͬʱ���ź�¼������֧��Ϊ1��Ĭ��0֧��
#define MCU_CAP_002_BIT19	1	//�Ƿ����ⲿ��Դ���루AUX/BT...) ������У�����Ҫ�ظ�MCU+CAP+PLM
#define MCU_CAP_002_BIT20	0	//����
#ifdef FUNC_WIFI_SUPPORT_RTC_EN
#define MCU_CAP_002_BIT21	1
#else
#define MCU_CAP_002_BIT21	0	//��������֧�֣�Ĭ��Ϊ0����֧��
#endif
#define MCU_CAP_002_BIT22	0	//���޶�ʱֹͣ���Ź��ܣ�Ĭ��Ϊ0��û��
#define MCU_CAP_002_BIT23	0	//�����ⲿ��ʱ�ػ����ܣ�Ĭ��Ϊ0 �����֧�֣�WiFi���ڶ�ʱ�㷢��AXX+POW+OFF��MCU��MCU�ɻظ�MCU+POW+OFF��WiFiȷ�ϣ����WiFi��Ȼ�ظ�AXX+POW+OFF����2�����޻ظ������ɹػ������WiFi�ظ�AXX+BURNING�����ɹػ���
#define MCU_CAP_002_BIT24	0	//����
#define MCU_CAP_002_BIT25	0	//����
#define MCU_CAP_002_BIT26	0	//����
#define MCU_CAP_002_BIT27	1	//MCU�Ƿ�֧����������Э�飬Ĭ��Ϊ0����֧��
#define MCU_CAP_002_BIT28	0	//�Ƿ������̫����Ĭ��Ϊ0�����ã�������ã���ʹ��������Ҳ������
#define MCU_CAP_002_BIT29	1	//�Ƿ��ֹWIFIģʽ�Զ����Ź��ܣ�Ĭ��Ϊ0����WIFIģʽ�Զ����ţ��Զ������ϴβ��ŵĸ赥��
#define MCU_CAP_002_BIT30	0	//����
#define MCU_CAP_002_BIT31	0	//����

//**MCU+CAP+LAUlanguage&:����������������
#if (defined(FUNC_WIFI_ALI_PROJECT_EN) || defined(FUNC_WIFI_DUMI_PROJECT_EN))
	#define MCU_CAP_LAU_ENGLISH		0	//Ӣ��
	#define MCU_CAP_LAU_CHINESE		1 //����
	#define MCU_CAP_LAU_FRENCH		0	//����
	#define MCU_CAP_LAU_PORTUGUESE	0	//������
	#define MCU_CAP_LAU_ITALIAN		0	//�����
	#define MCU_CAP_LAU_GERMANY		0	//����
	#define MCU_CAP_LAU_SPANISH		0	//������
	
#elif defined(FUNC_WIFI_ALEXA_PROJECT_EN)
	#define MCU_CAP_LAU_ENGLISH		1	//Ӣ��
	#define MCU_CAP_LAU_CHINESE		0 //����
	#define MCU_CAP_LAU_FRENCH		0	//����
	#define MCU_CAP_LAU_PORTUGUESE	0	//������
	#define MCU_CAP_LAU_ITALIAN		0	//�����
	#define MCU_CAP_LAU_GERMANY		0	//����
	#define MCU_CAP_LAU_SPANISH		0	//������
#endif

//**MCU+CAP+STMxxxxxxxx&:��Դ���ر�������
#if (defined(FUNC_WIFI_ALI_PROJECT_EN) || defined(FUNC_WIFI_DUMI_PROJECT_EN))
	#define MCU_CAP_STM_BIT0	0	//����MFI airplay��Ĭ��0
	#define MCU_CAP_STM_BIT1	1	//����Airplay��Ĭ��1��������λ1ͬʱΪ1
	#define MCU_CAP_STM_BIT2	1	//����DLNA��Ĭ��1
	#define MCU_CAP_STM_BIT3	1	//����QPlay��Ĭ��1
	#define MCU_CAP_STM_BIT4	0	//����
	#define MCU_CAP_STM_BIT5	1	//������DMS֧�֣�Ĭ��1
	#define MCU_CAP_STM_BIT6	0	//����
	#define MCU_CAP_STM_BIT7	0	//����
	#define MCU_CAP_STM_BIT8	0	//����
	#define MCU_CAP_STM_BIT9	0	//����
	#define MCU_CAP_STM_BIT10	1	//���춯����Ĭ��1
	#define MCU_CAP_STM_BIT11	1	//����FM��Ĭ��1
	#define MCU_CAP_STM_BIT12	0	//����
	#define MCU_CAP_STM_BIT13	0	//����
	#define MCU_CAP_STM_BIT14	1	//���ѣ�Ĭ��1
	#define MCU_CAP_STM_BIT15	1	//ϲ�����ţ�Ĭ��1
	#define MCU_CAP_STM_BIT16	0	//TuneIn�������̨����Ĭ��1
	#define MCU_CAP_STM_BIT17	0	//iHeartRadio��Ĭ��1
	#define MCU_CAP_STM_BIT18	0	//Tidal��Ĭ��1
	#define MCU_CAP_STM_BIT19	0	//����
	#define MCU_CAP_STM_BIT20	0	//����
	#define MCU_CAP_STM_BIT21	0	//Pandora��Ĭ��1
	#define MCU_CAP_STM_BIT22	0	//Spotify��Ĭ��1
	#define MCU_CAP_STM_BIT23	0	//RHAPSODY,Ĭ��1
	#define MCU_CAP_STM_BIT24	0	// QQFM
	#define MCU_CAP_STM_BIT25	0	// VTUNER
	#define MCU_CAP_STM_BIT26	0	//ALEXA
	#define MCU_CAP_STM_BIT27	0	//����
	#define MCU_CAP_STM_BIT28	0	//����
	#define MCU_CAP_STM_BIT29	0	//����
	#define MCU_CAP_STM_BIT30	0	//����
	#define MCU_CAP_STM_BIT31	0	//����

#elif defined(FUNC_WIFI_ALEXA_PROJECT_EN)
	#define MCU_CAP_STM_BIT0	0	//����MFI airplay��Ĭ��0
	#define MCU_CAP_STM_BIT1	1	//����Airplay��Ĭ��1��������λ1ͬʱΪ1
	#define MCU_CAP_STM_BIT2	1	//����DLNA��Ĭ��1
	#define MCU_CAP_STM_BIT3	1	//����QPlay��Ĭ��1
	#define MCU_CAP_STM_BIT4	0	//����
	#define MCU_CAP_STM_BIT5	1	//������DMS֧�֣�Ĭ��1
	#define MCU_CAP_STM_BIT6	0	//����
	#define MCU_CAP_STM_BIT7	0	//����
	#define MCU_CAP_STM_BIT8	0	//����
	#define MCU_CAP_STM_BIT9	0	//����
	#define MCU_CAP_STM_BIT10	0	//���춯����Ĭ��1
	#define MCU_CAP_STM_BIT11	0	//����FM��Ĭ��1
	#define MCU_CAP_STM_BIT12	0	//����
	#define MCU_CAP_STM_BIT13	0	//����
	#define MCU_CAP_STM_BIT14	0	//���ѣ�Ĭ��1
	#define MCU_CAP_STM_BIT15	0	//ϲ�����ţ�Ĭ��1
	#define MCU_CAP_STM_BIT16	1	//TuneIn�������̨����Ĭ��1
	#define MCU_CAP_STM_BIT17	1	//iHeartRadio��Ĭ��1
	#define MCU_CAP_STM_BIT18	1	//Tidal��Ĭ��1
	#define MCU_CAP_STM_BIT19	0	//����
	#define MCU_CAP_STM_BIT20	0	//����
	#define MCU_CAP_STM_BIT21	1	//Pandora��Ĭ��1
	#define MCU_CAP_STM_BIT22	1	//Spotify��Ĭ��1
	#define MCU_CAP_STM_BIT23	1	//RHAPSODY,Ĭ��1
	#define MCU_CAP_STM_BIT24	0	// QQFM
	#define MCU_CAP_STM_BIT25	0	// VTUNER
	#define MCU_CAP_STM_BIT26	1	//ALEXA
	#define MCU_CAP_STM_BIT27	0	//����
	#define MCU_CAP_STM_BIT28	0	//����
	#define MCU_CAP_STM_BIT29	0	//����
	#define MCU_CAP_STM_BIT30	0	//����
	#define MCU_CAP_STM_BIT31	0	//����
#endif

//**MCU+CAP+PLMxxxxxxxx&:ģʽ���ر�������
#define MCU_CAP_PLM_BIT0	0	//����
#define MCU_CAP_PLM_BIT1	1	//����Aux-In��Ĭ��0�����App����Ҫ��ʾ��ǰ����Aux-In״̬�������л���Aux״̬WiFi�Ѿ��رգ���˴�����Ҫ�ظ�1��
#define MCU_CAP_PLM_BIT2	1	//����������Ĭ��0�����App����Ҫ��ʾ��ǰ��������״̬�������л�������״̬WiFi�Ѿ��رգ���˴�����Ҫ�ظ�1��
#define MCU_CAP_PLM_BIT3	0	//�����ⲿ�忨���룬Ĭ��0�����ⲿ�忨��������оƬ�ϲ��ǽ���WiFiģ���ϣ�
#define MCU_CAP_PLM_BIT4	0	//���޹������룬Ĭ��0
#define MCU_CAP_PLM_BIT5	0	//����RCA���룬APP��δ֧��
#define MCU_CAP_PLM_BIT6	0	//����ͬ�����룬APP��δ֧��
#define MCU_CAP_PLM_BIT7	0	//����FM����
#define MCU_CAP_PLM_BIT8	0	//���޵ڶ���AUX IN
#define MCU_CAP_PLM_BIT9	0	//����
#define MCU_CAP_PLM_BIT10	0	//����
#define MCU_CAP_PLM_BIT11	0	//����
#define MCU_CAP_PLM_BIT12	0	//����
#define MCU_CAP_PLM_BIT13	0	//����
#define MCU_CAP_PLM_BIT14	0	//����
#define MCU_CAP_PLM_BIT15	0	//����
#define MCU_CAP_PLM_BIT16	0	//����
#define MCU_CAP_PLM_BIT17	0	//����
#define MCU_CAP_PLM_BIT18	0	//����
#define MCU_CAP_PLM_BIT19	0	//����
#define MCU_CAP_PLM_BIT20	0	//����
#define MCU_CAP_PLM_BIT21	0	//����
#define MCU_CAP_PLM_BIT22	0	//����
#define MCU_CAP_PLM_BIT23	0	//����
#define MCU_CAP_PLM_BIT24	0	//����
#define MCU_CAP_PLM_BIT25	0	//����
#define MCU_CAP_PLM_BIT26	0	//����
#define MCU_CAP_PLM_BIT27	0	//����
#define MCU_CAP_PLM_BIT28	0	//����
#define MCU_CAP_PLM_BIT29	0	//����
#define MCU_CAP_PLM_BIT30	0	//����
#define MCU_CAP_PLM_BIT31	0	//����

//MCU+PARAMS+m:nnnn&:����MCU������ʽ��ÿ�η����������ĳ���
#define MCU_PARAMS_FOR    0      //Ϊ0��ʾʹ��escape��ʽ����Ĭ�Ϸ�ʽ��Ϊ1��ʾʹ��HEX���루����Ϊ������
#define MCU_PARAMS_VAL    1024 //ÿ�θ�MCU���������������ݳ��ȣ�Ĭ��Ϊ1024��ͨ�����ֵ������ΪMCU ���ݻ��������ȵ�һ�룬����Ͳ���С��128������ܴ���4096��������16�ı�����

//**MCU+PRESET+n&������Ԥ�ð�������
#define MCU_PRESET_VAL		6	//����Ԥ�ð������������Ϊ10����Ϊ����ǰ���ݣ�0��ʾĬ��6��Ԥ�ã����ظ�Ĭ��Ϊ0����-1��ʾû��Ԥ�ù��ܡ�ע��nǰ���и�+�����ָ����������������Ҳ��Ч��

//MCU+CAP+PWDpassword&: ����WiFi AP����
#define MCU_CAP_PWD       ""     //����Ϊ���ģ���Ҫ���������ַ�����MCUЭ���ͻ������8-15�ֽڣ������������������������Ч��

//**MCU+VMX+nnn:������ʾ����������������
#define MCU_VMX_VAL		80	       //nnnΪ0~100�����֣���001, 022, 100��������ʾ��������������һ����ǰ������������������������������������ֵ����<10���ߴ���90��Ч��

//MCU+FMT+nnn:����WiFiģ��IIS���ݸ�ʽ��
#define MCU_FMT_VAL   000     //nnnΪ000�趨��׼��ʽ��nnn=001Ϊ�趨Left justified��ʽ��Ĭ��000����׼IIS��ʽ��
#ifdef __cplusplus
}
#endif//__cplusplus

#endif


