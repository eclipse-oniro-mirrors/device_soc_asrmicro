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
#include <stdlib.h>
#include "board.h"
#include "duet_cm4.h"
#include "duet_pinmux.h"
#include "duet_uart.h"
#include "printf_uart.h"
#if 0
#include "los_config.h"
#include "los_debug.h"
#include "los_interrupt.h"
#include "los_task.h"
#include "los_tick.h"
#else
#include "cmsis_os.h"
#include "lega_at_api.h"
#endif
#include "los_exc_info.h"
#include "devmgr_service_start.h"

/*
main task stask size(byte)
*/
#define OS_MAIN_TASK_STACK (4096)

osThreadAttr_t g_main_task = {"main_task", 0, NULL, 0, NULL, OS_MAIN_TASK_STACK, 15, 0, 0};

#ifdef CFG_HARMONY_TESTS
osThreadAttr_t harmony_test_task = {"test_task", 0, NULL, 0, NULL, OS_MAIN_TASK_STACK, 15, 0, 0};
#endif

static int sys_init_done = 0;
extern void HalPendSV(void);
extern void os_post_init_hook(void);
void PendSV_Handler(void)
{
    HalPendSV();
}

static void sys_init(void)
{
    os_post_init_hook();

#ifdef MS_RELEASE_DOMAIN
    // LOG("log_disable");
    lega_log_disable();
#endif

    duet_flash_kv_init();

    board_after_init();
    // debug_memory_access_err_check(0x0, 0x10000, MPU_AP_NA_NA);

#ifdef SYSTEM_COREDUMP
    // coredump_command_register(0, NULL);
#endif

#ifdef CFG_DUAL_AP
    comm_wifi_command_register(0, NULL);
#endif

    // lega_at_init(AT_TASK_NAME,AT_TASK_PRIORITY,AT_TASK_STACK_SIZE);
    // lega_at_cmd_register_all();
    lega_at_user_cmd_register();

#ifdef BLE_APP_AT_CMD
    atcmdplus_ble_register();
#endif

#ifdef CFG_MRFOTA_TEST
    //  extern void lega_rfota_wifi_test_at_init(void);
    //  lega_rfota_wifi_test_at_init();
    //   extern void lega_rfota_ble_test_at_init(void);
    // lega_rfota_ble_test_at_init();
#endif
    DeviceManagerStart();
    printf("sys_init running...\n");
    sys_init_done = 1;
}

#ifdef CFG_HARMONY_TESTS
static void harmony_test(void)
{
    while (sys_init_done == 0) {
        osDelay(500);
    }
    osDelay(500);
    extern void  OHOS_SystemInit(void);
    OHOS_SystemInit();
}
#endif

int main(void)
{
    osStatus_t ret;
    osThreadId_t threadId;

    board_before_init();

    board_init();

    NVIC_deinit();
    ret = osKernelInitialize();

    if (ret == osOK) {
        threadId = osThreadNew((osThreadFunc_t)sys_init, NULL, &g_main_task);
#ifdef CFG_HARMONY_TESTS
        osThreadNew((osThreadFunc_t)harmony_test, NULL, &harmony_test_task);
#endif
        if (threadId != NULL) {
            osKernelStart();
        }
    }

    while (1);
    return 0;
}
