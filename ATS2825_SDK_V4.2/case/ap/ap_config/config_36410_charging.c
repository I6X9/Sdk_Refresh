/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������ⲿ��緽���Ŀ���ģʽ������ģʽ�����˲���USB�ϵ���������ģʽ���Լ�����
 *       ΪON״̬ʱ����������ģʽ���ģʽ���ڸ�ģʽ�г��˼�⵽���ز���ON״̬�л���
 *       ��������ģʽ֮�⣬����Ӧ�κ������¼���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  �ⲿ��緽���Ŀ���ģʽ�Ĵ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void enable_onoff_key_reset_function(int enable)
{
    if(enable)
        act_writel(act_readl(ONOFF_KEY) | (1 << ONOFF_KEY_ONOFF_RST_EN), ONOFF_KEY);
    else
        act_writel(act_readl(ONOFF_KEY) & (~(1 << ONOFF_KEY_ONOFF_RST_EN)), ONOFF_KEY);
}

app_result_e config_36410_charging_mode_loop(void)
{
    app_result_e result = RESULT_NULL;
    msg_apps_t msg;

    enable_onoff_key_reset_function(0);         //disable reset function
    libc_print("enter charging mode...", 0, 0);
    //�������ģʽ
    set_cur_func_id(APP_FUNC_IDLE_MODE);
    while(1) {
        result = com_view_loop();

        if(IS_EXTERN_CAHREGE_IN() == 0) {
            result = RESULT_POWER_OFF;
            break;
        }

        if((act_readl(ONOFF_KEY) & (1 << ONOFF_KEY_ONOFF_PRESS_0)) == 0) {
            //restart system
            enable_onoff_key_reset_function(1);         //enable reset function
            libc_print("charging mode reset to normal", 0, 0);
            ENABLE_WATCH_DOG(0x00);
            while(1);
            break;
        }
        //����10ms�����������
        sys_os_time_dly(1);
        CLEAR_WATCH_DOG;
    }
    enable_onoff_key_reset_function(1);         //enable reset function

    return result;
}

