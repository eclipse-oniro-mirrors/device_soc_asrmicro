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

#ifndef _DUET_ADC_H_
#define _DUET_ADC_H_

#include "core_cm4.h"
#define ADC_ENABLE

#if defined ADC_ENABLE
typedef enum {
    ADC_SAMPLE_1M,
    ADC_SAMPLE_500K,
    ADC_SAMPLE_250K,
    ADC_SAMPLE_125K
} AUX_ADC_SMP_RATE;

#define  AUX_ADC_CLK  (1<<12)
#define  ADC_SAMPLE_SEL ADC_SAMPLE_125K
#define  AUX_ADC_IRQ  (1<<20)

#define SYS_REG_BASE                  0x40000000
#define SYS_REG_BASE_CLKCTRL_ENABLE   ((SYS_REG_BASE + 0x844))
#define SYS_REG_BASE_CLKCTRL_DISABLE  ((SYS_REG_BASE + 0x84C))
#define SYS_REG_BASE_IRQ_ENABLE       ((SYS_REG_BASE + 0x944))
#define SYS_REG_BASE_IRQ_DISABLE      ((SYS_REG_BASE + 0x948))

#define SYS_REG_BASE_AUXADC           ((SYS_REG_BASE + 0x89000))
#define SYS_REG_BASE_XOCTRL2          ((SYS_REG_BASE + 0xA70))
#define SYS_REG_BASE_REF_ROOT_CLK     ((SYS_REG_BASE + 0x80C))
#define SYS_REG_BASE_WIFI_CLK         ((SYS_REG_BASE + 0x85C))
#define SYS_REG_BASE_AUXADC           ((SYS_REG_BASE + 0x89000))
typedef void (*duet_adc_callback_func)(void *arg);

typedef struct {
    union {
        struct {
            __IO uint32_t adc_resv : 24;
            __IO uint32_t adc_int_mode : 1;
            __IO uint32_t adc_int_en : 1;
            __IO uint32_t adc_int_clr : 1;
            __IO uint32_t adc_resv1 : 5;
        } BITS_ADC_CTRL;
        __IO uint32_t ADC_CTRL; /* adc control */
    };
    __IO uint32_t ADC_DATA;
}                    duet_ADC;
#define ADC  ((duet_ADC *)(SYS_REG_BASE_AUXADC))
#define BIT(pos) (1U<<(pos))
/* Modem Config */
#define MDM_CLKGATEFCTRL0_ADDR          0x60C00874

typedef enum {
    ADC_CHANNEL_NUM0,
    ADC_CHANNEL_NUM1,
    ADC_CHANNEL_NUM2,
    ADC_CHANNEL_NUM3,
    ADC_CHANNEL_NUM4,
    ADC_CHANNEL_NUM5,
    ADC_CHANNEL_NUM6,
    ADC_CHANNEL_NUM7,
    ADC_CHANNEL_TEMN,
    ADC_CHANNEL_TEMP
} duet_adc_channel_t;

typedef struct {
    uint32_t sampling_cycle;  /* sampling period in number of ADC clock cycles */
} duet_adc_config_t;
typedef enum {
    MOD_TRIG,
    MOD_CNT10
} AUX_ADC_MOD;

typedef struct {
    uint8_t           port;   /* adc port */
    duet_adc_config_t config; /* adc config */
    void              *priv;   /* priv data */
} duet_adc_dev_t;

int32_t duet_adc_init(duet_adc_dev_t *adc_config);

int32_t duet_adc_get(duet_adc_dev_t *adc_config);

int32_t duet_adc_finalize(duet_adc_dev_t *adc_config);

int32_t duet_tempr_get(duet_adc_dev_t *adc_config);
#endif
#endif // _duet_ADC_H_
