/////////////////////////////////////////////
//
//

#include "type.h"

#ifndef __BLE_API_H__
#define __BLE_API_H__

enum
{
	NON_BROADCAST_MODE,
	BROADCAST_MODE
}; // GAP_BROADCASE_MODE;

enum
{
	NON_DISCOVERABLE_MODE,
	LIMITED_DISCOVERABLE_MODE,
	GENERAL_DISCOVERABLE_MODE
};// GAP_DISCOVERABLE_MODE;

enum
{
	NON_CONNECTABLE_MODE,
	DIRECTED_CONNECTABLE_MODE,
	UNDIRECTED_CONNECTABLE_MODE
} ;// GAP_CONNECTABLE_MODE;

enum
{
	NON_BONDABLE_MODE,
	BONDABLE_MODE
}; //GAP_BONDABLE_MODE;

typedef struct _GAP_MODE
{
	uint8_t broadcase_mode;
	uint8_t discoverable_mode;
	uint8_t connectable_mode;
	uint8_t bondable_mode;
} GAP_MODE;

//if this transaction mode, short value writed by client (Write Request)
#define TRANSACTION_MODE_NONE      0x0
//if this transaction mode, prepare write command send by client (Prepare Write Request)
#define TRANSACTION_MODE_ACTIVE    0x1
//if this transaction mode, excute write command send by client (Execute Write Request)
#define TRANSACTION_MODE_EXECUTE   0x2
//excute write command end
#define TRANSACTION_MODE_CANCEL    0x3


typedef enum{
	CENTRAL_DEVICE,
	PERIPHERAL_DEVICE
}BLE_DEVICE_ROLE;

typedef enum{
	BLE_STACK_INIT_OK = 0,				// Э��ջ��ʼ���ɹ�
	BLE_STACK_CONNECTED,				// ���ӳɹ�
	BLE_STACK_DISCONNECTED,				// �Ͽ�����

	GATT_SERVER_INDICATION_TIMEOUT,
	GATT_SERVER_INDICATION_COMPLETE,
} BLE_STACK_CALLBACK;

/*
* ����handle��ȡuuid��������Լ������uuid������Ҫ���� >= 0 ��ֵ�����򷵻� -1
*/
typedef int16_t (*att_read_f)(uint16_t /*con_handle*/, uint16_t /*attribute_handle*/, uint16_t /*offset*/, uint8_t * /*buffer*/, uint16_t /*buffer_size*/);

/*
* ����handle��ȡuuid��������Լ������uuid������Ҫ���� >= 0 ��ֵ�����򷵻� -1
*/
typedef int16_t (*att_write_f)(uint16_t /*con_handle*/, uint16_t /*attribute_handle*/, uint16_t /*transaction_mode*/, uint16_t /*offset*/, uint8_t * /*buffer*/, uint16_t /*buffer_size*/);


typedef struct{
	uint16_t			flags;//if current is characteristic value, need add ATT_PROPERTY_DYNAMIC flag, is is 128 uuid, need add ATT_PROPERTY_UUID128
	uint16_t			handle;    
	uint16_t			uuid;//all use 16bit uuid
	uint8_t *			uuid128;
	uint8_t *			value;
	uint8_t				value_len;
}attribute_item_t;


typedef struct  _GATT_SERVER_PROFILE
{
	uint8_t *			profile_data;

	att_read_f			attr_read;
	att_write_f			attr_write;
	
}GATT_SERVER_PROFILE;

typedef struct _BLE_APP_CONTEXT
{
	BLE_DEVICE_ROLE		ble_device_role;
}BLE_APP_CONTEXT;


/*
* ��ʼ��BLEЭ��ջ
* 
* ����ֵ
* 	0 : success
*	negative : failed (error code : see BLE_ERROR)
*/
int8_t	InitBleStack(BLE_APP_CONTEXT * ble_app_context, GATT_SERVER_PROFILE * gatt_server_profile);

/*
* ȥ��ʼ��BLEЭ��ջ
*/
int8_t UninitBleStack(void);

/*
* BLEЭ��ջ�ص�����
*/
void BLEStackCallBackFunc(uint8_t event);

/*
* ��ȡBLE�ĵ�ַ(BLE�ĵ�ַ�Ǹ��ݴ�ͳ������ַ����������ַ)
*/
void GetBleAddr(uint8_t * bd_addr);

/*
* Set gap mode
*/
void SetGapMode(GAP_MODE gap_mode);

/*
* get gap discoverable mode
*/
uint8_t GetGapDiscoverableMode(void);

/*
* ���ù㲥�����㲥���Ϊ32���ֽڡ����ù㲥�����Զ��򿪹㲥����
*/
void SetAdvertisingData(uint8_t * advertisement_data, uint32_t data_len);

/*
* ����ɨ����Ӧ�������Ϊ32���ֽڡ�����ɨ����Ӧ�����Զ��򿪹㲥����
*/
void SetScanResponseData(uint8_t * scan_resp_data, uint32_t data_len);

/*
* ���ܹ㲥
*/
void DisableAdvertising(void);

/*
* �򿪹㲥
*/
void EnableAdvertising(void);

/*
* ����handle��ȡuuid
*/
uint16_t GetUuidForHandle(uint16_t handle);

/*
* ����handle��ȡ��Ӧattribute item��
*/
attribute_item_t * GetAttrForHandle(attribute_item_t* attr_list, uint16_t attr_num, uint16_t attribute_handle);

/*
* ����Profile
*/
uint16_t CreateProfileData(attribute_item_t* attribute_list, uint16_t list_num, uint8_t* data_output, uint16_t offset);

/*
 * ��notify��ʽ�����ݴ������ݸ�client�ˣ�client�˲���Ҫresponse
 * returns:
 *		0 		: OK
 *		0x57	: BT Stack ACL buffer is full
 */
int GattServerNotify(uint16_t handle, uint8_t *value, uint16_t value_len);


/*
 * ��indicate��ʽ�����ݴ������ݸ�client�ˣ�client����Ҫresponse
 * ��BLECallback�л��յ���Ӧ�¼�
 *	GATT_SERVER_INDICATION_TIMEOUT	: response ��ʱ
 *	GATT_SERVER_INDICATION_COMPLETE	: response OK
 * 
 * returns:
 *		0 		: OK
 * 		0x90	: GATT_SERVER_INDICATION_IN_PORGRESS
 *		0x57	: BT Stack ACL buffer is full
 */
int GattServerIndicate(uint16_t handle, uint8_t *value, uint16_t value_len);


#endif
