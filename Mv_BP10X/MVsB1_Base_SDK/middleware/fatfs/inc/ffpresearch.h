/**
 *******************************************************************************
 * @file    ffpresearch.h
 * @author  Castle
 * @version V0.1.0
 * @date    24-August-2017
 * @brief
 * @maintainer Castle
 *******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, MVSILICON SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2013 MVSilicon </center></h2>
 */
/**
* @addtogroup FatFs
* @{
* @defgroup ffpresearch ffpresearch.h
* @{
*/

#ifndef _FFPRESEARCH_H_
#define _FFPRESEARCH_H_

//#define FLASH_STORE_PNODE

#define MAX_DIR_STACK_DEPTH 9
#define MAX_ACC_NODE_SIZE 10


/**
 * Refer to Audio Decoder Type Set
 */
typedef enum _FileType
{
	FILE_TYPE_UNKOWN = 0,
	FILE_TYPE_FLAC,
	FILE_TYPE_APE,
	FILE_TYPE_WAV,
	FILE_TYPE_AIF,
	FILE_TYPE_AMR,
	FILE_TYPE_AAC,
	FILE_TYPE_MP3,
	FILE_TYPE_WMA,
	FILE_TYPE_SBC,
	FILE_TYPE_MSBC,
} FileType;

typedef struct /*__attribute__ ((packed))*/ _ff_presearch_node
{
	FATFS*	fs;
	WORD	id;
	DWORD	sclust;
	FSIZE_t	objsize;
	DWORD	dir_sect;
	BYTE*	dir_ptr;
	TCHAR	altname[13];
#if FF_FS_EXFAT
	DWORD	c_ofs;
	BYTE	stat;
#endif
	BYTE	file_type;
} ff_presearch_node;


typedef struct /*__attribute__ ((packed))*/ _ff_acc_node
{
	//llist_head list;//for chain
	bool is_used;
	DIR dirs_stack[MAX_DIR_STACK_DEPTH];
	uint32_t dirs_depth;
	uint32_t FirstFileIndex;//ĩ���һ�������ļ���ֵ
	uint32_t prior_files_amount;//������ǰ�ļ��� ֮ǰ�ļ�������
	uint32_t prior_folder_amount;//������ǰ�ļ��е�֮ǰ�ļ���������
	uint32_t song_folder_amount;//������Ч�ļ����ļ�������
} ff_acc_node;

#define FILE_NUM_LEN_INVALID	0xffff //FileNumLen ȷ���ļ��в��������ļ��ţ���δɨ�����ͳ�ơ�
typedef struct /*__attribute__ ((packed))*/ _ff_dir
{
	DIR		FatDir;//fatfs dir�ṹ��
	uint16_t FolderIndex;//�ļ��кš� Ӧ�ò�����ֵ���ɼ������ṩ��0:δ���ã�����:������Ч,Len����
	uint16_t FirstFileIndex;//�����ļ��е�һ������֮ǰ�����������ļ� ������1��ʼ������
	uint16_t FileNumLen; //ͬ�����������0:û�и�����FolderIndexΪ��ʱ�����壬0xffff:��δ��ֵ��
} ff_dir;//�ļ��к͸������� ,��1��ʼ���� ����0��ֵ���� ,������prior_files_amount num_dir�ȸ���
//Ӧ�ò�˶��ļ���Ϣ������Ӧ��:�ϵ�����ļ��˶�
typedef void (*ff_file_callback)(DIR *dir, FILINFO *finfo, ff_acc_node *acc_node);
//����ֵΪTRUEʱ��Ԥ��������˴��ļ��У�����Ӧ��:���������¼���ļ���
typedef bool (*ff_folder_callback)(FILINFO *finfo);

//�����ûص������͹���ʱ��NULL���ӿ�Ԥ�����ٶ�
void ffpresearch_init(FATFS *fs, ff_file_callback fcb, ff_folder_callback dcb);

FRESULT f_scan_vol(char *volume);

FRESULT f_open_by_num(char *volume, ff_dir *dirhandle, FIL *filehandle, uint32_t file_num, char *file_long_name);


FRESULT f_opendir_by_num(char *volume, ff_dir *dirhandle, uint32_t dir_num, char *dir_long_name);


UINT f_file_real_quantity(void);
UINT f_dir_real_quantity(void);
UINT f_dir_with_song_real_quantity(void);//�ܵ���ЧĿ¼��Ŀ
UINT f_dir_with_song_real_quantity_cur(void);//��ǰ����ЧĿ¼��


FileType get_audio_type(TCHAR *string);

UINT f_file_count_in_dir(ff_dir *dirhandle);

FRESULT f_open_by_num_in_dir(ff_dir *dirhandle, FIL *filehandle, UINT filenum, char *file_long_name);
FRESULT f_open_by_name_in_dir(ff_dir *dirhandle, FIL *filehandle, char *file_long_name);

#endif

/**
 * @}
 * @}
 */
