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

#ifndef __DUET_RF_SPI_H__
#define __DUET_RF_SPI_H__

#define COMBO_SPI_CTRL_ADDR     0x4000B000
#define TRANS_MODE_OFT          0x0800
#define PRESCALER_OFT           0x0804
#define ADDR_REG_OFT            0x0808
#define READNOTWRITE_OFT        0x080c
#define WDATA_REG_OFT           0x0810
#define RDATA_REG_OFT           0x0814
#define START_FLAG_OFT          0x0818
#define SPI_COMMAND             COMBO_SPI_CTRL_ADDR
#define SPI_RDATA               COMBO_SPI_CTRL_ADDR

void spi_mst_write(uint16_t addr, uint16_t data);
uint16_t spi_mst_read(uint16_t addr);
uint16_t spi_sw_protect_read(uint16_t addr);
void spi_sw_protect_write(uint16_t addr, uint16_t data);

void rf_set_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len, uint16_t src_val);
uint16_t rf_get_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len);

#endif // __RF_SPI_H__