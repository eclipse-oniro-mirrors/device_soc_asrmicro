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

#ifndef __DUET_I2S_H
#define __DUET_I2S_H

#ifdef __cplusplus
extern "c" {
#endif

#include <errno.h>
#include "duet.h"

    /* Interrupt status register fields */
#define I2S_INTERRUPT_TXFO    (1 << 5)
#define I2S_INTERRUPT_TXFE    (1 << 4)
#define I2S_INTERRUPT_RXFO    (1 << 1)
#define I2S_INTERRUPT_RXDA    (1 << 0)

    /* I2S protocal mode */
#define I2S_MODE_LEFT_ALIGN   (0)
#define I2S_MODE_RIGHT_ALIGN  (1)
#define I2S_MODE_PHILIPS      (2)

#define I2S_TX_BLOCK_EN       (1)
#define I2S_RX_BLOCK_EN       (1)
#define I2S_TX_CHAN_EN        (1)
#define I2S_RX_CHAN_EN        (1)

#define I2S_FIFO_TRIGGERL_LEVEL_1   (1)
#define I2S_FIFO_TRIGGERL_LEVEL_2   (2)
#define I2S_FIFO_TRIGGERL_LEVEL_3   (3)
#define I2S_FIFO_TRIGGERL_LEVEL_4   (4)
#define I2S_FIFO_TRIGGERL_LEVEL_5   (5)
#define I2S_FIFO_TRIGGERL_LEVEL_6   (6)
#define I2S_FIFO_TRIGGERL_LEVEL_7   (7)
#define I2S_FIFO_TRIGGERL_LEVEL_8   (8)
#define I2S_FIFO_TRIGGERL_LEVEL_9   (9)
#define I2S_FIFO_TRIGGERL_LEVEL_10  (10)
#define I2S_FIFO_TRIGGERL_LEVEL_11  (11)
#define I2S_FIFO_TRIGGERL_LEVEL_12  (12)
#define I2S_FIFO_TRIGGERL_LEVEL_13  (13)
#define I2S_FIFO_TRIGGERL_LEVEL_14  (14)
#define I2S_FIFO_TRIGGERL_LEVEL_15  (15)
#define I2S_FIFO_TRIGGERL_LEVEL_16  (16)

#define I2S_FIFO_TRIGGER_LEVEL_MASK (0xf)

#define I2S_HW_SHIFT_LEFT_4        (0)
#define I2S_HW_SHIFT_LEFT_8        (1)
#define I2S_HW_SHIFT_LEFT_12       (2)
#define I2S_HW_SHIFT_LEFT_16       (3)
#define I2S_HW_SHIFT_LEFT_20       (4)
#define I2S_HW_SHIFT_RIGHT_4       (5)
#define I2S_HW_SHIFT_RIGHT_8       (6)
#define I2S_HW_SHIFT_RIGHT_12      (7)
#define I2S_HW_SHIFT_RIGHT_16      (8)
#define I2S_HW_SHIFT_RIGHT_20      (9)

#define I2S_HW_SHIFT_PADDING_1         (1<<4)
#define I2S_HW_SHIFT_PADDING_0         (0)

    /* I2S module parameters */
#define    I2S_WORDSIZE_DONT_CARE (0)
#define    I2S_WORDSIZE_12bit       (1)
#define    I2S_WORDSIZE_16bit       (2)
#define    I2S_WORDSIZE_20bit       (3)
#define    I2S_WORDSIZE_24bit       (4)
#define    I2S_WORDSIZE_32bit       (5)

#define    I2S_FIFO_DEPTH            (16)
#define    I2S_APB_DATA_WIDTH        (32)

#define I2S_RX_WORDSIZE_MASK       (0x7)
#define I2S_TX_WORDSIZE_MASK       (0x7)

#define I2S_ROLE_MASTER         (1)
#define I2S_ROLE_SLAVE          (0)

#define I2S_MCLK_SRC_FREQ72     (72000000)   // for sample rate 44.1KHz only
#define I2S_MCLK_SRC_FREQ96     (96000000)   // for sample rate 44.1KHz only
#define I2S_MCLK_SRC_FREQ120    (120000000)  // for sample rate 44.1/96/48/32/16/8KHZ

#define I2S_SAMPLE_RATE_44P1K   (44100)
#define I2S_SAMPLE_RATE_96K     (96000)
#define I2S_SAMPLE_RATE_48K     (48000)
#define I2S_SAMPLE_RATE_32K     (32000)
#define I2S_SAMPLE_RATE_16K     (16000)
#define I2S_SAMPLE_RATE_8K      (8000)

    typedef void (*duet_i2s_callback_func)(uint32_t, uint32_t);
    extern duet_i2s_callback_func g_duet_i2s_callback_handler;

    typedef struct {
        uint32_t i2s_sample_rate;
        uint32_t i2s_mclk_src;
        uint32_t i2s_ws;
        uint8_t i2s_role;
        uint8_t i2s_word_size;
        uint8_t i2s_tx_en;
        uint8_t i2s_rx_en;
        uint8_t i2s_fifo_threshold;
        uint8_t i2s_mode;
    } duet_i2s_dev_t;

    __STATIC_INLINE ITstatus i2s_get_interrupt_status(I2S_TypeDef *I2Sx, uint32_t i2s_interrupt)
    {
        return (I2Sx->ISR & i2s_interrupt) ? SET : RESET;
    }

    void duet_i2s_send_data(I2S_TypeDef *I2Sx, uint32_t *left_chan_data, uint32_t *right_chan_data, uint32_t len);

    void duet_i2s_struct_init(duet_i2s_dev_t *pI2S_struct);

    void duet_i2s_interrupt_config(I2S_TypeDef *I2Sx, uint32_t i2s_interrupt, uint32_t new_state);
    void duet_i2s_interrupt_clear(I2S_TypeDef *I2Sx, uint32_t i2s_interrupt);
    void duet_i2s_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);

    void duet_i2s_tx_block_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);
    void duet_i2s_rx_block_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);
    void duet_i2s_tx_channel_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);
    void duet_i2s_rx_channel_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);
    void duet_i2s_master_clock_cmd(I2S_TypeDef *I2Sx, uint32_t new_state);
    int  duet_i2s_init(I2S_TypeDef *I2Sx, duet_i2s_dev_t *pI2S_struct);
    uint32_t duet_i2s_receive_data(I2S_TypeDef *I2Sx, uint8_t lr);

#ifdef __cplusplus
}
#endif

#endif /* __DUET_I2S_H */
