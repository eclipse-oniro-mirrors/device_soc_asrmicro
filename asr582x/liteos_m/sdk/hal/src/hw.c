/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "duet_cm4.h"
#include "board.h"
#include "system_cm4.h"

void hal_reboot(void)
{
    duet_system_reset();
}

void hw_start_hal(void)
{
    printf("start-----------hal\n");
}

void HAL_NVIC_SystemReset(void)
{
}