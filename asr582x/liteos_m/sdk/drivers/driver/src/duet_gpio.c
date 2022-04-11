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
#include "duet_cm4.h"
#include "duet.h"
#include "duet_gpio.h"

duet_gpio_cb_t g_duet_gpio_handler[DUET_GPIO_TOTAL_NUM];
// gpio pinmux function table
const uint32_t GPIO_PINMUX_FUN[DUET_GPIO_TOTAL_NUM] = {
    0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 2, 0, 0, 0, 4, 4,
    0, 0, 0, 0, 0, 0, 0, 0
};

void GPIO_IRQHandler(void)
{
    // duet_intrpt_enter();
    for (int i = 0; i < DUET_GPIO_TOTAL_NUM; i++) {
        if (i < DUET_GPIO_NUM_PER_GROUP) {
            // gpio group0 irq
            if (GPIO_GROUP0->INTSTATUS & (0x0001 << i)) {
                // clear GPIO GROUP0 interrupt
                GPIO_GROUP0->INTSTATUS = (0x0001 << i);
                if (g_duet_gpio_handler[i].cb) {
                    g_duet_gpio_handler[i].cb(g_duet_gpio_handler[i].arg);
                }
            }
        } else {
            // gpio group1 irq
            if (GPIO_GROUP1->INTSTATUS & (0x0001 << (i - DUET_GPIO_NUM_PER_GROUP))) {
                // clear GPIO GROUP1 interrupt
                GPIO_GROUP1->INTSTATUS = (0x0001 << (i - DUET_GPIO_NUM_PER_GROUP));
                if (g_duet_gpio_handler[i].cb) {
                    g_duet_gpio_handler[i].cb(g_duet_gpio_handler[i].arg);
                }
            }
        }
    }
    // duet_intrpt_exit();
}

/**
 * Initialises a GPIO pin
 *
 * @note  Prepares a GPIO pin for use.
 *
 * @param[in]  gpio           the gpio pin which should be initialised
 * @param[in]  configuration  A structure containing the required gpio configuration
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_init(duet_gpio_dev_t *gpio)
{
    uint32_t reg_value;
    if (NULL == gpio) {
        return EIO;
    }
    // pinmux setting
    if (gpio->port < 8) {
        reg_value = REG_RD(PINMUX_CTRL_REG0) & (~(0x0000000F << (4 * gpio->port)));
        REG_WR(PINMUX_CTRL_REG0, (reg_value | (GPIO_PINMUX_FUN[gpio->port] << (4 * gpio->port))));
    } else if (gpio->port < 16) {
        reg_value = REG_RD(PINMUX_CTRL_REG1) & (~(0x0000000F << (4 * (gpio->port - 8))));
        REG_WR(PINMUX_CTRL_REG1, (reg_value | (GPIO_PINMUX_FUN[gpio->port] << (4 * (gpio->port - 8)))));
    } else if (gpio->port < 24) {
        reg_value = REG_RD(PINMUX_CTRL_REG2) & (~(0x0000000F << (4 * (gpio->port - 16))));
        REG_WR(PINMUX_CTRL_REG2, (reg_value | (GPIO_PINMUX_FUN[gpio->port] << (4 * (gpio->port - 16)))));
    }
#if (defined DUET_A0V1)
    else if (gpio->port < 32) {
        reg_value = REG_RD(PINMUX_CTRL_REG3) & (~(0x0000000F << (4 * (gpio->port - 24))));
        REG_WR(PINMUX_CTRL_REG3, (reg_value | (GPIO_PINMUX_FUN[gpio->port] << (4 * (gpio->port - 24)))));
    }
#endif
    else {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        switch (gpio->config) {
            case DUET_ANALOG_MODE:
                break;
            case DUET_IRQ_MODE:
            case DUET_INPUT_PULL_UP:
                if (GPIO_GROUP0->OUTENSET & (1 << gpio->port)) {
                    GPIO_GROUP0->OUTENCLR = (1 << gpio->port);
                }
                reg_value = REG_RD(HW_CTRL_PE_PS);
                REG_WR(HW_CTRL_PE_PS, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PS_REG);
                REG_WR(PAD_PS_REG, (reg_value | (1 << gpio->port)));
                reg_value = REG_RD(PAD_PE_REG);
                REG_WR(PAD_PE_REG, (reg_value | (1 << gpio->port)));
                break;
            case DUET_INPUT_PULL_DOWN:
                if (GPIO_GROUP0->OUTENSET & (1 << gpio->port)) {
                    GPIO_GROUP0->OUTENCLR = (1 << gpio->port);
                }
                reg_value = REG_RD(HW_CTRL_PE_PS);
                REG_WR(HW_CTRL_PE_PS, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PS_REG);
                REG_WR(PAD_PS_REG, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PE_REG);
                REG_WR(PAD_PE_REG, (reg_value | (1 << gpio->port)));
                break;
            case DUET_INPUT_HIGH_IMPEDANCE:
                if (GPIO_GROUP0->OUTENSET & (1 << gpio->port)) {
                    GPIO_GROUP0->OUTENCLR = (1 << gpio->port);
                }
                break;
            case DUET_OUTPUT_PUSH_PULL:
            case DUET_OUTPUT_OPEN_DRAIN_NO_PULL:
            case DUET_OUTPUT_OPEN_DRAIN_PULL_UP:
                if (!(GPIO_GROUP0->OUTENSET & (1 << gpio->port))) {
                    GPIO_GROUP0->OUTENSET = (1 << gpio->port);
                }
                break;
            default:
                return EIO;
                // break;
        }
    } else { // if(gpio->port < 32/24)
        switch (gpio->config) {
            case DUET_ANALOG_MODE:
                break;
            case DUET_IRQ_MODE:
                break;
            case DUET_INPUT_PULL_UP:
                if (GPIO_GROUP1->OUTENSET & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP))) {
                    GPIO_GROUP1->OUTENCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                }
                reg_value = REG_RD(HW_CTRL_PE_PS);
                REG_WR(HW_CTRL_PE_PS, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PS_REG);
                REG_WR(PAD_PS_REG, (reg_value | (1 << gpio->port)));
                reg_value = REG_RD(PAD_PE_REG);
                REG_WR(PAD_PE_REG, (reg_value | (1 << gpio->port)));
                break;
            case DUET_INPUT_PULL_DOWN:
                if (GPIO_GROUP1->OUTENSET & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP))) {
                    GPIO_GROUP1->OUTENCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                }
                reg_value = REG_RD(HW_CTRL_PE_PS);
                REG_WR(HW_CTRL_PE_PS, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PS_REG);
                REG_WR(PAD_PS_REG, (reg_value & (~(1 << gpio->port))));
                reg_value = REG_RD(PAD_PE_REG);
                REG_WR(PAD_PE_REG, (reg_value | (1 << gpio->port)));
                break;
            case DUET_INPUT_HIGH_IMPEDANCE:
                if (GPIO_GROUP1->OUTENSET & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP))) {
                    GPIO_GROUP1->OUTENCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                }
                break;
            case DUET_OUTPUT_PUSH_PULL:
            case DUET_OUTPUT_OPEN_DRAIN_NO_PULL:
            case DUET_OUTPUT_OPEN_DRAIN_PULL_UP:
                if (!(GPIO_GROUP1->OUTENSET & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP)))) {
                    GPIO_GROUP1->OUTENSET = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                }
                break;
            default:
                return EIO;
                // break;
        }
    }
    return 0;
}

/**
 * Sets an output GPIO pin high
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set high
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_high(duet_gpio_dev_t *gpio)
{
    if (NULL == gpio) {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        GPIO_GROUP0->DATAOUT |= (1 << gpio->port);
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        GPIO_GROUP1->DATAOUT |= (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
    } else {
        return EIO;
    }
    return 0;
}

/**
 * Sets an output GPIO pin low
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_low(duet_gpio_dev_t *gpio)
{
    if (NULL == gpio) {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        GPIO_GROUP0->DATAOUT &= ~(1 << gpio->port);
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        GPIO_GROUP1->DATAOUT &= ~(1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
    } else {
        return EIO;
    }
    return 0;
}

/**
 * Get the state of an output GPIO pin. Using this function on a
 * gpio pin which is set to input mode will return an undefined value.
 *
 * @param[in]  gpio   the gpio pin which should be read
 * @param[in]  value  gpio value
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 *
 * note: defined by asr
 */
int32_t duet_gpio_output_get(duet_gpio_dev_t *gpio, uint32_t *value)
{
    if (NULL == gpio) {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        *value = (GPIO_GROUP0->DATAOUT >> (gpio->port)) & 1;
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        *value = (GPIO_GROUP1->DATAOUT >> ((gpio->port - DUET_GPIO_NUM_PER_GROUP))) & 1;
    } else {
        return EIO;
    }
    return 0;
}

/**
 * Trigger an output GPIO pin's output. Using this function on a
 * gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_toggle(duet_gpio_dev_t *gpio)
{
    uint32_t value;
    if (NULL == gpio) {
        return EIO;
    }
    if (0 == duet_gpio_output_get(gpio, &value)) {
        if (value) {
            return duet_gpio_output_low(gpio);
        } else {
            return duet_gpio_output_high(gpio);
        }
    } else {
        return EIO;
    }
}

/**
 * Get the state of an input GPIO pin. Using this function on a
 * gpio pin which is set to output mode will return an undefined value.
 *
 * @param[in]  gpio   the gpio pin which should be read
 * @param[in]  value  gpio value
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_input_get(duet_gpio_dev_t *gpio, uint32_t *value)
{
    if (NULL == gpio) {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        *value = (GPIO_GROUP0->DATA >> gpio->port) & 1;
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        *value = (GPIO_GROUP1->DATA >> (gpio->port - DUET_GPIO_NUM_PER_GROUP)) & 1;
    } else {
        return EIO;
    }
    return 0;
}

/**
 * Enables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which is set to
 * output mode is undefined.
 *
 * @param[in]  gpio     the gpio pin which will provide the interrupt trigger
 * @param[in]  trigger  the type of trigger (rising/falling edge)
 * @param[in]  handler  a function pointer to the interrupt handler
 * @param[in]  arg      an argument that will be passed to the interrupt handler
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_enable_irq(duet_gpio_dev_t *gpio, duet_gpio_irq_trigger_t trigger,
                             duet_gpio_irq_handler_t handler, void *arg)
{
    if (NULL == gpio) {
        return EIO;
    }
    g_duet_gpio_handler[gpio->port].cb = handler;
    g_duet_gpio_handler[gpio->port].arg = arg;
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        switch (trigger) {
            case DUET_IRQ_TRIGGER_RISING_EDGE:
                GPIO_GROUP0->INTTYPESET = (1 << gpio->port); // edge or level trig
                GPIO_GROUP0->INTPOLSET = (1 << gpio->port); // trig polarity
                break;
            case DUET_IRQ_TRIGGER_FALLING_EDGE:
                GPIO_GROUP0->INTTYPESET = (1 << gpio->port);
                GPIO_GROUP0->INTPOLCLR = (1 << gpio->port);
                break;
            case DUET_IRQ_TRIGGER_BOTH_EDGES:
            default:
                return EIO;
        }
        GPIO_GROUP0->INTENSET = (1 << gpio->port); // int enable
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        switch (trigger) {
            case DUET_IRQ_TRIGGER_RISING_EDGE:
                GPIO_GROUP1->INTTYPESET = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                GPIO_GROUP1->INTPOLSET = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                break;
            case DUET_IRQ_TRIGGER_FALLING_EDGE:
                GPIO_GROUP1->INTTYPESET = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                GPIO_GROUP1->INTPOLCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
                break;
            case DUET_IRQ_TRIGGER_BOTH_EDGES:
            default:
                return EIO;
        }
        GPIO_GROUP1->INTENSET = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
    } else {
        return EIO;
    }
    NVIC_EnableIRQ(GPIO_IRQn); // 0x10
    return 0;
}

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref duet_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_disable_irq(duet_gpio_dev_t *gpio)
{
    if (NULL == gpio) {
        return EIO;
    }
    g_duet_gpio_handler[gpio->port].cb = NULL;
    g_duet_gpio_handler[gpio->port].arg = NULL;
    // NVIC_DisableIRQ(GPIO_IRQn); // common use
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        GPIO_GROUP0->INTENCLR = (1 << gpio->port);
        GPIO_GROUP0->INTTYPECLR = (1 << gpio->port);
        GPIO_GROUP0->INTPOLCLR = (1 << gpio->port);
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        GPIO_GROUP1->INTTYPECLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
        GPIO_GROUP1->INTPOLCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
        GPIO_GROUP1->INTENCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
    } else {
        return EIO;
    }
    return 0;
}

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref duet_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_clear_irq(duet_gpio_dev_t *gpio)
{
    if (NULL == gpio) {
        return EIO;
    }

    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        // GPIO interrupt status
        if (GPIO_GROUP0->INTSTATUS & (1 << gpio->port)) {
            // clear GPIO interrupt status
            GPIO_GROUP0->INTSTATUS = (1 << gpio->port);
        }
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        if (GPIO_GROUP1->INTSTATUS & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP))) {
            GPIO_GROUP1->INTSTATUS = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
        }
    } else {
        return EIO;
    }

    return 0;
}

/**
 * Set a GPIO pin in default state.
 *
 * @param[in]  gpio  the gpio pin which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_finalize(duet_gpio_dev_t *gpio)
{
    if (NULL == gpio) {
        return EIO;
    }
    if (gpio->port < DUET_GPIO_NUM_PER_GROUP) {
        if (GPIO_GROUP0->OUTENSET & (1 << gpio->port)) {
            GPIO_GROUP0->OUTENCLR = (1 << gpio->port);
        }
    } else if (gpio->port < DUET_GPIO_TOTAL_NUM) {
        if (GPIO_GROUP1->OUTENSET & (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP))) {
            GPIO_GROUP1->OUTENCLR = (1 << (gpio->port - DUET_GPIO_NUM_PER_GROUP));
        }
    } else {
        return EIO;
    }
    duet_gpio_disable_irq(gpio);
    duet_gpio_clear_irq(gpio);
    return 0;
}
