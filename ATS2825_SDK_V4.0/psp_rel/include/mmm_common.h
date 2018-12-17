/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona    2015-02-28 15:00     1.0             build this file
 ********************************************************************************/

/*! \endcond*/
#ifndef __MMM_COMMON_H__
#define __MMM_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#include <list.h>
#endif

#include "mmm_dae_def.h"

typedef void (*smart_speech_func)(uint8);

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum {

    /*! �нӸ����м����command���� */
    MMM_COM_BASE = 0x80,

    /*! ����ʶ��ʼ*/
    MMM_SPEECH_RECONGNITION_START,

    /*! ����ʶ�����*/
    MMM_SPEECH_RECONGNITION_STOP,

    /*! ����ʶ��ʼ*/
    //MMM_SPEECH_REC_BUF_START,

    // /*! ����ʶ��ʼ*/
    MMM_SPEECH_REC_BUF_TRANS,

    /*! ����ʶ�����*/
    ////MMM_SPEECH_REC_BUF_STOP,

    MMM_SPEECH_PAUSE_MUSIC_PLAY,

    MMM_SPEECH_RESUME_MUSIC_PLAY,

    MMM_SPEECH_CALLBK_SET,

} mmm_com_cmd_e;

typedef enum {
    RECONGNITION_SRC_TYPE_SPPBLE,
    RECONGNITION_SRC_TYPE_I2S,
    RECONGNITION_SRC_TYPE_SCO,
} recongnition_src_type_e;


/*!
 *  \brief
 *     �������������Ϣ
 */
typedef struct {
    /*! ��mmm_pp_input_e*/
    int input_type;
    /*!asrc index*/
    int asrc_index;
    /*! sample rate */
    int dac_sample;
    /*!dac param dac chanel*/
    int dac_chanel;
    /*!asrc mode select */
    int asrc_mode_sel;
    /*!adc param adc source select */
    int adc_sample;
    /*!adc source select */
    int adc_insel;//mic or fm or linein
    /*!adc param adc source select */
    int adc_gain;
    /*!adc source select */
    int ain_gain;//mic or fm or linein
    /*!output channel select */
    int output_chan;
} mmm_com_ainout_setting_t;

typedef struct {
    struct list_head list;
    uint16 buf_len;//byte
    uint8 *buf_addr;
} bt_cap_raw_t;

//for bt data get pip
/*!
 *  \brief
 *  //�������ݹܵ�
 */
typedef struct {
    uint16 read_ptr;
    uint16 write_ptr;
    uint16 capacity;
    uint16 length;
    uint8 *raw_data;
    struct list_head *pool_list;
} bt_stack_pipe_t;


/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_COMMON_H__
