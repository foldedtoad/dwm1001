/*
 *  spi_master.h
 */
#ifndef __SPI_MASTER_H__
#define __SPI_MASTER_H__

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

/* Needs to be aligned with the SPI master buffer size */
#define SPI_MAX_MSG_LEN         255

typedef struct spi_master_msg {
    uint32_t       length;
    uint8_t        data [SPI_MAX_MSG_LEN];
} spi_master_msg_t;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void spi_master_init(void);

#endif  /* __SPI_MASTER_H__ */