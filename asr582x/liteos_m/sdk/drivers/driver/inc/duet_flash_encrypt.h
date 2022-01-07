#ifndef _DUET_FLASH_ENCRYPT_H_
#define _DUET_FLASH_ENCRYPT_H_

#ifdef __cplusplus
    extern "C" {
#endif
#include <errno.h>

/**
 * Get the encrypt status
 *
 * @param[in]  none
 *
 * @return  0 : un-encrypt, 1 : encrypt
 */
int duet_flash_encrypt_status();

/**
 * Switch flash to encrypt mode.
 *
 * @note This operation is irreversible
 *
 * @param[in]  none
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int duet_flash_encrypt();

/**
 * Switch flash to disable encrypt mode.
 *
 * @note This operation is irreversible
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 *
 * @return  none
 */
int duet_flash_disable_encrypt();

/**
 * Get the total times of encrypt and dis-encrypt operation rest.
 * When the return value reduces to zero, duet_flash_encrypt and duet_flash_disable_encrypt will be invalid.
 *
 * @param[in]  none
 *
 * @return  The total times of encrypt and dis-encrypt operation rest.
 */
int duet_flash_encrypt_operation_rest();

#ifdef __cplusplus
   }
#endif

#endif // _DUET_FLASH_ENCRYPT_H_

