#ifndef _DUET_BOOT_H_
#define _DUET_BOOT_H_

#define AON_RST_CHECK_REG                   (ALWAYS_ON_REGFILE + 0x10)
#define AON_RST_CHECK_FLAG                  0x00000002
#define RET_RAM_SOFT_RST_FLAG               0xAA55A501
#define RET_RAM_DS_RST_FLAG                 0xAA55A502

#define BOOT_TYPE_MAGIC_ID      0x55AA5A00
#define PWR_ON_RST              (BOOT_TYPE_MAGIC_ID + 0x00)
#define HARDWARE_PIN_RST        (BOOT_TYPE_MAGIC_ID + 0x01)
#define SOFTWARE_RST            (BOOT_TYPE_MAGIC_ID + 0x02) //including NVIC_SystemReset and WDG RST
#define DEEP_SLEEP_RST          (BOOT_TYPE_MAGIC_ID + 0x03)
#define UNKNOWN_RST             (BOOT_TYPE_MAGIC_ID + 0xFF)

void duet_cfg_boot_type(void);
void duet_set_ds_boot_type(void);
uint32_t duet_get_boot_type(void);
#endif //_DUET_BOOT_H_