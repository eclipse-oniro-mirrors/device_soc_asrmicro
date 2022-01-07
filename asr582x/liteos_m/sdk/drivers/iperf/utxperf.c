#include "lwip_app_iperf.h"
#ifdef LWIP_APP_IPERF

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/udp.h"
#ifndef __PPC__
#include "iperf_printf.h"
#endif


static struct udp_pcb *connected_pcb = NULL;
static struct pbuf *pbuf_to_be_sent = NULL;

void utxperf_init()
{
    iperf_udp_client_status=IPERF_UDP_CLIENT_INIT;
    memset(&utxperf_outinfo,0,sizeof(utxperf_outinfo));
}

void disconnect_utxperf()
{
    if(connected_pcb) {
        udp_disconnect(connected_pcb);
        udp_remove(connected_pcb);
        connected_pcb=NULL;
    }
    iperf_printf("udp client:iperf terminate and disconnect\r\n");
    iperf_udp_client_status=IPERF_UDP_CLIENT_INIT;
}

int transfer_utxperf_data()
{
    err_t err;
    struct udp_pcb *pcb = connected_pcb;
    static signed int id = 0;
    int *payload;
    if(iperf_udp_client_status!=IPERF_UDP_CLIENT_TXRUNNING) goto TERM_UTXPERF;

    pbuf_to_be_sent = pbuf_alloc(PBUF_TRANSPORT, UDP_SEND_BUFSIZE, PBUF_ROM);
    if (!pbuf_to_be_sent) {
        iperf_printf("error allocating pbuf to send\r\n");
        return -1;
    }
    else {
        pbuf_to_be_sent->payload=iperf_send_buf;
       // memcpy(pbuf_to_be_sent->payload, iperf_send_buf, UDP_SEND_BUFSIZE);
    }

    /* always increment the id */
    payload = (int*)(pbuf_to_be_sent->payload);
    if (id == 0x7FFFFFFE )
        id = 0;
    id++;
    payload[0] = htonl(id);

    err = udp_send(pcb, pbuf_to_be_sent);
    if (err != ERR_OK) {
        //iperf_printf("Error on udp_send: %d\r\n", err);
        pbuf_free(pbuf_to_be_sent);
        return -1;
    }
    pbuf_free(pbuf_to_be_sent);
    utxperf_outinfo.currentByte+=(u64_t)UDP_SEND_BUFSIZE;
    utxperf_outinfo.currentPacketNum++;
    return 0;
TERM_UTXPERF:
    disconnect_utxperf();
    return -2;
}

int start_utxperf_application(ip_addr_t *ipaddr,int port)
{
    struct udp_pcb *pcb;
    err_t err;

    /* create a udp socket */
    pcb = udp_new();
    if (!pcb) {
        iperf_printf("Error creating PCB. Out of Memory\r\n");
        return -1;
    }

    /* bind local address */
    if ((err = udp_bind(pcb, IP_ADDR_ANY, 0)) != ERR_OK) {
        iperf_printf("error on udp_bind: %x\n\r", err);
        return -1;
    }

    /* connect to iperf server */
    err = udp_connect(pcb, ipaddr, port);
    if (err != ERR_OK){
        iperf_printf("error on udp_connect: %x\n\r", err);
        return -1;
    }

    //if(connected_pcb) udp_remove(connected_pcb);
    connected_pcb = pcb;
    lega_rtos_start_timer(&iperf_output_timer);
    iperf_udp_client_status = IPERF_UDP_CLIENT_TXRUNNING;
    return 0;
}
#endif
