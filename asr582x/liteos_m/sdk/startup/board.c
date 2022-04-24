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
#include "board.h"
#include "duet_cm4.h"
#include "duet_common.h"
#include "duet_uart.h"
#include "duet_pinmux.h"
#include "lega_at_api.h"
#include "arch.h"
#ifdef _SPI_FLASH_ENABLE_
#include "duet_flash_alg.h"
#include "duet_flash.h"
#endif
#ifdef STAR_MPU_ENABLE
#include "panic_mpu.h"
#endif
#include "lega_wlan_api.h"
#include "los_interrupt.h"
#include "target_config.h"
#include "uart.h"
#include "board.h"
#include "los_arch_interrupt.h"
#include "lega_rtos.h"

/***********************************************************
* init IRQ, set priority and enable IRQ
*
**********************************************************/
extern void intc_irq(void);
extern CRITICAL_FUNC_SEG void BLE_IRQHandler(void);
extern void CRYPTOCELL310_IRQHandler(void);
extern void D_APLL_UNLOCK_IRQHandler(void);
extern void D_SX_UNLOCK_IRQHandler(void);
extern void WDG_IRQHandler(void);
extern void SLEEP_IRQHandler(void);
extern void TIMER_IRQHandler(void);
extern void DMA_IRQHandler(void);
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void GPIO_IRQHandler(void);
void NVIC_init(void)
{
    // set irq priority, default set configLIBRARY_NORMAL_INTERRUPT_PRIORITY
    /*
        NVIC_SetPriority(UART0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(UART1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(UART2_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(CEVA_RW_IP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(D_APLL_UNLOCK_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(D_SX_UNLOCK_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(SLEEP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(WDG_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(FLASH_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(GPIO_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(TIMER_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(CRYPTOCELL310_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(DMA_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(SPI0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(SPI1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(SPI2_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(I2C0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(I2C1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(SDIO_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(PLF_WAKEUP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
        NVIC_SetPriority(RW_BLE_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY-1);
    */
    ArchHwiCreate(CEVA_RW_IP_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, intc_irq, 0);
    ArchHwiCreate(D_APLL_UNLOCK_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, D_APLL_UNLOCK_IRQHandler, 0);
    ArchHwiCreate(D_SX_UNLOCK_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, D_SX_UNLOCK_IRQHandler, 0);
    ArchHwiCreate(SLEEP_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, SLEEP_IRQHandler, 0);
    ArchHwiCreate(UART0_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, UART0_IRQHandler, 0);
    ArchHwiCreate(UART1_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, UART1_IRQHandler, 0);
    ArchHwiCreate(UART2_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, UART2_IRQHandler, 0);
    ArchHwiCreate(WDG_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, WDG_IRQHandler, 0);
    ArchHwiCreate(TIMER_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, TIMER_IRQHandler, 0);
    ArchHwiCreate(DMA_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, DMA_IRQHandler, 0);
    ArchHwiCreate(RW_BLE_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY - 1, 0, BLE_IRQHandler, 0);
    ArchHwiCreate(GPIO_IRQn, configLIBRARY_NORMAL_INTERRUPT_PRIORITY, 0, GPIO_IRQHandler, 0);
    NVIC_DisableIRQ(RW_BLE_IRQn);
}

void NVIC_deinit(void)
{
    // disable NVIC irq before kernel reloc IRQ interface
    NVIC_DisableIRQ(CEVA_RW_IP_IRQn);
    NVIC_DisableIRQ(D_APLL_UNLOCK_IRQn);
    NVIC_DisableIRQ(D_SX_UNLOCK_IRQn);
    NVIC_DisableIRQ(SLEEP_IRQn);
    NVIC_DisableIRQ(UART0_IRQn);
    NVIC_DisableIRQ(UART1_IRQn);
    NVIC_DisableIRQ(UART2_IRQn);
    NVIC_DisableIRQ(WDG_IRQn);
    NVIC_DisableIRQ(TIMER_IRQn);
    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_DisableIRQ(RW_BLE_IRQn);
}

void os_post_init_hook(void)
{
    lega_rtos_declare_critical();
    lega_rtos_enter_critical();
    *(volatile UINT32 *)OS_NVIC_CCR &= ~(UNALIGNFAULT); // allow unalign access
    lega_rtos_exit_critical();
}

// static duet_uart_dev_t uart_config_struct  = {0};
uart_dev_t uart_0;
void board_uart_init(void)
{
#if (LOSCFG_USE_SHELL == 1)
    uart_0.port = PORT_UART_STD;  /* logic port */
    uart_0.config.baud_rate = UART_BAUDRATE_115200;
    uart_0.config.data_width = DATA_8BIT;
    uart_0.config.flow_control = FLOW_CTRL_DISABLED;
    uart_0.config.parity = PARITY_NO;
    uart_0.config.stop_bits = STOP_1BIT;
    uart_0.config.mode = TX_RX_MODE;
    hal_uart_init(&uart_0);
#else
    duet_pinmux_config(PAD2, PF_UART1);
    duet_pinmux_config(PAD3, PF_UART1);

    duet_uart_struct_init(&uart_config_struct);
    uart_config_struct.port = UART1_INDEX;
    uart_config_struct.priv = at_handle_uartirq;
    // set user define config
    uart_config_struct.config.baud_rate = UART_BAUDRATE_115200;
    uart_config_struct.config.flow_control = FLOW_CTRL_DISABLED;
    // make config take effect
    duet_uart_init(&uart_config_struct);
#endif
    printf_uart_register(UART1_INDEX);
}

extern uint8_t *lega_ble_rf_get_txcali_from_efuse(uint8_t *test);
void lega_devInit()
{
    int ret = 0;
    (void)ret;
#if defined ALIOS_SUPPORT || defined HARMONYOS_SUPPORT
    ota_roll_back_pro();
#endif

    NVIC_init();

#ifdef DCDC_PFMMODE_CLOSE
    lega_drv_close_dcdc_pfm();
#endif

#ifdef WIFI_BLE_INIT_ENABLE
    lega_drv_rco_cal();
#endif

    // init uart
    board_uart_init();

#if (LOSCFG_USE_SHELL == 1)
    ret = LosShellInit();
    if (ret != LOS_OK) {
        printf("LosShellInit failed! ERROR: 0x%x\n", ret);
    }
    ret = OsShellInit();
    if (ret != LOS_OK) {
        printf("OsShellInit failed! ERROR: 0x%x\n", ret);
    }
#endif

#ifdef WIFI_BLE_INIT_ENABLE
    lega_wlan_efuse_read();
    duet_sram_rf_pta_init();
    duet_recovery_phy_fsm_config();
#endif
#ifdef CFG_MIMO_UF
    // config to support smartconfig in MIMO scenario
    // lega_wlan_smartconfig_mimo_enable();
#endif

#ifdef CFG_TMMT_DETECT
    // config to enable temperature compensation
    // lega_wlan_enable_tmmt_compensation(60);
#endif
#ifdef SECURITY_ENGINE_INIT
    lega_security_engine_init();
#endif
}
#ifdef WIFI_BLE_INIT_ENABLE
extern void lega_reset_rw_rf(void);
extern void lega_soc_wifi_ble_clk_disable(void);
extern void lega_enable_all_soc_interrupt(void);
#endif

void board_before_init(void)
{
    SCB_DisableDCache();
    SCB_DisableICache();

#ifdef _SPI_FLASH_ENABLE_
    duet_flash_init();
#endif

#ifdef STAR_MPU_ENABLE
    mpu_init();
#endif

    SCB_EnableICache();
    SCB_EnableDCache();

#ifdef WIFI_BLE_INIT_ENABLE
    lega_reset_rw_rf();
    // turn off all wifi/ble clock
    lega_soc_wifi_ble_clk_disable();
    // enable all soc interrupt
    lega_enable_all_soc_interrupt();
#endif
}

/**************************************************
*  after task run use board_sys_init to init board
**************************************************/
int board_after_init(void)
{
    lega_devInit();
#ifdef MS_RELEASE_DOMAIN
    // LOG("log_disable");
    lega_log_disable();
#endif
#ifdef WIFI_BLE_INIT_ENABLE
    tcpip_init(NULL, NULL);
    lega_wlan_init();
#endif
#if 0
    extern int init_ble_task(void);
    init_ble_task();
#endif
    return 0;
}

/**************************************************
*  before task run use board_sys_init to init board
**************************************************/
void board_init(void)
{
#ifdef _SPI_FLASH_ENABLE_
    flash_partition_init();
#endif
}
