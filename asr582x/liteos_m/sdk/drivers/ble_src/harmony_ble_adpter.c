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

#include "string.h"
#include "lega_wlan_api.h"
#include "sonata_ble_api.h"
#include "app.h"
#include "sonata_utils_api.h"
#include "sonata_gap_api.h"
#include "sonata_gap.h"

#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"
#include "ohos_bt_gatt_client.h"

static BtGattServerCallbacks  gatts_callback = {0};
static BtGattCallbacks        bt_gattcallback = {0};

#define print_log printf
#define BT_STATUS_SUCCESS (0)
#define BT_STATUS_FAIL    (-1)

#define ATT_BT_SIG_UUID_128 {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
        0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_SIG_UUID_128_LEN                        0x0010
#define ATT_SIG_UUID_16_LEN                         0x0002

int BleGattsSetEncryption(BdAddr bdAddr, BleSecAct secAct);

int BleGattsSendIndication(int serverId, GattsSendIndParam *param);
static int8_t test_value[5] = {0x2, 0x02, 0x02, 0x04, 0x05};
int test_handle = 0;

void test_ind(void)
{
    GattsSendIndParam indParam = {
        .connectId = 0,
        .attrHandle = test_handle  + 2,
        .confirm = 1,
        .valueLen = 5,
        .value = test_value,

    };

    static uint8_t count = 0;
    count++;
    test_value[4] = count;
    BleGattsSendIndication(0, &indParam);
}

int app_core_evt_ind_cb_h(app_core_evt_ind_t evt, void *p_param)
{

    print_log("harmony : %s evt %d\r\n", __FUNCTION__, evt);
    switch (evt) {
        case BLE_SERVICE_ADD_CMP:
            if (gatts_callback.serviceStartCb != NULL) {
                app_reg_service_cmp_t *param = (app_reg_service_cmp_t *)p_param;
                gatts_callback.serviceStartCb(param->status, 0, param->handler);
            }
            break;
        case  BLE_DEV_CONNECTED:
            if (gatts_callback.connectServerCb != NULL) {
                app_connect_status_ind_t *param = (app_connect_status_ind_t *)p_param;
                gatts_callback.connectServerCb(param->connId, 0, (BdAddr *)param->addr);
            }
            break;
        case BLE_DEV_DISCONNECTED:
            if (gatts_callback.disconnectServerCb != NULL) {
                app_connect_status_ind_t *param = (app_connect_status_ind_t *)p_param;
                gatts_callback.disconnectServerCb(param->connId, 0, (BdAddr *)param->addr);
            }
            break;
        case BLE_IND_SENT:
            if (gatts_callback.indicationSentCb != NULL) {
                app_ind_sent_ind_t *param = (app_ind_sent_ind_t *)p_param;
                gatts_callback.indicationSentCb(param->connId, param->status);
            }
            break;
        case BLE_MTU_CHANGE:
            if (gatts_callback.mtuChangeCb != NULL) {
                app_mtu_change_ind_t *param = (app_mtu_change_ind_t *)p_param;
                gatts_callback.mtuChangeCb(param->connId, param->mtu);
            }
            break;
        case BLE_ADV_START:
            if (bt_gattcallback.advEnableCb != NULL) {
                app_adv_status_ind_t *param = (app_adv_status_ind_t *)p_param;
                bt_gattcallback.advEnableCb(param->advId, param->status);
            }
            break;
        case BLE_ADV_STOP:
            if (bt_gattcallback.advDisableCb != NULL) {
                app_adv_status_ind_t *param = (app_adv_status_ind_t *)p_param;
                bt_gattcallback.advDisableCb(param->advId, param->status);
            }
            break;
        default :
            break;
    }
    return 0;
}

/* Initialize the Bluetooth protocol stack. */
int InitBtStack(void)
{
    return BT_STATUS_SUCCESS;
}

/* Bluetooth protocol stack enable */
int EnableBtStack(void)
{
    print_log("harmony : %s \r\n", __FUNCTION__);
    app_register_core_evt_ind(app_core_evt_ind_cb_h);
    app_set_connect_flag(1);
    app_ble_stack_start(USER_OHOS_MODULE_ID);
    return BT_STATUS_SUCCESS;
}

/* Bluetooth protocol stack disable */
int DisableBtStack(void)
{
    print_log("harmony : %s \r\n", __FUNCTION__);
    app_register_core_evt_ind(NULL);
    app_set_connect_flag(0);
    app_ble_stack_stop(USER_OHOS_MODULE_ID);
    return BT_STATUS_SUCCESS;
}

int SetDeviceName(const char *name, unsigned int len)
{
    print_log("harmony : %s \r\n", __FUNCTION__);
    app_ble_set_device_name((uint8_t *)name, len);
    return BT_STATUS_SUCCESS;
}

static uint8_t io_cap_h2a(BleIoCapMode mode)
{
    uint8_t io_Cap_temp = mode;
    return io_Cap_temp;
}

int BleSetSecurityIoCap(BleIoCapMode mode)
{
    uint8_t app_io_cap = io_cap_h2a(mode);
    app_set_security_io_cap(app_io_cap);
    return BT_STATUS_SUCCESS;
}

static uint8_t sec_auth_req_h2a(BleAuthReqMode mode)
{
    uint8_t auth_req_temp ;
    switch (mode) {
        case OHOS_BLE_AUTH_NO_BOND:
            auth_req_temp = SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND;
            break;
        case OHOS_BLE_AUTH_BOND:
            auth_req_temp = SONATA_GAP_AUTH_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_MITM:
            auth_req_temp = SONATA_GAP_AUTH_MITM;
            break;
        case OHOS_BLE_AUTH_REQ_SC_ONLY:
            auth_req_temp = SONATA_GAP_AUTH_SEC_CON;
            break;
        case OHOS_BLE_AUTH_REQ_SC_BOND:
            auth_req_temp = SONATA_GAP_AUTH_REQ_SEC_CON_BOND ;
            break;
        case OHOS_BLE_AUTH_REQ_SC_MITM:
            auth_req_temp = SONATA_GAP_AUTH_REQ_SEC_CON_NO_BOND;
            break;
        case OHOS_BLE_AUTH_REQ_SC_MITM_BOND:
            auth_req_temp = SONATA_GAP_AUTH_REQ_SEC_CON_BOND;
            break;
    }
    return auth_req_temp;
}

int BleSetSecurityAuthReq(BleAuthReqMode mode)
{
    print_log("harmony : %s  %d\r\n", __FUNCTION__, mode);
    uint8_t auth_req = sec_auth_req_h2a(mode);
    app_set_security_auth_req(auth_req);
    return BT_STATUS_SUCCESS;
}

int BleGattSecurityRsp(BdAddr bdAddr, bool accept)
{
    print_log("harmony : %s  %d\r\n", __FUNCTION__, accept);
    app_gap_notify_pair_request_rsp((uint8_t *)bdAddr.addr, accept);
    return BT_STATUS_SUCCESS;
}

int BleGattsDisconnect(int serverId, BdAddr bdAddr, int connId)
{
    if (0 != app_ble_disconnect_by_addr((uint8_t *)bdAddr.addr)) {
        return BT_STATUS_FAIL;
    }
    return BT_STATUS_SUCCESS;
}

static void perm_2_perm(uint8_t user_perm, uint8_t user_prop, uint16_t *p_perm)
{
    if ((user_prop  & OHOS_GATT_CHARACTER_PROPERTY_BIT_READ) == OHOS_GATT_CHARACTER_PROPERTY_BIT_READ) {
        if ((user_perm & OHOS_GATT_PERMISSION_READ) == OHOS_GATT_PERMISSION_READ) {
            *p_perm |= PRD_NA;
        }
        if ((user_perm & OHOS_GATT_PERMISSION_READ_ENCRYPTED) == OHOS_GATT_PERMISSION_READ_ENCRYPTED) {
            *p_perm |= PRD_UA;
        }
    }

    if ((user_prop  & OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE) == OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE) {
        if ((user_perm & OHOS_GATT_PERMISSION_WRITE) == OHOS_GATT_PERMISSION_WRITE) {
            *p_perm |= PWR_NA;
        }
        if ((user_perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) == OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) {
            *p_perm |= PWR_UA;
        }
    }
    if ((user_prop  & OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP) == OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP) {
        if ((user_perm & OHOS_GATT_PERMISSION_WRITE) == OHOS_GATT_PERMISSION_WRITE) {
            *p_perm |= PWC_NA;
        }
        if ((user_perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) == OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) {
            *p_perm |= PWC_UA;
        }
    }
    if ((user_prop  & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY) == OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY) {
        if ((user_perm & OHOS_GATT_PERMISSION_READ_ENCRYPTED) == OHOS_GATT_PERMISSION_READ_ENCRYPTED
            || (user_perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) == OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) {
            *p_perm |= PNTF_UA;
        } else {
            *p_perm |= PNTF_NA;
        }

    }
    if ((user_prop  & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE) == OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE) {
        if ((user_perm & OHOS_GATT_PERMISSION_READ_ENCRYPTED) == OHOS_GATT_PERMISSION_READ_ENCRYPTED
            || (user_perm & OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) == OHOS_GATT_PERMISSION_WRITE_ENCRYPTED) {
            *p_perm |= PIND_UA;
        } else {
            *p_perm |= PIND_NA;
        }
    }

    print_log("[Harmony]user_perm= %x prop = %x out : %x\r\n", user_perm, user_prop, *p_perm);
}

ble_gatt_att_reg_t   *ble_ohos_add_service(ble_gatt_att_reg_t *dst_attr, BleGattAttr *service_attr)
{
    if (service_attr->attrType != OHOS_BLE_ATTRIB_TYPE_SERVICE) {
        return dst_attr;
    }
    if (service_attr->uuidType == OHOS_UUID_TYPE_128_BIT) {
        print_log("[Harmony]service  %x %x %x %x\r\n", service_attr->uuid[0], service_attr->uuid[1], service_attr->uuid[2],
                  service_attr->uuid[3]);
        memmove(dst_attr->att_desc.uuid, service_attr->uuid, sizeof(dst_attr->att_desc.uuid));
        SONATA_PERM_SET(dst_attr->att_desc.ext_perm, UUID_LEN, 2);
        SONATA_PERM_SET(dst_attr->att_desc.perm, SVC_UUID_LEN, 2);
    } else if (service_attr->uuidType == OHOS_UUID_TYPE_16_BIT) {
        memmove(dst_attr->att_desc.uuid, service_attr->uuid, 2);
    }
    return dst_attr;
}

ble_gatt_att_reg_t *ble_ohos_add_char(ble_gatt_att_reg_t *dst_attr, BleGattAttr *char_attr, bool isLast)
{
    if (char_attr->attrType != OHOS_BLE_ATTRIB_TYPE_CHAR) {
        return dst_attr;
    }
    print_log("[Harmony]char  %x %x %x %x\r\n", char_attr->uuid[0], char_attr->uuid[1], char_attr->uuid[2],
              char_attr->uuid[3]);

    uint8_t char_uuid[ATT_SIG_UUID_128_LEN] = {0x03, 0x28, 0x0};

    memmove(dst_attr->att_desc.uuid, char_uuid, ATT_SIG_UUID_128_LEN);

    dst_attr->att_desc.perm = PRD_NA;
    dst_attr->att_desc.max_len = 0;
    dst_attr->att_desc.ext_perm = 0;

    dst_attr++;

    if (char_attr->uuidType == OHOS_UUID_TYPE_128_BIT) {
        memmove(dst_attr->att_desc.uuid, char_attr->uuid, sizeof(dst_attr->att_desc.uuid));
        SONATA_PERM_SET(dst_attr->att_desc.ext_perm, UUID_LEN, 2);
    } else if (char_attr->uuidType == OHOS_UUID_TYPE_16_BIT) {
        memmove(dst_attr->att_desc.uuid, char_attr->uuid, 2);
    }
    dst_attr->att_opr.ind_cb = char_attr->func.indicate;
    dst_attr->att_opr.read_request = char_attr->func.read;
    dst_attr->att_opr.write_request = char_attr->func.write;
    perm_2_perm(char_attr->permission, char_attr->properties, &dst_attr->att_desc.perm);
    dst_attr->att_desc.ext_perm |= PRI;
    dst_attr->att_desc.max_len = 512;

    if (((char_attr->properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE) == OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE
         || (char_attr->properties & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY) == OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY)
         && isLast) {
        dst_attr++;

        uint8_t char_uuid2[ATT_SIG_UUID_128_LEN] = {0x02, 0x29, 0x0};
        memmove(dst_attr->att_desc.uuid, char_uuid2, ATT_SIG_UUID_128_LEN);

        dst_attr->att_desc.perm = PRD_NA | PWR_NA;
        dst_attr->att_desc.max_len = 2;
        dst_attr->att_desc.ext_perm |= PRI;
    }

    return dst_attr;
}

int BleGattServiceRead1(unsigned char *buff, unsigned int *len)
{
    print_log("harmony  : %s \r\n", __FUNCTION__);
    uint8_t readvalue[5] = {0x1, 0x2, 0x3, 0x4, 0x5};
    *len = sizeof(readvalue);
    memmove(buff, readvalue, *len);
    test_ind();
    return 0;
}

int BleGattServiceWrite1(unsigned char *buff, unsigned int len)
{
    print_log("harmony  : %s \r\n", __FUNCTION__);
    test_ind();
    return 0;
}

int BleGattServiceIndicate1(unsigned char *buff, unsigned int len)
{
    print_log("harmony  : %s \r\n", __FUNCTION__);
    return 0;
}

BleGattOperateFunc  opr_func = {
    .read = BleGattServiceRead1,
    .write = BleGattServiceWrite1,
    .indicate = BleGattServiceIndicate1,
};

BleGattAttr h_attr1 = {
    .attrType = OHOS_BLE_ATTRIB_TYPE_SERVICE,
    .permission = OHOS_GATT_PERMISSION_READ,
    .uuidType = OHOS_UUID_TYPE_16_BIT,
    .uuid = {0x11, 0x22, 0x33},
    .value = NULL,
    .valLen = 0,
    .properties = 0,
    .func = {0},

};
BleGattAttr h_attr2 = {
    .attrType = OHOS_BLE_ATTRIB_TYPE_CHAR,
    .permission = OHOS_GATT_PERMISSION_READ | OHOS_GATT_PERMISSION_WRITE,
    .uuidType = OHOS_UUID_TYPE_16_BIT,
    .uuid = {0x11, 0x22, 0x66},
    .value = NULL,
    .valLen = 0,
    .properties = OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE | OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,
    .func = {
        .read = BleGattServiceRead1,
        .write = BleGattServiceWrite1,
        .indicate = BleGattServiceIndicate1,
    },

};

BleGattAttr h_attr3 = {
    .attrType = OHOS_BLE_ATTRIB_TYPE_SERVICE,
    .permission = OHOS_GATT_PERMISSION_READ,
    .uuidType = OHOS_UUID_TYPE_16_BIT,
    .uuid = {0x22, 0x22, 0x33},
    .value = NULL,
    .valLen = 0,
    .properties = 0,
    .func = {0},

};
BleGattAttr h_attr4 = {
    .attrType = OHOS_BLE_ATTRIB_TYPE_CHAR,
    .permission = OHOS_GATT_PERMISSION_READ | OHOS_GATT_PERMISSION_WRITE,
    .uuidType = OHOS_UUID_TYPE_16_BIT,
    .uuid = {0x33, 0x52, 0x66},
    .value = NULL,
    .valLen = 0,
    .properties = OHOS_GATT_CHARACTER_PROPERTY_BIT_READ | OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,
    .func = {
        .read = BleGattServiceRead1,
        .write = BleGattServiceWrite1,
        .indicate = BleGattServiceIndicate1,
    },

};

BleGattAttr h_attr5 = {
    .attrType = OHOS_BLE_ATTRIB_TYPE_CHAR,
    .permission = OHOS_GATT_PERMISSION_READ | OHOS_GATT_PERMISSION_WRITE,
    .uuidType = OHOS_UUID_TYPE_16_BIT,
    .uuid = {0x33, 0x88, 0x66},
    .value = NULL,
    .valLen = 0,
    .properties = OHOS_GATT_CHARACTER_PROPERTY_BIT_READ | OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,
    .func = {
        .read = BleGattServiceRead1,
        .write = BleGattServiceWrite1,
        .indicate = BleGattServiceIndicate1,
    },

};

void test_h_add_service(void)
{
    BleGattService service_info;
    BleGattAttr *attrList = malloc(3 * sizeof(BleGattAttr));
    service_info.attrNum = 3;
    service_info.attrList = attrList;
    memmove(attrList, &h_attr1, sizeof(BleGattAttr));
    attrList++;
    memmove(attrList, &h_attr2, sizeof(BleGattAttr));
    attrList++;
    memmove(attrList, &h_attr4, sizeof(BleGattAttr));
    int handler = 0;
    BleGattsStartServiceEx(&handler, &service_info);
}

void test_h_add_service2(void)
{
    static uint8_t flagg = 0;
    if (flagg == 1) {
        return;
    }
    flagg++;
    BleGattService service_info;
    BleGattAttr *attrList = malloc(2 * sizeof(BleGattAttr));
    if (attrList == NULL) {
        return;
    }
    service_info.attrNum = 2;
    service_info.attrList = attrList;
    memmove(attrList, &h_attr3, sizeof(BleGattAttr));
    attrList++;
    memmove(attrList, &h_attr5, sizeof(BleGattAttr));
    int handler = 0;
    BleGattsStartServiceEx(&handler, &service_info);
}

static unsigned int get_service_num(unsigned int attrNum, uint8_t indicate_is_exit)
{
    int num = 2 * (attrNum - 1) + 1 + indicate_is_exit;
    return num;
}

int BleGattsStartServiceEx(int *srvcHandle, BleGattService *srvcInfo)
{
    if (NULL == srvcInfo) {
        return -1;
    }
    print_log("[Harmony]BleGattsStartServiceEx 0x%x %d\r\n", srvcHandle, srvcInfo->attrNum);
    unsigned int att_nb = srvcInfo->attrNum;
    uint8_t indicate_is_exit = 0;
    BleGattAttr *att_db = srvcInfo->attrList;
    print_log("[Harmony]att nb %d \r\n", att_nb);
    if ((srvcInfo->attrList[att_nb - 1].properties  & OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE) ==
        OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE
        ||  (srvcInfo->attrList[att_nb - 1].properties  & OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY) ==
        OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY) {
        indicate_is_exit = 1;
    }
    att_nb =  get_service_num(srvcInfo->attrNum, indicate_is_exit);
    uint32_t len = (att_nb) * sizeof(ble_gatt_att_reg_t) ;
    ble_gatt_att_reg_t *att_list;
    ble_gatt_att_reg_t *att_temp;
    att_list = (ble_gatt_att_reg_t *)malloc(len);
    if (att_list == NULL) {
        return -1;
    }
    att_temp = att_list;
    memset(att_list, 0, len);

    for (int i = 0; i < srvcInfo->attrNum; i++) {
        if (att_db[i].attrType == OHOS_BLE_ATTRIB_TYPE_SERVICE) {
            att_temp = ble_ohos_add_service(att_temp, &att_db[i]);

        }

        else if (att_db[i].attrType == OHOS_BLE_ATTRIB_TYPE_CHAR) {
            bool IsLast_item = false;
            if (i == srvcInfo->attrNum - 1) {
                IsLast_item = true;
            }
            att_temp = ble_ohos_add_char(att_temp, &att_db[i], IsLast_item);
        }
        att_temp++;
    }

    uint16_t start_handle = *srvcHandle;
    for (int i = 0; i < att_nb; i++) {
        print_log("att %d %d %d\r\n", att_list[i].att_desc.perm, att_list[i].att_desc.ext_perm, att_list[i].att_desc.max_len);
        print_log("uudi %x %x %x %x", att_list[i].att_desc.uuid[0], att_list[i].att_desc.uuid[1], att_list[i].att_desc.uuid[2],
                  att_list[i].att_desc.uuid[3]);
    }
    app_ble_gatt_add_svc_helper(&start_handle, att_nb, att_list);
    int return_handle = start_handle;
    return_handle = return_handle << 16;
    *srvcHandle = return_handle;
    test_handle = return_handle;
    print_log("service handler 0x%lx", return_handle);
    free(att_list);
    return BT_STATUS_SUCCESS;
}

int BleGattsStopServiceEx(int srvcHandle)
{
    print_log("harmony  : %s %d\r\n", __FUNCTION__, srvcHandle);

    uint16_t handler = 0;
    handler = srvcHandle >> 16;
    return app_ble_disable_service_by_handler(handler);

}

int BleGattRegisterCallbacks(BtGattCallbacks *func)
{
    print_log("harmony  : %s \r\n", __FUNCTION__);
    memmove(&bt_gattcallback, func, sizeof(BtGattCallbacks));
    app_register_sec_cb((app_sec_req_cb)bt_gattcallback.securityRespondCb);
    return BT_STATUS_SUCCESS;

}

int BleGattsRegisterCallbacks(BtGattServerCallbacks *func)
{
    print_log("harmony  : %s \r\n", __FUNCTION__);
    memmove(&gatts_callback, func, sizeof(BtGattServerCallbacks));
    return BT_STATUS_SUCCESS;
}

static uint16_t get_indicate_att_offset(uint16_t offset)
{
    uint16_t r_offset = 0;
    r_offset = 2 * (offset) ;
    return r_offset;
}

int BleGattsSendIndication(int serverId, GattsSendIndParam *param)
{
    if (NULL == param) {
        return BT_STATUS_FAIL;
    }
    print_log("harmony  : %s 0x%lx\r\n", __FUNCTION__, param->attrHandle);
    uint16_t handler = 0;
    uint16_t att_offset = 0;
    int start_handler = param->attrHandle;
    handler = start_handler >> 16;
    att_offset = (start_handler & 0xFFFF);
    att_offset = get_indicate_att_offset(att_offset);
    print_log("harmony :hander %d ,offset 0x%lx\r\n", handler, att_offset);

    if (param->confirm) {
        app_ble_gatt_data_send(handler, att_offset, param->valueLen, param->value);
    } else {
        app_ble_gatt_data_send_notify(handler, att_offset, param->valueLen, param->value);
    }
    return BT_STATUS_SUCCESS;
}

int ReadBtMacAddr(unsigned char *mac, unsigned int len)
{
    uint8_t *temp_addr = sonata_get_bt_address();
    mac[0] = temp_addr[5];
    mac[1] = temp_addr[4];
    mac[2] = temp_addr[3];
    mac[3] = temp_addr[2];
    mac[4] = temp_addr[1];
    mac[5] = temp_addr[0];
    return BT_STATUS_SUCCESS;

}

int BleStartAdvEx(int *advId, const StartAdvRawData rawData, BleAdvParams advParam)
{
    print_log("harmony  : %s %d %d\r\n", __FUNCTION__, rawData.advDataLen, rawData.rspDataLen);
    *advId = 0;
    sonata_gap_directed_adv_create_param_t param;
    memset(&param, 0, sizeof(sonata_gap_directed_adv_create_param_t));
    param.addr_type = advParam.peerAddrType;
    param.disc_mode = SONATA_GAP_ADV_MODE_GEN_DISC;
    param.max_tx_pwr = advParam.txPower;
    param.filter_pol = advParam.advFilterPolicy;
    param.adv_intv_min = advParam.minInterval;
    param.adv_intv_max = advParam.maxInterval;
    param.chnl_map = advParam.channelMap;
    param.phy = SONATA_GAP_PHY_LE_1MBPS;
    memmove(param.addr.addr, &advParam.peerAddr, SONATA_GAP_BD_ADDR_LEN);
    if (advParam.advType == OHOS_BLE_ADV_IND) {
        param.prop = SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK;
    } else if (advParam.advType == OHOS_BLE_ADV_DIRECT_IND_HIGH) {
        param.prop = SONATA_GAP_ADV_PROP_DIR_CONN_HDC_MASK;
    } else if (advParam.advType == OHOS_BLE_ADV_SCAN_IND) {
        param.prop = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK;
    } else if (advParam.advType == OHOS_BLE_ADV_NONCONN_IND) {
        param.prop = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK;
    } else if (advParam.advType == OHOS_BLE_ADV_DIRECT_IND_LOW) {
        param.prop = SONATA_GAP_ADV_PROP_DIR_CONN_LDC_MASK;
    }
    ble_adv_data_set_t advdata;
    ble_scan_data_set_t scandata;
    advdata.advdataLen = rawData.advDataLen;
    scandata.respdataLen = rawData.rspDataLen;
    memmove(advdata.advdata, rawData.advData, advdata.advdataLen);
    memmove(scandata.respdata, rawData.rspData, scandata.respdataLen);
    app_ble_start_advertising_with_param(&param, &advdata, &scandata, advParam.ownAddrType, advParam.duration, 0);
    return BT_STATUS_SUCCESS;
}

static uint8_t sec_enc_req_h2a(BleSecAct secAct)
{
    switch (secAct) {
        case OHOS_BLE_SEC_NONE:
            return 0;
        case OHOS_BLE_SEC_ENCRYPT:
            return SONATA_GAP_AUTH_BOND;
        case OHOS_BLE_SEC_ENCRYPT_NO_MITM:
            return  SONATA_GAP_AUTH_SEC_CON | SONATA_GAP_AUTH_BOND;
        case OHOS_BLE_SEC_ENCRYPT_MITM:
            return  SONATA_GAP_AUTH_MITM | SONATA_GAP_AUTH_BOND | SONATA_GAP_AUTH_MITM;
        default:
            return 0;
    }
}

int BleGattsSetEncryption(BdAddr bdAddr, BleSecAct secAct)
{
    print_log("harmony  : %s\r\n", __FUNCTION__);
    if (OHOS_BLE_SEC_NONE == secAct) {
        print_log("harmony :no sec req\r\n");
        return BT_STATUS_SUCCESS;
    }
    uint8_t auth = sec_enc_req_h2a(secAct);
    app_gap_connect_confirm(bdAddr.addr, auth);
    return BT_STATUS_SUCCESS;
}

int BleStopAdv(int advId)
{
    print_log("harmony  : %s %d\r\n", __FUNCTION__, advId);
    app_ble_stop_adv_without_id();
    return BT_STATUS_SUCCESS;
}

// not used start!!!
/* gatt client register, callback return clientId */
int BleGattcRegister(BtUuid appUuid)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

/* clientId from BleGattcRegister */
int BleGattcUnRegister(int clientId)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

/* advertising id specified by upper layer */
int BleSetAdvData(int advId, const BleConfigAdvData *data)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleStartAdv(int advId, const BleAdvParams *param)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattcRegisterCallbacks(BtGattClientCallbacks *func)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsRegister(BtUuid appUuid)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsUnRegister(int serverId)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsAddService(int serverId, BtUuid srvcUuid, bool isPrimary, int number)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsDeleteService(int serverId, int srvcHandle)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsAddChar(int serverId, int srvcHandle, char *uuid, int properties, int permissions)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsAddDescr(int serverId, int srvcHandle, char *uuid, int permissions)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsStartService(int serverId, int srvcHandle)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsStopService(int serverId, int srvcHandle)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

int BleGattsSendResponse(int serverId, GattsSendRspParam *param)
{
    print_log("harmony ERROR : %s  should not be used\r\n", __FUNCTION__);
    return BT_STATUS_FAIL;
}

// not used end!!!

