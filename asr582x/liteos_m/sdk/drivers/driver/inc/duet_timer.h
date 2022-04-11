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

#ifndef _DUET_TIMER_H_
#define _DUET_TIMER_H_
#include <stdint.h>

#define DUET_TIMER1_INDEX 0
#define DUET_TIMER2_INDEX 1
#define DUET_TIMER_NUM 2

#define TIMER_RELOAD_AUTO  1  /* timer reload automatic */
#define TIMER_RELOAD_MANU  2  /* timer reload manual */

typedef void (*duet_timer_cb_handler_t)(void *arg);
typedef struct {
    duet_timer_cb_handler_t cb;
    void *arg;
} duet_timer_cb_t;

typedef struct {
    uint32_t       period;   /* us */
    uint8_t        reload_mode;
    duet_timer_cb_handler_t cb;
    void          *arg;
} duet_timer_config_t;

typedef struct {
    int8_t         port;   /* timer port */
    duet_timer_config_t config; /* timer config */
    void          *priv;   /* priv data */
} duet_timer_dev_t;

/**
 * init a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_timer_init(duet_timer_dev_t *tim);

/**
 * start a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_timer_start(duet_timer_dev_t *tim);

/**
 * get hardware timer remain time
 *
 * @return   success return remain time, EIO == failure
 */
int32_t duet_timer_get(duet_timer_dev_t *tim);
/**
 * reload hardware timer value
 *
 * @return   0 == success, EIO == failure
 */
int32_t duet_timer_reload(duet_timer_dev_t *tim);

/**
 * stop a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  none
 */
void duet_timer_stop(duet_timer_dev_t *tim);

/**
 * De-initialises an TIMER interface, Turns off an TIMER hardware interface
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_timer_finalize(duet_timer_dev_t *tim);

#endif // _DUET_TIMER_H_