#ifndef _LWIP_HOOKS_H
#define _LWIP_HOOKS_H
#include "lwip/arch.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif
void lwip_init_tcp_isn();
u32_t lwip_hook_tcp_isn(ip_addr_t *local_ip, u16_t local_port, ip_addr_t *remote_ip, u16_t remote_port);
#ifdef __cplusplus
}
#endif
#endif//_LWIP_HOOKS_H
