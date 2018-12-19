/*****************************************************************************
 * Module    : Config
 * File      : config.h
 * Function  : SDK�����ļ�
 *****************************************************************************/

#ifndef USER_FPGA_CONFIG_H
#define USER_FPGA_CONFIG_H
#include "config_define.h"
#include "xcfg.h"
/*****************************************************************************
SDK_V052
standard����Ĭ���õ���IO��(���ù���ʹ��Boombox.setting)����: ����ʱע��IO�Ƿ��г�ͻ.

PA0~PA6: 7������ʾʹ��,Ĭ����ʾΪ GUI_LEDSEG_7P7S
PA7:     ��ӡ������IO (��ӡ������uart0_mapping_sel�йرջ�����ӳ�䵽PB2/PB3)
PB0~PB2: SD���ӿ�(SD0MAP_G2), SD���Ĭ��ʹ�õ���IO���(PE5)
PB3~PB4: USB��
PE0:     MUTE����  (����Boombox.setting���޸�)
PE5:     Ĭ��SD���IO�� (����Boombox.setting���޸�SD��ⷽʽ)
PE6:     Ӳ������
PE7:     ADKEY(USER_ADKEY), Ĭ������10������(���֧��12��) (����Boombox.setting���޸�)
PF0,PF1: AUX (Ĭ����AUX���) (����Boombox.setting���޸�)
PF2:     MICL (����Boombox.setting���޸�)
PF3,PF4: ����LED�� (����Boombox.setting���޸�)
PWRKEY:  ֧��5������ (����Boombox.setting���޸�)
 *****************************************************************************/
/*****************************************************************************
 * Module    : Functionѡ���������
 *****************************************************************************/
#define FUNC_MUSIC_EN                   1   //�Ƿ��MUSIC����
#define FUNC_CLOCK_EN                   1   //�Ƿ��ʱ�ӹ���
#define FUNC_FMRX_EN                    1   //�Ƿ��FM����
#define FUNC_BT_EN                      1   //�Ƿ����������
#define FUNC_BTHID_EN                   1   //�Ƿ�򿪶���������ģʽ
#define FUNC_AUX_EN                     1   //�Ƿ��AUX����
#define FUNC_USBDEV_EN                  1   //�Ƿ��USB DEVICE����
#define FUNC_SPEAKER_EN                 0   //�Ƿ��Speakerģʽ
#define FUNC_SPDIF_EN                   0   //�Ƿ��SPDIF����
#define FUNC_FMAM_FREQ_EN               0   //�Ƿ��FMAM��Ƶ����ʾ����
#define FUNC_IDLE_EN                    0   //�Ƿ��IDLE����

/*****************************************************************************
 * Module    : ϵͳ����ѡ������
 *****************************************************************************/
#define BUCK_MODE_EN                    1                       //�Ƿ�BUCK MODE
#define SYS_CLK_SEL                     SYS_24M                 //ѡ��ϵͳʱ��
#define POWKEY_10S_RESET                xcfg_cb.powkey_10s_reset
#define SOFT_POWER_ON_OFF               1                       //�Ƿ�ʹ�����ػ�����  //�ⲿMOS�����ػ��뿪�� USER_PWRKEY_EXT
#define USB_SD_UPDATE_EN                1                       //�Ƿ�֧��UDISK/SD����������
#define EXT_32K_EN                      0                       //�Ƿ�ʹ���ⲿ32K����,�����ڲ�32K OSC����ػ���Ҫ���ӻ���ʱ����ʹ���ⲿ32K����
#define SYS_ADJ_DIGVOL_EN               0                       //ϵͳ�Ƿ����������, ������������ϵͳ����������������
#define GUI_SELECT                      GUI_LEDSEG_7P7S         //GUI Display Select
#define FLASH_SIZE                      FSIZE_512K              //LQFP48оƬ����1MB��������װоƬ����512KB(ʵ�ʵ���prd�ļ�ҪС��492K)
#define UART0_PRINTF_SEL                PRINTF_PA7             //ѡ��UART��ӡ��Ϣ���IO����رմ�ӡ��Ϣ���

/*****************************************************************************
 * Module    : ���ֹ�������
 *****************************************************************************/
#define MUSIC_UDISK_EN                  1   //�Ƿ�֧�ֲ���UDISK
#define MUSIC_SDCARD_EN                 1   //�Ƿ�֧�ֲ���SDCARD

#define MUSIC_WAV_SUPPORT               1   //�Ƿ�֧��WAV��ʽ����
#define MUSIC_WMA_SUPPORT               0   //�Ƿ�֧��WMA��ʽ����
#define MUSIC_APE_SUPPORT               0   //�Ƿ�֧��APE��ʽ����
#define MUSIC_FLAC_SUPPORT              0   //�Ƿ�֧��FLAC��ʽ����
#define MUSIC_SBC_SUPPORT               0   //�Ƿ�֧��SBC��ʽ����(SD/UDISK��SBC����, �˺겻Ӱ����������)

#define MUSIC_FOLDER_SELECT_EN          1   //�ļ���ѡ����
#define MUSIC_AUTO_SWITCH_DEVICE        1   //˫�豸ѭ������
#define MUSIC_BREAKPOINT_EN             1   //���ֶϵ���䲥��
#define MUSIC_QSKIP_EN                  1   //������˹���
#define MUSIC_PLAYMODE_NUM              4   //���ֲ���ģʽ����
#define MUSIC_MODE_RETURN               1   //�˳�����ģʽ֮���Ƿ񷵻�ԭ����ģʽ
#define MUSIC_PLAYDEV_BOX_EN            1   //�Ƿ���ʾ��USB��, "SD"����
#define MUSIC_ID3_TAG_EN                0   //�Ƿ��ȡMP3 ID3��Ϣ
#define MUSIC_REC_FILE_FILTER           0   //�Ƿ�����¼���ļ����¼���ļ��ֱ𲥷�

#define IPHONE_POWER_VAL                50  //ƻ������������
#define IPHONE_POWER_INDEX              190 //ƻ������������

/*****************************************************************************
 * Module    : ������������
 *****************************************************************************/
#define BT_BACKSTAGE_EN                 0   //������̨����ȫģʽʹ��������
#define BT_BACKSTAGE_PLAY_DETECT_EN     0   //������ģʽ�¼�⵽�ֻ������������֣����л�������ģʽ
#define BT_NAME_DEFAULT                 "BT-BOX"     //Ĭ���������ƣ�������31���ַ���
#define BT_NAME_WITH_ADDR_EN            0   //���������Ƿ񸽼ӵ�ַ��Ϣ�������ã����磺btbox-***��
#define BT_POWER_UP_RECONNECT_TIMES     xcfg_cb.bt_power_up_reconnect_times   //�ϵ��������
#define BT_TIME_OUT_RECONNECT_TIMES     xcfg_cb.bt_time_out_reconnect_times   //���߻�������
#define BT_SIMPLE_PAIR_EN               1   //�Ƿ������������Թ��ܣ��ر�ʱ��Ҫ�ֻ�������PIN�룩
#define BT_DISCOVER_CTRL_EN             0   //�Ƿ�ʹ�ð����򿪿ɱ����֣���һ�°������ܱ�������ԣ�
#define BT_PWRKEY_5S_DISCOVER_EN        0   //�Ƿ�ʹ�ó���5S��������ɱ�����(����������������)
#define BT_DISCOVER_TIMEOUT             100 //�����򿪿ɱ����ֺ󣬶�ú����������Զ��رգ�0���Զ��رգ���λ100ms
#define BT_ANTI_LOST_EN                 0   //�Ƿ��������������
#define BT_CBT_TEST_EN                  0   //����CBT����ʹ��
#define BT_FCC_TEST_EN                  0   //����FCC����ʹ��
#define BT_LOCAL_ADDR                   0   //�����Ƿ�ʹ�ñ��ص�ַ��0ʹ�����ù��ߵ�ַ

#define BT_TWS_EN                       1   //�Ƿ�֧��TWS
#define BT_TWS_M_RECON_EN             	1	//TWS��������ģʽѡ��0��ֻ�����ֻ����� 1�������ֻ��ʹӻ���

#define BT_2ACL_EN                      1   //�Ƿ�֧�����������ֻ�
#define BT_A2DP_EN                      1   //�Ƿ���������ַ���
#define BT_HFP_EN                       1   //�Ƿ������ͨ������
#define BT_SPP_EN                       1   //�Ƿ���������ڷ���
#define BT_HID_EN                       1   //�Ƿ������HID������������
#define BT_PBAP_EN                      0   //�Ƿ�������绰������
#define BT_HID_MENU_EN                  1   //����HID�Ƿ���Ҫ�ֶ�����/�Ͽ�
#define BT_HID_DISCON_DEFAULT_EN        1   //����HID����Ĭ�ϲ����ӣ���Ҫ�ֶ��������ӡ�
#define BT_HFP_CALL_PRIVATE_EN          1   //�Ƿ�ʹ��˽�ܽ��������������л�����
#define BT_HFP_CALL_PRIVATE_FORCE_EN    0   //�Ƿ�ǿ��ʹ��˽�ܽ������ֻ��˽�����
#define BT_HFP_RING_NUMBER_EN           1   //�Ƿ�֧�����籨��
#define BT_HFP_PLAY_IOS_RING_EN         0   //�Ƿ�֧��ios����������androidĬ����RING��ʾ����
#define BT_HFP_BAT_REPORT_EN            1   //�Ƿ�֧�ֵ�����ʾ
#define BT_HFP_MSBC_EN                  0   //�Ƿ�򿪿����������
#define BT_HFP_EC_NR_EN                 0   //�Ƿ���ֻ��˻����ͽ����㷨
#define BT_SCO_DBG_EN                   0   //�Ƿ�����ߵ���ͨ����������
#define BT_A2DP_VOL_CTRL_EN             1   //�Ƿ�֧��A2DP�������ֻ�ͬ��
#define BT_A2DP_RECON_EN                0   //�Ƿ�֧��A2DP���Ƽ�������/��ͣ����������������
#define BT_HSF_CALL_NUMBER_EN           0   //�Ƿ�֧�ֻ�ȡ�ذ�,���������绰�ĺ���

//ͨ������
#define BT_PLC_EN                       1
#define BT_NOISE_THRESHOID              xcfg_cb.bt_noise_threshoid  //����������ֵ�����ڴ���ֵ����Ϊ��������(0~65535)
#define BT_ANL_GAIN                     xcfg_cb.bt_anl_gain         //MICģ������(0~31)
#define BT_DIG_GAIN                     xcfg_cb.bt_dig_gain         //MIC��������(0-31),����0~3DB��Χϸ��,����3/32DB
#define BT_CALL_MAX_GAIN                xcfg_cb.bt_call_max_gain    //����ͨ��ʱDAC���ģ������

#define BT_AEC_EN                       1
#define BT_ECHO_LEVEL                   xcfg_cb.bt_echo_level       //�����������𣨼���Խ�ߣ�����˥��Խ���ԣ���ͨ��Ч��Խ�(0~15)
#define BT_FAR_OFFSET                   xcfg_cb.bt_far_offset       //Զ�˲���ֵ(0~255)

#define BT_ALC_EN                       1                           //�Ƿ�ʹ��ALC
#define BT_ALC_FADE_IN_DELAY            xcfg_cb.bt_alc_in_delay     //���˵�����ʱ
#define BT_ALC_FADE_IN_STEP             xcfg_cb.bt_alc_in_step      //���˵����ٶ�
#define BT_ALC_FADE_OUT_DELAY           xcfg_cb.bt_alc_out_delay    //Զ�˵�����ʱ
#define BT_ALC_FADE_OUT_STEP            xcfg_cb.bt_alc_out_step     //Զ�˵����ٶ�
#define BT_ALC_VOICE_THR                0x50000

#define LE_EN                           0   //�Ƿ��BLE����
#define LE_LIGHTING_EN                  0   //�Ƿ��BLE�ƹ���Ʒ���
#define LE_MUSIC_CTRL_EN                0   //�Ƿ��BLE���ֿ��Ʒ���


/*****************************************************************************
 * Module    : ʱ�ӹ�������
 *****************************************************************************/
#define ALARM_WAKEUP_EN                 0   //�Ƿ�ʹ��ALARM��ػ�����(��Ҫʹ���ⲿ32K����)


/*****************************************************************************
 * Module    : FMRX��������
 *****************************************************************************/
#define FMRX_HALF_SEEK_EN               1   //�Ƿ�򿪰��Զ���̨
#define FMRX_INSIDE_EN                  1   //�Ƿ�ʹ����������
#define FMRX_QN8035_EN                  0

///�������оƬ�������
#define FMRX_2_SDADC_EN                 1                               //��������Ƿ��SDADC������ֱͨDAC����SDADC���Ե�EQ��FMRX¼���ȹ��ܡ�
#define FMRX_CHANNEL_CFG               (CH_AUXL_PF0 | CH_AUXR_PF1)      //ѡ��FMRX��ͨ·
#define FMRX_AUX_ANL_GAIN               AUX_N0DB                        //AUX�����ģ������
#define FMRX_CLOCK                      FM_USE_CRYSTAL_32K
#define FMRX_CLKOMAP                    CLKOMAP_PB1

///���������Ĳ�������
#define FMRX_RFCAP_VAL                  xcfg_cb.fmrx_rfcap_val          //��Χ0~31, �����ⲿ��е�ƥ����ݡ����ⲿ���ʱ������Ϊ0��
#define FMRX_THRESHOLD_VAL              xcfg_cb.fmrx_threshold_val      //����FMRX��̨��ֵ, ֵԽС̨Խ�࣬��̨Ҳ�������ࡣ
#define FMRX_THRESHOLD_Z                1700                            //����ٺ�����̨�������ʵ���С
#define FMRX_THRESHOLD_FZ               800                             //����ٺ�����̨�������ʵ���С
#define FMRX_THRESHOLD_D                3000

/*****************************************************************************
 * Module    : FMTX��������
 *****************************************************************************/
#define FMTX_EN                         0   //�Ƿ�ʹ��FM TX����
#define FMTX_POWER                      1   //���书��ѡ��1���� 0��С
#define FMTX_DAC_EN                     1   //�Ƿ��DAC
#define FMTX_FIX_BT                     0   //�Ƿ�������Ż�ѡ�ѡС���书����ϣ�

/*****************************************************************************
* Module    : AUX��������
******************************************************************************/
#define AUX_CHANNEL_CFG                 (CH_AUXL_PF0 | CH_AUXR_PF1) //ѡ��LINEINͨ·
#define MIC_CHANNEL_CFG                 CH_MICL0                    //ѡ��MIC��ͨ·
#define AUX_2_SDADC_EN                  1                           //AUX�Ƿ��SDADC, ����ֱͨDAC����SDADC���Ե�AUX EQ, ��AUX¼���ȹ���
#define LINEIN_DETECT_EN                1                           //�Ƿ��LINEIN���
#define SDCMD_MUX_DETECT_LINEIN         1                           //�Ƿ���SDCMD���LINEIN����
#define MICL_MUX_DETECT_LINEIN          1                           //�Ƿ���MICL/PF2���LINEIN����
#define LINEIN_2_PWRDOWN_EN             0                           //�Ƿ����Linein��ֱ����ػ�����������ܣ�

///ͨ�����ù���ѡ����GPIO
#define LINEIN_DETECT_INIT()            linein_detect_init()
#define LINEIN_IS_ONLINE()              linein_is_online()
#define IS_DET_LINEIN_BUSY()            is_detect_linein_busy()


/*****************************************************************************
* Module    : DAC���ÿ���
******************************************************************************/
#define DAC_CH_SEL                      xcfg_cb.dac_sel         //DAC_MONO ~ DAC_VCMBUF_DUAL
#define DAC_FAST_SETUP_EN               0                       //DAC�����ϵ磬��������Ҫ�ⲿ����MUTE
#define DAC_MAX_GAIN                    xcfg_cb.dac_max_gain    //����DAC���ģ�����棬Ĭ������Ϊdac_vol_table[VOL_MAX]
#define DAC_OUT_SPR                     DAC_OUT_44K1            //dac out sample rate
#define DAC_LDOH_SEL                    xcfg_cb.dac_ldoh_sel
#define DAC_VCM_CAPLESS_EN              xcfg_cb.dac_vcm_less_en //DAC VCMʡ���ݷ���,ʹ���ڲ�VCM
#define DAC_PULL_DOWN_DELAY             80                      //����DAC��ֱ���ݵķŵ�ʱ��, �޵���ʱ����Ϊ0�����ٿ���ʱ�䡣
#define DAC_LOWPWR_EN                   0
#define DAC_DNR_EN                      1                       //�Ƿ�ʹ�ܶ�̬����
#define DAC_AUCAR_EN                    0                       //�Ƿ�����������ǰ�������������ڼ���Ч����
#define DAC_DRC_EN                      0                       //�Ƿ�ʹ��DRC���ܣ�ϵͳʱ����Ҫ����48MHz��

/*****************************************************************************
 * Module    : ¼����������
 *****************************************************************************/
#define FUNC_REC_EN                     0   //¼�������ܿ���
#define FMRX_REC_EN                     0   //�Ƿ��FM¼������
#define AUX_REC_EN                      0   //�Ƿ��AUX¼������
#define MIC_REC_EN                      0   //�Ƿ��MIC¼������
#define SPF_REC_EN                      0   //�Ƿ��SPDIF¼������
#define BT_REC_EN                       0   //�Ƿ����������¼������
#define BT_HFP_REC_EN                   0   //�Ƿ������ͨ��¼�����ܣ���֧��karaok��
#define KARAOK_REC_EN                   0   //�Ƿ��KARAOK¼�����ܣ���ͬ����KARAOK����
#define KARAOK_REC_NO_ECHO_EN           0   //¼���Ƿ�¼����
#define REC_ONE_FOLDER_EN               0   //�Ƿ��ģʽ¼���ŵ�ͬһĿ¼��
#define REC_DIR_LFN                     1   //�Ƿ�򿪳��ļ���Ŀ¼
#define REC_AUTO_PLAY                   1   //¼�������Ƿ�طŵ�ǰ¼������
#define REC_FAST_PLAY                   0   //������U�¿��ٲ������µ�¼���ļ�(˫��REC)
#define REC_STOP_MUTE_1S                0   //¼��ֹͣʱ, MUTE 1S����. //���ѿͻ�¼������.
#define REC_TYPE_SEL                    REC_MP3


#define REC_TO_SPIFALSH                 0
#if REC_TO_SPIFALSH    //TEST
#undef GUI_SELECT
#undef FLASH_SIZE
#undef FUNC_REC_EN
#undef MIC_REC_EN

#define GUI_SELECT                      GUI_NO
#define FLASH_SIZE                      FSIZE_1M
#define FUNC_REC_EN                     1
#define MIC_REC_EN                      1
#endif
/*****************************************************************************
 * Module    : K�蹦������
 *****************************************************************************/
#define SYS_KARAOK_EN                   0   //�Ƿ��K�蹦��
#define SYS_KARAOK_BUFMODE              0   //ʹ�ý��������ӱ��������
#define BT_HFP_CALL_KARAOK_EN           0   //ͨ���Ƿ�֧��KARAOK.
#define SYS_ECHO_EN                     1   //�Ƿ�ʹ�ܻ��칦��
#define SYS_ECHO_DELAY                  16  //����������
#define SYS_BASS_TREBLE_EN              0   //�Ƿ�ʹ�ܸߵ�������
#define SYS_MAGIC_VOICE_EN              1   //�Ƿ�ʹ��ħ������
#define SYS_HOWLING_EN                  1   //�Ƿ�ʹ�ܷ�Х�й���

#define MIC_DETECT_EN                   0   //�Ƿ�ʹ��MIC���
#define MIC_DNR_EN                      1   //�Ƿ�ʹ��MIC��̬����
#define DUAL_MIC_CHANNEL_EN             0   //˫MICʹ��.  //SYS_KARAOK_ENΪ1ʱ��Ч

///ͨ�����ù���ѡ����GPIO
#define MIC_DETECT_INIT()               mic_detect_init()
#define MIC_IS_ONLINE()                 mic_is_online()
#define IS_DET_MIC_BUSY()               is_detect_mic_busy()

/*****************************************************************************
 * Module    : User�������� (����ͬʱѡ����鰴��)
 *****************************************************************************/
#define USER_ADKEY                      1           //ADKEY��ʹ�ã� 0Ϊ��ʹ��
#define USER_ADKEY2                     0           //ADKEY2��ʹ�ã�0Ϊ��ʹ��
#define USER_PWRKEY                     1           //PWRKEY��ʹ�ã�0Ϊ��ʹ��
#define USER_IOKEY                      0           //IOKEY��ʹ�ã� 0Ϊ��ʹ��
#define USER_ADKEY_MUX_SDCLK            0           //�Ƿ�ʹ�ø���SDCLK��ADKEY, ����USER_ADKEY�İ���table
#define ADKEY_PU10K_EN                  0           //ADKEY�Ƿ�ʹ���ڲ�10K����, ������������ֵ��port_key.c
#define USER_ADKEY_MUX_LED              0           //�Ƿ�ʹ��ADKEY��LED����, ����USER_ADKEY������(ADKEY��BLED����ͬһIO)

#define USER_KEY_KNOB_EN                0           //��ť��ʹ�ã�0Ϊ��ʹ��
#define USER_KEY_KNOB_LEVEL             16          //��ť�ļ���

#define USER_KEY_DOUBLE_EN              1           //����˫��ʹ��
#define USER_KEY_THRICE_EN              0           //�����������ܣ���Ҫ�ȿ�˫������
#define USER_KEY_DOUBLE                 {KU_HSF, KU_PREV_VOL_DOWN, KU_NEXT_VOL_UP}     //֧��˫���İ���
#define ADKEY_CH                        xcfg_cb.adkey_ch    //ADCCH_PE7
#define ADKEY2_CH                       xcfg_cb.adkey2_ch   //ADCCH_PE6

#define PWRON_PRESS_TIME                (1500+500*xcfg_cb.pwron_press_time)             //����PWRKEY�೤ʱ�俪����
#define PWROFF_PRESS_TIME               (3+3*xcfg_cb.pwroff_press_time)                 //����PWRKEY�೤ʱ��ػ���
#define DOUBLE_KEY_TIME                 (xcfg_cb.double_key_time)

#define USER_PWRKEY_EXT                 0           //�Ƿ�ʹ���ⲿ���ػ�����, ��Ҫ�������MOS�ܵ�IO��.
#define IS_PWRKEY_PRESS()               0 //(0 == (GPIOA & BIT(3)))
#define PWRKEY_EXT_INIT()               //{GPIOADE |= BIT(3); GPIOASET = BIT(3); GPIOADIR &= ~BIT(3);}
#define PWRKEY_EXT_POWEROFF()           //{GPIOACLR = BIT(3); while(1) { WDT_CLR();}}
#define PWRKEY_EXT_SLEEP_HOLD()         //GPIOADE = BIT(3);   //����ң��������Ҫ�����е�

/*****************************************************************************
 * Module    : SD0����
 *****************************************************************************/
#define SD_SOFT_DETECT_EN               1           //�Ƿ�ʹ�������� (SD��������)
#define SDCLK_MUX_DETECT_SD             1           //�Ƿ���SDCLK���SD��
#define SD0_MAPPING                     SD0MAP_G2   //ѡ��SD0 mapping

///ͨ�����ù���ѡ����GPIO
#define SD_DETECT_INIT()                sdcard_detect_init()
#define SD_IS_ONLINE()                  sdcard_is_online()
#define IS_DET_SD_BUSY()                is_det_sdcard_busy()
#define SD_IS_SOFT_DETECT()             (xcfg_cb.sddet_iosel == 31)  //���ù�����ѡ��31��������.

/*****************************************************************************
 * Module    : I2C����
 *****************************************************************************/
#define I2C_EN                          0           //�Ƿ�ʹ��I2C����
#define I2C_MUX_SD_EN                   1           //�Ƿ�I2C����SD����IO

#define I2C_SCL_IN()                    SD_CMD_DIR_IN()
#define I2C_SCL_OUT()                   SD_CMD_DIR_OUT()
#define I2C_SCL_H()                     SD_CMD_OUT_H()
#define I2C_SCL_L()                     SD_CMD_OUT_L()

#define I2C_SDA_IN()                    SD_DAT_DIR_IN()
#define I2C_SDA_OUT()                   SD_DAT_DIR_OUT()
#define I2C_SDA_H()                     SD_DAT_OUT_H()
#define I2C_SDA_L()                     SD_DAT_OUT_L()
#define I2C_SDA_IS_H()                  SD_DAT_STA()

#define I2C_SDA_SCL_OUT()               {I2C_SDA_OUT(); I2C_SCL_OUT();}
#define I2C_SDA_SCL_H()                 {I2C_SDA_H(); I2C_SCL_H();}

/*****************************************************************************
 * Module    : I2S����
 *****************************************************************************/
#define I2S_EN                          0           //�Ƿ�ʹ��I2S����
#define I2S_DEVICE                      I2S_DEV_NO //I2S�豸ѡ��
#define I2S_MAPPING_SEL                 I2S_GPIOA   //I2S IO��ѡ��
#define I2S_MODE_SEL                    0           //I2S����ģʽѡ�� 0: master; 1:slave
#define I2S_BIT_MODE                    0           //I2S����λ��ѡ�� 0:16bit; 1:32bit
#define I2S_DATA_MODE                   1           //I2S���ݸ�ʽѡ�� 0:left-justified mode; 1:normal mode
#define I2S_DMA_EN                      0           //I2S����Դѡ�� 0:src; 1:dma
#define I2S_MCLK_EN                     1           //I2S�Ƿ��MCLK
#define I2S_MCLK_SEL                    2           //I2S MCLKѡ�� 0:64fs 1:128fs 2:256fs
#define I2S_PCM_MODE                    0           //I2S�Ƿ��PCM mode

/*****************************************************************************
 * Module    : SPDIF����
 *****************************************************************************/
 #define SPDIF_CH                       SPF_PE6_CH4

/*****************************************************************************
 * Module    : IRRX����
 *****************************************************************************/
#define IRRX_HW_EN                      1           //�Ƿ��IRRXӲ��ģ��
#define IRRX_SW_EN                      0           //�Ƿ��timer capture ir
#define IR_NUMKEY_EN                    1           //�Ƿ�����ּ�����
#define IR_INPUT_NUM_MAX                999         //�����������9999

//���Դ�һ������
#define IR_ADDR_FF00_EN                 1
#define IR_ADDR_BF00_EN                 0
#define IR_ADDR_FD02_EN                 1
#define IR_ADDR_FE01_EN                 0
#define IR_ADDR_7F80_EN                 0

#define IR_CAPTURE_PORT()               {GPIOEDE |= BIT(6); GPIOEPU  |= BIT(6); GPIOEDIR |= BIT(6);}
#define IRRX_MAPPING                    IRMAP_PE6
#define TMR3CAP_MAPPING                 TMR3MAP_PE6


/*****************************************************************************
 * Module    : usb device ����ѡ��
 *****************************************************************************/
#define UDE_STORAGE_EN                 1
#define UDE_SPEAKER_EN                 1
#define UDE_HID_EN                     1
#define UDE_MIC_EN                     1


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
#define PWM_RGB_EN                      0           //PWM RGB��ɫ�ƹ���
#define ENERGY_LED_EN                   0           //���������PWM��ʾ,����Խ��,�����ĵ�Խ��.
#define SYS_PARAM_RTCRAM                0           //�Ƿ�ϵͳ�������浽RTCRAM
#define PWRON_ENTER_BTMODE_EN           0           //�Ƿ��ϵ�Ĭ�Ͻ�����ģʽ
#define VBAT_DETECT_EN                  1           //��ص�����⹦��
#define VBAT2_ADCCH                     ADCCH_VBAT  //ADCCH_VBATΪ�ڲ�1/2��ѹͨ·������ѹӦ����Ҫ�ⲿADCͨ·���1/2��ص�ѹ.
#define VBAT_DETECT_MUX_SDCLK           0           //��ص�����⸴��SDCLK,  ����ص�1/2��ѹ. ��ؽ�100K������ʵ��1/2��ѹ.

#define VBAT_FILTER_USE_PEAK            0           //��ؼ���˲�ѡ��://0 ȡƽ��ֵ.//1 ȡ��ֵ(�����ڲ�������ʱ,��ز����Ƚϴ�����䷽��).
#define EQ_MODE_EN                      1           //�Ƿ����EQ MODE (POP, Rock, Jazz, Classic, Country)
#define EQ_DBG_IN_UART                  1           //�Ƿ�ʹ��UART���ߵ���EQ
#define EQ_DBG_IN_SPP                   1           //�Ƿ�ʹ��SPP���ߵ���EQ
#define SYS_EQ_FOR_IDX_EN               0           //�Ƿ�ʹ��10��EQ��������(�����ߵ���)
#define AUCAR_EQ_LOUD_BASS_TREBLE_EN    0           //����EQ/LOUD/BASS/TRMͬʱ��Ч.   //һ����Ҫͬʱ�� SYS_ADJ_DIGVOL_EN / DAC_AUCAR_EN

#define SLEEP_DAC_OFF_EN                (is_sleep_dac_off_enable()) //sfunc_sleep�Ƿ�ر�DAC�� ����MICL��ⷽ�����ܹ�DAC��
#define SYS_INIT_VOLUME                 xcfg_cb.sys_init_vol        //ϵͳĬ������
#define LPWR_WARNING_VBAT               xcfg_cb.lpwr_warning_vbat   //�͵����ѵ�ѹ
#define LPWR_OFF_VBAT                   xcfg_cb.lpwr_off_vbat       //�͵�ػ���ѹ
#define LPWR_WARING_TIMES               0xff                           //���͵����

/*****************************************************************************
 * Module    : LEDָʾ������
 *****************************************************************************/
#define LED_DISP_EN                     1           //�Ƿ�ʹ��LEDָʾ��(����)
#define LED_PWR_EN                      1           //��缰��Դָʾ��(���)
#define LED_LOWBAT_EN                   0                           //��ص͵��Ƿ������
#define BLED_CHARGE_FULL_EN             xcfg_cb.charge_full_bled    //������Ƿ�������
#define BT_RECONN_LED_EN                0           //��������״̬�Ƿ�ͬ�����Ʒ�ʽ

#define LED_INIT()                      bled_func.port_init(&bled_gpio)
#define LED_SET_ON()                    bled_func.set_on(&bled_gpio)
#define LED_SET_OFF()                   bled_func.set_off(&bled_gpio)

#define LED_PWR_INIT()                  rled_func.port_init(&rled_gpio)
#define LED_PWR_SET_ON()                rled_func.set_on(&rled_gpio)
#define LED_PWR_SET_OFF()               rled_func.set_off(&rled_gpio)

/*****************************************************************************
 * Module    : Loudspeaker mute�������������
 *****************************************************************************/
#define EARPHONE_DETECT_EN              1           //�Ƿ�򿪶������
#define SDCMD_MUX_DETECT_EARPHONE       1           //�Ƿ���SDCMD����������

///ͨ�����ù���ѡ����GPIO
#define EARPHONE_DETECT_INIT()          earphone_detect_init()
#define EARPHONE_IS_ONLINE()            earphone_is_online()
#define IS_DET_EAR_BUSY()               is_detect_earphone_busy()

#define LOUDSPEAKER_MUTE_EN             1           //�Ƿ�ʹ�ܹ���MUTE
#define LOUDSPEAKER_MUTE_INIT()         loudspeaker_mute_init()
#define LOUDSPEAKER_MUTE_DIS()          loudspeaker_disable()
#define LOUDSPEAKER_MUTE()              loudspeaker_mute()
#define LOUDSPEAKER_UNMUTE()            loudspeaker_unmute()
#define LOUDSPEAKER_UNMUTE_DELAY        6           //UNMUTE��ʱ���ã���λΪ5ms

#define AMPLIFIER_SEL_INIT()            amp_sel_cfg_init(xcfg_cb.ampabd_io_sel)
#define AMPLIFIER_SEL_D()               amp_sel_cfg_d()
#define AMPLIFIER_SEL_AB()              amp_sel_cfg_ab()


/*****************************************************************************
 * Module    : ��繦��ѡ��
 *****************************************************************************/
#define CHARGE_EN                       0           //�Ƿ�򿪳�繦��
#define CHARGE_TRICK_EN                 xcfg_cb.charge_trick_en     //�Ƿ�������繦��
#define CHARGE_DC_RESET                 xcfg_cb.charge_dc_reset     //�Ƿ��DC���븴λ����
#define CHARGE_DC_NOT_PWRON             xcfg_cb.charge_dc_not_pwron //DC���룬�Ƿ������� 1: DC INʱ���ܿ���
#define CHARGE_DC_IN()                  charge_dc_detect()//((RTCCON >> 20) & 0x01)

//����ֹ������0:2.5mA; 1:5mA; 2:10mA; 3:15mA; 4:20mA; 5:25mA; 6:30mA; 7:35mA
#define CHARGE_STOP_CURR                xcfg_cb.charge_stop_curr    //0~7
//����ֹ��ѹ��0:3.9v��1:4v��2:4.1v 3:4.15v
#define CHARGE_STOP_VOLT                3    //0~3
//������磨��ص�ѹ����2.9v��������2:30mA; 3:40mA; 4:50mA; 5:60mA; 6:70mA; 7:80mA; 8:90mA; 9:100mA;
//                                  10:110mA; 11:120mA; 12:140mA; 13:160mA; 14:180mA; 15:200mA
#define CHARGE_CONSTANT_CURR            (xcfg_cb.charge_constant_curr+2)
//�����ֹ��ѹ��0:2.9v; 1:3v
#define CHARGE_TRICK_STOP_VOLT          1
//�����磨��ص�ѹС��2.9v��������2:30mA; 3:40mA; 4:50mA;
#define CHARGE_TRICKLE_CURR             (xcfg_cb.charge_trickle_curr+2)


/*****************************************************************************
 * Module    : ��ʾ�� ����ѡ��
 *****************************************************************************/
#define WARNING_TONE_EN                 1               //�Ƿ����ʾ������, �ܿ���
#define WARING_MAXVOL_MP3               0               //���������ʾ��WAV��MP3ѡ�� ����WAV������MUSIC���Ӳ��š�
#define WARNING_VOLUME                  xcfg_cb.warning_volume   //������ʾ������������
#define LANG_SELECT                     LANG_EN      //��ʾ������ѡ��

#define WARNING_POWER_ON                1
#define WARNING_POWER_OFF               1
#define WARNING_FUNC_MUSIC              0
#define WARNING_FUNC_BT                 1
#define WARNING_FUNC_CLOCK              1
#define WARNING_FUNC_FMRX               1
#define WARNING_FUNC_AUX                1
#define WARNING_FUNC_USBDEV             1
#define WARNING_FUNC_SPEAKER            1
#define WARNING_FUNC_SPDIF              0
#define WARNING_LOW_BATTERY             1
#define WARNING_BT_WAIT_CONNECT         1
#define WARNING_BT_CONNECT              1
#define WARNING_BT_DISCONNECT           1
#define WARNING_BT_INCALL               1
#define WARNING_USB_SD                  1
#define WARNING_MAX_VOLUME              1
#define WARNING_MIN_VOLUME              0
#define WARNING_ADJ_VOLUME              0
#define WARNING_BT_HID_MENU             1            //BT HID MENU�ֶ�����/�Ͽ�HID Profile��ʾ��
#define WARNING_BTHID_CONN              0            //BTHIDģʽ�Ƿ��ж���������/�Ͽ���ʾ��
#define WARNING_BT_PAIR                 0            //BT PAIRING��ʾ��


#include "config_extra.h"

#endif // USER_FPGA_CONFIG_H
