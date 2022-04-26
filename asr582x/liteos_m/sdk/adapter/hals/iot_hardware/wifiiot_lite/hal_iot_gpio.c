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
#include "iot_gpio.h"
#include "duet_gpio.h"
#include "lega_rtos.h"

static duet_gpio_dev_t g_duet_gpio[DUET_GPIO_TOTAL_NUM];

unsigned int IoTGpioInit(unsigned int id)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    g_duet_gpio[id].port = id;

    return  duet_gpio_init(&g_duet_gpio[id]);
}

unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    if (dir == IOT_GPIO_DIR_IN) {
        g_duet_gpio[id].config = DUET_INPUT_PULL_DOWN;
    } else {
        g_duet_gpio[id].config = DUET_OUTPUT_PUSH_PULL;
    }

    return IOT_SUCCESS;
}

unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir)
{
    int duet_cfg = 0;

    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    duet_cfg = g_duet_gpio[id].config;

    if (duet_cfg >= DUET_OUTPUT_PUSH_PULL) {
        return IOT_GPIO_DIR_OUT;
    } else {
        return IOT_GPIO_DIR_IN;
    }
}

unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    if (val == IOT_GPIO_VALUE0) {
        duet_gpio_output_low(&g_duet_gpio[id]);
    } else {
        duet_gpio_output_high(&g_duet_gpio[id]);
    }

    return IOT_SUCCESS;
}

unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    return duet_gpio_output_get(&g_duet_gpio[id], &val);
}

unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    return duet_gpio_input_get(&g_duet_gpio[id], &val);
}

unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity,
                                    GpioIsrCallbackFunc func, char *arg)
{
    duet_gpio_irq_trigger_t trigger;

    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    if (intPolarity == IOT_GPIO_EDGE_FALL_LEVEL_LOW) {
        trigger = DUET_IRQ_TRIGGER_FALLING_EDGE;
    } else {
        trigger = DUET_IRQ_TRIGGER_RISING_EDGE;
    }

    return duet_gpio_enable_irq(&g_duet_gpio[id], trigger, func, arg);
}

unsigned int IoTGpioUnregisterIsrFunc(unsigned int id)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    return duet_gpio_disable_irq(&g_duet_gpio[id]);
}

unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    return duet_gpio_clear_irq(&g_duet_gpio[id]);
}

extern duet_gpio_cb_t g_duet_gpio_handler[DUET_GPIO_TOTAL_NUM];

unsigned int IoTGpioSetIsrMode(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
    GpioIsrCallbackFunc func;
    char *arg;
    duet_gpio_irq_trigger_t trigger;

    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    if (g_duet_gpio_handler[id].cb != NULL) {
        func = g_duet_gpio_handler[id].cb;
    }
    if (g_duet_gpio_handler[id].arg != NULL) {
        arg  =  g_duet_gpio_handler[id].arg;
    }

    if (intPolarity == IOT_GPIO_EDGE_FALL_LEVEL_LOW) {
        trigger = DUET_IRQ_TRIGGER_FALLING_EDGE;
    } else {
        trigger = DUET_IRQ_TRIGGER_RISING_EDGE;
    }

    if (duet_gpio_disable_irq(&g_duet_gpio[id]) != 0) {
        return IOT_FAILURE;
    }

    return duet_gpio_enable_irq(&g_duet_gpio[id], trigger, func, arg);
}

unsigned int IoTGpioDeinit(unsigned int id)
{
    if (id >= DUET_GPIO_TOTAL_NUM) {
        return IOT_FAILURE;
    }

    return duet_gpio_finalize(&g_duet_gpio[id]);
}

