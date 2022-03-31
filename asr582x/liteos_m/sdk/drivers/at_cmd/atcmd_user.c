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

#ifdef AT_USER_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "target_config.h"
#if (LOSCFG_USE_SHELL == 1)
#include "shell.h"
#include "shcmd.h"
#endif
#include "duet_flash_kv.h"
#include "flash.h"
#include "duet_flash.h"
#include "lega_rtos.h"
/*
 ************************************************************
 *                    USER AT CMD START
 *
 ************************************************************
 */
#ifdef CFG_HARMONY_SUPPORT
void openharmony_init()
{
    extern void  OHOS_SystemInit(void);
    OHOS_SystemInit();
}
#endif
int at_test_1(int argc, char **argv)
{
    printf("%s...\n", __func__);
    return 0;
}

int at_harmony_xts(int argc, char **argv)
{
#ifdef CFG_HARMONY_SUPPORT
    openharmony_init();
#endif
    return 0;
}

int at_hilink_start(int argc, char **argv)
{
    return 0;
}

int at_kv_clear(int argc, char **argv)
{
    lega_rtos_declare_critical();
    lega_enter_critical_expble();
    duet_flash_erase(PARTITION_PARAMETER_2, 0, KV_MAX_SIZE);
    lega_exit_critical_expble();

    printf("kv clear done\r\n");
    delay(1000);
    NVIC_SystemReset();
    return 0;
}
/*
 ************************************************************
 *                    USER AT CMD END
 *
 ************************************************************
 */

void lega_at_user_cmd_register(void)
{
#if (LOSCFG_USE_SHELL == 1)
    osCmdReg(CMD_TYPE_EX, "at_test1", 0, (CMD_CBK_FUNC)at_test_1);
    osCmdReg(CMD_TYPE_EX, "at_harmony_xts", 0, (CMD_CBK_FUNC)at_harmony_xts);
    osCmdReg(CMD_TYPE_EX, "at_hilink_start", 0, (CMD_CBK_FUNC)at_hilink_start);
    osCmdReg(CMD_TYPE_EX, "kv_clear", 0, (CMD_CBK_FUNC)at_kv_clear);
#endif
}

#endif
