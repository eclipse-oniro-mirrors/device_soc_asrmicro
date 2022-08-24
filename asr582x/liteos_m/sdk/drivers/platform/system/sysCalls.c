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

#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "duet_cm4.h"
#include "printf_uart.h"

#define DEC_BASE 10
#define ITOA_BASE 3

char *itoa(char *p, unsigned x)
{
    unsigned tmp_x = x;
    char *tmp_p = p;
    tmp_p += ITOA_BASE * sizeof(int);
    *--tmp_p = 0;
    do {
        *--tmp_p = '0' + tmp_x % DEC_BASE;
        tmp_x /= DEC_BASE;
    } while (tmp_x);
    return tmp_p;
}

const char *__locale_ctype_ptr (void)
{
    return "1.0.0-LiteOS_M";
}

int sscanf(const char *__restrict __s, const char *__restrict __format, ...)
{
    va_list args;
    int i;

    va_start(args, __format);
    i = sys_vsscanf(__s, __format, args);
    va_end(args);

    return i;
}
