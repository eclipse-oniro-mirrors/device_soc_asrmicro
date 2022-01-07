#include "lwip_app_iperf.h"
#ifdef LWIP_APP_IPERF
#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#ifndef __PPC__
#include "iperf_printf.h"
#endif

static struct tcp_pcb *connect_pcb = NULL;

void txperf_init()
{
    iperf_tcp_client_status=IPERF_TCP_CLIENT_INIT;
    memset(&txperf_outinfo,0,sizeof(txperf_outinfo));
}

void disconnect_txperf()
{
    if(connect_pcb){
        tcp_close(connect_pcb);
        connect_pcb=NULL;
    }
    iperf_printf("tcp client:iperf terminate and disconnect\r\n");
    iperf_tcp_client_status=IPERF_TCP_CLIENT_INIT;
}

int transfer_txperf_data()
{
    err_t err;
    struct tcp_pcb *tpcb = connect_pcb;

    if(iperf_tcp_client_status<IPERF_TCP_CLIENT_STARTING){
 //       iperf_printf("txperf: stoped for invalid status %d\n",iperf_tcp_client_status);
        goto TX_PERF_TERM;
    }
    if (!connect_pcb){
        iperf_printf("txperf: Error connected_pcb null\n");
        return -1;
    }
    while(tcp_sndbuf(tpcb)<=TCP_SEND_BUFSIZE){
        lega_rtos_delay_milliseconds(iperf_config.tx_delay_ms);//wait for send buf ready
        if(iperf_tcp_client_status<IPERF_TCP_CLIENT_STARTING)
            goto TX_PERF_TERM;
    }
    while (tcp_sndbuf(tpcb) > TCP_SEND_BUFSIZE) {
        if(iperf_tcp_client_status<IPERF_TCP_CLIENT_STARTING)
            goto TX_PERF_TERM;
        err = tcp_write(tpcb, iperf_send_buf, TCP_SEND_BUFSIZE, 0);
        if(err==ERR_MEM){
            //iperf_printf("txperf: Error on tcp_write: ERR_MEM\r\n");
            return -1;
        }
        if (err != ERR_OK) {
            iperf_printf("txperf: Error on tcp_write: %d\r\n", err);
            connect_pcb = NULL;
            return -1;
        }
        err = tcp_output(tpcb);
        if (err != ERR_OK) {
            iperf_printf("txperf: Error on tcp_output: %d\r\n",err);
            return -1;
        }
        txperf_outinfo.currentByte+=(u64_t)TCP_SEND_BUFSIZE;
        txperf_outinfo.currentPacketNum++;
        if(iperf_tcp_client_status==IPERF_TCP_CLIENT_STARTING) return 0;
    }
    return 0;
TX_PERF_TERM:
    tcp_write(tpcb, iperf_send_buf, tcp_sndbuf(tpcb), 0);
    tcp_output(tpcb);
    disconnect_txperf();
    return 0;
}

extern lega_semaphore_t iperf_Semaphore;
err_t txperf_sent_callback(void *arg, struct tcp_pcb *tpcb,
                              u16_t len)
{
    if(iperf_tcp_client_status != IPERF_TCP_CLIENT_INIT)
    {
        lega_rtos_set_semaphore(&iperf_Semaphore);
    }
    return 0;
}

static err_t
txperf_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    if(iperf_tcp_client_status!=IPERF_TCP_CLIENT_CONNECTING){
        iperf_printf("invalid status %d try to change to %d\n",iperf_tcp_client_status,IPERF_TCP_CLIENT_CONNECTING);
        return ERR_VAL;
    }

    iperf_printf("txperf: Connected to iperf tcp server %s:%d\r\n",inet_ntoa(tpcb->remote_ip),tpcb->remote_port);

    /* set callback values & functions */
    tcp_sent(tpcb, txperf_sent_callback);
    lega_rtos_start_timer(&iperf_output_timer);
    iperf_tcp_client_status=IPERF_TCP_CLIENT_CONNECTED;
    /* initiate data transfer */
    return ERR_OK;
}

void  txperf_conn_tcp_rst_callback(void *arg, err_t err)
{
    if(arg == (void*)connect_pcb)
    {
        //socket_pcb can be freed when call this callback, just need to put socket_pcb null
        connect_pcb = NULL;
        iperf_printf("%s %d\n",__func__,err);
    }
}

int start_txperf_application(ip_addr_t *ipaddr,int port)
{
    struct tcp_pcb *pcb;
    err_t err;
    int count=0;
    /* create new TCP PCB structure */
    pcb = tcp_new();
    if (!pcb) {
        iperf_printf("txperf: Error creating PCB. Out of Memory\r\n");
        return -1;
    }

    tcp_err(pcb, txperf_conn_tcp_rst_callback);
    tcp_arg(pcb, pcb);

    //if(socket_pcb) tcp_close(socket_pcb);
    connect_pcb=pcb;
    iperf_tcp_client_status=IPERF_TCP_CLIENT_CONNECTING;
    err = tcp_connect(pcb, ipaddr, port, txperf_connected_callback);
    if (err != ERR_OK) {
        iperf_printf("txperf: tcp_connect returned error: %d\r\n", err);
        return err;
    }
    while(iperf_tcp_client_status!=IPERF_TCP_CLIENT_CONNECTED){
        lega_rtos_delay_milliseconds(10);
        if(count++>300){
            iperf_printf("txperf: Setup tcp connect fail!\n");
            if(connect_pcb){
                tcp_close(connect_pcb);
                connect_pcb = NULL;
            }
            return -1;
        }
    }
    iperf_tcp_client_status=IPERF_TCP_CLIENT_STARTING;
    return 0;
}
#endif
