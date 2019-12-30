/*! -------------------------------------------------------------------------_
 *  @file    main.c
 *  @brief   TX then wait for response example code with GPIOs and LEDs
 *
 *           This is the same as the "TX then wait for response" example code
 *           with GPIOs and LEDs configured to help monitor TX and RX activity.
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
#ifdef EX_03C_DEF

#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_spi.h"
#include "port.h"

// zephyr includes
#include <zephyr.h>
#include <sys/printk.h>

#define LOG_LEVEL 3
#include <logging/log.h>
LOG_MODULE_REGISTER(main);

/* Example application name and version to display on console. */
#define APP_NAME "TX W4R LED v1.2\n"

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
    DWT_PHRMODE_EXT, /* PHY header mode. */
    (129)            /* SFD timeout (preamble length + 1 + SFD length - PAC size). 
                        Used in RX only. */           
};

/* The frame sent in this example is a blink encoded as per the 
 * ISO/IEC 24730-62:2013 standard. It is a 14-byte frame composed of the 
 * following fields:
 *     - byte 0: frame control (0xC5 to indicate a multipurpose frame using 
 *               64-bit addressing).
 *     - byte 1: sequence number, incremented for each new frame.
 *     - byte 2 -> 9: device ID, see NOTE 1 below.
 *     - byte 10: encoding header (0x43 to indicate no extended ID, 
 *                temperature, or battery status is carried in the message).
 *     - byte 11: EXT header (0x02 to indicate tag is listening for a response 
 *                immediately after this message).
 *     - byte 12/13: frame check-sum, automatically set by DW1000.
 */
static uint8 tx_msg[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E', 
                         0x43, 0x02, 0, 0};

/* Index to access to sequence number of the blink frame in the tx_msg array. */
#define BLINK_FRAME_SN_IDX 1

/* Inter-frame delay period, in milliseconds. */
#define TX_DELAY_MS 1000

/* Delay from end of transmission to activation of reception, expressed in UWB 
 * microseconds (1 uus is 512/499.2 microseconds). See NOTE 2 below.
 */
#define TX_TO_RX_DELAY_UUS 60

/* Receive response timeout, expressed in UWB microseconds. See NOTE 3 below. */
#define RX_RESP_TO_UUS 5000

/* Buffer to store received frame. See NOTE 4 below. */
#define FRAME_LEN_MAX 127
static uint8 rx_buffer[FRAME_LEN_MAX];

/* Hold copy of status register state here for reference so that it can be 
 * examined at a debug breakpoint.
 */
static uint32 status_reg = 0;

/* Hold copy of frame length of frame received (if good) so that it can be 
 * examined at a debug breakpoint.
 a*/
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

    /* Reset and initialise DW1000. See NOTE 5 below.
     * For initialisation, DW1000 clocks must be temporarily set to crystal 
     * speed. After initialisation SPI rate can be increased for optimum
     * performance.
     */

    /* Target specific drive of RSTn line into DW1000 low for a period. */
    reset_DW1000();

    port_set_dw1000_slowrate();

    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR) {
        printk("INIT FAILED");
        k_sleep(K_MSEC(500));
        while (1) { /* spin */ };
    }
    port_set_dw1000_fastrate();

    /* Configure GPIOs to show TX/RX activity. See NOTE 6 below. */
    dwt_setlnapamode(DWT_LNA_ENABLE | DWT_PA_ENABLE);

    /* Configure LEDs management. See NOTE 6 below. */
    dwt_setleds(DWT_LEDS_ENABLE);

    /* Configure DW1000. See NOTE 7 below. */
    dwt_configure(&config);

    /* Set delay to turn reception on after transmission of the frame. 
     * See NOTE 2 below. 
     */
    dwt_setrxaftertxdelay(TX_TO_RX_DELAY_UUS);

    /* Set response frame timeout. */
    dwt_setrxtimeout(RX_RESP_TO_UUS);

    /* Configure DW1000 LEDs */
    dwt_setleds(1);

    k_yield();

    /* Loop forever sending and receiving frames periodically. */
    while (1) {

        /* Write frame data to DW1000 and prepare transmission.
         * See NOTE 8 below.
         */
        
        /* Zero offset in TX buffer. */
        dwt_writetxdata(sizeof(tx_msg), tx_msg, 0);
        
        /* Zero offset in TX buffer, no ranging. */
        dwt_writetxfctrl(sizeof(tx_msg), 0, 0); 

        /* Start transmission, indicating that a response is expected so that
         * reception is enabled immediately after the frame is sent.
         s*/
        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

        /* We assume that the transmission is achieved normally, now poll for
         * reception of a frame or error/timeout. See NOTE 9 below.
         */
        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & 
            (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
        { /* spin */ };

        if (status_reg & SYS_STATUS_RXFCG) {

            /* Clear local RX buffer to avoid having leftovers from previous 
             * receptions. This is not necessary but is included here to aid 
             * reading the RX buffer.
             */
            for (int i = 0 ; i < FRAME_LEN_MAX; i++ ) {
                rx_buffer[i] = 0;
            }

            /* A frame has been received, copy it to our local buffer. */
            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFL_MASK_1023;
            if (frame_len <= FRAME_LEN_MAX)
            {
                dwt_readrxdata(rx_buffer, frame_len, 0);
            }

            /* TESTING BREAKPOINT LOCATION #1 */

            /* At this point, received frame can be examined in global 
             * "rx_buffer". An actual application would, for example, 
             * start by checking that the format and/or data of the response 
             * are the expected ones. A developer might put a breakpoint here
             * to examine this frame.
             */

            /* Clear good RX frame event in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG);
        }
        else {
            /* Clear RX error/timeout events in the DW1000 status register. */
            dwt_write32bitreg(SYS_STATUS_ID, 
                              SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
        }

        /* Execute a delay between transmissions. */
        Sleep(TX_DELAY_MS);

        /* Increment the blink frame sequence number (modulo 256). */
        tx_msg[BLINK_FRAME_SN_IDX]++;
    }
}
#endif  /* EX_03C_DEF */

/*****************************************************************************
 * NOTES:
 *
 * 1. The device ID is a hard coded constant in the blink to keep the example 
 *    simple but for a real product every device should have a unique ID.
 *    For development purposes it is possible to generate a DW1000 unique ID by
 *    combining the Lot ID & Part Number values programmed into the DW1000 
 *    during its manufacture. However there is no guarantee this will not 
 *    conflict with someone else's implementation. We recommended that
 *    customers buy a block of addresses from the IEEE Registration Authority 
 *    for their production items. See "EUI" in the DW1000 User Manual.
 * 2. TX to RX delay can be set to 0 to activate reception immediately after 
 *    transmission. But, on the responder side, it takes time to process the
 *    received frame and generate the response (this has been measured 
 *    experimentally to be around 70 usec). Using an RX to TX delay slightly less
 *    than this minimum turn-around time allows the application to make the 
 *    communication efficient while reducing power consumption by adjusting the 
 *    time spent with the receiver activated.
 * 3. This timeout is for complete reception of a frame, i.e. timeout duration 
 *    must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough 
 *    time to receive a complete frame sent by the "RX then send a response"
 *    example at the 110k data rate used (around 3 ms).
 * 4. In this example, maximum frame length is set to 127 bytes which is 
 *    802.15.4 UWB standard maximum frame length. DW1000 supports an extended 
 *    frame length (up to 1023 bytes long) mode which is not used in this 
 *    example.
 * 5. In this example, LDE microcode is not loaded upon calling dwt_
 *    initialise(). This will prevent the IC from generating an RX timestamp.
 *    If time-stamping is required, DWT_LOADUCODE parameter should be used. 
 *    See two-way ranging examples (e.g. examples 5a/5b).
 * 6. TX and RX activity can be monitored using the following GPIOs (e.g. 
 *    connected to an oscilloscope):
 *        - GPIO 2: RXLED signal. Driven high from the activation of reception 
 *                  for the LED blink time (~ 225 ms). If the reception 
 *                  remains activated for longer than that, this signal turns 
 *                  into a periodic signal where both ON and OFF phases last
 *                  for LED blink time.
 *        - GPIO 3: TXLED signal. Driven high from the start of a transmission
 *                  for the LED blink time.
 *        - GPIO 5: EXTTXE signal. Follows the transmitter enabled state.
 *        - GPIO 6: EXTRXE signal. Follows the receiver enabled state.
 * 7. In a real application, for optimum performance within regulatory limits, 
 *    it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved 
 *    in the target system or the DW1000 OTP memory.
 * 8. dwt_writetxdata() takes the full size of tx_msg as a parameter but only 
 *    copies (size - 2) bytes as the check-sum at the end of the frame is
 *    automatically appended by the DW1000. This means that our tx_msg could 
 *    be two bytes shorter without losing any data (but the sizeof would not
 *    work anymore then as we would still have to indicate the full length of
 *    the frame to dwt_writetxdata()).
 * 9. We use polled mode of operation here to keep the example as simple as 
 *    possible but all status events can be used to generate interrupts. 
 *    Please refer to DW1000 User Manual for more details on "interrupts".
 * 10. The user is referred to DecaRanging ARM application (distributed with 
 *     EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 *****************************************************************************/
