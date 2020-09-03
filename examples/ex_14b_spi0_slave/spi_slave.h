/*
 *  spi_slave.h
 */
#ifndef __SPI_SLAVE_H__
#define __SPI_SLAVE_H__

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* Needs to be aligned with the SPI master buffer size */
#define SPI_MAX_MSG_LEN         255

typedef struct spi_slave_msg {
    uint32_t       length;
    uint8_t        data [SPI_MAX_MSG_LEN];
} spi_slave_msg_t;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void spi_slave_init(void);

#endif  /* __SPI_SLAVE_H__ */