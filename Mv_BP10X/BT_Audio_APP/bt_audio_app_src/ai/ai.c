#include "type.h"
#include "ble_api.h"
//#include "ble_app_example.h"
#include "bt_app_func.h"
#include "bt_spp_api.h"


#include "debug.h"


#include "resampler.h"
#include "mcu_circular_buf.h"
#include "speex.h"
#include "resampler.h"
#include <nds32_intrinsic.h>
#include "audio_adc.h"
#include "rtos_api.h"

#include "app_config.h"
#include "bt_manager.h"

#ifdef CFG_FUNC_AI

extern void ai_resample_init(void);
extern uint16_t ai_resample_applay(int16_t* InBuf, int16_t* OutBuf, uint16_t SampleCnt);
extern void MD5_String(char *data, int len, char md5_text[32]);



static MCU_CIRCULAR_CONTEXT AI_CircularBuf;
static void* pSpeexEncoder;
static SpeexBits pSpeexBits;
static uint32_t ai_frame_size = 200;
static char send_data[48];//����CMD����
static int send_size = 0;//�������ݵĳ���
static uint32_t last_length = 0;//8K��PCM�������ĸ���
static uint8_t xq_pcmbuf[10*20];//speex���buffer 10֡SPEEXdata
static osMutexId AI_Mutex = NULL;
static int16_t pcm_8k_buf[86*2];//@48000max:512sample--->@8000max86sample
static int16_t pcm_8k_fifo[160*2];//8000��PCM������ max:2*ai_frame_size
static char speex_data[20]={0};//speex �������
static int send_error = 0;
static int start_speex = 0;
static char sp_data[20];
const char *APP_ID = "";
const char *SECRET = "";
int16_t ai_pcm_data[256];
uint32_t ai_start_f = 0;
extern uint8_t BleConnectFlag;


void app_send_authorize_rsp(unsigned char key1, unsigned char key2,unsigned char flage)
{
    char input_data[16];
	int i;
	send_data[0] = 0xA5;
    send_data[1] = 0x11;
    for(i=0; i<16; i=i+2) {
        input_data[i]   = SECRET[i] ^ key1;
        input_data[i+1] = SECRET[i+1] ^ key2;
    }
	MD5_String(input_data,16,(char*)send_data+2);
    memcpy(send_data+34, APP_ID, 12);
    send_data[46] = flage;
    send_data[47] = 0;
    for(i=0; i<47; i++) {
        send_data[47] += send_data[i];
    }
}

char crc_get_data(char *p,uint32_t size)
{
	int i;
	char crc=0;
    for(i=0; i<size; i++) {
    	crc += p[i];
    }
    return crc;
}

void app_rcv_data_proess(uint8_t *packet, uint16_t size, uint8_t flage)
{
	memset(send_data,0,48);
	if( (packet[0] == 0xAA) && (packet[1] == 0x10) && (packet[2] == 0x01) && (packet[3] == 0x01) )
	{
		send_data[0] = 0xA5;send_data[1] = 0x10;send_data[2] = 0x5B;send_data[3] = 0x5A;send_data[4] = 0x6A;
		send_data[4] = crc_get_data(send_data,4);
		send_size = 5;
		send_error = 0;
		start_speex = 0;
	}
	else if((packet[0] == 0xAA) && (packet[1] == 0x11))
	{
		app_send_authorize_rsp(packet[2],packet[3],flage);
		send_size = 48;
		start_speex = 0;
	}
	else if((packet[0] == 0xAA) && (packet[1] == 0x22) && (packet[2] == 0x01) && (packet[3] == 0x01))
	{
		memcpy(send_data,packet,4);
		send_data[0] = 0xA5;
		send_data[4] = crc_get_data(send_data,4);
		send_size = 5;
		start_speex = 0xFFFF;
		DBG("��ʼ��������\n");
	}
	else if((packet[0] == 0xAA) && (packet[1] == 0x22) && (packet[2] == 0x02) && (packet[3] == 0x01))
	{
		//mv_speex_deinit();
		DBG("ֹͣ��������\n");
		memcpy(send_data,packet,4);
		send_data[0] = 0xA5;
		send_data[4] = crc_get_data(send_data,4);
		send_size = 5;
		start_speex = 0;
	}
}


uint32_t mv_speex_get_len(void)
{
	uint32_t TEMP;
#ifdef FUNC_OS_EN
	osMutexLock(AI_Mutex);
#endif
	TEMP = MCUCircular_GetDataLen(&AI_CircularBuf);
#ifdef FUNC_OS_EN
	osMutexUnlock(AI_Mutex);
#endif
	return TEMP;
}

void mv_speex_get(uint8_t* buf,uint32_t size)
{
#ifdef FUNC_OS_EN
	osMutexLock(AI_Mutex);
#endif
	MCUCircular_GetData(&AI_CircularBuf,buf,size);
#ifdef FUNC_OS_EN
	osMutexUnlock(AI_Mutex);
#endif
}

int ble_send_packet(uint8_t res_handle,uint8_t data_handle,uint8_t *buf, uint32_t size)
{
	int ret = 0;
	if(start_speex == 0)
	{
		ret = att_server_notify(res_handle, buf, size);
	}
	else
	{
		ret = att_server_notify(data_handle, buf, size);
	}
	return ret;
}

//ble�������ݴ���
void ble_send_data(uint8_t res_handle,uint8_t data_handle)
{
	if(send_size == 0)
	{
		return;
	}
	if(send_error)
	{
		if(ble_send_packet(res_handle,data_handle,send_data,send_size) == 0)
		{
			send_error = 0;
		}
		return;
	}
	if(start_speex == 0xFFFF)//SPEEX start data
	{
		if(mv_speex_get_len() >= 20)
		{
			mv_speex_get(send_data,20);
			send_size = 20;
		}
		else
		{
			return ;
		}
	}
	if(ble_send_packet(res_handle,data_handle,send_data,send_size) == 0)
	{
		if(start_speex == 0)
		{
			send_size = 0;
		}
	}
	else
	{
		send_error = 1;
	}
}


//BLE���պ�����ݴ���
void ble_rcv_data_proess(uint8_t *p,uint32_t size)
{
	app_rcv_data_proess(p,size,2);
}


//spp���պ�����ݴ���
void spp_rcv_data_proess(uint8_t *p,uint32_t size)
{
	app_rcv_data_proess(p,size,1);
}


//spp�������ݴ���
void ai_spp_run_loop(void)
{
	if(start_speex == 0xFFFF)
	{
		if(mv_speex_get_len() >= 20)
		{
			mv_speex_get(sp_data,20);
			send_data[0] = 0xA5;send_data[1] = 0x21;send_data[2] = 0x00; send_data[3] = 20;send_data[4] = 0xEE;
			send_size = 5;
		}
		else
		{
			return ;
		}
	}
	else if(start_speex == 0xEEEE)
	{
		memcpy(send_data,sp_data,20);
		send_size = 20;
	}
	if(send_size == 0)
	{
		return;
	}
	SppSendData(send_data,send_size);
	if(start_speex == 0xFFFF)
	{
		start_speex = 0xEEEE;
	}
	else if(start_speex == 0xEEEE)
	{
		start_speex = 0xFFFF;
	}
	else
	{
		send_size = 0;
	}
}


//AI����ѭ��loop������������task�����
void ai_run_loop(void)
{
	if(ai_start_f)
	{
		ai_start_f = 0;
		send_data[0] = 0xA5;send_data[1] = 0x22;send_data[2] = 0x01;send_data[3] = 0x01;send_data[4] = 0xC9;
		send_size = 5;
	}
	if(GetBtManager()->sppState == BT_SPP_STATE_CONNECTED)
	{
		if(SppIsReady())
		{
			ai_spp_run_loop();
		}
	}
	else
	{
		if(BleConnectFlag)
		{
			ai_ble_run_loop();
		}
	}
}


//AI������س�ʼ��
void ai_init(void)
{
	uint32_t     SpeexParamQuality  = 3;
	speex_bits_init(&pSpeexBits);
	pSpeexEncoder = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
	speex_encoder_ctl(pSpeexEncoder, SPEEX_SET_QUALITY,     &SpeexParamQuality);
	speex_decoder_ctl(pSpeexEncoder, SPEEX_GET_FRAME_SIZE, &ai_frame_size);
	ai_resample_init();
	last_length = 0;
	send_error = 0;
	start_speex = 0;
#ifdef FUNC_OS_EN
	if(AI_Mutex == NULL)
	{
		AI_Mutex = xSemaphoreCreateMutex();
	}
#endif
#ifdef FUNC_OS_EN
	osMutexLock(AI_Mutex);
#endif
	MCUCircular_Config(&AI_CircularBuf,xq_pcmbuf,sizeof(xq_pcmbuf));
#ifdef FUNC_OS_EN
	osMutexUnlock(AI_Mutex);
#endif
}




//ѭ��loop�е��� audio coreһֱ����
void ai_audio_encode(int16_t* buf,uint32_t sample,uint8_t ch)
{
	uint32_t len = 0;
	int i;
	int ii;
	for(ii=0;ii<sample/128;ii++)
	{
		if(ch == 2)
		{
			for(i=0;i<128;i++)
			{
				ai_pcm_data[i] = buf[2*i + 256*ii];
			}
		}
		else
		{
			memcpy(ai_pcm_data,&buf[128*ii],128*2);
		}
		len = ai_resample_applay(ai_pcm_data,pcm_8k_buf,128);
		for(i=0;i<len;i++)
		{
			pcm_8k_buf[i] = __nds32__clips((pcm_8k_buf[i]*10),16-1);//�Ŵ�10��
		}
		len = len*2;
		uint8_t *p_fifo = (uint8_t *)pcm_8k_fifo;
		uint8_t *p_buffer = (uint8_t *)pcm_8k_buf;
		memcpy(p_fifo+last_length,p_buffer,len);
		last_length = last_length + len;
		if(last_length >= ai_frame_size*2)
		{
			speex_bits_reset(&pSpeexBits);
			speex_encode_int(pSpeexEncoder, (spx_int16_t*)pcm_8k_fifo, &pSpeexBits);
			len = speex_bits_write(&pSpeexBits, (char*)(speex_data), 20);
			memcpy(p_fifo,p_fifo+ai_frame_size*2,last_length - ai_frame_size*2);
			last_length = last_length - ai_frame_size*2;
	#ifdef FUNC_OS_EN
		osMutexLock(AI_Mutex);
	#endif
			MCUCircular_PutData(&AI_CircularBuf,speex_data,len);
	#ifdef FUNC_OS_EN
		osMutexUnlock(AI_Mutex);
	#endif
		}
	}
}


//��������������ʽ����
void ai_start(void)
{
	ai_start_f = 1;
}
#endif
