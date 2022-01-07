#include "lwip_app_iperf.h"
#ifdef LWIP_APP_IPERF


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#ifdef __arm__
#include "iperf_printf.h"
#endif

static struct tcp_pcb *connect_pcb[TCP_MAX_TXPERF_CONNECTION];
static struct tcp_pcb *socket_pcb;
void rxperf_init()
{
    int i=0;
    socket_pcb=NULL;
    for(i=0;i<TCP_MAX_TXPERF_CONNECTION;i++)
        connect_pcb[i]=NULL;

    iperf_tcp_server_status=IPERF_TCP_SERVER_INIT;
    memset(&rxperf_outinfo,0,sizeof(rxperf_outinfo));
}
static void disconnect_rxperf()
{
    lega_rtos_declare_critical();
    lega_rtos_enter_critical();

    int i=0;
    for(i=0;i<TCP_MAX_TXPERF_CONNECTION;i++)
        if(connect_pcb[i]!=NULL){
            tcp_close(connect_pcb[i]);
            connect_pcb[i]=NULL;
        }
    iperf_tcp_server_status=IPERF_TCP_SERVER_LISTENING;
    iperf_printf("tcp server:iperf disconnect\r\n");

    lega_rtos_exit_critical();
}

void clear_rxperf()
{
    lega_rtos_declare_critical();
    lega_rtos_enter_critical();

    disconnect_rxperf();
    if(socket_pcb){
        tcp_close(socket_pcb);
        socket_pcb=NULL;
    }
    iperf_tcp_server_status=IPERF_TCP_SERVER_INIT;
    iperf_printf("tcp server:iperf terminate\r\n");

    lega_rtos_exit_critical();
}
static err_t
rxperf_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    /* close socket if the peer has sent the FIN packet  */
    if (p == NULL) {
        iperf_printf("tcp server: peer(%s:%d) has sent the FIN packet!\r\n",inet_ntoa(tpcb->remote_ip),tpcb->remote_port);
        //iperf_printf("close pcb=%x\r\n",tpcb);
        do_iperf_terminate_timer("tcp server",&rxperf_outinfo);
        disconnect_rxperf();

        return ERR_OK;
    }

    /* all we do is say we've received the packet */
    /* we don't actually make use of it */
    tcp_recved(tpcb, p->tot_len);
    //iperf_printf("rxperf: receive package size=%d\r\n",p->tot_len);

    rxperf_outinfo.currentByte+=(u64_t)p->tot_len;
    rxperf_outinfo.currentPacketNum++;
    if(iperf_tcp_server_status!=IPERF_TCP_SERVER_RXRUNNING){
        do_iperf_terminate_timer("tcp server",&rxperf_outinfo);
        clear_rxperf();
    }
    pbuf_free(p);

    return ERR_OK;
}

void  rxperf_conn_tcp_rst_callback(void *arg, err_t err)
{
    int i=0;

    for(i=0;i<TCP_MAX_TXPERF_CONNECTION;i++){
        if(arg==connect_pcb[i]){
            connect_pcb[i]=NULL;
            iperf_printf("%s %d\n",__func__,err);
            break;
        }
    }
}

err_t rxperf_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    int i=0;

    if(iperf_tcp_server_status!=IPERF_TCP_SERVER_LISTENING)
        return ERR_VAL;
    for(i=0;i<TCP_MAX_TXPERF_CONNECTION;i++){
        if(newpcb==connect_pcb[i]) break;
        if(connect_pcb[i]==NULL){
            connect_pcb[i]=newpcb;
            break;
        }
    }
    if(i==TCP_MAX_TXPERF_CONNECTION){
        iperf_printf("rxperf: connection max %d, cannot accept another connection!\r\n",TCP_MAX_TXPERF_CONNECTION);
        tcp_close(newpcb);
        return ERR_OK;
    }

    tcp_err(newpcb, rxperf_conn_tcp_rst_callback);
    tcp_arg(newpcb, newpcb);

    iperf_printf("tcp server: Connection Accepted, remote addr %s:%d\r\n",inet_ntoa(newpcb->remote_ip),newpcb->remote_port);
    tcp_recv(newpcb, rxperf_recv_callback);
    lega_rtos_start_timer(&iperf_output_timer);
    iperf_tcp_server_status=IPERF_TCP_SERVER_RXRUNNING;

    return ERR_OK;
}

int start_rxperf_application(int port)
{
    struct tcp_pcb *pcb;
    err_t err;
    iperf_printf("Begin to run iperf tcp server\r\n");

    /* create new TCP PCB structure */
    pcb = tcp_new();
    if (!pcb) {
        iperf_printf("rxperf: Error creating PCB. Out of Memory\r\n");
        return -1;
    }

    /* bind to iperf @port */
    err = tcp_bind(pcb, IP_ADDR_ANY, port);
    if (err != ERR_OK) {
        iperf_printf("rxperf: Unable to bind to port %d: err = %d\r\n", iperf_config.port, err);
        return -2;
    }

    /* we do not need any arguments to callback functions :) */
    tcp_arg(pcb, NULL);

    /* listen for connections */
    pcb = tcp_listen(pcb);
    if (!pcb) {
        iperf_printf("rxperf: Out of memory while tcp_listen\r\n");
        return -3;
    }
    //iperf_printf("tcp_listen pcb=%x\r\n",pcb);
    //if(socket_pcb) tcp_close(socket_pcb);
    socket_pcb=pcb;

    /* specify callback to use for incoming connections */
    tcp_accept(pcb, rxperf_accept_callback);

    iperf_tcp_server_status=IPERF_TCP_SERVER_LISTENING;

    return 0;
}
#endif
