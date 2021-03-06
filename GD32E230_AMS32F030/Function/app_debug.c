/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : App_Debug.c
  版 本 号   : 初稿
  作    者   : qing
  生成日期   : 2019年5月9日
  最近修改   :
  功能描述   : 串口打印实现模块
  函数列表   :
              GetDebugBufferData
              GetDebugDataLen
              SendData2DebugBuf
  修改历史   :
  1.日    期   : 2019年5月9日
    作    者   : qing
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "app_config.h"
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define DEBUG_BUFFER_DATA_LEN			0xff

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
static uint8_t DebugBuffer[DEBUG_BUFFER_DATA_LEN];
static uint8_t DebugBufHead= 0,DebugBufTail = 0;
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/


/*********************************************************************************/
//GetDebugDataLen                                                            /
//EntryParameter:none                                                          /
//ReturnValue:获取缓冲区数据长度。                                                                /
/*********************************************************************************/
uint16_t GetDebugDataLen(void)
{
	return (DebugBufTail+DEBUG_BUFFER_DATA_LEN-DebugBufHead)%DEBUG_BUFFER_DATA_LEN;
}

/*********************************************************************************/
//GetDebugbufferData                                                              /
//EntryParameter:none                                                          /
//ReturnValue:打印缓冲区中最前面的一个字节。                                                                /
/*********************************************************************************/
void SendData2DebugBuf(uint8_t data)
{
	DebugBuffer[DebugBufTail++] = data;
	if(DebugBufTail > (DEBUG_BUFFER_DATA_LEN-1))
	{
		DebugBufTail = 0;
	}
}
/*********************************************************************************/
//GetDebugbufferData                                                              /
//EntryParameter:none                                                          /
//ReturnValue:打印缓冲区中最前面的一个字节。                                                                /
/*********************************************************************************/
uint8_t GetDebugBufferData(void)
{
	uint8_t Data;

	Data = DebugBuffer[DebugBufHead++];
	if(DebugBufHead > (DEBUG_BUFFER_DATA_LEN-1))
	{
		DebugBufHead = 0;
	}
	return Data;
}

/*****************************************************************************
 函 数 名  : fputc
 功能描述  : 打印输出重定向到USART TX
 输入参数  : int ch   
             FILE *f  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年4月15日
    作    者   : qing
    修改内容   : 新生成函数

*****************************************************************************/
int fputc(int ch, FILE *f)
{
    if (IS_DEBUG_BUSY(ch)) {
    	SendData2DebugBuf((uint8_t)ch);
    }
    return ch;
}


