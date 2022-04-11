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

#ifndef _DUET_GPIO_H_
#define _DUET_GPIO_H_
#include <stdint.h>
#include <errno.h>
#define GPIO0_INDEX 0
#define GPIO1_INDEX 1
#define GPIO2_INDEX 2
#define GPIO3_INDEX 3
#define GPIO4_INDEX 4
#define GPIO5_INDEX 5
#define GPIO6_INDEX 6
#define GPIO7_INDEX 7
#define GPIO8_INDEX 8
#define GPIO9_INDEX 9
#define GPIO10_INDEX 10
#define GPIO11_INDEX 11
#define GPIO12_INDEX 12
#define GPIO13_INDEX 13
#define GPIO14_INDEX 14
#define GPIO15_INDEX 15
#define GPIO16_INDEX 16
#define GPIO17_INDEX 17
#define GPIO18_INDEX 18
#define GPIO19_INDEX 19
#define GPIO20_INDEX 20
#define GPIO21_INDEX 21
#define GPIO22_INDEX 22
#define GPIO23_INDEX 23
#define GPIO24_INDEX 24
#define GPIO25_INDEX 25
#define GPIO26_INDEX 26
#define GPIO27_INDEX 27
#define GPIO28_INDEX 28
#define GPIO29_INDEX 29
#define GPIO30_INDEX 30
#define GPIO31_INDEX 31
#define DUET_GPIO_NUM_PER_GROUP 16

#define DUET_GPIO_TOTAL_NUM     24

/*
 * Pin configuration
 */
typedef enum {
    DUET_ANALOG_MODE,               /* Used as a function pin, input and output analog */
    DUET_IRQ_MODE,                  /* Used to trigger interrupt */
    DUET_INPUT_PULL_UP,             /* Input, with an internal pull-up resistor */
    DUET_INPUT_PULL_DOWN,           /* Input, with an internal pull-down resistor */
    DUET_INPUT_HIGH_IMPEDANCE,      /* Input, must always be driven, either actively or by an external pullup resistor */
    DUET_OUTPUT_PUSH_PULL,          /* Output, actively driven high and actively driven low */
    DUET_OUTPUT_OPEN_DRAIN_NO_PULL, /* Output, actively driven low. When set high, is high-impedance */
    DUET_OUTPUT_OPEN_DRAIN_PULL_UP, /* Output, actively driven low. When set high, is pulled high with an internal resistor */
} duet_gpio_config_t;

/*
 * GPIO dev struct
 */
typedef struct {
    uint8_t       port;    /* gpio port */
    duet_gpio_config_t config;  /* gpio config */
    void         *priv;    /* priv data */
} duet_gpio_dev_t;

/*
 * GPIO interrupt trigger
 */
typedef enum {
    DUET_IRQ_TRIGGER_RISING_EDGE  = 0x1, /* Interrupt triggered at input signal's rising edge  */
    DUET_IRQ_TRIGGER_FALLING_EDGE = 0x2, /* Interrupt triggered at input signal's falling edge */
    DUET_IRQ_TRIGGER_BOTH_EDGES   = DUET_IRQ_TRIGGER_RISING_EDGE | DUET_IRQ_TRIGGER_FALLING_EDGE,
} duet_gpio_irq_trigger_t;

/*
 * GPIO interrupt callback handler
 */
typedef void (*duet_gpio_irq_handler_t)(void *arg);

typedef struct {
    duet_gpio_irq_handler_t cb;
    void *arg;
} duet_gpio_cb_t;

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
int32_t duet_gpio_init(duet_gpio_dev_t *gpio);

/**
 * Sets an output GPIO pin high
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set high
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_high(duet_gpio_dev_t *gpio);

/**
 * Sets an output GPIO pin low
 *
 * @note  Using this function on a gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_low(duet_gpio_dev_t *gpio);

/**
 * Trigger an output GPIO pin's output. Using this function on a
 * gpio pin which is set to input mode is undefined.
 *
 * @param[in]  gpio  the gpio pin which should be set low
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_output_toggle(duet_gpio_dev_t *gpio);

/**
 * Get the state of an input GPIO pin. Using this function on a
 * gpio pin which is set to output mode will return an undefined value.
 *
 * @param[in]  gpio   the gpio pin which should be read
 * @param[in]  value  gpio value
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_input_get(duet_gpio_dev_t *gpio, uint32_t *value);

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
                             duet_gpio_irq_handler_t handler, void *arg);

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref duet_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_disable_irq(duet_gpio_dev_t *gpio);

/**
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref duet_gpio_input_irq_enable is undefined.
 *
 * @param[in]  gpio  the gpio pin which provided the interrupt trigger
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_clear_irq(duet_gpio_dev_t *gpio);

/**
 * Set a GPIO pin in default state.
 *
 * @param[in]  gpio  the gpio pin which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_gpio_finalize(duet_gpio_dev_t *gpio);

#endif // _DUET_GPIO_H_
