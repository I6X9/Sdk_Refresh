config(SUB, "系统配置", "系统的相关配置");
config(LIST, "提示音语言选择", "选择系统的语言", LANG_ID, 4, "英文", "中文", "中英文(出厂默认英文)", "中英文(出厂默认中文)", 2);
#config(CHECK, "SD/U盘播放功能", "是否需要SD/UDISK播放功能", FUNC_MUSIC_EN, 0);
#config(CHECK, "AUX输入功能", "是否需要AUX功能", FUNC_AUX_EN, 0);
config(CHECK, "BUCK MODE", "是否设置为BUCK MODE", BUCK_MODE_EN, 1);
config(CHECK, "第一次上电直接开机", "接入电池第一次上电是否直接开机", PWRON_FRIST_BAT_EN, 1);
config(CHECK, "软开关机功能", "软件开关机,包括内部和外部软开关机", SOFT_POWER_ON_OFF_EN, 1);
config(CHECK, "POWKEY 10s复位系统", "是否长按POWKEY 10s复位系统，用于防止系统死机", POWKEY_10S_RESET, 1);
config(CHECK, "功放MUTE功能", "是否使能功放MUTE功能", SPK_MUTE_EN, 0);
config(LIST, "功放MUTE控制IO选择", "功放MUTE控制GPIO选择", SPK_MUTE_IO_SEL, 27, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", "PF3", "PF4", 14, SPK_MUTE_EN);
config(CHECK, "高MUTE", "是否为高MUTE，否则为低MUTE", HIGH_MUTE, 1, SPK_MUTE_EN);
config(BYTE, "功放MUTE延时(单位5ms)", "功放MUTE的延时控制，防止解MUTE时间不够导致声音不全。", LOUDSPEAKER_UNMUTE_DELAY, 0, 255, 6, SPK_MUTE_EN);
config(LISTVAL, "自动休眠时间", "设置自动休眠时间", SYS_SLEEP_TIME, SHORT, 45,  21, ("不休眠", 0), ("10秒钟后", 10), ("20秒钟后", 20), ("30秒钟后", 30), ("45秒钟后", 45), ("1分钟后", 60), ("2分钟后", 120), ("3分钟后", 180), ("4分钟后", 240), ("5分钟后", 300), ("6分钟后", 360), ("7分钟后", 420), ("8分钟后", 480), ("9分钟后", 540), ("10分钟后", 600), ("15分钟后", 900), ("20分钟后", 1200), ("25分钟后", 1500), ("30分钟后", 1800), ("45分钟后", 2700), ("1小时后", 3600));
config(LISTVAL, "自动关机时间", "设置自动关机时间", SYS_OFF_TIME, 	SHORT, 300, 18, ("不关机", 0), ("30秒钟后", 30), ("1分钟后", 60),  ("2分钟后", 120), ("3分钟后", 180), ("4分钟后", 240), ("5分钟后", 300), ("6分钟后", 360), ("7分钟后", 420), ("8分钟后", 480), ("9分钟后", 540), ("10分钟后", 600), ("15分钟后", 900), ("20分钟后", 1200), ("25分钟后", 1500), ("30分钟后", 1800), ("45分钟后", 2700), ("1小时后", 3600));
config(CHECK, "低电提示", "当电压比较低的时候，会有提示音警告", LOWPOWER_WARNING_EN, 1);
config(LIST, "低电语音提示电压", "选择低电语音提示电压", LPWR_WARNING_VBAT, 10, "2.8V", "2.9V", "3.0V", "3.1V", "3.2V", "3.3V", "3.4V", "3.5V", "3.6V", "3.7V", 5, LOWPOWER_WARNING_EN);
config(LIST, "低电关机电压", "选择低电关机电压", LPWR_OFF_VBAT, 11, "不关机", "2.8V", "2.9V", "3.0V", "3.1V", "3.2V", "3.3V", "3.4V", "3.5V", "3.6V", "3.7V", 3, LOWPOWER_WARNING_EN);
config(BYTE, "低电语音播报周期(秒)", "设置低电语音播报周期(秒)", LPWR_WARNING_PERIOD, 1, 240, 30, LOWPOWER_WARNING_EN);
config(LIST, "音量级数", "选择系统音量级数", VOL_MAX, 3, "16级音量", "30级音量", "32级音量", 2);
config(CHECK, "开机音量固定为默认音量", "每次开机音量均设置为开机默认音量(相当于系统音量不记忆)", SYS_VOL_FIXED_2_INIT_VOL, 0);
config(BYTE, "开机默认音量", "开机默认音量级数", SYS_INIT_VOL, 0, 32, 10);
config(BYTE, "恢复默认音量的阈值", "开机读取的音量级数小于此阈值，恢复为开机默认音量", SYS_LIMIT_VOLUME, 0, 32, 10);
config(BYTE, "提示音播放音量", "提示音播放音量级数", WARNING_VOLUME, 0, 32, 14);
config(LIST, "OSC基础电容", "同时配置26M晶振OSCI与OSCO负载电容", OSC_BOTH_CAP, 2, "0PF", "6PF", 0);
config(BYTE, "OSCI电容(0.25PF)", "独立配置26M晶振OSCI负载电容，单位0.25PF。OSCI电容大小：n * 0.25PF + 基础电容", OSCI_CAP, 0, 63, 23);
config(BYTE, "OSCO电容(0.25PF)", "独立配置26M晶振OSCO负载电容，单位0.25PF。OSCO电容大小：n * 0.25PF + 基础电容", OSCO_CAP, 0, 63, 23);
config(CHECK, "EQ调试（蓝牙串口）", "是否使用SPP调试EQ功能", EQ_DGB_SPP_EN, 0);
config(CHECK, "EQ调试（UART）", "是否使用UART调试EQ功能", EQ_DGB_UART_EN, 0);
config(LIST, "EQ调试串口选择", "选择调试串口的IO", EQ_UART_SEL, 3, "PA7", "PB2", "PB3", 0, EQ_DGB_UART_EN);
config(CHECK, "插入LINEIN关机", "是否使能插入LINEIN关机功能，大耳包功能", LINEIN_2_PWRDOWN_EN, 0);
config(LIST, "插入LINEIN关机提示音", "插入LINEIN关机播放关机提示音选择", LINEIN_2_PWRDOWN_TONE_EN, 3, "None", "关机提示音", "LINEIN提示音", 0, LINEIN_2_PWRDOWN_EN);
config(LIST, "选择LINEIN检测IO", "选择LINEIN检测GPIO", LINEIN_DET_IO, 25, "复用MICL检测", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", 0, LINEIN_2_PWRDOWN_EN);

config(SUB, "DAC配置", "DAC的相关配置");
config(LIST, "DAC声道选择", "选择DAC的输出方式", DAC_SEL, 8, "单声道", "双声道", "差分单声道", "差分双声道", "VCMBUF单声道", "VCMBUF双声道", "MIX_VCM单声道", "MIX_VCM差分", 2);
config(LIST, "VDDDAC电压", "选择VDDDAC电压", DAC_LDOH_SEL, 6, "2.4V", "2.5V", "2.7V", "2.9V", "3.1V", "3.2V", 3);
config(BYTE, "DAC隔直电容放电时间", "控制DAC隔直电容放电等待时间, 无电容时可设为0，减少开机时间", DAC_PULL_DOWN_DELAY, 0, 255, 0);
config(CHECK, "省VCM方案", "是否使用内部VCM，SOP16, SSOP24, SSOP28使用内部VCM", DAC_VCM_LESS_EN, 1);
config(LIST, "DAC最大音量", "配置系统最大模拟增益，自动调整模拟音量表。", DAC_MAX_GAIN, 15, "-9DB", "-8DB", "-7DB", "-6DB", "-5DB", "-4DB", "-3DB", "-2DB", "-1DB", "0DB", "+1DB", "+2DB", "+3DB", "+4DB", "+5DB", 9);
config(LIST, "通话最大音量", "配置通话时最大模拟增益，自动调整模拟音量表。", BT_CALL_MAX_GAIN, 15, "-9DB", "-8DB", "-7DB", "-6DB", "-5DB", "-4DB", "-3DB", "-2DB", "-1DB", "0DB", "+1DB", "+2DB", "+3DB", "+4DB", "+5DB", 8);
config(NUM, "系统最大数字音量", "设置系统最大数字音量，固定数字音量调节模拟音量。", DAC_MAX_DIGVOL, 0, 32767, 32256);
config(CHECK, "最大音量提示音MP3格式", "最大音量提示音为MP3格式,关闭则为WAV格式", MAXVOL_TONE_IS_MP3, 0);
config(CHECK, "蓝牙连接时关闭DAC", "蓝牙连接时关闭DAC，仅对差分输出有效", DAC_OFF_FOR_CONN, 0);

config(SUB, "充电配置", "充电功能的相关配置");
config(CHECK, "充电使能", "是否打开充电功能", CHARGE_EN, 1);
config(CHECK, "支持外接充电IC", "支持外接充电IC时,外接充电IC和芯片充电引脚并连在一起,此时会关掉内部充电. 插入DC复位系统 或 插入DC禁止软开机 则可以根据需要配置", SUPPORT_EXT_CHARGE_IC, 0, CHARGE_EN);
config(CHECK, "涓流充电使能", "是否使能涓流充电", CHARGE_TRICK_EN, 1, CHARGE_EN);
config(CHECK, "插入DC复位系统", "是否插入DC充电复位系统，用于防止系统死机", CHARGE_DC_RESET, 1, CHARGE_EN);
config(CHECK, "插入DC禁止软开机", "PWRKEY软开机，DC Online时，禁止软开机", CHARGE_DC_NOT_PWRON, 1, CHARGE_EN);
config(LISTVAL, "充电仓充电操作选项", "耳机充电完成自动关机(充电完成,耳机进入低功耗160uA,耳机检测充电仓断电则自动关机) 或 耳机从充电仓拿起开机(充电仓不断电)", CHARGE_WAREHOUSE_SET, BIT, 3, 0, 3, ("不进行任何操作", 0), ("耳机充电完成自动关机", 1), ("耳机从充电仓拿起自动开机", 2),CHARGE_EN);
config(LIST, "充电截止电流", "选择充电截止电流", CHARGE_STOP_CURR, 8, "2.5mA", "5mA", "10mA", "15mA", "20mA", "25mA", "30mA", "35mA", 6, CHARGE_EN);
config(LIST, "恒流充电电流", "恒流充电（电池电压大于2.9v）电流", CHARGE_CONSTANT_CURR, 14, "30mA", "40mA", "50mA", "60mA", "70mA", "80mA", "90mA", "100mA", "110mA", "120mA", "140mA", "160mA", "180mA", "200mA", 10, CHARGE_EN);
config(LIST, "涓流充电电流", "涓流充电（电池电压小于2.9v）电流", CHARGE_TRICKLE_CURR, 3, "30mA", "40mA", "50mA", 0, CHARGE_TRICK_EN);

config(SUB, "蓝牙配置", "蓝牙的相关配置");
config(TEXT, "蓝牙名称", "手机上可以看到的蓝牙名称", BT_NAME, 32, "Bluetooth Music");
config(MAC, "蓝牙地址", "蓝牙的MAC地址", BT_ADDR, 6, 41:42:98:00:00:01, 41:42:98:FF:FF:FF, 41:42:00:00:00:01);
config(LIST, "#1 预置RF发射功率", "配置预置RF发射功率", BT_TXPWR, 5, "+4DBM", "+3DBM", "+2DBM", "+1DBM", "0DBM", 2);
config(LISTVAL, "#1 预置RF参数选择", "根据封装选择，影响发射、接收质量", BT_RF_PARAM, BIT, 8, 255, 9, ("LQFP48", 0), ("QFN32", 1), ("SOP28", 2), ("TSSOP24", 3), ("TSSOP20", 4), ("SOP16", 5), ("QFN32小耳机板", 129), ("SOP16小耳机板", 133), ("AUTO", 255));
config(LIST, "#2 main.c定义RF参数", "是否使用main.c定义的RF参数，打开后<预置RF参数选择>无效", BT_RF_USER_PARAM, 17, "不使用", "参数0", "参数1", "参数2", "参数3", "参数4", "参数5", "参数6", "参数7", "参数8", "参数9", "参数10", "参数11", "参数12", "参数13", "参数14", "参数15", 0);
config(CHECK, "#3 是否自定义RF参数", "选用自定义RF参数后，打开后<预置RF参数无效>和<main.c定义RF参数>无效", BT_USER_PARAM_EN, 0);
config(LEVEL, 0x00);
config(BYTE, "#3 TAG0",     "TAG0",     RF_TAG0,     1, 1,  1,  BT_USER_PARAM_EN);
config(BYTE, "#3 TAG1",     "TAG1",     RF_TAG1,     0, 0,  0,  BT_USER_PARAM_EN);
config(LEVEL, 0x03);
config(BYTE, "#3 CAPTUNE",  "CAPTUNE, 参考值5~7",    RF_CAPTUNE,  0, 7,  7,  BT_USER_PARAM_EN);
config(BYTE, "#3 PA_GAIN",  "PA_GAIN, 参考值7",      RF_PA_GAIN,  0, 7,  7,  BT_USER_PARAM_EN);
config(BYTE, "#3 MIX_GAIN", "MIX_GAIN, 参考值5~6",   RF_MIX_GAIN, 0, 7,  5,  BT_USER_PARAM_EN);
config(BYTE, "#3 DIG_GAIN", "DIG_GAIN, 参考值30~59", RF_DIG_GAIN, 0, 61, 55, BT_USER_PARAM_EN);
config(BYTE, "#3 PA_BIAS",  "PA_BIAS, 一般不改",     RF_PA_BIAS,  0, 7,  4,  BT_USER_PARAM_EN);
config(BYTE, "#3 VBKO_ADD", "VBKO_ADD, 参考值1~4",   RF_VBKO_ADD, 0, 4,  0,  BT_USER_PARAM_EN);
config(LEVEL, 0x00);
config(BYTE, "#3 DBM",      "DBM值(仅显示用)",       RF_TXDBM,    0, 8,  4,  BT_USER_PARAM_EN);
config(BYTE, "#3 UDF",      "UDF(保留使用)",         RF_UDF,      0, 0,  0,  BT_USER_PARAM_EN);
config(LEVEL, 0x03);
config(CHECK, "#3 通话RF参数", "是否单独配置通话RF参数", RF_PARAM_SCO_EN, 0, BT_USER_PARAM_EN);
config(BYTE, "#3 通话MIX_GAIN0", "通话MIX_GAIN0", RF_MIX_GAIN_SCO, 0, 7, 5, RF_PARAM_SCO_EN);
config(BYTE, "#3 通话DIG_GAIN0", "通话DIG_GAIN0", RF_DIG_GAIN_SCO, 20, 61, 55, RF_PARAM_SCO_EN);
config(CHECK, "降低回连TXPWR", "是否降低回连TXPWR", BT_RF_LTX_RECON, 0);
config(LIST, "长按几秒开机进配对", "是否支持长按N秒开机直接进入配对状态", BT_PWRKEY_NSEC_DISCOVER, 9, "不支持", "1秒", "2秒", "3秒", "4秒", "5秒", "6秒", "7秒", "8秒", 0);
config(BYTE,  "上电回连次数(每次5秒)", "上电回连次数(每次5秒)", BT_POWER_UP_RECONNECT_TIMES, 0, 255, 3);
config(BYTE,  "掉线回连次数(每次5秒)", "掉线回连次数(每次5秒)", BT_TIME_OUT_RECONNECT_TIMES, 0, 255, 20);
config(CHECK, "配对提示音", "是否播放配对提示音", WARNING_BT_PAIR, 0);
config(CHECK, "未配对是否进入休眠模式", "未配对是否休眠，否则直接关机", BT_PAIR_SLEEP_EN, 0);
config(CHECK, "连接两部手机功能", "是否支持连接两部手机功能", BT_2ACL_EN, 1);
config(CHECK, "音乐播放功能", "是否支持蓝牙音乐播放功能", BT_A2DP_EN, 1);
config(CHECK, "音乐音量同步", "是否支持A2DP音量与手机同步", BT_A2DP_VOL_CTRL_EN, 1, BT_A2DP_EN);
config(CHECK, "通话功能", "是否支持蓝牙通话的功能", BT_SCO_EN, 1);
config(CHECK, "私密接听功能", "是否使用手动私密接听（切换到手机端接听）", BT_HFP_PRIVATE_EN, 0, BT_SCO_EN);
config(CHECK, "来电报号功能", "是否支持来电报号功能", BT_HFP_RING_NUMBER_EN, 1, BT_SCO_EN);
config(CHECK, "串口功能", "是否支持蓝牙串口的功能", BT_SPP_EN, 0);
config(CHECK, "拍照功能", "是否支持蓝牙HID拍照的功能", BT_HID_EN, 1);
config(CHECK, "按键HID连接/断开功能", "是否支持蓝牙HID服务，按键手动连接/断开", BT_HID_MENU_EN, 1, BT_HID_EN);
config(CHECK, "HID默认不连接", "HID服务默认不连接，需要按键手动连接", BT_HID_DISCON_DEFAULT_EN, 0, BT_HID_EN);
config(CHECK, "HID独立自拍器模式", "是否支持独立自拍器模式功能", FUNC_BTHID_EN, 0);
config(TEXT, "HID蓝牙名称", "手机上可以看到的独立自拍器模式蓝牙名称", BTHID_NAME, 32, "BT-Photo", FUNC_BTHID_EN);
config(LISTVAL, "复用Siri键设置", "选择呼出Siri的复用按键", BT_SIRI_CTRL_EN, BIT, 3, 0, 3, ("不支持", 0), ("长按P/P键2S", 1), ("双击P/P键", 2), BT_SCO_EN);
config(LISTVAL, "独立Siri键设置", "选择呼出Siri的独立按键设置，在<按键配置>中分配SIRI键", BT_SIRI_KEY_SEL, BIT, 2, 0, 3, ("单击SIRI键", 0), ("双击SIRI键", 1), ("长按SIRI键", 2), BT_SCO_EN);
config(CHECK, "TWS功能", "是否支持TWS功能，打开后<连接两部手机功能>无效", BT_TWS_EN, 0);
config(LISTVAL, "TWS配对方式选择", "选择TWS配对方式", BT_TWS_PAIR_MODE, BIT, 3, 0, 4, ("自动配对", 0), ("双击PLAY/POWER键配对", 1), ("长按MODE键配对", 2), ("自定义配对<调用api>", 3), BT_TWS_EN);
config(LISTVAL, "TWS声道分配选择", "选择TWS声道分配方式", BT_TWS_LR_MODE, BIT, 4, 1, 5, ("不分配，主副均双声道输出", 0), ("自动分配，主右声道副左声道", 1), ("PWRKEY,有820K接地为左", 2), ("GPIOx有接地为左", 3), ("自动分配，主左声道副右声道", 4), BT_TWS_EN);
config(LISTVAL, "TWS GPIOx有接地为左", "TWS声道分配选择 选则 GPIOx有接地为左(声道), GPIOx在此选则", TWS_SEL_LEFT_GPIO_SEL, BIT, 6, 0, 28, ("None", 0), ("PA0", 1), ("PA1", 2), ("PA2", 3), ("PA3", 4), ("PA4", 5), ("PA5", 6), ("PA6", 7), ("PA7", 8), ("PB0", 9), ("PB1", 10), ("PB2", 11), ("PB3", 12), ("PB4", 13), ("PE0", 14), ("PE1", 15), ("PE2", 16), ("PE3", 17), ("PE4", 18), ("PE5", 19), ("PE6", 20), ("PE7", 21), ("PF0", 22), ("PF1", 23), ("PF2", 24), ("PF3", 25), ("PF4", 26), ("PF5", 27), BT_TWS_EN);
config(CHECK, "BLE控制功能", "是否支持BLE音乐控制的功能", BLE_EN, 0);
config(TEXT, "BLE名称", "手机上可以看到的BLE蓝牙名称", LE_NAME, 32, "LE-Remoter", BLE_EN);
config(CHECK, "测试模式双击回拨", "是否支持测试模式下双击所有按键回拨", BT_TEST_REDIAL_EN, 1);

config(SUB, "通话参数", "蓝牙的通话参数");
config(LIST, "MIC通路选择", "选择MIC通路", BT_CH_MIC, 4, "MICL0(MICL2ADCL)", "MICL1(MICR2ADCL)", "MICR0(MICR2ADCR)", "MICR1(MICL2ADCR)", 0);
config(BYTE, "MIC模拟增益", "MIC模拟增益配置(-6DB ~ +63DB), Step 3DB", BT_ANL_GAIN, 0, 23, 13);
config(BYTE, "MIC数字增益(0~3DB)", "MIC SDADC数字增益, Step 3/32DB, 用于细调0~3DB", BT_DIG_GAIN, 0, 31, 0);
config(NUM, "环境噪声阈值", "环境噪声阈值(低于此阈值便认为是噪声)", BT_NOISE_THRESHOID, 0, 65535, 1536, BT_SCO_EN);
config(CHECK, "AEC功能", "是否支持AEC功能，AEC/ALC只能二选一", BT_AEC_EN, 1, BT_SCO_EN);
config(BYTE, "AEC回声消除级别", "回声消除级别(级别越高，回声衰减越明显，但通话效果越差)", BT_ECHO_LEVEL, 0, 15, 1, BT_AEC_EN);
config(BYTE, "AEC远端补偿值", "远端补偿值(0~255)", BT_FAR_OFFSET, 0, 255, 36, BT_AEC_EN);
config(CHECK, "ALC功能", "是否支持ALC功能，AEC/ALC只能二选一", BT_ALC_EN, 0, BT_SCO_EN);
config(LIST, "ALC淡入延时", "ALC近端延时多少时间才淡入", BT_ALC_IN_DELAY, 6, "16ms", "64ms", "112ms", "160ms", "224ms", "288ms", 3, BT_ALC_EN);
config(LIST, "ALC淡入速度", "ALC近端淡入速度", BT_ALC_IN_STEP, 6, "4ms", "8ms", "16ms", "32ms", "64ms", "128ms", 4, BT_ALC_EN);
config(LIST, "ALC淡出延时", "ALC近端延时多少时间才淡出", BT_ALC_OUT_DELAY, 6, "16ms", "64ms", "112ms", "160ms", "224ms", "288ms", 0, BT_ALC_EN);
config(LIST, "ALC淡出速度", "ALC近端淡出速度", BT_ALC_OUT_STEP, 6, "4ms", "8ms", "16ms", "32ms", "64ms", "128ms", 0, BT_ALC_EN);

#config(SUB, "音乐配置", "音乐播放模式的相关配置", FUNC_MUSIC_EN);
#config(CHECK, "WAV解码", "是否支持WAV格式解码", MUSIC_WAV_SUPPORT, 0, FUNC_MUSIC_EN);
#config(CHECK, "WMA解码", "是否支持WMA格式解码", MUSIC_WMA_SUPPORT, 0, FUNC_MUSIC_EN);
#config(CHECK, "APE解码", "是否支持APE格式解码", MUSIC_APE_SUPPORT, 0, FUNC_MUSIC_EN);
#config(CHECK, "FLAC解码", "是否支持FLAC格式解码", MUSIC_FLAC_SUPPORT, 0, FUNC_MUSIC_EN);
#config(CHECK, "SBC解码", "是否支持SBC格式解码", MUSIC_SBC_SUPPORT, 0, FUNC_MUSIC_EN);

#config(SUB, "AUX配置", "AUX模式的相关配置", FUNC_AUX_EN);
#config(CHECK, "AUX通路进ADC", "AUX是否进ADC,否则直通DAC。进ADC可以调AUX EQ,及AUX录音等功能。", AUX_2_SDADC_EN, 0, FUNC_AUX_EN);
#config(LIST, "AUX模拟增益", "AUX模拟增益选择", AUX_ANL_GAIN, 5, "-6DB", "-3DB", "0DB", "+3DB", "+6DB", 2, FUNC_AUX_EN);
#config(BYTE, "AUX数字增益(0~3DB)", "AUX SDADC数字增益, Step 3/32DB, 用于细调0~3DB", AUX_DIG_GAIN, 0, 31, 0, FUNC_AUX_EN);
#config(LIST, "AUXL通路选择", "AUX左声道通路选择", AUXL_SEL, 9, "无AUXL输入", "AUXL_PA6", "AUXL_PB1", "AUXL_PE6", "AUXL_PF0", "AUXL_VOUTLN", "AUXL_MICL", "AUXL_MICR", "AUXL_VOUTRP", 4, FUNC_AUX_EN);
#config(LIST, "AUXR通路选择", "AUX右声道通路选择", AUXR_SEL, 9, "无AUXR输入", "AUXR_PA7", "AUXR_PB2", "AUXR_PE7", "AUXR_PF1", "AUXR_VOUTLN", "AUXR_MICL", "AUXR_MICR", "AUXR_VOUTRN", 4, FUNC_AUX_EN);

config(SUB, "按键配置", "选择按键及按键功能配置");
config(CHECK, "PWRKEY功能", "是否使用PWRKEY，使用软开关机时必须使能", USER_PWRKEY_EN, 1);
config(CHECK, "ADKEY1功能", "是否使用第一组ADKEY", USER_ADKEY_EN, 0);
config(CHECK, "ADKEY2功能", "是否使用第二组ADKEY", USER_ADKEY2_EN, 0);
config(CHECK, "IOKEY功能", "是否使用IOKEY", USER_IOKEY_EN, 0);
config(CHECK, "按键双击功能", "是否使能按键双击功能", USER_KEY_DOUBLE_EN, 1);
config(LISTVAL, "双击响应时间选择", "选择双击按键响应间隔时间", DOUBLE_KEY_TIME, BIT, 3, 1, 8, ("200ms", 0), ("300ms", 1), ("400ms", 2), ("500ms", 3), ("600ms", 4), ("700ms", 5), ("800ms", 6), ("900ms", 7), USER_KEY_DOUBLE_EN);
config(CHECK, "复用ADKEY检测LINEIN", "是否使能复用ADKEY检测LINEIN功能", ADC_DETECT_LINEIN, 0, USER_ADKEY_EN);
config(LIST, "ADKEY1通路选择", "第一组ADKEY io mapping select", ADKEY_CH, 11, "ADCCH_PA5", "ADCCH_PA6", "ADCCH_PA7", "ADCCH_PB1", "ADCCH_PB2", "ADCCH_PB3", "ADCCH_PB4", "ADCCH_PE5", "ADCCH_PE6", "ADCCH_PE7", "ADCCH_PF2", 9, USER_ADKEY_EN);
config(LIST, "ADKEY2通路选择", "第二组ADKEY io mapping select", ADKEY2_CH, 11, "ADCCH_PA5", "ADCCH_PA6", "ADCCH_PA7", "ADCCH_PB1", "ADCCH_PB2", "ADCCH_PB3", "ADCCH_PB4", "ADCCH_PE5", "ADCCH_PE6", "ADCCH_PE7", "ADCCH_PF2", 8, USER_ADKEY2_EN);
config(LIST, "软开机长按时间选择", "长按PWRKEY多长时间后软开机", PWRON_PRESS_TIME, 8, "1.5秒", "2秒", "2.5秒", "3秒", "3.5秒", "4秒", "4.5秒", "5秒" 0, USER_PWRKEY_EN);
config(LIST, "软关机长按时间选择", "长按PWRKEY多长时间后软关机", PWROFF_PRESS_TIME, 8, "1.5秒", "2秒", "2.5秒", "3秒", "3.5秒", "4秒", "4.5秒", "5秒" 0, USER_PWRKEY_EN);

config(CHECK, "支持双击的按键定制", "是否配置支持双击的按键，不勾选使用SDK默认的双击按键配置列表", KEY_DOUBLE_CONFIG_EN, 0, USER_KEY_DOUBLE_EN);
config(LIST, "双击按键1选择", "支持双击第1个按键", DBLKEY_NUM0, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 1, KEY_DOUBLE_CONFIG_EN);
config(LIST, "双击按键2选择", "支持双击第2个按键", DBLKEY_NUM1, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 2, KEY_DOUBLE_CONFIG_EN);
config(LIST, "双击按键3选择", "支持双击第3个按键", DBLKEY_NUM2, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 5, KEY_DOUBLE_CONFIG_EN);
config(LIST, "双击按键4选择", "支持双击第4个按键", DBLKEY_NUM3, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 7, KEY_DOUBLE_CONFIG_EN);
config(LIST, "双击按键5选择", "支持双击第5个按键", DBLKEY_NUM4, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 14, KEY_DOUBLE_CONFIG_EN);

config(CHECK, "PWRKEY按键定制", "是否配置PWRKEY按键功能，不勾选使用SDK默认按键配置", PWRKEY_CONFIG_EN, 0, USER_PWRKEY_EN);
config(LIST, "PWRKEY按键1功能", "PWRKEY第1个按键功能定制", PWRKEY_NUM0, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 1, PWRKEY_CONFIG_EN);
config(LIST, "PWRKEY按键2功能", "PWRKEY第2个按键功能定制", PWRKEY_NUM1, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 7, PWRKEY_CONFIG_EN);
config(LIST, "PWRKEY按键3功能", "PWRKEY第3个按键功能定制", PWRKEY_NUM2, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 5, PWRKEY_CONFIG_EN);
config(LIST, "PWRKEY按键4功能", "PWRKEY第4个按键功能定制", PWRKEY_NUM3, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 12, PWRKEY_CONFIG_EN);
config(LIST, "PWRKEY按键5功能", "PWRKEY第5个按键功能定制", PWRKEY_NUM4, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 14, PWRKEY_CONFIG_EN);

config(CHECK, "ADKEY1按键定制", "是否配置第一组ADKEY按键功能，不勾选使用SDK默认按键配置", ADKEY_CONFIG_EN, 0, USER_ADKEY_EN);
config(LIST, "ADKEY1按键1功能", "ADKEY1第1个按键功能定制", ADKEY1_NUM0, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 2, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键2功能", "ADKEY1第2个按键功能定制", ADKEY1_NUM1, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 11, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键3功能", "ADKEY1第3个按键功能定制", ADKEY1_NUM2, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 10, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键4功能", "ADKEY1第4个按键功能定制", ADKEY1_NUM3, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 12, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键5功能", "ADKEY1第5个按键功能定制", ADKEY1_NUM4, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 14, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键6功能", "ADKEY1第6个按键功能定制", ADKEY1_NUM5, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 9, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键7功能", "ADKEY1第7个按键功能定制", ADKEY1_NUM6, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 8, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键8功能", "ADKEY1第8个按键功能定制", ADKEY1_NUM7, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 15, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键9功能", "ADKEY1第9个按键功能定制", ADKEY1_NUM8, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 16, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键10功能", "ADKEY1第10个按键功能定制", ADKEY1_NUM9, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 7, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键11功能", "ADKEY1第11个按键功能定制", ADKEY1_NUM10, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 5, ADKEY_CONFIG_EN);
config(LIST, "ADKEY1按键12功能", "ADKEY1第12个按键功能定制", ADKEY1_NUM11, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 0, ADKEY_CONFIG_EN);

config(CHECK, "ADKEY2按键定制", "是否配置第二组ADKEY，不勾选使用SDK默认按键配置", ADKEY2_CONFIG_EN, 0, USER_ADKEY2_EN);
config(LIST, "ADKEY2按键1功能", "ADKEY2第1个按键功能定制", ADKEY2_NUM0, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 2, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键2功能", "ADKEY2第2个按键功能定制", ADKEY2_NUM1, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 11, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键3功能", "ADKEY2第3个按键功能定制", ADKEY2_NUM2, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 10, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键4功能", "ADKEY2第4个按键功能定制", ADKEY2_NUM3, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 12, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键5功能", "ADKEY2第5个按键功能定制", ADKEY2_NUM4, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 14, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键6功能", "ADKEY2第6个按键功能定制", ADKEY2_NUM5, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 9, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键7功能", "ADKEY2第7个按键功能定制", ADKEY2_NUM6, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 8, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键8功能", "ADKEY2第8个按键功能定制", ADKEY2_NUM7, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 15, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键9功能", "ADKEY2第9个按键功能定制", ADKEY2_NUM8, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 16, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键10功能", "ADKEY2第10个按键功能定制", ADKEY2_NUM9, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 7, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键11功能", "ADKEY2第11个按键功能定制", ADKEY2_NUM10, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 5, ADKEY2_CONFIG_EN);
config(LIST, "ADKEY2按键12功能", "ADKEY2第12个按键功能定制", ADKEY2_NUM11, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 0, ADKEY2_CONFIG_EN);

config(CHECK, "IOKEY按键定制", "是否配置IOKEY，不勾选使用SDK默认按键配置", IOKEY_CONFIG_EN, 0, USER_IOKEY_EN);
config(LIST, "IOKEY按键1的IO", "IOKEY第1个按键的GPIO口选择", IOKEY_IO0, 25, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", 22, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键2的IO", "IOKEY第2个按键的GPIO口选择", IOKEY_IO1, 25, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", 23, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键3的IO", "IOKEY第3个按键的GPIO口选择", IOKEY_IO2, 25, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", 0, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键4的IO", "IOKEY第4个按键的GPIO口选择", IOKEY_IO3, 25, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", 0, IOKEY_CONFIG_EN);

config(LIST, "IOKEY按键1功能", "IOKEY第1个按键功能定制", IOKEY_NUM0, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 2, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键2功能", "IOKEY第2个按键功能定制", IOKEY_NUM1, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 6, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键3功能", "IOKEY第3个按键功能定制", IOKEY_NUM2, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 4, IOKEY_CONFIG_EN);
config(LIST, "IOKEY按键4功能", "IOKEY第4个按键功能定制", IOKEY_NUM3, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 12, IOKEY_CONFIG_EN);
config(LIST, "两IO中间的按键功能", "IOKEY1与IOKEY2两IO中间的按键", IOKEY_NUM4, 20, "None", "PP/POWER", "P/P", "POWER", "NEXT/VOL+", "VOL+/NEXT", "PREV/VOL-", "VOL-/PREV", "VOL+", "VOL-", "NEXT", "PREV", "MODE", "PP/MODE", "HSF", "EQ", "REPEAT", "MUTE", "VOL/NEXT/PREV", "SIRI", 0, IOKEY_CONFIG_EN);

config(SUB, "LED灯配置", "选择及配置LED状态指示灯");
config(CHECK, "系统指示灯(蓝灯)", "是否使用系统状态指示灯", LED_DISP_EN, 1);
config(CHECK, "电源状态灯(红灯)", "是否使用充电/电源指示灯", LED_PWR_EN, 1);
config(CHECK, "配对状态LED独立状态", "打开长按PWRKEY开机进入配对功能后，是否等待长按进入配对后再进入LED配对状态。", LED_BTPAIR_5S_EN, 0, LED_DISP_EN);
config(CHECK, "回连状态LED独立状态", "回连结束后是否进入LED配对状态", BT_RECON_LED_EN, 0, LED_DISP_EN); 
config(CHECK, "充满电蓝灯亮", "充电过程红灯亮，充电满红灯灭。充电满是否蓝灯亮？", CHARGE_FULL_BLED, 1, CHARGE_EN);
config(NUM,   "充满电蓝灯亮的时间(秒)", "充满电蓝灯亮的时间(单位:秒)，32767表示长亮。", CHARGE_FULL_BLED_ON_TIME, 0, 32767, 32767,CHARGE_FULL_BLED);
config(CHECK, "蓝灯低电平亮", "蓝灯是否低电电平亮？", BLED_LOW2ON_EN, 0, LED_DISP_EN);
config(CHECK, "蓝灯频谱闪烁", "蓝牙音乐播放时，蓝灯是否按音乐旋律闪烁", BLED_FFT_EN, 0, LED_DISP_EN);
config(CHECK, "电池低电闪灯", "电池低电时，红灯或蓝灯是否闪烁？", RLED_LOWBAT_EN, 0, LED_DISP_EN);
config(CHECK, "红灯跟随语音闪烁", "电池低电时，红灯是否跟随低电语音闪烁2下", RLED_LOWBAT_FOLLOW_EN, 0, RLED_LOWBAT_EN);
config(LIST, "蓝灯IO选择", "蓝灯的GPIO口选择", BLED_IO_SEL, 27, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", "PF3", "PF4", 26, LED_DISP_EN);
config(LIST, "红灯IO选择", "红灯的GPIO口选择", RLED_IO_SEL, 27, "None", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PB2", "PB3", "PB4", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PF0", "PF1", "PF2", "PF3", "PF4", 25, LED_PWR_EN);

config(CHECK, "开机状态配置LED", "是否配置开机LED指示灯状态", LED_PWRON_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "开机", "配置开机状态下的闪灯状态", LED_POWERON, 0x02, 0x01, 10, 255, 		LED_PWRON_CONFIG_EN);

config(CHECK, "关机状态配置LED", "是否配置等待关机状态指示灯", LED_PWROFF_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "关机", "配置关机状态下的闪灯状态", LED_POWEROFF, 0x40, 0x15, 4, 255, 		LED_PWROFF_CONFIG_EN);

config(CHECK, "蓝牙初始化状态配置LED", "是否配置初始化状态指示灯(蓝牙开机播提示音、等待5秒按键时)", LED_BTINIT_CONFIG_EN, 0);
config(LED, "初始化", "配置蓝牙初始化状态下的闪灯状态", LED_BTINIT, 0x00, 0xaa, 10, 0, 	LED_BTINIT_CONFIG_EN);

config(CHECK, "蓝牙回连状态配置LED", "是否配置蓝牙回连状态指示灯(蓝牙未连接)", LED_RECONNECT_CONFIG_EN, 0);
config(LED, "回连", "配置回连状态下的闪灯状态", LED_RECONNECT, 0x00, 0xaa, 6, 0, 		LED_RECONNECT_CONFIG_EN);

config(CHECK, "蓝牙配对状态配置LED", "是否配置蓝牙配对状态指示灯(蓝牙未连接)", LED_BTPAIR_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "配对", "配置配对状态下的闪灯状态", LED_PAIRING, 0x60, 0x03, 3, 6, 			LED_BTPAIR_CONFIG_EN);

config(CHECK, "蓝牙已连接状态配置LED", "是否配置蓝牙已连接状态指示灯", LED_BTCONN_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "已连接", "配置已连接状态下的闪灯状态", LED_CONNECTED, 0x00, 0x02, 2, 86, 	LED_BTCONN_CONFIG_EN);

config(CHECK, "蓝牙音乐状态配置LED", "是否配置蓝牙音乐状态指示灯", LED_BTMUSIC_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "播放", "配置播放状态下的闪灯状态", LED_PLAYING, 0x00, 0x02, 2, 86, 		LED_BTMUSIC_CONFIG_EN);

config(CHECK, "蓝牙来电状态配置LED", "是否配置蓝牙来电状态指示灯", LED_BTRING_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "来电", "配置来电状态下的闪灯状态", LED_RING, 0x00, 0x02, 2, 86, 			LED_BTRING_CONFIG_EN);

config(CHECK, "蓝牙通话状态配置LED", "是否配置蓝牙通话状态指示灯", LED_BTCALL_CONFIG_EN, 0, LED_DISP_EN);
config(LED, "通话", "配置通话状态下的闪灯状态", LED_CALLING, 0x00, 0x02, 2, 86, 		LED_BTCALL_CONFIG_EN);

config(CHECK, "低电状态配置LED", "是否配置低电状态指示灯", LED_LOWBAT_CONFIG_EN, 0, RLED_LOWBAT_EN);
config(LED, "低电", "配置低电状态下的闪灯状态", LED_LOWBAT, 0xaa, 0x00, 6, 0, 			LED_LOWBAT_CONFIG_EN);

config(LEVEL, 0x100);
makecfgfile(xcfg.bin);
makecfgdef(xcfg.h);
xcopy(xcfg.h, ../../xcfg.h);