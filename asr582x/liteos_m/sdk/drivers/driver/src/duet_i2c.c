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
#include <string.h>
#include "duet_cm4.h"
#include "duet.h"
#include "duet_timer.h"
#include "duet_pinmux.h"
#include "duet_dma.h"
#include "duet_i2c.h"

// duet_i2c_slv_callback_t g_duet_i2c_slv_callback_handler[DUET_I2C_NUM];
duet_timer_dev_t g_duet_timer1;
volatile int g_duet_i2c_timeout = 0;
duet_i2c_priv_cfg_t duet_i2c_priv_cfg = {
    .speed_mode = I2C_MODE_STANDARD,
    .fifo_mode = I2C_MST_FIFO_MODE_ENABLE,
    .dma_mode = I2C_DMA_ENABLE,
};
void I2C0_IRQHandler(void)
{
    duet_intrpt_enter();
    if ((I2C0->SR) & I2C_STATUS_TRANS_DONE) {
        //        printf("I2C0 trans done interrupt\n");
        I2C0->CR &= (~I2C_CR_SEND_STOP);
        I2C0->SR |= I2C_STATUS_TRANS_DONE;
    }
    if ((I2C0->SR) & I2C_STATUS_SLAVE_ADDR_DET) {
        printf("I2C0 slave addr det\n");
        I2C0->SR |= I2C_STATUS_SLAVE_ADDR_DET;
    }
    if ((I2C0->SR) & I2C_STATUS_RX_FIFO_FULL) {
        printf("I2C0 I2C_STATUS_RX_FIFO_FULL\n");
        I2C0->SR |= I2C_STATUS_RX_FIFO_FULL;
    }
}

void I2C1_IRQHandler(void)
{
    duet_intrpt_enter();

    if ((I2C1->SR) & I2C_STATUS_TRANS_DONE) {
        printf("I2C1 trans done interrupt\n");
        I2C1->SR |= I2C_STATUS_TRANS_DONE;
    }
    if ((I2C1->SR) & I2C_STATUS_SLAVE_ADDR_DET) {
        printf("I2C1 slave addr det\n");
        I2C1->SR |= I2C_STATUS_SLAVE_ADDR_DET;
    }
    if ((I2C1->SR) & I2C_STATUS_RX_FIFO_HALF_FULL) {
        printf("I2C1 RX_FIFO_HALF_FULL\n");
        I2C1->SR |= I2C_STATUS_RX_FIFO_HALF_FULL;
    }
}

ITstatus i2c_get_flag_status(I2C_TypeDef *I2Cx, uint32_t I2C_flag)
{
    if (I2C_flag == I2C_STATUS_TX_FIFO_EMPTY) {
        if (I2Cx->WFIFO_WPTR != I2Cx->WFIFO_RPTR) {
            return RESET;
        } else {
            return SET;
        }
    } else if (I2C_flag == I2C_STATUS_TX_FIFO_FULL) {
        if ((I2Cx->WFIFO_WPTR > I2Cx->WFIFO_RPTR) ? (I2Cx->WFIFO_WPTR - I2Cx->WFIFO_RPTR) :
             (I2Cx->WFIFO_RPTR - I2Cx->WFIFO_WPTR) != I2C_TX_FIFO_DEPTH) {
            return RESET;
        } else {
            return SET;
        }
    } else {
        if (I2Cx->SR & I2C_flag) {
            return SET;
        } else {
            return RESET;
        }
    }
}

int32_t duet_i2c_reset(I2C_TypeDef *I2Cx)
{
    /* check unit busy */
    int32_t temp = I2C_WAIT_FOREVER;
    while (i2c_get_flag_status(I2Cx, I2C_STATUS_UNIT_BUSY)) {
        temp --;
    }

    if (temp) {
        I2Cx->CR &= I2C_UNIT_RESET; // clear rest of CR
        I2Cx->CR |= I2C_UNIT_RESET;  // set RESET bit
        I2Cx->SR = 0;
        I2Cx->CR &= ~I2C_UNIT_RESET; // clear RESET bit
    } else {
        return EIO;
    }

    return 0;
}

void duet_i2c_interrupt_config(I2C_TypeDef *I2Cx, uint32_t I2C_interrupt_en, int8_t new_state)
{
    if (new_state == ENABLE) {
        I2Cx->CR |= I2C_interrupt_en;
    } else {
        I2Cx->CR &= ~I2C_interrupt_en;
    }
}

/**
 * Initialises an I2C interface
 * Prepares an I2C hardware interface for communication as a master or slave
 *
 * @param[in]  i2c  the device for which the i2c port should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred during initialisation
 */
int32_t duet_i2c_init(duet_i2c_dev_t *i2c)
{
    I2C_TypeDef *I2Cx;
    uint32_t reg_val;

    if (NULL == i2c) {
        return EIO;
    }
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        return EIO;
    }

    // pinmux config
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0, set PAD2,3 for i2c0 func 4.
#if I2C0_PAD_GROUP0_ENABLE
        duet_pinmux_config(PAD2, PF_I2C0);
        duet_pinmux_config(PAD3, PF_I2C0);
#endif
#if I2C0_PAD_GROUP1_ENABLE
        duet_pinmux_config(PAD20, PF_I2C0);
        duet_pinmux_config(PAD21, PF_I2C0);
#endif
    } else {
#if I2C1_PAD_GROUP0_ENABLE
        duet_pinmux_config(PAD8, PF_I2C1);
        duet_pinmux_config(PAD9, PF_I2C1);
#endif
#if I2C1_PAD_GROUP1_ENABLE
        duet_pinmux_config(PAD22, PF_I2C1);
        duet_pinmux_config(PAD23, PF_I2C1);
#endif
    }

    // I2C clock enable
    if (I2C_DEVICE0 == i2c->port) {
        reg_val = REG_RD(PERI_CLK_EN_REG1);
        REG_WR(PERI_CLK_EN_REG1, (reg_val | (I2C0_BUS_CLOCK_ENABLE | I2C0_PERI_CLOCK_ENABLE)));
    } else {
        reg_val = REG_RD(PERI_CLK_EN_REG1);
        REG_WR(PERI_CLK_EN_REG1, (reg_val | (I2C1_BUS_CLOCK_ENABLE | I2C1_PERI_CLOCK_ENABLE)));
    }

    // I2C IRQ enable
    if (I2C_DEVICE0 == i2c->port) {
        reg_val = REG_RD(DUTE_IRQ_EN_REG);
        REG_WR(DUTE_IRQ_EN_REG, (reg_val | I2C0_IRQ_ENABLE));
    } else {
        reg_val = REG_RD(DUTE_IRQ_EN_REG);
        REG_WR(DUTE_IRQ_EN_REG, (reg_val | I2C1_IRQ_ENABLE));
    }

    /* reset unit */
    duet_i2c_reset(I2Cx);

    switch (i2c->config.freq) {
        case I2C_STANDARD_SPEED:
            duet_i2c_priv_cfg.speed_mode = I2C_MODE_STANDARD;
            break;
        case I2C_FAST_SPEED:
            duet_i2c_priv_cfg.speed_mode = I2C_MODE_FAST;
            break;
        case I2C_HIGH_SPEED:
            duet_i2c_priv_cfg.speed_mode = I2C_MODE_HIGH_SPEED_0;
            break;
        default:
            break;
    }

    if (i2c->config.mode == I2C_MASTER) {
        I2Cx->SAR = i2c->config.dev_addr; // set unit address as slave

        I2Cx->CR &= ~I2C_MODE_SET_MASK; // reset speed mode to 0
        I2Cx->CR |= (duet_i2c_priv_cfg.speed_mode << I2C_MODE_SET_POS); // set speed mode
        I2Cx->LCR = 0;
        I2Cx->LCR = (((I2C_CLK / I2C_STANDARD_SPEED - 8) / 2) | (((I2C_CLK / I2C_FAST_SPEED - 8) / 2 - 1) << 9) | (((
                         I2C_CLK / I2C_HIGH_SPEED - 9) / 2) << 18) | (((I2C_CLK / I2C_HIGH_SPEED - 9) / 2) << 27)); // set divider
        // set wait count value to adjust clock for standart and fast mode
        I2Cx->WCR = (((I2C_CLK / I2C_FAST_SPEED - 8) / 2 - 1) / 3);
        I2Cx->CR |= duet_i2c_priv_cfg.fifo_mode;  // set FIFO mode

        I2Cx->CR |= I2C_UNIT_ENABLE | I2C_SCL_ENABLE; // scl driving enable & unit enable

        duet_i2c_interrupt_config(I2Cx, I2C_INTERRUPT_SLAVE_ADDR_DET_EN | I2C_INTERRUPT_TRANS_DONE_EN |
                                  I2C_INTERRUPT_RX_FIFO_FULL_EN | I2C_INTERRUPT_BUS_ERROR_DET_EN \
                                  | I2C_INTERRUPT_MASTER_STOP_DET_EN, ENABLE); // master
    } else {
        /* i2c as slave */
        I2Cx->SAR = i2c->config.dev_addr; // set unit address as slave
        I2Cx->CR &= ~I2C_MODE_SET_MASK; // reset speed mode to 0
        I2Cx->CR |= (duet_i2c_priv_cfg.speed_mode << I2C_MODE_SET_POS); // set speed mode
        //            I2Cx->CR |= pI2C_InitStrcut->i2c_mst_fifo_mode;  // FIFO mode is not for slave mode, so this has no effect
        I2Cx->CR |= I2C_INTERRUPT_SLAVE_ADDR_DET_EN | I2C_INTERRUPT_RX_FIFO_FULL_EN | I2C_INTERRUPT_RX_BUFER_FULL_EN |
                    I2C_INTERRUPT_SLAVE_STOP_DET_EN | I2C_INTERRUPT_TRANS_DONE_EN | I2C_INTERRUPT_TX_BUFFER_EMPTY_EN; // master read
        I2Cx->CR |= I2C_UNIT_ENABLE; // unit enable

        duet_i2c_interrupt_config(I2Cx, I2C_INTERRUPT_SLAVE_ADDR_DET_EN | I2C_INTERRUPT_RX_FIFO_FULL_EN |
                                  I2C_INTERRUPT_RX_BUFER_FULL_EN | I2C_INTERRUPT_SLAVE_STOP_DET_EN \
                                  | I2C_INTERRUPT_TRANS_DONE_EN | I2C_INTERRUPT_TX_BUFFER_EMPTY_EN, ENABLE); // slave
    }
    if (duet_i2c_priv_cfg.dma_mode == I2C_DMA_ENABLE) {
        I2Cx->CR |= I2C_DMA_ENABLE;
    }

    // I2C interrupt enable should be put before I2C enable
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        NVIC_EnableIRQ(I2C0_IRQn); // enable I2C0 interrupt
    } else {
        NVIC_EnableIRQ(I2C1_IRQn); // enable I2C1 interrupt
    }

    /* check the bus busy after unit enable */
    if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_BUSY)) {
        return EIO;
    } else {
        return 0;
    }
}

void duet_timer1_irq_handler(void *arg)
{
    g_duet_i2c_timeout = 1;
}

int32_t duet_i2c_master_send(duet_i2c_dev_t *i2c, uint16_t dev_addr, const uint8_t *data, uint16_t size,
                             uint32_t timeout)
{
    int32_t ret = 0;
    I2C_TypeDef *I2Cx;
    uint16_t temp;

    if (NULL == i2c) {
        return -EI2CNUMERR;
    }

    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        return -EI2CNUMERR;
    }

    if (0xFFFFFFFF != timeout) {
        g_duet_timer1.port = DUET_TIMER1_INDEX;
        g_duet_timer1.config.reload_mode = TIMER_RELOAD_MANU;
        g_duet_timer1.config.cb = duet_timer1_irq_handler;
        g_duet_timer1.config.arg = NULL;

        g_duet_timer1.config.period = timeout * 1000; // us

        duet_timer_init(&g_duet_timer1);
        g_duet_i2c_timeout = 0;
        duet_timer_start(&g_duet_timer1);
    }
    // set TXBEGIN bit before starting another transfer
    I2Cx->CR |= I2C_TRANS_BEGIN;

    // send slave address first
    I2Cx->WFIFO = (((uint8_t)(dev_addr & 0x00FF)) << 1) | I2C_WRITE | I2C_SEND_START | I2C_TB;

    // send write cmd
    while (1) {
        //wait till tx fifo is empty to avoid overflowing tx fifo
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY)) {
            if (g_duet_i2c_timeout) {
                g_duet_i2c_timeout = 0;
                ret = -ETIMEOUT;
                goto EXIT;
            }
        }

        if (size > I2C_TX_FIFO_DEPTH) {
            // send 8 bytes
            for (temp = 0; temp < I2C_TX_FIFO_DEPTH; temp++) {
                if (g_duet_i2c_timeout) {
                    g_duet_i2c_timeout = 0;
                    ret = -ETIMEOUT;
                    goto EXIT;
                }

                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    ret = -EBUSERR; // bus error
                    goto EXIT;
                } else {
                    I2Cx->WFIFO = (*data++) | I2C_TB;
                }
            }
            size -= I2C_TX_FIFO_DEPTH;
        } else {
            // send remaining bytes
            for (temp = 0; temp < size; temp++) {
                if (g_duet_i2c_timeout) {
                    g_duet_i2c_timeout = 0;
                    ret = -ETIMEOUT;
                    goto EXIT;
                }

                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    ret =  -EBUSERR; // bus error
                    goto EXIT;
                } else {
                    if (temp == size - 1) {
                        I2Cx->WFIFO = (*data) | I2C_SEND_STOP | I2C_TB;
                        ret = I2C_SUCCESS; // success
                        goto EXIT;
                    } else {
                        I2Cx->WFIFO = (*data++) | I2C_TB;
                    }
                }
            }
        }
    }
    ret = -ENOENT;

EXIT:
    if (0xFFFFFFFF != timeout) {
        duet_timer_finalize(&g_duet_timer1);
    }
    return ret;
}

int32_t duet_i2c_master_recv(duet_i2c_dev_t *i2c, uint16_t dev_addr, uint8_t *data, uint16_t size, uint32_t timeout)
{
    I2C_TypeDef *I2Cx;
    uint16_t i = 0;
    int32_t ret;

    if (NULL == i2c) {
        return -EI2CNUMERR;
    }

    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        return -EI2CNUMERR;
    }

    if (0xFFFFFFFF != timeout) {
        g_duet_timer1.port = DUET_TIMER1_INDEX;
        g_duet_timer1.config.reload_mode = TIMER_RELOAD_MANU;
        g_duet_timer1.config.cb = duet_timer1_irq_handler;
        g_duet_timer1.config.arg = NULL;

        g_duet_timer1.config.period = timeout * 1000; // us

        duet_timer_init(&g_duet_timer1);
        g_duet_i2c_timeout = 0;
        duet_timer_start(&g_duet_timer1);
    }

    // set TXBEGIN bit before starting another transfer
    I2Cx->CR |= I2C_TRANS_BEGIN;

    // send slave address first
    I2Cx->WFIFO = (dev_addr << 1) | I2C_READ | I2C_SEND_START | I2C_TB;
    while (i < size) {
        if (g_duet_i2c_timeout) {
            g_duet_i2c_timeout = 0;
            ret = -ETIMEOUT;
            goto EXIT;
        }
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY)) {
            if (g_duet_i2c_timeout) {
                g_duet_i2c_timeout = 0;
                ret = -ETIMEOUT;
                goto EXIT;
            }
        }
        if ((size - 1) == i) {
            I2Cx->WFIFO = I2C_SEND_STOP | I2C_SEND_NACK | I2C_TB;
        } else {
            I2Cx->WFIFO = I2C_TB;
        }

        while (!(I2Cx->RFIFO_STATUS & 0xF0)) {
            if (g_duet_i2c_timeout) {
                g_duet_i2c_timeout = 0;
                ret = -ETIMEOUT;
                goto EXIT;
            }
        }
        *(data + i) = I2Cx->RFIFO & 0xFF;
        i++;
    }

EXIT:
    if (0xFFFFFFFF != timeout) {
        duet_timer_finalize(&g_duet_timer1);
    }
    return ret;
}

int8_t duet_i2c_master_repeated_write_read(I2C_TypeDef *I2Cx, uint8_t slave_addr, const uint8_t *pwdata, uint8_t *rdata,
        uint32_t wlen, uint32_t rlen)
{
    /**** master write ****/

    uint8_t temp;
    uint8_t i = 0;

    // set TXBEGIN bit before starting another transfer
    I2Cx->CR |= I2C_TRANS_BEGIN;

    // send slave address first
    I2Cx->WFIFO = (slave_addr << 1) | I2C_WRITE | I2C_SEND_START | I2C_TB;

    // send write cmd
    while (1) {
        //wait till tx fifo is empty to avoid overflowing tx fifo
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));

        if (wlen > I2C_TX_FIFO_DEPTH) {
            // send 8 bytes
            for (temp = 0; temp < I2C_TX_FIFO_DEPTH; temp++) {
                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    return EIO;    // bus error
                } else {
                    I2Cx->WFIFO = (*pwdata++) | I2C_TB;
                }
            }
            wlen -= I2C_TX_FIFO_DEPTH;
        } else {
            // send remaining bytes
            for (temp = 0; temp < wlen; temp++) {
                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    return EIO;    // bus error
                } else {
                    if (temp == wlen - 1) {
                        I2Cx->WFIFO = (*pwdata) | I2C_TB;   // no need to send STOP for repeated read
                        break; // write completed
                    } else {
                        I2Cx->WFIFO = (*pwdata++) | I2C_TB;
                    }
                }
            }
            break;
        }
    }

    /**** master read ****/
    while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));

    // send slave address first
    I2Cx->WFIFO = (slave_addr << 1) | I2C_READ | I2C_SEND_START | I2C_TB;
    while (i < rlen) {
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));
        if ((rlen - 1) == i) {
            I2Cx->WFIFO = I2C_SEND_STOP | I2C_SEND_NACK | I2C_TB;
            while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));
        } else {
            I2Cx->WFIFO = I2C_TB;
        }

        while (!(I2Cx->RFIFO_STATUS & 0xF0));
        *(rdata + i) = I2Cx->RFIFO & 0xFF;
        i++;
    }

    return 0;
}

/* program memory of a slave device  via I2C */
int32_t duet_i2c_mem_write(duet_i2c_dev_t *i2c, uint16_t dev_addr, uint16_t mem_addr, \
                           uint16_t mem_addr_size, const uint8_t *data, uint16_t len, \
                           uint32_t timeout)
{
    I2C_TypeDef *I2Cx;
    uint8_t temp;
    int8_t i;
    int32_t ret = 0;

    if (NULL == i2c) {
        ret = -EI2CNUMERR;
        goto EXIT;
    }

    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        ret = -EI2CNUMERR;
        goto EXIT;
    }

    if (0xFFFFFFFF != timeout) {
        g_duet_timer1.port = DUET_TIMER1_INDEX;
        g_duet_timer1.config.reload_mode = TIMER_RELOAD_MANU;
        g_duet_timer1.config.cb = duet_timer1_irq_handler;
        g_duet_timer1.config.arg = NULL;

        g_duet_timer1.config.period = timeout * 1000; // us

        duet_timer_init(&g_duet_timer1);
        g_duet_i2c_timeout = 0;
        duet_timer_start(&g_duet_timer1);
    }

    // set TXBEGIN bit before starting another transfer
    I2Cx->CR |= I2C_TRANS_BEGIN;

    // send slave address first
    i2c_write_byte(I2Cx, ((uint8_t)dev_addr << 1) | I2C_WRITE | I2C_SEND_START | I2C_TB);

    // send memory address
    for (i = 0; i < mem_addr_size; i++) {
        i2c_write_byte(I2Cx, ((mem_addr >> (8 * i)) & 0xff) | I2C_WRITE | I2C_TB);
    }

    // send write cmd
    while (1) {
        if (g_duet_i2c_timeout) {
            g_duet_i2c_timeout = 0;
            ret = -ETIMEOUT;
            goto EXIT;
        }
        //wait till tx fifo is empty to avoid overflowing tx fifo
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY)) {
            if (g_duet_i2c_timeout) {
                g_duet_i2c_timeout = 0;
                ret = -ETIMEOUT;
                goto EXIT;
            }
        }

        if (len > I2C_TX_FIFO_DEPTH) {
            // send 8 bytes
            for (temp = 0; temp < I2C_TX_FIFO_DEPTH; temp++) {
                if (g_duet_i2c_timeout) {
                    g_duet_i2c_timeout = 0;
                    ret = -ETIMEOUT;
                    goto EXIT;
                }

                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    ret = -EBUSERR; // bus error
                    goto EXIT;
                } else {
                    i2c_write_byte(I2Cx, (*data++) | I2C_TB);
                }
            }
            len -= I2C_TX_FIFO_DEPTH;
        } else {
            // send remaining bytes
            for (temp = 0; temp < len; temp++) {
                if (g_duet_i2c_timeout) {
                    g_duet_i2c_timeout = 0;
                    ret = -ETIMEOUT;
                    goto EXIT;
                }

                if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
                    ret = -EBUSERR; // bus error
                    goto EXIT;
                } else {
                    if (temp == len - 1) {
                        I2Cx->WFIFO = (*data) | I2C_SEND_STOP | I2C_TB;
                        ret = I2C_SUCCESS; // success
                        goto EXIT;
                    } else {
                        i2c_write_byte(I2Cx, (*data++) | I2C_TB);
                    }
                }
            }
        }
    }

EXIT:
    if (0xFFFFFFFF != timeout) {
        duet_timer_finalize(&g_duet_timer1);
    }
    return ret;
}

int32_t duet_i2c_mem_read(duet_i2c_dev_t *i2c, uint16_t dev_addr, uint16_t mem_addr,
                          uint16_t mem_addr_size, uint8_t *data, uint16_t size,
                          uint32_t timeout)
{
    I2C_TypeDef *I2Cx;
    int16_t i;
    int32_t ret = 0;

    if (NULL == i2c) {
        return -EI2CNUMERR;
    }

    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        return -EI2CNUMERR;
    }

    if (0xFFFFFFFF != timeout) {
        g_duet_timer1.port = DUET_TIMER1_INDEX;
        g_duet_timer1.config.reload_mode = TIMER_RELOAD_MANU;
        g_duet_timer1.config.cb = duet_timer1_irq_handler;
        g_duet_timer1.config.arg = NULL;

        g_duet_timer1.config.period = timeout * 1000; // us

        duet_timer_init(&g_duet_timer1);
        g_duet_i2c_timeout = 0;
        duet_timer_start(&g_duet_timer1);
    }

    // set TXBEGIN bit before starting another transfer
    I2Cx->CR |= I2C_TRANS_BEGIN;

    // send slave address first
    i2c_write_byte(I2Cx, ((uint8_t)dev_addr << 1) | I2C_WRITE | I2C_SEND_START | I2C_TB);

    // send memory address
    for (i = 0; i < mem_addr_size; i++) {
        if (g_duet_i2c_timeout) {
            g_duet_i2c_timeout = 0;
            ret = -ETIMEOUT;
            goto EXIT;
        }
        i2c_write_byte(I2Cx, ((mem_addr >> (8 * i)) & 0xff) | I2C_WRITE | I2C_TB);
    }
    /**** master read ****/
    while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));

    // send slave address first
    i2c_write_byte(I2Cx, (dev_addr << 1) | I2C_READ | I2C_SEND_START | I2C_TB);

    i = 0;
    // send read cmd
    while (i < size) {
        //wait till tx fifo is empty to avoid overflowing tx fifo
        while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));

        if (g_duet_i2c_timeout) {
            g_duet_i2c_timeout = 0;
            ret = -ETIMEOUT;
            goto EXIT;
        }

        if (i2c_get_flag_status(I2Cx, I2C_STATUS_BUS_ERROR_DET)) {
            ret = -EBUSERR; // bus error
            goto EXIT;
        } else {
            if (i == size - 1) {
                i2c_write_byte(I2Cx, I2C_SEND_STOP | I2C_SEND_NACK | I2C_TB);
                while (!i2c_get_flag_status(I2Cx, I2C_STATUS_TX_FIFO_EMPTY));
                ret = I2C_SUCCESS;
                while (!(I2Cx->RFIFO_STATUS & 0xF0)) {
                    if (g_duet_i2c_timeout) {
                        g_duet_i2c_timeout = 0;
                        ret = -ETIMEOUT;
                        goto EXIT;
                    }
                }
                *(data + i) = I2Cx->RFIFO & 0xFF;
                goto EXIT;
            } else {
                i2c_write_byte(I2Cx, I2C_TB);
            }
        }

        while (!(I2Cx->RFIFO_STATUS & 0xF0)) {
            if (g_duet_i2c_timeout) {
                g_duet_i2c_timeout = 0;
                ret = -ETIMEOUT;
                goto EXIT;
            }
        }
        *(data + i) = I2Cx->RFIFO & 0xFF;
        i++;

    }
EXIT:
    if (0xFFFFFFFF != timeout) {
        duet_timer_finalize(&g_duet_timer1);
    }
    return ret;
}

/**
 * Deinitialises an I2C device
 *
 * @param[in]  i2c  the i2c device
 *
 * @return  0 : on success, EIO : if an error occurred during deinitialisation
 */
int32_t duet_i2c_finalize(duet_i2c_dev_t *i2c)
{
    I2C_TypeDef *I2Cx;
    uint32_t reg_val;

    if (NULL == i2c) {
        return EIO;
    }
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        I2Cx = I2C0;
    } else if (I2C_DEVICE1 == i2c->port) { // I2C_DEVICE1
        I2Cx = I2C1;
    } else {
        return EIO;
    }

    I2Cx = I2Cx;
    // disable i2c cm4 irq
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        NVIC_DisableIRQ(I2C0_IRQn); // disable I2C0 interrupt
    } else {
        NVIC_DisableIRQ(I2C1_IRQn); // disable I2C1 interrupt
    }

    // I2C IRQ disable
    if (I2C_DEVICE0 == i2c->port) {
        reg_val = REG_RD(DUTE_IRQ_DIS_REG);
        REG_WR(DUTE_IRQ_DIS_REG, (reg_val | I2C0_IRQ_DISABLE));
    } else {
        reg_val = REG_RD(DUTE_IRQ_DIS_REG);
        REG_WR(DUTE_IRQ_DIS_REG, (reg_val | I2C1_IRQ_DISABLE));
    }

    // i2c clk disable
    if (I2C_DEVICE0 == i2c->port) { // I2C_DEVICE0
        // I2C0 clock disable
        reg_val = REG_RD(PERI_CLK_DIS_REG1);
        REG_WR(PERI_CLK_DIS_REG1, (reg_val & ((I2C0_BUS_CLOCK_DISABLE | I2C0_PERI_CLOCK_DISABLE))));
    } else { // I2C_DEVICE1
        // I2C1 clock disable
        reg_val = REG_RD(PERI_CLK_DIS_REG1);
        REG_WR(PERI_CLK_DIS_REG1, (reg_val & ((I2C1_BUS_CLOCK_DISABLE | I2C1_PERI_CLOCK_DISABLE))));
    }
    // callback function pointer clear
    // g_duet_i2c_slv_callback_handler[i2c->port].tx_func = NULL;
    // g_duet_i2c_slv_callback_handler[i2c->port].rx_func = NULL;

    return 0;
}

void duet_i2c_master_dma_send(uint8_t iic_idx, uint32_t *data, uint16_t len)
{
    uint8_t dma_chan = 0;
    I2C_TypeDef *I2Cx = 0;
    if (iic_idx == 0) {
        dma_chan = 12;
        I2Cx = I2C0;
    }
    // malloc for channel descriptor
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();
    Chan_Ctl_Data_TypeDef ch_ctl_data;
    Chan_Cfg_TypeDef ch_cfg;

    ch_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC; // DMA_OP_MODE_AUTO_REQ;
    ch_ctl_data.n_minus_1 = len - 1;
    ch_ctl_data.R_pow = 0;
    ch_ctl_data.src_inc = DMA_SRC_ADDR_INC_WORD;
    ch_ctl_data.dst_inc = DMA_DST_ADDR_INC_FIX;
    ch_ctl_data.src_size = DMA_SRC_DATA_WIDTH_WORD;
    ch_ctl_data.dst_size = DMA_DST_DATA_WIDTH_WORD;

    ch_cfg.chan_ctr = ch_ctl_data;
    ch_cfg.chan_src_end_ptr = (uint32_t)(data + len - 1);
    ch_cfg.chan_dst_end_ptr = (uint32_t) & (I2Cx->WFIFO);

    (pChan_Cfg_Align + dma_chan)->chan_ctr = ch_cfg.chan_ctr;
    (pChan_Cfg_Align + dma_chan)->chan_src_end_ptr = ch_cfg.chan_src_end_ptr;
    (pChan_Cfg_Align + dma_chan)->chan_dst_end_ptr = ch_cfg.chan_dst_end_ptr;

    //    NVIC_EnableIRQ(DMA_IRQn);
    // set dma_wationreq to high for I2C single req, handshake rule 16
    // DMA_WAIT_ON_REQ |= (1<<dma_chan);
    DMA->CFG |= 0x1; // dma enable
    DMA_INT_MASK |= (1 << dma_chan); // dma interrupt unmask, write 1
    // set channel to use primary data struct only for basic/auto-request type
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);
    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;
    // set channel useburst bit to diasble sreq from generating dma request
    //        DMA->CHAN_USE_BURST_SET |= (1<<dma_chan);
    DMA->CHAN_EN_CLR |= ~(1 << dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel 6
}

void duet_i2c_master_dma_recv(uint8_t iic_idx, uint32_t *data, uint16_t len)
{
    uint8_t dma_chan = 13;  // i2c0 rx using dma channel 10

    // malloc for channel descriptor
    Chan_Cfg_TypeDef *pChan_Cfg_Align = duet_dma_ctrl_block_init();

    Chan_Ctl_Data_TypeDef ch1_ctl_data;
    Chan_Cfg_TypeDef ch1_cfg;

    ch1_ctl_data.cycle_ctl = DMA_OP_MODE_BASIC; // DMA_OP_MODE_AUTO_REQ;
    ch1_ctl_data.n_minus_1 = len - 1;
    ch1_ctl_data.R_pow = 0;
    ch1_ctl_data.src_inc = DMA_SRC_ADDR_INC_FIX;
    ch1_ctl_data.dst_inc = DMA_SRC_ADDR_INC_WORD;
    ch1_ctl_data.src_size = DMA_SRC_DATA_WIDTH_WORD;
    ch1_ctl_data.dst_size = DMA_DST_DATA_WIDTH_WORD;

    ch1_cfg.chan_ctr = ch1_ctl_data;
    ch1_cfg.chan_src_end_ptr = (uint32_t) & (I2C0->RFIFO);
    ch1_cfg.chan_dst_end_ptr = (uint32_t)(data + len - 1);

    memcpy((char *)(pChan_Cfg_Align + dma_chan), (char *)(&ch1_cfg), sizeof(Chan_Cfg_TypeDef));

    // set dma_wationreq to high for I2C single req, handshake rule 16
    //    DMA_WAIT_ON_REQ |= (1<<dma_chan);

    DMA->CFG |= 0x1; // dma enable

    // set chan1 to use primary data struct only
    DMA->CHAN_PRI_ALT_CLR |= (1 << dma_chan);

    DMA->CTL_BASE_PTR = (uint32_t)pChan_Cfg_Align;

    DMA->CHAN_EN_CLR |= ~(1 << dma_chan); // disable other channels
    DMA->CHAN_EN_SET |= (1 << dma_chan); // enable channel

    //        free(pChan_Cfg);
}
