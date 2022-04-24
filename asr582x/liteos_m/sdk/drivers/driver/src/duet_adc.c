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

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "duet_cm4.h"
#include "duet_common.h"
#include "duet_rf_spi.h"
#include "duet_cm4.h"
#include "duet.h"
#include "duet_gpio.h"
#include "duet_adc.h"

#if defined ADC_ENABLE
duet_adc_callback_func g_duet_adc_callback_handler;
uint16_t reg_f_value = 0;
static void duet_rf_auxadc_mode(AUX_ADC_MOD mode)
{
    // have verify on case 12,pass.
    spi_mst_write(0x06, 0x07AF);
    spi_mst_write(0x07, 0xBFF3);
    rf_set_reg_bit(0x1E, 3, 3, 0x7);
    spi_mst_write(0x0F, 0x8000);
    spi_mst_write(0x75, 0xE9C0);
    if (mode == MOD_TRIG) {
        rf_set_reg_bit(0xA3, 6, 1, 0x0);
    } else if (mode == MOD_CNT10) {
        rf_set_reg_bit(0xA3, 6, 1, 0x1);
    }
    delay(250);
}

int32_t duet_adc_init(duet_adc_dev_t *adc_config)
{
    uint32_t reg_value;
    uint8_t temp_chan;
    duet_gpio_dev_t config_gpio;
    if (adc_config->port > 9) {
        return 0;
    }
    config_gpio.port = adc_config->port + 4;
    config_gpio.config = DUET_INPUT_HIGH_IMPEDANCE;
    config_gpio.priv = NULL;
    duet_gpio_init(&config_gpio);
    reg_value = REG_RD(HW_CTRL_PE_PS);
    REG_WR(HW_CTRL_PE_PS, (reg_value & (~(1 << config_gpio.port)))); // cfg by// user
    reg_value = REG_RD(PAD_PE_REG);                                  //
    // REG_WR(PAD_PE_REG, (reg_value|((1 << gpio->port))));
    REG_WR(PAD_PE_REG, (reg_value & (~(1 << config_gpio.port))));
    //    adc_config->duet_adc_handler_struct.cb = adc_config->priv;
    if (adc_config->priv) {
        duet_rf_auxadc_mode(MOD_CNT10);
    } else {
        duet_rf_auxadc_mode(MOD_TRIG);
    }
    //    duet_rf_auxadc_mode(adc_config->mode);
    rf_set_reg_bit(0xa3, 12, 2, ADC_SAMPLE_125K);
    // Enable AUXADC
    rf_set_reg_bit(0x06, 14, 1, 0x0);
    // Enable TMMT
    rf_set_reg_bit(0x07, 4, 1, 0x0);
    // Enable CLK AUXADC13M; D_XO_CLK_AUXADC13M_EN= 1
    rf_set_reg_bit(0x75, 14, 1, 0x1);
    // Enable XO CLK AUCADC, DFF's RB;D_RST_XO_CLK_AUXADC= 0
    rf_set_reg_bit(0x0F, 11, 1, 0);
    delay(250); // delay(2000)
    // Open channel 1
    if (adc_config->port <= 7) {
        REG_WR(SYS_REG_BASE_CLKCTRL_ENABLE, REG_RD(SYS_REG_BASE_CLKCTRL_ENABLE) | AUX_ADC_CLK);
        REG_WR(SYS_REG_BASE_IRQ_ENABLE, REG_RD(SYS_REG_BASE_IRQ_ENABLE) | AUX_ADC_IRQ);
        REG_WR(SYS_REG_BASE_XOCTRL2, ((REG_RD(SYS_REG_BASE_XOCTRL2) & (~(uint32_t)0x7)) | (uint32_t)adc_config->port) | ((
                    uint32_t)(BIT(9))));
        REG_WR(SYS_REG_BASE_AUXADC, REG_RD(SYS_REG_BASE_AUXADC) | ((uint32_t)(BIT(9))));
    } else {
        temp_chan = adc_config->port - 8;
        REG_WR(SYS_REG_BASE_XOCTRL2, ((REG_RD(SYS_REG_BASE_XOCTRL2) & (~(uint32_t)0x7)) | (uint32_t)temp_chan) & (~(uint32_t)(
                    BIT(9))));
    }
    delay(200);
    if (adc_config->priv) {
        g_duet_adc_callback_handler = (duet_adc_callback_func)(adc_config->priv);
        ADC->BITS_ADC_CTRL.adc_int_clr = 0;
        ADC->BITS_ADC_CTRL.adc_int_mode = MOD_CNT10;
        ADC->BITS_ADC_CTRL.adc_int_en = 1;
        NVIC_EnableIRQ(AUX_ADC_IRQn);
    } else {
        g_duet_adc_callback_handler = NULL;
        ADC->BITS_ADC_CTRL.adc_int_clr = 0;
        ADC->BITS_ADC_CTRL.adc_int_mode = MOD_TRIG;
        ADC->BITS_ADC_CTRL.adc_int_en = 0;
    }
    return 0;
}

int32_t duet_adc_get(duet_adc_dev_t *adc_config)
{
    int32_t vol_value = 0;
    if (rf_get_reg_bit(0x06, 14, 1)) {
        return 0;
    }
    vol_value = (ADC->ADC_DATA & 0xFFF0) >> 4;
    if (adc_config->port < 8) {
        return (int32_t)(0.4243 * vol_value + 6.9805);
    } else {
        return 0;
    }
}
int32_t duet_tempr_get(duet_adc_dev_t *adc_config)
{
    static int16_t temp_n, temp_p;
    static uint8_t tp_flag = 0;
    if (adc_config->port < 8) {
        return 0;
    }
    if (adc_config->port == ADC_CHANNEL_TEMN) {
        temp_n = (int16_t)((ADC->ADC_DATA & 0xFFF0) >> 4);
        tp_flag |= 0x1;
    } else {
        temp_p = (int16_t)((ADC->ADC_DATA & 0xFFF0) >> 4);
        tp_flag |= 0x2;
    }
    if (tp_flag == 0x3) {
        tp_flag = 0;
        return (int32_t)((0.29 * (temp_n - temp_p)) / 5.25 + 41.5);
    } else {
        return 0;
    }
}

int32_t duet_adc_finalize(duet_adc_dev_t *adc_config)
{
    // Close AUXADC
    rf_set_reg_bit(0x06, 14, 1, 0x1);
    // Close XO CLK AUCADC; D_RST_XO_CLK_AUXADC= 1
    spi_mst_write(0x0f, reg_f_value);
    // Close CLK AUXADC13M; D_XO_CLK_AUXADC13M_EN= 0
    rf_set_reg_bit(0x75, 14, 1, 0x0);
    ADC->BITS_ADC_CTRL.adc_int_clr = 0;
    ADC->BITS_ADC_CTRL.adc_int_mode = 0;
    ADC->BITS_ADC_CTRL.adc_int_en = 0;
    NVIC_DisableIRQ(AUX_ADC_IRQn);
    return 0;
}

void AUX_ADC_IRQHandler(void)
{
    uint32_t vol_value[10] = {0};
    if (g_duet_adc_callback_handler) {
        for (uint8_t i = 0; i < 10; i++) {
            vol_value[i] = (ADC->ADC_DATA & 0xFFF0) >> 4;
        }
        g_duet_adc_callback_handler(vol_value);
    }
    ADC->BITS_ADC_CTRL.adc_int_clr = 1;
    while (ADC->BITS_ADC_CTRL.adc_int_clr) {
        ADC->BITS_ADC_CTRL.adc_int_clr = 0;
    }
}

#endif
