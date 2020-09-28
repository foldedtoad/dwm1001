/*! ----------------------------------------------------------------------------
 * @file    deca_spi.c
 * @brief   SPI access functions
 *
 * @attention
 *
 * Copyright 2015 (c) DecaWave Ltd, Dublin, Ireland.
 * Copyright 2019 (c) Frederic Mes, RTLOC.
 * Copyright 2019 (c) Callender-Consulting LLC.
 *
 * All rights reserved.
 *
 * @author DecaWave
 */

#include "deca_spi.h"
#include "deca_device_api.h"
#include "port.h"

#include <errno.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define LOG_LEVEL 3
#include <logging/log.h>
LOG_MODULE_REGISTER(deca_spi);

const struct device * spi;
struct spi_config * spi_cfg;
struct spi_config   spi_cfgs [4] = {0};

#define SPI_CFGS_COUNT ((sizeof(spi_cfgs)/sizeof(spi_cfgs[0])))

uint8_t tx_buf [255];
uint8_t rx_buf [255];

struct spi_buf bufs [2];

struct spi_buf_set tx; 
struct spi_buf_set rx;

static struct spi_cs_control cs_ctrl;

/*
 *****************************************************************************
 *
 *                              DW1000 SPI section
 *
 *****************************************************************************
 */

/*
 * Function: openspi()
 *
 * Low level abstract function to open and initialise access to the SPI device.
 * returns 0 for success, or -1 for error
 */
int openspi(void)
{
    /* Propagate CS config into all spi_cfgs[] elements */
    cs_ctrl.gpio_dev = device_get_binding(DT_LABEL(DT_PHANDLE_BY_IDX(DT_NODELABEL(spi1), cs_gpios, 0)));
    if (!cs_ctrl.gpio_dev) {
        printk("%s: GPIO binding failed.\n", __func__);
        return -1;
    }
    cs_ctrl.gpio_pin = DT_PHA(DT_NODELABEL(spi1), cs_gpios, pin);
    cs_ctrl.delay = 0U;
    cs_ctrl.gpio_dt_flags =  DT_PHA(DT_NODELABEL(spi1), cs_gpios, flags);
    for (int i=0; i < SPI_CFGS_COUNT; i++) {
        spi_cfgs[i].cs = &cs_ctrl;
    }

    spi_cfg = &spi_cfgs[0];

    spi = device_get_binding(DT_LABEL(DT_NODELABEL(spi1)));
    if (!spi) {
        printk("%s: SPI binding failed.\n", __func__);
        return -1;
    }
    spi_cfg->operation = SPI_WORD_SET(8);
    spi_cfg->frequency = 2000000;

    memset(&tx_buf[0], 0, 255);
    memset(&rx_buf[0], 0, 255);
    bufs[0].buf = &tx_buf[0];
    bufs[1].buf = &rx_buf[0];
    tx.buffers = &bufs[0];
    rx.buffers = &bufs[1];
    tx.count = 1;
    rx.count = 1;

    return 0;
}

void set_spi_speed_slow(void)
{
    spi_cfg = &spi_cfgs[0];
    spi_cfg->operation = SPI_WORD_SET(8);
    spi_cfg->frequency = 2000000;

    memset(&tx_buf[0], 0, 255);
    memset(&rx_buf[0], 0, 255);
}

void set_spi_speed_fast(void)
{
    spi_cfg = &spi_cfgs[1];
    spi_cfg->operation = SPI_WORD_SET(8);
    spi_cfg->frequency = 8000000;

    memset(&tx_buf[0], 0, 255);
    memset(&rx_buf[0], 0, 255);
}

/*
 * Function: closespi()
 *
 * Low level abstract function to close the the SPI device.
 * returns 0 for success, or -1 for error
 */
int closespi(void)
{
    //TODO
    return 0;
}

/*
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success
 */
int writetospi(uint16           headerLength,
               const    uint8 * headerBuffer,
               uint32           bodyLength,
               const    uint8 * bodyBuffer)
{
    decaIrqStatus_t  stat;

#if 0
    LOG_HEXDUMP_INF(headerBuffer, headerLength, "writetospi: Header");
    LOG_HEXDUMP_INF(bodyBuffer, bodyLength, "writetospi: Body");
#endif

    stat = decamutexon();

    memcpy(&tx_buf[0], headerBuffer, headerLength);
    memcpy(&tx_buf[headerLength], bodyBuffer, bodyLength);

    bufs[0].len = headerLength + bodyLength;
    bufs[1].len = headerLength + bodyLength;

    spi_transceive(spi, spi_cfg, &tx, &rx);

    decamutexoff(stat);

    return 0;
}

/*
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data 
 * may be found, or returns 0
 */
int readfromspi(uint16        headerLength,
                const uint8 * headerBuffer,
                uint32        readLength,
                uint8       * readBuffer)
{
    decaIrqStatus_t  stat;

    stat = decamutexon();

    memset(&tx_buf[0], 0, headerLength + readLength);
    memcpy(&tx_buf[0], headerBuffer, headerLength);

    bufs[0].len = headerLength + readLength;
    bufs[1].len = headerLength + readLength;

    spi_transceive(spi, spi_cfg, &tx, &rx);

    memcpy(readBuffer, rx_buf + headerLength, readLength);

    decamutexoff(stat);

#if 0
    LOG_HEXDUMP_INF(headerBuffer, headerLength, "readfromspi: Header");
    LOG_HEXDUMP_INF(readBuffer, readLength, "readfromspi: Body");
#endif

    return 0;
}
