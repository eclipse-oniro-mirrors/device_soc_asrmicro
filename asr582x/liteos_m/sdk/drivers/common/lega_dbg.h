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

#ifndef _LEGA_DBG_H_
#define _LEGA_DBG_H_

#include <stdio.h>
#include "lega_types.h"

#define D_CRT       1
#define D_ERR       2
#define D_WRN       3
#define D_INF       4
#define D_DBG       5

#define D_LWIFI_KE        BIT(0)
#define D_LWIFI_DMA       BIT(1)
#define D_LWIFI_MM        BIT(2)
#define D_LWIFI_TX        BIT(3)
#define D_LWIFI_RX        BIT(4)
#define D_LWIFI_PHY       BIT(5)
#define D_LWIFI           BIT(6)

#define D_UWIFI_DATA      BIT(11)
#define D_UWIFI_CTRL      BIT(12)
#define D_UWIFI_SUPP      BIT(13)

#define D_OS              BIT(14)
#define D_LWIP            BIT(15)

#define D_AT              BIT(16)

#define LWIFI_COMP        (D_LWIFI_KE | D_LWIFI_DMA | D_LWIFI_MM | \
                           D_LWIFI_TX | D_LWIFI_RX | D_LWIFI_PHY | D_LWIFI)
#define UWIFI_COMP        (D_UWIFI_DATA | D_UWIFI_CTRL | D_UWIFI_SUPP)
#define OS_COMP           (D_OS)
#define LWIP_COMP         (D_LWIP)
#define AT_COMP           (D_AT)

extern uint32_t  GlobalDebugComponents;
extern uint32_t  GlobalDebugLevel;
extern uint8_t   GlobalDebugEn;
extern uint32_t lega_rtos_get_time(void);
extern char *component_info(uint32_t module_t);

#define component_t(module_t) component_info(module_t)

#define dbg(Level, Comp, Fmt, ...) \
    do {\
        if(GlobalDebugEn && (Comp & GlobalDebugComponents) && (Level <= GlobalDebugLevel)) {\
            printf("[%u] ",(unsigned int)lega_rtos_get_time());\
            printf(Fmt "\r\n", ## __VA_ARGS__);\
        }\
    } while (0)

#endif // _LEGA_DBG_H_
