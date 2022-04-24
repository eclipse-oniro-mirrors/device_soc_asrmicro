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

#ifndef __DUET_DMA_H
#define __DUET_DMA_H

#include "duet.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_SRC_DATA_WIDTH_BYTE       (0)
#define DMA_SRC_DATA_WIDTH_HALFWORD   (1)
#define DMA_SRC_DATA_WIDTH_WORD       (2)
#define DMA_DST_DATA_WIDTH_BYTE       (0)
#define DMA_DST_DATA_WIDTH_HALFWORD   (1)
#define DMA_DST_DATA_WIDTH_WORD       (2)

#define DMA_SRC_ADDR_INC_BYTE         (0)
#define DMA_SRC_ADDR_INC_HALFWORD     (1)
#define DMA_SRC_ADDR_INC_WORD         (2)
#define DMA_SRC_ADDR_INC_FIX          (3)

#define DMA_DST_ADDR_INC_BYTE         (0)
#define DMA_DST_ADDR_INC_HALFWORD     (1)
#define DMA_DST_ADDR_INC_WORD         (2)
#define DMA_DST_ADDR_INC_FIX          (3)

#define DMA_OP_MODE_STOP                (0)
#define DMA_OP_MODE_BASIC               (1)
#define DMA_OP_MODE_AUTO_REQ            (2)
#define DMA_OP_MODE_PING_PONG           (3)
#define DMA_OP_MODE_MEM_SCT_GAT_PRI     (4)
#define DMA_OP_MODE_MEM_SCT_GAT_ALT     (5)
#define DMA_OP_MODE_PERI_SCT_GAT_PRI    (6)
#define DMA_OP_MODE_PERI_SCT_GAT_ALT    (7)

#define DMA_R_POWER_1                   (0)   // every dma period   nums of transfer uinit
#define DMA_R_POWER_2                   (1)
#define DMA_R_POWER_4                   (2)
#define DMA_R_POWER_8                   (3)
#define DMA_R_POWER_16                 (4)
#define DMA_R_POWER_32                 (5)
#define DMA_R_POWER_64                 (6)
#define DMA_R_POWER_128               (7)
#define DMA_R_POWER_256               (8)
#define DMA_R_POWER_512               (9)
#define DMA_R_POWER_1024             (10)

#define DMA_N_1_MAX                     (1024)

#define DMA_MAX_CHAN_NUM                (32)  // nums of primary channels
typedef enum {
    DMA_CH_UART0_TX = 0,
    DMA_CH_UART0_RX,
    DMA_CH_UART1_TX,
    DMA_CH_UART1_RX,
    DMA_CH_UART2_TX,
    DMA_CH_UART2_RX,
    DMA_CH_SPI0_TX,
    DMA_CH_SPI0_RX,
    DMA_CH_SPI1_TX,
    DMA_CH_SPI1_RX,
    DMA_CH_SPI2_TX,
    DMA_CH_SPI2_RX,
    DMA_CH_I2C0_RX,
    DMA_CH_I2C0_TX,
    DMA_CH_I2C1_RX,
    DMA_CH_I2C1_TX,
} DMA_CHANNEL;

typedef void (*duet_dma_callback_func)(uint32_t);

typedef struct {
    uint32_t cycle_ctl : 3;
    uint32_t next_useburst : 1;
    uint32_t n_minus_1 : 10;
    uint32_t R_pow : 4;
    uint32_t src_prot : 3;
    uint32_t dest_prot : 3;
    uint32_t src_size : 2;
    uint32_t src_inc : 2;
    uint32_t dst_size : 2;
    uint32_t dst_inc : 2;

} Chan_Ctl_Data_TypeDef;

typedef struct {
    uint32_t SrcEndPtr;
    uint32_t DstEndPtr;
    uint32_t n_minus1;
    uint8_t chan_num;
    uint8_t next_useburst;
    uint32_t cycle;
    uint32_t R_power;
    uint32_t SrcDataWidth;
    uint32_t DstDataWidth;
    uint32_t SrcAddrInc;
    uint32_t DstAddrInc;
    uint8_t interrupt_en;
    uint8_t use_pri_only;
} DMA_Init_Struct_Type;

typedef struct {
    uint32_t chan_src_end_ptr;
    uint32_t chan_dst_end_ptr;
    Chan_Ctl_Data_TypeDef chan_ctr;
    uint32_t resv;
} Chan_Cfg_TypeDef;

Chan_Cfg_TypeDef *duet_dma_ctrl_block_init(void);
void duet_dma_init(void);
void duet_dma_interrupt_clear(uint32_t chan_idx);
ITstatus duet_dma_get_interrupt_status(uint32_t chan_idx);
void duet_dma_interrupt_config(uint32_t chan_idx, uint8_t new_state);
void duet_dma_channel_cmd(uint32_t chan_idx, uint8_t new_state);
void duet_dma_generate_sw_req(uint32_t chan_idx);
void duet_dma_mem2mem(uint8_t chan_num, uint8_t *mem_src, uint8_t *mem_dst, uint16_t len);
void duet_dma_uart_rx(uint8_t uart_idx, uint8_t *data, uint16_t len);
void duet_dma_uart_tx(uint8_t uart_idx, uint8_t *data, uint16_t len);
void duet_dma_spi_rx(uint8_t ssp_idx, uint8_t *data, uint16_t len);
void duet_dma_spi_tx(uint8_t ssp_idx, uint8_t *data, uint16_t len);
void duet_dma_callback_register(uint8_t chn_idx, duet_dma_callback_func func);

#endif // __DUET_DMA_H
