#include "lwip_app_iperf.h"
#ifdef LWIP_APP_IPERF


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/udp.h"
#ifndef __PPC__
#include "iperf_printf.h"
#endif
static struct udp_pcb *socket_udp_pcb=NULL;
void urxperf_init()
{
    iperf_udp_server_status=IPERF_UDP_SERVER_INIT;
    memset(&urxperf_outinfo,0,sizeof(urxperf_outinfo));
}

void clear_urxperf()
{
    if(socket_udp_pcb){
        udp_disconnect(socket_udp_pcb);
        udp_remove(socket_udp_pcb);
        socket_udp_pcb=NULL;
    }
    iperf_udp_server_status=IPERF_UDP_SERVER_INIT;
    iperf_printf("udp server:iperf terminate\r\n");
}
#ifndef LWIP_DUALSTACK
void
urxperf_recv_callback(void *arg, struct udp_pcb *tpcb,
                               struct pbuf *p,const struct ip4_addr *addr, u16_t port)
#else
void
urxperf_recv_callback(void *arg, struct udp_pcb *tpcb,
                               struct pbuf *p,const struct ip4_addr *addr, u16_t port)
#endif
{
#if 0
    static int first = 1;
    static int expected_id = 0;
    static int n_dropped = 0;
    int recv_id;

    /* first, see if the datagram is received in order */
    recv_id =  *((int *)(p->payload));

    if (first)
        expected_id = recv_id;
    else if (expected_id != recv_id)  {
        n_dropped += (recv_id - expected_id);
        expected_id = recv_id;
    }

    expected_id++;
    first = 0;
#else
    if(p){
        if(iperf_udp_server_status==IPERF_UDP_SERVER_RXRUNNING){

            urxperf_outinfo.currentByte+=(u64_t)p->tot_len;
            urxperf_outinfo.currentPacketNum++;
        }

        pbuf_free(p);

        //iperf_printf("urxperf: receive package size=%d\r\n",p->tot_len);
    }
#endif
}

int start_urxperf_application(int port)
{
    struct udp_pcb *pcb;
    err_t err;

    /* create new TCP PCB structure */
    pcb = udp_new();
    if (!pcb) {
        iperf_printf("Error creating PCB. Out of Memory\r\n");
        return -1;
    }

    //if(socket_udp_pcb!=NULL) udp_remove(socket_udp_pcb);
    socket_udp_pcb=pcb;
    /* bind to iperf @port */
    err = udp_bind(pcb, IP_ADDR_ANY, port);
    if (err != ERR_OK) {
        iperf_printf("Unable to bind to port %d: err = %d\r\n", iperf_config.port, err);
        return -2;
    }
    iperf_udp_server_status=IPERF_UDP_SERVER_RXRUNNING;
    lega_rtos_start_timer(&iperf_output_timer);
    udp_recv(pcb, urxperf_recv_callback, NULL);

    return 0;
}

#endif
