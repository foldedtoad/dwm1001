/*! ----------------------------------------------------------------------------
 *  @file    main.c
 *  @brief   RX using SNIFF mode example code
 *
 * @attention
 *
 * Copyright 2016 (c) Decawave Ltd, Dublin, Ireland.
 * Copyright 2019 (c) Frederic Mes, RTLOC.
 *
 * All rights reserved.
 *
 * @author Decawave
 */
#ifdef EX_02D_DEF

#include <zephyr/kernel.h>

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

#define LOG_LEVEL 3
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

/* Example application name and version to display on console. */
#define APP_NAME "RX SNIFF v1.2"

/* Default communication configuration. */
static dwt_config_t config = {
    5,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_128,    /* Preamble length. Used in TX only. */
    DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8,      /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (129)            /* SFD timeout (preamble length + 1 + SFD length - PAC size). 
                      * Used in RX only. */
};

/* SNIFF mode on/off times.
 * ON time is expressed in multiples of PAC size (with the IC adding 1 
 * PAC automatically). 
 * So the ON time of 1 here gives 2 PAC times and, since the
 * configuration (above) specifies DWT_PAC8, we get an ON time of 2x8 symbols, 
 * or around 16 usec.
 * OFF time is expressed in multiples of 128/125 usec (~1 usec).
 * These values will lead to a roughly 50% duty-cycle, each ON and OFF phase 
 * lasting for about 16 usec.
 */
#define SNIFF_ON_TIME 1
#define SNIFF_OFF_TIME 16

/* Buffer to store received frame. See NOTE 1 below. */
#define FRAME_LEN_MAX 127
static uint8 rx_buffer[FRAME_LEN_MAX];

/* Hold copy of status register state here for reference so that it 
 * can be examined at a debug breakpoint.
 */
static uint32 status_reg = 0;

/*  Hold copy of frame length of frame received (if good) so that it 
 *  can be examined at a debug breakpoint. 
 */
static uint16 frame_len = 0;

/**
 * Application entry point.
 */
int dw_main(void)
{
    /* Display application name on console. */
    printk(APP_NAME);

    /* Configure DW1000 SPI */
    openspi();

    /* Reset and initialise DW1000. See NOTE 2 below.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. 
     * After initialisation SPI rate can be increased for optimum
     * performance.
     */

	/* Target specific drive of RSTn line into DW1000 low for a period. */
    reset_DW1000(); 

    port_set_dw1000_slowrate();

    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR) {
        printk("INIT FAILED");
        while (1) { /* spin */ };
    }

    port_set_dw1000_fastrate();

    /* This is put here for testing, so that we can see the 
     * receiver ON/OFF pattern using an oscilloscope.
     */
    dwt_setlnapamode(DWT_LNA_ENABLE | DWT_PA_ENABLE);

    /* Configure DW1000. */
    dwt_configure(&config);

    /* Configure SNIFF mode. */
    dwt_setsniffmode(1, SNIFF_ON_TIME, SNIFF_OFF_TIME);

    /* Configure DW1000 LEDs */
    dwt_setleds(1);

    /* Loop forever receiving frames. */
    while (1) {
        int i;

        /* TESTING BREAKPOINT LOCATION #1 */

        /* Clear local RX buffer to avoid having leftovers from previous receptions.
         * This is not necessary but is included here to aid reading the RX buffer.
         * This is a good place to put a breakpoint. Here (after first time through
         * the loop) the local status register will be set for last event
         * and if a good receive has happened the data buffer will have the data 
         * in it, and frame_len will be set to the length of the RX frame.
         */
        for (i = 0 ; i < FRAME_LEN_MAX; i++ ) {
            rx_buffer[i] = 0;
        }

        /* Activate reception immediately. See NOTE 3 below. */
        dwt_rxenable(DWT_START_RX_IMMEDIATE);

        /* Poll until a frame is properly received or an RX error occurs. 
         * See NOTE 4 below.
         * STATUS register is 5 bytes long but we are not interested in the 
         * high byte here, so we read a more manageable 32-bits with this API call.
         */
        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & 
                (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_ERR)))
        { /* spin */ };

        if (status_reg & SYS_STATUS_RXFCG) {
            /* Clear good RX frame event in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);

            /* A frame has been received, copy it to our local buffer. */
            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
            if (frame_len <= FRAME_LEN_MAX) {
                dwt_readrxdata(rx_buffer, frame_len, 0);

                LOG_HEXDUMP_INF(rx_buffer, frame_len, "sniff data");
            }
        }
        else {
            /* Clear RX error events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_ERR);
        }
    }
}
#endif /* EX_02D_DEF */

/*****************************************************************************
 * NOTES:
 *
 * 1. The device ID is a hard coded constant in the blink to keep the example 
 *    simple but for a real product every device should have a unique ID.
 *    For development purposes it is possible to generate a DW1000 unique ID 
 *    by combining the Lot ID & Part Number values programmed into the
 *    DW1000 during its manufacture. However there is no guarantee this will 
 *    not conflict with someone else's implementation. We recommended that
 *    customers buy a block of addresses from the IEEE Registration Authority 
 *    for their production items. See "EUI" in the DW1000 User Manual.
 * 2. In this example, LDE microcode is not loaded upon calling dwt_initialise().
 *    This will prevent the IC from generating an RX timestamp. 
 *    If time-stamping is required, DWT_LOADUCODE parameter should be used. 
 *    See two-way ranging examples (e.g. examples 5a/5b).
 * 3. In a real application, for optimum performance within regulatory limits, 
 *    it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in 
 *    the target system or the DW1000 OTP memory.
 * 4. dwt_writetxdata() takes the full size of tx_msg as a parameter but only 
 *    copies (size - 2) bytes as the check-sum at the end of the frame is
 *    automatically appended by the DW1000. This means that our tx_msg could 
 *    be two bytes shorter without losing any data (but the sizeof would not
 *    work anymore then as we would still have to indicate the full length of
 *    the frame to dwt_writetxdata()).
 * 5. We use polled mode of operation here to keep the example as simple as 
 *    possible but the TXFRS status event can be used to generate an interrupt.
 *    Please refer to DW1000 User Manual for more details on "interrupts".
 * 6. The user is referred to DecaRanging ARM application (distributed with 
 *    EVK1000 product) for additional practical example of usage, and to the
 *    DW1000 API Guide for more details on the DW1000 driver functions.
 ****************************************************************************/
