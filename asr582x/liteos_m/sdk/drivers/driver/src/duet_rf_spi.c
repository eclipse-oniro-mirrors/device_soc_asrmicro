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

#include <stdint.h>
#include <stdio.h>
#include "duet_cm4.h"
#include "duet_common.h"
#include "duet_rf_spi.h"
void spi_sw_protect_write(uint16_t addr, uint16_t data)
{
    *((volatile int *) (SPI_COMMAND + TRANS_MODE_OFT))   = 0;
    *((volatile int *) (SPI_COMMAND + PRESCALER_OFT))    = 3; // 8:80M/16=5M, 2:80M/4=20M
    *((volatile int *) (SPI_COMMAND + ADDR_REG_OFT))     = addr;
    *((volatile int *) (SPI_COMMAND + READNOTWRITE_OFT)) = 0;
    *((volatile int *) (SPI_COMMAND + WDATA_REG_OFT))     = data;
    *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))   = 1;

    while (1) {
        // printf("\n rdate:%08x\n",((uint32_t)*((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) &0x00010000));
        if (((uint32_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) & 0x00010000) == 0) {
            break;
        } else {
            *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))  = 1;
        }
    }

    while (1) {
        if (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)) == 0) {
            break;
        }
    }

}

uint16_t spi_sw_protect_read(uint16_t addr)
{
    *((volatile int *)(SPI_COMMAND + TRANS_MODE_OFT))   = 0;
    *((volatile int *)(SPI_COMMAND + PRESCALER_OFT))    = 8;
    *((volatile int *)(SPI_COMMAND + ADDR_REG_OFT))     = addr;
    *((volatile int *)(SPI_COMMAND + READNOTWRITE_OFT)) = 1;
    *((volatile int *)(SPI_COMMAND + START_FLAG_OFT))   = 1;

    while (1) {
        // printf("\n rdate:%08x\n",((uint32_t)*((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) &0x00010000));
        if (((uint32_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) & 0x00010000) == 0) {
            break;
        } else {
            *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))  = 1;
        }
    }

    while (1) {
        if (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)) == 0) {
            return ((uint16_t) *((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)));
        }
    }

}

void rf_sw_set_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len, uint16_t src_val)
{
    uint16_t tmp, mask, val;

    if ((reg < 0xFF) && (start_bit < 16) && (len <= 16) && (src_val < (1 << len))) {
        tmp = spi_sw_protect_read(reg);

        mask = (1 << len) - 1;      // 1. clear dst bit. eg: len=4, mask = 0xf, 1111
        mask = ~(mask << start_bit); // ~(mask<<4):0xff0f: 1111 1111 0000 1111

        val = tmp & mask;           // 2.val =spi_read() & 0xff0f, clear [7:4]

        src_val = (src_val << start_bit);
        val = val | src_val;        // 3. val spi_read & 0xff0f | val << 4

        spi_sw_protect_write(reg, val);
    } else {
        printf("set_reg input parms not support \r\n");
        return;
    }
}

uint16_t rf_sw_get_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len)
{
    uint16_t mask, val;

    if ((reg < 0xFF) && (start_bit < 16) && (len <= 16)) {
        val = spi_sw_protect_read(reg);    // 1. read reg val

        mask = (1 << len) - 1;      // eg: len =4, 0xf,1111
        mask = mask << start_bit;   // 0x0f00;
        val = val & mask;           // 2. get dst bit

        val = (val >> start_bit);   // 3. ror

        return val;
    } else {
        return -1;
    }
}

/// single wifi run and without PTA init,these api is useful
uint8_t hw_spi_pta = 0;

FLASH_COMMON2_SEG void spi_mst_write(uint16_t addr, uint16_t data)
{
    uint32_t var = 0;

    *((volatile int *) (SPI_COMMAND + TRANS_MODE_OFT))   = 0;
    *((volatile int *) (SPI_COMMAND + PRESCALER_OFT))    = 8; // 8:80M/16=5M, 2:80M/4=20M
    *((volatile int *) (SPI_COMMAND + ADDR_REG_OFT))     = addr;
    *((volatile int *) (SPI_COMMAND + READNOTWRITE_OFT)) = 0;
    *((volatile int *) (SPI_COMMAND + WDATA_REG_OFT))     = data;
    *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))   = 1;

    if (hw_spi_pta == 0) {
        do {
            var = (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)));
        } while (var);
    } else if (hw_spi_pta == 1) {
        while (1) {
            // printf("\n rdate:%08x\n",((uint32_t)*((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) &0x00010000));
            if (((uint32_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) & 0x00010000) == 0) {
                break;
            } else {
                *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))  = 1;
            }
        }

        while (1) {
            if (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)) == 0) {
                break;
            }
        }
    }
    delay(40);
}

FLASH_COMMON2_SEG uint16_t spi_mst_read(uint16_t addr)
{
    uint32_t var = 0;

    *((volatile int *)(SPI_COMMAND + TRANS_MODE_OFT))   = 0;
    *((volatile int *)(SPI_COMMAND + PRESCALER_OFT))    = 8;
    *((volatile int *)(SPI_COMMAND + ADDR_REG_OFT))     = addr;
    *((volatile int *)(SPI_COMMAND + READNOTWRITE_OFT)) = 1;
    *((volatile int *)(SPI_COMMAND + START_FLAG_OFT))   = 1;

    if (hw_spi_pta == 0) {
        do {
            var = (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)));
        } while (var);

        return ((uint16_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)));
    } else if (hw_spi_pta == 1) {
        while (1) {
            if (((uint32_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)) & 0x00010000) == 0) {
                break;
            } else {
                *((volatile int *) (SPI_COMMAND + START_FLAG_OFT))  = 1;
            }
        }

        while (1) {
            if (*((volatile int *)(SPI_COMMAND + START_FLAG_OFT)) == 0) {
                delay(40);
                return ((uint16_t) * ((volatile uint32_t *)(SPI_RDATA + RDATA_REG_OFT)));
            }
        }
    }

    return 0;
}

FLASH_COMMON2_SEG void rf_set_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len, uint16_t src_val)
{
    uint16_t tmp, mask, val;

    if ((reg < 0xFF) && (start_bit < 16) && (len <= 16) && (src_val < (1 << len))) {
        tmp = spi_mst_read(reg);

        mask = (1 << len) - 1;      // 1. clear dst bit. eg: len=4, mask = 0xf, 1111
        mask = ~(mask << start_bit); // ~(mask<<4):0xff0f: 1111 1111 0000 1111

        val = tmp & mask;           // 2.val =spi_read() & 0xff0f, clear [7:4]

        src_val = (src_val << start_bit);
        val = val | src_val;        // 3. val spi_read & 0xff0f | val << 4

        spi_mst_write(reg, val);
    } else {
        printf("set_reg input parms not support \r\n");
        return;
    }
}

uint16_t rf_get_reg_bit(uint16_t reg, uint8_t start_bit, uint8_t len)
{
    uint16_t mask, val;

    if ((reg < 0xFF) && (start_bit < 16) && (len <= 16)) {
        val = spi_mst_read(reg);    // 1. read reg val

        mask = (1 << len) - 1;      // eg: len =4, 0xf,1111
        mask = mask << start_bit;   // 0x0f00;
        val = val & mask;           // 2. get dst bit

        val = (val >> start_bit);   // 3. ror

        return val;
    } else {
        return -1;
    }
}

