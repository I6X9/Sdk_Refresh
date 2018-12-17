#include "include.h"
#include "bsp_led.h"

#if LED_DISP_EN

typedef struct {
    u8 rled_sta;   //��� ��-�����
    u8 bled_sta;   //���� ��-�����
    u8 uint;        //50ms��λ (��Χ��1~255)
    u8 period;      //������� (��Χ��0~255)

    u8 cnt;
    u8 bcnt;
    u8 space_cnt;

    u8 flag;
} led_cb_t;

led_cb_t led_cb AT(.buf.led);
led_cb_t led_bak AT(.buf.led);
volatile int port2led_sta AT(.buf.led);

AT(.com_text.led)
void tmr5ms_set_sync(void)
{
    led_cb.flag |= LED_SYNC;
}

AT(.com_text.led)
static void led_sync(void)
{
#if BT_TWS_EN
    if((led_cb.flag & LED_SYNC)) {
        led_cb.flag &= ~LED_SYNC;
        led_cb.bcnt = 0;
        led_cb.cnt = 0;
        led_cb.space_cnt = 0;
        bled_set_off();
        rled_set_off();
    }
#endif
}

//50ms��������
AT(.com_text.led_disp)
void led_scan(void)
{
    if (!is_led_scan_enable()) {
        return;
    }

    led_sync();

    //�ȴ����ʱ��
    if (led_cb.space_cnt) {
        led_cb.space_cnt--;
        return;
    }

    if (led_cb.cnt < led_cb.uint) {
        led_cb.cnt++;
    }
    if (led_cb.cnt == led_cb.uint) {    //��λ��ƣ����ʱ��
        led_cb.cnt = 0;
        if (led_cb.bcnt == 8) {
            led_cb.bcnt = 0;
            if (led_cb.period) {        //���ʱ��
                if (led_cb.period == 0xff) {
                    //ֻ��һ��
                    led_cb.bled_sta = 0;
                    led_cb.rled_sta = 0;
                    led_cb.period = 0;
                }
                led_cb.space_cnt = led_cb.period;
                return;
            }
        }
        if (led_cb.bled_sta & BIT(led_cb.bcnt)) {
            bled_set_on();
        } else {
            bled_set_off();
        }
        if (led_cb.rled_sta & BIT(led_cb.bcnt)) {
            rled_set_on();
        } else {
            rled_set_off();
        }
        led_cb.bcnt++;
    }
}

//��ȡLED��ǰ���õ�״̬
AT(.com_text.led_disp)
u8 get_led_sta(u32 type)
{
    if (type) {
        return led_cb.rled_sta;
    } else {
        return led_cb.bled_sta;
    }
}

AT(.text.led_disp)
void led_cfg_port_init(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (g->type) {
        //PE0��ѹIOû���ڲ�300R���������ⲿ��������
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxCLR] = BIT(g->num);
    } else {
        g->sfr[GPIOxDIR] |= BIT(g->num);
        g->sfr[GPIOxPU300] &= ~BIT(g->num);
        g->sfr[GPIOxDE] &= ~BIT(g->num);
    }
}

AT(.com_text.led_disp)
void led_cfg_set_on(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (g->type) {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxSET] = BIT(g->num);
    } else {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxPU300] |= BIT(g->num);
    }
}

AT(.com_text.led_disp)
void led_cfg_set_off(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (g->type) {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxCLR] = BIT(g->num);
    } else {
        g->sfr[GPIOxDE] &= ~BIT(g->num);
        g->sfr[GPIOxPU300] &= ~BIT(g->num);
    }
}

#if USER_ADKEY_MUX_LED
AT(.text.led_disp)
void led_mux_cfg_port_init(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    g->sfr[GPIOxDE] |= BIT(g->num);
    g->sfr[GPIOxDIR] &= ~BIT(g->num);
    g->sfr[GPIOxSET] = BIT(g->num);
}

AT(.com_text.led_disp)
void led_mux_cfg_set_on(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    g->sfr[GPIOxDE] |= BIT(g->num);
    g->sfr[GPIOxDIR] &= ~BIT(g->num);
    g->sfr[GPIOxCLR] = BIT(g->num);
}

AT(.com_text.led_disp)
void led_mux_cfg_set_off(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    g->sfr[GPIOxDE] |= BIT(g->num);
    g->sfr[GPIOxDIR] &= ~BIT(g->num);
    g->sfr[GPIOxSET] = BIT(g->num);
}
#endif // USER_ADKEY_MUX_LED

//һ��IO��������LED��
void led2_port_init(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }

    if (g->type) {
        //PE0��ѹIOû���ڲ�300R���������ⲿ��������
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] |= BIT(g->num);
    } else {
        g->sfr[GPIOxDE] &= ~BIT(g->num);
        g->sfr[GPIOxDIR] |= BIT(g->num);
        g->sfr[GPIOxPU300] &= ~BIT(g->num);
        g->sfr[GPIOxPD300] &= ~BIT(g->num);
    }
    port2led_sta = 0;
}

//�������������
static ALWAYS_INLINE void port_2bled_on(gpio_t *g)
{
    if (g->type) {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxCLR] = BIT(g->num);
    } else {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] |= BIT(g->num);
        g->sfr[GPIOxPD300] |= BIT(g->num);
    }
}

//�����𣬺����
static ALWAYS_INLINE void port_2rled_on(gpio_t *g)
{
    if (g->type) {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxSET] = BIT(g->num);
    } else {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] &= ~BIT(g->num);
        g->sfr[GPIOxPD300] &= ~BIT(g->num);
        g->sfr[GPIOxSET] = BIT(g->num);
    }
}

//�����𣬺����
static ALWAYS_INLINE void port_2led_off(gpio_t *g)
{
    if (g->type) {
        g->sfr[GPIOxDE] |= BIT(g->num);
        g->sfr[GPIOxDIR] |= BIT(g->num);
    } else {
        g->sfr[GPIOxDE] &= ~BIT(g->num);
        g->sfr[GPIOxDIR] |= BIT(g->num);
        g->sfr[GPIOxPU300] &= ~BIT(g->num);
        g->sfr[GPIOxPD300] &= ~BIT(g->num);
    }
}

AT(.com_text.led_disp)
void bled2_set_on(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }
    port_2bled_on(g);
    port2led_sta |= BIT(0);
}

AT(.com_text.led_disp)
void bled2_set_off(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }
    port_2led_off(g);
    port2led_sta &= ~BIT(0);
}

AT(.com_text.led_disp)
void rled2_set_on(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }
    port_2rled_on(g);
    port2led_sta |= BIT(1);
}

AT(.com_text.led_disp)
void rled2_set_off(gpio_t *g)
{
    if ((g == NULL) || (g->sfr == NULL)) {
        return;
    }
    port_2led_off(g);
    port2led_sta &= ~BIT(1);
}

//ÿmsɨ��һ��
AT(.com_text.led_disp)
void port_2led_scan(void)
{
    gpio_t *g = &bled_gpio;
    if ((g->sfr == NULL) || (!sys_cb.port2led_en)) {
        return;
    }

    port2led_sta ^= BIT(7);
    if ((port2led_sta & 0x03) == 0x03) {
        if (port2led_sta & BIT(7)) {
            port_2bled_on(g);
        } else {
            port_2rled_on(g);
        }
    } else if ((port2led_sta & 0x81) == 0x81) {
        port_2bled_on(g);
    } else if ((port2led_sta & 0x82) == 0x82) {
        port_2rled_on(g);
    } else {
        port_2led_off(g);
    }
}

//1��IO�������ƣ����״̬�²���sleep����������쳣��
AT(.sleep_text.led)
bool port_2led_is_sleep_en(void)
{
    if ((sys_cb.port2led_en) && (CHARGE_DC_IN())) {
        return false;
    }
    return true;
}

AT(.com_text.led_disp)
void bled_set_off(void)
{
    if (!sys_cb.charge_bled_flag) {       //����״̬���ȿ���
        LED_SET_OFF();
    }
}

AT(.com_text.led_disp)
void bled_set_on(void)
{
   if (!sys_cb.charge_bled_flag) {        //����״̬���ȿ���
        LED_SET_ON();
    }
}

AT(.com_text.led_disp)
void rled_set_off(void)
{
    if (!sys_cb.charge_sta) {             //���״̬���ȿ���
        LED_PWR_SET_OFF();
    }
}

AT(.com_text.led_disp)
void rled_set_on(void)
{
   if (!sys_cb.charge_sta) {               //���״̬���ȿ���
        LED_PWR_SET_ON();
    }
}

AT(.text.led_disp)
void led_init(void)
{
    LED_INIT();
    LED_PWR_INIT();
    memset(&led_cb, 0, sizeof(led_cb));
}

//������
AT(.text.led_disp)
void led_off(void)
{
    led_cb.bled_sta = 0x00;
    bled_set_off();
}

//������
AT(.text.led_disp)
void led_on(void)
{
    led_cb.bled_sta = 0xff;
    bled_set_on();
}

AT(.text.led_disp)
void rled_on(void)
{
    led_cb.rled_sta = 0xff;
    rled_set_on();
}

AT(.text.led_disp)
void rled_off(void)
{
    led_cb.rled_sta = 0x00;
    rled_set_off();
}

//�γ��������
AT(.com_text.led_disp)
void charge_led_off(void)
{
    LED_PWR_SET_OFF();
}

//�������
AT(.com_text.led_disp)
void charge_led_on(void)
{
    LED_PWR_SET_ON();
}

//�������Ƴ���
void charge_bled_on(void)
{
    LED_SET_ON();
}

AT(.com_text.led_disp)
void led_set_sta(u8 rled_sta, u8 bled_sta, u8 uint, u8 period)
{
    led_cb_t *s = &led_cb;

    if (s->flag & LED_TOG_LBAT) {                   //�͵���������
        s = &led_bak;
    }

    rled_set_off();
    bled_set_off();

    s->rled_sta = rled_sta;
    s->bled_sta = bled_sta;
    s->uint = uint;
    s->period = period;

    s->cnt = 0;
    s->bcnt = 0;
    s->space_cnt = 0;
    s->flag |= LED_SYNC;
}

AT(.com_text.led_disp)
void led_cfg_set_sta(led_cfg_t *cfg_cb)
{
    led_cb_t *s = &led_cb;

    if (s->flag & LED_TOG_LBAT) {                   //�͵���������
        s = &led_bak;
    }

    rled_set_off();
    bled_set_off();

    memcpy(s, cfg_cb, 4);
    s->cnt = 0;
    s->bcnt = 0;
    s->space_cnt = 0;
    s->flag |= LED_SYNC;
}

AT(.com_text.led_disp)
void led_lowbat(void)
{
    if (!(led_cb.flag & LED_TOG_LBAT)) {
        memcpy(&led_bak, &led_cb, sizeof(led_cb_t));
        if (xcfg_cb.led_lowbat_config_en) {
            led_cfg_set_sta((led_cfg_t *)&xcfg_cb.led_lowbat);
        } else {
            led_set_sta(0xaa, 0x00, 6, 0);              //���300ms������˸
        }
        led_cb.flag |= LED_TOG_LBAT;
    }
}

AT(.com_text.led_disp)
void led_lowbat_recover(void)
{
    if (led_cb.flag & LED_TOG_LBAT) {
        memcpy(&led_cb, &led_bak, sizeof(led_cb_t));
        led_cb.flag &= ~LED_TOG_LBAT;
    }
}

void led_lowbat_follow_warning(void)
{
    if (!(led_cb.flag & LED_TOG_LBAT)) {
        memcpy(&led_bak, &led_cb, sizeof(led_cb_t));
        led_set_sta(0x0a, 0x00, 2, 0xff);              //���100ms������2��
        led_cb.flag |= LED_TOG_LBAT;
    }
}

#endif
