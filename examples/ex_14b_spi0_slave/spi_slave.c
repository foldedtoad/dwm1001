/*! --------------------------------------------------------------------------
 *  @file    spi_slave.c
 *  @brief   Support for slave-mode SPI communications.
 *
 * @attention
 *
 * Copyright 2019 (c) Callender-Consulting, LLC
 *
 * All rights reserved.
 *
 * @author Robin Callender
 */
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/spi.h>

#include "spi_slave.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int spi_slave_write(const struct device * spi, 
                    struct spi_config * spi_cfg,
                    uint16_t * data)
{
    struct spi_buf bufs = {
            .buf = data,
            .len = 2
    };
    struct spi_buf_set tx = {
        .buffers = &bufs
    };
    
    tx.count = 1;

    return spi_write(spi, spi_cfg, &tx);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
int spi_slave_read(const struct device * spi, 
                   struct spi_config * spi_cfg,
                   uint16_t * data)
{
    struct spi_buf bufs = {
            .buf = data,
            .len = 2
    };
    struct spi_buf_set rx = {
        .buffers = &bufs
    };
    
    rx.count = 1;

    return spi_read(spi, spi_cfg, &rx);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void spi_slave_init(void)
{
    const struct device * spi;
    struct spi_config spi_cfg;

    uint16_t tx_data = 0x5678;
    uint16_t rx_data = 0;

    printk("SPI Slave example application\n");

    printk("rx_data buffer at %p\n", &rx_data);

    spi = device_get_binding(DT_LABEL(DT_NODELABEL(spi0)));
    if (!spi) {
        printk("Could not find SPI driver\n");
        return;
    }

    /*
     *  SPI controller configuration structure for the nRF52 series.
     *
     *  param frequency is the bus frequency in hertz
     *  param operation is a bit field with the following parts:
     *
     *     operational mode  [ 0 ]       - master or slave.
     *     mode              [ 1 : 3 ]   - Polarity, phase and loop mode.
     *     transfer          [ 4 ]       - LSB or MSB first.
     *     word_size         [ 5 : 10 ]  - Size of a data frame in bits.
     *     lines             [ 11 : 12 ] - MISO lines: Single/Dual/Quad/Octal.
     *     cs_hold           [ 13 ]      - Hold on the CS line if possible.
     *     lock_on           [ 14 ]      - Keep resource locked for the caller.
     *     cs_active_high    [ 15 ]      - Active high CS logic.
     *
     *  param slave is the slave number from 0 to host controller slave limit.
     *  param cs is a valid pointer on a struct spi_cs_control is CS line is
     *        emulated through a gpio line, or NULL otherwise.
     */

    /* Note: Implicid Mode-0; e.g. CPOL=0, CPHA=0 -- the only mode supported */
    /* Note: The nRF52 series doesn't support CS pin configuration */

    memset(&spi_cfg, 0, sizeof(spi_cfg));

    spi_cfg.operation = SPI_WORD_SET(8) | SPI_OP_MODE_SLAVE;
    spi_cfg.frequency = 1000000;

    printk("%s: %s slave config @ %p:"
            " wordsize(%u), mode(%u/%u/%u)\n", __func__, 
            DT_LABEL(DT_NODELABEL(spi0)),
            &spi_cfg,
            SPI_WORD_SIZE_GET(spi_cfg.operation),
            (SPI_MODE_GET(spi_cfg.operation) & SPI_MODE_CPOL) ? 1 : 0,
            (SPI_MODE_GET(spi_cfg.operation) & SPI_MODE_CPHA) ? 1 : 0,
            (SPI_MODE_GET(spi_cfg.operation) & SPI_MODE_LOOP) ? 1 : 0);

    printk("%s: SPI pin config -- "
           "MOSI(P0.%d), MISO(P0.%d), SCK(P0.%d), CS(P0.%d)\n",
           __func__,
           DT_PROP(DT_NODELABEL(spi0), mosi_pin),
           DT_PROP(DT_NODELABEL(spi0), miso_pin),
           DT_PROP(DT_NODELABEL(spi0), sck_pin),
           DT_PROP(DT_NODELABEL(spi0), csn_pin));

    while (1) {
        spi_slave_read(spi, &spi_cfg, &rx_data);

        if (rx_data == 0x1234) {
            spi_slave_write(spi, &spi_cfg, &tx_data);
        }
        printk("Received: 0x%04X -- %s\n", rx_data,
            (rx_data == 0x1234) ? "correct" : "wrong");
    }
}
