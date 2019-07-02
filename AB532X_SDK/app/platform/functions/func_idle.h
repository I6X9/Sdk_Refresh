#ifndef _FUNC_IDLE_H
#define _FUNC_IDLE_H

#define	WAVE_MIN_FREQ					490			
#define	WAVE_MAX_FREQ					510
#define	WAVE_STILL_FREQ					495
#define WAVE_MAX_OPERATION				42

#define DEF_SCAN_TIMER					1000
#define GUI_IDLE_TIMER					5000
#define GUI_WORK_TIMER					2000				
#define GUI_ERRO_TIMER					500
#define RGB_SWAP_TIMER					3000			//ȫ����ɫ�л�ʱ��
#define RGB_STOP_TIMER					5000			//ȫ����ɫͣ��ʱ��

enum {
	COLOUR_IDLE = 0,
	COLOUR_RED,
	COLOUR_ORANGE,
	COLOUR_YELLOW,
	COLOUR_GREEN,
	COLOUR_CYAN,
	COLOUR_BLUE,
	COLOUR_PURPLE,
	COLOUR_WHITE,
	COLOUR_MAX,
};

typedef struct {
	gpio_t		wave_gpio;
	uint16_t	wave_freq;
	uint32_t	wave_timer;
	uint32_t	rgb_blink;
	uint8_t		rgb_colour;
	uint8_t		wave_cnt;
	uint8_t		cur_status;
	bool		out_level;
}func_idle_t;

extern func_idle_t		f_idle;

void func_idle_message(u16 msg);
void func_water_pump_process(bool work);

#if (GUI_SELECT != GUI_NO)
void func_idle_display(void);
#else
#define func_idle_display()
#endif

#endif // _FUNC_IDLE_H
