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

#endif  /* __SPI_SLAVE_H__ */