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

#ifndef _DUET_H_
#define _DUET_H_
#include <stdint.h>
#include <stdio.h>
#if 0 // use duet_cm33.h
typedef enum { RESET = 0, SET = !RESET} FlagStatus, ITstatus;
typedef enum { DISABLE = 0, ENABLE = !DISABLE } State;

typedef enum IRQn {
    /**************   Processor Exceptions Numbers ******************************************/
    NonMaskableInt_IRQn           = -14,      /* !< 2 Non Maskable Interrupt                         */
    MemoryManagement_IRQn         = -12,      /* !< 4 Cortex-M3 Memory Management Interrupt          */
    BusFault_IRQn                 = -11,      /* !< 5 Cortex-M3 Bus Fault Interrupt                  */
    UsageFault_IRQn               = -10,      /* !< 6 Cortex-M3 Usage Fault Interrupt                */
    SVCall_IRQn                   = -5,       /* !< 11 Cortex-M3 SV Call Interrupt                   */
    DebugMonitor_IRQn             = -4,       /* !< 12 Cortex-M3 Debug Monitor Interrupt             */
    PendSV_IRQn                   = -2,       /* !< 14 Cortex-M3 Pend SV Interrupt                   */
    SysTick_IRQn                  = -1,       /* !< 15 Cortex-M3 System Tick Interrupt               */
    /******   Interrupt Numbers *******************************************************/
    CEVA_RW_IP_IRQn               = 0,        /* !< CEVA RW IP Interrupt                             */
    SLEEP_IRQn                    = 1,        /* !< Sleep Wake-Up Interrupt                          */
    WDG_IRQn                      = 2,        /* !< Window WatchDog                                  */
    FLASH_IRQn                    = 3,        /* !< FLASH Interrupt                                  */
    GPIO_IRQn                     = 4,        /* !< GPIO Interrupt                                   */
    TIMER_IRQn                    = 5,        /* !< Timer Interrupt                                  */
    CRYPTOCELL310_IRQn            = 6,        /* !< CryptoCell 310 Interrupt                         */
    DMA_IRQn                      = 7,        /* !< Generic DMA Ctrl Interrupt                       */
    UART0_IRQn                    = 8,        /* !< UART0 Interrupt                                  */
    UART1_IRQn                    = 9,        /* !< UART1 Interrupt                                  */
    UART2_IRQn                    = 10,       /* !< UART2 Interrupt                                  */
    SPI0_IRQn                     = 11,       /* !< SPI0 Interrupt                                   */
    SPI1_IRQn                     = 12,       /* !< SPI1 Interrupt                                   */
    SPI2_IRQn                     = 13,       /* !< SPI2                                             */
    I2C0_IRQn                     = 14,       /* !< I2C0 Interrupt                                   */
    I2C1_IRQn                     = 15,       /* !< I2C1 Interrupt                                   */
    SDIO_IRQn                     = 16,       /* !< SDIO Combined Interrupt                          */
    D_APLL_UNLOCK_IRQn            = 17,       /* !< RF added: D_APLL_UNLOCK Interrupt                */
    D_SX_UNLOCK_IRQn              = 18,       /* !< RF added: D_SX_UNLOCK Interrupt                  */
    PLF_WAKEUP_IRQn               = 23,       /* !< WiFi Platform Wake-Up Interrupt                  */
    I2S_IRQn                      = 24,       /* !< I2S Interrupt                                    */
    RW_BLE_IRQn                   = 25,       /* !< BLE Interrupt                                    */
} IRQn_Type;
#endif
#include "duet_cm4.h"
#include "core_cm4.h"

// Macro to read a register
#define REG_RD(addr)              (*(volatile uint32_t *)(addr))
// Macro to write a register
#define REG_WR(addr, value)       (*(volatile uint32_t *)(addr)) = (value)

/*
    system control register definition
*/
typedef struct Sys_Con_Block {
    union {
        struct {
            __IO uint32_t hclk_src_sel : 2;
            __IO uint32_t pclk_div_sel : 3;
            __IO uint32_t kp_sclk_sel : 1;
            __IO uint32_t timer1_sclk_sel : 1;
            __IO uint32_t timer2_sclk_sel : 1;
            __IO uint32_t timer3_sclk_sel : 1;
            __IO uint32_t timer4_sclk_sel : 1;
            __IO uint32_t rw_mst_clk_freq_sel : 6;
            __I  uint32_t rsv0 : 1;
            __IO uint32_t pwm_pclk_sel : 1;
            __I  uint32_t rsv1 : 14;
        } BITS_CLK_SEL;
        __IO uint32_t REG_00;
    };
    __IO uint32_t REG_04;
    __IO uint32_t REG_08;
    __I  uint32_t REG_0C;
    __I  uint32_t REG_10;
    __IO uint32_t REG_14;
    __IO uint32_t REG_18;
    __IO uint32_t REG_1C;
    /* I2S clock divider */
    union {
        struct {
            __IO uint32_t i2s_ws_div : 8;
            __IO uint32_t i2s_sclk_div : 8;
            __IO uint32_t i2s_mclk_div : 8;
            __IO uint32_t RESV3 : 8;
        } BITS_REG_20;
        __IO uint32_t REG_20;
    };
    __IO uint32_t REG_24;
    __I  uint32_t REG_28;
    __IO uint32_t REG_2C;
    __IO uint32_t REG_30;
    __IO uint32_t REG_34;
    __I  uint32_t REG_38;
    __I  uint32_t REG_3C;
    __IO uint32_t REG_40;
    __IO uint32_t REG_44;
    __IO uint32_t REG_48;
    __IO uint32_t REG_4C;
    __IO uint32_t REG_50;
    __IO uint32_t REG_54;
    __IO uint32_t REG_58;
    __IO uint32_t REG_5C;
    __IO uint32_t REG_60;
    __IO uint32_t REG_64;
    union {
        struct {
            __IO uint32_t zr_gp0_sel : 4;
            __IO uint32_t zr_gp1_sel : 4;
            __IO uint32_t zr_gp2_sel : 4;
            __IO uint32_t zr_gp3_sel : 4;
            __IO uint32_t zr_gp4_sel : 4;
            __IO uint32_t zr_gp5_sel : 4;
            __IO uint32_t zr_gp6_sel : 4;
            __IO uint32_t zr_gp7_sel : 4;
        } BITS_REG_68;
        __IO uint32_t REG_68;    /* pinmux control */
    };
    union {
        struct {
            __IO uint32_t zr_gp8_sel : 4;
            __IO uint32_t zr_gp9_sel : 4;
            __IO uint32_t zr_gp10_sel : 4;
            __IO uint32_t zr_gp11_sel : 4;
            __IO uint32_t zr_gp12_sel : 4;
            __IO uint32_t zr_gp13_sel : 4;
            __IO uint32_t zr_gp14_sel : 4;
            __IO uint32_t zr_gp15_sel : 4;
        } BITS_REG_6C;
        __IO uint32_t REG_6C;    /* pinmux control */
    };
    union {
        struct {
            __IO uint32_t zr_gp16_sel : 4;
            __IO uint32_t zr_gp17_sel : 4;
            __IO uint32_t zr_gp18_sel : 4;
            __IO uint32_t zr_gp19_sel : 4;
            __IO uint32_t zr_gp20_sel : 4;
            __IO uint32_t zr_gp21_sel : 4;
            __IO uint32_t zr_gp22_sel : 4;
            __IO uint32_t zr_gp23_sel : 4;
        } BITS_REG_70;
        __IO uint32_t REG_70;    /* pinmux control */
    };
    union {
        struct {
            __IO uint32_t zr_gp24_sel : 4;
            __IO uint32_t zr_gp25_sel : 4;
            __IO uint32_t zr_gp26_sel : 4;
            __IO uint32_t zr_gp27_sel : 4;
            __IO uint32_t zr_gp28_sel : 4;
            __IO uint32_t zr_gp29_sel : 4;
            __IO uint32_t zr_gp30_sel : 4;
            __I  uint32_t rsv7 : 4;
        } BITS_REG_74;
        __IO uint32_t REG_74;    /* pinmux control */
    };

    __IO uint32_t REG_78;
    __IO uint32_t REG_7C;
    __IO uint32_t REG_80;
    __IO uint32_t REG_84;
    __IO uint32_t REG_88;
    __IO uint32_t REG_8C;
    __IO uint32_t REG_90;    /* IR control */
    __IO uint32_t REG_94;
    __IO uint32_t REG_98;
    __IO uint32_t REG_9C;
    __IO uint32_t REG_A0;
    __IO uint32_t REG_A4;
    __IO uint32_t REG_A8;
    __I  uint32_t REG_AC;
    __IO uint32_t REG_B0;
    __IO uint32_t REG_B4;
    __IO uint32_t REG_B8;
    __IO uint32_t REG_BC;
    __IO uint32_t REG_C0;
} SYS_CON_BLOCK;

#define SYS_CON_REG_BASE   (0x40000000)
#define SYS_CON            ((SYS_CON_BLOCK *)(SYS_CON_REG_BASE))

#define PINMUX_REG_BASE    (SYS_CON_REG_BASE + 0x04)
#define PAD_PULL_CTRL_REG_BASE    (SYS_CON_REG_BASE + 0x54)

#define PERI_CG_REG_BASE    (SYS_CON_REG_BASE + 0x14)
#define PERI_SOFT_RESET_REG_BASE    (SYS_CON_REG_BASE + 0x18)
#define REG_PERI_CG_EN    (*((volatile uint32_t *)(PERI_CG_REG_BASE)))
#define REG_PERI_SOFT_RESET_REG    (*((volatile uint32_t *)(PERI_SOFT_RESET_REG_BASE)))

#define SYSCTRL_BASE SYS_CON_REG_BASE
#define SYS_CON_00    *(volatile int unsigned *)(SYSCTRL_BASE + (0x000*4))
#define RTC_COUNT_1S    *(volatile int unsigned *)(SYSCTRL_BASE + (0x023*4))

#define APBCLK_DIV_0   (0)
#define APBCLK_DIV_2   (0x00000001<<2)
#define APBCLK_DIV_3   (0x00000002<<2)
#define APBCLK_DIV_4   (0x00000003<<2)
#define APBCLK_DIV_CFG APBCLK_DIV_0

#define APB_CLK_DIV_REG        (0X40000808)
#define PERI_CLK_EN_REG0       (0X40000840)
#define PERI_CLK_EN_REG1       (0X40000844)
#define PERI_CLK_DIS_REG0      (0X40000848)
#define PERI_CLK_DIS_REG1      (0X4000084C)
#define DUTE_IRQ_EN_REG        (0X40000944)
#define DUTE_IRQ_DIS_REG       (0X40000948)
#define DMA_BUFFER_REG         (0X4000C000)

/* pinmux definitions */
enum PINMUX_FUNC {
    GPIO_FUNC,
    UART_FUNC,
    SPI_FUNC,
    I2C_I2S_FUNC,
    PWM_FUNC,
    AXIS_FUNC,
    KEYPAD_FUNC,
    DEBUG_FUNC
};

/*
  DUET DMA definitions
*/
#define DMA_REG_BASE   (0x40004000)

typedef struct __DMACR {
    __I uint32_t STAT;
    __O uint32_t CFG;
    __IO uint32_t CTL_BASE_PTR;
    __I uint32_t ALT_CTL_BASE_PTR;
    __I uint32_t WAIT_ON_REQ_STAT;
    __O uint32_t CHAN_SW_REQ;
    __IO uint32_t CHAN_USE_BURST_SET;
    __O uint32_t CHAN_USE_BURST_CLR;
    __IO uint32_t CHAN_REQ_MASK_SET;
    __O uint32_t CHAN_REQ_MASK_CLR;
    __IO uint32_t CHAN_EN_SET;
    __O uint32_t CHAN_EN_CLR;
    __IO uint32_t CHAN_PRI_ALT_SET;
    __O uint32_t CHAN_PRI_ALT_CLR;
    __IO uint32_t CHAN_PRIORITY_SET;
    __O uint32_t CHNA_PRIORITY_CLR;  /* 0x3c */
    __I uint32_t RESV[3];
    __O uint32_t ERR_CLR;       /* 0x4c */
    __I uint32_t RESV1[0x3ec];           /* 0x50 - 0xffc */
} DMA_TypeDef;

#define DMA                   ((DMA_TypeDef *)DMA_REG_BASE)
#define DMA_ERROR             (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x70))
#define DMA_INT_STAT          (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x74))
#define DMA_RAW_INT_STAT      (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x78))
#define DMA_WAIT_ON_REQ       (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x7C))
#define DMA_REQ_DONE_EN       (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x80))
#define DMA_STALL             (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x84))
#define DMA_INT_CLR           (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x88))
#define DMA_INT_MASK          (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x8C))
#define DMA_HANDSHAKE_CFG0    (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x90))
#define DMA_HANDSHAKE_CFG1    (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0x94))

#define DMA_IRQ_BIT            (0X01<<7)
#define DMA_CLK_EN             (0X01<<13)

/*
   DUET I2S definition
*/
typedef struct __I2S {
    /* channel-common registers */
    __IO uint32_t IER;   // 0x00
    __IO uint32_t IRER;  // 0x04
    __IO uint32_t ITER;  // 0x08
    __IO uint32_t CER;   // 0x0c
    __IO uint32_t CCR;   // 0x10
    __O  uint32_t RXFFR; // 0x14
    __O  uint32_t TXFFR; // 0x18
    __I  uint32_t RESV0; // 0x1c

    /* channel-specific registers */
    __IO uint32_t LRBR_LTHR;  // 0x20
    __IO uint32_t RRBR_RTHR;  // 0x24
    __IO uint32_t RER;        // 0x28
    __IO uint32_t TER;        // 0x2c
    __IO uint32_t RCR;        // 0x30
    __IO uint32_t TCR;        // 0x34
    __I  uint32_t ISR;        // 0x38
    __IO uint32_t IMR;        // 0x3c
    __I  uint32_t ROR;        // 0x40
    __I  uint32_t TOR;        // 0x44
    __IO uint32_t RFCR;       // 0x48
    __IO uint32_t TFCR;       // 0x4c
    __O  uint32_t RFF;        // 0x50
    __O  uint32_t TFF;        // 0x54
    __I  uint32_t RESV1[0x5a];
    __IO uint32_t RXDMA;      // 0x1c0
    __O  uint32_t RRXDMA;
    __IO uint32_t TXDMA;
    __O  uint32_t RTXDMA;     // 0x1cc
    __I  uint32_t RESV2[8];
    __I  uint32_t I2S_COMP_PARAM_2; // 0x1f0
    __I  uint32_t I2S_COMP_PARAM_1;
    __I  uint32_t I2S_COMP_VERSION;
    __I  uint32_t I2S_COMP_TYPE;
} I2S_TypeDef;

#define REG_I2S_HW_SHFIT_SET       (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0xC0))
#define REG_I2S_HW_SHIFT_DATAIN    (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0xC4))
#define REG_I2S_HW_SHIFT_DATAOUT   (*(volatile uint32_t *)(SYS_CON_REG_BASE + 0xC8))

#define I2S_BASE    (0x4008D000)
#define I2S         ((I2S_TypeDef *)I2S_BASE)

/* I2S clock divider */
typedef struct {
    __IO uint32_t i2s_lrclk_divider : 9;
    __IO uint32_t i2s_lrclk_delay_sel : 1;
    __IO uint32_t i2s_sclk_divider : 10;
    __IO uint32_t i2s_mclk_divider : 10;
    __IO uint32_t i2s_slave_mode : 1;
    __IO uint32_t reserved : 1;
} I2S_CLK_DIV_TypeDef;

#define I2S_CLK_DIV         ((I2S_CLK_DIV_TypeDef *)0x40000810)

/*
    DUET SPI definitions
*/

/* SPI register block */
typedef struct __SPI {
    __IO  uint32_t   CR0   ; /* 0x0  */
    __IO  uint32_t   CR1   ; /* 0x4  */
    __IO  uint32_t   DR    ; /* 0x8  */
    __I   uint32_t   SR    ; /* 0xC  */
    __IO  uint32_t   CPSR  ; /* 0x10 */
    __IO  uint32_t   IMSC  ; /* 0x14 */
    __I   uint32_t   RIS   ; /* 0x18 */
    __I   uint32_t   MIS   ; /* 0x1C */
    __IO  uint32_t   ICR   ; /* 0x20 */
    __IO  uint32_t DMA_CR  ; /* 0x24 */
} SPI_TypeDef ;

#define SPI_NUM_PORTS 3
#define SPI0_BASE (0x40080000+0x4000)
#define SPI1_BASE (0x40080000+0x5000)
#define SPI2_BASE (0x40080000+0x6000)

#define SPI0    ((SPI_TypeDef *)SPI0_BASE)
#define SPI1    ((SPI_TypeDef *)SPI1_BASE)
#define SPI2    ((SPI_TypeDef *)SPI2_BASE)

#define SPI0_BUS_CLK_EN        (0X01<<4)
#define SPI0_PERI_CLK_EN       (0X01<<17)
#define SPI1_BUS_CLK_EN        (0X01<<5)
#define SPI1_PERI_CLK_EN       (0X01<<18)
#define SPI2_BUS_CLK_EN        (0X01<<6)
#define SPI2_PERI_CLK_EN       (0X01<<19)

#define SPI0_IRQ_BIT           (0X01<<11)
#define SPI1_IRQ_BIT           (0X01<<12)
#define SPI2_IRQ_BIT           (0X01<<13)

/*
    SPI definition end
*/

/*
    DUET KEYPAD definitions
*/
typedef struct __KEYPAD {
    __IO  uint32_t CR;               /* 0x00 */
    __I   uint32_t RESV0[7];          /* 0x04-0x1C */
    __I   uint32_t SR;                /* 0x20 */
    __I   uint32_t RESV1;
    __I   uint32_t MULTI_KEY_COL01;   /* 0x28 */
    __I   uint32_t RESV2;
    __I   uint32_t MULTI_KEY_COL23;   /* 0x30 */
    __I   uint32_t RESV3;
    __I   uint32_t MULTI_KEY_COL45;   /* 0x38 */
    __I   uint32_t RESV4;
    __I   uint32_t MULTI_KEY_COL67;   /* 0x40 */
    __I   uint32_t RESV5;
    __IO  uint32_t DEBOUNCE_TIME;     /* 0x48 */
    __I   uint32_t RESV6;
    __IO  uint32_t ROW_MASK;          /* 0x50 */
} KEYPAD_TypeDef;

#define KEYPAD_BASE  (0x40015000)
#define KEYPAD       ((KEYPAD_TypeDef *)KEYPAD_BASE)
/*
    DUET GPIO definitons
*/
typedef struct __GPIO {
    __IO uint32_t DATA;
    __IO uint32_t DATAOUT;
    __I  uint32_t DUMMY0[2];
    __IO uint32_t OUTENSET;
    __IO uint32_t OUTENCLR;
    __I  uint32_t DUMMY1[2];
    __IO uint32_t INTENSET;
    __IO uint32_t INTENCLR;
    __IO uint32_t INTTYPESET;
    __IO uint32_t INTTYPECLR;
    __IO uint32_t INTPOLSET;
    __IO uint32_t INTPOLCLR;
    __IO uint32_t INTSTATUS;
} GPIO_TypeDef;

#define GPIO_GROUP0_REG_BASE 0x40001000
#define GPIO_GROUP1_REG_BASE 0x40002000
#define GPIO_GROUP0 ((GPIO_TypeDef *)(GPIO_GROUP0_REG_BASE))
#define GPIO_GROUP1 ((GPIO_TypeDef *)(GPIO_GROUP1_REG_BASE))

#define PAD_PE_REG      0x40000014
#define PAD_PS_REG      0x40000018
#define PAD_IS_REG      0x4000001C
#define HW_CTRL_PE_PS   0x40000020

/*
    DUET GPIO END
*/

/*
    DUET I2C BEGIN
*/
typedef struct  __I2C {
    __IO uint32_t  CR; // 0x00
    __IO uint32_t  SR;
    __IO uint32_t  SAR; // 0x08
    __IO uint32_t  DBR;
    __IO uint32_t  LCR;
    __IO uint32_t  WCR;  // 0x14
    __IO uint32_t  RST_CYCL; // 0x18
    __I  uint32_t  BMR;  // 0x1c
    __IO uint32_t  WFIFO; // 0x20
    __IO uint32_t  WFIFO_WPTR; // 0x24
    __IO uint32_t  WFIFO_RPTR; // 0x28
    __IO uint32_t  RFIFO; // 0x2c
    __IO uint32_t  RFIFO_WPTR;
    __IO uint32_t  RFIFO_RPTR;  // 0x34
    __IO uint32_t  RESV[2];       // 0x38 0x3C
    __I uint32_t   WFIFO_STATUS;  // 0x40
    __I uint32_t   RFIFO_STATUS;  // 0x44
} I2C_TypeDef;

#define I2C0_BASE                   0x4008A000
#define I2C1_BASE                   0x4008B000
#define I2C0                       ((I2C_TypeDef *)I2C0_BASE)
#define I2C1                       ((I2C_TypeDef *)I2C1_BASE)

#define I2C0_BUS_CLOCK_BIT          7
#define I2C1_BUS_CLOCK_BIT          8
#define I2C0_PERI_CLOCK_BIT         20
#define I2C1_PERI_CLOCK_BIT         21

#define I2C0_BUS_CLOCK_ENABLE       (1 << I2C0_BUS_CLOCK_BIT)
#define I2C1_BUS_CLOCK_ENABLE       (1 << I2C1_BUS_CLOCK_BIT)
#define I2C0_PERI_CLOCK_ENABLE       (1 << I2C0_PERI_CLOCK_BIT)
#define I2C1_PERI_CLOCK_ENABLE       (1 << I2C1_PERI_CLOCK_BIT)

#define I2C0_BUS_CLOCK_DISABLE       (1 << I2C0_BUS_CLOCK_BIT)
#define I2C1_BUS_CLOCK_DISABLE       (1 << I2C1_BUS_CLOCK_BIT)
#define I2C0_PERI_CLOCK_DISABLE       (1 << I2C0_PERI_CLOCK_BIT)
#define I2C1_PERI_CLOCK_DISABLE       (1 << I2C1_PERI_CLOCK_BIT)

#define I2C0_IRQ_BIT                14
#define I2C1_IRQ_BIT                15

#define I2C0_IRQ_ENABLE             (1 << I2C0_IRQ_BIT)
#define I2C1_IRQ_ENABLE             (1 << I2C1_IRQ_BIT)

#define I2C0_IRQ_DISABLE             (1 << I2C0_IRQ_BIT)
#define I2C1_IRQ_DISABLE             (1 << I2C1_IRQ_BIT)
/// set pad2:3 for i2c0 func 4
// #define I2C0_PINMUX_MASK0   0x0000FF00
// #define I2C0_PINMUX_VALUE0  0x00004400
/// set pad20:21 for i2c0 scl/sda func 1
// #define I2C0_PINMUX_MASK1   0x00FF0000
// #define I2C0_PINMUX_VALUE1  0x00110000
/// set pad 8:9 for i2c1 func 3
// #define I2C1_PINMUX_MASK0   0x000000FF
// #define I2C1_PINMUX_VALUE0  0x00000033
/// set pad22:23 for i2c1 scl/sda func 1
// #define I2C1_PINMUX_MASK1   0xFF000000
// #define I2C1_PINMUX_VALUE1  0x11000000

/*
    DUET I2C END
*/

/*
    DUET PWM BEGIN
*/
//////////////////// to do////////////////////////////
/*
    DUET PWM END
*/

/*
    DUET RTC BEGIN
*/
typedef struct __RTC {
    __IO uint32_t CTRL;
    __IO uint32_t CNT_TICK;
    __IO uint32_t CNT_DATE;
    __I  uint32_t DUMMY[3];
    __I  uint32_t CURRENT_TICK;
    __I  uint32_t CURRENT_DATE;
} RTC_TypeDef;

#define RTC_REG_BASE 0x40000A20
#define RTC ((RTC_TypeDef *)(RTC_REG_BASE))

#define COUNT_IN_SECOND 32768

#define RTC_IRQ_BIT   0

#define RTC_CNT_CYCLE_ENABLE    (1 << 15)
#define RTC_ENABLE              (1 << 14)
#define CLK32K_SEL              (1 << 12) // A0V1 bug, sel ext XO no function, same as internal RC clk
#define RTC_INT_ENABLE          (1 << 11)
#define SW_OVERRIDE_REG_POR     (1 << 4)

#define RTC_TICK_CNT            32768 // 1s for asic

#define GET_RTC_CURRENT_DATE()  (RTC->CURRENT_DATE)

#define RTC_INIT_YEAR           118 // 2018
#define RTC_INIT_MONTH          8 // month 9
#define RTC_INIT_DATE           12
#define RTC_INIT_HOUR           8
#define RTC_INIT_MINUTE         0
#define RTC_INIT_SECOND         0
#define RTC_INIT_WEEKDAY        0 // auto modify

#define RTC_REFRESH_DAY         255 // 255 max unit: day
#define RTC_REFRESH_HOUR        23 // 0 - 23
#define RTC_REFRESH_MINUTE      59 // 0 - 59
#define RTC_REFRESH_SECOND      59 // 0 - 59

#define RTC_MAX_DAY             256
#define SECOND_PER_DAY          (24*3600)

#define DUET_IRQ_STS_REG_ADDR           0x40000AA0

// retention ram addr need to be planned
#define RTC_TIME_RETENTION_RAM_ADDR     0x40008000
#define RTC_FLAG_RETENTION_RAM_ADDR     0x40008008

#define RTC_BUS_CLK_BIT         (0X01<<6)
#define SLEEP_IRQ_BIT           (0X01<<1)
/*
    DUET RTC END
*/

/*
    RETENTION SRAM BEGIN
*/
struct DUET_RETENTION_SRAM {
    __IO uint8_t RTC_DATE[16];
    __IO uint32_t BOOT_CFG;
    __IO uint32_t BOOT_TYPE;
    uint8_t RSVD1[40];
};

#define RETENTION_RAM_ADDR                0x40008000
#define RETENTION_SRAM                    ((struct DUET_RETENTION_SRAM *)(RETENTION_RAM_ADDR))
#define DUET_RETENTION_SRAM_CUSTOM_SIZE   64
// #define RTC_TIME_RETENTION_RAM_ADDR       RETENTION_RAM_ADDR

/*
    RETENTION SRAM END
*/

/*
    DUET TIMER BEGIN
*/

//////////// to do ////////////
/*
    DUET TIMER END
*/

/*
    DUET UART BEGIN
*/

/* DUET UART register block */
typedef struct __UART {
    __IO      uint32_t  DR              ; /* 0x0  */
    __IO      uint32_t  RSC_ECR     ; /* 0x4  */
    __I       uint32_t  RSV0[4]     ; /* 0x8~0x14  */
    __I       uint32_t  FR          ; /* 0x18 */
    __I       uint32_t  RSV1        ; /* 0x1C */
    __IO       uint32_t  ILPR        ; /* 0x20 */
    __IO      uint32_t  IBRD        ; /* 0x24 */
    __IO      uint32_t  FBRD        ; /* 0x28 */
    __IO      uint32_t  LCR_H       ; /* 0x2C */
    __IO      uint32_t  CR          ; /* 0x30 */
    __IO      uint32_t  IFLS        ; /* 0x34 */
    __IO      uint32_t  IMSC        ; /* 0x38 */
    __I       uint32_t  RIS         ; /* 0x3C */
    __I       uint32_t  MIS         ; /* 0x40 */
    __O       uint32_t  ICR         ; /* 0x44 */
    __IO      uint32_t  DMACR       ; /* 0x48 */
    __I       uint32_t  RSV2[997]   ; /* 0x04C~0xFDC */
    __I       uint32_t  ID[8]         ; /* 0xFE0~0xFFC */
} UART_TypeDef;

#define UART0_BASE (0x40080000+0x1000)
#define UART1_BASE (0x40080000+0x2000)
#define UART2_BASE (0x40080000+0x3000)

#define UART0     ((UART_TypeDef *)UART0_BASE)
#define UART1     ((UART_TypeDef *)UART1_BASE)
#define UART2     ((UART_TypeDef *)UART2_BASE)

#define UART0_BUS_CLK_EN       (0X01<<1)
#define UART0_PERI_CLK_EN      (0X01<<14)
#define UART1_BUS_CLK_EN       (0X01<<2)
#define UART1_PERI_CLK_EN      (0X01<<15)
#define UART2_BUS_CLK_EN       (0X01<<3)
#define UART2_PERI_CLK_EN      (0X01<<16)

#define UART0_IRQ_BIT           (0X01<<8)
#define UART1_IRQ_BIT           (0X01<<9)
#define UART2_IRQ_BIT           (0X01<<10)

/*
    DUET UART END
*/

/*
    DUET WDG BEGIN
*/
struct DUET_WDOG {
    __IO uint32_t LOAD;
    __I  uint32_t VALUE;
    __IO uint32_t CONTROL;
    __O  uint32_t INTCLR;
    __I  uint32_t RIS;
    __I  uint32_t MIS; // 0x14
    __I  uint32_t DUMMY0[0x2FA];
    __IO uint32_t LOCK; // 0xC00
    __I  uint32_t DUMMY1[0xBF];
    __IO uint32_t ITCR; // 0xF00
    __O  uint32_t ITOP; // 0xF04
    __I  uint32_t DUMMY2[0x32];
    __I  uint32_t PERIPHID4; // 0xFD0
    __I  uint32_t PERIPHID5;
    __I  uint32_t PERIPHID6;
    __I  uint32_t PERIPHID7;
    __I  uint32_t PERIPHID0;
    __I  uint32_t PERIPHID1;
    __I  uint32_t PERIPHID2;
    __I  uint32_t PERIPHID3;
    __I  uint32_t PCELLID0;
    __I  uint32_t PCELLID1;
    __I  uint32_t PCELLID2;
    __I  uint32_t PCELLID3;
};
#define WDOG_BASE       0x40080000
#define WATCHDOG ((struct DUET_WDOG *)(WDOG_BASE))

#define WDG_LOCK_TOKEN 0x1ACCE551
#define WDG_RESEN (1 << 1)
#define WDG_INTEN 1
#define WDG_BUS_CLK_BIT       (0X01<<9)
#define WDG_IRQ_BIT           (0X01<<2)
/*
    DUET WDG END
*/

/*
    DUET TIMER BEGIN
*/
struct DUET_TIMER {
    __IO uint32_t LOAD;
    __I  uint32_t VALUE;
    __IO uint32_t CONTROL;
    __O  uint32_t INTCLR;
    __I  uint32_t RIS;
    __I  uint32_t MIS;
    __IO uint32_t BGLOAD;
};

#define TIMER1_REG_BASE          0x40087000
#define TIMER2_REG_BASE          0x40087020
#define TIMER1 ((struct DUET_TIMER *)(TIMER1_REG_BASE))
#define TIMER2 ((struct DUET_TIMER *)(TIMER2_REG_BASE))

#define COUNTER_16BIT 0
#define COUNTER_32BIT (1 << 1)
#define TIMER_SIZE COUNTER_32BIT

#define CLOCK_DIV_1 0
#define CLOCK_DIV_16 (1 << 2)
#define CLOCK_DIV_256 (2 << 2)
#define TIMER_PRESCALE CLOCK_DIV_1

#define FREE_RUNNING_MODE 0
#define PERIODIC_MODE (1 << 6)
#define ONE_SHOT_MODE 1

#define INTERRUPT_EN (1 << 5)
#define INTERRUPT_DIS 0

#define TIMER_ENABLE (1 << 7)
#define TIMER_DISABLE 0

#define TIMER_BUS_CLK_BIT       (0X01<<10)
#define TIMER_IRQ_BIT           (0X01<<5)
/*
    DUET TIMER END
*/

/*
    DUET PWM BEGIN
*/
struct DUET_PWM {
    __IO uint32_t PWMCFG;
    __IO uint32_t PWMINVERTTRIG;
    __IO uint32_t PWM01TRIG;
    __IO uint32_t PWM23TRIG;
    __IO uint32_t PWM45TRIG;
    __IO uint32_t PWMINTEN1;
    __IO uint32_t PWMINTEN2;
    __I  uint32_t PWMRIS1;
    __I  uint32_t PWMRIS2;
    __IO uint32_t PWMRIC1;

    __IO uint32_t PWMRIC2;
    __I  uint32_t PWMIS1;
    __I  uint32_t PWMIS2;
    __IO uint32_t PWMCTL;
    __IO uint32_t PWM01LOAD;
    __IO uint32_t PWM23LOAD;
    __IO uint32_t PWM45LOAD;
    __I  uint32_t PWM01COUNT;
    __I  uint32_t PWM23COUNT;
    __I  uint32_t PWM45COUNT;

    __IO uint32_t PWM0CMP;
    __IO uint32_t PWM1CMP;
    __IO uint32_t PWM2CMP;
    __IO uint32_t PWM3CMP;
    __IO uint32_t PWM4CMP;
    __IO uint32_t PWM5CMP;
    __IO uint32_t PWM01DB;
    __IO uint32_t PWM23DB;
    __IO uint32_t PWM45DB;
    __IO uint32_t CAPCTL;

    __IO uint32_t CAPINTEN;
    __I  uint32_t CAPRIS;
    __IO uint32_t CAPIC;
    __I  uint32_t CAPIS;
    __I  uint32_t CAP01T;
    __I  uint32_t CAP23T;
    __I  uint32_t CAP45T;
    __IO uint32_t CAP01MATCH;
    __IO uint32_t CAP23MATCH;
    __IO uint32_t CAP45MATCH;

    __IO uint32_t TIMINTEN;
    __I  uint32_t TIMRIS;
    __IO uint32_t TIMIC;
    __I  uint32_t TIMIS;
    __IO uint32_t TIM01LOAD;
    __IO uint32_t TIM23LOAD;
    __IO uint32_t TIM45LOAD;
    __IO uint32_t TIM01COUNT;
    __IO uint32_t TIM23COUNT;
    __IO uint32_t TIM45COUNT;
};
#define PWM_REG_BASE 0x40088000
#define PWM ((struct DUET_PWM *)(PWM_REG_BASE))

#define COUNT_UP_MODE 0
#define COUNT_UP_DOWN_MODE 1
#define PWM_COUNT_MODE COUNT_UP_MODE

#define CNT_CLK_DIV_EN (0x00000001 << 27)
#define CNT_CLK_DIV_DIS 0
#define CLK_DIV_2 (0)
#define CLK_DIV_4 (0x00000001 << 24)
#define CLK_DIV_8 (0x00000002 << 24)
#define CLK_DIV_16 (0x00000003 << 24)
#define CLK_DIV_32 (0x00000004 << 24)
#define CLK_DIV_64 (0x00000005 << 24)
#define CLK_DIV_128 (0x00000006 << 24)
#define CLK_DIV_CFG CLK_DIV_8

#define PWM0_INVERT_EN (0x00000001 << 0)
#define PWM1_INVERT_EN (0x00000001 << 1)
#define PWM2_INVERT_EN (0x00000001 << 2)
#define PWM3_INVERT_EN (0x00000001 << 3)
#define PWM4_INVERT_EN (0x00000001 << 4)
#define PWM5_INVERT_EN (0x00000001 << 5)
#define PWM6_INVERT_EN (0x00000001 << 6)
#define PWM7_INVERT_EN (0x00000001 << 7)
#define PWMX_INVERT_EN 0

#define PWM_BUS_CLK_BIT         (0X01<<11)

/*
    DUET PWM END
*/

/*
    DUET EFUDE BEGIN
*/
struct DUET_EFUSE {
    __IO uint32_t CFG_TYPE; // 0x00
    __IO uint32_t WR_TYPE;
    __IO uint32_t START;
    __IO uint32_t RD_CNT;
    __IO uint32_t WR_CNT; // 0x10
    __IO uint32_t DIV_CNT;
    __IO uint32_t B_ADDR;
    __IO uint32_t PGM_DATA;
    __IO uint32_t RDBK_DATA; // 0x20
    __I  uint32_t RSVD;
    __IO uint32_t INT_EN;
    __IO uint32_t INT_CLR;
    __IO uint32_t E_ENABLE; // 0x30
};

#define EFUSE_CTRL_BASE              0x40005000
#define EFUSE ((struct DUET_EFUSE *)(EFUSE_CTRL_BASE))

#define EFUSE_CTRL_BASE              0x40005000
#define EFUSE_REG_CFG_TYPE          *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x000))
#define EFUSE_REG_WR_TYPE           *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x004))
#define EFUSE_REG_START             *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x008))
#define EFUSE_REG_RD_CNT            *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x00C))
#define EFUSE_REG_WR_CNT            *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x010))
#define EFUSE_REG_DIV_CNT           *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x014))
#define EFUSE_REG_B_ADDR            *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x018))
#define EFUSE_REG_PGM_DATA          *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x01C))
#define EFUSE_REG_RDBK_DATA         *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x020))
// #define EFUSE_REG_DONE               *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x024))
#define EFUSE_REG_INT_EN            *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x028))
#define EFUSE_REG_INT_CLR           *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x02C))
#define EFUSE_REG_ENABLE            *((volatile uint32_t *)(EFUSE_CTRL_BASE + 0x030))

#define PSRAM_REG_BASE    0x4000A000
#define PSRAM_MCR        (PSRAM_REG_BASE + 0x000)
#define PSRAM_IPCR       (PSRAM_REG_BASE + 0x008)
#define PSRAM_FLSHCR     (PSRAM_REG_BASE + 0x00C)
#define PSRAM_BUF0CR     (PSRAM_REG_BASE + 0x010)
#define PSRAM_BUF1CR     (PSRAM_REG_BASE + 0x014)
#define PSRAM_BUF2CR     (PSRAM_REG_BASE + 0x018)
#define PSRAM_BUF3CR     (PSRAM_REG_BASE + 0x01C)
#define PSRAM_BFGENCR    (PSRAM_REG_BASE + 0x020)
#define PSRAM_SOCCR      (PSRAM_REG_BASE + 0x024)
#define PSRAM_BUF0IND    (PSRAM_REG_BASE + 0x030)
#define PSRAM_BUF1IND    (PSRAM_REG_BASE + 0x034)
#define PSRAM_BUF2IND    (PSRAM_REG_BASE + 0x038)
#define PSRAM_SFAR       (PSRAM_REG_BASE + 0x100)
#define PSRAM_SMPR       (PSRAM_REG_BASE + 0x108)
#define PSRAM_RBSR       (PSRAM_REG_BASE + 0x10C)
#define PSRAM_RBCT       (PSRAM_REG_BASE + 0x110)
#define PSRAM_TBSR       (PSRAM_REG_BASE + 0x150)
#define PSRAM_TBDR       (PSRAM_REG_BASE + 0x154)
#define PSRAM_TBCT       (PSRAM_REG_BASE + 0x158)
#define PSRAM_SR         (PSRAM_REG_BASE + 0x15C)
#define PSRAM_FR         (PSRAM_REG_BASE + 0x160)
#define PSRAM_RSER       (PSRAM_REG_BASE + 0x164)
#define PSRAM_SPNDST     (PSRAM_REG_BASE + 0x168)
#define PSRAM_SPTRCLR    (PSRAM_REG_BASE + 0x16C)
#define PSRAM_SFA1AD     (PSRAM_REG_BASE + 0x180)
#define PSRAM_SFA2AD     (PSRAM_REG_BASE + 0x184)
#define PSRAM_SFB1AD     (PSRAM_REG_BASE + 0x188)
#define PSRAM_SFB2AD     (PSRAM_REG_BASE + 0x18C)
#define PSRAM_DLPV       (PSRAM_REG_BASE + 0x190)
#define PSRAM_RBDR0      (PSRAM_REG_BASE + 0x200)
#define PSRAM_RBDR1      (PSRAM_REG_BASE + 0x204)
#define PSRAM_RBDR2      (PSRAM_REG_BASE + 0x208)
#define PSRAM_LUTKEY     (PSRAM_REG_BASE + 0x300)
#define PSRAM_LCKCR      (PSRAM_REG_BASE + 0x304)
#define PSRAM_LUT0       (PSRAM_REG_BASE + 0x310)
#define PSRAM_LUT1       (PSRAM_REG_BASE + 0x314)
#define PSRAM_LUT2       (PSRAM_REG_BASE + 0x318)
#define PSRAM_LUT3       (PSRAM_REG_BASE + 0x31C)

/*
    DUET EFUDE END
*/

/*
    DUET LPUART BEGIN
*/
////////////////////// to do /////////////////
/*
    DUET LPUART END
*/
#endif // _DUET_H_
