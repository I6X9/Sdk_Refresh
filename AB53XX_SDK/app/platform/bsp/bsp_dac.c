#include "include.h"
#include "bsp_dac.h"

typedef struct {
    u8 type;
    u8 res[3];
    const int (*coef)[10];
}drc_cfg_t;

const u8  *dac_avol_table;
const u16 *dac_dvol_table;

AT(.text.bsp.dac.table)
const u8 dac_vol_tbl_16[16 + 1] = {
    //取值范围：(54-54) ~ (54+5),共60个值;
    //说明：值(54-54)对应音量-54dB; 54对应音量0dB;
    54-54, 54-43, 54-32, 54-26, 54-22, 54-18, 54-15, 54-12,
    54-10, 54-8,  54-6,  54-4,  54-3,  54-2,  54-1,  54,  54+1
};

AT(.text.bsp.dac.table)
const u8 dac_vol_tbl_32[32 + 1] = {
    //取值范围：(54-54) ~ (54+5),共60个值;
    //说明：值(54-54)对应音量-54dB; 54对应音量0dB;
    54-54, 54-43, 54-38, 54-35, 54-32, 54-30, 54-28, 54-26,
    54-24, 54-22, 54-21, 54-20, 54-19, 54-18, 54-17, 54-16,
    54-15, 54-14, 54-13, 54-12, 54-11, 54-10, 54-9,  54-8,
    54-7,  54-6,  54-5,  54-4,  54-3,  54-2,  54-1,  54,
    54+1,
};

AT(.text.bsp.dac.table)
const u8 dac_vol_tbl_50[50 + 1] = {
    //取值范围：(54-54) ~ (54+5),共60个值;
    //说明：值(54-54)对应音量-54dB; 54对应音量0dB;
    54-54, 54-43, 54-39, 54-38, 54-37, 54-36, 54-35, 54-34,
    54-33, 54-32, 54-31, 54-30, 54-29, 54-28, 54-27, 54-26,
    54-25, 54-24, 54-23, 54-22, 54-21, 54-20, 54-19, 54-18,
    54-17, 54-16, 54-15, 54-14, 54-13, 54-12, 54-11, 54-10,
    54-9,  54-8,  54-7,  54-6,  54-6,  54-5,  54-5,  54-4,
    54-4,  54-3,  54-3,  54-2,  54-2,  54-1,  54-1,  54,
    54,    54+1,  54+1,
};

#if DAC_DRC_EN
//限幅最大幅度:-4dB，压缩最小幅度:-34dB
//压缩起控时间:0.17ms，压缩释放时间:100ms
//限幅起控时间:0.17ms，限幅释放时间:25ms
//通过修改表来修改以上参数，后续开放工具修改
const int drc_coef_tbl[10] = {
    0x0411ab96,
    0x00020af8,
    0x0411ab96,
    0x00082a4f,
    0x01e04499,
    0xf55ead01,
    0xa5a4be81,
    0x05a0b378,
    0x10000000,
    0x01e1e1e2,
};

drc_cfg_t drc_cfg = {
    .type = 0,
    .res[0] = 0,
    .res[1] = 0,
    .res[2] = 0,
    .coef = &drc_coef_tbl,
};
#endif

#if SYS_ADJ_DIGVOL_EN
AT(.text.bsp.dac.table)
const u16 dac_dvol_tbl_16[16 + 1] = {
    DIG_N60DB,  DIG_N43DB,  DIG_N32DB,  DIG_N26DB,  DIG_N24DB,  DIG_N22DB,  DIG_N20DB,  DIG_N18DB, DIG_N16DB,
    DIG_N14DB,  DIG_N12DB,  DIG_N10DB,  DIG_N8DB,   DIG_N6DB,   DIG_N4DB,   DIG_N2DB,   DIG_N0DB,
};

AT(.text.bsp.dac.table)
const u16 dac_dvol_tbl_32[32 + 1] = {
    DIG_N60DB,  DIG_N50DB,  DIG_N43DB,  DIG_N38DB,  DIG_N35DB,  DIG_N30DB,  DIG_N28DB,  DIG_N26DB,
    DIG_N24DB,  DIG_N22DB,  DIG_N21DB,  DIG_N20DB,  DIG_N19DB,  DIG_N18DB,  DIG_N17DB,  DIG_N16DB,
    DIG_N16DB,  DIG_N15DB,  DIG_N14DB,  DIG_N13DB,  DIG_N12DB,  DIG_N11DB,  DIG_N10DB,  DIG_N9DB,
    DIG_N8DB,   DIG_N7DB,   DIG_N6DB,   DIG_N5DB,   DIG_N4DB,   DIG_N3DB,   DIG_N2DB,   DIG_N1DB,   DIG_N0DB,
};

AT(.text.bsp.dac.table)
const u16 dac_dvol_tbl_50[50 + 1] = {
    DIG_N60DB,  DIG_N56DB,  DIG_N52DB,  DIG_N48DB,  DIG_N44DB,  DIG_N42DB,  DIG_N40DB,  DIG_N38DB,
    DIG_N37DB,  DIG_N36DB,  DIG_N35DB,  DIG_N34DB,  DIG_N33DB,  DIG_N32DB,  DIG_N31DB,  DIG_N30DB,
    DIG_N29DB,  DIG_N28DB,  DIG_N28DB,  DIG_N27DB,  DIG_N26DB,  DIG_N25DB,  DIG_N25DB,  DIG_N24DB,
    DIG_N23DB,  DIG_N22DB,  DIG_N21DB,  DIG_N20DB,  DIG_N20DB,  DIG_N19DB,  DIG_N18DB,  DIG_N18DB,
    DIG_N17DB,  DIG_N16DB,  DIG_N16DB,  DIG_N15DB,  DIG_N14DB,  DIG_N13DB,  DIG_N12DB,  DIG_N11DB,
    DIG_N10DB,  DIG_N9DB,   DIG_N8DB,   DIG_N7DB,   DIG_N6DB,   DIG_N5DB,   DIG_N4DB,   DIG_N3DB,
    DIG_N2DB,   DIG_N1DB,   DIG_N0DB,
};
#endif // SYS_ADJ_DIGVOL_EN


AT(.text.bsp.dac)
void bsp_change_volume(u8 vol)
{
#if FMTX_EN
    bsp_fmtx_set_vol(vol);
#endif

#if !SYS_ADJ_DIGVOL_EN
    ///sys adjust dac analog volume
    u8 anl_vol = 0;
    if (vol <= VOL_MAX) {
        if (vol > 0) {
            anl_vol = dac_avol_table[vol] + sys_cb.anl_gain_offset;
        }
        dac_set_volume(anl_vol);
    }
#else
    ///sys adjust dac digital volume
    u16 dig_vol = 0;
    if (vol <= VOL_MAX) {
        dig_vol = dac_dvol_table[vol];
    #if AUCAR_EQ_LOUD_BASS_TREBLE_EN
        digital_vol_set_for_aucar(dig_vol);
    #else
        dac_set_dvol(dig_vol);
    #endif
        //digital_vol_set_before_eq(dig_vol,music_coef);  //只调高低音, 不支持EQ. DAC_AUCAR_EN/SYS_BASS_TREBLE_EN/SYS_ADJ_DIGVOL_EN
        //printf("dig_vol = %d\n",dig_vol);
    }
#endif
}

AT(.text.bsp.dac)
bool bsp_set_volume(u8 vol)
{
    bsp_change_volume(vol);
    if (vol == sys_cb.vol) {
        gui_box_show_vol();
        return false;
    }

    if (vol <= VOL_MAX) {
        sys_cb.vol = vol;
#if SYS_KARAOK_EN
        sys_cb.show_type = 0;
#endif
        gui_box_show_vol();
        param_sys_vol_write();
        sys_cb.cm_times = 0;
        sys_cb.cm_vol_change = 1;
    }
    return true;
}

//开机控制DAC电容放电等待时间
AT(.text.dac)
void dac_pull_down_delay(void)
{
    delay_5ms(DAC_PULL_DOWN_DELAY);
}

AT(.com_text.dac)
void dac_set_mute_callback(u8 mute_flag)
{
    if (mute_flag) {
        bsp_loudspeaker_mute();
    } else {
        if (!sys_cb.mute) {
            bsp_loudspeaker_unmute();
#if EARPHONE_DETECT_EN
            if (!dev_is_online(DEV_EARPHONE))
#endif
            {
                //DAC延时淡入，防止UNMUTE时间太短导致喇叭声音不全的问题
                dac_unmute_set_delay(LOUDSPEAKER_UNMUTE_DELAY);
                printf("dac_unmute_set_delay = %d;\n", LOUDSPEAKER_UNMUTE_DELAY);
            }
        }
    }
}

AT(.text.bsp.dac)
u8 bsp_volume_inc(u8 vol)
{
    vol++;
    if(vol > VOL_MAX)
        vol = VOL_MAX;
    return vol;
}

AT(.text.bsp.dac)
u8 bsp_volume_dec(u8 vol)
{
    if(vol > 0)
        vol--;
    return vol;
}

AT(.text.bsp.dac)
void dac_set_anl_offset(u8 bt_call_flag)
{
    if (bt_call_flag) {
        sys_cb.anl_gain_offset = 54 - 9 + BT_CALL_MAX_GAIN - dac_avol_table[VOL_MAX];
    } else {
        sys_cb.anl_gain_offset = 54 - 9 + DAC_MAX_GAIN - dac_avol_table[VOL_MAX];
    }
}

AT(.text.bsp.dac)
void dac_set_vol_table(u8 vol_max)
{
    if (vol_max == 16) {
        dac_avol_table = dac_vol_tbl_16;
#if SYS_ADJ_DIGVOL_EN
        dac_dvol_table = dac_dvol_tbl_16;
#endif // SYS_ADJ_DIGVOL_EN
    } else if (vol_max <= 32) {
        dac_avol_table = dac_vol_tbl_32;
#if SYS_ADJ_DIGVOL_EN
        dac_dvol_table = dac_dvol_tbl_32;
#endif // SYS_ADJ_DIGVOL_EN
    } else {
        dac_avol_table = dac_vol_tbl_50;
#if SYS_ADJ_DIGVOL_EN
        dac_dvol_table = dac_dvol_tbl_50;
#endif // SYS_ADJ_DIGVOL_EN
    }
    dac_set_anl_offset(0);
}

AT(.text.bsp.dac)
void dac_init(void)
{
    dac_set_vol_table(xcfg_cb.vol_max);
    printf("[%s] vol_max:%d, offset: %d\n", __func__, xcfg_cb.vol_max, sys_cb.anl_gain_offset);

    adpll_init(DAC_OUT_SPR);
    dac_obuf_init();
    dac_power_on();

#if (DAC_OUT_SPR == DAC_OUT_48K)
    DACDIGCON0 |= BIT(1);           //dac out sample 48K
#endif
    dac_digital_enable();

    plugin_music_eq();

#if SYS_ADJ_DIGVOL_EN
    dac_set_volume(54);             //analog volume设置0DB
#endif // SYS_ADJ_DIGVOL_EN

#if DAC_DNR_EN
    dac_dnr_init(2, 0x10, 40, 0x10); //dnr, 20ms检测一次. 自动MUTE时间计算: 40*20 = 800ms  + 16(16级音量)*5 (模拟音量淡出) + 60ms(数字音量淡出) = 940ms
#endif

#if DAC_AUCAR_EN
    bsp_aucar_init();
#endif // DAC_AUCAR_EN

#if DAC_DRC_EN
    dac_drc_init(&drc_cfg);
#endif

#if DAC_VBS_EN
    dac_vbs_init(NULL);
#endif
}


