/*
 *  i2c_slave.h
 */
#ifndef __SPI_SLAVE_H__
#define __SPI_SLAVE_H__

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

#define SPI_SLAVE_BUS_NAME     			DT_INST_0_NORDIC_NRF_SPIS_LABEL
#define SPI_SLAVE_GPIOS_NAME    		DT_ALIAS_GPIO_0_LABEL
#define SPI_SLAVE_GPIO_IRQ_PIN  		DT_ALIAS_SPI_0_CSN_PIN

/* FIXME review these defines later */
#define SPI_SLAVE_TX_STACK_SIZE 		1024
#define SPI_SLAVE_CMD_BUF_SIZE    		32
#define SPI_SLAVE_CMD_COUNT				3
#define SPI_SLAVE_USER_DATA_MIN   		32

#define QUEUE_ELEMENTS      			8
#define ALIGNMENT            			4  // 32-bit alignment

/* Special Values */
#define SPI_WRITE               0x0A
#define SPI_READ                0x0B
#define READY_NOW               0x02
#define SANITY_CHECK            0x02

/* Offsets */
#define STATUS_HEADER_READY     0
#define STATUS_HEADER_TOREAD    3

#define PACKET_TYPE             0
#define EVT_BLUE_INITIALIZED    0x01

/* Needs to be aligned with the SPI master buffer size */
#define SPI_MAX_MSG_LEN         255


typedef struct spi_slave_msg {
	u32_t       length;
    u8_t        data [SPI_MAX_MSG_LEN];
} spi_slave_msg_t;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void spi_slave_init(void);
int  spi_slave_enqueue_msg(u8_t * data, u32_t length);

#endif  /* __SPI_SLAVE_H__ */