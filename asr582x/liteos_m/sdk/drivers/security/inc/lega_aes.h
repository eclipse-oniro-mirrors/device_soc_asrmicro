#ifndef _LEGA_AES_H_
#define _LEGA_AES_H_
#include "ssi_aes_defs.h"
#include "ssi_aes.h"

/************************ Functions *****************************/

/*!
@brief This function is used to initialize an AES operation context.
       To operate the AES machine, this must be the first API called.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesInit(
    SaSiAesUserContext_t * pContext,            /*!< [in]  Pointer to the AES context buffer that is allocated by the caller and initialized by this API.
                                   Should be used in all subsequent calls that are part of the same operation. */
    SaSiAesEncryptMode_t   encryptDecryptFlag,  /*!< [in]  A flag specifying whether an AES Encrypt (SASI_AES_Encrypt) or Decrypt (SASI_AES_Decrypt) operation should be performed.
                                   Must be set to CRYS_AES_Encrypt in CBC-MAC, XCBC-MAC and CMAC modes. */
    SaSiAesOperationMode_t operationMode,       /*!< [in]  The operation cipher/mode. */
    SaSiAesPaddingType_t   paddingType          /*!< [in]  The padding type for AES operation:
                                - NONE  - supported for all operation modes.
                                - PKCS7 - supported for ECB, CBC, CBC-MAC operation modes. */
);


/*!
@brief This function sets the key information for the AES operation, in the context that was initialized by SaSi_AesInit.
\note When FIPS certification mode is set to ON, and the mode is AES-XTS, weak keys are not allowed (128/256 lsb bits must be
different than 128/256 msb bits, according to the key size).
@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesSetKey(
    SaSiAesUserContext_t * pContext,        /*!< [in]  Pointer to the AES context, after it was initialized by SaSi_AesInit. */
    SaSiAesKeyType_t       keyType,         /*!< [in]  The type of key to be used for the AES operation.
                               Currently only SASI_AES_USER_KEY is supported - the key is plaintext and provided in the pKeyData parameter. */
    void *                 pKeyData,        /*!< [in]  Pointer to the key data structure (to be casted to the relevant struct type). */
    size_t                 keyDataSize      /*!< [in]  The size of data passed in pKeyData in bytes. */
);


/*!
@brief This function sets the IV, counter or tweak data for the following AES operation on the same context.
       The context must be first initialized by SaSi_AesInit.
       It must be called at least once prior to the first SaSi_AesBlock operation on the same context - for those ciphers that require it.
       If needed, it can also be called to override the IV in the middle of a sequence of SaSi_AesBlock operations.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesSetIv(
    SaSiAesUserContext_t * pContext,    /*!< [in]  Pointer to the AES context. */
    SaSiAesIv_t            pIV          /*!< [in]  Pointer to the buffer of the IV, counter or tweak.
                            <ul><li> For CBC, CBC-CTS, OFB and CBC-MAC modes - the IV value.</li>
                            <li> For CTR mode - the counter.</li>
                            <li> For XTS mode - the tweak value.</li>
                            <li> For all other modes - N/A. </li></ul>*/
);


/*!
@brief This function retrieves the current IV, counter or tweak from the AES context.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesGetIv(
    SaSiAesUserContext_t * pContext,    /*!< [in]  Pointer to the AES context. */
    SaSiAesIv_t            pIV          /*!< [out] Pointer to the buffer of the IV, counter or tweak.
                            <ul><li> For CBC, CBC-CTS, OFB and CBC-MAC modes - the IV value.</li>
                            <li> For CTR mode - the counter.</li>
                            <li> For XTS mode - the tweak value.</li>
                            <li> For all other modes - N/A. </li></ul> */
);


/*!
@brief This function performs an AES operation on an input data buffer, according to the configuration defined in the context parameter.
       It can be called as many times as needed, until all the input data is processed.
       SaSi_AesInit, SaSi_AesSetKey, and for some ciphers SaSi_AesSetIv, must be called before
       the first call to this API with the same context.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesBlock(
    SaSiAesUserContext_t * pContext,    /*!< [in]  Pointer to the AES context. */
    uint8_t *              pDataIn,     /*!< [in]  Pointer to the buffer of the input data to the AES. The pointer does not need to be aligned.
                               For TZ, the size of the scatter/gather list representing the data buffer is limited to 128 entries,
                               and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB).
                               For ARM CryptoCell 3xx, The buffer must be contiguous and limited to 64KB. */
    size_t                 dataInSize,  /*!< [in]  Size of the input data in bytes.
                            <ul><li> For all modes except XTS, must be multiple of 16 bytes.</li>
                            <li> For XTS mode, only the following data sizes are supported: 64, 512, 520, 521, 1024 and 4096 bytes.
                                 The data passed in a single SaSi_AesBlock call is considered to be a single XTS unit.
                                 All subsequent calls to this API with the same context must use the same data size. </li></ul>*/
    uint8_t *              pDataOut     /*!< [out] Pointer to the output buffer. The pointer does not need to be aligned.
                               For CBC-MAC, XCBC-MAC, CMAC modes it may be NULL.
                               For TZ, the size of the scatter/gather list representing the data buffer is limited to 128 entries,
                               and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB).
                               For ARM CryptoCell 3xx, The buffer must be contiguous and limited to 64KB. */
);


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
    SaSiAesUserContext_t * pContext,       /*!< [in]  Pointer to the AES context. */
    size_t                 dataSize,       /*!< [in]  The size of the input data in bytes.
                               <ul><li> For CBC-CTS mode, must be > 16. Can be <=16 only if this is the only data (no previous calls were
                                        made to SaSi_AesBlock with the same context).</li>
                               <li> For XTS mode, the data size must conform to the dataInSize rules as listed for XTS under the
                                SaSi_AesBlock API, and match the data size passed in the previous calls to SaSi_AesBlock with the
                                same context.</li>
                               <li> For all other modes, zero is a valid size.</li>
                               <li> For ECB, CBC, CBC-MAC modes: </li>
                                 <ul><li> Must be >= 0, if direction is SASI_AES_ENCRYPT and padding type is SASI_AES_PADDING_PKCS7.</li>
                                 <li> Must be >= 16 and a multiple of 16 bytes, if direction is SASI_AES_DECRYPT and padding type
                                   is SASI_AES_PADDING_PKCS7.</li>
                                 <li> Must be a multiple of 16 bytes, otherwise. </li></ul></ul>*/
    uint8_t *              pDataIn,        /*!< [in]  Pointer of the input data buffer.
                              For TZ, the size of the scatter/gather list representing the data buffer is limited to 128 entries,
                              and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB).
                              For ARM CryptoCell 3xx, The buffer must be contiguous and limited to 64KB. */
    size_t                 dataInBuffSize, /*!< [in]  Size of pDataIn buffer in bytes.
                               <ul><li> Must be >= dataSize. </li>
                               <li> According to padding type, must be >= dataSize + padding. For PKCS7, padding size is
                                maximum SASI_AES_BLOCK_SIZE_IN_BYTES. </li></ul>*/
    uint8_t *              pDataOut,       /*!< [out] Pointer to the output buffer.
                              For TZ, the size of the scatter/gather list representing the data buffer is limited to 128 entries,
                              and the size of each entry is limited to 64KB (fragments larger than 64KB are broken into fragments <= 64KB).
                              For ARM CryptoCell 3xx, The buffer must be contiguous and limited to 64KB. */
    size_t *               dataOutBuffSize /*!< [in,out]  In - Size of pDataOut buffer in bytes.
                              The output buffer size must be no less than:
                               <ul><li> For CBC-MAC, XCBC-MAC, CMAC modes - 16 bytes (for MAC result).</li>
                               <li> For non-MAC modes - dataInBuffSize.</li></ul>
                              Out - The size in bytes of the actual output data:
                               <ul><li> If direction is SASI_AES_ENCRYPT and padding type is SASI_AES_PADDING_PKCS7, it is the actual size
                                 with the padding.</li>
                               <li> If direction is SASI_AES_DECRYPT and padding type is SASI_AES_PADDING_PKCS7, it is the size without
                                 the padding. </li>
                               <li> For CBC-MAC, XCBC-MAC, CMAC modes - always 16 bytes. </li></ul>*/
);


/*!
@brief This function releases and crears resources after AES operations.

@return SASI_OK on success,
@return A non-zero value from ssi_aes_error.h on failure.
*/
int lega_AesFree(
    SaSiAesUserContext_t * pContext     /*!< [in] Pointer to the AES context. */
);

void lega_aes_deinit();
#endif //_LEGA_AES_H_