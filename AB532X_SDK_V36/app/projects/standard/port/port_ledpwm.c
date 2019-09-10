#include "include.h"
#include "func.h"
#include "func_idle.h"

#if PWM_TIMER_EN					//timer pwm��ʾ

#define RGB_MAX_DUTY				32				//RGB ���ռ�ձȣ���λ0.1%
#define GUI_MAX_DUTY				1000			//GUI ��ʾ���ռ�ձȣ���λ0.1%
#define DUTY_SCAN_CNT				50				//��ɫ�л��ֱ���

#define PWM_RED_DUTY_SET(Duty) 		timer_pwm_config(PWM_TIMER5|PWM_CHANNEL2, 500, Duty, 0)
#define PWM_GREEN_DUTY_SET(Duty)	timer_pwm_config(PWM_TIMER5|PWM_CHANNEL1, 500, Duty, 0)
#define PWM_BLUE_DUTY_SET(Duty) 	timer_pwm_config(PWM_TIMER5|PWM_CHANNEL0, 500, Duty, 0)
#define PWM_GUI_DUTY_SET(Duty)		timer_pwm_config(PWM_TIMER4|PWM_CHANNEL2, 50000, Duty, 1)

typedef struct {
	uint8_t		red;
	uint8_t		green;
	uint8_t		blue;
}rgb_duty;

const rgb_duty rgb_duty_tbl[COLOUR_MAX] = {
	{0, 	0,		0,	}, 							//��
	{0,		100,	100,},							//��
	{100,	0,		100,}, 							//��
	{100,	15,		0,	}, 							//��
	{100,	100,	100,}, 							//��
	{100,	50,		0,	}, 							//��
};

rgb_duty	set_rgb, cur_rgb, range;
uint32_t	rgb_timer = 0, gui_timer = 0;
uint16_t	gui_period = DEF_SCAN_TIMER, rgb_period = DEF_SCAN_TIMER;


void pwm_gpio_init(void)
{
	GPIOADE |= (BIT(5)|BIT(6)|BIT(7));
	GPIOADIR &= ~(BIT(5)|BIT(6)|BIT(7));
	GPIOASET = (BIT(5)|BIT(6)|BIT(7));
	func_water_pump_process(0);
}
/*****************************************************************************
 �� �� ��  : pwm_init
 ��������  : pwm�ӿڳ�ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��19��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void timer_pwm_init(void)
{
	pwm_gpio_init();
	PWM_GUI_DUTY_SET(0);
	timer_pwm_enable(PWM_TIMER4|PWM_CHANNEL2, TIMEPWMMAP_G2);
	PWM_RED_DUTY_SET(0);
	timer_pwm_enable(PWM_TIMER5|PWM_CHANNEL0, TIMEPWMMAP_G1);
	PWM_GREEN_DUTY_SET(0);
	timer_pwm_enable(PWM_TIMER5|PWM_CHANNEL1, TIMEPWMMAP_G1);
	PWM_BLUE_DUTY_SET(0);
	timer_pwm_enable(PWM_TIMER5|PWM_CHANNEL2, TIMEPWMMAP_G1);
	memset(&set_rgb, 0, sizeof(rgb_duty));
}

/*****************************************************************************
 �� �� ��  : gui_light_scan
 ��������  : gui��ʾLEDɨ�账��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��22��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
static void gui_light_scan(void)
{
	static uint16_t gui_duty;
	static bool	direction;

	if (direction) {
		gui_duty += (GUI_MAX_DUTY/DUTY_SCAN_CNT);
		
		if (gui_duty >= GUI_MAX_DUTY) {
			direction = !direction;
		}
	}
	else {
		if (gui_duty >= (GUI_MAX_DUTY/DUTY_SCAN_CNT)) {
			gui_duty -= (GUI_MAX_DUTY/DUTY_SCAN_CNT);
		}
		else {
			direction = !direction;
		}
	}
	//printf ("gui set duty %d;\n", gui_duty);
	
	PWM_GUI_DUTY_SET(gui_duty);
}
/*****************************************************************************
 �� �� ��  : pwm_rgb_duty_calc
 ��������  : ���㵱ǰӦ����ʾ��ռ�ձ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��19��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
static void pwm_rgb_scan(void)
{
	static uint16_t red, green, blue;
	
	if (cur_rgb.red != set_rgb.red) {
		if (cur_rgb.red < set_rgb.red) {
			red += range.red*100/DUTY_SCAN_CNT;
		}
		else {
			red -= range.red*100/DUTY_SCAN_CNT;
		}
		cur_rgb.red = red/100;
		
		PWM_RED_DUTY_SET(cur_rgb.red);
	}

	if (cur_rgb.green != set_rgb.green) {
		if (cur_rgb.green < set_rgb.green) {
			green += range.green*100/DUTY_SCAN_CNT;
		}
		else {
			green -= range.green*100/DUTY_SCAN_CNT;
		}
		cur_rgb.green = green/100;
		
		PWM_GREEN_DUTY_SET(cur_rgb.green);
	}

	if (cur_rgb.blue != set_rgb.blue) {
		if (cur_rgb.blue < set_rgb.blue) {
			blue += range.blue*100/DUTY_SCAN_CNT;
		}
		else {
			blue -= range.blue*100/DUTY_SCAN_CNT;
		}
		cur_rgb.blue = blue/100;
		
		PWM_BLUE_DUTY_SET(cur_rgb.blue);
	}
}

/*****************************************************************************
 �� �� ��  : pwm_gui_setting
 ��������  : ����gui ��ʾ�仯����
 �������  : uint16_t period  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��24��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void pwm_gui_setting(uint16_t period)
{
	gui_period = period/DUTY_SCAN_CNT;
}

/*****************************************************************************
 �� �� ��  : pwm_rgb_write
 ��������  : ����rgb ����ʾ��ɫ���仯����
 �������  : uint8_t rgb_colour   
             uint16_t period_rgb  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��24��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
void pwm_rgb_setting(uint8_t rgb_colour, uint16_t period_rgb)
{
	static uint8_t prev_disp_colour;

	rgb_period = period_rgb/DUTY_SCAN_CNT;

	if (prev_disp_colour != rgb_colour) {
		printf ("cur disp colour %d;\n", rgb_colour);
		prev_disp_colour = rgb_colour;
		set_rgb.red = RGB_MAX_DUTY*rgb_duty_tbl[rgb_colour].red/100;
		if (set_rgb.red >= cur_rgb.red) {
			range.red = (set_rgb.red - cur_rgb.red);
		}
		else {
			range.red = (cur_rgb.red - set_rgb.red);
		}
	 
		set_rgb.green = RGB_MAX_DUTY*rgb_duty_tbl[rgb_colour].green/100;
		if (set_rgb.green >= cur_rgb.green) {
			range.green = (set_rgb.green - cur_rgb.green);
		}
		else {
			range.green = (cur_rgb.green - set_rgb.green);
		}
		
		set_rgb.blue = RGB_MAX_DUTY*rgb_duty_tbl[rgb_colour].blue/100;
		if (set_rgb.blue >= cur_rgb.blue) {
			range.blue = (set_rgb.blue - cur_rgb.blue);
		}
		else {
			range.blue = (cur_rgb.blue - set_rgb.blue);
		}
	}
}

/*****************************************************************************
 �� �� ��  : ��ʾ����
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��6��19��
    ��    ��   : qing
    �޸�����   : �����ɺ���

*****************************************************************************/
AT(.text.ledpwm)
void func_led_pwm_scan(void)
{
	if (tick_check_expire(rgb_timer, rgb_period)) {
		rgb_timer = tick_get();
		pwm_rgb_scan();
	}

	if (tick_check_expire(gui_timer, gui_period)) {
		gui_timer = tick_get();
		gui_light_scan();
	}
}

#endif
