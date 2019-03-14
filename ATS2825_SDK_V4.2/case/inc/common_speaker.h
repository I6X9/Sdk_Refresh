/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������PA�����ȿ���ģ����غ궨�壬�ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _common_speaker_H
#define _common_speaker_H

#include "psp_includes.h"
#include "case_independent.h"

#define SPEAKER_A_PA_TYPE_D          0  //D�๦��
#define SPEAKER_A_PA_TYPE_AB         1  //AB�๦��

#define SPEAKER_A_PA_BST_EN_TIME     3  //����PA �����ȶ�ʱ�䣬��λ10ms
#define SPEAKER_A_PA_BYPASS_TIME     15 //����PA BYPASS���ݳ���ȶ�ʱ�䣬��λ10ms

#if (CASE_BOARD_TYPE == CASE_BOARD_DVB_ATS2823)
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 21)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_BST_EN_GPIOOUT  GPIOAOUTEN
#define SPEAKER_A_PA_BST_EN_GPIODATA GPIOADAT
#define SPEAKER_A_PA_BST_EN_PIN      (1 << 22)
#define SPEAKER_A_PA_BST_EN_LEVEL    ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_D_AB_GPIOOUT    GPIOAOUTEN
#define SPEAKER_A_PA_D_AB_GPIODATA   GPIOADAT
#define SPEAKER_A_PA_D_AB_PIN        (1 << 6)
#define SPEAKER_A_PA_D_AB_LEVEL      ENABLE_LEVEL_HIGH //D�๦��Ϊ�ߵ�ƽ��AB�๦��Ϊ�͵�ƽ
#elif (CASE_BOARD_TYPE == CASE_BOARD_DEMO)
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 21)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_BST_EN_GPIOOUT  GPIOAOUTEN
#define SPEAKER_A_PA_BST_EN_GPIODATA GPIOADAT
#define SPEAKER_A_PA_BST_EN_PIN      (1 << 6)
#define SPEAKER_A_PA_BST_EN_LEVEL    ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_D_AB_GPIOOUT    GPIOAOUTEN
#define SPEAKER_A_PA_D_AB_GPIODATA   GPIOADAT
#define SPEAKER_A_PA_D_AB_PIN        (1 << 6)
#define SPEAKER_A_PA_D_AB_LEVEL      ENABLE_LEVEL_HIGH //D�๦��Ϊ�ߵ�ƽ��AB�๦��Ϊ�͵�ƽ
#elif defined(BOARD_LINKPLAY_MUVO)
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 21)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_BST_EN_GPIOOUT  GPIOAOUTEN
#define SPEAKER_A_PA_BST_EN_GPIODATA GPIOADAT
#define SPEAKER_A_PA_BST_EN_PIN      (1 << 14)
#define SPEAKER_A_PA_BST_EN_LEVEL    ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_CTRL_GPIOOUT    GPIOAOUTEN
#define SPEAKER_A_PA_CTRL_GPIODATA   GPIOADAT
#define SPEAKER_A_PA_CTRL_PIN        (1 << 7)
#define SPEAKER_A_PA_CTRL_LEVEL      ENABLE_LEVEL_LOW
#elif defined(BOARD_LINKPLAY_DEMO) || defined(BOARD_LINKPLAY_FAR_DEMO)
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 5)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH
//no speak, so no need control
#elif defined(BOARD_DPI_7C370_FAR) || defined(BOARD_JL_36410_FAR)
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 5)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH
#else
#define SPEAKER_A_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_A_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_A_PA_EN_PIN          (1 << 21)
#define SPEAKER_A_PA_EN_LEVEL        ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_BST_EN_GPIOOUT  GPIOAOUTEN
#define SPEAKER_A_PA_BST_EN_GPIODATA GPIOADAT
#define SPEAKER_A_PA_BST_EN_PIN      (1 << 22)
#define SPEAKER_A_PA_BST_EN_LEVEL    ENABLE_LEVEL_HIGH

#define SPEAKER_A_PA_D_AB_GPIOOUT    GPIOAOUTEN
#define SPEAKER_A_PA_D_AB_GPIODATA   GPIOADAT
#define SPEAKER_A_PA_D_AB_PIN        (1 << 6)
#define SPEAKER_A_PA_D_AB_LEVEL      ENABLE_LEVEL_HIGH //D�๦��Ϊ�ߵ�ƽ��AB�๦��Ϊ�͵�ƽ
#endif


//I2S���������PA�����ȣ���Ҫһ��RESET�ܽź�һ��EN�ܽţ��Լ�I2S��MCLK��BCLK��LRCLK��DOUT 4����
#define SPEAKER_D_PA_RESET_GPIOOUT   GPIOAOUTEN
#define SPEAKER_D_PA_RESET_GPIODATA  GPIOADAT
#define SPEAKER_D_PA_RESET_PIN       (1 << 0)
#define SPEAKER_D_PA_RESET_LEVEL     ENABLE_FIX_ACTIVE //ֱ�����߿���ʼ��ʹ��PA
#define SPEAKER_D_PA_EN_GPIOOUT      GPIOAOUTEN
#define SPEAKER_D_PA_EN_GPIODATA     GPIOADAT
#define SPEAKER_D_PA_EN_PIN          (1 << 5)
#define SPEAKER_D_PA_EN_LEVEL        ENABLE_LEVEL_HIGH


extern void SPEAKER_CONTROL_ENABLE(void) __FAR__;
extern void SPEAKER_CONTROL_DISABLE(void) __FAR__;
extern void SPEAKER_ON(void) __FAR__;
extern void SPEAKER_OFF(void) __FAR__;
extern bool IS_SPEAKER_ON(void) __FAR__;
extern void SPEAKER_D_AB_SWITCH(uint8 type) __FAR__;
extern void SPEAKER_OUT_SWITCH(uint8 hp_in) __FAR__;

#endif
