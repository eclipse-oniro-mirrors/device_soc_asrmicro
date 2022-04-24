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
 * @file (atcmdpuls_ble.h)
 *
 * @brief
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "atcmdplus_ble.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lega_at_api.h"
#include "app.h"
#include "sonata_gap.h"
#include "sonata_gatt_api.h"

/*
 * MACRO DEFINES
 ****************************************************************************************
 */
#define PARA_ID_0 (0)
#define PARA_ID_1 (1)
#define PARA_ID_2 (2)
#define PARA_ID_3 (3)
#define PARA_ID_4 (4)

/*
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

uint8_t char2HexValue(char ch)
{
    uint8_t result = 0;

    if (ch >= '0' && ch <= '9') {
        result = ch - '0';
    } else if (ch >= 'a' && ch <= 'z') {
        result = (ch - 'a') + 10;
    } else if (ch >= 'A' && ch <= 'Z') {
        result = (ch - 'A') + 10;
    } else {
        result = -1;
    }
    return result;
}

void hexValue2Char(uint8_t value, uint8_t *chars)
{
    uint8_t d1 = value & 0xF;
    uint8_t d2 = value >> 4;
    chars[0] = d2 > 9 ? 'A' + d2 - 10 : '0' + d2;
    chars[1] = d1 > 9 ? 'A' + d1 - 10 : '0' + d1;
}
void macChar2Value(uint8_t *chars, uint8_t *mac, bool colon)
{
    uint8_t d0, d1, d2, d3, d4, d5;
    if (colon) {
        d0 = char2HexValue(chars[0]) * 16 + char2HexValue(chars[1]);
        d1 = char2HexValue(chars[3]) * 16 + char2HexValue(chars[4]);
        d2 = char2HexValue(chars[6]) * 16 + char2HexValue(chars[7]);
        d3 = char2HexValue(chars[9]) * 16 + char2HexValue(chars[10]);
        d4 = char2HexValue(chars[12]) * 16 + char2HexValue(chars[13]);
        d5 = char2HexValue(chars[15]) * 16 + char2HexValue(chars[16]);
        mac[0] = d5; // MAC in stack should reverse with Real MAC
        mac[1] = d4;
        mac[2] = d3;
        mac[3] = d2;
        mac[4] = d1;
        mac[5] = d0;
    } else {
        d0 = char2HexValue(chars[0]) * 16 + char2HexValue(chars[1]);
        d1 = char2HexValue(chars[2]) * 16 + char2HexValue(chars[3]);
        d2 = char2HexValue(chars[4]) * 16 + char2HexValue(chars[5]);
        d3 = char2HexValue(chars[6]) * 16 + char2HexValue(chars[7]);
        d4 = char2HexValue(chars[8]) * 16 + char2HexValue(chars[9]);
        d5 = char2HexValue(chars[10]) * 16 + char2HexValue(chars[11]);
        mac[0] = d5; // MAC in stack should reverse with Real MAC
        mac[1] = d4;
        mac[2] = d3;
        mac[3] = d2;
        mac[4] = d1;
        mac[5] = d0;
    }
}

uint8_t macValue2Char(uint8_t *mac, uint8_t *chars, bool colon)
{
    if (colon) {
        hexValue2Char(mac[5], &chars[0]);
        chars[2] = ':';
        hexValue2Char(mac[4], &chars[3]);
        chars[5] = ':';
        hexValue2Char(mac[3], &chars[6]);
        chars[8] = ':';
        hexValue2Char(mac[2], &chars[9]);
        chars[11] = ':';
        hexValue2Char(mac[1], &chars[12]);
        chars[14] = ':';
        hexValue2Char(mac[0], &chars[15]);
        return 17;
    } else {
        hexValue2Char(mac[5], &chars[0]);
        hexValue2Char(mac[4], &chars[2]);
        hexValue2Char(mac[3], &chars[4]);
        hexValue2Char(mac[2], &chars[6]);
        hexValue2Char(mac[1], &chars[8]);
        hexValue2Char(mac[0], &chars[10]);
        return 12;
    }
}

void apcmdplue_print_command(int argc, char **argv)
{
    APP_TRC("-------------------[%d]\r\n", argc);
    if (argc > 0) {
        APP_TRC("     P0:%s\r\n", argv[PARA_ID_0]);
    }
    if (argc > 1) {
        APP_TRC("     P1:%s\r\n", argv[PARA_ID_1]);
    }
    if (argc > 2) {
        APP_TRC("     P2:%s\r\n", argv[PARA_ID_2]);
    }
    if (argc > 3) {
        APP_TRC("     P3:%s\r\n", argv[PARA_ID_3]);
    }
    if (argc > 4) {
        APP_TRC("     P4:%s\r\n", argv[PARA_ID_4]);
    }
    APP_TRC("-------------------\r\n");

}

int atcmdplus_adv(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    if (strcmp(argv[PARA_ID_1], "1") == 0) {
        app_ble_config_legacy_advertising();
    } else if (strcmp(argv[PARA_ID_1], "0") == 0) {
        app_ble_advertising_stop(0);
    }
    return CONFIG_OK;
}

int atcmdplus_scan(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    if (strcmp(argv[PARA_ID_1], "1") == 0) {
        app_ble_config_scanning();
    } else if (strcmp(argv[PARA_ID_1], "0") == 0) {
        app_ble_stop_scanning();
    }
    return CONFIG_OK;
}

int atcmdplus_conn(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    if (argc != 2) {
        return PARAM_RANGE;
    }

    uint8_t targetAddr[SONATA_GAP_BD_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    macChar2Value((uint8_t *)argv[PARA_ID_1], targetAddr, false);

    app_ble_set_target_address(targetAddr);
    app_ble_config_initiating();
    return CONFIG_OK;
}

int atcmdplus_uuid(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    app_uuids uuids;
    uint16_t d1 = char2HexValue(argv[PARA_ID_1][0]) * 16 + char2HexValue(argv[PARA_ID_1][1]);
    uint16_t d0 = char2HexValue(argv[PARA_ID_1][2]) * 16 + char2HexValue(argv[PARA_ID_1][3]);
    uuids.service = ((d1 * 0X100) + d0);

    d1 = char2HexValue(argv[PARA_ID_2][0]) * 16 + char2HexValue(argv[PARA_ID_2][1]);
    d0 = char2HexValue(argv[PARA_ID_2][2]) * 16 + char2HexValue(argv[PARA_ID_2][3]);
    uuids.read = ((d1 * 0X100) + d0);

    d1 = char2HexValue(argv[PARA_ID_3][0]) * 16 + char2HexValue(argv[PARA_ID_3][1]);
    d0 = char2HexValue(argv[PARA_ID_3][2]) * 16 + char2HexValue(argv[PARA_ID_3][3]);
    uuids.write = ((d1 * 0X100) + d0);

    d1 = char2HexValue(argv[PARA_ID_4][0]) * 16 + char2HexValue(argv[PARA_ID_4][1]);
    d0 = char2HexValue(argv[PARA_ID_4][2]) * 16 + char2HexValue(argv[PARA_ID_4][3]);
    uuids.ntf = ((d1 * 0X100) + d0);
    app_ble_set_uuids(uuids.service, uuids.read, uuids.write, uuids.ntf);
    return CONFIG_OK;
}

int atcmdplus_discovery(int argc, char **argv)
{
    uint8_t conidx = char2HexValue(argv[PARA_ID_1][0]);
    sonata_ble_gatt_disc_all_svc(conidx); // --->app_gatt_disc_svc_callback()
    return CONFIG_OK;
}

int atcmdplus_lesend(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);

    uint8_t conidx = char2HexValue(argv[PARA_ID_1][0]);
    uint16_t dataLen = char2HexValue(argv[PARA_ID_2][0]); // Todo only support 1 char now.
    uint8_t *data = (uint8_t *)argv[PARA_ID_3];
    app_ble_master_write_data(conidx, dataLen, data);
    return CONFIG_OK;
}

int atcmdplus_ntf(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    app_uuids *uuids = app_ble_get_uuids();
    if (uuids->service == 0) {
        return PARAM_RANGE;
    }
    uint8_t idValue = *argv[PARA_ID_1] - '0';

    if (strcmp(argv[PARA_ID_2], "1") == 0) {
        app_ble_master_turn_ntf(idValue, true);
    } else if (strcmp(argv[PARA_ID_2], "0") == 0) {
        app_ble_master_turn_ntf(idValue, false);
    }
    return CONFIG_OK;
}

int atcmdplus_ledisc(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    uint8_t idValue = *argv[PARA_ID_1] - '0';
    app_ble_disconnect(idValue);
    return CONFIG_OK;
}

int atcmdplus_chinfo(int argc, char **argv)
{
    apcmdplue_print_command(argc, argv);
    actives *act = app_get_active();
    uint8_t out[50] = {0};
    for (int i = 0; i < APP_ACTIVE_MAX; ++i) {
        if (act[i].runing == true) {
            uint8_t offset = 0;
            offset = macValue2Char(act[i].peer, &out[0], true);
            out[offset] = ',';
            offset += 1;
            out[offset] = act[i].assign_id + '0';
            offset += 1;
            out[offset] = ',';
            offset += 1;
            out[offset] = '3';
            offset += 1;
            for (int i = 0; i < offset; ++i) {
                printf("%c", out[i]);
            }
            printf("\r\n");
        }
    }
    return CONFIG_OK;
}

int atcmdplus_test(int argc, char **argv)
{
    printf("AT CMD PLUS, %s, \r\n", __FUNCTION__);
    return CONFIG_OK;
}

#if (LOSCFG_USE_SHELL == 1)
#include "shell.h"
#include "shcmd.h"
#include "target_config.h"
#endif
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void atcmdplus_ble_register(void)
{
#if (LOSCFG_USE_SHELL == 1)
    osCmdReg(CMD_TYPE_STD, "le_bletest", 0, (CMD_CBK_FUNC)atcmdplus_test);
    osCmdReg(CMD_TYPE_STD, "le_adv", 0, (CMD_CBK_FUNC)atcmdplus_adv);
    osCmdReg(CMD_TYPE_STD, "le_scan", 0, (CMD_CBK_FUNC)atcmdplus_scan);
    osCmdReg(CMD_TYPE_STD, "le_conn", 0, (CMD_CBK_FUNC)atcmdplus_conn);
    osCmdReg(CMD_TYPE_STD, "le_uuid", 0, (CMD_CBK_FUNC)atcmdplus_uuid);
    osCmdReg(CMD_TYPE_STD, "le_discovery", 0, (CMD_CBK_FUNC)atcmdplus_discovery);
    osCmdReg(CMD_TYPE_STD, "le_lesend", 0, (CMD_CBK_FUNC)atcmdplus_lesend);
    osCmdReg(CMD_TYPE_STD, "le_ntf", 0, (CMD_CBK_FUNC)atcmdplus_ntf);
    osCmdReg(CMD_TYPE_STD, "le_ledisc", 0, (CMD_CBK_FUNC)atcmdplus_ledisc);
    osCmdReg(CMD_TYPE_STD, "le_chinfo", 0, (CMD_CBK_FUNC)atcmdplus_chinfo);
#endif
}

