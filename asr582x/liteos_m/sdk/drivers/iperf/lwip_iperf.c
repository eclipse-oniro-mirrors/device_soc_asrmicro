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

#include "lwip_app_iperf.h"
#ifdef LWIP_APP_IPERF
#include <unistd.h>
#include <lwip/inet.h>
#include <string.h>
#include <stdlib.h>
#include "iperf_printf.h"
#include "lega_rtos.h"

#define IPERF_PORT 5001
#define IPERF_CLIENT_TIMEOUT_MS 0xFFFFFFFF

lega_semaphore_t iperf_Semaphore = NULL;
volatile IPERF_TCP_SERVER_STATUS iperf_tcp_server_status;
volatile IPERF_UDP_SERVER_STATUS iperf_udp_server_status;
volatile IPERF_TCP_CLINET_STATUS iperf_tcp_client_status;
volatile IPERF_UDP_CLIENT_STATUS iperf_udp_client_status;
char iperf_send_buf[SEND_BUFSIZE];
struct lwip_iperf_config_t iperf_config;
lega_timer_t iperf_output_timer;
struct lwip_iperf_outputInfo txperf_outinfo;
struct lwip_iperf_outputInfo rxperf_outinfo;
struct lwip_iperf_outputInfo utxperf_outinfo;
struct lwip_iperf_outputInfo urxperf_outinfo;

void iperf_value_format(char *format_value, int format_value_len, u64_t value)
{
    u64_t gv = value / (1000000000ULL);
    u64_t mv = value / (1000000ULL);
    u64_t kv = value / (1000ULL);

    lega_rtos_declare_critical();
    lega_rtos_enter_critical();

    if (gv == 0) {
        if (mv == 0) {
            if (kv == 0) {
                snprintf(format_value, format_value_len, "%llu", value);
            } else {
                snprintf(format_value, format_value_len, "%llu.%02uK", kv, (unsigned int)(value % 1000) / 10);
            }
        } else {
            snprintf(format_value, format_value_len, "%llu.%02uM", mv, (unsigned int)(value % 1000000) / 10000);
        }
    } else {
        snprintf(format_value, format_value_len, "%llu.%02uG", gv, (unsigned int)(value % 1000000000) / 10000000);
    }

    lega_rtos_exit_critical();
}

void do_iperf_terminate_timer(char *mode, struct lwip_iperf_outputInfo *outputInfo)
{
    u64_t bytes = outputInfo->currentByte;
    u64_t packets = outputInfo->currentPacketNum;
    char byteFormat[24] = {0};
    char bitFormat[24] = {0};
    lega_rtos_stop_timer(&iperf_output_timer);
    iperf_value_format(byteFormat, sizeof(byteFormat), bytes);
    iperf_value_format(bitFormat, sizeof(bitFormat), (bytes * (8ULL)) / ((u64_t)outputInfo->seconds));
    iperf_printf("%s:total %d sec    %llu packets    %s Bytes    %s bits/sec\r\n", mode, outputInfo->seconds, packets,
                 byteFormat, bitFormat);
    memset(outputInfo, 0, sizeof(struct lwip_iperf_outputInfo));
}
void do_iperf_terminate(char *mode, struct lwip_iperf_outputInfo *outputInfo, discon_handle_t discon)
{
    do_iperf_terminate_timer(mode, outputInfo);
    discon();
}

static void lega_wifi_iperf_usage()
{
    iperf_printf("usage: iperf -c host [-p port] [-u] [-t]\r\n\
aiperf -s [-p port] [-u] [-t]\r\n\
-c host     :run as iperf client connect to host\r\n\
-s          :run as iperf server\r\n\
-p port     :client connect to/server port default 5001\r\n\
-u          :use udp do iperf client/server\r\n\
             If -u not enable, use tcp default\r\n\
-t          :terminate iperf client/server/all\r\n\
-d          :tx packets delay time(ms) default 10\r\n");
}
void lega_wifi_iperf(int argc, char **argv)
{
    int c = 0;
    struct lwip_iperf_config_t temp_config = {0};

    temp_config.port = IPERF_PORT;
    temp_config.tx_delay_ms = 2;

    for (c = 1; c < argc; c++) {
        if (strcmp(argv[c], "-c") == 0) {
            temp_config.mode = IPERF_MODE_CLIENT;
            if (inet_aton(argv[++c], &(temp_config.ipaddr)) == 0) {
                goto IPERF_PARSE_FAIL;
            }
        } else if (strcmp(argv[c], "-p") == 0) {
            temp_config.port = strtoul(argv[++c], NULL, 10);
            if (temp_config.port <= 0 || temp_config.port > 65536) {
                goto IPERF_PARSE_FAIL;
            }
        } else if (strcmp(argv[c], "-s") == 0) {
            temp_config.mode = IPERF_MODE_SERVER;
        } else if (strcmp(argv[c], "-u") == 0) {
            temp_config.protocol = IPERF_PROTOCOL_UDP;
        } else if (strcmp(argv[c], "-t") == 0) {
            temp_config.termFlag = 1;
        } else if (strcmp(argv[c], "-d") == 0) {
            temp_config.tx_delay_ms = atoi(argv[c + 1]);
            c++;
        } else {
            goto IPERF_PARSE_FAIL;
        }
    }

    if ((temp_config.termFlag == 0) && (iperf_config.termFlag == 0)) {
        iperf_printf("please close your last iperf connection firstly!\n");
        goto IPERF_PARSE_FAIL;
    } else if ((temp_config.termFlag == 1) && (iperf_config.termFlag == 0)) {
        iperf_config.termFlag = 1;
    } else {
        memcpy(&iperf_config, &temp_config, sizeof(iperf_config));
    }

    if ((iperf_config.mode == IPERF_MODE_UNINIT) && (iperf_config.termFlag != 1)) {
        goto IPERF_PARSE_FAIL;
    }
    if ((iperf_config.mode == IPERF_MODE_CLIENT) && (iperf_Semaphore != NULL)) {
        if (lega_rtos_set_semaphore(&iperf_Semaphore) == kNoErr) {
            // iperf_printf("Begin to run iperf client:\n");
        } else {
            iperf_printf("Run iperf client fail for semaphore error\n");
        }
        return;
    }
    if (iperf_config.protocol == IPERF_PROTOCOL_TCP) {
        if (iperf_config.mode == IPERF_MODE_SERVER) {
            // tcp server
            if (iperf_config.termFlag == 1) {
                // iperf_printf("Terminate iperf tcp server");
                // terminate_rxperf();
                do_iperf_terminate("tcp server", &rxperf_outinfo, clear_rxperf);
            } else {
                if (iperf_tcp_server_status <= IPERF_TCP_SERVER_INIT) {
                    rxperf_init();
                    start_rxperf_application(iperf_config.port);
                } else {
                    iperf_printf("iperf tcp server already running status:%d,please terminate it before run again!\r\n",
                                 iperf_tcp_server_status);
                }
            }
        }
    } else {
        if (iperf_config.mode == IPERF_MODE_SERVER) {
            // udp server
            if (iperf_config.termFlag == 1) {

                do_iperf_terminate("udp server", &urxperf_outinfo, clear_urxperf);
            } else {
                if (iperf_udp_server_status <= IPERF_UDP_SERVER_INIT) {
                    iperf_udp_server_status = IPERF_UDP_SERVER_INIT;
                    urxperf_init();
                    start_urxperf_application(iperf_config.port);
                } else {
                    iperf_printf("iperf udp server already running status:%d,please terminate it before run again!\r\n",
                                 iperf_udp_server_status);
                }
            }
        }
    }
    return;
IPERF_PARSE_FAIL:
    lega_wifi_iperf_usage();
    return;
}

void do_iperf_output(char *mode, struct lwip_iperf_outputInfo *outputInfo)
{
    u64_t bytes = outputInfo->currentByte - outputInfo->lastByte;
    u64_t packets = outputInfo->currentPacketNum - outputInfo->lastPacketNum;
    char byteFormat[24] = {0};
    char bitFormat[24] = {0};
    iperf_value_format(byteFormat, sizeof(byteFormat), bytes);
    iperf_value_format(bitFormat, sizeof(bitFormat), bytes * (8ULL));
    outputInfo->seconds++;
    if (bytes != 0) {
        iperf_printf("%s:%d-%d sec    %llu packets    %s Bytes    %s bits/sec\r\n", mode, outputInfo->seconds - 1,
                     outputInfo->seconds, packets, byteFormat, bitFormat);
    }
    outputInfo->lastByte = outputInfo->currentByte;
    outputInfo->lastPacketNum = outputInfo->currentPacketNum;
}

void iperf_output(void *args)
{
    printf("\nlalala %s\n", __func__);
    if (iperf_tcp_client_status == IPERF_TCP_CLIENT_TXRUNNING) {
        do_iperf_output("tcp client", &txperf_outinfo);
    }
    if (iperf_tcp_server_status == IPERF_TCP_SERVER_RXRUNNING) {
        do_iperf_output("tcp server", &rxperf_outinfo);
    }
    if (iperf_udp_client_status == IPERF_UDP_CLIENT_TXRUNNING) {
        do_iperf_output("udp client", &utxperf_outinfo);
    }
    if (iperf_udp_server_status == IPERF_UDP_SERVER_RXRUNNING) {
        do_iperf_output("udp server", &urxperf_outinfo);
    }

}
void lega_wifi_iperf_client_start()
{
    int count = 0;
    if (lega_rtos_init_timer(&iperf_output_timer, IPERF_OUTPUT_INTERVIEW * 1000, iperf_output, NULL) != kNoErr) {
        iperf_printf("iperf timer fail!\r\n");
    }
    for (;;) {
        if (lega_rtos_get_semaphore(&iperf_Semaphore, IPERF_CLIENT_TIMEOUT_MS) == kNoErr) {
            // AT command set lwip_iperf_config over and begin to run

            if (iperf_config.mode != IPERF_MODE_CLIENT) {
                continue;
            }
            if (iperf_config.protocol == IPERF_PROTOCOL_TCP) {
                if (iperf_config.termFlag == 1) {
                    do_iperf_terminate("tcp client", &txperf_outinfo, disconnect_txperf);
                    continue;
                }

                if (iperf_tcp_client_status != IPERF_TCP_CLIENT_TXRUNNING) {
                    if (iperf_tcp_client_status >= IPERF_TCP_CLIENT_CONNECTING) {
                        iperf_printf("iperf tcp client already running, please terminate it before run again!\r\n");
                        continue;
                    }
                    txperf_init();
                    if (start_txperf_application(&(iperf_config.ipaddr), iperf_config.port) != 0) {
                        iperf_printf("iperf tcp connect fail!\n");
                        continue;
                    }
                    count = 0;
                    if (iperf_tcp_client_status == IPERF_TCP_CLIENT_TXRUNNING) {
                        break;
                    }
                    while (iperf_tcp_client_status < IPERF_TCP_CLIENT_STARTING) { //wait for tcp connect
                        lega_rtos_delay_milliseconds(100);
                        if (++count > 100) {
                            iperf_printf("iperf connect to %s:%d fail!\n", ip4addr_ntoa((const ip4_addr_t *) & (iperf_config.ipaddr)),
                                         iperf_config.port);
                            break;
                        }
                    }
                    if (count > 100) {
                        continue;
                    }
                    iperf_tcp_client_status = IPERF_TCP_CLIENT_TXRUNNING;
                }

                transfer_txperf_data();
            } else if (iperf_config.protocol == IPERF_PROTOCOL_UDP) {
                if (iperf_config.termFlag == 1) {
                    do_iperf_terminate("udp client", &utxperf_outinfo, disconnect_utxperf);
                    continue;
                }
                if (iperf_udp_client_status >= IPERF_UDP_CLIENT_START) {
                    iperf_printf("iperf udp client already running status:%d,please terminate it before run again!\r\n",
                                 iperf_udp_client_status);
                    continue;
                }
                utxperf_init();
                if (start_utxperf_application(&(iperf_config.ipaddr), iperf_config.port) != 0) {
                    continue;
                }
                while (transfer_utxperf_data() != -2) {
                    lega_rtos_delay_milliseconds(iperf_config.tx_delay_ms);
                    if (iperf_config.termFlag) {
                        break;
                    }
                }
                if (iperf_config.termFlag) {
                    continue;
                }
            }
        }
    }
}

lega_thread_t iper_handler = NULL;
#define     IPERF_CLIENT_THREAD_NAME    "iperf-client"
#define     IPERF_CLIENT_PRIORITY       (28)
#define     IPERF_CLIENT_STACK_SIZE     4096
void lega_wifi_iperf_init(void)
{
    int i = 0;

    rxperf_init();

    urxperf_init();
    utxperf_init();

    if (iperf_Semaphore == NULL) {
        lega_rtos_init_semaphore(&iperf_Semaphore, 0);
    }

    /* initialize data buffer being sent */
    for (i = 0; i < SEND_BUFSIZE; i++) {
        iperf_send_buf[i] = (i % 10) + '0';
    }

    memset(&iperf_config, 0, sizeof(iperf_config));
    iperf_config.termFlag = 1;

    lega_rtos_create_thread(&iper_handler, IPERF_CLIENT_PRIORITY, IPERF_CLIENT_THREAD_NAME, lega_wifi_iperf_client_start,
                            IPERF_CLIENT_STACK_SIZE, 0);
}
#endif
