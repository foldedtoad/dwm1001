/*
 *  ex_13a_accelerometer.c
 *
 * Copyright (c) 2018, Yannis Damigos
 * Copyright (c) 2019, Callender-Consulting LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <stdio.h>
#include <sys/util.h>

#include "ex_13a_accelerometer.h"

#define LOG_LEVEL 3
#include <logging/log.h>
LOG_MODULE_REGISTER(accel);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static int32_t read_sensor(struct device * sensor, 
                           enum sensor_channel channel)
{
    int32_t ret = 0;

    struct sensor_value val[3];

    do {
        ret = sensor_sample_fetch(sensor);
        if (ret < 0 && ret != -EBADMSG) {
            LOG_ERR("Sensor sample update error");
            break;
        }

        ret = sensor_channel_get(sensor, channel, val);
        if (ret < 0) {
            LOG_ERR("Cannot read sensor channels");
            break;
        }

        /* 
         *  Note that resultant values are in fix-point notation.
         *  The 'val1' is the integer part, while 'val2' is the fractional part.
         */
        LOG_INF("x=%d.%u  y=%d.%u  z=%d.%u", 
            val[0].val1, val[0].val2,
            val[1].val1, val[1].val2,
            val[2].val1, val[2].val2);    

    } while (0);

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void accel_init(void)
{
    struct device * accelerometer = device_get_binding(DT_LABEL(DT_INST(0, st_lis2dh)));

    if (accelerometer == NULL) {
        LOG_ERR("Could not get %s device", DT_LABEL(DT_INST(0, st_lis2dh)));
        return;
    }

    while (1) {

        //LOG_INF("Accelerometer data:");
        if (read_sensor(accelerometer, SENSOR_CHAN_ACCEL_XYZ) < 0) {
            LOG_INF("Failed to read accelerometer data");
        }

        k_sleep(K_MSEC(1000));
    }
}

/*---------------------------------------------------------------------------*/
/*  Minimal dw_main (stub)                                                   */
/*---------------------------------------------------------------------------*/
int dw_main(void)
{
    /*  Just spin in a sleepy loop */
    while (1) {
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
