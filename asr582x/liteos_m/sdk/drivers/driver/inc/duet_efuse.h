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

#ifndef _DUET_EFUSE_H_
#define _DUET_EFUSE_H_

#define EFUSE_READ_OP_WAIT_CYCLE        5
#define EFUSE_WRITE_OP_WAIT_CYCLE       (SYSTEM_CLOCK/100000) // 10us
#define EFUSE_DIV_OP_WAIT_CYCLE         31

#define EFUSE_LDO25_OPEN  1
#define EFUSE_LDO25_CLOSE 0

void duet_efuse_init(uint8_t ldo25_open);

uint8_t duet_efuse_byte_read(uint16_t addr);

uint32_t duet_efuse_word_read(uint16_t addr);

void duet_efuse_byte_write(uint16_t addr, uint8_t data);

void duet_efuse_word_write(uint16_t addr, uint32_t data);

void duet_efuse_multi_read(uint16_t start_addr, uint16_t size_in_bytes, uint8_t *pData);

#endif // _LEGA_EFUSE_H_
