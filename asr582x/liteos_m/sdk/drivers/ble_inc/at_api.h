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

/**
 ****************************************************************************************
 *
 * @file lega_at_api.h
 *
 * @brief AT API.
 *
 ****************************************************************************************
 */
#ifndef _AT_API_H_
#define _AT_API_H_

#include <stdio.h>
#include "stdio.h"
#include "sonata_ble_hook.h"

#define SONATA_AT_VERSION        "AT-V0.9.1"
#define SONATA_SDK_VERSION       "SDK-V1.0.10"
#define SONATA_APP_VERSION       "APP-V1.0.10"

typedef struct _cmd_entry {
    char *name;
    int (*function)(int, char **);
    char *help;
} cmd_entry;

typedef struct {
    uint8_t  uart_echo;       /* echo uart input info log */
    uint8_t  max_txpwr;       /* max tx power for both sta and softap */
    uint8_t  flag_sap;        /* flag of user set softap ip config */
    uint8_t  flag_sta;        /* flag of user set sta ip config */
    uint8_t  dis_dhcp;        /* disable dhcp func, use static ip */
    uint8_t  at_scan;         /* scan flag which indicate call by at task */
    uint8_t  sta_connected;   /* indicate status of station is connected */
    uint8_t  sap_opend;       /* indicate status of softap is open done */
    // ip_addr_t at_ping;        /* save ping ip addr for at cmd */
    char     ip[16];          /* Local IP address on the target wlan interface, ASCII */
    char     gw[16];          /* Router IP address on the target wlan interface, ASCII */
    char     mask[16];        /* Netmask on the target wlan interface, ASCII */
} at_user_info_s;

typedef enum {
    CONFIG_OK,          /* indicate at cmd set success and response OK */
    PARAM_RANGE,        /* indicate some at cmd param is out of range */
    PARAM_MISS,         /* indicate at cmd param is less than needed count */
    CONFIG_FAIL,        /* indicate at cmd set failed, or execute fail */
    CONN_TIMEOUT,       /* indicate connect timeout in station mode */
    CONN_EAPOL_FAIL,    /* indicate 4-way handshake failed in station mode */
    CONN_DHCP_FAIL,     /* indicate got ip by dhcp failed in station mode */
    RSP_NULL = 0xFF
} at_rsp_status_t;

typedef struct {
    char *command;  /* at cmd string */
    int (*function)(int argc, char **argv); /* at cmd proccess function */
} at_cmd_entry;
int at_init(void);
void at_command_process_ble(void);
/** @brief  register user at cmd.
 *
 * @param cmd_entry    : user at cmd array pointer
 * @param cmd_num      : user at cmd number
 */
void at_cmd_register(const cmd_entry *cmd);

/** @brief  at init functin, user should call it before use at cmd
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_at_init(const char *task_name, uint8_t task_pri, uint32_t task_stack_size);

/** @brief  at deinit functin, user should call it when donot use at any more, to free resources
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int at_deinit(void);

/** @brief  at command callback function, used to register to uart.
 */
void at_handle_uartirq(char ch);

/** @brief  uart handle for receiving at command.
 */

/** @brief  at register init functin, register all support cmd and user register cmd
 */
void at_cmd_register_all(void);

/** @brief  register user cmd
 */
void at_user_cmd_register(void);
void at_gapc_cmd_register(void);
void at_gapm_cmd_register(void);
void at_gattc_cmd_register(void);

/** @brief  at response, OK indicate success, others indicate failed.
 */
void at_response(at_rsp_status_t status);
extern int at_printf(const char *format, ...);

extern char at_dbgflg;

#define at_printf printf

#define dbg_at(Fmt, ...)         do {if (at_dbgflg) at_printf(Fmt "\r\n", ## __VA_ARGS__);} while (0)
#define dbg_atnn(Fmt, ...)       do {if (at_dbgflg) at_printf(Fmt, ## __VA_ARGS__);} while (0)

#define at_rspdata(Fmt, ...)     at_printf("+" Fmt "\r\n", ## __VA_ARGS__)
#define at_rspdatann(Fmt, ...)   at_printf("+" Fmt, ## __VA_ARGS__)
#define at_rspinfor(Fmt, ...)    at_printf(Fmt "\r\n", ## __VA_ARGS__)
#define at_rspinfornn(Fmt, ...)  at_printf(Fmt, ## __VA_ARGS__)

#endif  // _AT_API_H_

