#include "include.h"
#include "func.h"
#include "func_music.h"

#if FUNC_MUSIC_EN
AT(.text.func.music.msg)
void func_music_message(u16 msg)
{
    switch (msg) {
        case KU_PLAY:
        case KU_PLAY_POWER:
        case KU_PLAY_MODE:
#if IR_NUMKEY_EN
            if (box_cb.disp_sta == GUI_BOX_INPUTNUM) {
                box_cb.times = 0;           //确认数字选曲
                return;
            }
#endif // IR_NUMKEY_EN

            if (f_msc.pause) {
                f_msc.pause = 0;
                music_control(MUSIC_MSG_PLAY);
                led_music_play();
                printf("music continue playing\n");
            } else {
                f_msc.pause = 1;
                bsp_clr_mute_sta();
                music_control(MUSIC_MSG_PAUSE);
                led_idle();
                printf("music pause\n");
            }
            break;

        case KU_PREV_VOL_DOWN:
        case KL_VOL_DOWN_PREV:
        case KU_PREV:
            bsp_clr_mute_sta();
            func_music_switch_file(0);
            break;

        case KU_NEXT_VOL_UP:
        case KL_VOL_UP_NEXT:
        case KU_NEXT:
            bsp_clr_mute_sta();
            func_music_switch_file(1);
            break;

#if MUSIC_QSKIP_EN
        case KL_NEXT:
        case KH_NEXT:
            bsp_clr_mute_sta();
            music_qskip(QSKIP_BACKWARD, 3);             //快进
            break;

        case KL_PREV:
        case KH_PREV:
            bsp_clr_mute_sta();
            music_qskip(QSKIP_FORWARD, 3);               //快退
            break;

        case KLU_NEXT:
        case KLU_PREV:
            music_qskip_end();
            break;
#endif // MUSIC_QSKIP_EN

#if MUSIC_FOLDER_SELECT_EN
        case KD_VOL_DOWN_PREV:
            func_music_switch_dir(0);
            break;

        case KD_VOL_UP_NEXT:
            func_music_switch_dir(1);
            break;
#endif // MUSIC_FOLDER_SELECT_EN

        case KL_PLAY:
        	karaok_voice_rm_switch();
            break;

        case KU_REPEAT:
            music_playmode_next();
            break;

        case KU_MODE:
        case KU_MODE_POWER:
        case KL_PLAY_MODE:
            bsp_clr_mute_sta();
            if ((f_msc.cur_dev == DEV_UDISK) && (dev_is_online(DEV_SDCARD))) {
                f_msc.dev_change = 1;
                music_breakpoint_save();
            } else {
                if (dev_is_online(DEV_UDISK) && (dev_is_online(DEV_SDCARD))) {
                    sys_cb.cur_dev = DEV_UDISK;
                }
                func_cb.sta = FUNC_NULL;
            }
        break;

        case KU_HSF:
        case KU_USB_SD:
            //双设备切换
            if (dev_is_online(DEV_UDISK) && (dev_is_online(DEV_SDCARD))) {
                bsp_clr_mute_sta();
                f_msc.dev_change = 1;
                music_breakpoint_save();
            }
            break;

#if IR_NUMKEY_EN
        case KEY_NUM_0:
        case KEY_NUM_1:
        case KEY_NUM_2:
        case KEY_NUM_3:
        case KEY_NUM_4:
        case KEY_NUM_5:
        case KEY_NUM_6:
        case KEY_NUM_7:
        case KEY_NUM_8:
        case KEY_NUM_9:
            gui_box_show_inputnum(msg - KEY_NUM_0);
            break;
#endif // IR_NUMKEY_EN

        case EVT_UDISK_REMOVE:
        case EVT_SD_REMOVE:
            bsp_clr_mute_sta();
            if (dev_is_online(DEV_SDCARD) || dev_is_online(DEV_UDISK)) {
                f_msc.dev_change = 1;
                music_breakpoint_save();
            } else {
#if MUSIC_MODE_RETURN
                func_cb.sta = func_cb.last;
#else
                func_cb.sta = FUNC_NULL;
#endif
            }
            break;

        case EVT_UDISK_INSERT:
            //防止插入苹果手机打断SD卡播放
            if (!dev_udisk_activation_try(0)) {
                break;
            }
        case EVT_SD_INSERT:
            bsp_clr_mute_sta();
            f_msc.dev_change = 1;
            music_breakpoint_save();
            break;

#if SYS_MAGIC_VOICE_EN
        case KU_REC:
            magic_voice_switch();
            break;
#endif

//#if KARAOK_REC_EN
//        case KU_REC:
//            sfunc_record();
//    #if REC_AUTO_PLAY
//            if (rec_cb.flag_play) {
//                rec_cb.flag_play = 0;
//                sfunc_record_play();
//            }
//    #endif
//            break;
//#endif


#if REC_FAST_PLAY
        case KD_REC:    //播放最新的录音.
            if ((sys_cb.rec_num) && (sys_cb.rec_num != f_msc.file_num)) {
                f_msc.file_change = 1;
                f_msc.file_num = sys_cb.rec_num;
            }
            break;
#endif

        case KL_REC:
#if FUNC_REC_EN
            if ((!strncmp(f_msc.fname, "mic", 3)) || (!strncmp(f_msc.fname, "aux", 3)) || (!strncmp(f_msc.fname, "fm", 2)) || (!strncmp(f_msc.fname, "bt", 2))) {
                music_control(MUSIC_MSG_STOP);
                if (fs_delete(f_msc.file_num)) {
                    func_music_filter_set();
                    pf_scan_music(0);
                    if (f_msc.file_num > f_msc.file_total) {
                        f_msc.file_num = 1;
                    }
                    f_msc.file_change = 1;
                }
            }
#endif // FUNC_REC_EN
            break;

#if MUSIC_REC_FILE_FILTER
        case KL_MODE:
            if (f_msc.rec_type) {
                if (func_music_filter_switch(0)) {
                    f_msc.rec_type = 0;
                }
            } else {
                if (func_music_filter_switch(1)) {
                    f_msc.rec_type = 1;
                }
            }
            break;
#endif // MUSIC_REC_FILE_FILTER

        case MSG_SYS_1S:
#if MUSIC_LRC_EN
            bsp_lrc_sta_process();
#endif // MUSIC_LRC_EN
            break;

        default:
            func_message(msg);
            break;
    }
}
#endif // FUNC_MUSIC_EN

