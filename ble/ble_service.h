/*
 *  DWM1001 ble_service.h
 */
#ifndef __BLE_SERVICE_H__
#define __BLE_SERVICE_H__

/*---------------------------------------------------------------------------*/
/*  BLE Report                                                               */
/*---------------------------------------------------------------------------*/

struct ble_rep {
	uint16_t node_id;
	float dist;
	uint8_t tqf;
}__attribute__((__packed__));

typedef struct ble_rep ble_rep_t;

struct ble_reps {
    uint8_t cnt;
	ble_rep_t ble_rep[10];
}__attribute__((__packed__));

typedef struct ble_reps ble_reps_t;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int dwm1001_notify(uint8_t * data, uint32_t len);
                   
#endif  // __BLE_SERVICE_H__