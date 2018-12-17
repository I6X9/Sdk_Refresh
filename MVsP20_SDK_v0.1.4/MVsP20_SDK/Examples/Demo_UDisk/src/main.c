#include <stdio.h>
#include <nds32_intrinsic.h>
#include "cache_tcm.h"
#include "gpio.h"
#include "uarts.h"
#include "watchdog.h"
#include "get_bootup_info.h"
#include "uarts_interface.h"
#include "debug.h"
#include "irqn.h"
#include "spi_flash.h"
#include "timeout.h"
#include "clk.h"
#include "type.h"
#include "delay.h"
#include "string.h"
#include "otg_detect.h"
#include "timer.h"

#define UPDATE_ADDR 0X100000//˫BANK�����ļ����д��λ��


void Timer_OTG(void)
{
	static uint16_t count=0;
	OTG_PortLinkCheck();
	count++;
/*	if(count>3000)
	{
		count=0;
		if(OTG_PortHostIsLink())
		{
			DBG("U��������\n");
		}
		else
		{
			DBG("U���Ѱγ�\n");
		}
	}*/
}

__attribute__((section(".driver.isr"))) void Timer4Interrupt(void)
{
	Timer_OTG();
	Timer_InterruptFlagClear(TIMER4, UPDATE_INTERRUPT_SRC);
}



#include "ff.h"
FATFS myfs[2]; /* �߼��������Ĺ�����(�ļ�ϵͳ����) */
FIL fsrc, fdst; /* �ļ����� */
BYTE buffer[40960]={"0123456789;"}; /* �ļ����������� */

uint8_t FilseCopy(uint8_t * srcPath,uint8_t *destPath)//�ļ�����
{
	FRESULT res; /* FatFs ��������������� */
	UINT br, bw; /* �ļ���/д�ֽڼ��� */
	FIL fsrc, fdst; /* �ļ����� */

	QWORD curFsize = 0;//�Ѿ����ļ���С
	UINT tmpPercent,prePercent=0;

	/* �������� 0 �ϵ�Դ�ļ� */
	res = f_open(&fsrc, (const char *)srcPath, FA_OPEN_ALWAYS   | FA_WRITE | FA_READ);
	if (res != FR_OK)
	{
		DBG("f_open  %s err!\n",srcPath);
		f_close(&fsrc);
		return res;
	}
	/* �������� 1 �ϴ���Ŀ���ļ� */
	res = f_open(&fdst,(const char *)destPath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (res != FR_OK)
	{
		DBG("f_open  %s err!\n",destPath);
		f_close(&fdst);
		return res;
	}

	DBG("File size = %dKB,[0%%",(int)(f_size(&fsrc)/1000));
	while(1)
	{
		res = f_read(&fsrc, buffer, sizeof(buffer), &br);
		if (res || br == 0)
		{
			break; /* �ļ��������� */
		}
		else
		{
//			DBG("READ:%s\n",buffer);
//			DBG("f_read OK\n");
		}

		res = f_write(&fdst, buffer, br, &bw);
		if (res || bw < br)
		{
			DBG("f_write err\n");
			break; /* ���������� */
		}
		else
		{
			//DBG("f_write OK\n");
		}

		//����Ϊ��ʾ������
		curFsize += br;
		tmpPercent = (10*curFsize)/f_size(&fsrc);//�����Ѿ����ļ��İٷֱ�
		if(tmpPercent > prePercent)
		{
			prePercent = tmpPercent;
			DBG("..%d%%",tmpPercent*10);//��ʾ��ǰ�ٷֱ�,ֻ��ʾ10%��20%��
		}
	}
	f_close(&fsrc);
	f_close(&fdst);
	DBG("]\n");
   return FR_OK;
}






//�����ļ�������
//path:·��
//����ֵ:ִ�н��
uint8_t SD_USB_Copy(uint8_t * srcPath,uint8_t *destPath)
{
	FRESULT res;
	FILINFO fileinfo;
	DIR dir;
	uint16_t np,desNP;

	res = f_opendir(&dir,(const TCHAR*)srcPath); //��һ��Ŀ¼
	if (res == FR_OK)
	{
		np = strlen((const char *)srcPath);
		desNP = strlen((const char *)destPath);
		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
			//if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
			DBG("%s/%s\n", srcPath,fileinfo.fname);//��ӡ·�����ļ�(��)��

			if(fileinfo.fattrib & AM_DIR)//Ŀ¼
			{
                *(srcPath + np) = '/';
                strcpy((char *)(srcPath + np + 1), &fileinfo.fname[0]);
                *(destPath + desNP) = '/';
                strcpy((char *)(destPath + desNP + 1), &fileinfo.fname[0]);

				res=f_mkdir((const char *)destPath);
				if (res == FR_OK)
				{
					DBG("f_mkdir ok:%s!\n",destPath);
				}
				else//�ļ����Ѵ��ڻ򴴽��ļ���ʧ��
				{

				}

                res = SD_USB_Copy(srcPath,destPath);//�ݹ�����
                *(srcPath + np) = '\0';
                *(destPath + desNP) = '\0';
                if(res != FR_OK) break;
			}
			else//�ļ�
			{
                *(srcPath + np) = '/';
                strcpy((char *)(srcPath + np + 1), &fileinfo.fname[0]);//���ļ�����ӵ�·��
                *(destPath + desNP) = '/';
                strcpy((char *)(destPath + desNP + 1), &fileinfo.fname[0]);

//                char* pExt = strrchr((const char *)srcPath,'.');//��ȡ��׺
//                if(strcasecmp(pExt,".mp3") == 0
//                  || strcasecmp(pExt,".doc") == 0
//				  || strcasecmp(pExt,".docx") == 0
//				  || strcasecmp(pExt,".zip") == 0
//				  || strcasecmp(pExt,".rar") == 0)//��׺���жϣ����Դ�Сд��,ȫ�̿���ʱ�����Խ��˴�ע�͵�
                {
					uint32_t t0,t1;

					t0 = GetSysTick1MsCnt();
					res = FilseCopy(srcPath,destPath);//�����ļ�
					t1 = GetSysTick1MsCnt();//���㿽���ļ�����ʱ�䣬��λms

					*(srcPath + np) = '\0';
					*(destPath + desNP) = '\0';

					if(res != FR_OK)
					{
						DBG("Copy File err\n");
						break;
					}
					else
					{
						DBG("Copy File ok,time = %dms\n",(int)(t1-t0));
					}
                }

			}
		}
	}
	f_closedir(&dir);

	return res;
}


void fatfs_test (void)//FATFS�ļ�ϵͳʵ�� SD����U�̻�������
{

	//vTaskDelay(100);//�ϵ���ʱ������USB��ʼ������ʧ��

	/* Ϊ�߼�������ע�Ṥ���� */
#define   USB_PATH          "USB:"
#define   SD_PATH           "SD:"

	f_mount( &myfs[0],USB_PATH,1);//����
	f_mount( &myfs[1],SD_PATH,1);


	uint8_t *psrcPath = malloc(2048);//��̬�����ڴ棬���ڱ���·��
	uint8_t *pdestPath = malloc(2048);

	memset(psrcPath,0,2048);
	memset(pdestPath,0,2048);
	strcpy((char *)psrcPath,(const char *)USB_PATH);//����Դ��ַ
	strcpy((char *)pdestPath,(const char *)SD_PATH);//����Ŀ�ĵ�ַ

	SD_USB_Copy(psrcPath,pdestPath);//SD����USB����,�õĵݹ鷨Ҫע��ջ���

	free(psrcPath);
	free(pdestPath);

	f_mount(NULL,SD_PATH,1);
	f_mount(NULL,USB_PATH,1);


	DBG("END\n");
}

void UDisk_Upgrade(uint8_t * MvaPath)
{

	FRESULT res;
	FILINFO fileinfo;
	DIR dir;
	FIL fsrc;
	UINT br;
	uint32_t np,len;
	uint8_t read_buf[4096];
	uint8_t *Path = malloc(2048);
	strcpy((char *)Path,(const char *)MvaPath);
	np = strlen((const char *)Path);
	res = f_opendir(&dir,(const TCHAR*)Path); //��һ��Ŀ¼
	if (res == FR_OK)
	{
		while(1)
		{
			res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
			if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
			DBG("%s/%s\n",Path,fileinfo.fname);				//��ӡ·�����ļ�(��)��
			if((fileinfo.fattrib & AM_DIR)==0x00)				//ֻУ���ļ����ļ���ֱ�Ӻ��ԣ�
			{
				char* pExt = strrchr((const char *)fileinfo.fname,'.');//��ȡ��׺
				 if(strcasecmp(pExt,".MVA") == 0)
				 {
					 *(Path + np) = '/';
					 strcpy((char *)(Path + np + 1), &fileinfo.fname[0]);//���ļ�����ӵ�·��
					 res = f_open(&fsrc, (const char *)Path, FA_OPEN_ALWAYS | FA_READ);
					 if(res == FR_OK)
					 {
						 DBG("���ļ�%s�ɹ�\n",fileinfo.fname);
						 len = fileinfo.fsize;
						 uint32_t i=0;
						 for(i=0;i<(fileinfo.fsize/4096);i++)
						 {
							 f_read(&fsrc,read_buf,4096,&br);//ÿ�ζ�ȡ4K����
							 SpiFlashErase(SECTOR_ERASE,UPDATE_ADDR/0x1000+i,0);
							 SpiFlashWrite(UPDATE_ADDR+i*4096,read_buf,4096,10);
						 }
						 if((fileinfo.fsize%4096)!=0)
						 {
							 res = f_read(&fsrc,read_buf, fileinfo.fsize%4096, &br);
							 SpiFlashErase(SECTOR_ERASE,UPDATE_ADDR/0x1000+i,0);
							 SpiFlashWrite(UPDATE_ADDR+i*4096,read_buf,fileinfo.fsize%4096,10);
						 }
						 DBG("MVA�ļ�д��OK��\n");
						 DBG("˫BANK��������λ������\n");
						 *(volatile uint32_t *)0x4002E800 = 0xFF;
						 *(volatile uint32_t *)0x4002E824 = UPDATE_ADDR;
						 *(volatile uint32_t *)0x4002E800 = 0;
						 *(volatile uint32_t *)0x40022008 = 1;
						 *(volatile uint32_t *)0x40022008 = 0;
					 }
					 else
					 {
						 DBG("���ļ�%sʧ��\n",fileinfo.fname);
					 }
					 f_close(&fsrc);//�ر��ļ�
				 }
			}
		}
	}
	else
	{
		DBG("��Ŀ¼%sʧ��\n",MvaPath);
	}
	f_closedir(&dir);
	return res;
}



const char * fileData = "U�̶�д�ļ�����:��U�̸�Ŀ¼�´���Udisk.txt�ļ�����д�뱾������;";
uint8_t UDisk_FileTest(uint8_t * srcPath)
{
	FRESULT res; /* FatFs ��������������� */
	UINT br, bw; /* �ļ���/д�ֽڼ��� */
	FIL fsrc; /* �ļ����� */

	/* ������  �ϴ���Դ�ļ� */
	res = f_open(&fsrc, (const char *)srcPath, FA_CREATE_ALWAYS   | FA_WRITE | FA_READ);
	if (res != FR_OK)
	{
		DBG("f_open  %s err!\n",srcPath);
		f_close(&fsrc);
		return res;
	}

	do
	{
		/****д�ļ�**/
		br = strlen(fileData);
		res = f_write(&fsrc, fileData, br, &bw);
		if (res || bw < br)
		{
			DBG("f_write err\n");/* ���������� */
			break;
		}
		else
		{
			DBG("f_write OK\n");
		}

		/****���ļ�**/
		f_lseek(&fsrc,0);//ָ���ļ�ͷ

		res = f_read(&fsrc, buffer, sizeof(buffer), &br);
		if (res || br == 0)
		{
			DBG("READ ERR\n");
			break; /* �ļ��������� */
		}
		else
		{
			DBG("Read: %s\n",buffer);//�ļ����ݴ�ӡ
			DBG("f_read OK\n");
		}
	}
	while(0);
	f_close(&fsrc);

   return FR_OK;
}

/*
void fatfs_USB (void)//FATFS�ļ�ϵͳʵ�� U�̲���
{
#define   USB_PATH          "USB:"

	f_mount( &myfs[0],USB_PATH,1);//����


	UDisk_FileTest("USB:Udisk.txt");

	f_mount(NULL,USB_PATH,1);

	DBG("END\n");
}
*/

void UDisk_Fatfs_Test()
{
	if(OTG_PortHostIsLink())
	{
		DBG("U��������\n");
#ifndef USB_PATH
	#define   USB_PATH          "USB:"
#endif
	FATFS fs;
	f_mount( myfs,USB_PATH,1);//�����ļ�ϵͳ
	UDisk_FileTest("USB:Udisk.txt");//���U�����Ƿ����Udisk.txt�ļ������ڸ��ļ�����ӡ�ļ����ݷ��򴴽����ļ���д������
	UDisk_Upgrade(USB_PATH);//��U���в����Ƿ����MVA��ʽ�ļ����������ļ�����flash����˫BANK��������
	f_mount(NULL,USB_PATH,1);
	DBG("END\n");
	}
	else
	{
		DBG("U��δ����\n");
	}
}







void Udisk_Iint(void)
{
	OTG_PortSetDetectMode(1,1);
}


void TIMER4_INIT(void)
{
	NVIC_EnableIRQ(TMR4_IRQn);
	Timer_Config(TIMER4, 1000, 0);
	Timer_Start(TIMER4);
}


void InitZiArea(void)
{
	/* Use compiler builtin and memset */
	#define MEMSET(s, c, n) __builtin_memset ((s), (c), (n))

	extern char _end;
	extern char __bss_start;
	int size;

	/* Clear bss section */
	size = &_end - &__bss_start;
	MEMSET(&__bss_start, 0, size);
	return;
}

int main()
{
	WDG_Disable();

	CacheTcmInitFast(PHYMEM_16KBPM0 /*cache*/, PHYMEM_16KBPM1/*tcm r0*/, 0/*tcm r0 offset*/, 16/*tcm r0 size*/, PHYMEM_NONE/*tcm r1*/, 0/*tcm r1 offset*/, 0/*tcm r1 size*/);
	InitZiArea();

	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB30);
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB30);
	GPIO_RegOneBitClear(GPIO_B_PU, GPIOB31);
	GPIO_RegOneBitSet(GPIO_B_PD, GPIOB31);
	Clock_Config(1, 12000000);
	Clock_PllLock(480000);
	Clock_SysClkSelect(PLL_CLK_MODE);
	Clock_UARTClkSelect(1);
	Clock_ModuleEnable(ALL_MODULE_CLK_SWITCH);

	SpiFlashInit(80000000, MODE_4BIT, 0, 1);
	mem_init();
	GPIO_UartTxIoConfig(0, 1);
	GPIO_UartRxIoConfig(0, 1);
	DbgUartInit(0,  115200, 8,  0,  1);

	SysTickInit();
	GIE_ENABLE();

	NVIC_EnableIRQ(SWI_IRQn);

	DBG("****************************************************************\n");
	DBG("               MVsP2_UDiskUpgrade_Demo 2018 MVSilicon           \n");
	DBG("****************************************************************\n");
	TIMER4_INIT();
	Udisk_Iint();
	DelayMs(1000);
	//UDisk_Upgrade();
	//fatfs_USB();
	while(1)
	{
		UDisk_Fatfs_Test();
		DelayMs(1000);
	}

}

