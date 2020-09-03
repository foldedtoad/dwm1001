/* 
 *  ble_device.c 
 */
#include <zephyr/types.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <zephyr.h>

#include "ble_device.h"
#include "ble_base.h"

#define LOG_LEVEL 3
#include <logging/log.h>
LOG_MODULE_REGISTER(ble_device);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
typedef struct ble_msg {
    ble_event_t event;
    uint8_t     data;
} ble_msg_t;

#define QUEUE_ELEMENTS       8
#define ALIGNMENT            4  // 32-bit alignment

K_MSGQ_DEFINE(ble_queue, sizeof(ble_msg_t), QUEUE_ELEMENTS, ALIGNMENT);

int  DeviceIdLen = 0;
char DeviceId [MAX_DEVICEID_STRING_LEN];

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
#define STACKSIZE 1024

struct k_thread ble_queue_thread;
K_THREAD_STACK_DEFINE(ble_queue_stack, STACKSIZE);

static k_tid_t tBleQ;

static bool active = false;

static struct k_work disconnect_work;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int ble_command(uint8_t command)
{
    int status = 0;

    switch (command) {

        case BLE_CMD__TEST:
            LOG_INF("%s: TEST", __func__);

            status = 0;        
            break;

        default:
            LOG_INF("%s: <unknown> %d", __func__, command);
            status = 0;        
            break;
    }

    return status;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void disconnect_work_cb(struct k_work * work)
{
    ble_disconnect();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void ble_device_tasks(void)
{
    while (active) {

        k_sleep(K_MSEC(MSEC_PER_SEC));

        if (is_connected()) {
            /* Battery level (simulation) */
            bas_notify();
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int ble_enqueue_msg(ble_event_t event, uint32_t data)
{
    ble_msg_t msg;

    msg.event = event;
    msg.data = data;

    while (k_msgq_put(&ble_queue, &msg, K_NO_WAIT) != 0) {
        LOG_ERR("%s: k_msgq_put error: purging queue", __func__);
        k_msgq_purge(&ble_queue);
        return -EIO;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void ble_queue_service(void)
{
    int status;
    ble_msg_t msg;
    
    printk("%s: started\n", __func__);

    while (1) {

        k_msgq_get(&ble_queue, &msg, K_FOREVER);

        switch (msg.event) {

            case BLE_EVENT__COMMAND:
                status = ble_command(msg.data);
                break;

            default:
                break;
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void ble_device_name(void)
{
    uint32_t deviceid = NRF_FICR->DEVICEID[0];

    sprintf(DeviceId, "%s_%08x", CONFIG_BT_DEVICE_NAME, deviceid);

    DeviceIdLen = strlen(DeviceId);

    printk("DeviceId: %s\n", DeviceId);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static int ble_queue_init(void)
{
    int status = 0;

    tBleQ = k_thread_create(&ble_queue_thread, 
                            ble_queue_stack, STACKSIZE,
                            (k_thread_entry_t)ble_queue_service, 
                            NULL, NULL, NULL, -1, K_USER, K_FOREVER);

    k_thread_start(&ble_queue_thread);

    return status;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int ble_device_init(void)
{
    int status;

    /*
     *  Create unique device identifier.
     */
    ble_device_name();

    /*
     *  Start base BLE.
     */
    status = ble_base_init();
    if (status < 0) {
        return status;
    }

    /*
     *  Start Advertising.
     */
    status = ble_start_advertising();
    if (status < 0) {
        return status;
    }

    /*
     *  Start queue service on its own thread.
     */
    status = ble_queue_init();
    if (status < 0) {
        return status;
    }

    disconnect_work.handler = disconnect_work_cb;

    /*
     *  Indicate all is initialized.
     */
    active = true;

    /* 
     *  Start polling tasks: thread should never return. Keep last.
     */
    ble_device_tasks();

    return -EINVAL;
}
