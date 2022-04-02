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
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "at_api.h"
#ifdef ALIOS_SUPPORT
#include "aos/cli.h"
#endif
#include "sonata_ble_api.h"
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif

#ifdef CFG_MRFOTA_TEST
uint8_t rfota_ble_open = 0;
extern uint8_t rfota_wifi_open;

extern uint8_t rfota_ble_test;

static void lega_at_ble_start(char *pwbuf, int blen, int argc, char **argv)
{
    if (rfota_wifi_open > 0) {
        printf("please close wifi before test ble!");
        return;
    }

    if (rfota_ble_open == 0) {
        lega_rtos_delay_milliseconds(1000);
        extern void hci_uart_deinit(void);
        hci_uart_deinit();
        init_ble_task();
        rfota_ble_open = 0x1;
    } else {
        printf("ble is alread open!\n");
    }
    return 0;

}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_ble_start = {
    .name     = "rfota_ble_start",
    .help     = "rfota_ble_start",
    .function = lega_at_ble_start,
};
#endif
static void lega_at_ble_stop(char *pwbuf, int blen, int argc, char **argv)
{
    if (rfota_ble_open == 0x1) {
        rfota_ble_open = 0;
        ble_close();
    } else if (rfota_ble_open == 0) {
        printf("ble don't need close!\n");
    }

    return 0;
}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_ble_stop = {
    .name     = "rfota_ble_stop",
    .help     = "rfota_ble_stop for hci test",
    .function = lega_at_ble_stop,
};
#endif
static void lega_enable_rfota_ble_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "on") == 0) {
        rfota_ble_test = 1;
        printf("rfota ble test begin\n");
    } else if (strcmp(argv[c], "off") == 0) {
        rfota_ble_test = 0;
        printf("rfota ble test end\n");
    }
}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_ble_cmd = {
    .name     = "rfota_ble_test",
    .help     = "rfota_ble_test on/off",
    .function = lega_enable_rfota_ble_test,
};

void lega_rfota_ble_test_at_init(void)
{
    aos_cli_register_command(&rfota_ble_cmd);
    aos_cli_register_command(&rfota_ble_start);
    aos_cli_register_command(&rfota_ble_stop);
}
#endif
#endif
