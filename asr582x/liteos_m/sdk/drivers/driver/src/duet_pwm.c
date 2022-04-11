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

#include <stdio.h>
#include <errno.h>
#include "duet.h"
#include "duet_cm4.h"
#include "duet_pinmux.h"
#include "duet_pwm.h"

void PWM_IRQHandler(void)
{
    //    duet_intrpt_enter();
    //    duet_intrpt_exit();
}

// pwm pinmux init
void duet_pwm_pinmux_init(duet_pwm_dev_t *pwm)
{
    switch (pwm->port) {
        case PWM_OUTPUT_CH0:
            // pin mux control
            // PWM0_PAD PAD14 1
            duet_pinmux_config(PAD14, PF_PWM0);
            break;
        case PWM_OUTPUT_CH1:
            // PWM1_PAD PAD10 1
            duet_pinmux_config(PAD10, PF_PWM1);
            break;
        case PWM_OUTPUT_CH2:
            // PWM2_PAD PAD15 1
            duet_pinmux_config(PAD15, PF_PWM2);
            break;
        case PWM_OUTPUT_CH3:
            // PWM3_PAD PAD11 1
            duet_pinmux_config(PAD11, PF_PWM3);
            break;
        case PWM_OUTPUT_CH4:
            // PWM4_PAD PAD6  4
            duet_pinmux_config(PAD6, PF_PWM4);
            break;
        case PWM_OUTPUT_CH5:
            // PWM5_PAD PAD0  4
            duet_pinmux_config(PAD0, PF_PWM5);
            break;
        case PWM_OUTPUT_CH6:
            // PWM6_PAD PAD7  4
            duet_pinmux_config(PAD7, PF_PWM6);
            break;
        case PWM_OUTPUT_CH7:
            // PWM7_PAD PAD1  4
            duet_pinmux_config(PAD1, PF_PWM7);
            break;
        default:
            break;
    }
}

// pwm freq and duty cycle config
void duet_pwm_cfg(duet_pwm_dev_t *pwm)
{
    uint32_t tmp_value;
    switch (pwm->port) {
        case PWM_OUTPUT_CH0:
            PWM->PWMCTL |= PWM_COUNT_MODE; // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM01LOAD & (~(0x0000FFFF));
            tmp_value |= (SYSTEM_CLOCK / pwm->config.freq - 1);
            PWM->PWM01LOAD = tmp_value;

            tmp_value = PWM->PWM0CMP & (~(0x0000FFFF));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle));
            PWM->PWM0CMP = tmp_value;
            PWM->PWM01DB = 0;
            break;
        case PWM_OUTPUT_CH1:
            PWM->PWMCTL |= PWM_COUNT_MODE; // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM01LOAD & (~(0x0000FFFF));
            tmp_value |= (SYSTEM_CLOCK / pwm->config.freq - 1);
            PWM->PWM01LOAD = tmp_value;

            tmp_value = PWM->PWM0CMP & (~(0xFFFF0000));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle)) << 16;
            PWM->PWM0CMP = tmp_value;
            PWM->PWM01DB = 0;
            break;
        case PWM_OUTPUT_CH2:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 1); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM01LOAD & (~(0xFFFF0000));
            tmp_value |= ((uint16_t)(SYSTEM_CLOCK / pwm->config.freq - 1) << 16);
            PWM->PWM01LOAD = tmp_value;

            tmp_value = PWM->PWM1CMP & (~(0x0000FFFF));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle));
            PWM->PWM1CMP = tmp_value;
            PWM->PWM01DB = 0;
            break;
        case PWM_OUTPUT_CH3:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 1); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM01LOAD & (~(0xFFFF0000));
            tmp_value |= ((uint16_t)(SYSTEM_CLOCK / pwm->config.freq - 1) << 16);
            PWM->PWM01LOAD = tmp_value;

            tmp_value = PWM->PWM1CMP & (~(0xFFFF0000));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle)) << 16;
            PWM->PWM1CMP = tmp_value;
            PWM->PWM01DB = 0;
            break;
        case PWM_OUTPUT_CH4:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 2); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM23LOAD & (~(0x0000FFFF));
            tmp_value |= (SYSTEM_CLOCK / pwm->config.freq - 1);
            PWM->PWM23LOAD = tmp_value;

            tmp_value = PWM->PWM2CMP & (~(0x0000FFFF));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle));
            PWM->PWM2CMP = tmp_value;
            PWM->PWM23DB = 0;
            break;
        case PWM_OUTPUT_CH5:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 2); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM23LOAD & (~(0x0000FFFF));
            tmp_value |= (SYSTEM_CLOCK / pwm->config.freq - 1);
            PWM->PWM23LOAD = tmp_value;

            tmp_value = PWM->PWM2CMP & (~(0xFFFF0000));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle)) << 16;
            PWM->PWM2CMP = tmp_value;
            PWM->PWM23DB = 0;
            break;
        case PWM_OUTPUT_CH6:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 3); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM23LOAD & (~(0xFFFF0000));
            tmp_value |= ((uint16_t)(SYSTEM_CLOCK / pwm->config.freq - 1) << 16);
            PWM->PWM23LOAD = tmp_value;

            tmp_value = PWM->PWM3CMP & (~(0x0000FFFF));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle));
            PWM->PWM3CMP = tmp_value;
            PWM->PWM23DB = 0;
            break;
        case PWM_OUTPUT_CH7:
            PWM->PWMCTL |= (PWM_COUNT_MODE << 3); // 0: count-up mode, 1: count-up/down mode
            tmp_value = PWM->PWM23LOAD & (~(0xFFFF0000));
            tmp_value |= ((uint16_t)(SYSTEM_CLOCK / pwm->config.freq - 1) << 16);
            PWM->PWM23LOAD = tmp_value;

            tmp_value = PWM->PWM3CMP & (~(0xFFFF0000));
            tmp_value |= (uint16_t)(SYSTEM_CLOCK / pwm->config.freq * (1 - pwm->config.duty_cycle)) << 16;
            PWM->PWM3CMP = tmp_value;
            PWM->PWM23DB = 0;
            break;
        default:
            break;
    }
}

/**
 * Initialises a PWM pin
 *
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_pwm_init(duet_pwm_dev_t *pwm)
{
    uint32_t reg_value;
    if (NULL == pwm) {
        return EIO;
    }
    if (pwm->port >= DUET_PWM_CH_NUM) {
        return EIO;
    }
    // pinmux
    duet_pwm_pinmux_init(pwm);

    // pwm clock enable
    reg_value = REG_RD(PERI_CLK_EN_REG1) & (~PWM_BUS_CLK_BIT);
    REG_WR(PERI_CLK_EN_REG1, (reg_value | (PWM_BUS_CLK_BIT)));

    PWM->PWMCFG &= ~(1 << pwm->port);
    // PWM->PWMCFG |= (CNT_CLK_DIV_EN | CLK_DIV_CFG);
    duet_pwm_cfg(pwm);
    PWM->PWMINVERTTRIG = 0; // invert control
    return 0;
}

/**
 * Starts Pulse-Width Modulation signal output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_pwm_start(duet_pwm_dev_t *pwm)
{
    if (NULL == pwm) {
        return EIO;
    }
    if (pwm->port >= DUET_PWM_CH_NUM) {
        return EIO;
    }
    PWM->PWMCFG |= (1 << pwm->port);
    return 0;
}

/**
 * Stops output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_pwm_stop(duet_pwm_dev_t *pwm)
{
    if (NULL == pwm) {
        return EIO;
    }
    if (pwm->port >= DUET_PWM_CH_NUM) {
        return EIO;
    }
    PWM->PWMCFG &= ~(1 << pwm->port);
    return 0;
}

/**
 * change the para of pwm
 *
 * @param[in]  pwm  the PWM device
 * @param[in]  para the para of pwm
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_pwm_para_chg(duet_pwm_dev_t *pwm, duet_pwm_config_t para)
{
    if (NULL == pwm) {
        return EIO;
    }
    if (pwm->port >= DUET_PWM_CH_NUM) {
        return EIO;
    }
    // duet_pwm_stop(pwm);
    pwm->config = para;
    duet_pwm_cfg(pwm);
    // duet_pwm_start(pwm);
    return 0;
}

/**
 * De-initialises an PWM interface, Turns off an PWM hardware interface
 *
 * @param[in]  pwm  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_pwm_finalize(duet_pwm_dev_t *pwm)
{
    // pwm clock disable
    // uint32_t reg_value;
    if (NULL == pwm) {
        return EIO;
    }
    if (pwm->port >= DUET_PWM_CH_NUM) {
        return EIO;
    }
    // one clk enable for 8 pwm channel
    // reg_value = REG_RD(PERI_CLK_CFG);
    // REG_WR(PERI_CLK_CFG, (reg_value&(~PWM_CLK_EN)));
    return duet_pwm_stop(pwm);
}

