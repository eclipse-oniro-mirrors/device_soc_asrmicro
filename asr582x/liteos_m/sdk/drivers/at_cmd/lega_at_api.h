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
#ifndef _LEGA_AT_API_H_
#define _LEGA_AT_API_H_
#ifdef CFG_PLF_DUET
#include "duet_uart.h"
#else
#include "lega_uart.h"
#endif

#include <stdio.h>
#ifndef LWIP_IGNORE
#include "lwip/ip_addr.h"
#endif
#include "target_config.h"
#if (LOSCFG_KERNEL_PRINTF != 0)
#define TASKLIST_SUPPORT
#define HEAPSHOW_SUPPORT
// LOS_TaskInfoMonitor
#endif

#define LEGA_AT_VERSION        "AT-V2.1.3"

#define     AT_TASK_NAME                "AT_task"
#define     AT_TASK_PRIORITY            (20)
#define     AT_TASK_STACK_SIZE          2048

typedef struct _cmd_entry {
    char *name;
    int (*function)(int, char **);
    char *help;
} cmd_entry;

#define AT_MAX_COMMANDS           90
struct cli_cmd_t {
    int cmds_num;
    cmd_entry *cmds[AT_MAX_COMMANDS];
};

typedef struct {
    uint8_t   uart_echo;       /* echo uart input info log */
    uint8_t   max_txpwr;       /* max tx power for both sta and softap */
    uint8_t   flag_sap;        /* flag of user set softap ip config */
    uint8_t   flag_sta;        /* flag of user set sta ip config */
    uint8_t   dis_dhcp;        /* disable dhcp func, use static ip */
    uint8_t   at_scan;         /* scan flag which indicate call by at task */
    uint8_t   sta_connected;   /* indicate status of station is connected */
    uint8_t   sap_opend;       /* indicate status of softap is open done */
#ifndef LWIP_IGNORE
    ip_addr_t at_ping;         /* save ping ip addr for at cmd */
#endif
    char      staip[16];       /* Local IP address on the target wlan interface for station mode, ASCII */
    char      stagw[16];       /* Router IP address on the target wlan interface for station mode, ASCII */
    char      stamask[16];     /* Netmask on the target wlan interface for station mode, ASCII */
    char      sapip[16];       /* Local IP address on the target wlan interface for softap mode, ASCII */
    char      sapgw[16];       /* Router IP address on the target wlan interface for softap mode, ASCII */
    char      sapmask[16];     /* Netmask on the target wlan interface for softap mode, ASCII */
    char      start_ip[16];    /* start ip addr of dhcp pool in softap mode */
    char      end_ip[16];      /* end ip addr of dhcp pool in softap mode */
} _at_user_info;

typedef enum {
    CONFIG_OK,          /* indicate at cmd set success and response OK */
    PARAM_RANGE,        /* indicate some at cmd param is out of range */
    PARAM_MISS,         /* indicate at cmd param is less than needed count */
    CONFIG_FAIL,        /* indicate at cmd set failed, or execute fail */
    CONN_TIMEOUT,       /* indicate connect timeout in station mode */
    CONN_EAPOL_FAIL,    /* indicate 4-way handshake failed in station mode */
    CONN_DHCP_FAIL,     /* indicate got ip by dhcp failed in station mode */
    WAIT_PEER_RSP,
    RSP_NULL = 0xFF
} lega_at_rsp_status_t;

typedef struct {
    char *command;  /* at cmd string */
    int (*function)(int argc, char **argv); /* at cmd proccess function */
} lega_at_cmd_entry;

#ifdef CFG_SDIO_SUPPORT
typedef void (*lega_at_sdio_host_tx_hdlr)(uint8_t *data, uint32_t data_len);
typedef struct {
    uint32_t sdio_data_len;
    uint32_t p_sdio_at_cmd;
} at_msg_t;
#endif

/** @brief  register user at cmd.
 *
 * @param cmd_entry    : user at cmd array pointer
 * @param cmd_num      : user at cmd number
 */
void lega_at_cmd_register(cmd_entry *cmd);

/** @brief  at init functin, user should call it before use at cmd
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_at_init(const char *task_name, uint8_t task_pri, uint32_t task_stack_size);

/** @brief  at deinit functin, user should call it when donot use at any more, to free resources
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_at_deinit(void);

/** @brief  at command callback function, used to register to uart.
 */
void at_handle_uartirq(char ch);

/** @brief  uart handle for receiving at command.
 */

#ifdef CFG_PLF_DUET
extern duet_uart_dev_t lega_at_uart;
#else
extern lega_uart_dev_t lega_at_uart;
#endif

/** @brief  at register init functin, register all support cmd and user register cmd
 */
void lega_at_cmd_register_all(void);

/** @brief  register user cmd
 */
void lega_at_user_cmd_register(void);

/** @brief  at response, OK indicate success, others indicate failed.
 */
void lega_at_response(lega_at_rsp_status_t status);

extern char at_dbgflg;

#ifdef PRINTF2_SUPPORT
extern int printf2(const char *format, ...);
#else
#define printf2 printf
#endif

#define dbg_at(Fmt, ...)         do {if (at_dbgflg) printf2(Fmt "\r\n", ## __VA_ARGS__);} while (0)
#define dbg_atnn(Fmt, ...)       do {if (at_dbgflg) printf2(Fmt, ## __VA_ARGS__);} while (0)
#define at_rspdata(Fmt, ...)     printf2("+" Fmt "\r\n", ## __VA_ARGS__)
#define at_rspdatann(Fmt, ...)   printf2("+" Fmt, ## __VA_ARGS__)
#define at_rspinfor(Fmt, ...)    printf2(Fmt "\r\n", ## __VA_ARGS__)
#define at_rspinfornn(Fmt, ...)  printf2(Fmt, ## __VA_ARGS__)

#endif  // _LEGA_AT_API_H_

