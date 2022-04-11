/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DUET_SPI_H
#define __DUET_SPI_H

#include "duet.h"
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DUET_SPI0_INDEX    0
#define DUET_SPI1_INDEX    1
#define DUET_SPI2_INDEX    2
#define DUET_SPI_NUM       3

#define SPI_ROLE_MASTER       (0x0)
#define SPI_ROLE_SLAVE        (0x4)
#define SPI_ROLE_MASTER_SLAVE  SPI_ROLE_MASTER

#define SPI_CPOL_CPHA_MODE 0
#define SPI_DMA_RX_CONFIG  DISABLE
#define SPI_DMA_TX_CONFIG  DISABLE

#define SPI_CLK               (52000000)
#define SPI_FIFO_DEPTH        (8)

#define SPI_FRAME_FORMAT_SPI  (0x0)

#define SPI_SLAVE_OUTPUT_DISABLE  (1 << 3)

#define SPI_CLK_POLARITY_POS   (0x6)
#define SPI_CLK_POLARITY_LOW   (0x0)
#define SPI_CLK_POLARITY_HIGH  (0x1 << SPI_CLK_POLARITY_POS)

#define SPI_CLK_PHASE_POS      (0x7)
#define SPI_CLK_PHASE_1EDGE    (0x0)
#define SPI_CLK_PHASE_2EDGE    (0x1 << SPI_CLK_PHASE_POS)

#define SPI_DATA_SIZE_4BIT    (0x3)
#define SPI_DATA_SIZE_8BIT    (0x7)
#define SPI_DATA_SIZE_16BIT   (0xF)

/* SPI flags */
#define SPI_FLAG_TX_FIFO_EMPTY         (0x1)
#define SPI_FLAG_TX_FIFO_NOT_FULL      (1 << 1)
#define SPI_FLAG_RX_FIFO_NOT_EMPTY     (1 << 2)
#define SPI_FLAG_RX_FIFO_FULL          (1 << 3)
#define SPI_FLAG_BUSY                  (1 << 4

/* SPI interrupts */
#define SPI_INTERRUPT_RX_FIFO_OVERRUN  (1 << 0)
#define SPI_INTERRUPT_RX_TIMEOUT       (1 << 1)
#define SPI_INTERRUPT_RX_FIFO_TRIGGER  (1 << 2)  // there are four or more entries in rx fifo
#define SPI_INTERRUPT_TX_FIFO_TRIGGER  (1 << 3)  // there are four or fewer entries in tx fifo. what happens when fifo level is exactly 4??
#define SPI_INTERRUPT_ALL              (0xf)
#define SPI_DISABLE_INTERRUPT_ALL      (0x0)

#define SPI_DMA_TX_EN                  (1<<1)
#define SPI_DMA_RX_EN                  (1)

typedef struct {
    uint32_t mode;        /* spi communication mode */
    uint32_t freq;        /* communication frequency Hz */
} duet_spi_config_t;

typedef struct {
    uint8_t      port;    /* spi port */
    duet_spi_config_t config;  /* spi config */
    void        *priv;    /* priv data */
} duet_spi_dev_t;

typedef void (*duet_spi_callback_func)(uint8_t);
extern duet_spi_callback_func g_duet_spi_callback_handler[DUET_SPI_NUM];

__STATIC_INLINE void duet_spi_interrupt_clear(SPI_TypeDef *SPIx, uint8_t spi_interrupt)
{
    SPIx->ICR |= spi_interrupt;
}

__STATIC_INLINE ITstatus duet_spi_get_flag_status(SPI_TypeDef *SPIx, uint8_t spi_flag)
{
    return SPIx->SR & spi_flag;
}

__STATIC_INLINE ITstatus duet_spi_get_interrupt_status(SPI_TypeDef *SPIx, uint8_t spi_interrupt)
{
    return SPIx->MIS & spi_interrupt;
}

__STATIC_INLINE ITstatus duet_spi_get_raw_interrupt_status(SPI_TypeDef *SPIx, uint8_t spi_interrupt)
{
    return SPIx->RIS & spi_interrupt;
}

void duet_spi_interrupt_config(SPI_TypeDef *SPIx, uint8_t spi_interrupt, uint8_t new_state);
int32_t duet_spi_dma_config(duet_spi_dev_t *spi, uint8_t dma_tx_rx_sel, uint8_t new_state);
int32_t duet_spi_cpol_cpha_config(duet_spi_dev_t *spi, uint8_t mode);
void duet_spi_cmd(SPI_TypeDef *SPIx, uint8_t new_state);
void duet_spi_struct_init(duet_spi_dev_t *init_struct);

/**
 * Initialises the SPI interface for a given SPI device
 *
 * @param[in]  spi  the spi device
 *
 * @return  0 : on success, EIO : if the SPI device could not be initialised
 */
int32_t duet_spi_init(duet_spi_dev_t *spi);

/**
 * Spi send
 *
 * @param[in]  spi      the spi device
 * @param[in]  data     spi send data
 * @param[in]  size     spi send data size
 * @param[in]  timeout  timeout in ms
 *
 * @return  0 : on success, EIO : if the SPI device could not be initialised
 */
int32_t duet_spi_send(duet_spi_dev_t *spi, const uint8_t *data, uint16_t size, uint32_t timeout);

/**
 * De-initialises a SPI interface
 *
 *
 * @param[in]  spi the SPI device to be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred
 */
int32_t duet_spi_finalize(duet_spi_dev_t *spi);

#ifdef __cplusplus
}
#endif

#endif /* __DUET_SPI_H */
