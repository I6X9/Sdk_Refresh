#include "include.h"
#include "bsp_lcd.h"

#if DISP_LCD_EN
static void lcd_delay_ms(u16 ms)
{
    while(ms--) {
        delay_us(800);
    }
}

/****************************************************
��    ��: ���з��͹��ɵ�Ԫ
��������: ˽��
��ڲ���: u8 base_byte (���з��͵�Ԫ�ֽ�)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void serial_base(u8 base_byte)
{
    s8 i;
    for(i=7; i>=0; i--) {
        if(base_byte&(0x01<<i)) {
            LCD_DAT_SET();
            asm("nop");
        } else {
            LCD_DAT_CLR();
            asm("nop");
        }
        LCD_CLK_SET();
        asm("nop");asm("nop");
        asm("nop");asm("nop");      //�½���
        LCD_CLK_CLR();
        asm("nop");
    }
}

/****************************************************
��    ��: ���з������ݻ�ָ��
��������: ˽��
��ڲ���: byte(��������),type(�������ͣ�����orָ��)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_serial_send(u8 byte, u8 type)
{
    u8 temp = 0;

    cli();
    LCD_CLK_CLR();
    LCD_CS_SET();                                   //���жϣ�sclk=0��cs=1
    if(type==0x01) {
        temp = 0xf8;    //rw=0,rs=0
    } else {
        temp = 0xfa;    //rw=0,rs=1
    }
    asm("nop");
    serial_base(temp);
    serial_base(byte&0xf0);                         //���͸���λ
    serial_base((byte<<4)&0xf0);                    //���͵���λ
    LCD_CS_CLR();
    sei();                                          //cs=0,���ж�
    delay_us(72);
}

/****************************************************
��    ��: 12864��ʼ��
��������: ����
��ڲ���: ��
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_init(void)
{
    LCD_DIR_DIROUT();
    LCD_CS_DIROUT();
    LCD_CLK_DIROUT();
    LCD_DAT_DIROUT();                               //cs,sclk,std���������

    LCD_DIR_SET();
    LCD_CS_CLR();
    LCD_CLK_CLR();
    LCD_DAT_CLR();                                  //cs=sclk=std=0

    lcd_delay_ms(30);
    lcd_serial_send(lcd_funtion_base, command);     //����ָ�lcd_funtion_base=0x30
    lcd_serial_send(lcd_disp_on, command);          //����ʾ
    lcd_serial_send(lcd_clean_disp, command);       //����ʾ
    lcd_delay_ms(5);
    lcd_serial_send(lcd_dote_left, command);
    lcd_delay_ms(5);
    lcd_clean_cgram();
}

/****************************************************
��    ��: ������RAM
��������: ˽��
��ڲ���: ��
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_clean_ddram(void)
{
    lcd_serial_send(lcd_funtion_base, command);
    lcd_serial_send(lcd_clean_disp, command);
    lcd_delay_ms(5);
}

/****************************************************
��    ��: ��ͼ��RAM
��������: ˽��
��ڲ���: ��
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_clean_cgram(void)
{
    u8 x;
    u8 y;

    lcd_serial_send(lcd_funtion_additional, command);       //��չָ�
    lcd_serial_send(lcd_move_select, command);              //�����趨cgram��ַ
    lcd_serial_send(lcd_funtion_additional, command);       //��cgram��ʾ
    for(x=0; x<16; x++) {
        for(y=0; y<32; y++) {
            lcd_serial_send(lcd_cgram_address|y, command);  //���û�ͼ����Y��ַ����
            lcd_serial_send(lcd_cgram_address|x, command);  //���û�ͼ����X��ַ����
            lcd_serial_send(0x00, data);
            lcd_serial_send(0x00, data);
        }
    }
    lcd_serial_send(lcd_funtion_additional|0x02, command);  //��cgram��ʾ
}

/****************************************************
��    ��: �趨��������
��������: ˽��
��ڲ���: x(�����1-8),y(�����1-4)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_set_xy(u8 x, u8 y)
{
    u8 address_temp = 0;

    if(--x>7) x=0;
    if(--y>3) y=0;
    switch(y) {
        case 0: address_temp = 0x80+x; break;
        case 1: address_temp = 0x90+x; break;
        case 2: address_temp = 0x88+x; break;
        case 3: address_temp = 0x98+x; break;
    }
    lcd_serial_send(lcd_set_ddram_address|address_temp, command);
}

/****************************************************
��    ��: ��ʾһ������
��������: ����
��ڲ���: x(�����1-18),y(�����1-4)word(����ascii��)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_disp_char(u8 x, u8 y, u8 word_high, u8 word_low)
{
    lcd_serial_send(lcd_funtion_base, command);
    lcd_set_xy(x, y);
    lcd_serial_send(word_high,data);
    lcd_serial_send(word_low,data);
}

/****************************************************
��    ��: ��ʾһ���ַ������ĺͷ��ţ�
��������: ����
��ڲ���: x(�����1-8), y(�����1-4), *str(�ַ���ָ��)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.8
����޸�: ����(2009.10.8)  �޸���(zoro)
****************************************************/
void lcd_disp_str(u8 x, u8 y, char *str)
{
    u8 i = 0;

    x--;
    while(str[i]) {
        if(++x >= 9) {
            x = 1;
            y++;
        }

        if(str[i+1] == '\0') {
            lcd_disp_char(x, y, str[i], '\0');
            break;
        } else {
            lcd_disp_char(x, y, str[i], str[i+1]);
        }
        i = i+2;
    }
}

/****************************************************
��    ��: ��ͼ��ʽд��һ������(16*16)
��������: ����
��ڲ���: *charture(�ֿ���ָ��)
���ڲ���: ��
��    ��: ljf
��    ��: 2009.1.6
����޸�: ����(2009.1.6)  �޸���(ljf)
***************************************************
void lcd_disp_charture(u8 x, u8 y, u8 *charture)
{
    u16 index = 0;
    u8 x_start_temp = x-1;
    u8 y_start_temp = (y-1)*16;
    u8 x_end_temp = x;//x+1-1
    u8 y_end_temp = y*16;//16+(y-1)*16

    lcd_serial_send(lcd_funtion_additional, command);       //��չָ�
    lcd_serial_send(lcd_move_select, command);              //��������IRAM��ַ
    if(y_start_temp < 32) {                                 //���ڵ�1��2��ʱ
        while(y_start_temp < y_end_temp) {
            while(x_start_temp < x_end_temp) {
                lcd_serial_send(lcd_cgram_address|y_start_temp, command);            //y����
                lcd_serial_send(lcd_cgram_address|x_start_temp, command);            //x����
                lcd_serial_send(charture[index], data);                //���ֽ�
                index++;
                lcd_serial_send(charture[index], data);                //���ֽ�
                index++;
                x_start_temp++;
            }
            x_start_temp = x-1;
            y_start_temp++;
        }
    } else {                                                    //���ڵ�3��4��ʱ
        y_start_temp -= 32;
        y_end_temp -= 32;
        x_start_temp += 8;
        x_end_temp += 8;
        while(y_start_temp < y_end_temp) {
            while(x_start_temp < x_end_temp) {
                lcd_serial_send(lcd_cgram_address|y_start_temp, command);   //y����
                lcd_serial_send(lcd_cgram_address|x_start_temp, command);   //x����
                lcd_serial_send(charture[index], data);         //���ֽ�
                index++;
                lcd_serial_send(charture[index], data);         //���ֽ�
                index++;
                x_start_temp++;
            }
            x_start_temp = x+7;//x+8-1
            y_start_temp++;
        }
    }
    lcd_serial_send(lcd_picture_on, command);                   //��cgram��ʾ
}
*/
/****************************************************
��    ��: ��ͼ��ʽд��һ������(16*16)
��������: ����
��ڲ���: *str_ture(�ֿ���ָ��)
���ڲ���: ��
��    ��: ljf
��    ��: 2009.1.6
����޸�: ����(2009.1.6)  �޸���(ljf)
***************************************************
void lcd_disp_picstr_slow(u8 x,unsigned y,u8 *picstr)
{
    u8 i = 0;
    u8 count = 0;
    while(picstr[i]) {
        if(++x > 9) {
            x = 2;
            y++;
        }
        lcd_disp_charture(x-1, y, picstr[i]);
        i = i+2;
        count++;
        lcd_delay_ms(200);
        if(count > 31) {
            lcd_clean_ddram();
            lcd_delay_ms(200);
            count=0;
            x=1;
            y=1;
        }
    }
}
*/

/****************************************************
��    ��: ��ʾһ��ͼƬ
��������: ����
��ڲ���: *picture(ͼƬ��ָ��)
���ڲ���: ��
��    ��: zoro
��    ��: 2009.10.9
����޸�: ����(2009.10.9)  �޸���(zoro)
***************************************************
void lcd_disp_picture(u8 x, u8 y, u8 *picture)
{
    volatile u8 x_max = 0;
    volatile u8 y_max = 0;
    u16 index = 2;
    u8 x_start_temp;
    u8 y_start_temp;
    u8 x_end_temp;
    u8 y_end_temp;
    u8 temp;

    lcd_serial_send(lcd_funtion_additional, command);   //��չָ�
    lcd_serial_send(lcd_move_select, command);          //��������IRAM��ַ

    x_max = picture[0]+x;
    y_max = picture[1]+y;
    if(y > 31) {
        y_start_temp = y-32;
        x_start_temp = (x/16)+8;
        x_end_temp = (x_max/16)+8;
        y_end_temp = y_max-32;
        while(y_start_temp < y_end_temp) {
            while(x_start_temp < x_end_temp) {
                lcd_serial_send(lcd_cgram_address|y_start_temp,command);
                lcd_serial_send(lcd_cgram_address|x_start_temp,command);
                lcd_serial_send(picture[index],data);
                lcd_serial_send(picture[index+1],data);
                index = index+2;
                x_start_temp++;
            }
            x_start_temp = (x/16)+8;
            y_start_temp++;
        }
    } else {
        if(y_max > 31) {
            y_start_temp = y;
            x_start_temp = x/16;
            x_end_temp = (x_max/16) + 8;
            y_end_temp = y_max-32;
            while(y_start_temp < 32) {
                while(x_start_temp<x_end_temp) {
                    lcd_serial_send(lcd_cgram_address|y_start_temp, command);
                    lcd_serial_send(lcd_cgram_address|x_start_temp, command);
                    lcd_serial_send(picture[index], data);
                    lcd_serial_send(picture[index+1], data);
                    index = index+2;
                    x_start_temp++;
                }
                x_start_temp = x/16;
                y_start_temp++;
            }
            y_start_temp = 0;
            x_start_temp = x/16+8;

            while(y_start_temp < y_end_temp) {
                while(x_start_temp < x_end_temp) {
                    lcd_serial_send(lcd_cgram_address|y_start_temp, command);
                    lcd_serial_send(lcd_cgram_address|x_start_temp, command);
                    lcd_serial_send(picture[index], data);
                    lcd_serial_send(picture[index+1], data);
                    index = index+2;
                    x_start_temp++;
                }
                x_start_temp = x/16+8;
                y_start_temp++;
            }
        } else {
            y_start_temp = y;
            x_start_temp = x/16;
            x_end_temp = x_max/16;
            y_end_temp = y_max;
            while(y_start_temp < y_start_temp) {
                while(x_start_temp < x_end_temp) {
                    lcd_serial_send(lcd_cgram_address|y_start_temp, command);
                    lcd_serial_send(lcd_cgram_address|x_start_temp, command);
                    lcd_serial_send(picture[index], data);
                    lcd_serial_send(picture[index+1], data);
                    index = index+2;
                    x_start_temp++;
                }
                x_start_temp = x/16;
                y_start_temp++;
            }
        }
    }

    lcd_serial_send(lcd_picture_on, command);           //��cgram��ʾ
}
*/

#endif //DISP_LCD_EN
