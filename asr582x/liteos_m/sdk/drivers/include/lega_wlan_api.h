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
 * @file lega_wifi_api.h
 *
 * @brief WiFi API.
 *
 ****************************************************************************************
 */
#ifndef _LEGA_WIFI_API_H_
#define _LEGA_WIFI_API_H_

#include <stdint.h>
#include <stdbool.h>

/**
 *  @brief  wlan network interface enumeration definition.
 */
typedef enum {
    SOFTAP,   /*Act as an access point, and other station can connect, 4 stations Max */
    STA,      /*Act as a station which can connect to an access point */
} lega_wlan_type_e;

typedef enum {
    EVENT_STATION_UP = 1,  /* used in station mode,
                            indicate station associated in open mode or 4-way-handshake done in WPA/WPA2 */
    EVENT_STATION_DOWN,    /* used in station mode, indicate station deauthed */
    EVENT_AP_UP,           /* used in softap mode, indicate softap enabled */
    EVENT_AP_DOWN,         /* used in softap mode, indicate softap disabled */
} lega_wifi_event_e;
typedef enum {
    CONNECT_SUCC,
    CONNECT_SCAN_FAIL,
    CONNECT_CONN_FAIL,
} lega_start_adv_results_e;

/**
 *  @brief  Scan result using normal scan.
 */
typedef struct {
    uint8_t is_scan_adv;
    char ap_num;       /**< The number of access points found in scanning. */
    struct {
        char    ssid[32 + 1]; /* ssid max len:32. +1 is for '\0'. when ssidlen is 32  */
        char    ap_power;   /**< Signal strength, min:0, max:100. */
        char    bssid[6];     /* The BSSID of an access point. */
        char    channel;      /* The RF frequency, 1-13 */
        uint8_t security;     /* Security type, @ref wlan_sec_type_t */
    } *ap_list;
} lega_wlan_scan_result_t;

typedef enum {
    WLAN_SECURITY_OPEN,                   // NONE
    WLAN_SECURITY_WEP,                    // WEP
    WLAN_SECURITY_WPA,                    // WPA
    WLAN_SECURITY_WPA2,                   // WPA2
    WLAN_SECURITY_AUTO,                   // WPA or WPA2
    WLAN_SECURITY_MAX,
} lega_wlan_security_e;

/* used in event callback of station mode, indicate softap informatino which is connected */
typedef struct {
    int     rssi;           /* rssi */
    char    ssid[32 + 1];   /* ssid max len:32. +1 is for '\0' when ssidlen is 32  */
    char    pwd[64 + 1];    /* pwd max len:64. +1 is for '\0' when pwdlen is 64 */
    char    bssid[6];       /* BSSID of the wlan needs to be connected. */
#ifdef CFG_NAN_CONFIG
    char    pmk[32];
#endif
    char    ssid_len;       /* ssid length */
    char    pwd_len;        /* password length */
    char    channel;       /* wifi channel 0-13. */
    char    security;      /* refer to lega_wlan_security_d */
} lega_wlan_ap_info_adv_t;

/* only used in station modd */
typedef struct {
    char    dhcp;             /* no use currently */
    char    macaddr[16];      /* mac address on the target wlan interface, ASCII */
    char    ip[16];           /* Local IP address on the target wlan interface, ASCII */
    char    gate[16];         /* Router IP address on the target wlan interface, ASCII */
    char    mask[16];         /* Netmask on the target wlan interface, ASCII */
    char    dns[16];          /* no use currently , ASCII */
    char    broadcastip[16];  /* no use currently , ASCII */
} lega_wlan_ip_stat_t;

/* only used in station modd */
typedef struct {
    int     is_connected;  /* The link to wlan is established or not, 0: disconnected, 1: connected. */
    int     wifi_strength; /* Signal strength of the current connected AP */
    char    ssid[32 + 1];    /* ssid max len:32. +1 is for '\0'. when ssidlen is 32  */
    char    bssid[6];      /* BSSID of the current connected wlan */
    int     channel;       /* Channel of the current connected wlan */
} lega_wlan_link_stat_t;

/* used in open cmd for AP mode */
typedef struct {
    char    ssid[32 + 1];   /* ssid max len:32. +1 is for '\0' when ssidlen is 32  */
    char    pwd[64 + 1];    /* pwd max len:64. +1 is for '\0' when pwdlen is 64 */
    int     interval;       /* beacon listen interval */
    int     hide;           /* hidden SSID */
    int     channel;       /* Channel */
} lega_wlan_ap_init_t;

/* used in open cmd of hal_wifi_module_t */
typedef struct {
    char    wifi_mode;              /* refer to hal_wifi_type_t */
    char    security;               /* security mode */
    char    wifi_ssid[32];          /* in station mode, indicate SSID of the wlan needs to be connected.
                                       in softap mode, indicate softap SSID */
    char    wifi_key[64];           /* in station mode, indicate Security key of the wlan needs to be connected,
                                       in softap mode, indicate softap password.(ignored in an open system.) */
    char    local_ip_addr[16];      /* used in softap mode to config ip for dut */
    char    net_mask[16];           /* used in softap mode to config gateway for dut */
    char    gateway_ip_addr[16];    /* used in softap mode to config netmask for dut */
    char    dns_server_ip_addr[16]; /* no use currently */
    char    dhcp_mode;              /* no use currently */
    char    channel;                /* softap channel in softap mode; connect channel in sta modd */
    char    mac_addr[6];            /* connect bssid in sta modd */
    char    reserved[32];           /* no use currently */
#ifdef CFG_NAN_CONFIG
    uint8_t pmk[32];
#endif
    int     wifi_retry_interval;    /* no use currently */
} lega_wlan_init_type_t;

/*WLAN error status */
typedef enum {
    WLAN_STA_MODE_BEACON_LOSS = 1,           // in sta mode, cannot receive beacon of peer connected AP for a long time
    WLAN_STA_MODE_AUTH_FAIL,                 // in sta mode, connect fail during auth
    WLAN_STA_MODE_ASSOC_FAIL,                // in sta mode, connect fail during association
    WLAN_STA_MODE_PASSWORD_ERR,              // in sta mode, connect fail as password error
    WLAN_STA_MODE_NO_AP_FOUND,               // in sta mode, connect fail as cannot find the connecting AP during scan
    WLAN_STA_MODE_DHCP_FAIL,                 // in sta mode, connect fail as dhcp fail
} lega_wlan_err_status_e;

typedef struct {
    uint8_t first_channel;
    uint8_t last_channel;
    uint8_t max_tx_power;
    uint8_t dfs_req;
} lega_wlan_country_channel_table_t;

#define LEGA_WLAN_CC_CHAN_TABLE_NUM_MAX 1
typedef struct {
    uint8_t country_code[4];
    uint8_t channel_table_num;
    lega_wlan_country_channel_table_t table[LEGA_WLAN_CC_CHAN_TABLE_NUM_MAX];
} lega_wlan_country_code_param_t;

/**
 * @brief sta ip and mac address used in softap mode
 * sta_ip_addr: e.g. when ip addr==192.168.1.1<-->sta_ip_addr == 0x0101A8C0
 */
typedef struct {
    uint32_t sta_ip_addr;                           /* station ip addr  */
    uint8_t  sta_mac_addr[6];                       /* station mac addr */
} lega_wlan_client_addr_info_t;

/* store linked station info */
typedef struct {
    int client_num;                                 /* linked station number */
    lega_wlan_client_addr_info_t sta[4];            /* linked station entry, max client number is 4 */
} lega_wlan_ap_client_info_t;
// start with dataelem struct declaration
typedef enum {
    MS_OK = 0,
    MS_ERROR
} MS_STATUS;

typedef struct ms_hal_realtime_info_s {
    int8_t   rssi;                      /* indicator of radio signal strength of uplink from STA to AP */
    uint8_t  channel;                   /* the channel number scanned by radio given */
    uint32_t unicast_send_speed;        /* statistics for unicast Bytes Sent per second */
    uint32_t unicast_recv_speed;        /* statistics for unicast Bytes Receive per second */
    uint32_t multicast_send_speed;      /* statistics for multicast Bytes Sent per second */
    uint32_t multicast_recv_speed;      /* statistics for multicast Bytes Receive per second */
    uint32_t broadcast_send_speed;      /* statistics for broadcast Bytes Sent per second */
    uint32_t broadcast_recv_speed;      /* statistics for broadcast Bytes Receive per second */
    uint32_t last_uplink_rate;          /* datarate of data-frame transmit from associated STA to AP */
    uint32_t last_downlink_rate;        /* datarate of data-frame transmit from AP to associated STA */
    uint32_t utilization_transmit;      /* the time of radio spent on channel transmit data (ms) */
    uint32_t utilization_receive;       /* the time of radio spent on channel receive data (ms) */
    uint32_t est_uplink_rate;           /* estimate of MAC layer throuhtput in uplink if chn 100% used */
    uint32_t est_downlink_rate;         /* estimate of MAC layer throuhtput in downlink if chn 100% used */
    uint32_t byte_send_speed;           /* bytes sent to associate AP per second */
    uint32_t byte_recv_speed;           /* bytes receive from associate AP per second */
    uint32_t packet_send_speed;         /* packets sent to associate AP per second */
    uint32_t packet_recv_speed;         /* packets receive from associate AP per second */
    uint32_t errors_send_speed;         /* error packet AP transmit to STA per second (retry limit and other) */
    uint32_t errors_recv_speed;         /* error packet STA transmit to AP per second */
    uint32_t retrains_count;            /* total retransmission include same packted retransmit in hw */
    uint8_t  chn_utilization;           /* measure by the radio on the scanned 20MHZ channel */
    int8_t   noise;                     /* indicator of the average radio noise plus interference power */
} ms_hal_realtime_info_t;

typedef struct ms_hal_base_info_s {
    int8_t   rssi;                      /* indicator of radio signal strength of uplink from STA to AP */
    uint8_t  channel;                   /* the channel number scanned by radio given */
    uint8_t  snr;                       /* snr of connected ap for station mode */
    uint8_t  bssid[6];                  /* mac address of associated AP */
} ms_hal_base_info_t;

typedef struct ms_hal_status_info_s {
    uint8_t  enc_type;                  /* security mode, refer to lega_wlan_security_e */
    uint8_t  dns[16];                   /* dns address assigned to the client */
    uint64_t bts;                       /* beacon timestamp of associated AP */
    uint8_t  ipv4[16];                  /* ipv4 address assigned to client */
    uint8_t  ipv6[16];                  /* ipv6 address assigned to client */
    uint16_t ssid_len;                  /* ssid length of STA */
    uint8_t  ssid[32];                  /* ssid of associated AP */
} ms_hal_status_info_t;

typedef struct ms_hal_test_report_s {
    uint8_t  sta_num_on_router;         /* the number of STAs associated to this BSS */
    uint8_t  numofNeighbors;            /* the number of neithbor bss in associate channel */
    uint32_t byte_tx_speed;             /* bytes sent to associate AP per second */
    uint32_t byte_rx_speed;             /* bytes receive from associate AP per second */
    uint32_t packet_tx_speed;           /* packets sent to associate AP per second */
    uint32_t packet_rx_spped;           /* bytes receive from associate AP per second */
    uint32_t auth_time;                 /* time used between auth req and last auth rsp */
    uint32_t assoc_time;                /* time used between assoc req and last assoc rsp */
    uint8_t  power_save_enable;         /* check powersave mode is whether enable */
    uint8_t  measure_num;               /*  number of measure frame report */
    uint8_t  *measure;                  /* measure report content */
} ms_hal_test_report_t;

/** @brief  get realtime wireless info, call it after in sta mode and connected status
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
MS_STATUS ms_hal_realtime_info_get(ms_hal_realtime_info_t *info);

/** @brief  get wireless basic info, call it after in sta mode and connected status
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
MS_STATUS ms_hal_base_info_get(ms_hal_base_info_t *info);

/** @brief  get wireless basic info, call it after start sta mode
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
MS_STATUS ms_hal_status_info_get(ms_hal_status_info_t *info);

/** @brief  start test wireless quality, call it after start sta mode
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
MS_STATUS ms_hal_test_start(void);
/** @brief  get wireless quality test info, call it after call ms_hal_test_start
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
MS_STATUS ms_hal_test_report_get(ms_hal_test_report_t *report);
// end of dataelem struct declaration

/** @brief  wifi init functin, user should call it before use any wifi cmd
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_init(void);

/** @brief  wifi deinit functin, call it when donot use wifi any more to free resources
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_deinit(void);

/** @brief  used in station and softap mode, open wifi cmd
 *
 * @param init_info    : refer to lega_wlan_init_type_t
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_open(lega_wlan_init_type_t *init_info);

/** @brief  used in softap mode, open wifi cmd
 *
 * @param init_info    : refer to lega_wlan_ap_init_t
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_ap_open(lega_wlan_ap_init_t *init_info);

/** @brief  used in station and softap mode, close wifi cmd
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_close(void);

/** @brief  used in station mode, scan cmd
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_start_scan(void);

/** @brief  used in station mode, scan cmd
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_start_scan_adv(void);

/** @brief  used in station mode, scan cmd
 *
 * @param ssid    : target ssid to scan
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_start_scan_active(const char *ssid);

/** @brief  used in station mode, scan cmd
 *
 * @param ssid    : target ssid to scan, could be null
 * @param bssid   : target bssid to scan, could be null
 * @param channel : target channel to scan, could be null
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_start_scan_detail(char *ssid, int channel, char *bssid);

/** @brief  used in station and softap mode, get mac address(in hex mode) of WIFI device
 *
 * @param mac_addr    : pointer to get the mac address
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_get_mac_address(uint8_t *mac_addr);

/** @brief  used in station and softap mode, set mac address for WIFI device
 *
 *  @param mac_addr    : src mac address pointer to set
 *
 */
void lega_wlan_set_mac_address(uint8_t *mac_addr);

/** @brief  used in station mode, get the ip information
 *
 * @param void
 * @return    NULL    : error occurred.
 * @return    pointer : ip status got.
 */
lega_wlan_ip_stat_t *lega_wlan_get_ip_status(void);

/** @brief  used in station mode, get link status information
 *
 * @param link_status    : refer to lega_wlan_link_stat_t
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_get_link_status(lega_wlan_link_stat_t *link_status);

/** @brief  used in station mode, get the associated ap information
 *
 * @param void
 * @return    NULL    : error occurred.
 * @return    pointer : associated ap info got.
 */
lega_wlan_ap_info_adv_t *lega_wlan_get_associated_apinfo(void);

/* used in sniffer mode, open sniffer mode
*  @return    0       : on success.
*  @return    other   : error occurred
*/
int lega_wlan_start_monitor(void);

/* used in sniffer mode, close sniffer mode
*  @return    0       : on success.
*  @return    other   : error occurred
*/
int lega_wlan_stop_monitor(void);

/** @brief  used in sniffer mode, set the sniffer channel, should call this cmd after call start_monitor cmd
 *
 * @param channel    : WIFI channel(1-13)
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_monitor_set_channel(int channel);

/** @brief  used in sta mode, set the ps bc mc and listen interval, called before connect to ap.
 *
 * @param listen_bc_mc    : true or false
 * @param listen_interval :1, 3, 10
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_set_ps_options(uint8_t listen_bc_mc, uint16_t listen_interval);

/** @brief  used in sta mode, set ps mode on/off
 *
 * @param ps_on    : true or false
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_set_ps_mode(uint8_t ps_on);

/* when use monitor mode, user should register this type of callback function to get the received MPDU */
typedef void (*monitor_cb_t)(uint8_t *data, int len, int rssi);

/* when use monitor-ap mode, user should register this type of callback function to turn off monitor */
typedef void (*monitor_ap_cb_t)(void);

/** @brief  used in sniffer mode, callback function to get received MPDU, should register before start_monitor
 *
 * @param fn    : refer to monitor_data_cb_t
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_register_monitor_cb(monitor_cb_t fn);

/** @brief  used in sniffer-ap mode, callback function for application
 *
 * @param fn    : refer to monitor_ap_cb_t
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_register_monitor_ap_cb(monitor_ap_cb_t fn);

/* start adv callback function, notify the connect results */
typedef void (*start_adv_cb_t)(lega_start_adv_results_e status);

/** @brief  used in sta mode, callback function to notify the connecting results
 *
 * @param fn    : refer to start_adv_cb_t
 *
 *  @return    0       : on success.
 *  @return    other   : error occurred
 */
int lega_wlan_register_start_adv_cb(start_adv_cb_t fn);

/** @brief  used in station mode or sniffer mode, call this cmd to send a MPDU constructed by user
 *
 * @param buf    :  mac header pointer of the MPDU
 * @param len    :  length of the MPDU
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_send_raw_frame(uint8_t *buf, int len);

/* @brief api for nanconfig
 *
 */
#ifdef CFG_NAN_CONFIG
typedef void (*ap_cb_t)(uint32_t len, uint8_t *output, uint8_t *mac_src);
int lega_wlan_register_protobuf_frame_handle_cb(ap_cb_t fn);
int lega_wlan_nan_update_beacon(uint32_t outlen, uint8_t *output, uint8_t *mac_dst, uint8_t iftype);
int lega_wlan_stop_recev_protobuf_frame(void);
int lega_wlan_nan_set_seq_num(uint8_t *pframe, uint8_t len);
int lega_wlan_nan_set_scan_channel(uint8_t channel, uint32_t duration);
int lega_wlan_disable_nanconfig(void);
#endif

/* enable WIFI stack log, will be output by uart
*
* @return    0       : on success.
* @return    other   : error occurred
*/
int lega_wlan_start_debug_mode(void);

/* disable WIFI stack log
*
* @return    0       : on success.
* @return    other   : error occurred
*/
int lega_wlan_stop_debug_mode(void);

/*
 * The event callback function called at specific wifi events occurred by wifi stack.
 * user should register these callback if want to use the informatin.
 *
 * @note For HAL implementors, these callbacks must be
 *       called under normal task context, not from interrupt.
 */
typedef void (*lega_wlan_cb_ip_got)(lega_wlan_ip_stat_t *ip_status);
/** @brief  used in station mode, WIFI stack call this cb when get ip
 *
 * @param fn    : cb function type, refer to lega_wlan_ip_stat_t
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_ip_got_cb_register(lega_wlan_cb_ip_got fn);

typedef void (*lega_wlan_cb_stat_chg)(lega_wifi_event_e wlan_event);
/** @brief  used in station and softap mode,
 *          WIFI stack call this cb when status change, refer to lega_wifi_event_e
 *
 * @param fn    : cb function type
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_stat_chg_cb_register(lega_wlan_cb_stat_chg fn);

typedef void (*lega_wlan_cb_scan_compeleted)(lega_wlan_scan_result_t *result);
/** @brief  used in station mode,
 *          WIFI stack call this cb when scan complete
 *
 * @param fn    : cb function type
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_scan_compeleted_cb_register(lega_wlan_cb_scan_compeleted fn);

typedef void (*lega_wlan_cb_associated_ap)(lega_wlan_ap_info_adv_t *ap_info);
/** @brief  used in station mode,
 *          WIFI stack call this cb when associated with an AP, and tell the AP information
 *
 * @param fn    : cb function type
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_associated_ap_cb_register(lega_wlan_cb_associated_ap fn);

/** @brief  calibration RCO clock for RTC
 *
 */
void lega_drv_rco_cal(void);

/** @brief  config to close DCDC PFM mode
 *
 */
void lega_drv_close_dcdc_pfm(void);

/** @brief  config to support smartconfig in MIMO scenario
 *
 */
void lega_wlan_smartconfig_mimo_enable(void);

/** @brief  set information about country code
 *
 * @param country_code_param  : refer to lega_wlan_country_code_param_t
 */
void lega_wlan_set_country_code(lega_wlan_country_code_param_t *country_code_param);

/** @brief  start monitor and ap coexist mode
 *
 * @param init_info    : refer to lega_wlan_init_type_t
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_start_monitor_ap(lega_wlan_init_type_t *init_info);

/** @brief  stop monitor and ap coexist mode
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_stop_monitor_ap(void);

typedef void (*lega_wlan_err_stat_handler)(lega_wlan_err_status_e err_info);
/** @brief  user use to register err status callback function,
 *          WIFI stack call this cb when some error occured, refer to lega_wlan_err_status_e
 *
 * @param fn    : cb function type
 *
 * @return    0       : on success.
 * @return    other   : error occurred
 */
int lega_wlan_err_stat_cb_register(lega_wlan_err_stat_handler fn);

/** @brief  enable temperature compensation
 *  called before wifi open
 *
 * @param timer_in_sec    : input tmmt detect interval,min 10(second)
 *
 */
void lega_wlan_enable_tmmt_compensation(uint64_t timer_in_sec);

/** @brief  get linked station ip and address in ap mode
 *  @param            : input param to get sta ip and mac addr
 */
void lega_get_client_ip_mac(lega_wlan_ap_client_info_t *sta_addr);

/** @brief  api for uart log control
 *
 */
void lega_log_enable(void);
void lega_log_disable(void);
bool lega_log_is_enable(void);

#endif  // _LEGA_WIFI_API_H_
