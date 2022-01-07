#include "duet.h"
#include "duet_flash_encrypt.h"
#include "duet_efuse.h"

#define FLASH_CRYPT_EFUSE_KEY_ADDR      124
#define FLASH_CRYPT_EFUSE_ENABLE_ADDR   128
#define FLASH_CRYPT_EFUSE_KEY           0x12345678
#define FLASH_CRYPT_RESET               *((volatile unsigned int *)(0x40005038)) //default 0, write 1 reset, HW auto clear

#define FLASH_CRYPT_BIT_NUM             23  //bit0~bit22

int duet_flash_encrypt_status()
{
    return ((*((volatile unsigned int *)(0x40000910))) & 0x0020) ? 1 : 0;
}

int switch_flash_encrypt()
{
    uint32_t reg;
    duet_efuse_init(1);

    // write flash crypt key and enable
    duet_efuse_word_write(FLASH_CRYPT_EFUSE_KEY_ADDR, FLASH_CRYPT_EFUSE_KEY);
    // flash crypt enable
    reg = duet_efuse_word_read(FLASH_CRYPT_EFUSE_ENABLE_ADDR);
    for(int i = 0; i < FLASH_CRYPT_BIT_NUM; i++)
    {
        if(!(reg & (0x00000001 << i)))
        {
            duet_efuse_word_write(FLASH_CRYPT_EFUSE_ENABLE_ADDR, (0x00000001 << i));
            // reset to make flash crypt module work
            FLASH_CRYPT_RESET = 1;
            while(FLASH_CRYPT_RESET);
            return 0;
        }
    }
    return EIO;
}

int duet_flash_encrypt()
{
    if( duet_flash_encrypt_status())
        return 0;

    return switch_flash_encrypt();
}

int duet_flash_disable_encrypt()
{
    if(!duet_flash_encrypt_status())
        return 0;

    return switch_flash_encrypt();
}

int duet_flash_encrypt_operation_rest()
{
    int num = 0;
    uint32_t reg;

    duet_efuse_init(1);
    reg = duet_efuse_word_read(FLASH_CRYPT_EFUSE_ENABLE_ADDR);
    for(int i = 0; i < FLASH_CRYPT_BIT_NUM; i++)
    {
        num += !(reg & (0x00000001 << i));
    }
    return num;
}