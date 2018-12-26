/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     user1_main.c
 * \brief    picture��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ap_manager_test.h"
#include "ap_autotest_mptest.h"
#include "bt_controller_interface.h"

#pragma __PRQA_IGNORE_START__

static const uint8 bt_drv_name[] = "bt_ctrl.drv";
static const uint8 bt_mpdat_name[] = "mpdata.bin";

static const uint8 hci_tx_channel_cmd[] = {0x01, 0x4a, 0xfd, 0x04, 0x3f, 0x00, 0x00, 0x00 };

extern int read_trim_cap_efuse(uint32 mode) __FAR__;

extern uint32 att_write_trim_cap(uint16 index, uint32 mode) __FAR__;

void act_test_report_mptest_result(int32 ret_val, mp_test_arg_t *mp_arg, cfo_return_arg_t *cfo_return_arg,
                                   uint32 test_mode)
{
    return_result_t *return_data;
    uint16 trans_bytes = 0;

    //DEBUG_ATT_PRINT("mp cfo val", cfo_return_arg->cfo_val, 2);

    return_data = (return_result_t *)(STUB_ATT_RETURN_DATA_BUFFER);

    if(test_mode == 0) {
        return_data->test_id = TESTID_MP_TEST;

        if(ret_val == TRUE) {
            if((cfo_return_arg->cfo_index - CFO_THRESHOLD_ADJUST_VAL) > CFO_THRESHOLD_LEFT) {
                cfo_return_arg->cfo_index_low = cfo_return_arg->cfo_index - CFO_THRESHOLD_ADJUST_VAL;
            } else {
                cfo_return_arg->cfo_index_low = CFO_THRESHOLD_LEFT;
            }

            if((cfo_return_arg->cfo_index + CFO_THRESHOLD_ADJUST_VAL) < CFO_THRESHOLD_RIGHT) {
                cfo_return_arg->cfo_index_high = cfo_return_arg->cfo_index + CFO_THRESHOLD_ADJUST_VAL;
            } else {
                cfo_return_arg->cfo_index_high = CFO_THRESHOLD_RIGHT;
            }
        } else {

            cfo_return_arg->cfo_index_low = mp_arg->cfo_index_low;

            cfo_return_arg->cfo_index_high = mp_arg->cfo_index_high;
        }
    } else {
        return_data->test_id = TESTID_MP_READ_TEST;

        cfo_return_arg->cfo_index_low = mp_arg->cfo_index_low;

        cfo_return_arg->cfo_index_high = mp_arg->cfo_index_high;
    }

    if(ret_val == TRUE) {
        print_log("test over! cfo:%d pwr %d index:%d", cfo_return_arg->cfo_val, cfo_return_arg->pwr_val,
                  cfo_return_arg->cfo_index);
    } else {
        print_log("test failed! min cfo: %d pwr %d index:%d", cfo_return_arg->cfo_val, cfo_return_arg->pwr_val,
                  cfo_return_arg->cfo_index);
    }

    return_data->test_result = ret_val;

    //print_log("return arg: index low %d, index high %d, cfo val %d, pwr val %d, ber val %d",
    //    cfo_return_arg->cfo_index_low, cfo_return_arg->cfo_index_high, cfo_return_arg->cfo_val,
    //    cfo_return_arg->pwr_val, cfo_return_arg->ber_val);

    int32_to_unicode(mp_arg->cfo_channel[0], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_channel[1], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_channel[2], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_test, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    //Ҫ�޸ĵĲ���
    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(cfo_return_arg->cfo_index_low, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(cfo_return_arg->cfo_index_high, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_index_changed, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_threshold_low, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_threshold_high, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_write_mode, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->cfo_upt_offset, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->pwr_test, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->pwr_threshold_low, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    return_data->return_arg[trans_bytes++] = 0x002c;

    int32_to_unicode(mp_arg->pwr_threshold_high, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    //�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
    if((trans_bytes % 2) != 0) {
        return_data->return_arg[trans_bytes++] = 0x0000;
    }

    act_test_report_result(return_data, trans_bytes * 2 + 4);
}

uint32 att_modify_tx_channel(uint8 *hci_data, int index, int channel)
{
    hci_data[index] = channel;

    return TRUE;
}

uint32 att_modify_mp_data_sub(uint8 *hci_data, uint32 data_len, uint32 pkt_index, int channel,
                              modify_cb_func_t cb)
{
    uint32 i, j;
    cmd_pkt_head_t *cmd_pkt_head;
    uint8 *data_buffer;
    uint8 cmd_len;

    cmd_pkt_head = (cmd_pkt_head_t *)(ATT_MPDATA_TEMP_BUFFER);

    data_buffer = (uint8 *) cmd_pkt_head;

    for(;;) {
        if(cmd_pkt_head->pkt_index == pkt_index) {
            break;
        }

        if(cmd_pkt_head->next_pkt_offset >= g_mpdata_bin_len) {
            return FALSE;
        }

        cmd_pkt_head = (cmd_pkt_head_t *)((uint8 *) ATT_MPDATA_TEMP_BUFFER + cmd_pkt_head->next_pkt_offset);
    }

    data_buffer = (uint8 *)((uint8 *) cmd_pkt_head + sizeof(cmd_pkt_head_t));

    for(i = 0; i < cmd_pkt_head->total_node; i++) {
        cmd_len = *data_buffer;

        data_buffer++;

        if(cmd_len >= data_len) {
            for(j = 0; j < data_len; j++) {
                if(data_buffer[j] != hci_data[j]) {
                    break;
                }
            }

            //PRINT_DATA(data_buffer, j);

            if(j == data_len) {
                cb(data_buffer, j, channel);
                return TRUE;
            }
        }

        data_buffer += cmd_len;
    }

    return FALSE;
}

uint32 att_modify_mp_data(int channel)
{
    att_modify_mp_data_sub(hci_tx_channel_cmd, 5, PKT_CFO_BEGINE_INDEX, channel,
                           att_modify_tx_channel);

    return TRUE;
}

uint32 att_read_mpdata(mp_test_arg_t *mp_arg)
{
    uint32 i;
    atf_head_t *atf_head;
    uint8 *data_buffer = (uint8 *) ATT_MPDATA_TEMP_BUFFER;

    att_test_fread(data_buffer, 512, 0);

    atf_head = (atf_head_t *) data_buffer;

    for(i = 0; i < (512 / sizeof(atf_dir_t)); i++) {
        if(libc_memcmp(atf_head->atf_dir[i].filename, bt_mpdat_name, 10) == 0) {
            break;
        }
    }

    g_mpdata_bin_len = atf_head->atf_dir[i].length;

    //��ȡhci����
    if(i < (512 / sizeof(atf_dir_t))) {
        if(g_mpdata_bin_len <= ATT_MPDATA_MEX_LENGTH) {
            att_test_fread(data_buffer, atf_head->atf_dir[i].length, atf_head->atf_dir[i].offset);

            return TRUE;
        }
    }

    //att_modify_mp_data(mp_arg);

    return FALSE;
}

void ringbuf_init(void)
{
    g_ringbuf_rw.read_ptr = 0;
    g_ringbuf_rw.write_ptr = 0;
    g_ringbuf_rw.readable_len = 0;
    g_ringbuf_rw.read_buffer = (uint8 *) ATT_RINGBUFFER_ADDR;
}


uint32 att_cfo_test_init(mp_test_arg_t *mp_arg, int channel)
{
    uint32 ret_val;

    cfo_param_t cfo_param;

    libc_memset(&cfo_param, 0, sizeof(cfo_param));

    cfo_param.ic_type = MP_ICTYPE;
    cfo_param.channel = channel;
    cfo_param.tx_gain_idx = MP_TX_GAIN_IDX;
    cfo_param.tx_gain_val = MP_TX_GAIN_VAL;
    cfo_param.payload = PAYLOADTYPE_SET;
    cfo_param.pkt_type = PKTTYPE_SET;
    cfo_param.tx_dac = MP_TX_DAC;
    cfo_param.whitening_cv = WHITENCOEFF_SET;
    cfo_param.pkt_header = PKTHEADER_SET;
    cfo_param.hit_target_l = HIT_ADDRESS_SET_L;
    cfo_param.hit_target_h = HIT_ADDRESS_SET_H;
    cfo_param.sut_state = g_SUT_state;
    cfo_param.report_interval = MP_REPORT_RX_INTERVAL;
    cfo_param.skip_report_count = MP_SKIP_PKT_NUM;
    cfo_param.once_report_pkts = MP_ONCE_REPORT_MIN_PKT_NUM;
    cfo_param.report_timeout = MP_REPORT_TIMEOUT;

    cfo_param.recv_cfo_count = MP_RETURN_RESULT_NUM;
    //cfo_param.sut_download_patch = mp_arg->cfo_force_sut_init;

    libc_memcpy((uint8 *)(STUB_ATT_RW_TEMP_BUFFER + sizeof(stub_ext_cmd_t)), &cfo_param, sizeof(cfo_param));

    ret_val = att_write_data(STUB_CMD_ATT_CFO_TX_BEGIN, sizeof(cfo_param_t), STUB_ATT_RW_TEMP_BUFFER);

    if(ret_val == 0) {
        att_read_data(STUB_CMD_ATT_ACK, 0, STUB_ATT_RW_TEMP_BUFFER);
    }

    if(g_SUT_state == 0) {
        g_SUT_state = 1;
    }

    //libc_print("--BTTxbegin", 0, 0);

    return TRUE;
}


uint32 att_mptest_init(mp_test_arg_t *mp_arg, int channel)
{
    bt_drv_param_t tmp_bt_drv_param;

    btt_priv_data_t btt_priv_data;

    g_SUT_state = 0;

    ringbuf_init();

    //�������������bank�������������PC���߾ͽ���SUT�ĳ�ʼ�������ʱ��
    //������Ӧ���ߵ��κ����������������ʵ��һ���ĺ���������bank�л�
    att_cfo_test_init(mp_arg, channel);

    if(sys_get_drv_install_info(DRV_GROUP_BT) > 0) {
        //��ж��BT����
        sys_drv_uninstall(DRV_GROUP_BT);
    }

    //��װ��BT����
    tmp_bt_drv_param.cbk = put_hci_data_to_ringbuf;

    tmp_bt_drv_param.bd_addr[0] = 0x00;
    tmp_bt_drv_param.bd_addr[1] = 0x01;
    tmp_bt_drv_param.bd_addr[2] = 0x02;
    tmp_bt_drv_param.bd_addr[3] = 0xfd;
    tmp_bt_drv_param.bd_addr[4] = 0x4e;
    tmp_bt_drv_param.bd_addr[5] = 0xf4;

    tmp_bt_drv_param.uart_idle_timer = 5000;
    tmp_bt_drv_param.p_hci_buffer_get_len_cb = &(g_hci_deal.get_data_len);
    tmp_bt_drv_param.p_hci_buffer_read_data_cb = &(g_hci_deal.read_data);
    sys_drv_install(DRV_GROUP_BT, &tmp_bt_drv_param, bt_drv_name);

    bt_drv_controller_poweron();

    sys_mdelay(100);

    btt_priv_data.download_patch = FALSE;
    btt_priv_data.reset_controller = TRUE;

    bt_drv_down_patchcode(TRUE, BT_BTT_MODE, &btt_priv_data);

    att_read_mpdata(mp_arg);

    //#if 1
    //    *((REG32)(DEBUGSEL)) = 0x13; /*debug module select cmu*/
    //    *((REG32)(DEBUGOE)) |= 0x00200000; /*enable gpio_a21 debug output*/
    //    *((REG32)(CMU_DEBUG_CTL)) &= 0xfffffff0;
    //    *((REG32)(CMU_DEBUG_CTL)) |= 0x1; /*cmu backdoor output hosc*/
    //#endif
    return TRUE;
}



uint32 do_mp_test(mp_test_arg_t *mp_arg, cfo_return_arg_t *return_arg, uint32 test_mode, uint32 channel)
{
    att_mptest_init(mp_arg, channel);

    //�޸�ͨ��ֵ
    att_modify_mp_data(channel);

    //�����������ֵҪ��������У��Ƶƫ
    return att_cfo_test(mp_arg, return_arg, test_mode, channel);
}


test_result_e att_mp_test(void *arg_buffer)
{
    uint32 i;
    int16 cfo_val[3];
    int16 pwr_val[3];
    uint16 cfo_index[3];
    int32 ret_val;
    int32 test_channel_num;
    cfo_return_arg_t cfo_return_arg;
    mp_test_arg_t *mp_arg;
    uint32 check_record = FALSE;

    bool adjusted_ok = FALSE;
    uint32 cap_value;

    mp_arg = (mp_test_arg_t *) arg_buffer;

    libc_memset(&cfo_return_arg, 0, sizeof(cfo_return_arg_t));

    libc_memset(&cfo_val, 0, sizeof(cfo_val));

    libc_memset(&pwr_val, 0, sizeof(pwr_val));

    libc_memset(&cfo_index, 0, sizeof(cfo_index));

    test_channel_num = 0;

    for(i = 0; i < 3; i++) {
        if(mp_arg->cfo_channel[i] != 0xff) {
            test_channel_num++;
        }
    }

    for(i = 0; i < 3; i++) {
        if(mp_arg->cfo_channel[i] == 0xff) {
            continue;
        }

        if(check_record == FALSE) {
            //���ж��Ƿ������Ƶƫ
            if((cap_value = read_trim_cap_efuse(RW_TRIM_CAP_SNOR)) != -1) {
                print_log("check cap_value: 0x%x", cap_value);

                if((ret_val = do_mp_test(mp_arg, &cfo_return_arg, 1, mp_arg->cfo_channel[i])) == TRUE) {
                    print_log("check ok");

                    //�����ȡƵƫֵ�ǶԵģ�ֱ�ӷ��ز��Գɹ�
                    adjusted_ok = TRUE;

                    break;
                }

                print_log("check fail, readjust...");
            }

            check_record = TRUE;
        }

        ret_val = do_mp_test(mp_arg, &cfo_return_arg, 0, mp_arg->cfo_channel[i]);

        if(ret_val == FALSE) {
            break;
        }

        print_log("test channel %d cf0 %dhz pwr %d index %d\n", mp_arg->cfo_channel[i],
                  cfo_return_arg.cfo_val, cfo_return_arg.pwr_val, cfo_return_arg.cfo_index);

        cfo_val[i] = cfo_return_arg.cfo_val;
        pwr_val[i] = cfo_return_arg.pwr_val;
        cfo_index[i] = cfo_return_arg.cfo_index;
    }

    if(ret_val == TRUE && adjusted_ok == FALSE) {
        cfo_return_arg.cfo_val = (cfo_val[0] + cfo_val[1] + cfo_val[2]) / (test_channel_num);

        cfo_return_arg.pwr_val = (pwr_val[0] + pwr_val[1] + pwr_val[2]) / (test_channel_num);

        cfo_return_arg.cfo_index = (cfo_index[0] + cfo_index[1] + cfo_index[2]) / (test_channel_num);

        if(mp_arg->cfo_write_mode == 1) {
            ret_val = att_write_trim_cap(cfo_return_arg.cfo_index, RW_TRIM_CAP_EFUSE);
        }

        if(mp_arg->cfo_write_mode == 2) {
            ret_val = att_write_trim_cap(cfo_return_arg.cfo_index, RW_TRIM_CAP_SNOR);
        }
    }

    act_test_report_mptest_result(ret_val, mp_arg, &cfo_return_arg, 0);

    bt_drv_controller_poweroff();

    //�������ж��bt����
    sys_drv_uninstall(DRV_GROUP_BT);

    return TEST_PASS;
}

test_result_e att_mp_read_test(void *arg_buffer)
{
    uint32 i;
    int16 cfo_val[3];
    int16 pwr_val[3];
    uint16 cfo_index[3];
    int32 test_channel_num;

    int32 ret_val;
    cfo_return_arg_t cfo_return_arg;
    mp_test_arg_t *mp_arg;

    mp_arg = (mp_test_arg_t *) arg_buffer;

    libc_memset(&cfo_return_arg, 0, sizeof(cfo_return_arg_t));

    libc_memset(&cfo_val, 0, sizeof(cfo_val));

    libc_memset(&pwr_val, 0, sizeof(pwr_val));

    libc_memset(&cfo_index, 0, sizeof(cfo_index));

    test_channel_num = 0;

    for(i = 0; i < 3; i++) {
        if(mp_arg->cfo_channel[i] != 0xff) {
            test_channel_num++;
        }
    }

    //print_log("start read freq trim result...");

    for(i = 0; i < 3; i++) {
        if(mp_arg->cfo_channel[i] == 0xff) {
            continue;
        }

        //print_log("read channel %d\n", mp_arg->cfo_channel[i]);

        ret_val = do_mp_test(mp_arg, &cfo_return_arg, 1, mp_arg->cfo_channel[i]);

        print_log("read channel %d cfO %dhz pwr %d index %d\n", mp_arg->cfo_channel[i],
                  cfo_return_arg.cfo_val, cfo_return_arg.pwr_val, cfo_return_arg.cfo_index);

        if(ret_val == FALSE) {
            break;
        }
        cfo_val[i] = cfo_return_arg.cfo_val;
        pwr_val[i] = cfo_return_arg.pwr_val;
        cfo_index[i] = cfo_return_arg.cfo_index;
    }

    cfo_return_arg.cfo_val = (cfo_val[0] + cfo_val[1] + cfo_val[2]) / (test_channel_num);

    cfo_return_arg.pwr_val = (pwr_val[0] + pwr_val[1] + pwr_val[2]) / (test_channel_num);

    cfo_return_arg.cfo_index = (cfo_index[0] + cfo_index[1] + cfo_index[2]) / (test_channel_num);

    act_test_report_mptest_result(ret_val, mp_arg, &cfo_return_arg, 1);

    bt_drv_controller_poweroff();

    //�������ж��bt����
    sys_drv_uninstall(DRV_GROUP_BT);

    return TEST_PASS;
}
#pragma __PRQA_IGNORE_END__
