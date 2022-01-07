#include "duet_cm4.h"
#include "duet_common.h"
#include "duet.h"
#include "duet_boot.h"

void duet_cfg_boot_type(void)
{
    uint32_t flag1 = RETENTION_SRAM->BOOT_CFG;
    uint32_t flag2 = REG_RD(AON_RST_CHECK_REG);
    if((RET_RAM_SOFT_RST_FLAG == flag1) && (AON_RST_CHECK_FLAG == (flag2 & AON_RST_CHECK_FLAG)))
    {
        RETENTION_SRAM->BOOT_TYPE = SOFTWARE_RST;
    }
    else if((RET_RAM_DS_RST_FLAG == flag1) && (AON_RST_CHECK_FLAG == (flag2 & AON_RST_CHECK_FLAG)))
    {
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = DEEP_SLEEP_RST;
    }
    else if((RET_RAM_SOFT_RST_FLAG == flag1) && (0 == flag2))
    {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_TYPE = HARDWARE_PIN_RST;
    }
    else if(0 == flag2)
    {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = PWR_ON_RST;
    }
    else
    {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = UNKNOWN_RST;
    }
}

void duet_set_ds_boot_type(void)
{
    RETENTION_SRAM->BOOT_CFG = RET_RAM_DS_RST_FLAG;
}

uint32_t duet_get_boot_type(void)
{
    return (RETENTION_SRAM->BOOT_TYPE);
}
