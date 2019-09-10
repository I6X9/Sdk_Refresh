
///////////////////////////////////////////////////////////////////////////////
//               Mountain View Silicon Tech. Inc.
//  Copyright 2012, Mountain View Silicon Tech. Inc., Shanghai, China
//                       All rights reserved.
//  Filename: bt_config.h
//  maintainer: keke
///////////////////////////////////////////////////////////////////////////////

#ifndef __BT_DEVICE_CFG_H__
#define __BT_DEVICE_CFG_H__
#include "type.h"

#define ENABLE						TRUE
#define DISABLE						FALSE

/*****************************************************************
 *
 * BLE config
 *
 */
#define BLE_SUPPORT					DISABLE

/*****************************************************************
 *
 * Bluetooth stack common config
 *
 */

//��������֧������,��Ҫʹ��URL����
//BLE�������޸���ble�㲥����������(ble_app_func.c)
#define BT_NAME						"BP10_BT"
#define BT_NAME_SIZE				40//���֧��name size,�����޸�
#define BT_ADDR_SIZE				6

#define BT_TRIM						0x18

#define BT_PINCODE					"0000"

#define BT_TX_POWER_LEVEL			22	//23:max tx power level(max:8db / -2db step)

//inquiry scan params
#define BT_INQUIRYSCAN_INTERVAL		0x100	//default:0x1000
#define BT_INQUIRYSCAN_WINDOW		0x12	//default:0x12

//page scan params
#define BT_PAGESCAN_INTERVAL		0x1000	//default:0x1000
#define BT_PAGESCAN_WINDOW			0x12	//default:0x12

//page timeout
//time:4096*0.625=2.56s
#define BT_PAGE_TIMEOUT				0x1000	//default:0x2000

/**
 * ���º����������޸ģ����������������
 * ע��A2DP��AVRCP�Ǳ��䣬����ҪENABLE
 */
#define BT_A2DP_SUPPORT				ENABLE
#if (BT_A2DP_SUPPORT == ENABLE)
#define BT_AVRCP_SUPPORT			ENABLE	//a2dp and avrcp must be enable at the same time
#define BT_HFP_SUPPORT				ENABLE
#define BT_SPP_SUPPORT				ENABLE
#else
#define BT_AVRCP_SUPPORT			DISABLE
#define BT_HFP_SUPPORT				DISABLE
#define BT_SPP_SUPPORT				DISABLE
#endif

//�ڷ�����ģʽ��,���������Զ��л�������ģʽ
//#define BT_AUTO_ENTER_PLAY_MODE

/*****************************************************************
 *
 * A2DP config
 *
 */
#if BT_A2DP_SUPPORT == ENABLE

#include "bt_a2dp_api.h"

#define BT_A2DP_AUDIO_DATA				A2DP_AUDIO_DATA_PCM

#endif /* BT_A2DP_SUPPORT == ENABLE */

/*****************************************************************
 *
 * AVRCP config
 *
 */
#if BT_AVRCP_SUPPORT == ENABLE

#include "bt_avrcp_api.h"

/*
 * ���鿪���߼�AVRCP,�رպ���ܻᵼ��A2DP����״̬�ĸ����쳣
 */
#define BT_AVRCP_ADVANCED				ENABLE

#if (BT_AVRCP_ADVANCED == ENABLE)
/*
 * If it doesn't support Advanced AVRCP, TG side will be ignored
 * ����ͬ��������Ҫ�����ú꿪��
 */
#define BT_AVRCP_VOLUME_SYNC			DISABLE

/*
 * If it doesn't support Advanced AVRCP, song play state will be ignored
 * ��������ʱ��
 */
#define BT_AVRCP_SONG_PLAY_STATE		DISABLE

/*
 * If it doesn't support Advanced AVRCP, song track infor will be ignored
 * ����ID3��Ϣ����
 * ������Ϣ����������ʱ������ȡ,���BT_AVRCP_SONG_PLAY_STATEͬ������
 */
#define BT_AVRCP_SONG_TRACK_INFOR		DISABLE

#else

#define BT_AVRCP_VOLUME_SYNC			DISABLE

#define BT_AVRCP_SONG_TRACK_INFOR		DISABLE

#define BT_AVRCP_SONG_PLAY_STATE		DISABLE

#endif

/*
 * AVRCP���ӳɹ����Զ����Ÿ���
 */
#define BT_AUTO_PLAY_MUSIC				DISABLE

#endif /* BT_AVRCP_SUPPORT == ENABLE */

/*****************************************************************
 *
 * HFP config
 *
 */
#if BT_HFP_SUPPORT == ENABLE

#include "bt_hfp_api.h"

//DISABLE: only cvsd
//ENABLE: cvsd + msbc
#define BT_HFP_SUPPORT_WBS				ENABLE

/*
 * If it doesn't support WBS, only PCM format data can be
 * transfered to application.
 */
#define BT_HFP_AUDIO_DATA				HFP_AUDIO_DATA_mSBC


/*
 * ͨ���������
 */
//AEC��ز������� (MIC gain, AGC, DAC gain, �������)
#define BT_HFP_AEC_ENABLE

//MIC���˷�,ʹ�����²���(�ο�)
#define BT_HFP_MIC_PGA_GAIN				2  //ADC PGA GAIN +18db(0~31, 0:max, 31:min)
#define BT_HFP_MIC_DIGIT_GAIN			30000
#define BT_HFP_INPUT_DIGIT_GAIN			2000

//MIC���˷�,ʹ�����²���(�ο�������)
//#define BT_HFP_MIC_PGA_GAIN				14  //ADC PGA GAIN +2db(0~31, 0:max, 31:min)
//#define BT_HFP_MIC_DIGIT_GAIN				4095
//#define BT_HFP_INPUT_DIGIT_GAIN			4095

#define BT_HFP_AEC_ECHO_LEVEL			1 //Echo suppression level: 0(min)~5(max)
#define BT_HFP_AEC_NOISE_LEVEL			0 //Noise suppression level: 0(min)~5(max)

#define BT_HFP_AEC_MAX_DELAY_BLK		32
#define BT_HFP_AEC_DELAY_BLK			10 //MIC���˷Ųο�ֵ
//#define BT_HFP_AEC_DELAY_BLK			14 //MIC���˷Ųο�ֵ(�ο�������)


//����ͨ��ʱ������ѡ��
#define BT_HFP_CALL_DURATION_DISP


#endif /* BT_HFP_SUPPORT == ENABLE */


/*****************************************************************
 * ram config
 */
//A2DP+AVRCP+SPP = 24K
//A2DP+AVRCP+SPP+HFP = 26K

#if (BT_AVRCP_VOLUME_SYNC == ENABLE)
#define BT_AVRCP_TG_MEM_SIZE		2*1024
#else
#define BT_AVRCP_TG_MEM_SIZE		0
#endif

#if ((BT_A2DP_SUPPORT == ENABLE)&&(BT_HFP_SUPPORT == ENABLE))
#define BT_STACK_MEM_SIZE			(26*1024+BT_AVRCP_TG_MEM_SIZE)
#elif (BT_A2DP_SUPPORT == ENABLE)
#define BT_STACK_MEM_SIZE			(24*1024+BT_AVRCP_TG_MEM_SIZE)
#endif

/*****************************************************************
 * bt mode config
 */
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

//Ԥ���ù��ܺͶ�Ӧ�ĺ����ӿ�
//#define BT_FAST_POWER_ON_OFF_FUNC						// ���ٴ�/�ر���������

#endif /*__BT_DEVICE_CFG_H__*/

