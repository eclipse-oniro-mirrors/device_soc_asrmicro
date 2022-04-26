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

#include "duet_i2s.h"
#include "apll_rf.h"

duet_i2s_callback_func g_duet_i2s_callback_handler;

void duet_i2s_struct_init(duet_i2s_dev_t *pI2S_struct)
{
    pI2S_struct->i2s_role = I2S_ROLE_MASTER;
    pI2S_struct->i2s_word_size = I2S_WORDSIZE_16bit;
    pI2S_struct->i2s_tx_en = ENABLE;
    pI2S_struct->i2s_rx_en = ENABLE;
    pI2S_struct->i2s_fifo_threshold = I2S_FIFO_TRIGGERL_LEVEL_4;
    pI2S_struct->i2s_sample_rate = I2S_SAMPLE_RATE_44P1K;
    pI2S_struct->i2s_mclk_src = I2S_MCLK_SRC_FREQ120;
    pI2S_struct->i2s_ws  = 16;
    pI2S_struct->i2s_mode = I2S_MODE_PHILIPS;
}

void duet_i2s_interrupt_config(I2S_TypeDef *I2Sx, uint32_t i2s_interrupt, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->IMR &= ~i2s_interrupt;
    } else {
        I2Sx->IMR |= i2s_interrupt;
    }
}

void duet_i2s_interrupt_clear(I2S_TypeDef *I2Sx, uint32_t i2s_interrupt)
{
    if (i2s_interrupt == I2S_INTERRUPT_TXFO) {
        I2Sx->TOR;    // read to clear interrupt
    } else if (i2s_interrupt == I2S_INTERRUPT_RXFO) {
        I2Sx->ROR;
    }
}

void duet_i2s_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->IER |= ENABLE;
    } else {
        I2Sx->IER &= DISABLE;
    }
}

void duet_i2s_tx_block_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->ITER |= ENABLE;
    } else {
        I2Sx->ITER &= DISABLE;
    }
}

void duet_i2s_rx_block_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->IRER |= ENABLE;
    } else {
        I2Sx->IRER &= DISABLE;
    }
}

void duet_i2s_tx_channel_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->TER |= ENABLE;
    } else {
        I2Sx->TER &= DISABLE;
    }
}

void duet_i2s_rx_channel_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->RER |= ENABLE;
    } else {
        I2Sx->RER &= DISABLE;
    }
}

void duet_i2s_master_clock_cmd(I2S_TypeDef *I2Sx, uint32_t new_state)
{
    if (new_state == ENABLE) {
        I2Sx->CER |= ENABLE;
        REG_WR(0X40000844, (0x1 << 13) | (0x1 << 24)); // open clock source of i2s
    } else {
        I2Sx->CER &= DISABLE;
        REG_WR(0X4000084C, (0x1 << 13) | (0x1 << 24)); // close clock source of i2s
    }
}

int duet_i2s_init(I2S_TypeDef *I2Sx, duet_i2s_dev_t *pI2S_struct)
{
    uint32_t mclk_divider = 0;
    uint32_t sclk_divider = 0;
    uint32_t lrclk_divider = 0;

    if (pI2S_struct->i2s_role == I2S_ROLE_MASTER) {
        if (pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_44P1K) {
            if (pI2S_struct->i2s_mclk_src == I2S_MCLK_SRC_FREQ120
                ||  pI2S_struct->i2s_mclk_src == I2S_MCLK_SRC_FREQ96
                ||  pI2S_struct->i2s_mclk_src == I2S_MCLK_SRC_FREQ72) {
                mclk_divider = 17;
            }
        } else if (pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_96K
                   || pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_48K
                   || pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_32K
                   || pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_16K
                   || pI2S_struct->i2s_sample_rate == I2S_SAMPLE_RATE_8K) {
            if (pI2S_struct->i2s_mclk_src == I2S_MCLK_SRC_FREQ120) {
                mclk_divider = 13;
            }
        }

        if (mclk_divider == 0) {
            return EIO;
        }

        lrclk_divider = 2 * pI2S_struct->i2s_ws;
        sclk_divider = pI2S_struct->i2s_mclk_src / mclk_divider / pI2S_struct->i2s_sample_rate / lrclk_divider;
    }

    if (duet_config_apll_clk(pI2S_struct->i2s_mclk_src) != 0) {
        return EIO;
    }

    duet_i2s_master_clock_cmd(I2Sx, ENABLE);
    duet_i2s_cmd(I2Sx, DISABLE);

    I2Sx->RCR &= ~I2S_RX_WORDSIZE_MASK;
    I2Sx->RCR |= pI2S_struct->i2s_word_size;

    I2Sx->TCR &= ~I2S_TX_WORDSIZE_MASK;
    I2Sx->TCR |= pI2S_struct->i2s_word_size;

    I2Sx->RFCR &= ~I2S_FIFO_TRIGGER_LEVEL_MASK;
    I2Sx->RFCR |= pI2S_struct->i2s_fifo_threshold;

    I2Sx->TFCR &= ~I2S_FIFO_TRIGGER_LEVEL_MASK;
    I2Sx->TFCR |= pI2S_struct->i2s_fifo_threshold;
    I2Sx->RFF |= 0x1;
    I2Sx->TFF |= 0x1;
    if (pI2S_struct->i2s_tx_en == ENABLE) {
        duet_i2s_tx_block_cmd(I2Sx, ENABLE);
        duet_i2s_tx_channel_cmd(I2Sx, ENABLE);
    } else {
        duet_i2s_tx_block_cmd(I2Sx, DISABLE);
        duet_i2s_tx_channel_cmd(I2Sx, DISABLE);
    }

    if (pI2S_struct->i2s_rx_en == ENABLE) {

        duet_i2s_rx_block_cmd(I2Sx, ENABLE);
        duet_i2s_rx_channel_cmd(I2Sx, ENABLE);
    } else {
        duet_i2s_rx_block_cmd(I2Sx, DISABLE);
        duet_i2s_rx_channel_cmd(I2Sx, DISABLE);
    }

    if (pI2S_struct->i2s_role == I2S_ROLE_MASTER) {
        I2S_CLK_DIV->i2s_mclk_divider = mclk_divider - 1;
        I2S_CLK_DIV->i2s_sclk_divider = sclk_divider - 1;
        I2S_CLK_DIV->i2s_lrclk_divider = lrclk_divider - 1;
        I2S_CLK_DIV->i2s_slave_mode = 0; // set to I2S master
    } else {
        I2S_CLK_DIV->i2s_slave_mode = 1; // set to I2S slave
    }

    return 0;
}

uint32_t duet_i2s_receive_data(I2S_TypeDef *I2Sx, uint8_t lr)
{
    if (lr == 0) { // left channel
        return I2Sx->LRBR_LTHR;
    } else { // right channel
        return I2Sx->RRBR_RTHR;
    }
}

void duet_i2s_send_data(I2S_TypeDef *I2Sx, uint32_t *left_chan_data, uint32_t *right_chan_data, uint32_t len)
{
    while (len) {
        while (!i2s_get_interrupt_status(I2Sx, I2S_INTERRUPT_TXFE)); // wait till tx fifo emptys
        for (int i = 0; i < I2S_FIFO_DEPTH && len > 0; i++, len--) {
            I2Sx->LRBR_LTHR = *left_chan_data++;
            I2Sx->RRBR_RTHR = *right_chan_data++;
        }
    }
    while (!i2s_get_interrupt_status(I2Sx, I2S_INTERRUPT_TXFE)); // wait till tx fifo emptys
}

void I2S_IRQHandler(void)
{
    uint32_t g_data_l = 0;
    uint32_t g_data_r = 0;
    if (i2s_get_interrupt_status(I2S, I2S_INTERRUPT_RXDA)) { // rx data available
        g_data_l = I2S->LRBR_LTHR;
        g_data_r = I2S->RRBR_RTHR;
        if (g_duet_i2s_callback_handler != NULL) {
            g_duet_i2s_callback_handler(g_data_l, g_data_r);
        }
    }
}
