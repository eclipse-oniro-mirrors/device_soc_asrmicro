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

#ifndef _DUET_RTC_H_
#define _DUET_RTC_H_
#include <stdint.h>
#include <errno.h>

#define HAL_RTC_FORMAT_DEC  1
#define HAL_RTC_FORMAT_BCD  2

typedef struct {
    uint8_t  format;    /* time formart DEC or BCD */
} duet_rtc_config_t;

typedef struct {
    uint8_t port;        /* rtc port */
    duet_rtc_config_t config; /* rtc config */
    void   *priv;        /* priv data */
} duet_rtc_dev_t;

/*
 * RTC time
 */
typedef struct {
    uint8_t sec;         /* DEC format:value range from 0 to 59, BCD format:value range from 0x00 to 0x59 */
    uint8_t min;         /* DEC format:value range from 0 to 59, BCD format:value range from 0x00 to 0x59 */
    uint8_t hr;          /* DEC format:value range from 0 to 23, BCD format:value range from 0x00 to 0x23 */
    uint8_t weekday;     /* DEC format:value range from 1 to  7, BCD format:value range from 0x01 to 0x07 */
    uint8_t date;        /* DEC format:value range from 1 to 31, BCD format:value range from 0x01 to 0x31 */
    uint8_t month;       /* DEC format:value range from 1 to 12, BCD format:value range from 0x01 to 0x12 */
    uint8_t year;        /* DEC format:value range from 0 to 99, BCD format:value range from 0x00 to 0x99 */
} duet_rtc_time_t;

/**
 * This function will initialize the on board CPU real time clock
 *
 *
 * @param[in]  rtc  rtc device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_init(duet_rtc_dev_t *rtc);

/**
 * This function will return the value of time read from the on board CPU real time clock.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_get_time(duet_rtc_dev_t *rtc, duet_rtc_time_t *time);

/**
 * This function will set MCU RTC time to a new value.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_set_time(duet_rtc_dev_t *rtc, const duet_rtc_time_t *time);

/**
 * De-initialises an RTC interface, Turns off an RTC hardware interface
 *
 * @param[in]  RTC  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_finalize(duet_rtc_dev_t *rtc);

#endif //_DUET_RTC_H_