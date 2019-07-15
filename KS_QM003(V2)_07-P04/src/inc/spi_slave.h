/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : spi_slave.h
  �� �� ��   : ����
  ��    ��   : qing
  ��������   : 2019��7��10��
  ����޸�   :
  ��������   : spi_slave.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2019��7��10��
    ��    ��   : qing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define		MAS_BOT_DON					0x01	//spi���������ɹ�
#define		MAS_SET_RBT					0x02	//SPI����������
#define		MAS_CLR_RBT					0x03	//SPI �����ָ�������


#ifndef __SPI_SLAVE_H__
#define __SPI_SLAVE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum _SPIM_CMD
{
	SPIM_UNKOWN = 0,
	SPIM_READ,//param length
	SPIM_WRITE//Param length	
}SPIM_CMD;

typedef enum _SPIS_RESPOND
{
	SPIS_UNKONW = 0,
	SPIS_NO_DATA,//not enough data to be sent.	//param  actually data length
	SPIS_NO_ROOM,//not enouth room to receive data	//param  actually data room
	SPIS_OK
}SPIS_RESPOND;


typedef struct _SPICmdContext
{
	uint8_t  SyncWord;
	uint8_t  Message;
	uint16_t Param;
	uint16_t CrcValue;
}SpiMessContext;


#define SPI_SLAVE_SPIS_INIT()					do {\
												GpioSetRegOneBit(SPI_SLAVE_CS_PORT_PU, SPI_SLAVE_CS_BIT_MASK);\
												GpioClrRegOneBit(SPI_SLAVE_CS_PORT_PD, SPI_SLAVE_CS_BIT_MASK);\
												GpioClrRegOneBit(SPI_SLAVE_CS_PORT_IE, SPI_SLAVE_CS_BIT_MASK);\
												GpioSetRegOneBit(SPI_SLAVE_CS_PORT_OE, SPI_SLAVE_CS_BIT_MASK);\
												GpioSetRegOneBit(SPI_SLAVE_CS_PORT_OUT, SPI_SLAVE_CS_BIT_MASK);\
												} while (0)

#define SPI_SLAVE_SPIS_ENABLE()					GpioClrRegOneBit(SPI_SLAVE_CS_PORT_OUT, SPI_SLAVE_CS_BIT_MASK)

#define SPI_SLAVE_SPIS_DISABLE()				GpioSetRegOneBit(SPI_SLAVE_CS_PORT_OUT, SPI_SLAVE_CS_BIT_MASK)

extern void Spi_SlaveInit(void);
extern void Spi_SendCmdToSlave(uint8_t cmd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SPI_SLAVE_H__ */
