/********************************************************************************
 *                            USDK(US282A_171204_GL5116_DVB_SVN13422_for_opus)
 *                            Module: SYSTEM
 *                 Copyright(c) 2003-2017 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>                      <version >          <desc>
 *      wuyufan   2017��12��4��-����2:00:34             1.0             build this file
 ********************************************************************************/
/*!
 * \file     btplay_eg_music_control_force.c
 * \brief    ǿ��װ���м��
 * \author
 * \version  1.0
 * \date  2017��12��4��-����2:00:34
 *******************************************************************************/

#include "btplay_engine.h"

static const uint8 s_output_channel[] = { OUTPUT_CHANNEL_NORMAL, //�������м�����ʼ��Ϊ����
                                          OUTPUT_CHANNEL_L_R_SWITCH, //������������ L = R0, R = L0
                                          OUTPUT_CHANNEL_L_R_MERGE, //���������ϲ���������������������� L = R = (L0 + R0) / 2
                                          OUTPUT_CHANNEL_L_R_ALL_L, //������������������� L = R = L0
                                          OUTPUT_CHANNEL_L_R_ALL_R, //������������������� L = R = R0
                                        };

void a2dp_decode_init_force(uint32 media_type)
{
    int mmm_init_ret;
    int result;
    mmm_bp_aout_setting_t mmm_bp_aout_param;

    char *mmm_name = { "mmm_bp.al" };

    //btplay_gl_var.media_type = media_type;

    mmm_bp_aout_param.asrc_index = K_OUT0_U0;
    mmm_bp_aout_param.asrc_mode_sel = 2;
    mmm_bp_aout_param.dac_chanel = DAF0_EN;

    //�����м������
    sys_load_mmm(mmm_name, TRUE);

    btplay_gl_var.codec_install = TRUE;

    btplay_gl_var.codec_flag = TRUE;
    //g_p_btplay_info->status = BTPLAY_PAUSE;

    //��ʼ���м����
    mmm_init_ret = mmm_bp_cmd(&mp_handle, MMM_BP_OPEN, (unsigned int) NULL);
    if(mmm_init_ret == 0) {
        if(1 == g_app_info_state_all.fix_sample_rate_flag) {
            mmm_bp_cmd(mp_handle, MMM_BP_FIX_SAMPLE_RATE, (unsigned int) NULL);
        }

        mmm_bp_cmd(mp_handle, MMM_BP_AOUT_OPEN, (unsigned int) &mmm_bp_aout_param);

        a2dp_set_codec_type(media_type);

        //�����������ѡ��
        //#define OUTPUT_CHANNEL_NORMAL      0 //�������м�����ʼ��Ϊ����
        //#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
        //#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
        //#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
        //#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0
        //mmm_bp_cmd(mp_handle, MMM_BP_SET_OUTPUT_CHANNEL, (unsigned int) OUTPUT_CHANNEL_NORMAL);

        mmm_bp_cmd(mp_handle, MMM_BP_SET_OUTPUT_CHANNEL, btplay_gl_var.output_channel_config);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //��ʱ��������
        if(g_p_bt_stack_cur_info->dev_role == 1) {
            //������
            mmm_bp_cmd(mp_handle, MMM_BP_SET_OUTPUT_CHANNEL,
                       (unsigned int) s_output_channel[btplay_gl_var.tws_m_output_channel]);

            PRINT_INFO_INT("channel:", s_output_channel[btplay_gl_var.tws_m_output_channel]);
        } else if(g_p_bt_stack_cur_info->dev_role == 2) {
            //������
            mmm_bp_cmd(mp_handle, MMM_BP_SET_OUTPUT_CHANNEL,
                       (unsigned int) s_output_channel[btplay_gl_var.tws_s_output_channel]);

            PRINT_INFO_INT("channel:", s_output_channel[btplay_gl_var.tws_s_output_channel]);
        } else {
            ;        // do nothing
        }
#endif
        //SBC�����ʱ�Ƿ���������
        result = mmm_bp_cmd(mp_handle, MMM_BP_RESTRAIN_NOISE, TRUE);

#ifdef WAVES_ASET_TOOLS
        //���Ͳ�������
        g_waves.bin_number = g_app_info_state_all.bin_number;
        result = mmm_bp_cmd(mp_handle, MMM_BP_PLAY, (unsigned int) &g_waves);
#else
        //���Ͳ�������
        result = mmm_bp_cmd(mp_handle, MMM_BP_PLAY, (unsigned int) NULL);
#endif

        if(result == 0) {
            PRINT_INFO("btplay mmm init");
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            uint32 temp_flag;
            temp_flag = sys_local_irq_save();
            g_p_tws_info->mmm_init_flag = 1;
            sys_local_irq_restore(temp_flag);
#endif
            mmm_bp_cmd(mp_handle, MMM_BP_GET_ENERGY_INFO, (unsigned int) & (g_app_info_state_all.p_energy_value));
            g_app_info_state_all.energy_available_flag = TRUE;
        }
    }
}

void a2dp_decode_quit_force(void)
{
    uint32 temp_flag;

    if(mp_handle != NULL) {

        g_app_info_state_all.energy_available_flag = FALSE;
#ifdef WAVES_ASET_TOOLS
        mmm_bp_cmd(mp_handle, MMM_BP_STOP, (unsigned int) &g_waves);
#else
        mmm_bp_cmd(mp_handle, MMM_BP_STOP, (unsigned int) NULL);
#endif
        mmm_bp_cmd(mp_handle, MMM_BP_AOUT_CLOSE, (unsigned int) NULL);
        //�ر��м���豸
        mmm_bp_cmd(mp_handle, MMM_BP_CLOSE, (unsigned int) NULL);

        //ж�ؽ�������
        sys_free_mmm(TRUE);

        btplay_gl_var.codec_install = FALSE;
        mp_handle = NULL;
    }

    PRINT_INFO("btplay mmm exit");

    adjust_freq_add_value(0, 0);

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);

    com_set_sound_out(SOUND_OUT_STOP);
}
