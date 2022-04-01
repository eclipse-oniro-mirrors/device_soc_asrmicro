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

#include "lega_dbg.h"

uint32_t  GlobalDebugComponents = 0xFFFFFFFF;
uint32_t  GlobalDebugLevel = D_ERR;
uint8_t   GlobalDebugEn = 1;

char uwifi_m[6] = "uwifi";
char lwifi_m[6] = "lwifi";
char lwip_m[5]  = "Lwip";
char os_m[3]    = "OS";
char at_m[3]    = "AT";
char other_m[6] = "Other";

char *component_info(uint32_t module_t)
{
    if (module_t &LWIFI_COMP) {
        return lwifi_m;
    } else if (module_t &UWIFI_COMP) {
        return uwifi_m;
    } else if (module_t &OS_COMP) {
        return os_m;
    } else if (module_t &LWIP_COMP) {
        return lwip_m;
    } else if (module_t &AT_COMP) {
        return at_m;
    }

    return other_m;
}
