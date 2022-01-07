#include <stdio.h>
#include "lega_ccm.h"
#include "lega_hw_common.h"

uint32_t  lega_CC_AESCCM(
               SaSiAesEncryptMode_t       EncrDecrMode,     /*!< [in] A flag specifying whether an AES Encrypt (::SASI_AES_ENCRYPT) or Decrypt
                                          (::SASI_AES_DECRYPT) operation should be performed. */
               CRYS_AESCCM_Key_t          CCM_Key,          /*!< [in] Pointer to AES-CCM key. */
               CRYS_AESCCM_KeySize_t      KeySizeId,        /*!< [in] Enumerator defining the key size (only 128 bit is valid). */
               uint8_t                   *N_ptr,            /*!< [in] Pointer to the Nonce. */
               uint8_t                    SizeOfN,          /*!< [in] Nonce byte size. The valid values depend on the ccm mode:
                                                                                        <ul><li>CCM:  valid values = [7 .. 13].</li>
                                                                                        <li>CCM*: valid values = [13].</li></ul> */
               uint8_t                   *ADataIn_ptr,      /*!< [in] Pointer to the additional input data. The buffer must be contiguous. */
               uint32_t                   ADataInSize,      /*!< [in] Byte size of the additional data. */
               uint8_t                   *TextDataIn_ptr,   /*!< [in] Pointer to the plain-text data for encryption or cipher-text data for decryption.
                                          The buffer must be contiguous. */
               uint32_t                   TextDataInSize,   /*!< [in] Byte size of the full text data. */
               uint8_t                   *TextDataOut_ptr,  /*!< [out] Pointer to the output (cipher or plain text data according to encrypt-decrypt mode)
                                           data. The buffer must be contiguous. */
               uint8_t                    SizeOfT,          /*!< [in] AES-CCM MAC (tag) byte size. The valid values depend on the ccm mode:
                                                                                        <ul><li>CCM:  valid values = [4, 6, 8, 10, 12, 14, 16].</li>
                                                                                        <li>CCM*: valid values = [0, 4, 8, 16].</li></ul>*/
               CRYS_AESCCM_Mac_Res_t      Mac_Res,           /*!< [in/out] Pointer to the MAC result buffer. */
                           uint32_t ccmMode                             /*!< [in] Flag specifying whether AES-CCM or AES-CCM* should be performed. */
){
    int ret, ret0;
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    ret0 = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret0 != 0)
        printf("ccm lock err\r\n");
#endif
    ret = CC_AESCCM(EncrDecrMode, CCM_Key, KeySizeId, N_ptr, SizeOfN, ADataIn_ptr, ADataInSize, TextDataIn_ptr, TextDataInSize, TextDataOut_ptr, SizeOfT, Mac_Res, ccmMode);
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    ret0 = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret0 != 0)
        printf("ccm unlock err\r\n");
#endif
    return ret;
}
