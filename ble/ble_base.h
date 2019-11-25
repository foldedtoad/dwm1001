/*
 *  ble_base.h
 */
#ifndef __BLE_BASE_H__
#define __BLE_BASE_H__

#include "ble_device.h"
#include "ble_service.h"
#include "ble_uuids.h" 

bool is_connected(void);
void bas_notify(void);

int  ble_start_advertising(void);
int  ble_stop_advertising(void);
int  ble_disconnect(void);
int  ble_base_init(void);
                   
#endif  // __BLE_BASE_H__