#ifndef _WIFI_MCU_COMMAND_H_
#define _WIFI_MCU_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

//****************************************************************************************
//					MCU �����
//****************************************************************************************

//******************************************************************
//	3 ��������
//******************************************************************
#define MCU_CAP_PRJ 	0x0301	//MCU�ظ���Ŀ��,�������ظ�����ʽҪ��Ϊ��branchname_modelname�������硰MUZO_W001��,�»���ǰΪƷ�������»��ߺ�Ϊ�ͺš�
#define MCU_PTV_XXX		0x0302	//nnnΪ0~999�����֣���ʾʹ�õ�MCU Э��İ汾�ţ�Ĭ��000�������趨��capacity ��λ������������ϵ��
#define MCU_ALI_PID		0x0303	//�������һ������С�ǵ���Ŀ����ظ����productid���԰����ṩ��prodconf.json�ļ�����model�ֶ�
#define MCU_ALV_NUM   	0x0304	//�������һ������С�ǵ���Ŀ����ظ����nnnΪ0~999�����֣���ʾʹ�ð���AlinkЭ��İ汾�ţ�Ĭ��000����2017��5���Ժ��¿��İ���������Ŀ�����趨nnn>=001
#define MCU_SPY_NAM 	0x0305	//�����ҪSpotify��֤���ظ���������modename��������ظ�Ĭ����project_nameһ��
#define MCU_SPY_BRN 	0x0306	//�����ҪSpotify��֤���ظ���������brandname��������ظ�Ĭ��Ĭ����project_nameһ��
#define MCU_SPY_MOD 	0x0307	//�����ҪSpotify��֤���ظ���������modedisplayname��������ظ�Ĭ��Ĭ����modenameһ��
#define MCU_SPY_BRD 	0x0308	//�����ҪSpotify��֤���ظ���������branddisplayname��������ظ�Ĭ��Ĭ����brandnameһ��
#define MCU_SPY_CID		0x0309	//�����ҪSpotify��֤���ظ���������client id, client idΪ32λ����spotify�ٷ�;��������
#define MCU_SPY_TYP 	0x030A	//�����ҪSpotify��֤���ظ�������������

#define MCU_CAP_001 	0x0320	//-MCUһЩ���ر������ã���16���Ʊ�ʾ��ǰ�治Ҫ��0x
#define MCU_CAP_002 	0x0321	//-MCUһЩ���ر������ã���16���Ʊ�ʾ��ǰ�治Ҫ��0x
#define MCU_CAP_LAU 	0x0322	//-MCUһ����Ĭ������
#define MCU_LAULIST		0x0323	//xxxx��ʾ32λ���ݣ���0λ��ʼ�����ָ��������ù̼�֧�ֵ����ԣ���ӦApp��������������ֻ����ʾMCUָ��֧�ֵ����ԡ�
#define MCU_CAP_STM 	0x0324	//-MCUһЩ���ر������ã���16���Ʊ�ʾ��ǰ�治Ҫ��0x
#define MCU_CAP_PLM 	0x0325	//-MCUһЩ���ر������ã���16���Ʊ�ʾ��ǰ�治Ҫ��0x
#define MCU_CAP_EXT 	0x0326	//ĿǰΪ0��ʾû�����õ�Ч�������ʾ�����õƿء�
#define MCU_PAR_XXX   	0x0327  //����MCU�����ļ��ĸ�ʽ�ͷ����ļ����ĳ��ȡ�
#define MCU_PRESETN 	0x0328	//����Ԥ�ð������������Ϊ10����Ϊ����ǰ���ݣ�0��ʾĬ��6��Ԥ�ã����ظ�Ĭ��Ϊ0����-1��ʾû��Ԥ�ù��ܡ�ע��nǰ���и�+�����ָ����������������Ҳ��Ч��
#define MCU_SET_PWD 	0x0329	//�����豸APģʽ��Ĭ�����룬����Ϊ���ģ���Ҫ���������ַ�������MCUЭ��Ķ����ͻ����������8�ֽڣ����15�ֽڡ����ָ������������������Ч��
#define MCU_VMX_XXX 	0x032A	//������ʾ��������������nnnΪ0~100�����֣���001, 022, 100��һ����ǰ������������������������������������ֵ����<10���ߴ���90��Ч��
#define MCU_FMT_XXX   	0x0315  //����WIFIģ��I2S�����ݸ�ʽ��nnn=000�趨��׼I2S��ʽ��nnn=001�趨Left Justified ��ʽ��Ĭ����000��׼I2S��ʽ��
#define MCU_SID_VAL 	0x0313	//���������豸Ĭ��SSID����, ������������������������ɺ����ظ����ָ������SSID���������ظ�

//******************************************************************
//	4 ��������/����/�ָ�����/���߻���/���
//******************************************************************
#define MCU_BOT_DON   	0x0401  //ģ�����յ�MCU+BOT+DONʱ��ظ�AXX+BOT+DON������������⣩
#define MCU_GET_UID 	0x0402	//��ȡ��ǰģ��̼��汾����Ŀuuid��ģ��ظ� AXX+UID+uuid&����Ŀ��uuid��8���ֽڵ�ascii�룬��FF��ͷ
#define MCU_GET_MAC 	0x0403	//��ȡ��ǰģ���MAC��ַ��ģ��ظ� AXX+MAC+%02X:%02X:%02X:%02X:%02X:%02X&
#define MCU_DEV_RST 	0x0404	//MCU��WiFiģ������
#define MCU_POW_OFF 	0x0405  //MCUҲ���Է���MCU+POW+OFF��ģ��ȥ�ر�һЩ���̣����WiFi�ظ�AXX+POW+OFF����2�����޻ظ���MCU���ɸ�ģ��ϵ磻���WiFi�ظ�AXX+BURNING�����ɹػ�
#define MCU_FACTORY 	0x0406	//MCUҲ���Է���ָ���ģ��ָ��������ã�MCU+FACTORY ���MCU����MCU+FCRYPOW����ģ��ָ������󲻻����������Ƿ���AXX+POW+OFF��MCU����ϵ硣
#define MCU_POW_SAV 	0x0407	//MCU ��WiFiģ���������״̬
#define MCU_POW_RES 	0x0408	//MCU ��WiFiģ�黽��
#define MCU_POW_STA 	0x0409	//��ȡWiFi ģ��״̬��ģ��ظ� AXX+POW+%03d���������ֿ���Ϊ��000�� ����  001�� ʡ��ģʽ(RF�ر�)    002���̼������� 003���豸������
#define MCU_BAT_ON  	0x040A	//MCU֪ͨģ�飬��ش��ڳ��״̬
#define MCU_BAT_OFF 	0x040B	//MCU֪ͨģ�飬��ش��ڷŵ�״̬
#define MCU_BAT_VAL 	0x040C	//MCU֪ͨģ���ص�����xxxΪ�ٷֱȣ�����001��ʾ1%��100��ʾ100%�����MCU��δ֪ͨģ���ص�������ģ����Ϊû�е�ش��ڡ�App��⵽����>0�ͻ���ʾ����
#define MCU_BAT_LOW 	0x040D	//MCU֪ͨģ���ص͵�
#define MCU_MCP_XXX 	0x040E	//MCU�յ�wifiģ���֪ͨ��, ���е�Դ�������, �����ò�������wifiģ��һ���µ�״̬,�������ֿ���Ϊ��000�� ����״̬001�� ����״̬800�� �л�״̬ (һ��MCU�����������ʹ�֪ͨ)

//******************************************************************
//	6  MCU��������
//******************************************************************
#define MCU_VER_NUM 	0x0601	//MCU���͵�ǰ�İ汾��,����version��һ��4λ���ֵİ汾�ţ�����0119
#define MCU_UPG_STA 	0x0602	//MCUͬ��������ʼ
#define MCU_UPG_RUN 	0x0603	//MCU�����ɹ�����
#define MCU_UPG_FAL 	0x0604	//MCU����ʧ����ֹ����
#define MCU_UPG_END 	0x0605	//MCU���������

//******************************************************************
//	7 ��������
//******************************************************************
//WPS����
#define MCU_WIF_WPS 	0x0701	//����WPS��������ָ�MCU����ģ�鿪ʼWPSɨ�裬ɨ��WPS Server������ʱ�����·��������WPS������������һ��ģ���յ�MCU+WIF+WSSָ����ģ�������֮��?
#define MCU_WIF_WSS 	0x0702	//MCU����ģ�����WPS Serverģʽ,WSSģʽ��WPSģʽ��������Ӧ��ģʽ��WPSģʽ���豸���Զ�����WSSģʽ���豸������Ҫ��������
#define MCU_WIF_EPS 	0x0703	//MCU����ģ���˳�WPS/WSS����ģʽ
#define MCU_WIF_WAC 	0x0704	//֪ͨWIFI����/�˳�WACģʽ��MCU+WIF+WAC:  ֪ͨWIFI����WACģʽ��MCU+WIF-WAC:   ֪ͨWIFI�˳�WACģʽ

//����Station
#define MCU_STA_ON  	0x0705 	//Enable WiFi Station��Ĭ�Ͽ�����enable�� 
#define MCU_STA_OFF 	0x0706	//�ر�WiFi Station��ģ���Ͽ���·�������������������
#define MCU_STA_DEL 	0x0707	//�Ͽ���ǰ·�������Ӳ�ɾ�����·������¼

//����AP
#define MCU_RA0_ON		0x0708	//��AP�ȵ㹦��
#define MCU_RA0_OFF 	0x0709	//����AP�ȵ㹦��

//����״̬֪ͨ
#define MCU_STA_GET 	0x070A	//- MCU֪ͨģ�鷢��·�����������
#define MCU_ETH_GET 	0x070B	//- MCU֪ͨģ�鷢���豸��̫���������
#define MCU_WWW_GET 	0x070C	//- MCU֪ͨģ�鷢���豸�������
#define MCU_RA0_GET 	0x070D	//- MCU֪ͨģ�鷢���豸���������

//******************************************************************
//	8 ��������
//******************************************************************
//���ſ���
#define MCU_PLY_PAU 	0x0801	//-	MCU����ģ�飬�����ͣ��ʼ���ţ��������������ͣ����
#define MCU_PLY_PLA		0x0802	//-	MCU����ģ��ָ����ţ����������ͣ״̬�����
#define MCU_PLY_PUS		0x0803	//-MCU����ģ����ͣ���ţ�������ǲ���״̬�����
#define MCU_PLY_STP		0x0804	//MCU����ģ��ֹͣ���ţ����ѵ�ǰ���Ÿ赥���
#define MCU_PLY_PUQ 	0x0805	//MCU����ģ�鲥���ϴεĸ赥������ǿ������򲥷��ϴιػ�ʱ���ڲ��ŵĸ赥
#define MCU_PLY_NXT		0x0806	//-	MCU����ģ�鲥����һ��
#define MCU_PLY_PRV		0x0807	//-	MCU����ģ�鲥����һ��

//��������
#define MCU_VOL_ADD 	0x0808	//-	MCU����ģ�����������xxxΪ0~100��ģ���ͨ��AXX+VOL+xxx����ʵ������
#define MCU_CUR_VOL 	0x0809	//-	MCU֪ͨģ�鵱ǰ������xxxΪ0~100��ע�⣺��-xxx����+xxx�����ʾģ��ֻ����µ�ǰ������ֵ������ʵ�ʵ���������Ҳ����ظ�MCU
#define MCU_VOL_GET 	0x080A	//-	MCU֪ͨģ�鷢��������ģ���ظ�AXX+VOL+xxx
#define MCU_MUT_SET 	0x080B	//-	MCU����ģ����������, xxxΪ0����1, ���MCU���п�����������ָ����Ч
#define MCU_MUT_GET 	0x080C	//-	MCU֪ͨģ�鷢�;���״̬��ģ���ظ�AXX+MUT+xxx
#define MCU_CUR_MUT 	0x080D	//-	���MCU���п���������MCU֪ͨģ�龲��״̬��xxxΪ0����1
#define MCU_CHN_XXX 	0x080E	//-	�����л�

//Ԥ�ø赥���л�
#define MCU_KEY_XNN 	0x080F	//MCU֪ͨģ���а����¼�����, nn����������ֵ (10���ƣ�01�C99), x��ʾ��Ϣ:  2 ������ 3���£�4�ɿ�; ����ֵΪ�̰�
#define MCU_KEY_NXT 	0x0810	// MCU֪ͨģ�鲥����һ������Ԥ�ø赥
#define MCU_KEY_PRE 	0x0811	// MCU֪ͨģ�鲥����һ������Ԥ�ø赥
#define MCU_PRE_SET 	0x0812	//�ѵ�ǰ���ŵĸ赥Ԥ�õ��հװ�����������Ѿ���Ԥ�����ݣ������θ��ǡ�
#define MCU_PRE_NNN 	0x0813	//�ѵ�ǰ���ŵĸ赥Ԥ�õ�ָ��������nnn���Դ�001��009

//����״̬֪ͨ
#define MCU_PLY_GET		0x0814	//-	MCU֪ͨģ�鷢�Ͳ���״̬��ģ���ظ�AXX+PLY+xxx
#define MCU_PLP_GET		0x0815	//- MCU֪ͨģ�鷢�͵�ǰ����Repeatģʽ
#define MCU_PLP_XXX		0x0816	//ѭ��ģʽ���ã�MCU����ͨ��MCU+PLP+GET����ȡ��ǰģʽ(000 loop all 001 single loop 002 shuffle 003 shuffle once Others: no loop)

//ָ������USB/TF��ĳ�׸���
#define MCU_MSX_NNN 	0x0817	//MCU֪ͨģ�鲥��USB(x=1)��TF��(x=2)�ϵ�nnn�׸���(n��1��ʼ)

//******************************************************************
//	9  ʱ��Ͷ�ʱ������
//******************************************************************
#define MCU_RTC_GET 	0x0901	//��ѯ��ǰϵͳʱ��
#define MCU_WEK_GET 	0x0902	//��ѯ��ǰϵͳ����
#define MCU_SND_RTC 	0x0903	//����WiFi ��RTC
#define MCU_ALM_NXT 	0x0904	//��ȡ��һ�����ӵ�ʱ��.
#define MCU_ALM_STP 	0x0905	//MCU����ģ��رյ�ǰ����
#define MCU_ALM_SNZ 	0x0906	//MCU����ģ��Ե�ǰ���ӽ�������ģʽ

//******************************************************************
//	10 �෿�书������
//******************************************************************
#define MCU_SLV_CHK		0x0A01	//MCU��ѯ��ǰ�豸������״̬
#define MCU_SLV_GET		0x0A02	//-MCU��ȡ�����������
#define MCU_JNGROUP 	0x0A03	//-MCUһ����������Ŀǰ����Ϊ������豸��������״̬�����ͬһ�����ȵ������豸��Ϊ�����䡣����豸���ڲ�����״̬������������������豸��Ϊ������
#define MCU_UNGROUP 	0x0A04	//-MCUһ����������

//******************************************************************
//	11 AUX/BT/RCA/USB ģʽ�л�����
//******************************************************************
#define MCU_PLM_SUB   	0x0B01  //- ͨ�������л�ģʽʱ��MCU���� MCU+PLM-xxx��ģ�顣��ע���һ��ָ����-����+��ģ��ֻ���޸�ģʽ���������κ����顣
#define MCU_PLM_ADD   	0x0B02  //- ͨ�������л�ģʽʱ��MCU���� MCU+PLM+xxx��ģ�顣��ע���һ��ָ����+����-��ģ����޸�ģʽ��ͬʱ��I2S data-inץ����ͬ��ת�������������䲥��
#define MCU_PLM_GET 	0x0B03	//- ��ѯ��ǰģʽ
#define MCU_USB_GET 	0x0B04	//-��ѯu���Ƿ���ڣ�ģ���ظ�AXX+USB+001����AXX+USB+000
#define MCU_MMC_GET 	0x0B05	//-��ѯT���Ƿ���ڣ�ģ���ظ�AXX+MMC+001����AXX+MMC+000

//******************************************************************
//	12  MCU����
//******************************************************************
//��������
#define MCU_AP_SCAN 	0x0C01	//MCU����ģ��ɨ�貢���������е�AP�б�ģ����AXX+WAN+LST����
#define MCU_AP_CONN 	0x0C02	//MCU����ģ������ָ����AP��ָ��ap��SSID�����룬SSID������֮���� : ������
#define MCU_STA_INF 	0x0C03	//MCU��ѯģ�鵱ǰ������������Ϣ��ģ�鷵��AXX+STA+INFessid&������essid�ǵ�ǰ���ӵ�·���������֡�
#define MCU_WCH_SET 	0x0C04	//���ò�����ʱ��Ĭ��WiFi�ŵ�
#define MCU_PWD_SET 	0x0C05	//����WiFi APģʽ��Ĭ������

//��������
#define MCU_SID_SET		0x0C06	//����SSID�ź�����
#define MCU_NAM_SET 	0x0C07	//�����豸�����ƣ�DLNA��Airplay��ʾ�����ƣ�

//******************************************************************
//	13  LCD��ʾ��
//******************************************************************
#define MCU_INF_GET 	0x0D01	//��ȡ��ϸ���豸��Ϣ�������ʽ�ɲο� HttpAPI getStatusExָ���JSON��ʽ���룬�Ƚϳ��������ٸ��ֽڡ�
#define MCU_PINF_GET 	0x0D02	//��ȡ���ŵ���ϸ��Ϣ�������ʽ�ɲο� HttpAPI getPlayerStatusָ���JSON��ʽ���룻����������Ƚϳ��Ļ����������ٸ��ַ���
#define MCU_WEA_GET 	0x0D03	//��ȡ������Ϣ������״̬����Ч��AXX+WWW+001������ʽΪ������;����;���ڼ�;����;����¶�;����¶�;��;PM2.5
#define MCU_DEV_GET 	0x0D04	//��ȡ�豸�ļ�Ҫ��Ϣ����ʽΪ��SSID;�汾����;�豸����;���ӵ�·����������;�ź�ǿ��;���״̬;��ص������а汾�������Ϊbackup����ʾϵͳ�����쳣ģʽ
#define MCU_MEA_GET 	0x0D05	//MCU������ѯ��ǰ�ĸ�����ϢMetadata���ظ�����Ĭ�ϱ�����UTF8��ʽ���ظ���ʽ��AXX+MEA+DAT����һ��
#define MCU_MEA_GBK 	0x0D06	//MCU������ѯ��ǰ�ĸ�����ϢMetadata���ظ����ݱ�����GBK��ʽ���ظ���ʽ��AXX+MEA+DAT����һ��
#define MCU_PLY_POS 	0x0D07	//����ǰ����ʱ�䣬����

//******************************************************************
//	14  ����С��ƽ̨��������
//******************************************************************
#define MCU_PLY_LKE		0x0E01	//MCU֪ͨģ���ղ�/ϲ��
#define MCU_CHL_NXT 	0x0E02	//MCU֪ͨģ����һ��Ƶ��
#define MCU_SDS_ACT 	0x0E03	//����SDS���뷽ʽ��������Ҫ���������İ���ͬʱ��Ҫ����һ�������������
#define MCU_REC_ON  	0x0E04	//���̰�¼������MCU���͸����ʼ¼��
#define MCU_REC_OFF 	0x0E05	//���ٴζ̰�¼������MCU���͸��������¼��
#define MCU_AIM_ON  	0x0E06	//������Ⱥ�ļ�1���MCU���͸����ʼ����Ⱥ��¼��
#define MCU_AIM_OFF 	0x0E07	//��Ⱥ�ļ�����ʱ��MCU���͸��������¼��������
#define MCU_AIM_PLY 	0x0E08	//���̰���Ⱥ�ļ���MCU���͸������֪ģ�鲥��Ⱥ����Ϣ
#define MCU_AST_DAT 	0x0E09	//MCU�ظ�wifiģ���豸״̬��״̬�����ݸ�ʽ���հ��ﶨ����豸״̬��֯����data ��Ҫhex���룬wifiģ��ֻ��͸����Alink��

//******************************************************************
//	15 ��������
//******************************************************************
#define MCU_FCRYPOW 	0x0F01	//���MCU����MCU+FCRYPOW����ģ��ָ������󲻻����������Ƿ���AXX+POW+OFF��MCU����ϵ硣

//******************************************************************
//	16 ͸��
//******************************************************************
//����ͬһ���෿���MCU֮����Դ���ָ��
#define MCU_M2S_NNN 	0x1001	//�豸��Masterʱ��MCU����MCU+M2S+nnnָ�ģ����ָ��ת�������е�Slave
#define MCU_S2M_NNN 	0x1002	//�豸��Slaveʱ��MCU����MCU+S2M+nnnָ�ģ����ָ��ת����Master

//App��MCU֮��Ҳ����͸������
#define MCU_PAS_DATA 	0x1003	//MCU����App, WiFiģ���յ�����MCU+PAS+��MCUָ��ʱ���������������ֱ������ & ���ţ�Ȼ���� MCU+PAS+data&ָ���App

//******************************************************************
//	18 ������ʾ����
//******************************************************************
#define MCU_LAU_XXX 	0x1201	//-MCU�ظ���ǰ����
#define MCU_LAU_GET 	0x1202	//-MCU��ѯ��ǰ����
#define MCU_PMT_GET 	0x1203	//-MCU��ѯ������ʾ״̬

//******************************************************************
//	19 I2S bypass
//******************************************************************
#define MCU_BYP_000 	0x1301 	//�ر�i2s bypass mode
#define MCU_BYP_001 	0x1302	//�� i2s bypass mode
#define MCU_BYP_GET 	0x1303	//��ѯI2S bypass ģʽ

//******************************************************************
//	20 ����ʶ������
//******************************************************************
#define MCU_TALK_ON  	0x1401	//- MCU������������ʶ������
#define MCU_TLK_OFF 	0x1402	//- ������ʶ�������ʱ��MCU���͸������������ʶ��
#define MCU_TLK_TIM 	0x1403	//-MCU֪ͨģ����������ʱ�䣬Ĭ�ϲ�֧��
#define MCU_WEATHER 	0x1404	//-MCU֪ͨģ����������������Ĭ�ϲ�֧��

//******************************************************************
//	21 EQ����
//******************************************************************
#define MCU_PAS_EQ_TREBLE	0x1501	//�ظ�EQ Trebleֵ
#define MCU_PAS_EQ_BASS 	0x1502	//�ظ�EQ Bassֵ

//******************************************************************
//	22 �ƹ����
//******************************************************************
#define MCU_PAS_LIGHTXXX 		0x1601	//- ů��ֵ
#define MCU_PAS_COLDXXX 		0x1602	//- ���ֵ
#define MCU_PAS_STYLEXXX    	0x1603	//- ���ֵ
#define MCU_PAS_AUTO_ONNX		0x1682	//- �ظ�����ʱ����ʱ��
#define MCU_PAS_AUTO_OFFX		0x1683	//- �ָ�����ʱ�ص�ʱ��

//******************************************************************
//	23 �򿪺͹ر�Webҳ��̼��������ܡ�
//******************************************************************
#define MCU_WEB_UPDX 			0x1701	//- x=0 �ر�;x=1 �� ��Ĭ�ϣ�

//******************************************************************
//	24 Alexaƽ̨��
//******************************************************************
#define MCU_TLK_ENA 			0x1801	//-MCU֪ͨģ���MIC
#define MCU_TLK_DIS 			0x1802	//-MCU֪ͨģ��ر�MIC

//******************************************************************
//	25 AVS������������
//******************************************************************
#define MCU_BT_CONN	  			0x1901	//- MCU����������������������ϴε��豸
#define MCU_BT_PAIR	  			0x1902	//-  MCU������������������Ͽ���ǰ����
//����������������ģ���Դ����������
#define MCU_BT_CTIS	  			0x1903	//- MCU֪ͨWiFiģ���������ӳɹ�
#define MCU_BT_CTIF 			0x1904	//- MCU֪ͨWiFiģ���������ʧ��
#define MCU_BT_PRDS 			0x1905	//- MCU֪ͨWiFiģ��������Գɹ�
#define MCU_BT_PARF 			0x1906	//- MCU֪ͨWiFiģ���������ʧ��
#define MCU_BT_DCTS 			0x1907	//- MCU֪ͨWiFiģ�������Ͽ����ӳɹ�
#define MCU_BT_DCTF 			0x1908	//- MCU֪ͨWiFiģ�������Ͽ�����ʧ��
#define MCU_BT_EDMS   			0x1909	//- MCU֪ͨWiFiģ��������������״̬�ɹ�
#define MCU_BT_EDMF  			0x190A	//- MCU֪ͨWiFiģ��������������״̬ʧ��
#define MCU_BT_CTPS 			0x190B	//- MCU֪ͨWiFiģ������profile���ӳɹ�
#define MCU_BT_CTPF 			0x190C	//- MCU֪ͨWiFiģ������profile����ʧ��
#define MCU_BT_GPRL 			0x190D	//- MCU֪ͨWiFiģ��������Ե���Ϣ�б�
#define MCU_BT_PLAY 			0x190E	//- MCU֪ͨWiFiģ����������״̬
#define MCU_BT_STOP 			0x190F	//- MCU֪ͨWiFiģ��������ͣ״̬
#define MCU_BT_PREV 			0x1910	//- MCU֪ͨWiFiģ��������һ���л�״̬
#define MCU_BT_NEXT				0x1911	//- MCU֪ͨWiFiģ��������һ���л�״̬
#define MCU_BT_STAS				0x1912	//- MCU֪ͨWiFiģ��������ǰ����״̬
#define MCU_BT_SNDU 			0x1913	//- MCU֪ͨWiFiģ������ɨ�赽���豸��Ϣ
#define MCU_BT_SNDF 			0x1914	//- MCU֪ͨWiFiģ������ɨ��ʧ��
#define MCU_BT_NAME 			0x1915	//- MCU֪ͨWiFiģ�������豸��Ϣ

//******************************************************************
//26 UVS IOTָ�Ŀǰ UVS IOTֻ������Kohler��Ŀ����Ҫ���ڿ���LED������ɫ�¡����ȵ��ڣ�����������δʵ�֣�
//��ע��MCUָ���е� i ��ӦΪ���豸ID�������ǰ�豸��ע��Ϊ���豸ʱ���յ���AlexaSkill��Ϣ�л�����豸ID��
//******************************************************************
#define MCU_IOTN__ON 	  		0x1A01	//- ֪ͨWIFI״̬�������豸
#define MCU_IOTN_OFF 	  		0x1A02	//- ֪ͨWIFI״̬ ���ر��豸
#define MCU_IOTN_COLOR 	  		0x1A03	//- MCU֪ͨWIFI��Ӧ��ɫ
#define MCU_IOTN_LIGHT	  		0x1A04	//- MCU֪ͨWIFI�豸����
#define MCU_IOTN_PER_ADD 	  	0x1A05	//- MCU֪ͨWIFI���ȼ�
#define MCU_IOTN_PER_SUB	  	0x1A06	//- MCU֪ͨWIFI���ȼ�

//******************************************************************
//27 ��ѶС΢��Ŀ
//******************************************************************
#define MCU_BUT_ALP 	  		0x1B01	//- �����绰
//Airplay����
#define MCU_AIR__ON 	  		0x1B02	//- 
#define MCU_AIR_OFF 	  		0x1B03	//- 
#define MCU_AIR_GET	  			0x1B04	//-
//Favorite��
#define MCU_FAV_ADD 	  		0x1B05	//-
#define MCU_FAV_DEL	  			0x1B06	//-
//¼��
#define MCU_REC_STA 	  		0x1B07	//-
#define MCU_REC_STP	  			0x1B08	//-
#define MCU_REC_SVR	  			0x1B09	//-

//******************************************************************
//28 �������ܹ��»���������
//******************************************************************
#define MCU_RAD_XXX 			0x1C01	//- MCU֪ͨWIFI��ǰ���ŵĿ����̨
#define MCU_LGT__ON 			0x1C02	//- MCU֪ͨWIFIСҹ�ƴ�
#define MCU_LGT_OFF 			0x1C03	//- MCU֪ͨWIFIСҹ�ƹر�
#define MCU_LCK__ON 			0x1C04	//- MCU֪ͨWIFI������ͯ��
#define MCU_LCK_OFF   			0x1C05	//- MCU֪ͨWIFI�رն�ͯ��
#define MCU_VLM_XXX  			0x1C06	//- MCU֪ͨWIFI�����������
#define MCU_SLP__ON   			0x1C07	//- MCU֪ͨWIFI�����˯ģʽ
#define MCU_SLP_OFF  			0x1C08	//- MCU֪ͨWIFI�˳���˯ģʽ
#define MCU_SLP_PWR				0x1C09	//- MCU֪ͨWIFI��˯��ɹػ�
#define MCU_POW_NNN				0x1C0A	//- MCU�ظ�ģ�鿪��ģʽ

#ifdef __cplusplus
}
#endif//__cplusplus

#endif

