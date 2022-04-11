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

#ifndef __LWIP_APP_IPERF_H__
#define __LWIP_APP_IPERF_H__

#include "lwipopts.h"
#ifdef LWIP_APP_IPERF
#include "lwip/ip_addr.h"
#include "lwip/inet.h"
#include "lega_rtos.h"
#define TCP_MAX_TXPERF_CONNECTION 1
#define IPERF_OUTPUT_INTERVIEW 1 // 1s
#define TCP_SEND_BUFSIZE (TCP_MSS)
#define UDP_SEND_BUFSIZE (TCP_MSS+12)
#define SEND_BUFSIZE UDP_SEND_BUFSIZE // (TCP_SEND_BUFSIZE>UDP_SEND_BUFSIZE?TCP_SEND_BUFSIZE:UDP_SEND_BUFSIZE)
typedef unsigned long long   u64_t;

extern char iperf_send_buf[SEND_BUFSIZE];
extern lega_timer_t iperf_output_timer;
typedef enum {
    IPERF_MODE_UNINIT = 0,
    IPERF_MODE_SERVER,
    IPERF_MODE_CLIENT
} IPERF_MODE;

typedef enum {
    IPERF_PROTOCOL_TCP = 0,
    IPERF_PROTOCOL_UDP
} IPERF_PROTOCOL;

typedef enum {
    IPERF_TCP_SERVER_UNINIT = 0,
    IPERF_TCP_SERVER_INIT,
    IPERF_TCP_SERVER_LISTENING,
    IPERF_TCP_SERVER_RXRUNNING
} IPERF_TCP_SERVER_STATUS;
typedef enum {
    IPERF_UDP_SERVER_UNINIT = 0,
    IPERF_UDP_SERVER_INIT,
    IPERF_UDP_SERVER_RXRUNNING
} IPERF_UDP_SERVER_STATUS;
typedef enum {
    IPERF_TCP_CLINET_UNINIT = 0,
    IPERF_TCP_CLIENT_INIT,
    IPERF_TCP_CLIENT_CONNECTING,
    IPERF_TCP_CLIENT_CONNECTED,
    IPERF_TCP_CLIENT_STARTING,
    IPERF_TCP_CLIENT_TXRUNNING,
} IPERF_TCP_CLINET_STATUS;
typedef enum {
    IPERF_UDP_CLIENT_UNINIT = 0,
    IPERF_UDP_CLIENT_INIT,
    IPERF_UDP_CLIENT_START,
    IPERF_UDP_CLIENT_TXRUNNING
} IPERF_UDP_CLIENT_STATUS;
typedef void (*discon_handle_t)();
extern volatile IPERF_TCP_SERVER_STATUS iperf_tcp_server_status;
extern volatile IPERF_UDP_SERVER_STATUS iperf_udp_server_status;
extern volatile IPERF_TCP_CLINET_STATUS iperf_tcp_client_status;
extern volatile IPERF_UDP_CLIENT_STATUS iperf_udp_client_status;
// extern volatile int iperf_tcp_server_term;
// extern volatile int iperf_udp_server_term;
// extern volatile int iperf_tcp_client_running;
// extern volatile int iperf_udp_client_term;

struct lwip_iperf_outputInfo {
    int seconds; // start time
    u64_t lastByte;
    u64_t currentByte;
    u64_t lastPacketNum;
    u64_t currentPacketNum;
};
struct lwip_iperf_config_t {
    int termFlag;
    IPERF_MODE mode;
    IPERF_PROTOCOL protocol;
    int port;
    ip4_addr_t ipaddr;
    u8_t tx_delay_ms;
};
extern struct lwip_iperf_config_t iperf_config;
extern struct lwip_iperf_outputInfo txperf_outinfo;
extern struct lwip_iperf_outputInfo rxperf_outinfo;
extern struct lwip_iperf_outputInfo utxperf_outinfo;
extern struct lwip_iperf_outputInfo urxperf_outinfo;

void lega_wifi_iperf(int argc, char **argv);
void lega_wifi_iperf_init(void);

void rxperf_init();
void txperf_init();
void urxperf_init();
void utxperf_init();
void txperf_output();

int start_txperf_application(ip4_addr_t *ipaddr, int port);
int transfer_txperf_data();
int start_utxperf_application(ip4_addr_t *ipaddr, int port);
int transfer_utxperf_data();
int start_rxperf_application(int port);
int start_urxperf_application(int port);
void clear_rxperf();
void disconnect_txperf();
void clear_urxperf();
void disconnect_utxperf();
void do_iperf_terminate_timer(char *mode, struct lwip_iperf_outputInfo *outputInfo);
#endif
#endif
