#ifndef _API_FMRX_H
#define _API_FMRX_H

#define FM_FREQ_MIN                         8750
#define FM_FREQ_MAX                         10800

void fmrx_pulldown(void);
void fmrx_dac_sync(void);
void fmrx_power_on(u32 val);
void fmrx_power_off(void);
void fmrx_analog_init(void);
void fmrx_digital_stop(void);
void fmrx_digital_start(void);
void fmrx_set_freq(u16 freq);
u8 fmrx_check_freq(u16 freq);
void fmrx_logger_out(void);
void fmrx_set_rf_cap(u8 val);
void fmrx_set_audio_channel(u8 mono_flag);  //设置立体声或单声道解调输出  1:mono, 0:stereo
bool fmrx_cur_freq_is_stereo(void);         //检测当前收听台是不是立体声, 用于车机的ST标志显示(判断需要100ms)

void fmrx_rec_enable(void);
void fmrx_rec_start(void);
void fmrx_rec_stop(void);

bool fmtx_init(void *cfg);
void fmtx_set_freq(u16 freq);
void fmtx_set_vol(u16 vol);
void fmtx_exit(void);

void fmrx_rec_set_sdbaud(u16 freq);
void fmrx_manual_gain_init(u16 gain);  //8192(0DB)
s8 fmrx_rssi_dbu_get(void);  //10dbu~49dbu  基本一致 //小于 10dbu 或大于49dbu误差会变大
void fmrx_pga_gain_set(u8 gain);   //gain 取值范围 0~13. 默认为8
#endif // _API_FMRX_H
