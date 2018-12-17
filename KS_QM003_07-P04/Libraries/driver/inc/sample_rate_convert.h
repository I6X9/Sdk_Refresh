#ifndef __SAMPLE_RATE_CONVERT_H__
#define __SAMPLE_RATE_CONVERT_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "type.h"
    
    
/***************************************************************
            �������������������в�����ת44.1KHZʹ��
***************************************************************/    
    
//������ת��ģ���ʼ��
//SrcIndex:      �ṩ1��������ת��ʵ�壬���Ϊ0
//InSampleRate:  ������ת��ǰ�Ĳ����ʣ�9�ֲ�����
//OutSampleRate: ������ת����Ĳ����ʣ�Ŀǰֻ����44100
//ChannelNum:	 ����������1 or 2
bool SampleRateConvertInit(uint8_t SrcIndex, uint16_t InSampleRate, uint16_t OutSampleRate, uint8_t ChannelNum);   

////������ת��������
////SrcIndex:      ��1��������ת��ʵ�壬���Ϊ0
////InBuf:         ������ת��ǰ��PCM����
////OutBuf:        ������ת�����PCM����
////SampleCnt:	   ������ת��ǰ�Ĳ�������
////return:        ת�����OutBuf����������Ч��������
uint16_t SampleRateConvert(uint8_t SrcIndex, int16_t* InBuf, int16_t* OutBuf, uint16_t SampleCnt);


/***************************************************************
            ���º����������в�������4����
***************************************************************/ 

//��������4����ʼ��
//W_Addr: ����������ʱ��Ҫ�õ���FIR�˲��������ַ
//w_Sample: FIR�˲��������С����λΪ��Sample
//Channel: ͨ���ţ�1 -- �������� 2 -- ˫������Ŀǰ��֧�ֵ�����
void SampleRateUpSampleby4Init(int16_t *W_Addr, int32_t W_Sample, int16_t Channel);

//��������4��Ŀǰֻ֧�ֵ�����
//BufIn -- �����ַ
//BufOut -- �����ַ
//n -- ����Ĳ����㣬ע��n * 4 + 30������W_Sample
//����ֵ -- >=0����Ĳ�������, -1 �����������
int32_t SampleRateUpSampleby4(int16_t *BufIn, int16_t *BufOut, int32_t n);
    
#ifdef  __cplusplus
}
#endif//__cplusplus

#endif   //
