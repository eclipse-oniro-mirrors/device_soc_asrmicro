/**
 ****************************************************************************************
 *
 * @file sonata_ble_rom.h
 *
 * @brief This file contains the declare of the rom
 *
 *
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_BLE_ROM_H_
#define _SONATA_BLE_ROM_H_

#if BLE_ROM
/*
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition

#define DATA_SEG_INIT   0x01
#define BSS_SEG_INIT    0x02
#define FUNC_SEG_INIT   0x04
#define ALL_SEG_INIT    (DATA_SEG_INIT|BSS_SEG_INIT|FUNC_SEG_INIT)
/*
 * FUNCTION DECLARATION
 *****************************************************************************************
 */
extern void sonata_ble_rom_init(uint8_t init);

#endif //BLE_ROM

#endif // _SONATA_BLE_ROM_H_

