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
#include "app.h"
#ifdef ALIOS_SUPPORT
#include "aos/cli.h"
#endif
#include "duet_gpio.h"
#include "msm_ble_api.h"

static uint8_t string_asc_to_hex(char param)
{
    uint8_t val;

    if ((param >= '0') && (param <= '9')) {
        val = param - '0';
    } else if ((param >= 'A') && (param <= 'F')) {
        val = (uint8_t)(param - 'A') + 0xA;
    } else if ((param >= 'a') && (param <= 'f')) {
        val = (uint8_t)(param - 'a')  + 0xA;
    } else {
        val = 0xA;
    }
    return val;
}

static void sonata_string_to_array(char *input_str, uint8_t input_length, uint8_t *hex_addr)
{
    int i;
    input_length = (input_length + 1) & 0xFFFE;
    for (i = 0; i < input_length; i++) {
        if (i % 2 || i == 1) {
            hex_addr[i >> 1] = string_asc_to_hex(input_str[i]) | (hex_addr[i >> 1] & 0xF0);
        } else {
            hex_addr[i >> 1] = (string_asc_to_hex(input_str[i]) << 4) | (hex_addr[i >> 1] & 0xF);
        }
    }
}

duet_gpio_dev_t  g_duet_gpio11;
duet_gpio_dev_t  g_duet_gpio12;
duet_gpio_dev_t  g_duet_gpio6;

void lega_ble_adv(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;

    if (strcmp(argv[c], "start") == 0 && argc > 2) {

        printf("adv start!!!!!!!!!!! \r\n");
        uint8_t hex_value[32];
        memset(hex_value, 0, sizeof(hex_value));
        uint16_t hex_length = (strlen(argv[c + 1]) + 1) / 2;
        sonata_string_to_array(argv[c + 1], strlen(argv[c + 1]), hex_value);
        duet_gpio_output_toggle(&g_duet_gpio11);
        duet_gpio_output_toggle(&g_duet_gpio12);
        duet_gpio_output_toggle(&g_duet_gpio6);
#ifdef HARMONYOS_TEMP
#else
        ms_test_ble_adv_start(hex_value, hex_length, NULL, 0);
#endif
    } else if (strcmp(argv[c], "stop") == 0) {
        app_ble_advertising_stop(0);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command adv_cmd = {
    .name     = "adv",
    .help     = "adv start|stop [adv data] [rsp data]",
    .function = lega_ble_adv,
};
#endif

void lega_ble_scan(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 0;

    for (c = 1; c < argc; c++) {
        if (strcmp(argv[c], "start") == 0) {
            sonata_api_app_timer_set(8, 20);
            sonata_api_app_timer_active(8);
        } else if (strcmp(argv[c], "stop") == 0) {
            app_ble_stop_scanning();
        }
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command scan_cmd = {
    .name     = "scan",
    .help     = "scan start|stop",
    .function = lega_ble_scan,
};
#endif

void lega_ble_adv_open_close_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "start") == 0) {
        app_ble_set_test_count(200);
        sonata_api_app_timer_set(6, 20);
        sonata_api_app_timer_active(6);
    } else if (strcmp(argv[c], "stop") == 0) {
        app_ble_set_test_count(0);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command adv_per_cmd = {
    .name     = "adv-per",
    .help     = "adv-per start|stop [adv data] ",
    .function = lega_ble_adv_open_close_test,
};
#endif

void lega_ble_adv_update_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "start") == 0) {
        app_ble_set_test_count(200);
        sonata_api_app_timer_set(7, 20);
        sonata_api_app_timer_active(7);
    } else if (strcmp(argv[c], "stop") == 0) {
        app_ble_set_test_count(0);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command update_cmd = {
    .name     = "adv-update",
    .help     = "adv-update start|stop  ",
    .function = lega_ble_adv_update_test,
};
#endif

void lega_ble_connect_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "start") == 0) {
        uint8_t hex_value[6];
        sonata_string_to_array(argv[c + 1], strlen(argv[c + 1]), hex_value);
        app_ble_set_test_target(hex_value);
        sonata_api_app_timer_set(3, 20);
        sonata_api_app_timer_active(3);

    } else {
        sonata_ble_stop_initiating();
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command connect_cmd = {
    .name     = "connect",
    .help     = "connect start|stop target_addr",
    .function = lega_ble_connect_test,
};
#endif

void lega_ble_write_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "start") == 0) {
        sonata_api_app_timer_set(4, 20);
        sonata_api_app_timer_active(4);
        app_ble_set_test_interval(atoi(argv[c + 1]));

    } else {
        sonata_api_app_timer_clear(4);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command write_cmd = {
    .name     = "write",
    .help     = "write start| stop {interval}",
    .function = lega_ble_write_test,
};
#endif

void lega_ble_disconnect_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (argc == 2) {
        app_ble_disconnect(0);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command disconnect_cmd = {
    .name     = "disconnect",
    .help     = "disconnect target_addr",
    .function = lega_ble_disconnect_test,
};
#endif

void lega_ble_test_mode(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    if (strcmp(argv[c], "on") == 0) {
        printf("ble test on \r\n");
        // gpio1 output
        g_duet_gpio11.port = GPIO11_INDEX;
        g_duet_gpio11.config = DUET_OUTPUT_PUSH_PULL;
        g_duet_gpio11.priv = NULL;
        duet_gpio_init(&g_duet_gpio11);
        duet_gpio_output_low(&g_duet_gpio11);

        g_duet_gpio6.port = GPIO6_INDEX;
        g_duet_gpio6.config = DUET_OUTPUT_PUSH_PULL;
        g_duet_gpio6.priv = NULL;
        duet_gpio_init(&g_duet_gpio6);
        duet_gpio_output_low(&g_duet_gpio6);

        g_duet_gpio12.port = GPIO12_INDEX;
        g_duet_gpio12.config = DUET_OUTPUT_PUSH_PULL;
        g_duet_gpio12.priv = NULL;
        duet_gpio_init(&g_duet_gpio12);
        duet_gpio_output_low(&g_duet_gpio12);

        duet_gpio_output_low(&g_duet_gpio11);

        app_set_ble_test_mode(true);
    } else {
        app_set_ble_test_mode(false);
    }
}

#ifdef ALIOS_SUPPORT
static struct cli_command test_cmd = {
    .name     = "ble-test-mode",
    .help     = "ble-test-mode {on|off}",
    .function = lega_ble_test_mode,
};
#endif

void lega_ble_adv_state(char *pwbuf, int blen, int argc, char **argv)
{
    extern void app_print_adv_status(void);
    app_print_adv_status();
    sonata_api_app_timer_set(5, 500);
    sonata_api_app_timer_active(5);
}

#ifdef ALIOS_SUPPORT
static struct cli_command adv_state = {
    .name     = "printf-adv-state",
    .help     = "printf-adv-state",
    .function = lega_ble_adv_state,
};
#endif

void lega_ble_set_uuid(char *pwbuf, int blen, int argc, char **argv)
{
    int c = 1;
    uint8_t hex_value[6];
    sonata_string_to_array(argv[c], strlen(argv[c]), hex_value);
    app_ble_set_test_write_uuid(hex_value);
    app_ble_set_test_read_uuid(hex_value);

    ble_set_max_mtu(200);
}

#ifdef ALIOS_SUPPORT
static struct cli_command set_test_uuid = {
    .name     = "test-uuid",
    .help     = "test-uuid {uuid}",
    .function = lega_ble_set_uuid,
};
#endif

void lega_ble_add_test_service(char *pwbuf, int blen, int argc, char **argv)
{
#ifdef HARMONYOS_TEMP
#else
    ms_add_test_service();
#endif
}

#ifdef ALIOS_SUPPORT
static struct cli_command add_test_service = {
    .name     = "add-test-service",
    .help     = "add-test-service",
    .function = lega_ble_add_test_service,
};

void lega_ble_at_init(void)
{
    aos_cli_register_command(&scan_cmd);
    aos_cli_register_command(&adv_cmd);
    aos_cli_register_command(&adv_per_cmd);
    aos_cli_register_command(&update_cmd);
    aos_cli_register_command(&connect_cmd);
    aos_cli_register_command(&write_cmd);
    aos_cli_register_command(&disconnect_cmd);
    aos_cli_register_command(&test_cmd);
    aos_cli_register_command(&adv_state);
    aos_cli_register_command(&set_test_uuid);
    aos_cli_register_command(&add_test_service);
}
#endif
