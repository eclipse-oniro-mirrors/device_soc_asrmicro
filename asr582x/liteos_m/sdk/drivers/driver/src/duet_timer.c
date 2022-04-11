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
#include "duet_timer.h"

duet_timer_cb_t g_duet_timer_handler[DUET_TIMER_NUM];

void TIMER_IRQHandler(void)
{
    //    duet_intrpt_enter();
    if (TIMER1->MIS) {
        TIMER1->INTCLR = 1; // clear irq
        if (g_duet_timer_handler[DUET_TIMER1_INDEX].cb) {
            g_duet_timer_handler[DUET_TIMER1_INDEX].cb(g_duet_timer_handler[DUET_TIMER1_INDEX].arg);
        }
    }
    if (TIMER2->MIS) {
        TIMER2->INTCLR = 1; // clear irq
        if (g_duet_timer_handler[DUET_TIMER2_INDEX].cb) {
            g_duet_timer_handler[DUET_TIMER2_INDEX].cb(g_duet_timer_handler[DUET_TIMER2_INDEX].arg);
        }
    }
    //    duet_intrpt_exit();
}

/**
 * init a hardware timer
 *
 * @param[in]  tmr         timer struct
 * @param[in]  period      micro seconds for repeat timer trigger
 * @param[in]  auto_reoad  set to 0, if you just need oneshot timer
 * @param[in]  cb          callback to be triggered after useconds
 * @param[in]  ch          timer channel
 * @param[in]  arg         passed to cb
 */
int32_t duet_timer_init(duet_timer_dev_t *tim)
{
    uint8_t timer_mode;
    uint32_t reg_value;
    uint32_t system_clock = SYSTEM_CLOCK;

    if (NULL == tim) {
        return EIO;
    }
    if (tim->port >= DUET_TIMER_NUM) {
        return EIO;
    }
    // Set Timer Clock Enable
    reg_value = REG_RD(PERI_CLK_EN_REG1) & (~TIMER_BUS_CLK_BIT);
    REG_WR(PERI_CLK_EN_REG1, (reg_value | (TIMER_BUS_CLK_BIT)));
    if (TIMER_RELOAD_AUTO == tim->config.reload_mode) {
        timer_mode = PERIODIC_MODE;
    } else if (TIMER_RELOAD_MANU == tim->config.reload_mode) {
        timer_mode = ONE_SHOT_MODE;
    } else {
        return EIO;
    }
    g_duet_timer_handler[tim->port].cb = tim->config.cb;
    g_duet_timer_handler[tim->port].arg = tim->config.arg;
    if (DUET_TIMER1_INDEX == tim->port) {
        TIMER1->CONTROL = TIMER_DISABLE; // disable timer first
        TIMER1->LOAD = tim->config.period * (system_clock / 1000000); // 1000000 for us
        TIMER1->CONTROL |= (TIMER_PRESCALE | TIMER_SIZE | timer_mode); // timer control
    } else { // if(DUET_TIMER2_INDEX == tim->port)
        TIMER2->CONTROL = TIMER_DISABLE; // disable timer first
        TIMER2->LOAD = tim->config.period * (system_clock / 1000000); // 1000000 for us
        TIMER2->CONTROL |= (TIMER_PRESCALE | TIMER_SIZE | timer_mode); // timer control
    }
    return 0;
}

/**
 * start a hardware timer
 *
 * @return  0 == success, EIO == failure
 */
int32_t duet_timer_start(duet_timer_dev_t *tim)
{
    uint32_t reg_value;
    if (NULL == tim) {
        return EIO;
    }
    if (DUET_TIMER1_INDEX == tim->port) {
        TIMER1->CONTROL |= (TIMER_ENABLE | INTERRUPT_EN); // timer control
    } else if (DUET_TIMER2_INDEX == tim->port) {
        TIMER2->CONTROL |= (TIMER_ENABLE | INTERRUPT_EN); // timer control
    } else {
        return EIO;
    }
    // open TIMER interrupt
    reg_value = REG_RD(DUTE_IRQ_EN_REG) & (~TIMER_IRQ_BIT);
    REG_WR(DUTE_IRQ_EN_REG, (reg_value | (TIMER_IRQ_BIT)));
    NVIC_EnableIRQ(TIMER_IRQn); // 0x20
    return 0;
}

/**
 * get hardware timer remain time
 *
 * @return   success return remain time, EIO == failure
 */
int32_t duet_timer_get(duet_timer_dev_t *tim)
{
    uint32_t reg_value = 0;
    uint32_t system_clock = SYSTEM_CLOCK;

    if (NULL == tim) {
        return -1;
    }
    if (DUET_TIMER1_INDEX == tim->port) {
        reg_value = TIMER1->VALUE; // timer current value
        return (reg_value / (system_clock / 1000000)); // time for us
    } else if (DUET_TIMER2_INDEX == tim->port) {
        reg_value = TIMER2->VALUE; // timer current value
        return (reg_value / (system_clock / 1000000)); // time for us
    } else {
        return -1;
    }
    return 0;
}

/**
 * reload hardware timer value
 *
 * @return   0 == success, EIO == failure
 */
int32_t duet_timer_reload(duet_timer_dev_t *tim)
{
    uint32_t system_clock = SYSTEM_CLOCK;

    if (NULL == tim) {
        return -1;
    }
    if (tim->port >= DUET_TIMER_NUM) {
        return -1;
    }
    if (DUET_TIMER1_INDEX == tim->port) {
        TIMER1->LOAD = tim->config.period * (system_clock / 1000000); // 1000000 for us
    } else { // if(DUET_TIMER2_INDEX == tim->port)
        TIMER2->LOAD = tim->config.period * (system_clock / 1000000); // 1000000 for us
    }
    return 0;
}

/**
 * stop a hardware timer
 *
 * @param[in]  tmr  timer struct
 * @param[in]  cb   callback to be triggered after useconds
 * @param[in]  arg  passed to cb
 */
void duet_timer_stop(duet_timer_dev_t *tim)
{
    if (NULL == tim) {
        return;
    }

    if (DUET_TIMER1_INDEX == tim->port) {
        TIMER1->CONTROL = TIMER_DISABLE; // timer control
    } else if (DUET_TIMER2_INDEX == tim->port) {
        TIMER2->CONTROL = TIMER_DISABLE; // timer control
    } else {
        return;
    }

    // two timers use same IRQ
    // NVIC_DisableIRQ(TIMER_IRQn);
    g_duet_timer_handler[tim->port].cb = NULL;
    g_duet_timer_handler[tim->port].arg = NULL;
}

/**
 * De-initialises an TIMER interface, Turns off an TIMER hardware interface
 *
 * @param[in]  timer  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_timer_finalize(duet_timer_dev_t *tim)
{
    // uint32_t reg_value;
    duet_timer_stop(tim);
    // Set Timer Clock Disable
    // one clk enable for 2 timer
    // reg_value = REG_RD(PERI_CLK_DIS_REG1) & (~TIMER_BUS_CLK_BIT);
    // REG_WR(PERI_CLK_DIS_REG1, (reg_value | (TIMER_BUS_CLK_BIT)));
    return 0;
}
