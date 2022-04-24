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

#ifndef __PMU_H
#define __PMU_H

#include "stdint.h"

typedef enum pmu_state {
    PMU_STATE_ACTIVE = 0,
    PMU_STATE_MODEMSLEEP,
    PMU_STATE_LIGHTSLEEP,
    PMU_STATE_DEEPSLEEP,
    PMU_STATE_LAST
} pmu_state_t;

/** \brief  Wait For Interrupt

    Wait For Interrupt is a hint instruction that suspends execution
    until one of a number of events occurs.
 */
__attribute__((always_inline)) static inline void __WFI__(void)
{
    __asm volatile("dsb");
    __asm volatile("wfi");
    __asm volatile("isb");
}

/** @brief  get current RTC counter, only used when RTC enabled.
 *
 *  @return   : current RTC counter
 */
uint32_t lega_drv_rtc_counter_get(void);

/** @brief  get MAX RTC counter we can set, only used when RTC enabled.
 *
 *  @return   : MAX RTC counter we can set
 */
uint32_t lega_drv_rtc_max_ticks_get();

/** @brief  get MAX RTC counter we can set, only used when RTC enabled.
 *  @param    cc_counter: RTC counter to set.
 *  @param    whitch state you want goto, default modemsleep.
 *  @return   0: set OK.
 *  @return   other: set error.
 */
uint16_t lega_drv_rtc_cc_set(uint32_t cc_counter, pmu_state_t pmu_state);

/** @brief  enable RTC clock, used after lega_drv_rtc_cc_set.
 */
void lega_drv_rtc_enable(void);

/** @brief  goto sleep state.
 *  @param  pmu_state: state to set:MODEMSLEEP/LIGHTSLEEP/DEEPSLEEP.
 *  @param  pmu_state: state to set:MODEMSLEEP/LIGHTSLEEP/DEEPSLEEP.
 */
void lega_drv_goto_sleep(pmu_state_t pmu_state);

/** @brief  goto active state, call when waked from SLEEP states.
 */
void lega_drv_goto_active(void);

/** @brief  check if wakeup gpio is valid or not.
 */
uint8_t lega_drv_wakeup_gpio_is_valid(uint8_t wakeup_gpio);

/** @brief  goto deepsleep state.
 */
void lega_drv_goto_deepsleep(uint32_t sleep_ms, uint8_t wakeup_gpio);

#endif // __PMU_H