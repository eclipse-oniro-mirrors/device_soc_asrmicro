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
#include "duet_cm4.h"
#include "duet.h"
#include "duet_wdg.h"
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif
// WDG APB CLOCK 0-15 BIT 4-7
#define APB_DIV_1   0
#define APB_DIV_2   1
#define APB_DIV_3   2
#define APB_DIV_4   3
#define APB_DIV_5   4
#define APB_DIV_6   5
#define APB_DIV_7   6
#define APB_DIV_8   7
#define APB_DIV_9   8
#define APB_DIV_10  9
#define APB_DIV_11  10
#define APB_DIV_12  11
#define APB_DIV_13  12
#define APB_DIV_14  13
#define APB_DIV_15  14
#define APB_DIV_16  15
#define WDG_APB_DIV APB_DIV_1
#define WDG_APB_DIV_REG 0X40000854

void hw_watchdog_unlock(void)
{
    WATCHDOG->LOCK = WDG_LOCK_TOKEN;
}

void hw_watchdog_lock(void)
{
    WATCHDOG->LOCK = ~(WDG_LOCK_TOKEN);
}

void hw_watchdog_disable(void)
{
    hw_watchdog_unlock();
    WATCHDOG->CONTROL = 0x0;
    WATCHDOG->LOAD = 0xffffffff;
    hw_watchdog_lock();
}

void hw_watchdog_isr(unsigned int delay)
{
    hw_watchdog_unlock();
    WATCHDOG->CONTROL = WDG_INTEN;
    WATCHDOG->LOAD = delay;
    hw_watchdog_lock();
}

void hw_watchdog_reset(unsigned int delay)
{
    hw_watchdog_unlock();
    WATCHDOG->CONTROL = WDG_RESEN | WDG_INTEN;
    WATCHDOG->LOAD = delay;
    hw_watchdog_lock();
}

void hw_watchdog_isr_clr(void)
{
    hw_watchdog_unlock();
    WATCHDOG->INTCLR = 0x1;
    hw_watchdog_lock();
}

void WDG_IRQHandler(void)
{
    duet_intrpt_enter();
    NVIC_DisableIRQ(WDG_IRQn);
    duet_intrpt_exit();
}

/**
 * This function will initialize the on board CPU hardware watch dog
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_init(duet_wdg_dev_t *wdg)
{
    uint32_t reg_value;
    if (NULL == wdg) {
        return EIO;
    }
    if (0 == wdg->port) {
        // OPEN WDG CLOCK
        reg_value = REG_RD(PERI_CLK_EN_REG1) & (~WDG_BUS_CLK_BIT);
        REG_WR(PERI_CLK_EN_REG1, (reg_value | (WDG_BUS_CLK_BIT)));
        // WDG CLOCK DIV SET
        reg_value = REG_RD(WDG_APB_DIV_REG);
        REG_WR(WDG_APB_DIV_REG, (reg_value | (WDG_APB_DIV << 4)));
        hw_watchdog_reset(wdg->config.timeout * (SYSTEM_CLOCK / (WDG_APB_DIV + 1) / 1000 /
                          2)); // 1000 for ms, 2 for watchdog feature
        // ENABLE WDG IRQ
        reg_value = REG_RD(DUTE_IRQ_EN_REG) & (~WDG_IRQ_BIT);
        REG_WR(DUTE_IRQ_EN_REG, (reg_value | (WDG_IRQ_BIT)));
        NVIC_EnableIRQ(WDG_IRQn);
        return 0;
    } else {
        return EIO;
    }
}

/**
 * Reload watchdog counter.
 *
 * @param[in]  wdg  the watch dog device
 */
void duet_wdg_reload(duet_wdg_dev_t *wdg)
{
    if (NULL == wdg) {
        return;
    }
    if (0 == wdg->port) {
        lega_rtos_declare_critical();
        lega_rtos_enter_critical();
        hw_watchdog_isr_clr();
        NVIC_ClearPendingIRQ(WDG_IRQn);
        NVIC_EnableIRQ(WDG_IRQn);
        lega_rtos_exit_critical();
    }
}

/**
 * This function performs any platform-specific cleanup needed for hardware watch dog.
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_finalize(duet_wdg_dev_t *wdg)
{
    uint32_t reg_value;
    if (NULL == wdg) {
        return EIO;
    }
    if (0 == wdg->port) {
        // DIS WDG IRQ
        reg_value = REG_RD(DUTE_IRQ_DIS_REG) & (~WDG_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (reg_value | (WDG_IRQ_BIT)));
        NVIC_DisableIRQ(WDG_IRQn);
        hw_watchdog_disable();
        // Set WDG Clock Disable
        reg_value = REG_RD(PERI_CLK_DIS_REG1) & (~WDG_BUS_CLK_BIT);
        REG_WR(PERI_CLK_DIS_REG1, (reg_value | (WDG_BUS_CLK_BIT)));
        return 0;
    } else {
        return EIO;
    }
}

