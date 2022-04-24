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

#include "duet_dma.h"

duet_dma_callback_func   g_duet_dma_callback_handler[DMA_MAX_CHAN_NUM] = {0};

Chan_Cfg_TypeDef *duet_dma_ctrl_block_init(void)
{
    return (Chan_Cfg_TypeDef *)(0X4000C000);
}

void duet_dma_init(void)
{
    uint32_t tmp_value = 0;
    DMA_HANDSHAKE_CFG0 = 0XFFFFFFFF;
    DMA_HANDSHAKE_CFG1 = 0XFFFFFFFF;
    // OPEN DMA CLOCK
    tmp_value = REG_RD(PERI_CLK_EN_REG0) & (~DMA_CLK_EN);
    REG_WR(PERI_CLK_EN_REG0, (tmp_value | (DMA_CLK_EN)));
    // open DMA interrupt
    tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~DMA_IRQ_BIT);
    REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (DMA_IRQ_BIT)));
}

void duet_dma_interrupt_clear(uint32_t chan_idx)
{
    DMA_INT_CLR |= (1 << chan_idx); // write 1 to clear, then reset to 0
    DMA_INT_CLR &= ~(1 << chan_idx);
}

ITstatus duet_dma_get_interrupt_status(uint32_t chan_idx)
{
    if (DMA_INT_STAT & (1 << chan_idx)) {
        return SET;
    } else {
        return RESET;
    }
}

void duet_dma_interrupt_config(uint32_t chan_idx, uint8_t new_state)
{
    if (new_state == ENABLE) {
        DMA_INT_MASK |= (1 << chan_idx);    // write 1 to unmask
    } else {
        DMA_INT_MASK &= ~(1 << chan_idx);
    }
}

void duet_dma_channel_cmd(uint32_t chan_idx, uint8_t new_state)
{
    if (new_state == ENABLE) {
        DMA->CHAN_EN_SET |= (1 << chan_idx);
    } else {
        DMA->CHAN_EN_CLR |=  (1 << chan_idx);
    }
}

void duet_dma_alt_channel_cmd(uint32_t chan_idx, uint8_t new_state)
{
    if (new_state == ENABLE) {
        DMA->CHAN_PRI_ALT_SET |= (1 << chan_idx);
    } else {
        DMA->CHAN_PRI_ALT_CLR |= (1 << chan_idx);
    }
}

void duet_dma_generate_sw_req(uint32_t chan_idx)
{
    DMA->CHAN_SW_REQ |= (1 << chan_idx);
}
void duet_dma_generate_perip_req(uint32_t chan_idx)
{
    DMA_WAIT_ON_REQ |= (1 << chan_idx);
}

void duet_dma_mem2mem(uint8_t chan_num, uint8_t *mem_src, uint8_t *mem_dst, uint16_t len)
{
    uint8_t dma_chan = chan_num;
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();
    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_AUTO_REQ;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 1;
    ch_ctl_data.src_inc = DMA_SRC_ADDR_INC_BYTE;
    ch_ctl_data.dst_inc = DMA_DST_ADDR_INC_BYTE;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_BYTE;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_BYTE;

    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t)&mem_src[len - 1];
    ch_cfg.chan_dst_end_ptr = (uint32_t)&mem_dst[len - 1];

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;
    DMA->CFG |= 0x1; // dma enable
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;
    //    DMA->CHAN_EN_CLR |= ~(1<<dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enbale channel 0
    NVIC_EnableIRQ(DMA_IRQn);
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    // manually generate software request for channel 0 for mem2mem transfer
    DMA->CHAN_SW_REQ |= (1 << dma_chan);
}

void duet_dma_uart_rx(uint8_t uart_idx, uint8_t *data, uint16_t len)
{
    uint8_t dma_chan;
    UART_TypeDef *UARTx;
    UARTx = NULL;
    dma_chan = 0;
    if (uart_idx == 2) {
        dma_chan = 5; // uart2 rx channel
        UARTx = UART2;
    } else if (uart_idx == 1) {
        dma_chan = 3; // uart1 rx channel
        UARTx = UART1;
    } else if (uart_idx == 0) {
        dma_chan = 1; // uart0 rx channel
        UARTx = UART0;
    } else {
        return;
    }

    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();
    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 1;
    ch_ctl_data.src_inc = DMA_SRC_ADDR_INC_FIX;
    ch_ctl_data.dst_inc = DMA_DST_ADDR_INC_BYTE;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_BYTE;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_BYTE;

    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t) & (UARTx->DR);
    ch_cfg.chan_dst_end_ptr = (uint32_t)(data + len - 1);

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;
    NVIC_EnableIRQ(DMA_IRQn);
    DMA_WAIT_ON_REQ |= (1 << dma_chan);
    DMA->CFG |= 0x1; // dma enable
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;
    //    DMA->CHAN_EN_CLR |= ~(1<<dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel
}

void duet_dma_uart_tx(uint8_t uart_idx, uint8_t *data, uint16_t len)
{
    uint8_t dma_chan = 0;
    UART_TypeDef *UARTx = NULL;

    if (uart_idx == 2) {
        dma_chan = 4; // uart2 tx channel
        UARTx = UART2;
    } else if (uart_idx == 1) {
        dma_chan = 2; // uart1 tx channel
        UARTx = UART1;
    } else if (uart_idx == 0) {
        dma_chan = 0; // uart0 tx channel
        UARTx = UART0;
    } else {
        return;
    }
    // malloc for channel descriptor
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();

    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 2;
    ch_ctl_data.src_inc = DMA_SRC_ADDR_INC_BYTE;
    ch_ctl_data.dst_inc = DMA_DST_ADDR_INC_FIX;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_BYTE;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_BYTE;
    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t)(data + len - 1);
    ch_cfg.chan_dst_end_ptr = (uint32_t) & (UARTx->DR);

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;
    NVIC_EnableIRQ(DMA_IRQn);
    DMA->CFG |= 0x1; // dma enable
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;
    //    DMA->CHAN_EN_CLR |= ~(1<<dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel
}

void duet_dma_spi_tx(uint8_t ssp_idx, uint8_t *data, uint16_t len)
{
    uint8_t dma_chan = 0;
    // malloc for channel descriptor
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();
    SPI_TypeDef *SPIx = NULL;
    if (ssp_idx == 0) {
        dma_chan = 6; // SPI0 tx channel
        SPIx = SPI0;
    } else if (ssp_idx == 1) {
        dma_chan = 8; // SPI1 tx channel
        SPIx = SPI1;
    } else if (ssp_idx == 2) {
        dma_chan = 10; // SPI2 tx channel
        SPIx = SPI2;
    } else {
        return;
    }
    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 1;
    ch_ctl_data.src_inc = DMA_SRC_ADDR_INC_BYTE;
    ch_ctl_data.dst_inc = DMA_DST_ADDR_INC_FIX;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_BYTE;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_BYTE;

    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t)(data + len - 1);
    ch_cfg.chan_dst_end_ptr = (uint32_t) & (SPIx->DR);

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;

    NVIC_EnableIRQ(DMA_IRQn);
    DMA->CFG |= 0x1; // dma enable
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;
    // set channel useburst bit to diasble sreq from generating dma request
    DMA->CHAN_USE_BURST_SET |= (1 << dma_chan);
    //    DMA->CHAN_EN_CLR |= ~(1<<dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel
}

void duet_dma_spi_rx(uint8_t ssp_idx, uint8_t *data, uint16_t len)
{
    uint8_t dma_chan = 0;
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();
    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;
    SPI_TypeDef *SPIx = NULL;
    if (ssp_idx == 0) {
        dma_chan = 7; // SPI0 Rx channel
        SPIx = SPI0;
    } else if (ssp_idx == 1) {
        dma_chan = 9; // SPI1 Rx channel
        SPIx = SPI1;
    } else if (ssp_idx == 2) {
        dma_chan = 11; // SPI2 Rx channel
        SPIx = SPI2;
    } else {
        return;
    }

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 1;
    ch_ctl_data.src_inc = DMA_DST_ADDR_INC_FIX;
    ch_ctl_data.dst_inc = DMA_SRC_ADDR_INC_BYTE;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_BYTE;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_BYTE;

    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t) & (SPIx->DR);
    ch_cfg.chan_dst_end_ptr = (uint32_t)(data + len - 1);

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;

    NVIC_EnableIRQ(DMA_IRQn);
    DMA->CFG |= 0x1; // dma enable
    DMA_WAIT_ON_REQ |= (1 << dma_chan);
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;

    DMA->CHAN_USE_BURST_CLR |= (1 << dma_chan);
    //    DMA->CHAN_EN_CLR |= ~(1<<dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel
}

void duet_dma_callback_register(uint8_t chn_idx, duet_dma_callback_func func)
{
    g_duet_dma_callback_handler[chn_idx] = func;

}

void DMA_IRQHandler(void)
{
    uint8_t i;
    uint32_t chan_used = DMA_INT_STAT; // get all enabled channels
    for (i = 0; i < DMA_MAX_CHAN_NUM; i++) {
        if (chan_used & (1 << i)) {
            duet_dma_interrupt_clear(i);
            if (g_duet_dma_callback_handler[i] != NULL) {
                g_duet_dma_callback_handler[i](i);
            }
        }
    }
}

