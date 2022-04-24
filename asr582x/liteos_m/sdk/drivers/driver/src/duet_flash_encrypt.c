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

#include "duet.h"
#include "duet_efuse.h"
#include "duet_flash_encrypt.h"

#define FLASH_CRYPT_EFUSE_KEY_ADDR      124
#define FLASH_CRYPT_EFUSE_ENABLE_ADDR   128
#define FLASH_CRYPT_EFUSE_KEY           0x12345678
#define FLASH_CRYPT_RESET               *((volatile unsigned int *)(0x40005038)) // default 0, write 1 reset, HW auto clear

#define FLASH_CRYPT_BIT_NUM             23  // bit0~bit22

int duet_flash_encrypt_status(void)
{
    return ((*((volatile unsigned int *)(0x40000910))) & 0x0020) ? 1 : 0;
}

int switch_flash_encrypt(void)
{
    uint32_t reg;
    duet_efuse_init(1);

    // write flash crypt key and enable
    duet_efuse_word_write(FLASH_CRYPT_EFUSE_KEY_ADDR, FLASH_CRYPT_EFUSE_KEY);
    // flash crypt enable
    reg = duet_efuse_word_read(FLASH_CRYPT_EFUSE_ENABLE_ADDR);
    for (int i = 0; i < FLASH_CRYPT_BIT_NUM; i++) {
        if (!(reg & (0x00000001 << i))) {
            duet_efuse_word_write(FLASH_CRYPT_EFUSE_ENABLE_ADDR, (0x00000001 << i));
            // reset to make flash crypt module work
            FLASH_CRYPT_RESET = 1;
            while (FLASH_CRYPT_RESET);
            return 0;
        }
    }
    return EIO;
}

int duet_flash_encrypt(void)
{
    if (duet_flash_encrypt_status()) {
        return 0;
    }

    return switch_flash_encrypt();
}

int duet_flash_disable_encrypt(void)
{
    if (!duet_flash_encrypt_status()) {
        return 0;
    }

    return switch_flash_encrypt();
}

int duet_flash_encrypt_operation_rest(void)
{
    int num = 0;
    uint32_t reg;

    duet_efuse_init(1);
    reg = duet_efuse_word_read(FLASH_CRYPT_EFUSE_ENABLE_ADDR);
    for (int i = 0; i < FLASH_CRYPT_BIT_NUM; i++) {
        num += !(reg & (0x00000001 << i));
    }
    return num;
}