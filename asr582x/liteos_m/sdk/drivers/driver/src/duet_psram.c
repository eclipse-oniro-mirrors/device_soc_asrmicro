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

#include "duet_psram.h"
#include "duet_pinmux.h"

#define PSRAM_CMD_STOP           0
#define PSRAM_CMD_CMD            1
#define PSRAM_CMD_ADDR           2
#define PSRAM_CMD_DUMMY          3
#define PSRAM_CMD_MODE           4
#define PSRAM_CMD_MODE2          5
#define PSRAM_CMD_MODE4          6
#define PSRAM_CMD_READ           7
#define PSRAM_CMD_WRITE          8
#define PSRAM_CMD_JMP_ON_CS      9
#define PSRAM_CMD_ADDR_DDR       10
#define PSRAM_CMD_MODE_DDR       11
#define PSRAM_CMD_MODE2_DDR      12
#define PSRAM_CMD_MODE4_DDR      13
#define PSRAM_CMD_READ_DDR       14
#define PSRAM_CMD_WRITE_DDR      15
#define PSRAM_CMD_DATA_LEARN     16
#define PSRAM_CMD_CMD_DDR        17
#define PSRAM_CMD_CADDR          18
#define PSRAM_CMD_CADDR_DDR      19

#define PSRAM_SEQ_ID_READ        5
#define PSRAM_SEQ_ID_WRITE_EVICT 6
#define PSRAM_SEQ_ID_WRITE       7
#define PSRAM_SEQ_ID_READ_ID     8
#define PSRAM_SEQ_ID_QUAD_ENABLE 9
#define PSRAM_SEQ_ID_QUAD_EXIT   10
#define PSRAM_SEQ_ID_MODE_REG_READ   11
#define PSRAM_SEQ_ID_MODE_REG_WRITE  12

int mode_config = -1;

static void psram_wait_finish(void)
{
    while (REG_RD(PSRAM_SR) & 0x2);
    while (!(REG_RD(PSRAM_FR) & 0x1));
    REG_WR(PSRAM_FR, 1);
}

static void psram_init_lut_ps_read(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_READ));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_JMP_ON_CS << 26) | (0x0 << 24) | (0x00 << 16) | (PSRAM_CMD_READ << 10) | (0x0 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_write(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_WRITE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_JMP_ON_CS << 26) | (0x0 << 24) | (0x00 << 16) | (PSRAM_CMD_WRITE << 10) | (0x0 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_write_evict(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_WRITE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id), ((PSRAM_CMD_JMP_ON_CS << 26) | (0 << 24) | (0x00 << 16) | (21 << 10) | (0 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_mode_register_read(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_MODE_REG_READ));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_READ << 26) | (0x0 << 24) | (0x2 << 16) | (PSRAM_CMD_DUMMY << 10) | (0x0 << 8) | (8 - 1)));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), ((PSRAM_CMD_JMP_ON_CS << 10) | (0x0 << 8) | 0x0));
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_mode_register_write(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_MODE_REG_WRITE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_JMP_ON_CS << 26) | (0x0 << 24) | (0x00 << 16) | (PSRAM_CMD_WRITE << 10) | (0x0 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_qmode_enable(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id), ((PSRAM_CMD_CMD << 10) | (0 << 8) | FLASH_CMD_ENTER_QUAD_MODE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_qmode_exit(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id), ((PSRAM_CMD_CMD << 10) | (0x2 << 8) | FLASH_CMD_EXIT_QUAD_MODE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_read_4x(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x2 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x2 << 8) | FLASH_CMD_FAST_READ_QUAD));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_READ << 26) | (0x2 << 24) | (0x20 << 16) | (PSRAM_CMD_DUMMY << 10) | (0x2 << 8) | (6 - 1)));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), ((PSRAM_CMD_JMP_ON_CS << 10) | (0x2 << 8) | 0x00));
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_write_4x(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x2 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x2 << 8) | FLASH_CMD_WRITE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id),
           ((PSRAM_CMD_JMP_ON_CS << 26) | (0x2 << 24) | (0x00 << 16) | (PSRAM_CMD_WRITE << 10) | (0x2 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_write_evict_4x(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x2 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x2 << 8) | FLASH_CMD_WRITE));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id), ((PSRAM_CMD_JMP_ON_CS << 26) | (0x2 << 24) | (0x00 << 16) | (21 << 10) | (0x2 << 8) | 0x20));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

static void psram_init_lut_ps_readid(unsigned int seq_id)
{
    REG_WR((PSRAM_LUT0 + 0x10 * seq_id),
           ((PSRAM_CMD_ADDR << 26) | (0x0 << 24) | (0x18 << 16) | (PSRAM_CMD_CMD << 10) | (0x0 << 8) | FLASH_CMD_READ_ID));
    REG_WR((PSRAM_LUT1 + 0x10 * seq_id), ((PSRAM_CMD_READ << 10) | (0x0 << 8) | 0x9));
    REG_WR((PSRAM_LUT2 + 0x10 * seq_id), 0);
    REG_WR((PSRAM_LUT3 + 0x10 * seq_id), 0);
}

int psram_set_channel(duet_psram_channel channel)
{
    if (channel != PSRAM_CHANNEL_4_9 && channel != PSRAM_CHANNEL_16_21) {
        return -1;
    }
    REG_WR(ADC_SDIO_BLE_DEBUG_CTRL_REG, REG_RD(ADC_SDIO_BLE_DEBUG_CTRL_REG) & (~(1 << 3))); // disable sdio debug
    if (channel == PSRAM_CHANNEL_4_9) {
        duet_pinmux_config(PAD4, PF_PSRAM); // set pad4 mux to psram
        duet_pinmux_config(PAD5, PF_PSRAM); // set pad5 mux to psram
        duet_pinmux_config(PAD6, PF_PSRAM); // set pad6 mux to psram
        duet_pinmux_config(PAD7, PF_PSRAM); // set pad7 mux to psram
        duet_pinmux_config(PAD8, PF_PSRAM); // set pad8 mux to psram
        duet_pinmux_config(PAD9, PF_PSRAM); // set pad9 mux to psram
        REG_WR(HW_CTRL_PE_PS,REG_RD(HW_CTRL_PE_PS)& 0xfffffc0f); // set all pad pull up and down
    } else { // set
        duet_pinmux_config(PAD16, PF_PSRAM); // sett pad16 mux to psram
        duet_pinmux_config(PAD17, PF_PSRAM); // set pad17 mux to psr// set
        duet_pinmux_config(PAD18, PF_PSRAM); // set pad18 mux to psram
        duet_pinmux_config(PAD19, PF_PSRAM); // set pad19 mux to psram
        duet_pinmux_config(PAD20, PF_PSRAM); // set pad20 mux to psram
        duet_pinmux_config(PAD21, PF_PSRAM); // set pad21 mux to psram
        REG_WR(HW_CTRL_PE_PS,REG_RD(HW_CTRL_PE_PS)& 0xffc0ffff); // set all pad pull up and down
    }// set
    REG_WR(PERI_CLK_EN_REG0, (REG_RD(PERI_CLK_EN_REG0) | (0x0000203F))); //open clock of dma and psram

    return 0; // set
}
int psram_config(duet_psram_mode mode)
{
    if (mode != PSRAM_MODE_SPI && mode != PSRAM_MODE_QSPI) {
        return -1;
    }
    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) & (~(1 << 14))); // MCR,MDIS = 0

    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) | ((3))); // MCR , AHB and SF domain reset

    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) & 0xfffffffc); // clear AHB and SF reset

    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) | 1 << 14); // MCR , MDIS=1

    if (mode == PSRAM_MODE_SPI) { // the simple logic has been modified. set bit7 means to use the original logic
        REG_WR(PSRAM_SMPR, 0x00000080);    // sampled by sfif_clk_b; half cycle delay
    } else {
        REG_WR(PSRAM_SMPR, 0x00000000);    // sampled by sfif_clk_b; half cycle delay
    }

    REG_WR(PSRAM_FLSHCR, 0x00000000);  // set setup and hold time for psram

    // Give the default source address:
    REG_WR(PSRAM_SFAR, PSRAM_FLASH_A1_BASE);  // set IDATSIZ

    // lijin: 2018_01_11-11:07 total buffer size is 64*64b = 512B = 0x200B
    // config the ahb buffer// set
    REG_WR(PSRAM_BUF0IND, 0x00000080);
    REG_WR(PSRAM_BUF1IND, 0x00000100);
    REG_WR(PSRAM_BUF2IND, 0x00000180);

    // lijin: 2018_01_11-11:07 each buffer is 8B * 16 = 128B = 0x80B

    // kzheng: TO DO, now hmaster[3:0] tie0, so need modified here
    // mst id =0 is CPU
    // PSRAM0_BUF0CR = 0x00001001; // CPU A9
    REG_WR(PSRAM_BUF0CR, 0x00001000);  // CPU A9
    REG_WR(PSRAM_BUF1CR, 0x00001006);  // CPU M4
    REG_WR(PSRAM_BUF2CR, 0x00001003);  // SDMA
    REG_WR(PSRAM_BUF3CR, 0x80001002);  // other masters

    // config the flash mmap
    REG_WR(PSRAM_SFA1AD, PSRAM_FLASH_A1_TOP & 0xfffffc00);
    REG_WR(PSRAM_SFA2AD, PSRAM_FLASH_A2_TOP & 0xfffffc00);
    REG_WR(PSRAM_SFB1AD, PSRAM_FLASH_B1_TOP & 0xfffffc00);
    REG_WR(PSRAM_SFB2AD, PSRAM_FLASH_B2_TOP & 0xfffffc00);

    // ISD3FB, ISD2FB, ISD3FA, ISD2FA = 1; ENDIAN = 'h3; END_CFG='h3
    // DELAY_CLK4X_EN = 1
    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) | 0x000F000C);

    // ddr_en   data aligned with 2x serial flash half clock
    // REG_WR(PSRAM_FLSHCR, REG_RD(PSRAM_FLSHCR)| 0x00010000);

    // MCR SCLKCFG 0, dqs en =1
    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) & 0xfbffffff);

    // dqs_loopback_en = 1, dqs_loopback_from_pad = 1
    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) | (3 << 24));

    // ddr_en = 1, enable 2x and 4x clock
    /// REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR)| 1<<7);

    // MDIS = 0, enable psram clocks,must clear MDIS to enable clock for transfer.
    REG_WR(PSRAM_MCR, REG_RD(PSRAM_MCR) & 0xffffbfff);

    // printf("PSRAM initialize done. \n");
    // printf("begin initialize LUT tables. \n");

    if (mode == PSRAM_MODE_SPI) {
        psram_init_lut_ps_read(0);
        psram_init_lut_ps_read(PSRAM_SEQ_ID_READ);
        psram_init_lut_ps_write_evict(PSRAM_SEQ_ID_WRITE_EVICT);
        psram_init_lut_ps_write(PSRAM_SEQ_ID_WRITE);
        psram_init_lut_ps_readid(PSRAM_SEQ_ID_READ_ID);
        psram_init_lut_ps_mode_register_read(PSRAM_SEQ_ID_MODE_REG_READ);
        psram_init_lut_ps_mode_register_write(PSRAM_SEQ_ID_MODE_REG_WRITE);
    } else {
        psram_init_lut_ps_read_4x(0);
        psram_init_lut_ps_read_4x(PSRAM_SEQ_ID_READ);
        psram_init_lut_ps_write_evict_4x(PSRAM_SEQ_ID_WRITE_EVICT);
        psram_init_lut_ps_write_4x(PSRAM_SEQ_ID_WRITE);
    }
    psram_init_lut_ps_qmode_enable(PSRAM_SEQ_ID_QUAD_ENABLE);
    psram_init_lut_ps_qmode_exit(PSRAM_SEQ_ID_QUAD_EXIT);

    // printf(" initialize LUT tables done. \n");

    // set read miss cmd, evict is in next lut
    REG_WR(PSRAM_BFGENCR, PSRAM_SEQ_ID_READ << 12);

    if (mode_config != mode) {
        if (mode == PSRAM_MODE_SPI) {
            REG_WR(PSRAM_SFAR, PSRAM_AMBA_BASE);
            REG_WR(PSRAM_IPCR, PSRAM_SEQ_ID_QUAD_EXIT << 24);
            psram_wait_finish();
        } else {
            REG_WR(PSRAM_SFAR, PSRAM_AMBA_BASE);
            REG_WR(PSRAM_IPCR, PSRAM_SEQ_ID_QUAD_ENABLE << 24);
            psram_wait_finish();
        }
    }
    mode_config = mode;
    return 0;
}
