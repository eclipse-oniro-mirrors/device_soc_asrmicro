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

#include "station_info.h"
#include "wifi_device.h"
#include "wifi_device_config.h"
#include "wifi_error_code.h"
#include "wifi_event.h"
#include "wifi_hotspot.h"
#include "wifi_hotspot_config.h"
#include "wifi_linked_info.h"
#include "wifi_scan_info.h"
#include "lega_wlan_api_aos.h"
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif
#include "duet_flash_kv.h"
#include "lwip/inet.h"
#ifdef HARMONYOS_TEMP
// uint8_t wifi_ready = 0;
// uint8_t ble_open = 0;
// uint16_t ble_con_interval = 50;
#endif

typedef enum {
    STA_MODE_E = 0x01,
    SAP_MODE_E,
    SNIFFER_MODE_E,
    TEST_MODE_E,
} current_iftype_e;
extern uint32_t current_iftype;

int WifiFreqToChannel(int freq)
{
    int channel = 0;

    // Check if frequency is in the expected range
    if ((freq < 2412) || (freq > 2484)) {
        return channel;
    }

    // Compute the channel number
    if (freq == 2484) {
        channel = 14;
    } else {
        channel = (freq - 2407) / 5;
    }

    return channel;
}

WifiErrorCode EnableWifi(void)
{
    int ret;

    printf("%s\r\n", __func__);

    if (current_iftype != 0xFF) {
        return ERROR_WIFI_BUSY;
    }

    lega_wlan_init_type_t init_param = {0};

    init_param.wifi_mode = STA;

    ret = lega_wlan_open(&init_param);
    if (ret == 0) {
        return WIFI_SUCCESS;
    } else {
        return ERROR_WIFI_UNKNOWN;
    }
}

void lalala_test(void)
{
    lega_wlan_init_type_t init_param = {0};
    char *ssid = "asr-guest";
    char *pwd = "asr123456";
    memcpy(init_param.wifi_ssid, ssid, sizeof(init_param.wifi_ssid));
    memcpy(init_param.wifi_key,  pwd, sizeof(init_param.wifi_key));
    init_param.wifi_mode = STA;
    lega_wlan_open(&init_param);
    (void)memset_s(&init_param, sizeof(init_param), 0, sizeof(init_param));
}

void lalala_test2(void)
{
    lega_wlan_init_type_t init_param = {0};
    char *ssid = "LALALALALA";
    char *pwd = "12345678";
    memcpy(init_param.wifi_ssid, ssid, sizeof(init_param.wifi_ssid));
    memcpy(init_param.wifi_key,  pwd, sizeof(init_param.wifi_key));
    init_param.wifi_mode = SOFTAP;
    lega_wlan_open(&init_param);
    (void)memset_s(&init_param, sizeof(init_param), 0, sizeof(init_param));
}

extern lega_semaphore_t lega_wlan_vendor_close_sta_semaphore;
WifiErrorCode DisableWifi(void)
{
    int ret;

    printf("%s\r\n", __func__);

    if (current_iftype != STA_MODE_E) {
        return ERROR_WIFI_NOT_STARTED;
    }

    lega_rtos_init_semaphore(&lega_wlan_vendor_close_sta_semaphore, 0);
    ret = lega_wlan_close();
    if (ret) {
        printf("%s fail\r\n", __func__);
        lega_rtos_deinit_semaphore(&lega_wlan_vendor_close_sta_semaphore);
        lega_wlan_vendor_close_sta_semaphore = 0;
        return ERROR_WIFI_UNKNOWN;
    }
    lega_rtos_get_semaphore(&lega_wlan_vendor_close_sta_semaphore, LEGA_NEVER_TIMEOUT);
    lega_rtos_deinit_semaphore(&lega_wlan_vendor_close_sta_semaphore);
    lega_wlan_vendor_close_sta_semaphore = 0;

    return WIFI_SUCCESS;
}

int IsWifiActive(void)
{
    printf("%s\r\n", __func__);

    if (current_iftype == STA_MODE_E) {
        return WIFI_STATE_AVAILABLE;
    } else {
        return WIFI_STATE_NOT_AVAILABLE;
    }
}

typedef struct {
    uint8_t is_scan_adv;
    char ap_num;       /* *< The number of access points found in scanning. */
    struct {
        char    ssid[32 + 1]; /* ssid max len:32. +1 is for '\0'. when ssidlen is 32  */
        char    ap_power;     /* *< Signal strength, min:0, max:100. */
        char    bssid[6];     /* The BSSID of an access point. */
        char    channel;      /* The RF frequency, 1-13 */
        uint8_t security;     /* Security type, @ref wlan_sec_type_t */
    } ap_list[WIFI_SCAN_HOTSPOT_LIMIT];
} lega_wlan_scan_result_store_t;

extern lega_semaphore_t lega_wlan_vendor_scan_semaphore;
extern lega_wlan_cb_scan_compeleted lega_wlan_vendor_scan_comp_callback;
lega_wlan_scan_result_store_t *lega_wlan_scan_result_store_ptr = NULL;
void ScanCompCallback(lega_wlan_scan_result_t *scan_result)
{
    int i;

    printf("%s\r\n", __func__);

    if (lega_wlan_scan_result_store_ptr == NULL) {
        printf("%s fail\r\n", __func__);
        return;
    }

    lega_wlan_scan_result_store_ptr->is_scan_adv = scan_result->is_scan_adv;
    for (i = 0; (i < scan_result->ap_num) && (i < WIFI_SCAN_HOTSPOT_LIMIT); i++) {
        memcpy(&(lega_wlan_scan_result_store_ptr->ap_list[i]), scan_result->ap_list,
               sizeof(lega_wlan_scan_result_store_ptr->ap_list[i]));
    }
    lega_wlan_scan_result_store_ptr->ap_num = i;
}

WifiErrorCode Scan(void)
{
    int ret;

    printf("%s\r\n", __func__);

    if (current_iftype != STA_MODE_E) {
        return ERROR_WIFI_IFACE_INVALID;
    }

    if (lega_wlan_scan_result_store_ptr == NULL) {
        lega_wlan_scan_result_store_ptr = lega_rtos_malloc(sizeof(lega_wlan_scan_result_store_t));
    }

    lega_wlan_vendor_scan_comp_callback = ScanCompCallback;

    lega_rtos_init_semaphore(&lega_wlan_vendor_scan_semaphore, 0);
    ret = lega_wlan_start_scan_adv();
    if (ret) {
        printf("%s fail\r\n", __func__);
        lega_rtos_deinit_semaphore(&lega_wlan_vendor_scan_semaphore);
        lega_wlan_vendor_scan_semaphore = 0;
        return ERROR_WIFI_UNKNOWN;
    }
    lega_rtos_get_semaphore(&lega_wlan_vendor_scan_semaphore, LEGA_NEVER_TIMEOUT);
    lega_rtos_deinit_semaphore(&lega_wlan_vendor_scan_semaphore);
    lega_wlan_vendor_scan_semaphore = 0;

    return WIFI_SUCCESS;
}

WifiErrorCode AdvanceScan(WifiScanParams *params)
{
    int ret;
    int channel = 0;
    char *ssid_ptr = 0;
    char *bssid_ptr = 0;
    char bssid_null[WIFI_MAC_LEN] = {0};

    printf("%s\r\n", __func__);

    if (current_iftype != STA_MODE_E) {
        return ERROR_WIFI_IFACE_INVALID;
    }

    if (params == NULL) {
        return ERROR_WIFI_UNKNOWN;
    }

    if ((params->scanType < WIFI_FREQ_SCAN) || (params->scanType > WIFI_BAND_SCAN)) {
        // return WIFI_SUCCESS;
        // adapter for xts
        params->scanType = WIFI_BAND_SCAN;
    }

    if (params->scanType == WIFI_FREQ_SCAN) {
        channel = WifiFreqToChannel(params->freqs);
        if (channel == 0) {
            return ERROR_WIFI_UNKNOWN;
        }
    }

    if (params->scanType == WIFI_SSID_SCAN) {
        if (params->ssidLen == 0) {
            return ERROR_WIFI_UNKNOWN;
        }
        ssid_ptr = lega_rtos_malloc(WIFI_MAX_SSID_LEN);
        if (ssid_ptr) {
            memcpy(ssid_ptr, params->ssid, WIFI_MAX_SSID_LEN);
        }
    }

    if (params->scanType == WIFI_BSSID_SCAN) {
        if (memcmp(bssid_null, params->bssid, 6) == 0) {
            return ERROR_WIFI_UNKNOWN;
        }
        bssid_ptr = lega_rtos_malloc(WIFI_MAC_LEN);
        if (bssid_ptr) {
            memcpy(bssid_ptr, params->bssid, WIFI_MAC_LEN);
        }
    }

    if (lega_wlan_scan_result_store_ptr == NULL) {
        lega_wlan_scan_result_store_ptr = lega_rtos_malloc(sizeof(lega_wlan_scan_result_store_t));
    }

    lega_wlan_vendor_scan_comp_callback = ScanCompCallback;

    lega_rtos_init_semaphore(&lega_wlan_vendor_scan_semaphore, 0);

    ret = lega_wlan_start_scan_detail(ssid_ptr, channel, bssid_ptr);

    if (ssid_ptr) {
        lega_rtos_free(ssid_ptr);
    }

    if (bssid_ptr) {
        lega_rtos_free(bssid_ptr);
    }

    if (ret) {
        printf("%s fail\r\n", __func__);
        lega_rtos_deinit_semaphore(&lega_wlan_vendor_scan_semaphore);
        lega_wlan_vendor_scan_semaphore = 0;
        return ERROR_WIFI_UNKNOWN;
    }
    lega_rtos_get_semaphore(&lega_wlan_vendor_scan_semaphore, LEGA_NEVER_TIMEOUT);
    lega_rtos_deinit_semaphore(&lega_wlan_vendor_scan_semaphore);
    lega_wlan_vendor_scan_semaphore = 0;

    return WIFI_SUCCESS;
}

WifiErrorCode GetScanInfoList(WifiScanInfo *result, unsigned int *size)
{
    int i;

    printf("%s\r\n", __func__);

    if (lega_wlan_scan_result_store_ptr == NULL) {
        *size = 0;
        return WIFI_SUCCESS;
    }

    for (i = 0; i < lega_wlan_scan_result_store_ptr->ap_num; i++) {
        result->band = 0;
        result->frequency = lega_wlan_scan_result_store_ptr->ap_list[i].channel;
        result->rssi = 0;
        result->securityType = lega_wlan_scan_result_store_ptr->ap_list[i].security;
        memcpy(result->ssid, lega_wlan_scan_result_store_ptr->ap_list[i].ssid, WIFI_MAX_SSID_LEN);
        memcpy(result->bssid, lega_wlan_scan_result_store_ptr->ap_list[i].bssid, WIFI_MAC_LEN);
    }
    *size = i;

    lega_rtos_free(lega_wlan_scan_result_store_ptr);
    lega_wlan_scan_result_store_ptr = NULL;

    return WIFI_SUCCESS;
}

uint8_t lega_wifi_device_config_flag_array[WIFI_MAX_CONFIG_SIZE] = {0};
WifiDeviceConfig lega_wifi_device_config_array[WIFI_MAX_CONFIG_SIZE] = {0};
// "wifi_device_config_flag_array"  "wifi_device_config_array"
WifiErrorCode AddDeviceConfig(const WifiDeviceConfig *config, int *result)
{
    int i;
    int32_t len;

    printf("%s\r\n", __func__);

    len = sizeof(WifiDeviceConfig) * WIFI_MAX_CONFIG_SIZE;
    duet_flash_kv_get("wifi_device_config_array", lega_wifi_device_config_array, &len);
    len = WIFI_MAX_CONFIG_SIZE;
    duet_flash_kv_get("wifi_device_config_flag_array", lega_wifi_device_config_flag_array, &len);

    for (i = 0; i < WIFI_MAX_CONFIG_SIZE; i++) {
        if (lega_wifi_device_config_flag_array[i] == 0) {
            break;
        }
    }

    if (i >= WIFI_MAX_CONFIG_SIZE) {
        printf("%s ERROR_WIFI_BUSY\r\n", __func__);
        return ERROR_WIFI_BUSY;
    }

    lega_wifi_device_config_flag_array[i] = 1;
    memcpy(&lega_wifi_device_config_array[i], config, sizeof(WifiDeviceConfig));
    // netID == 0, means invalid
    lega_wifi_device_config_array[i].netId = i + 1;;
    *result = i + 1;

    duet_flash_kv_set("wifi_device_config_array", lega_wifi_device_config_array,
                      sizeof(WifiDeviceConfig)*WIFI_MAX_CONFIG_SIZE, 1);
    duet_flash_kv_set("wifi_device_config_flag_array", lega_wifi_device_config_flag_array, WIFI_MAX_CONFIG_SIZE, 1);

    return WIFI_SUCCESS;
}

WifiErrorCode GetDeviceConfigs(WifiDeviceConfig *result, unsigned int *size)
{
    int i;
    int32_t len;

    printf("%s %d\r\n", __func__, WIFI_MAX_CONFIG_SIZE);

    len = sizeof(WifiDeviceConfig) * WIFI_MAX_CONFIG_SIZE;
    duet_flash_kv_get("wifi_device_config_array", lega_wifi_device_config_array, &len);
    len = WIFI_MAX_CONFIG_SIZE;
    duet_flash_kv_get("wifi_device_config_flag_array", lega_wifi_device_config_flag_array, &len);

    for (i = 0; i < WIFI_MAX_CONFIG_SIZE; i++) {
        printf("%s flag: %d\r\n", __func__, lega_wifi_device_config_flag_array[i]);
        if (lega_wifi_device_config_flag_array[i]) {
            break;
        }
    }

    if (i >= WIFI_MAX_CONFIG_SIZE) {
        printf("%s not found\r\n", __func__);
        memset(result, 0, sizeof(WifiDeviceConfig)*WIFI_MAX_CONFIG_SIZE);
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    memcpy(result, lega_wifi_device_config_array, sizeof(WifiDeviceConfig)*WIFI_MAX_CONFIG_SIZE);
    *size = WIFI_MAX_CONFIG_SIZE;

    return WIFI_SUCCESS;
}

WifiErrorCode RemoveDevice(int networkId)
{
    printf("%s %d\r\n", __func__, networkId);

    if ((networkId <= 0) || (networkId > WIFI_MAX_CONFIG_SIZE)) {
        printf("%s ERROR_WIFI_NOT_AVAILABLE\r\n", __func__);
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    lega_wifi_device_config_flag_array[networkId - 1] = 0;
    memset(&lega_wifi_device_config_array[networkId - 1], 0, sizeof(WifiDeviceConfig));

    duet_flash_kv_set("wifi_device_config_array", lega_wifi_device_config_array,
                      sizeof(WifiDeviceConfig)*WIFI_MAX_CONFIG_SIZE, 1);
    duet_flash_kv_set("wifi_device_config_flag_array", lega_wifi_device_config_flag_array, WIFI_MAX_CONFIG_SIZE, 1);

    return WIFI_SUCCESS;
}

void lalala_sta_device_config_set(void)
{
    WifiDeviceConfig config;
    int result = 1;
    memcpy(config.ssid, "AP2311", sizeof("AP2311"));
    memcpy(config.preSharedKey, "12345678", sizeof("12345678"));
    RemoveDevice(result);
    AddDeviceConfig(&config, &result);
    RemoveDevice(result);
}

WifiErrorCode ConnectTo(int networkId)
{
    lega_wlan_init_type_t init_param = {0};
    int ret;

    printf("%s\r\n", __func__);

    if (current_iftype != STA_MODE_E) {
        return ERROR_WIFI_NOT_STARTED;
    }

    if ((networkId <= 0) || (networkId > WIFI_MAX_CONFIG_SIZE)) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    if (lega_wifi_device_config_flag_array[networkId - 1]) {
        init_param.wifi_mode = STA;
        memcpy(init_param.wifi_ssid, lega_wifi_device_config_array[networkId - 1].ssid, sizeof(init_param.wifi_ssid));
        memcpy(init_param.wifi_key,  lega_wifi_device_config_array[networkId - 1].preSharedKey, sizeof(init_param.wifi_key));
        if ((lega_wifi_device_config_array[networkId - 1].freq >= 1)
            && (lega_wifi_device_config_array[networkId - 1].freq <= 14)) {
            init_param.channel = (char)lega_wifi_device_config_array[networkId - 1].freq;
        }
        ret = lega_wlan_open(&init_param);
        if (ret == 0) {
            return WIFI_SUCCESS;
        } else {
            return ERROR_WIFI_UNKNOWN;
        }
    } else {
        return ERROR_WIFI_NOT_AVAILABLE;
    }
}

WifiErrorCode Disconnect(void)
{
    int ret;

    printf("%s\r\n", __func__);

    if (current_iftype != STA_MODE_E) {
        return ERROR_WIFI_NOT_STARTED;
    }

    ret = lega_wlan_suspend_sta();
    if (ret == 0) {
        return WIFI_SUCCESS;
    } else {
        return ERROR_WIFI_UNKNOWN;
    }
}

WifiErrorCode GetLinkedInfo(WifiLinkedInfo *result)
{
    int ret;
    lega_wlan_link_stat_t link_status;
    lega_wlan_ip_stat_t *stat;
    ip_addr_t ipaddr;

    printf("%s\r\n", __func__);

    if ((current_iftype != STA_MODE_E) || !result) {
        return ERROR_WIFI_NOT_STARTED;
    }

    ret = lega_wlan_get_link_status(&link_status);

    if (ret) {
        return ERROR_WIFI_UNKNOWN;
    }

    result->connState = link_status.is_connected;
    memcpy(result->ssid, link_status.ssid, WIFI_MAX_SSID_LEN);
    memcpy(result->bssid, link_status.bssid, WIFI_MAC_LEN);
    result->rssi = link_status.wifi_strength;

    stat = lega_wlan_get_ip_status();

    if (stat == NULL) {
        return ERROR_WIFI_UNKNOWN;
    } else {
        inet_aton(stat->ip, &ipaddr);
        result->ipAddress = *(int *)(&ipaddr);
        return WIFI_SUCCESS;
    }
}

WifiErrorCode GetDeviceMacAddress(unsigned char *result)
{
    int ret;

    printf("%s\r\n", __func__);

    ret = lega_wlan_get_mac_address(result);
    if (ret == 0) {
        return WIFI_SUCCESS;
    } else {
        return ERROR_WIFI_UNKNOWN;
    }
}

static int is_wifi_connected = 0;
int lega_wlan_get_connected_status(void)
{
    return is_wifi_connected;
}

WifiEvent *lega_wifi_event_ptr = 0;
#ifdef AOS_COMP_MSMART
extern void wlan_service_event(lega_wlan_event_e event);
#else
void wlan_service_event(lega_wlan_event_e event)
{

}
#endif
#ifdef CFG_MRFOTA_TEST
extern uint8_t rfota_wifi_test;
#endif
void wifi_event_cb(lega_wlan_event_e evt, void *info)
{
    printf("w_evt_cb 0x%x\n", evt);
#ifdef CFG_MRFOTA_TEST
    if (rfota_wifi_test == 1) {
        if (evt == WLAN_EVENT_IP_GOT) {
            lega_wlan_ip_stat_t *p_in_ip_stat = (lega_wlan_ip_stat_t *)info;
            printf("Got ip: %s, gw: %s, mask: %s\n", p_in_ip_stat->ip, p_in_ip_stat->gate, p_in_ip_stat->mask);
        }
        return;
    }
#endif
    switch (evt) {
        case WLAN_EVENT_SCAN_COMPLETED:
            if ((lega_wifi_event_ptr == NULL) || (lega_wifi_event_ptr->OnWifiScanStateChanged == NULL)) {
                return;
            }
            lega_wifi_event_ptr->OnWifiScanStateChanged(1, 0);
            break;
        case WLAN_EVENT_ASSOCIATED:
            is_wifi_connected = 0;
            break;
        case WLAN_EVENT_CONNECTED:
            if ((lega_wifi_event_ptr == NULL) || (lega_wifi_event_ptr->OnWifiScanStateChanged == NULL)) {
                return;
            }
            lega_wifi_event_ptr->OnWifiScanStateChanged(1, 0);
            break;
        case WLAN_EVENT_IP_GOT: {
            lega_wlan_ip_stat_t *p_in_ip_stat = (lega_wlan_ip_stat_t *)info;
            printf("Got ip: %s, gw: %s, mask: %s\r\n", p_in_ip_stat->ip, p_in_ip_stat->gate, p_in_ip_stat->mask);
            /* enable wifi power save */
            lega_wlan_set_ps_mode(1);
#ifdef CFG_DATA_ELEM
            lega_wlan_dataelem_start(0);
#endif
            wlan_service_event(WLAN_EVENT_IP_GOT);
            is_wifi_connected = 1;
        }
        break;
        case WLAN_EVENT_DISCONNECTED:
            if ((lega_wifi_event_ptr) && (lega_wifi_event_ptr->OnWifiScanStateChanged)) {
                lega_wifi_event_ptr->OnWifiScanStateChanged(0, 0);
            }
            wlan_service_event(WLAN_EVENT_DISCONNECTED);
            is_wifi_connected = 0;
            break;
        case WLAN_EVENT_AP_UP:
            if ((lega_wifi_event_ptr) && (lega_wifi_event_ptr->OnHotspotStateChanged)) {
                lega_wifi_event_ptr->OnHotspotStateChanged(1);
            }
            wlan_service_event(WLAN_EVENT_AP_UP);
            break;
        case WLAN_EVENT_AP_DOWN:
            if ((lega_wifi_event_ptr) && (lega_wifi_event_ptr->OnHotspotStateChanged)) {
                lega_wifi_event_ptr->OnHotspotStateChanged(0);
            }
            wlan_service_event(WLAN_EVENT_AP_DOWN);
            break;
        case WLAN_EVENT_CONNECT_FAILED:
            if ((lega_wifi_event_ptr) && (lega_wifi_event_ptr->OnWifiScanStateChanged)) {
                lega_wifi_event_ptr->OnWifiScanStateChanged(0, 0);
            }
            wlan_service_event(WLAN_EVENT_CONNECT_FAILED);
            break;
        case WLAN_EVENT_SCAN_FAILED:
            if ((lega_wifi_event_ptr == NULL) || (lega_wifi_event_ptr->OnWifiScanStateChanged == NULL)) {
                return;
            }
            lega_wifi_event_ptr->OnWifiScanStateChanged(0, 0);
            break;
        case WLAN_EVENT_AP_PEER_UP: {
            if ((lega_wifi_event_ptr == NULL) || (lega_wifi_event_ptr->OnHotspotStaJoin == NULL)) {
                return;
            }
            StationInfo sta_info = {0};
            sta_info.disconnectedReason = 0;
            memcpy(sta_info.macAddress, ((lega_wlan_client_addr_info_t *)info)->sta_mac_addr, WIFI_MAC_LEN);
            sta_info.ipAddress = ((lega_wlan_client_addr_info_t *)info)->sta_ip_addr;
            lega_wifi_event_ptr->OnHotspotStaJoin(&sta_info);
        }
        break;
        case WLAN_EVENT_AP_PEER_DOWN: {
            if ((lega_wifi_event_ptr == NULL) || (lega_wifi_event_ptr->OnHotspotStaLeave == NULL)) {
                return;
            }
            StationInfo sta_info = {0};
            sta_info.disconnectedReason = 0;
            memcpy(sta_info.macAddress, ((lega_wlan_client_addr_info_t *)info)->sta_mac_addr, WIFI_MAC_LEN);
            sta_info.ipAddress = ((lega_wlan_client_addr_info_t *)info)->sta_ip_addr;
            lega_wifi_event_ptr->OnHotspotStaLeave(&sta_info);
        }
        break;
        default:
            // lega_wifi_event_ptr->OnHotspotStaJoin
            // lega_wifi_event_ptr->OnHotspotStaLeave
            printf("WiFi HAL %s EVENT[%d] not implemeted yet!\r\n", __func__, evt);
            break;
    }
}

WifiErrorCode RegisterWifiEvent(WifiEvent *event)
{
    printf("%s\r\n", __func__);

    lega_wifi_event_ptr = event;

    return WIFI_SUCCESS;
}

WifiErrorCode UnRegisterWifiEvent(const WifiEvent *event)
{
    printf("%s\r\n", __func__);

    if (lega_wifi_event_ptr == event) {
        lega_wifi_event_ptr = 0;
    } else {
        printf("%s fail 0x%x 0x%x\r\n", __func__, (unsigned int)lega_wifi_event_ptr, (unsigned int)event);
    }

    return WIFI_SUCCESS;
}

HotspotConfig lega_hotspot_config = {0};
uint8_t lega_hotspot_config_flag = 0;
// "hotspot_config" "hotspot_config_flag"
WifiErrorCode EnableHotspot(void)
{
    int ret;
    lega_wlan_init_type_t init_param = {0};

    printf("%s\r\n", __func__);

    if ((current_iftype != 0xFF) || (lega_hotspot_config_flag == 0)) {
        return ERROR_WIFI_BUSY;
    }

    init_param.wifi_mode = SOFTAP;
    memcpy(init_param.wifi_ssid, lega_hotspot_config.ssid, 32);
    memcpy(init_param.wifi_key,  lega_hotspot_config.preSharedKey, 64);
    init_param.channel = lega_hotspot_config.channelNum;
    ret = lega_wlan_open(&init_param);
    if (ret) {
        return ERROR_WIFI_UNKNOWN;
    } else {
        return WIFI_SUCCESS;
    }
}

extern lega_semaphore_t lega_wlan_vendor_close_ap_semaphore;
WifiErrorCode DisableHotspot(void)
{
    printf("%s\r\n", __func__);

    if (current_iftype != SAP_MODE_E) {
        return ERROR_WIFI_NOT_STARTED;
    }
    lega_rtos_init_semaphore(&lega_wlan_vendor_close_ap_semaphore, 0);
    lega_wlan_close();
    if (lega_rtos_get_semaphore(&lega_wlan_vendor_close_ap_semaphore, LEGA_NEVER_TIMEOUT)) {
        // MS_LOGI("%s timeout\n",__FUNCTION__);
    }
    lega_rtos_deinit_semaphore(&lega_wlan_vendor_close_ap_semaphore);
    lega_wlan_vendor_close_ap_semaphore = 0;

    return WIFI_SUCCESS;
}

WifiErrorCode SetHotspotConfig(const HotspotConfig *config)
{
    printf("%s\r\n", __func__);

    lega_hotspot_config_flag = 1;
    lega_hotspot_config = *config;

    duet_flash_kv_set("hotspot_config", &lega_hotspot_config, sizeof(HotspotConfig), 1);
    duet_flash_kv_set("hotspot_config_flag", &lega_hotspot_config_flag, 1, 1);
    return WIFI_SUCCESS;
}

WifiErrorCode GetHotspotConfig(HotspotConfig *result)
{
    int32_t len;
    printf("%s\r\n", __func__);

    len = sizeof(HotspotConfig);
    duet_flash_kv_get("hotspot_config", &lega_hotspot_config, &len);
    len = 1;
    duet_flash_kv_get("hotspot_config_flag", &lega_hotspot_config_flag, &len);

    if (lega_hotspot_config_flag) {
        lega_hotspot_config.band = HOTSPOT_BAND_TYPE_2G;
        *result = lega_hotspot_config;
        return WIFI_SUCCESS;
    } else {
        return ERROR_WIFI_UNKNOWN;
    }
}

static int last_set_band = 0;
WifiErrorCode SetBand(int band)
{
    printf("%s\r\n", __func__);

    if (band != HOTSPOT_BAND_TYPE_2G) {
        last_set_band = band;
        return ERROR_WIFI_NOT_SUPPORTED;
    }

    lega_hotspot_config.band = band;

    return WIFI_SUCCESS;
}

WifiErrorCode GetBand(int *result)
{
    printf("%s\r\n", __func__);

    if (last_set_band != 0) {
        *result = last_set_band;
        last_set_band = 0;
        return ERROR_WIFI_UNKNOWN;
    }

    if (lega_hotspot_config.band == 0) {
        return ERROR_WIFI_NOT_AVAILABLE;
    }

    *result = HOTSPOT_BAND_TYPE_2G;

    return WIFI_SUCCESS;
}

int IsHotspotActive(void)
{
    printf("%s\r\n", __func__);

    if (current_iftype == SAP_MODE_E) {
        return WIFI_STATE_AVAILABLE;
    } else {
        return WIFI_STATE_NOT_AVAILABLE;
    }
}

WifiErrorCode GetStationList(StationInfo *result, unsigned int *size)
{
    int i;
    lega_wlan_ap_client_info_t client_info;

    printf("%s\r\n", __func__);

    lega_get_client_ip_mac(&client_info);

    printf("%s : %d\r\n", __func__, client_info.client_num);

    *size = client_info.client_num;
    for (i = 0; i < client_info.client_num; i++) {
        memcpy(result[i].macAddress, client_info.sta[i].sta_mac_addr, WIFI_MAC_LEN);
        result[i].disconnectedReason = 0;
    }

    return WIFI_STATE_NOT_AVAILABLE;
}

#define RSSI_LEVEL_4_2_G (-65)
#define RSSI_LEVEL_3_2_G (-75)
#define RSSI_LEVEL_2_2_G (-82)
#define RSSI_LEVEL_1_2_G (-88)

#define RSSI_LEVEL_4_5_G (-65)
#define RSSI_LEVEL_3_5_G (-72)
#define RSSI_LEVEL_2_5_G (-79)
#define RSSI_LEVEL_1_5_G (-85)

int GetSignalLevel(int rssi, int band)
{
    printf("%s\r\n", __func__);

    if (band == HOTSPOT_BAND_TYPE_2G) {
        if (rssi >= RSSI_LEVEL_4_2_G) {
            return RSSI_LEVEL_4;
        }
        if (rssi >= RSSI_LEVEL_3_2_G) {
            return RSSI_LEVEL_3;
        }
        if (rssi >= RSSI_LEVEL_2_2_G) {
            return RSSI_LEVEL_2;
        }
        if (rssi >= RSSI_LEVEL_1_2_G) {
            return RSSI_LEVEL_1;
        }
    }

    if (band == HOTSPOT_BAND_TYPE_5G) {
        if (rssi >= RSSI_LEVEL_4_5_G) {
            return RSSI_LEVEL_4;
        }
        if (rssi >= RSSI_LEVEL_3_5_G) {
            return RSSI_LEVEL_3;
        }
        if (rssi >= RSSI_LEVEL_2_5_G) {
            return RSSI_LEVEL_2;
        }
        if (rssi >= RSSI_LEVEL_1_5_G) {
            return RSSI_LEVEL_1;
        }
    }

    return ERROR_WIFI_INVALID_ARGS;
}

int lega_wlan_get_ip_addr(char *ip, int len)
{
    lega_wlan_ip_stat_t *stat;

    stat = lega_wlan_get_ip_status();

    if (stat == NULL) {
        return -1;
    } else {
        memcpy(ip, stat->ip, len);
        return 0;
    }
}

extern int lega_wlan_softap_deauth_peer(uint8_t *mac);
WifiErrorCode DisassociateSta(unsigned char *mac, int macLen)
{
    lega_wlan_softap_deauth_peer(mac);
    return WIFI_SUCCESS;
}

WifiErrorCode AddTxPowerInfo(int power)
{
    return ERROR_WIFI_UNKNOWN;
}

/** @} */
