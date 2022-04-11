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

#ifndef _LEGA_WIFI_API_AOS_H_
#define _LEGA_WIFI_API_AOS_H_

#include "lega_wlan_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WLAN_EVENT_SCAN_COMPLETED,
    WLAN_EVENT_ASSOCIATED,
    WLAN_EVENT_CONNECTED,
    WLAN_EVENT_IP_GOT,
    WLAN_EVENT_DISCONNECTED,
    WLAN_EVENT_AP_UP,
    WLAN_EVENT_AP_DOWN,
    WLAN_EVENT_SCAN_FAILED,
    WLAN_EVENT_CONNECT_FAILED,
    WLAN_EVENT_AP_PEER_UP,
    WLAN_EVENT_AP_PEER_DOWN,
    WLAN_EVENT_MAX,
} lega_wlan_event_e;

/**
 *  @brief  Input network precise paras in lega_wlan_start_adv function.
 */
typedef struct {
    lega_wlan_ap_info_adv_t ap_info;         /**< @ref apinfo_adv_t. */
    char  key[64];                /**< Security key or PMK of the wlan. */
    int   key_len;                /**< The length of the key. */
    char  local_ip_addr[16];      /**< Static IP configuration, Local IP address. */
    char  net_mask[16];           /**< Static IP configuration, Netmask. */
    char  gateway_ip_addr[16];    /**< Static IP configuration, Router IP address. */
    char  dns_server_ip_addr[16]; /**< Static IP configuration, DNS server IP address. */
    char  dhcp_mode;              /**< DHCP mode, @ref DHCP_Disable, @ref DHCP_Client and @ref DHCP_Server. */
    char  reserved[32];
    int   wifi_retry_interval;    /**< Retry interval if an error is occured when connecting an access point,
                                  time unit is millisecond. */
} lega_wlan_init_info_adv_st;

/** @brief  used in station and softap mode, get mac address(in char mode) of WIFI device
 *
 * @param mac_addr    : pointer to get the mac address
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_get_mac_address_inchar(char *puc_mac);

int lega_wlan_suspend_sta(void);
int lega_wlan_suspend_ap(void);
int lega_wlan_suspend(void);
void lega_wlan_register_mgmt_monitor_cb(monitor_cb_t fn);

/*Wifi event callback interface
*
* @return    void
*/
extern void wifi_event_cb(lega_wlan_event_e evt, void *info);
#ifdef __cplusplus
}
#endif

#endif // _LEGA_WIFI_API_AOS_H_

