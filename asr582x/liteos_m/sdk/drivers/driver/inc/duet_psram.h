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

#ifndef __PSRAM_PSRAM_H
#define __PSRAM_PSRAM_H

#ifdef PSRAM_CHIP_APS1604_SQR
#define PSRAM_FLASH_SIZE                    0x200000
#elif PSRAM_CHIP_LY68L6400
#define PSRAM_FLASH_SIZE                    0x800000
#else
#error No any psram chip defined!!!
#endif
#define PSRAM_AMBA_BASE          0x30000000 // AHB base addr
#define PSRAM_FLASH_A1_BASE      PSRAM_AMBA_BASE
#define PSRAM_FLASH_A1_TOP       (PSRAM_FLASH_A1_BASE + PSRAM_FLASH_SIZE)
#define PSRAM_FLASH_A2_BASE      PSRAM_FLASH_A1_TOP
#define PSRAM_FLASH_A2_TOP       PSRAM_FLASH_A1_TOP
#define PSRAM_FLASH_B1_BASE      PSRAM_FLASH_A1_TOP
#define PSRAM_FLASH_B1_TOP       PSRAM_FLASH_A1_TOP
#define PSRAM_FLASH_B2_BASE      PSRAM_FLASH_A1_TOP
#define PSRAM_FLASH_B2_TOP       PSRAM_FLASH_A1_TOP

/**
 *APS1604-SQR
 */
#define FLASH_CMD_READ_ID           0x9F
#define FLASH_CMD_READ              0x03
#define FLASH_CMD_FAST_READ         0x0B
#define FLASH_CMD_FAST_READ_QUAD    0xEB
#define FLASH_CMD_WRITE             0x02
#define FLASH_CMD_QUAD_WRITE        0x38
#define FLASH_CMD_WRAPPED_READ      0x8B
#define FLASH_CMD_WRAPPED_WRITE     0x82
#define FLASH_CMD_MODE_REG_READ     0xB5
#define FLASH_CMD_MODE_REG_WRITE    0xB1
#define FLASH_CMD_ENTER_QUAD_MODE   0x35
#define FLASH_CMD_EXIT_QUAD_MODE    0xF5
#define FLASH_CMD_RESET_ENABLE      0x66
#define FLASH_CMD_RESET             0x99
#define FLASH_CMD_BURST_LENGTH_TOGGLE   0xC0

typedef enum {
    PSRAM_CHANNEL_4_9,
    PSRAM_CHANNEL_16_21
} duet_psram_channel;
typedef enum {
    PSRAM_MODE_SPI,
    PSRAM_MODE_QSPI
} duet_psram_mode;
/**
 * psram set the channel used
 * @note  this function must be called before psram config
 * @param[in]  channel  The sets of gpio pads used for psram
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int psram_set_channel(duet_psram_channel channel);

/**
 * psram config
 * @note  this function must be called before use psram address space
 * @param[in]  mode  The psram config mode
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int psram_config(duet_psram_mode mode);

#endif // __PSRAM_PSRAM_H
