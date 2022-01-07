#include <stdio.h>
#include "duet_cm4.h"
#include "duet_common.h"
#include "duet.h"
#include "sns_silib.h"
#include "crys_rnd.h"
#include "lega_aes.h"
#include "lega_hw_common.h"

/************************ Functions *****************************/

/*!
@brief This function is used to initialize an AES operation context.
       To operate the AES machine, this must be the first API called.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesInit(
    SaSiAesUserContext_t * pContext,
    SaSiAesEncryptMode_t   encryptDecryptFlag,
    SaSiAesOperationMode_t operationMode,
    SaSiAesPaddingType_t   paddingType
){
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret != 0)
        printf("aes lock err\r\n");
#endif
    return SaSi_AesInit( pContext, encryptDecryptFlag, operationMode, paddingType);
}


/*!
@brief This function sets the key information for the AES operation, in the context that was initialized by SaSi_AesInit.
\note When FIPS certification mode is set to ON, and the mode is AES-XTS, weak keys are not allowed (128/256 lsb bits must be
different than 128/256 msb bits, according to the key size).
@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesSetKey(
    SaSiAesUserContext_t * pContext,
    SaSiAesKeyType_t       keyType,
    void *                 pKeyData,
    size_t                 keyDataSize
){
    return SaSi_AesSetKey(pContext, keyType, pKeyData,  keyDataSize);
}


/*!
@brief This function sets the IV, counter or tweak data for the following AES operation on the same context.
       The context must be first initialized by SaSi_AesInit.
       It must be called at least once prior to the first SaSi_AesBlock operation on the same context - for those ciphers that require it.
       If needed, it can also be called to override the IV in the middle of a sequence of SaSi_AesBlock operations.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesSetIv(
    SaSiAesUserContext_t * pContext,
    SaSiAesIv_t            pIV
){
    return SaSi_AesSetIv(pContext, pIV);
}


/*!
@brief This function retrieves the current IV, counter or tweak from the AES context.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesGetIv(
    SaSiAesUserContext_t * pContext,
    SaSiAesIv_t            pIV
){
    return SaSi_AesGetIv(pContext, pIV);
}


/*!
@brief This function performs an AES operation on an input data buffer, according to the configuration defined in the context parameter.
       It can be called as many times as needed, until all the input data is processed.
       SaSi_AesInit, SaSi_AesSetKey, and for some ciphers SaSi_AesSetIv, must be called before
       the first call to this API with the same context.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesBlock(
    SaSiAesUserContext_t * pContext,
    uint8_t *              pDataIn,
    size_t                 dataInSize,
    uint8_t *              pDataOut
){
    return SaSi_AesBlock(pContext, pDataIn, dataInSize, pDataOut);
}


/*!
@brief This function is used to finish AES operation.

       It processes the last data block if needed, finalizes the AES operation (cipher-specific),
       and produces operation results (for MAC operations).
       \note In case AES padding is used (PKCS#7) Din and Dout user's buffers must include extra space for
       the padding scheme.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesFinish(
    SaSiAesUserContext_t * pContext,
    size_t                 dataSize,
    uint8_t *              pDataIn,
    size_t                 dataInBuffSize,
    uint8_t *              pDataOut,
    size_t *               dataOutBuffSize
){
    return SaSi_AesFinish(pContext, dataSize, pDataIn, dataInBuffSize, pDataOut, dataOutBuffSize);
}


/*!
@brief This function releases and crears resources after AES operations.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesFree(
    SaSiAesUserContext_t * pContext
){
    return SaSi_AesFree(pContext);
}

void lega_aes_deinit()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret != 0)
        printf("aes unlock err\r\n");
#endif
}
