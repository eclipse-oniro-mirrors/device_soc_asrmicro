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

#include "duet_spi.h"

duet_spi_callback_func g_duet_spi_callback_handler[DUET_SPI_NUM] = {0};

void duet_spi_struct_init(duet_spi_dev_t *init_struct)
{
    init_struct->config.freq = 1000000; // 1M
    init_struct->config.mode = SPI_ROLE_MASTER;
    init_struct->priv = NULL;
}

void duet_spi_interrupt_config(SPI_TypeDef *SPIx, uint8_t spi_interrupt, uint8_t new_state)
{
    if (new_state == ENABLE) {
        SPIx->IMSC |= spi_interrupt;
    } else {
        SPIx->IMSC &= ~spi_interrupt;
    }
}

int32_t duet_spi_dma_config(duet_spi_dev_t *spi, uint8_t dma_tx_rx_sel, uint8_t new_state)
{
    SPI_TypeDef *SPIx = NULL;
    if (DUET_SPI0_INDEX == spi->port) {
        SPIx = SPI0;
    } else if (DUET_SPI1_INDEX == spi->port) {
        SPIx = SPI1;
    } else if (DUET_SPI2_INDEX == spi->port) {
        SPIx = SPI2;
    } else {
        return EIO;
    }
    if (new_state == ENABLE) {
        SPIx->DMA_CR |= dma_tx_rx_sel;
    } else {
        SPIx->DMA_CR &= ~dma_tx_rx_sel;
    }
    return 0;
}

void duet_spi_cmd(SPI_TypeDef *SPIx, uint8_t new_state)
{
    if (new_state == ENABLE) {
        SPIx->CR1 |= (0x1 << 1);
    } else {
        SPIx->CR1 &= ~(0x1 << 1);
    }
}
int32_t duet_spi_cpol_cpha_config(duet_spi_dev_t *spi, uint8_t mode)
{
    uint8_t cpol, cpha;
    SPI_TypeDef *SPIx = NULL;
    if (DUET_SPI0_INDEX == spi->port) {
        SPIx = SPI0;
    } else if (DUET_SPI1_INDEX == spi->port) {
        SPIx = SPI1;
    } else if (DUET_SPI2_INDEX == spi->port) {
        SPIx = SPI2;
    } else {
        return EIO;
    }
    switch (mode) {
        case 0:
            cpol = 0;
            cpha = 0;
            break;
        case 1:
            cpol = 0;
            cpha = 1;
            break;
        case 2:
            cpol = 1;
            cpha = 0;
            break;
        case 3:
            cpol = 1;
            cpha = 1;
            break;
        default:
            cpol = 0;
            cpha = 0;
            break;
    }
    SPIx->CR0 &= ~(0x3 << 6);  // reset SPI clk phase/polarity setting to mode 0
    SPIx->CR0 |= (cpol << SPI_CLK_POLARITY_POS) | (cpha << SPI_CLK_PHASE_POS);
    return 0;
}

int32_t duet_spi_init(duet_spi_dev_t *spi)
{
    uint32_t tmp_value, spi_clk = SPI_CLK;
    uint8_t cpol, cpha;
    SPI_TypeDef *SPIx = NULL;
    if (DUET_SPI0_INDEX == spi->port) {
        SPIx = SPI0;
    } else if (DUET_SPI1_INDEX == spi->port) {
        SPIx = SPI1;
    } else if (DUET_SPI2_INDEX == spi->port) {
        SPIx = SPI2;
    } else {
        return EIO;
    }
    // enable spi clk
    if (SPIx == SPI0) {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN)));
    } else if (SPIx == SPI1) {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(SPI1_BUS_CLK_EN | SPI1_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (SPI1_BUS_CLK_EN | SPI1_PERI_CLK_EN)));
    } else {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(SPI2_BUS_CLK_EN | SPI2_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (SPI2_BUS_CLK_EN | SPI2_PERI_CLK_EN)));
    }
    // fpga no effect, soc need
    duet_spi_cmd(SPIx, DISABLE);
    duet_spi_interrupt_config(SPIx, SPI_INTERRUPT_ALL, DISABLE);
    duet_spi_interrupt_clear(SPIx, SPI_INTERRUPT_ALL);

    /* set frame format */
    SPIx->CR0 &= ~(0x3 << 4);  // reset FRF to 0
    SPIx->CR0 |= SPI_FRAME_FORMAT_SPI;
    /* set sclk divider */
    SPIx->CPSR &= ~0xff; // reset CPSR to 0
    SPIx->CPSR |= 0x2;   // set CPSR to 2, shoule be even number between 2-254
    SPIx->CR0 &= (0x00ff); // reset SCR to 0
    SPIx->CR0 |= (spi_clk / 2 / spi->config.freq - 1) << 8; // set SCR to 0x7, serial clk = 16M/2/(1+7) = 1M
    switch (SPI_CPOL_CPHA_MODE) {
        case 0:
            cpol = 0;
            cpha = 0;
            break;
        case 1:
            cpol = 0;
            cpha = 1;
            break;
        case 2:
            cpol = 1;
            cpha = 0;
            break;
        case 3:
            cpol = 1;
            cpha = 1;
            break;
        default:
            cpol = 0;
            cpha = 0;
            break;
    }
    /* set sclk polarity & phase */
    SPIx->CR0 &= ~(0x3 << 6);  // reset SPI clk phase/polarity setting to mode 0
    SPIx->CR0 |= (cpol << SPI_CLK_POLARITY_POS) | (cpha << SPI_CLK_PHASE_POS);

    /* set data size */
    SPIx->CR0 &= ~(0xf);   // reset data size to 0
    SPIx->CR0 |= SPI_DATA_SIZE_8BIT;

    if (spi->config.mode == SPI_ROLE_MASTER) {
        SPIx->CR1 &= ~(0x1 << 2);  // reset master/slave select to 0, which is master mode
    } else {
        SPIx->CR1 &= ~(0x1 << 2);  // reset master/slave select to 0, which is master mode
        SPIx->CR1 |= SPI_ROLE_SLAVE; // set to slave role
    }

    /* dma handshake config,
    should be enabled after dmac has been configured and ready */
    //    if(SPI_DMA_TX_CONFIG == ENABLE)
    //    {
    //        SPIx->DMA_CR |= SPI_DMA_TX_EN;
    //    }
    //    else
    //    {
    //        SPIx->DMA_CR &= ~SPI_DMA_TX_EN;
    //    }
    //    if(SPI_DMA_RX_CONFIG == ENABLE)
    //    {
    //        SPIx->DMA_CR |= SPI_DMA_RX_EN;
    //    }
    //    else
    //    {
    //        SPIx->DMA_CR &= ~SPI_DMA_RX_EN;
    //    }
    if (spi->priv) {
        // enable rx interrupt
        SPIx->IMSC |= (SPI_INTERRUPT_RX_TIMEOUT | SPI_INTERRUPT_RX_FIFO_TRIGGER);
        // enable cm4 interrupt
        if (SPIx == SPI0) {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~SPI0_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (SPI0_IRQ_BIT)));
            NVIC_EnableIRQ(SPI0_IRQn);
        } else if (SPIx == SPI1) {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~SPI1_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (SPI1_IRQ_BIT)));
            NVIC_EnableIRQ(SPI1_IRQn);
        } else {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~SPI2_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (SPI2_IRQ_BIT)));
            NVIC_EnableIRQ(SPI2_IRQn);
        }
        g_duet_spi_callback_handler[spi->port] = (duet_spi_callback_func)(spi->priv);
    }
    SPIx->CR1 |= (0x1 << 1);
    return 0;
}

/**
 * De-initialises a SPI interface
 *
 *
 * @param[in]  spi the SPI device to be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred
 */
int32_t duet_spi_finalize(duet_spi_dev_t *spi)
{
    SPI_TypeDef *SPIx;
    unsigned int tmp_value;

    if (NULL == spi) {
        return EIO;
    }

    if (DUET_SPI0_INDEX == spi->port) {
        SPIx = SPI0;
    } else if (DUET_SPI1_INDEX == spi->port) {
        SPIx = SPI1;
    } else if (DUET_SPI2_INDEX == spi->port) {
        SPIx = SPI2;
    } else {
        return EIO;
    }

    // disable all spi interrupt
    SPIx->IMSC  = SPI_DISABLE_INTERRUPT_ALL;
    // disable all spi config
    SPIx->CR0 = 0;
    SPIx->CR1 = 0;

    // disable cm4 interrupt
    if (SPI0 == SPIx) {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~SPI0_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (SPI0_IRQ_BIT)));
        NVIC_DisableIRQ(SPI0_IRQn);
    } else if (SPI1 == SPIx) {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~SPI1_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (SPI1_IRQ_BIT)));
        NVIC_DisableIRQ(SPI1_IRQn);
    } else {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~SPI2_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (SPI2_IRQ_BIT)));
        NVIC_DisableIRQ(SPI2_IRQn);
    }

    // spi sclk disable, fpga no effect, soc need
    if (SPI0 == SPIx) {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN)));
    } else if (SPI1 == SPIx) {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN)));
    } else {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (SPI0_BUS_CLK_EN | SPI0_PERI_CLK_EN)));
    }

    g_duet_spi_callback_handler[spi->port] = NULL;
    return 0;
}

int32_t duet_spi_send(duet_spi_dev_t *spi, const uint8_t *data, uint16_t size, uint32_t timeout)
{
    SPI_TypeDef *SPIx = NULL;
    if (DUET_SPI0_INDEX == spi->port) {
        SPIx = SPI0;
    } else if (DUET_SPI1_INDEX == spi->port) {
        SPIx = SPI1;
    } else if (DUET_SPI2_INDEX == spi->port) {
        SPIx = SPI2;
    } else {
        return EIO;
    }
    while (size--) {
        while (!(duet_spi_get_flag_status(SPIx, SPI_FLAG_TX_FIFO_NOT_FULL))); // wait till tx fifo is not full
        SPIx->DR = *data++;
    }
    return 0;
}

// void duet_spi_receive(SPI_TypeDef * SPIx, void * rx_data, uint16_t len)
// {
//    while (len--)
//    {
//        while (!(spi_get_flag_status(SPIx, SPI_FLAG_RX_FIFO_NOT_EMPTY))); // wait till rx fifo is not empty, change to timeout mechanism???

//        *rx_data++ = SPIx->DR ;
//    }
// }

SPI_TypeDef *getSpixViaIdx(uint8_t spi_idx)
{
    switch (spi_idx) {
        case DUET_SPI0_INDEX:
            return SPI0;
        case DUET_SPI1_INDEX:
            return SPI1;
        case DUET_SPI2_INDEX:
            return SPI2;
        default:
            return NULL;
    }
}

/* spi interrupt handler */
void SPIX_IRQHandler(uint8_t spi_idx)
{
    uint16_t tmp;
    SPI_TypeDef *SPIx = getSpixViaIdx(spi_idx);
    if (duet_spi_get_interrupt_status(SPIx, SPI_INTERRUPT_TX_FIFO_TRIGGER)) {
        duet_spi_interrupt_config(SPIx, SPI_INTERRUPT_TX_FIFO_TRIGGER, DISABLE); // disable
        duet_spi_interrupt_clear(SPIx, SPI_INTERRUPT_TX_FIFO_TRIGGER); // clear
        duet_spi_interrupt_config(SPIx, SPI_INTERRUPT_TX_FIFO_TRIGGER, ENABLE); // enable
    }
    if (duet_spi_get_interrupt_status(SPIx, SPI_INTERRUPT_RX_FIFO_TRIGGER)
         || duet_spi_get_interrupt_status(SPIx, SPI_INTERRUPT_RX_TIMEOUT)) {
        duet_spi_interrupt_config(SPIx, SPI_INTERRUPT_RX_FIFO_TRIGGER | SPI_INTERRUPT_RX_TIMEOUT, DISABLE); // disable
        duet_spi_interrupt_clear(SPIx, SPI_INTERRUPT_RX_FIFO_TRIGGER | SPI_INTERRUPT_RX_TIMEOUT); // clear
        while (SPIx->SR & SPI_FLAG_RX_FIFO_NOT_EMPTY) {
            tmp = (uint16_t)(SPIx->DR);
            if (g_duet_spi_callback_handler[spi_idx] != NULL) {
                g_duet_spi_callback_handler[spi_idx](tmp);
            }
        }
        duet_spi_interrupt_config(SPIx, SPI_INTERRUPT_RX_FIFO_TRIGGER | SPI_INTERRUPT_RX_TIMEOUT, ENABLE); // enable
    }
}

void SPI0_IRQHandler(void)
{
    duet_intrpt_enter();
    SPIX_IRQHandler(0);
    duet_intrpt_exit();
}

void SPI1_IRQHandler(void)
{
    duet_intrpt_enter();
    SPIX_IRQHandler(1);
    duet_intrpt_exit();
}

void SPI2_IRQHandler(void)
{
    duet_intrpt_enter();
    SPIX_IRQHandler(2);
    duet_intrpt_exit();
}

void duet_spi_set_callback(uint8_t spi_idx, duet_spi_callback_func func)
{
    if (spi_idx >= DUET_SPI_NUM) {
        return;
    }
    g_duet_spi_callback_handler[spi_idx] = func;

}