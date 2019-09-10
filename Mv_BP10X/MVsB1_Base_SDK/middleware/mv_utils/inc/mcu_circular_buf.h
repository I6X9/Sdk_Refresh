 /**
 **************************************************************************************
 * @file    mcu_circular_buf.h
 * @brief   MCU management cycle buf
 *
 * @author  Sam
 * @version V1.1.0
 *
 * $Created: 2015-11-02 15:56:11$
 *
 * &copy; Shanghai Mountain View Silicon Technology Co.,Ltd. All rights reserved.
 **************************************************************************************
 */
#ifndef		__MCU_CIRCULAR_BUF_H__
#define		__MCU_CIRCULAR_BUF_H__

/**
 * @addtogroup mv_utils
 * @{
 * @defgroup MCUCricularBuf MCUCricularBuf.h
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef struct __MCU_CIRCULAR_CONTEXT__
{
    uint32_t    R;              //ѭ��buf��ָ��,byte��
    uint32_t    W;              //ѭ��bufдָ��,byte��
    uint32_t    BufDepth;       //ѭ������,byte��  
    int8_t*     CircularBuf;    //ѭ��bufָ��
} MCU_CIRCULAR_CONTEXT;

extern MCU_CIRCULAR_CONTEXT OTGCircularBuf;

/*
 * ���ѭ��fifo����
 */
void MCUCircular_Config(MCU_CIRCULAR_CONTEXT* CircularBuf, void* Buf, uint32_t Len);

/*
 * ��ȡѭ��fifoʣ��ռ䳤�ȣ���λByte
 * ע�⣺��дָ���غϣ�ֻ������MCU ѭ��fifo ��ʼ�����߲��յ�ʱ��
 * ϵͳ��fifo��д������Ҫ����ˮλ����ֹ�����������
 */
int32_t MCUCircular_GetSpaceLen(MCU_CIRCULAR_CONTEXT* CircularBuf);

/*
 * ��ѭ��fifo�д������
 * ע�⣺��дָ���غϣ�ֻ������MCU ѭ��fifo ��ʼ�����߲��յ�ʱ��
 * ϵͳ��fifo��д������Ҫ����ˮλ����ֹ�����������
 */
void MCUCircular_PutData(MCU_CIRCULAR_CONTEXT* CircularBuf, void* InBuf, uint16_t Len);

/*
 * ��ѭ��fifo�ж�����
 */
int32_t MCUCircular_GetData(MCU_CIRCULAR_CONTEXT* CircularBuf, void* OutBuf, uint16_t MaxLen);

/*
 * ��ȡѭ��fifo��Ч���ݳ��ȣ���λByte
 */
uint16_t MCUCircular_GetDataLen(MCU_CIRCULAR_CONTEXT* CircularBuf);

#ifdef __cplusplus
}
#endif//__cplusplus

/**
 * @}
 * @}
 */
#endif
//
