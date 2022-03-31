/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

#include "iot_errno.h"
#include "iot_i2c.h"
#include "duet.h"
#include "duet_i2c.h"

static duet_i2c_dev_t g_duet_i2c[DUET_I2C_NUM];

#define I2C_TIMEOUT 0xFFFFFFFF

unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen)
{
    if (id >= DUET_I2C_NUM) {
        return IOT_FAILURE;
    }

    return duet_i2c_master_send(&g_duet_i2c[id], deviceAddr, data, dataLen, I2C_TIMEOUT);
}

unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen)
{
    if (id >= DUET_I2C_NUM) {
        return IOT_FAILURE;
    }

    return duet_i2c_master_recv(&g_duet_i2c[id], deviceAddr, data, dataLen, I2C_TIMEOUT);
}

unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate)
{
    if (id >= DUET_I2C_NUM) {
        return IOT_FAILURE;
    }

    g_duet_i2c[id].port = id;
    g_duet_i2c[id].config.mode = I2C_MASTER;
    g_duet_i2c[id].config.dev_addr = I2C_ADDRESS;

    return duet_i2c_init(&g_duet_i2c[id]);
}

unsigned int IoTI2cDeinit(unsigned int id)
{
    if (id >= DUET_I2C_NUM) {
        return IOT_FAILURE;
    }

    return  duet_i2c_finalize(&g_duet_i2c[id]);
}

unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate)
{
    if (id >= DUET_I2C_NUM) {
        return IOT_FAILURE;
    }

    if (baudrate >= 1700000) {
        g_duet_i2c[id].config.freq = I2C_HIGH_SPEED;
    } else if (baudrate <= 100000) {
        g_duet_i2c[id].config.freq = I2C_STANDARD_SPEED;
    } else {
        g_duet_i2c[id].config.freq = I2C_FAST_SPEED;
    }

    return IOT_SUCCESS;
}
