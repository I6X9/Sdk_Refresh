#include "type.h"
#include "debug.h"
#include "spi_flash.h"

//LWIP ���
#include "sockets.h"
#include <net_mgr.h>
#include <lwip/api.h>
#include <host_apis.h>
#include "net_mgr.h"


#include "fsdata.h"//html/ͼƬ



const  unsigned char http_html_hdr[]="HTTP/1.1  200 OK\r\nContent-type: text/html\r\n\r\n";
const  unsigned char http_html_img[]="HTTP/1.1  200 OK\r\nContent-type: image/gif\r\n\r\n";

const unsigned char errhtml[] = "\
        <html>\
            <head>\
               <title>Error!</title>\
            </head>\
            <body>\
               <h1>404 - Page not found</h1>\
            </body>\
        </html>";





#define NON_NUM '0'
static char Char2Num(char ch)
{
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return NON_NUM;
}
/************************************************
* ���ַ�������URL���롣
* ���룺
* str: Ҫ������ַ���
* strSize: �ַ����ĳ��ȡ�
* result: ����������ĵ�ַ
* resultSize:�����ַ�Ļ�������С������<=strSize
* ����ֵ��
* >0: result��ʵ����Ч���ַ����ȣ�
* 0: ����ʧ�ܣ�ԭ���ǽ��������result�ĳ���̫С
************************************************/
static int URLDecode(const char* str, const int strSize, char* result, const int resultSize) //url����
{
	char ch, ch1, ch2;
	int i;
	int j = 0; /* for result index */

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0)) {
		return 0;
	}

	for (i=0; (i<strSize) && (j<resultSize); i++)
	{
		ch = str[i];
		switch (ch)
		{
			case '+':
			result[j++] = ' ';
			break;

			case '%':
			if (i+2 < strSize)
			{
				ch1 = Char2Num(str[i+1]);
				ch2 = Char2Num(str[i+2]);
				if ((ch1 != NON_NUM) && (ch2 != NON_NUM))
				{
					result[j++] = (char)((ch1<<4) | ch2);
					i += 2;
					break;
				}
			}
			/* goto default */
			default:
			result[j++] = ch;
			break;
		}
	}
	result[j] = 0;
	return j;
}
/**
  * @brief serve tcp connection
  * @param conn: connection socket
  * @retval None
  */
//unsigned char recv_buffer[1500];
//unsigned char content_buffer[512];

void http_server(int conn)
{
	int buflen = 1600;
	int ret;
	int tmp;
	uint8_t * content_buffer;
	uint8_t * recv_buffer = pvPortMalloc(buflen);

  	if(recv_buffer == NULL)
	{
		DBG("recv_buffer Malloc ERR\n");
		return;
	}
	memset(recv_buffer,0,buflen);
	/* Read in the request */
	ret = read(conn, recv_buffer, buflen);
	if(ret <= 0)
	{
		close(conn);
		vPortFree(recv_buffer);
		printf("read failed\r\n");
		return;
	}

    //printf("http server response!\r\n");
    if(strncmp((char *)recv_buffer, "GET /LED ", sizeof("GET /LED ")-1) == 0)//GET��ҳ�磺192.168.25.1/LED
    {
		write(conn, http_html_hdr, sizeof(http_html_hdr)-1);
        write(conn, file_index_html[0].data,file_index_html[0].len);
        printf("write:%d\n",(int)file_index_html[0].len);
    }
    else if(strncmp((char *)recv_buffer, "GET /img/led.gif", sizeof("GET /img/led.gif")-1) == 0)//GETͼƬ
	{
    	write(conn, http_html_img, sizeof(http_html_img)-1);//����֡ͷ

//		int len = 0;
//		tmp = file_img_led_gif[0].len;
//		while(tmp > 1024)
//		{
//			send(conn, file_img_led_gif[0].data + len, 1024,0x2000);//����ͼƬ���ְ�����
//			tmp -= 1024;
//			len += 1024;
//			vTaskDelay(2);//��ʱ����
//		}
//		if(tmp)//�������µ�����
//		{
//			send(conn, file_img_led_gif[0].data + len, tmp,0x2000);//����ͼƬ���µ�����
//		}


    	write(conn, file_img_led_gif[0].data, file_img_led_gif[0].len);//����ͼƬ

    	//printf("GET /img/led.gif OK!\n");
	}
    else if(strncmp((char *)recv_buffer, "GET /?Value", sizeof("GET /?Value")-1) == 0)//web �ύ����
    {
    	char* ptr;
    	printf("\n�ύ����:\n");

    	write(conn, http_html_hdr, sizeof(http_html_hdr)-1);


    	if((ptr = strstr((const char *)recv_buffer,"mode=")) != NULL)//���ַ�ʽ
		{
    		tmp = atoi(ptr+sizeof("mode=")-1);
    		printf("mode=%d\n",tmp);
		}
    	if((ptr = strstr((const char *)recv_buffer,"light=")) != NULL)//��������
		{
    		tmp = atoi(ptr+sizeof("light=")-1);
    		printf("light=%d\n",tmp);
		}
    	if((ptr = strstr((const char *)recv_buffer,"speed=")) != NULL)//�����ٶ�
		{
    		tmp = atoi(ptr+sizeof("speed=")-1);
    		printf("speed=%d\n",tmp);
		}
    	if((ptr = strstr((const char *)recv_buffer,"number=")) != NULL)//��Ļ����
		{
    		tmp = atoi(ptr+sizeof("number=")-1);
    		printf("number=%d\n",tmp);
    		if(tmp > 50)
    		{
    	    	write(conn, "no", sizeof("no")-1);//��ʾʧ��
    	    	goto __END;
    		}
		}
    	if((ptr = strstr((const char *)recv_buffer,"content=")) != NULL)//��ʾ����
		{
    		char* endPtr = ptr;
    		while((*endPtr) != ' ') endPtr++;

    		ptr = ptr+sizeof("content=")-1;//ָ��ָ��������ʼλ��
    		tmp = (uint32_t)(endPtr-ptr);
    		if(tmp > 600)//��ʾ���ݴ���200�ֽڣ�url����3���ֽ�Ϊһ��byte
    		{
    			printf("err:content bytes =%d \n",tmp/3);
    	    	write(conn, "no", sizeof("no")-1);////��ҳ��ʾ:�ύʧ��
    	    	goto __END;
    		}
    		else
    		{
    			content_buffer = pvPortMalloc(1024);
    			if(content_buffer == NULL)
    			{
    				DBG("content_buffer Malloc ERR\n");
    				goto __END;
    			}
    			memset(content_buffer,0,1024);
				URLDecode((const char *)ptr,tmp,(char*)content_buffer,1024);
				printf("content=%s\n",content_buffer);
				vPortFree(content_buffer);
    		}
		}

    	write(conn, "yes", sizeof("yes")-1);//��ҳ��ʾ:�ύ�ɹ�
    }
    else//������ʾ����
    {
        write(conn, errhtml, sizeof(errhtml)-1);
    }

  __END:

//    printf("\n\nrecv_buffer:%sd\n",recv_buffer);

    close(conn);  /* Close connection socket */
    vPortFree(recv_buffer);

}

/**
  * @brief  http_task
  * @param arg: pointer on argument(not used here)
  * @retval None
  */
void http_task(void *arg)
{
  int sock, newconn, size;
  struct sockaddr_in address, remotehost;

  vTaskDelay(200);
  printf("\n http_task \n");
 /* create a TCP socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("can not create socket\n");
    return;
  }
  else
  {
	  printf("socket ok\n");
  }

  /* bind to port 80 at any interface */
  address.sin_family = AF_INET;
  address.sin_port = htons(80);
  address.sin_addr.s_addr = INADDR_ANY;
  if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
  {
    printf("can not bind socket\n");
    close(sock);
    return;
  }
  else
  {
	  printf("bind ok\n");
  }

  /* listen for connections (TCP listen backlog = 1) */
  listen(sock, 1);
  size = sizeof(remotehost);
  while (1)
  {
     newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
     if (newconn >= 0)
     {
       http_server(newconn);
     }
     else
     {
       close(newconn);
     }
//     DBG("FreeHeap=%x,MinHeap=%x\n",(unsigned int)xPortGetFreeHeapSize(),(unsigned int)xPortGetMinimumEverFreeHeapSize());

  }
}
