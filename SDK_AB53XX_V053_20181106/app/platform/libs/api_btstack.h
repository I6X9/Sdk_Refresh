#ifndef _API_BTSTACK_H
#define _API_BTSTACK_H

//��������
#define PROF_A2DP                       0x0007  //�������ֹ���
#define PROF_HID                        0x0018  //�������̹���
#define PROF_HFP                        0x0060  //����ͨ������
#define PROF_SPP                        0x0080  //�������ڹ���
#define PROF_PBAP                       0x0100  //�����绰������

//��������
#define HFP_BAT_REPORT                  0x01    //�Ƿ�֧���ֻ�������ʾ
#define HFP_3WAY_CALL                   0x02    //�Ƿ�֧����ͨ�绰
#define HFP_IOS_RING                    0x04    //�Ƿ�֧��IOS��������
#define HFP_CALL_PRIVATE                0x08    //�Ƿ��ǿ��˽�ܽ���
#define HFP_SIRI_CMD                    0x10    //�Ƿ��siri��������
#define HFP_EC_AND_NR                   0x20    //�Ƿ���ֻ��˻����ͽ���
#define A2DP_VOL_CTRL                   0x01    //�Ƿ�֧���ֻ���������
#define A2DP_RECON_CTRL                 0x02    //�Ƿ��A2DP���Ƽ�����

//AVRCP��Ϣ
#define AVC_VOLUME_UP               0x41
#define AVC_VOLUME_DOWN             0x42
#define AVC_MUTE                    0x43
#define AVC_PLAY                    0x44
#define AVC_STOP                    0x45
#define AVC_PAUSE                   0x46
#define AVC_RECORD                  0x47
#define AVC_REWIND                  0x48
#define AVC_FAST_FORWARD            0x49
#define AVC_EJECT                   0x4a
#define AVC_FORWARD                 0x4b
#define AVC_BACKWARD                0x4c

//��չ��־
#define BT_FLAG0_TWS_SCAN           0x01
#define BT_FLAG0_CLK_ADVANCE        0x02
#define BT_FLAG0_TWS_M_RECON        0x04
#define BT_FLAG0_TWS_FAST_RECON     0x08
#define BT_FLAG0_TWS_MENU_PAIR      0x10    //�ֶ�����TWS����, ����tws api

#define BT_FLAG1_LTX_RECON          0x01

typedef struct
{
    u8 txpwr;                       //0=4dbm, 1=3dbm, 2=2dbm, 3=1dbm, 4=0dbm
    u8 packet;                      //0=LQFP48, 1=QFN32, 3=, 4=, 5=TSSOP24, 6=SOP16
    u8 res[6];
} rf_cfg_t;

typedef struct {
    u8 pwrup_connect_times;
    u8 tout_connect_times;
    u16 profile;
    u8 max_acl_link;
    u8 max_nor_link;
    u8 a2dp_feature;
    u8 hfp_feature;
    u8 vx_set;
    u16 scan_timeout;
    u8 res[2];

    u16 dual_mode           : 1;
    u16 tws_mode            : 1;
    u16 fcc_test_mode       : 1;
    u16 cbt_test_mode       : 1;
    u16 simple_pair_mode    : 1;
    u16 scan_ctrl           : 1;

    u16 sco_plc_en          : 1;
    u16 sco_msbc_en         : 1;
} bt_cfg_t;

//����״̬
enum {
    BT_STA_OFF,                                 //����ģ���ѹر�
    BT_STA_INITING,                             //��ʼ����
    BT_STA_IDLE,                                //����ģ��򿪣�δ����

    BT_STA_SCANNING,                            //ɨ����
    BT_STA_DISCONNECTING,                       //�Ͽ���
    BT_STA_CONNECTING,                          //������

    BT_STA_CONNECTED,                           //������
    BT_STA_PLAYING,                             //����
    BT_STA_INCOMING,                            //��������
    BT_STA_OUTGOING,                            //���ں���
    BT_STA_INCALL,                              //ͨ����
    BT_STA_OTA,                                 //OTA������
};

//ͨ��״̬
enum {
    BT_CALL_IDLE,                               //
    BT_CALL_INCOMING,                           //��������
    BT_CALL_OUTGOING,                           //���ں���
    BT_CALL_ACTIVE,                             //ͨ����
    BT_CALL_3WAY_CALL,                          //��ͨ�绰�������ֻ�ͨ��
};

//����֪ͨ
enum {
    BT_NOTICE_INIT_FINISH,                      //������ʼ�����
    BT_NOTICE_DISCONNECT,                       //�����Ͽ�
    BT_NOTICE_LOSTCONNECT,                      //�������Ӷ�ʧ
    BT_NOTICE_CONNECTED,                        //�������ӳɹ�
    BT_NOTICE_INCOMING,                         //����
    BT_NOTICE_RING,                             //��������
    BT_NOTICE_OUTGOING,                         //ȥ��
    BT_NOTICE_CALL,                             //����ͨ��
    BT_NOTICE_SET_SPK_GAIN,                     //����ͨ������
    BT_NOTICE_MUSIC_PLAY,                       //�������ֿ�ʼ����
    BT_NOTICE_MUSIC_STOP,                       //��������ֹͣ����
    BT_NOTICE_MUSIC_CHANGE_VOL,                 //�ı�������������
    BT_NOTICE_MUSIC_SET_VOL,                    //����������������
    BT_NOTICE_HID_CONN_EVT,                     //HID���������¼�
    BT_NOTICE_KEY_PRESS_EVT,                    //Զ�˰�������
    BT_NOTICE_TBOX_EVT,
    BT_NOTICE_TWS_SET_VOL,                      //����TWS��������
    BT_NOTICE_TWS_HID_SHUTTER,                  //Զ��TWS���ռ�
    BT_NOTICE_TWS_INFO,                         //TWSͬ����Ϣ
    BT_NOTICE_TWS_USER_KEY,                     //TWS�Զ��尴��
};

//������Ϣ
enum bt_msg_t {
    BT_MSG_OFF          = 0,                    //�ر�����
    BT_MSG_ON,                                  //������
    BT_MSG_SCAN_ENABLE,                         //��ɨ��
    BT_MSG_SCAN_DISABLE,                        //�ر�ɨ��
    BT_MSG_CONNECT,                             //��������
    BT_MSG_DISCONNECT,                          //�Ͽ�����
    BT_MSG_PLAY_PAUSE,                          //�л����š���ͣ
    BT_MSG_VOL_CHANGE,                          //��������
    BT_MSG_CALL_REDIAL,                         //�ز��绰�����һ��ͨ����
    BT_MSG_CALL_ANSWER_INCOM,                   //��������
    BT_MSG_CALL_TERMINATE,                      //�Ҷ�ͨ��������
    BT_MSG_CALL_SWAP,                           //�л���ͨ�绰
    BT_MSG_HFP_REPORT_BAT,                      //�����ص���
    BT_MSG_HFP_SPK_GAIN,                        //����ͨ������������
    BT_MSG_HFP_MIC_GAIN,                        //����ͨ����˷�����
    BT_MSG_HFP_PRIVATE_SWITCH,                  //�л�˽��ͨ��
    BT_MSG_HID_CONNECT,                         //����HID
    BT_MSG_HID_DISCONNECT,                      //�Ͽ�HID
    BT_MSG_AUDIO_BYPASS,                        //��������SBC/SCO AUDIO
    BT_MSG_AUDIO_ENABLE,                        //ʹ������SBC/SCO AUDIO
    BT_MSG_TWS_SCAN_ENABLE,                     //��TWSɨ�裬���Ӻ���Զ��ر�
    BT_MSG_TWS_SCAN_DISABLE,                    //�ر�TWSɨ��
    BT_MSG_TWS_DISCONNECT,                      //�Ͽ�TWS����
    BT_MSG_TWS_CONNECT,                         //����TWS����
    BT_MSG_TWS_SEARCH_SLAVE,                    //����������TWS����
    BT_MSG_RES2,
    BT_MSG_SWITCH_SIRI,
    BT_MSG_OTA_READ,
    BT_MSG_OTA_STATUS,
    BT_MSG_CALL_REDIAL_NUMBER,                  //�ز��绰����hfp_get_outgoing_number��ȡ���룩
    BT_MSG_HFP_AT_CMD,
    BT_MSG_RES5,
    BT_MSG_RES6,
    BT_MSG_RES7,
    BT_MSG_RES8,
    BT_MSG_RES9,
    BT_MSG_RES10,
    BT_MSG_RES11,
    BT_MSG_RES12,
    BT_MSG_BLE_SEND,
    BT_MSG_RES13,
    BT_MSG_RES14,
    BT_MSG_MAX,

    BT_MSG_HID_KEY      = 0x30,                 //HID����
    BT_MSG_HID_CONSUMER,                        //CONSUMER����
    BT_MSG_TWS_USER_KEY,                        //TWS�û��Զ�����Ϣ
    BT_MSG_PARAM_MAX,

    BT_MSG_PLAY         = AVC_PLAY,             //����
    BT_MSG_PAUSE        = AVC_PAUSE,            //��ͣ
    BT_MSG_STOP         = AVC_STOP,             //ֹͣ
    BT_MSG_PREV         = AVC_BACKWARD,         //��һ��
    BT_MSG_NEXT         = AVC_FORWARD,          //��һ��
    BT_MSG_REWIND       = AVC_REWIND,           //��ʼ����
    BT_MSG_FAST_FORWARD = AVC_FAST_FORWARD,     //��ʼ���
    BT_MSG_REWIND_END       = AVC_REWIND|0x80,  //��������
    BT_MSG_FAST_FORWARD_END = AVC_FAST_FORWARD|0x80, //�������
};

extern bt_cfg_t bt_cfg;

//control
void bt_fcc_init(void);
void bt_init(void);                             //��ʼ����������
int bt_setup(void);                             //������ģ��
void bt_off(void);                              //�ر�����ģ��
void bt_wakeup(void);                           //��������ģ��
void bt_send_msg(uint msg);                     //����������Ϣ���������bt_msg_t
void bt_audio_bypass(void);                     //����SBC/SCOͨ·�ر�
void bt_audio_enable(void);                     //����SBC/SCOͨ·ʹ��
u8 bt_get_packet(u8 index);
uint8_t bt_get_hvbko(u8 rf_packet);

void bt_set_scan(bool enable);                  //��/�رշ���������״̬
void bt_connect(void);                          //�����豸����
void bt_disconnect(void);                       //�����豸�Ͽ�
void bt_hid_connect(void);                      //����HID�������
void bt_hid_disconnect(void);                   //����HID����Ͽ�
int bt_hid_is_connected(void);
bool bt_hid_is_ready_to_discon(void);

//status
uint bt_get_disp_status(void);                  //��ȡ�����ĵ�ǰ��ʾ״̬, V060
uint bt_get_status(void);                       //��ȡ�����ĵ�ǰ״̬
uint bt_get_call_indicate(void);                //��ȡͨ���ĵ�ǰ״̬
bool bt_is_calling(void);                       //�ж��Ƿ�����ͨ��
bool bt_is_playing(void);                       //�ж��Ƿ����ڲ���
bool bt_is_ring(void);                          //�ж��Ƿ���������
bool bt_is_testmode(void);                      //�жϵ�ǰ�����Ƿ��ڲ���ģʽ
bool bt_is_sleep(void);                         //�ж������Ƿ��������״̬
bool bt_is_connected(void);                     //�ж������Ƿ������ӣ�TWS���������ӣ����������ֻ������ӣ�
bool bt_is_ios_device(void);                    //�жϵ�ǰ���ŵ��Ƿ�ios�豸
uint32_t bt_sleep_proc(void);
void bt_enter_sleep(void);
void bt_exit_sleep(void);

//tws api
bool bt_tws_is_connected(void);                 //tws�豸�Ƿ�������
bool bt_tws_is_scaning(void);                   //tws�Ƿ��Ѵ򿪿ɱ�����������
void bt_set_tws_scan(bool enable);              //��/�ر�tws�ɱ�����������
void bt_tws_disconnect(void);                   //�Ͽ�tws�豸
void bt_tws_connect(void);                      //����tws�豸
void bt_tws_search_slave(void);                 //����tws�豸������������
bool bt_tws_get_link_info(uint8_t *bd_addr);    //��ȡtws�����Ϣ������Ϊ�Ƿ���Թ�TWS���ж�
void bt_tws_delete_link_info(void);             //ɾ��tws�����Ϣ
bool bt_tws_user_key(uint keycode);             //����tws�Զ��尴��, 10bit��Ч
bool bt_nor_is_connected(void);                 //�Ƿ��������ֻ�
void bt_nor_connect(void);                      //�Ͽ��ֻ�
void bt_nor_disconnect(void);                   //�����ֻ�

//��������
#define bt_scan_enable()                        bt_send_msg(BT_MSG_SCAN_ENABLE)     //��ɨ��
#define bt_scan_disable()                       bt_send_msg(BT_MSG_SCAN_DISABLE)    //�ر�ɨ��

//��������
#define bt_music_play()                         bt_send_msg(BT_MSG_PLAY)            //����
#define bt_music_pause()                        bt_send_msg(BT_MSG_PAUSE)           //��ͣ
#define bt_music_play_pause()                   bt_send_msg(BT_MSG_PLAY_PAUSE)      //�л�����/��ͣ
#define bt_music_stop()                         bt_send_msg(BT_MSG_STOP)            //ֹͣ
#define bt_music_prev()                         bt_send_msg(BT_MSG_PREV)            //��һ��
#define bt_music_next()                         bt_send_msg(BT_MSG_NEXT)            //��һ��
#define bt_music_rewind()                       bt_send_msg(BT_MSG_REWIND)          //��ʼ����
#define bt_music_rewind_end()                   bt_send_msg(BT_MSG_REWIND_END)      //��������
#define bt_music_fast_forward()                 bt_send_msg(BT_MSG_FAST_FORWARD)    //��ʼ���
#define bt_music_fast_forward_end()             bt_send_msg(BT_MSG_FAST_FORWARD_END)//�������

//����ͨ��
#define bt_call_redial_last_number()            bt_send_msg(BT_MSG_CALL_REDIAL)     //�绰�ز������һ��ͨ����
#define bt_call_answer_incoming()               bt_send_msg(BT_MSG_CALL_ANSWER_INCOM) //�����绰
#define bt_call_terminate()                     bt_send_msg(BT_MSG_CALL_TERMINATE)  //�Ҷϵ绰
#define bt_call_swap()                          bt_send_msg(BT_MSG_CALL_SWAP)       //�л���ͨ�绰
#define bt_call_private_switch()                bt_send_msg(BT_MSG_HFP_PRIVATE_SWITCH)       //�л���ͨ�绰
#define bt_siri_switch()                        bt_send_msg(BT_MSG_SWITCH_SIRI)     //����SIRI, android��Ҫ�����������д򿪡�������������������, ios��Ҫ��siri����

uint bt_get_hfp_feature(void);
int bt_spp_tx(uint8_t *packet, uint16_t len);

//hid
bool bt_hid_key(int keycode);                 //��׼HID��, ��Enter
bool bt_hid_consumer(int keycode);            //�Զ���HID��, ��VOL+ VOL-

bool bt_pb_check(void);

/*****************************************************************************
 * BLE API�����ӿ�
 *****************************************************************************/
u8 le_get_status(void);
bool le_is_connect(void);
void le_set_adv(u8 chanel, u8 type);
void le_set_adv_interval(u16 interval);
void le_enable_adv(u8 allow);
void le_disable_adv(void);
void le_disconnect(void);
bool le_tx_notify(u8 index, u8* buf, u8 len);

#endif //_API_BTSTACK_H
