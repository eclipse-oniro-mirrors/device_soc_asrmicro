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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "duet.h"
#include "duet_common.h"
#ifdef CFG_DUET_FREERTOS
#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#endif
// void jumpToApp(int addr)
// {
//    __asm("LDR SP, [R0]");
//    __asm("LDR PC, [R0, #4]");
// }

void duet_memset(char *buf, int value, int size)
{
    if (NULL == buf) {
        return;
    }

    while (size--) {
        *buf++ = value;
    }
}

void duet_memcpy(char *dst, char *src, int size)
{
    while (size--) {
        *dst++ = *src++;
    }
}

FLASH_COMMON2_SEG void delay(unsigned int cycles)
{
    unsigned int us2cycles = (cycles * 52) / 4;
    while (us2cycles > 0) {
        us2cycles--;
        asm("nop");
    }
}

/* please open flash cache when using this function.
otherwise, it is not accuratd */
void udelay_pl(unsigned int us)
{
    unsigned int us2cycles = us * (SYSTEM_CLOCK / 1000000) / 9;
    us2cycles = (us2cycles >= 6) ? (us2cycles - 6) : 0;
    /* 9 clock cycles */
    while (us2cycles > 0) {
        us2cycles--;
        asm("nop");
    }
}

#ifdef CFG_DUET_FREERTOS
void lega_enter_critical_expble(void)
{
    portCLEAR_INTERRUPT_MASK_FROM_ISR((configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1) << (8 - configPRIO_BITS));
}

void lega_exit_critical_expble(void)
{
    portCLEAR_INTERRUPT_MASK_FROM_ISR(0);
}
#endif

#if 1
int convert_str_to_int(char *str)
{
    int result = 0;
    char *stop;
    result = strtol(str, &stop, 0);
    if (*stop != '\0') {
        return DUET_STR_TO_INT_ERR;
    }
    return result;
}
#else
int convert_str_to_int(char *str)
{
    int val = 0, size = 0;
    char *p = str;

    if (NULL == p) {
        return -1;
    }

    if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')) { // it is a hex value
        p += 2; // pass '0x'

        while (size < 8) {
            if ('0' <= *p && *p <= '9') {
                val = val * 16 + (*p - '0');
                size++;
            } else if ('a' <= *p && *p <= 'f') {
                val = val * 16 + (*p - 'a' + 10); // 'a' is 97
                size++;

            } else if ('A' <= *p && *p <= 'F') {
                val = val * 16 + (*p - 'A' + 10); // 'A' is 65
                size++;
            } else {
                break;
            }
            p++;

        }
    } else { // it is a dec. value
        while ('0' <= *p && *p <= '9') {
            val = val * 10 + (*p - '0');
            p++;
        }
    }
    return val;
}
#endif
// type = 1, val is uint8
// type = 2, val is uint16
// type = 4, val is uint32
void convert_int_to_str(unsigned int val, unsigned int type, char *ch)
{
    int i = 0, tempVal;

    if (ch == 0) {
        return;
    }

    while (i < (type << 1)) {
        tempVal = (val >> (((type << 1) - i - 1) << 2)) & 0xF;

        if (tempVal >= 0x0 && tempVal <= 0x9) {
            ch[i] = tempVal + '0'; // 0 to 9
        } else if ((tempVal >= 0xA && tempVal <= 0xF)) {
            ch[i] = tempVal - 0xA + 'A'; // a to f
        }
        i++;
    }
    ch[i] = '\0';
}

void duet_write32_bit(uint32_t reg, uint8_t start_bit, uint8_t len, uint32_t src_val)
{
    uint32_t tmp, mask, val;

    if ((start_bit < 32) && (len <= 32) && (src_val <= ((1 << len) - 1))) {
        tmp = REG_RD(reg);

        mask = (1 << len) - 1;      // eg: len=4, mask = 0xf, 1111
        mask = ~(mask << start_bit);

        val = tmp & mask;

        src_val = (src_val << start_bit);
        val = val | src_val;

        REG_WR(reg, val);
    } else {
        printf("write32_bit input parms not support \r\n");
    }
}

uint32_t duet_read32_bit(uint32_t reg, uint8_t start_bit, uint8_t len)
{
    uint32_t mask, val;

    if ((start_bit < 32) && (len <= 32)) {
        val = REG_RD(reg);

        mask = (1 << len) - 1;      // eg: len =4, 0xf,1111
        mask = mask << start_bit;
        val = val & mask;

        val = (val >> start_bit);

        return val;
    } else {
        printf("read32_bit input parms not support \r\n");
        return -1;
    }
}

/********************* end *********************/
