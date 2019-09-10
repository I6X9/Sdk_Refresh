#ifndef __AI_H__
#define __AI_H__

#include <string.h>
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

//AI��ʼ��
void ai_init(void);

//AI���ݱ���  buf����Ϊ������Ϊ44100��PCM����
void ai_audio_encode(int16_t* buf,uint32_t sample,uint8_t ch);

//��������������ʽ����
void ai_start(void);


//BLE���պ�����ݴ���
void ble_rcv_data_proess(uint8_t *p,uint32_t size);

//spp���պ�����ݴ���
void spp_rcv_data_proess(uint8_t *p,uint32_t size);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif //__ADC_KEY_H__
