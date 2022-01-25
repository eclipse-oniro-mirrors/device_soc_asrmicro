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

#include "duet_cm4.h"
#include "core_cm4.h"

uint32_t system_bus_clk  = SYSTEM_BUS_CLOCK_INIT;
uint32_t system_core_clk = SYSTEM_CORE_CLOCK_INIT;

void duet_system_reset(void)
{
    //disable irq when reboot
    __disable_irq();

#ifdef HIGHFREQ_MCU160_SUPPORT
    if(system_core_clk == SYSTEM_CORE_CLOCK_HIGH)
        duet_clk_sel_low();
#endif

    NVIC_SystemReset();
}

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
}


void SystemCoreClockUpdate(void)
{

}




/********END OF FILE ***********/
