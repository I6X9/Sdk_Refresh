#ifndef _KEY_UART_H_
#define _KEY_UART_H_

//#include "key_inner.h"
//#include "psp_includes.h"
#include "key_interface.h"

#define WHOLECHIP_SIM
//#define K_PLL_FREQ        36
#define K_MCUPLL_VALUE    (0x40|(K_PLL_FREQ/6))
//#define I2C_SPEED         0x00   // 00: 100k;  32=0x20:400K

#define K_VARBUF_SIZE     0x21 //������������ݰ�����
//��дbuffer ��С
#define PYH_WRBUF_SIZE 8

///////////////////////////////////////////////////////////////////////////////////
#define PASSED     0
#define FAILED     1

//logical communication finite state machine
#define COMUNICATE_IDLE               0x00 /*0��ʾ����״̬*/
#define COMUNICATE_BUSY               0x01 /*1��ʾæ״̬*/
#define COMUNICATE_REPLY              0x02 /*2��ʾI2C�ӻ���ʼӦ�𣬳�Ӧ������һ�����ݰ�*/
#define COMUNICATE_COMND              0x03 /*3��ʾ��������ɹ�*/
#define COMUNICATE_FAIL               0x04 /*4��ʾI2C�ӻ�����ʧ��*//*��ʱ����*/
//#define COMUNICATE_GET_RET_DATA       0x05 /*5��ʾI2C�������ڶ����ݣ��������������*/
#define COMUNICATE_COM_CONT           0x06 /*6��ʾI2C�ӻ�����д����Ĳ���������*/
#define COMUNICATE_WAIT_ACK           0x07 /*7��ʾI2C�ӻ�Ӧ�����ݺ�ȴ�����ACK*/

//uart bus finite state machine
#define UART_IDLE                    0x00 /*0��ʾ����״̬*/

#define UART_RECEIVE                 0x01 /*5��ʾ���ڶ����ݣ��������������*/

#define UART_SEND                    0x02 /*2��ʾ����״̬*/

#define UART_BUSY                    0x03 /*��ʾ������յ�������*/

///////////////////////////////////////////////////////////////////////////////////
//typedef volatile unsigned int* REG32;
//typedef volatile unsigned short* REG16;
//typedef volatile unsigned char* REG8;

typedef enum {
    SEND_CMD_DATA,
    SEND_CMD_START,
    SEND_CMD_END,
    SEND_VALID_FLAG,
    SEND_INVALID_FLAG
} send_packet_e;

typedef enum {
    SEND_STATUS_START,
    SEND_STATUS_CONT,
    SEND_STATUS_FLAG,
    SEND_STATUS_END,
    SEND_STATUS_MAX
} send_status_e;

//cmd status
typedef enum {
    RECEIVE_STATUS_IDLE,
    RECEIVE_STATUS_START
    //RECEIVE_STATUS_CONT,
    //RECEIVE_STATUS_END
} receive_status_e;

void uart1_int_process(void) __FAR__;
void uart1_init(void) __FAR__;

extern uint8 g_uart_rx_buf[PYH_WRBUF_SIZE]; //uart��������ͨ��buffer
extern uint8 g_uart_tx_buf[PYH_WRBUF_SIZE]; //uart��������ͨ��buffer
extern uint8 g_receive_buf[K_VARBUF_SIZE];
extern uint8 g_transmit_buf[MAX_SEGMENT_LEN]; //Ӧ������buffer
extern uint8 *g_read_ptr;//��ָ��
extern uint8 *g_write_ptr;//дָ��

extern uint16 long_answer_len; //Ӧ�𲻶�����������
extern bool long_answer_reply; //����Ӧ�𲻶�������
extern uint8 *pmasterdata; //���������������ָ��
extern uint8 *pmasterdata_head; //��������ָ��ͷ���������շ���RCP���󴫲�
extern uint16 long_command_len; //�������������������
extern bool long_command_recv; //���ڽ��ճ������������
//extern spp_command_t cur_spp_cmd; //��ǰ���յ���������
//extern spp_command_t reply_spp_cmd; //Ӧ��������

extern uint16 g_rw_index; //��д�������
extern uint8 g_bus_status; //�������ͣ���Ϊд��������ݣ�������״̬���Ϳ���

extern send_status_e g_cmd_status; //�����״̬������״̬��æ״̬����Ӧ��ɹ�״̬����Ӧ��ɹ�״̬��

extern uint8 g_retry_times; //�ش�����
extern uint8 g_send_error;//�����־

extern uint16 g_send_data_len;

extern uint8 g_send_cont;

extern void uart1_transmit_deal(send_packet_e mode, uint8 send_len) __FAR__;

extern void store_command_data(void) __FAR__;

extern void load_answer_data(void) __FAR__;

extern uint8 get_communicate_status(void) __FAR__;

extern void set_communicate_status(uint8 status) __FAR__;

extern uint8 get_send_status(void) __FAR__;

extern void set_send_status(uint8 status) __FAR__;

#endif //_KEY_UART_H_
