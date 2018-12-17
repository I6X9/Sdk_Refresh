#ifndef _SLAVE_MCU_COMMAND_H_
#define _SLAVE_MCU_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

//****************************************************************************************
//					MCU �����
//****************************************************************************************

//******************************************************************
//	1 MCU���͸�����
//******************************************************************
#define COM_PP      	0x01	//����/��ͣ
#define COM_PA    		0x02	//����
#define COM_PU    		0x03	//��ͣ
#define COM_PV      	0x04	//��һ��
#define COM_PN      	0x05	//��һ��
#define COM_VD      	0x06	//������
#define COM_VP      	0x07	//������
#define COM_MC      	0x08	//ģʽ�л�
#define COM_MBT     	0x09	//�ӵ�ǰģʽ�л�������ģʽ
#define COM_MAX     	0x0A	//�ӵ�ǰģʽ�л���AUXģʽ
#define COM_MFM     	0x0B	//�ӵ�ǰģʽ�л���FMģʽ
#define COM_MSD     	0x0C	//�ӵ�ǰģʽ�л���SDģʽ
#define COM_MUD     	0x0D	//�ӵ�ǰģʽ�л���USBģʽ
#define COM_PWD     	0x0E	//�ػ�
#define COM_BT_PR     0x0F	//�����������״̬
#define COM_BT_AC     0x10	//�������ڻ��������Ե��豸
#define COM_BT_DC     0x11	//�����Ͽ�����
#define COM_BT_CA     0x12	//������������
#define COM_BT_CE     0x13	//��������ͨ��
#define COM_BT_CJ     0x14	//��������ܽ�
#define COM_BT_CR     0x15	//����ĩ���ز�
#define COM_BT_FR     0x16	//��������
#define COM_BT_FF     0x17	//�������
#define COM_REC_OK    0x18	//¼������OK
#define COM_REC_ERR   0x19	//¼������ʧ�ܣ���������TF��/������
#define COM_REC_END   0x1A	//¼������
#define COM_PA_OK     0x1B	//¼������OK
#define COM_PA_ERR    0x1C	//¼�����Ŵ����޿�/��¼���ļ�
#define COM_SD_PN     0x1D	//SD������
#define COM_SD_PO     0x1E	//SD���γ�
#define COM_SD_ON     0x1F	//SD���ߣ��������ֻ���ڿ���ʱ�ظ�������SD���Ƿ�����
#define COM_SD_OFF    0x20	//SD�����ߣ��������ֻ���ڿ���ʱ�ظ�������SD���Ƿ�����

//******************************************************************
//	2 �������͸�MCU
//******************************************************************
#define SY_PO        	0x60	//ϵͳ����
#define SY_PF        	0x61	//ϵͳ�ػ�
#define BT_AC        	0x62	//��ǰ������ģʽ���������ڻ���
#define BT_WP        	0x63	//��ǰ������ģʽ�������������״̬
#define BT_WC        	0x64	//��ǰ������ģʽ���������ڵȴ�����״̬
#define BT_CN        	0x65	//��ǰ������ģʽ�������Ѿ�����
#define BT_PA       	0x66	//��ǰ������ģʽ���������ڲ���
#define BT_IC        	0x67	//��ǰ������ģʽ������������
#define BT_OC        	0x68	//��ǰ������ģʽ�������е绰���
#define BT_EC        	0x69	//��ǰ������ģʽ����������ͨ����
#define SD_PA        	0x6A	//��ǰ��SD��ģʽ��SD�����ڲ���
#define SD_PU        	0x6B	//��ǰ��SD��ģʽ��SD��������ͣ״̬
#define AX_PA       	0x6C	//��ǰ��AUXģʽ��AUX���ڲ���
#define AX_PU        	0x6D	//��ǰ��AUXģʽ��AUX������ͣ״̬
#define REC_START     0x6E	//����֪ͨMCU��¼������/����
#define REC_PA        0x6F	//����֪ͨMCU������¼���ļ�

//******************************************************************
//	3 �����������״̬
//******************************************************************
#define MAIN_SY_OFF   0x00	//�����Ѿ��ػ�
#define MAIN_BT_DN    0x90	//����������ģʽ����û����������
#define MAIN_BT_CN    0x91	//����������ģʽ���������Ѿ�����
#define MAIN_BT_PA    0x92	//����������ģʽ�����������ڲ�������
#define MAIN_BT_PU    0x93	//����������ģʽ��������������ͣ
#define MAIN_AX_PA    0x94	//������AUXʽ�������ڲ�������
#define MAIN_AX_PU    0x95	//������AUXʽ���Ҳ���������ͣ
#define MAIN_SY_ON    0xFF	//�����Ѿ�����

#ifdef __cplusplus
}
#endif//__cplusplus

#endif

