/*
 *  ble_device.h
 */
#ifndef __BLE_DEVICE_H__
#define __BLE_DEVICE_H__

#include "ble_base.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

typedef enum {
	BLE_EVENT__INVALID = 0,
	BLE_EVENT__COMMAND,
	BLE_EVENT__LAST
} ble_event_t;

typedef enum {
	BLE_CMD__INVALID = 0,  
	BLE_CMD__TEST, 
	BLE_CMD__LAST
} ble_cmd_t;

#define MAX_DEVICEID_STRING_LEN 20

extern int  DeviceIdLen;
extern char DeviceId [MAX_DEVICEID_STRING_LEN];

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int  ble_enqueue_msg(ble_event_t charact, uint32_t data);
void ble_device_events(void);
int  ble_device_init(void);
void ble_device_name(void);
                   
#endif  // __BLE_DEVICE_H__