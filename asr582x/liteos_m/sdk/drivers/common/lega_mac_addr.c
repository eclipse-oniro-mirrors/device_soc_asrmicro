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
#include <stdlib.h>
#include <stdio.h>
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif
#include "duet_common.h"

int get_random_int_c()
{
    int i;
    srand(lega_rtos_get_time());
    i = rand();
    return i;
}

/**
  * @brief    if no mac address burned in efuse memory,
  *           user could modify this function to set default mac address
  *
  * @param    mac_addr : mac_addr pointer wifi stack used
  */
void lega_wlan_default_mac_addr(uint8_t *mac_addr)
{
    mac_addr[0] = 0x8C;
    mac_addr[1] = 0x59;
    mac_addr[2] = 0xDC;
    mac_addr[3] = (uint8_t)((get_random_int_c()>>16)&0xff);
    mac_addr[4] = (uint8_t)((get_random_int_c()>>8)&0xff);
    mac_addr[5] = (uint8_t)((get_random_int_c()>>0)&0xff);
}

void lega_rf_set_xo_freq_bias(void)
{
#ifdef XO_FREQ_BIAS_CONFIG
    //XO freq bias for blue board
    REG_WR(0x40000A74, 0x7171);
#endif
    printf("Current xo config is 0x%x\r\n",(unsigned int)REG_RD(0x40000A74));
}