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

#ifndef _DUET_WDG_H_
#define _DUET_WDG_H_
#include <stdint.h>

typedef struct {
    uint32_t timeout;  /* Watchdag timeout */
} duet_wdg_config_t;

typedef struct {
    uint8_t      port;   /* wdg port */
    duet_wdg_config_t config; /* wdg config */
    void        *priv;   /* priv data */
} duet_wdg_dev_t;

/**
 * This function will initialize the on board CPU hardware watch dog
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_init(duet_wdg_dev_t *wdg);

/**
 * Reload watchdog counter.
 *
 * @param[in]  wdg  the watch dog device
 */
void duet_wdg_reload(duet_wdg_dev_t *wdg);

/**
 * This function performs any platform-specific cleanup needed for hardware watch dog.
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_finalize(duet_wdg_dev_t *wdg);

#endif // _LEGA_WDG_H_