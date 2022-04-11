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

#ifndef _DUET_FLASH_ALG_H_
#define _DUET_FLASH_ALG_H_

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define FLASH_BASE      0x40003000

#define PP   0x02
#define QPP  0x32
#define PP4  0x38

#define WREN  0x06
#define SE    0x20
#define BE    0xD8
#define BE32K 0x52
#define CE    0xC7
#define EQPI  0x35
#define RSTEN 0x66
#define RST   0x99

#define READ  0x03
#define F_RD  0x0B
#define DREAD 0x3B
#define QREAD 0x6B
#define READ2 0xBB
#define READ4 0xEB

#define RDID 0x9F
#define WRSR 0x01
#define RDSR 0x05
#define RDSR1 0x35

#define OSC (100)
#define OSC_D ((OSC*144)/8)

typedef struct {
    vu32 QSPI_CR     ;        // offset 0x00
    vu32 QSPI_DCR    ;        // offset 0x04
    vu32 QSPI_SR     ;        // offset 0x08
    vu32 QSPI_FCR    ;        // offset 0x0C
    vu32 QSPI_DLR    ;        // offset 0x10
    vu32 QSPI_CCR    ;        // offset 0x14
    vu32 QSPI_AR     ;        // offset 0x18
    vu32 QSPI_ABR    ;        // offset 0x1C
    vu32 QSPI_DR     ;        // offset 0x20
    vu32 QSPI_PSMKR  ;        // offset 0x24
    vu32 QSPI_PSMAR  ;        // offset 0x28
    vu32 QSPI_PIR    ;        // offset 0x2C
    vu32 QSPI_TOR    ;        // offset 0x30
    vu32 RESERVERD[19];
    vu32 QSPI_HIT0R  ;        // offset 0x80
    vu32 QSPI_HIT1R  ;        // offset 0x84
    vu32 QSPI_MIR    ;        // offset 0x88
    vu32 QSPI_CFGR   ;        // offset 0x8C
    vu32 SBUS_START  ;        // offset 0x90
} FLASH_TypeDef;

#define NVM_CTRL_WR_LOCK        *((volatile unsigned int *)(0x40000A04))
#define FLASH_ACCESS_CONTROL0   *((volatile unsigned int *)(0x40000A08))
#define FLASH_ACCESS_CONTROL1   *((volatile unsigned int *)(0x40000A0C))
#define FLASH_REMAPPING_REG     *((volatile unsigned int *)(0x4000006C))
#define FLASH_REMAPPING_BIT     0x1

#define FLASH_CACHE_CTRL        *((volatile unsigned int *)(0x4001C000))

#define CACHE_BYPASS            (1 << 1)
#define CACHE_FLUSH             (1)

#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)
#define VERS       1           // Interface Version 1.01

#define UNKNOWN    0           // Unknown
#define ONCHIP     1           // On-chip Flash Memory
#define EXT8BIT    2           // External Flash Device on 8-bit  Bus
#define EXT16BIT   3           // External Flash Device on 16-bit Bus
#define EXT32BIT   4           // External Flash Device on 32-bit Bus
#define EXTSPI     5           // External Flash Device on SPI

#define SPI_FLASH_ADDR_START        0x10000000
#define SPI_FLASH_ADDR_END          0x17FFFFFF
#define SPI_FLASH_SEC_SIZE  0x1000
#define SPI_FLASH_BLOCK_32K_SIZE    0x8000
#define SPI_FLASH_BLOCK_64K_SIZE    0x10000
#define SPI_FLASH_PAGE_SIZE 0x100
#define SECTOR_NUM             512         // Max Number of Sector Items
#define PAGE_MAX               65536       // Max Page Size for Programming
#define CLK_FLASH_SEL       0x40000808

#define FLASH_52_CLK        0x0            // flash clk src 52MHz
#define FLASH_120_CLK       0x1            // flash clk src 120MHz
#define FLASH_240_CLK       0x2            // flash clk src 240MHz

#define FLASH_QSPI_DIV4     0x09004801
#define FLASH_QSPI_DIV3     0x89004801
#define FLASH_QSPI_DIV2     0x08004801
#define FLASH_QSPI_L1       0x2D1C250B
#define FLASH_QSPI_L2       0x4E1C253B
#define FLASH_QSPI_L4       0x6F0CEDEB
#define FLASH_QSPI_DCR_2M   0x00140000
#define FLASH_QSPI_DCR_4M   0x00150000

#define CHIP_ERASE_CMD      0x0000160
#define SECTOR_ERASE_CMD    0x0002520
#define BLOCK32_ERASE_CMD   0x0002552
#define BLOCK64_ERASE_CMD   0x00025D8

struct FlashSectors  {
    unsigned long   szSector;    // Sector Size in Bytes
    unsigned long AddrSector;    // Address of Sector
};

#define SECTOR_END 0xFFFFFFFF, 0xFFFFFFFF

struct FlashDevice  {
    unsigned short     Vers;    // Version Number and Architecture
    char       DevName[128];    // Device Name and Description
    unsigned short  DevType;    // Device Type: ONCHIP, EXT8BIT, EXT16BIT, ...
    unsigned long    DevAdr;    // Default Device Start Address
    unsigned long     szDev;    // Total Size of Device
    unsigned long    szPage;    // Programming Page Size
    unsigned long     szSector;
    unsigned long       Res;    // Reserved for future Extension
    unsigned char  valEmpty;    // Content of Erased Memory

    unsigned long    toProg;    // Time Out of Program Page Function
    unsigned long   toErase;    // Time Out of Erase Sector Function

    struct FlashSectors sectors[SECTOR_NUM];
};

#define FLASH_DRV_VERS (0x0100+VERS)   // Driver Version, do not modify!

#define FLASH_DRIVER_SEG __attribute__((section("seg_flash_driver")))

void duet_flash_alg_read_buf_clr(void);
// void duet_flash_alg_cache_bypass(void);
void duet_flash_alg_cache_enable(void);
// void duet_flash_alg_cache_flush(void);
int  duet_flash_alg_init (void);
int duet_flash_alg_erase(unsigned int cmd, unsigned long adr);
int  duet_flash_alg_programpage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);
#ifdef _SPI_FLASH_240MHz_
void duet_flash_alg_set_clk_240(unsigned char en_240m);
#elif defined  _SPI_FLASH_120MHz_
void duet_flash_alg_set_clk_120(unsigned char en_120m);
#endif
#endif // _LEGA_FLASH_ALG_H_