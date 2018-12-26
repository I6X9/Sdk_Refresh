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
#include "ap_autotest_bertest.h"

static const uint8 bt_drv_name[] = "bt_ctrl.drv";

static const uint8 hci_rx_channel_cmd[] = {0x01, 0x4a, 0xfd, 0x04, 0x3f, 0x80, 0x00, 0x00 };

/**
 ����: �ϱ����Խ����PC
 ����:
 ����:
 ˵��:
*/
static void _ber_test_report_result(ber_test_arg_t *ber_arg, uint8 ret_val)
{
    return_result_t *return_data;
    uint16 trans_bytes = 0;

    return_data = (return_result_t *)(STUB_ATT_RETURN_DATA_BUFFER);

    if(g_ber_control->rssi_test == FALSE) {
        return_data->test_id = TESTID_BER_TEST;
    } else {
        return_data->test_id = TESTID_RSSI_TEST;
    }

    return_data->test_result = ret_val;

    /** 0x002c�ǲ����ָ���
    */
    int32_to_unicode(ber_arg->ber_channel[0], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->ber_channel[1], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->ber_channel[2], &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->ber_thr_low, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->ber_thr_high, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->rssi_thr_low, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);
    return_data->return_arg[trans_bytes++] = 0x002c;
    int32_to_unicode(ber_arg->rssi_thr_high, &(return_data->return_arg[trans_bytes]), &trans_bytes, 10);

    /** ��ӽ�����,�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
    */
    return_data->return_arg[trans_bytes++] = 0x0000;
    if((trans_bytes % 2) != 0) {
        return_data->return_arg[trans_bytes++] = 0x0000;
    }

    act_test_report_result(return_data, trans_bytes * 2 + 4);
}



/**
����: ֪ͨPC����ֹͣ����
����:
����:
˵��:
*/
static uint8  _ber_test_stop_att(ber_test_arg_t *ber_arg, uint32 test_result)
{
    uint32 ret_val;
    uint32 read_len;
    uint8 *pdata;

    print_log("ber att stop!");

    /** �����ȵ���ֵ
    */
    //g_ber_control->dut2pc_data.ber_thr = ber_arg->ber_thr;

    /** �Ѳ��Խ������ATT���߲�֪ͨATT����stop
    */
    libc_memcpy(STUB_ATT_RW_TEMP_BUFFER, &(g_ber_control->dut2pc_data), sizeof(ber_update_arg_t));
    ret_val = att_write_data(STUB_CMD_ATT_BER_TX_STOP, sizeof(ber_update_arg_t), STUB_ATT_RW_TEMP_BUFFER);
    if(ret_val == 0) {
        read_len = sizeof(ber_retrun_arg_t);
        ret_val = att_read_data(STUB_CMD_ATT_BER_TX_STOP, read_len, STUB_ATT_RW_TEMP_BUFFER);
        if(ret_val == 0) {
            pdata = (uint8 *)STUB_ATT_RW_TEMP_BUFFER;

            /** ��PC�������������Ƿ���Ҫ��bytes 6��ʼ???
            */
            libc_memcpy(&(g_ber_control->pc2dut_data), &pdata[0], read_len);
        }
    }

    if(test_result == TRUE) {
        /** ����RX stop
        */
        _ber_test_write_berdata(PKT_CFO_STOP_INDEX);
    }
    return TRUE;
}



/**
 ����: ��berdata.bin,����BEGINE&STOP RX
 ����:
 ����: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ˵��:
*/
static uint8 _ber_test_read_berdata(ber_test_arg_t *ber_arg)
{
    uint32 i;
    atf_head_t *atf_head;
    uint8 *data_buffer = (uint8 *)ATT_MPDATA_TEMP_BUFFER;

    att_test_fread(data_buffer, 512, 0);

    atf_head = (atf_head_t *) data_buffer;
    for(i = 0; i < (512 / sizeof(atf_dir_t)); i++) {
        if(libc_memcmp(atf_head->atf_dir[i].filename, BER_HCI_CMD_BIN, 10) == 0) {
            break;
        }
    }

    g_ber_control->berdata_len = atf_head->atf_dir[i].length;

    /** ��ȡHCI����
    */
    if(i < (512 / sizeof(atf_dir_t))) {
        if(g_ber_control->berdata_len <= ATT_MPDATA_MEX_LENGTH) {
            att_test_fread(data_buffer, atf_head->atf_dir[i].length, atf_head->atf_dir[i].offset);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 ����: ֪ͨPC���߿�ʼ����
 ����:
 ����:
 ˵��:
*/
static uint8  _ber_test_start_att(ber_test_arg_t *ber_arg, int channel)
{
    uint32 ret_val;

    ber_param_t ber_param;


    /** ֪ͨATT����START
    */
    libc_memset(&ber_param, 0, sizeof(ber_param_t));
    ber_param.ic_type = MP_ICTYPE;
    ber_param.channel = channel;
    ber_param.tx_gain_idx = MP_TX_GAIN_IDX;
    ber_param.tx_gain_val = MP_TX_GAIN_VAL;
    //ber_param.payload = PAYLOADTYPE_SET;
    ber_param.payload = BT_PAYLOAD_TYPE_1111_0000;
    ber_param.pkt_type = PKTTYPE_SET;
    ber_param.tx_dac = MP_TX_DAC;
    ber_param.whitening_cv = WHITENCOEFF_SET;
    ber_param.pkt_header = PKTHEADER_SET;
    ber_param.tx_pkt_cnt = BER_TX_PKT_CNT;
    ber_param.hit_target_l = HIT_ADDRESS_SET_L;
    ber_param.hit_target_h = HIT_ADDRESS_SET_H;
    ber_param.sut_state = 0;
    ber_param.report_interval = BER_TX_REPORT_INTERVAL;
    ber_param.report_timeout = BER_TX_REPORT_TIMEOUT;

    libc_memcpy((uint8 *)(STUB_ATT_RW_TEMP_BUFFER + sizeof(stub_ext_cmd_t)), &ber_param, sizeof(ber_param_t));
    ret_val = att_write_data(STUB_CMD_ATT_BER_TX_BEGIN, sizeof(ber_param_t), STUB_ATT_RW_TEMP_BUFFER);

    if(ret_val == 0) {
        att_read_data(STUB_CMD_ATT_ACK, 0, STUB_ATT_RW_TEMP_BUFFER);
    }


    return TRUE;
}

uint32 att_modify_rx_channel(uint8 *hci_data, int index, int channel)
{
    hci_data[index] = ((channel & 0x7f) | 0x80);

    return TRUE;
}

uint32 att_modify_ber_data_sub(uint8 *hci_data, uint32 data_len, uint32 pkt_index, int channel,
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

            if(j == data_len) {
                cb(data_buffer, j, channel);
                return TRUE;
            }
        }

        data_buffer += cmd_len;
    }

    return FALSE;
}

uint32 att_modify_ber_rx_channel(int channel)
{
    att_modify_ber_data_sub(hci_rx_channel_cmd, 5, PKT_CFO_BEGINE_INDEX, channel,
                            att_modify_rx_channel);

    return TRUE;
}


/**
 ����: ģ����Գ�ʼ��
 ����: ber_test_arg  �༭�������õĲ�����Ϣ
 ����:
 ˵��:
*/
uint8 _ber_test_init(ber_test_arg_t *ber_arg, int channel)
{
    uint32 retry_cnt;
    uint32 ret_val;
    bt_drv_param_t tmp_bt_drv_param;
    btt_priv_data_t btt_priv_data;

    /** ��ʼ��ȫ�ֿ��ƽṹ
    */
    g_ber_control = (ber_control_t *)ATT_RINGBUFFER_ADDR;
    libc_memset(g_ber_control, 0, sizeof(ber_control_t));
    g_ber_control->get_pktinfo_timer = -1;
    g_ber_control->ber_test_state = BER_STATE_STOP;

    retry_cnt = 0;

retry:
    if(sys_get_drv_install_info(DRV_GROUP_BT) > 0) {
        sys_drv_uninstall(DRV_GROUP_BT);
    }

    /** ��װ��BT����
    */
    tmp_bt_drv_param.cbk = _ber_test_read_hci_data;
    tmp_bt_drv_param.bd_addr[0] = 0x00;
    tmp_bt_drv_param.bd_addr[1] = 0x01;
    tmp_bt_drv_param.bd_addr[2] = 0x02;
    tmp_bt_drv_param.bd_addr[3] = 0xfd;
    tmp_bt_drv_param.bd_addr[4] = 0x4e;
    tmp_bt_drv_param.bd_addr[5] = 0xf4;
    tmp_bt_drv_param.uart_idle_timer = 5000;
    tmp_bt_drv_param.p_hci_buffer_get_len_cb = &(g_ber_control->g_hci_deal.get_data_len);
    tmp_bt_drv_param.p_hci_buffer_read_data_cb = &(g_ber_control->g_hci_deal.read_data);
    sys_drv_install(DRV_GROUP_BT, &tmp_bt_drv_param, bt_drv_name);

    bt_drv_controller_poweron();

    sys_mdelay(100);

    /** ����PATCH CODE
    */
    btt_priv_data.download_patch = FALSE;
    btt_priv_data.reset_controller = TRUE;
    bt_drv_down_patchcode(TRUE, BT_BTT_MODE, &btt_priv_data);

    /** pkt typeΪ1 bytes
    */
    g_ber_control->g_hci_deal.minReqLen = 3;
    g_ber_control->g_hci_deal.reqRxParseStatus = UART_HCI_STATUS_RX_TYPE;
    g_ber_control->cmd_state = BER_CMD_STATE_WRITE_PACKET_NUM;

    /** ����berdata.bin
    */
    if(_ber_test_read_berdata(ber_arg) == FALSE) {
        return FALSE;
    }

    att_modify_ber_rx_channel(channel);

    /** ����RX begin
    */
    ret_val = _ber_test_write_berdata(PKT_CFO_BEGINE_INDEX);

    if(ret_val == FALSE) {
        retry_cnt++;

        if(retry_cnt < 3) {
            print_log("retry start ber %d", retry_cnt);
            goto retry;
        } else {
            return FALSE;
        }
    }
    g_ber_control->ber_test_state = BER_STATE_START;

    return TRUE;
}

int32 do_ber_test(ber_test_arg_t *ber_arg, int channel, int test_rssi)
{
    int32 ret_val;

    int32 retry_cnt = 0;

retry:
    /** ����SUT��tx pkt����
    */
    if(_ber_test_start_att(ber_arg, channel) == FALSE) {
        print_log("ber_test start fail !");
        return FALSE;
    }

    sys_mdelay(20);

    /** �����ͱ�����ʼ��
    */
    if(_ber_test_init(ber_arg, channel) == FALSE) {
        print_log("ber_test init fail !");
        return FALSE;
    }

    if(test_rssi) {
        g_ber_control->rssi_test = TRUE;

        print_log("rssi att start!");
    } else {
        g_ber_control->rssi_test = FALSE;

        print_log("ber att start!");
    }

    sys_mdelay(20);

    ret_val = ber_test_loop_deal(ber_arg, channel);

    retry_cnt++;

ber_test_end:
    /** finish test
    */
    _ber_test_stop_att(ber_arg, ret_val);

    if(sys_get_drv_install_info(DRV_GROUP_BT) > 0) {
        bt_drv_controller_poweroff();

        sys_drv_uninstall(DRV_GROUP_BT);
    }

    if(retry_cnt < 3 && ret_val == FALSE) {
        goto retry;
    }

    return ret_val;
}


/**
 ����: ����������������
 ����: arg_buffer  �༭�������õĲ�����Ϣ
 ����: ���Խ��
 ˵��: 1. ʹ��Golden sample������ȷ����������;
       2. ����������Ե�ʵ���ϲ���������,������������صĿ�����һ���Բο���ֵ;
*/
test_result_e act_test_ber_test(void *arg_buffer)
{
    uint32 i;

    uint32 test_channel_num;

    ber_test_arg_t *ber_arg;

    int32 ret_val;

    print_log("enter ber_test !");

    ber_arg = (ber_test_arg_t *)arg_buffer;

    for(i = 0; i < 3; i++) {
        if(ber_arg->ber_channel[i] != 0xff) {
            test_channel_num++;
        }
    }

    for(i = 0; i < 3; i++) {
        if(ber_arg->ber_channel[i] == 0xff) {
            continue;
        }

        ret_val = do_ber_test(ber_arg, ber_arg->ber_channel[i], 0);

        if(ret_val != TRUE) {
            break;
        }
    }

    _ber_test_report_result(ber_arg, ret_val);

    return TEST_PASS;
}


/**
 ����: ����������������
 ����: arg_buffer  �༭�������õĲ�����Ϣ
 ����: ���Խ��
 ˵��: 1. ʹ��Golden sample������ȷ����������;
       2. ����������Ե�ʵ���ϲ���������,������������صĿ�����һ���Բο���ֵ;
*/
test_result_e act_test_rssi_test(void *arg_buffer)
{
    uint32 i;

    uint32 test_channel_num;

    ber_test_arg_t *ber_arg;

    int32 ret_val;

    print_log("enter rssi_test !");

    ber_arg = (ber_test_arg_t *)arg_buffer;

    for(i = 0; i < 3; i++) {
        if(ber_arg->ber_channel[i] != 0xff) {
            test_channel_num++;
        }
    }

    for(i = 0; i < 3; i++) {
        if(ber_arg->ber_channel[i] == 0xff) {
            continue;
        }

        ret_val = do_ber_test(ber_arg, ber_arg->ber_channel[i], 1);

        if(ret_val != TRUE) {
            break;
        }
    }

    _ber_test_report_result(ber_arg, ret_val);

    return TEST_PASS;
}

