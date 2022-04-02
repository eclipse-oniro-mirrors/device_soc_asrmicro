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
#include "iot_pwm.h"
#include "duet_pwm.h"

static duet_pwm_dev_t duet_pwm[DUET_PWM_CH_NUM];

unsigned int IoTPwmInit(unsigned int port)
{
    if (port >= DUET_PWM_CH_NUM) {
        return IOT_FAILURE;
    }

    duet_pwm[port].port = port;
    return duet_pwm_init(&duet_pwm[port]);
}

unsigned int IoTPwmDeinit(unsigned int port)
{
    if (port >= DUET_PWM_CH_NUM) {
        return IOT_FAILURE;
    }

    return duet_pwm_finalize(&duet_pwm[port]);
}

unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq)
{
    if (port >= DUET_PWM_CH_NUM) {
        return IOT_FAILURE;
    }

    duet_pwm[port].config.freq = freq;
    duet_pwm[port].config.duty_cycle = duty;
    return duet_pwm_start(&duet_pwm[port]);
}

unsigned int IoTPwmStop(unsigned int port)
{
    if (port >= DUET_PWM_CH_NUM) {
        return IOT_FAILURE;
    }

    return duet_pwm_stop(&duet_pwm[port]);
}

