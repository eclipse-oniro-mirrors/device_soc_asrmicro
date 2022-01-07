#ifndef _LEGA_HMAC_H_
#define _LEGA_HMAC_H_
#include "crys_hmac.h"

/*!
@brief This function initializes the HMAC machine.

It allocates and initializes the HMAC Context. It initiates a HASH session and processes a HASH update on the Key XOR ipad,
then stores it in the context

@return CRYS_OK on success.
@return A non-zero value from crys_hmac_error.h on failure.
*/
int lega_CRYS_HMAC_Init(
                        CRYS_HMACUserContext_t     *ContextID_ptr,         /*!< [in]  Pointer to the HMAC context buffer allocated by the user, which is used
                                              for the HMAC machine operation. */
                        CRYS_HASH_OperationMode_t  OperationMode,          /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
                        uint8_t                    *key_ptr,               /*!< [in]  The pointer to the user's key buffer. */
                        uint16_t                    keySize                /*!< [in]  The key size in bytes. */
);

void lega_hmac_deinit();

/*!
@brief This function processes a block of data to be HASHed.

It receives a handle to the HMAC Context, and updates the HASH value with the new data.

@return CRYS_OK on success.
@return A non-zero value from crys_hmac_error.h on failure.
*/

int lega_CRYS_HMAC_Update(
                        CRYS_HMACUserContext_t  *ContextID_ptr,         /*!< [in]  Pointer to the HMAC context buffer allocated by the user
                                           that is used for the HMAC machine operation. */
                        uint8_t                 *DataIn_ptr,            /*!< [in]  Pointer to the input data to be HASHed.
                                                                                   The size of the scatter/gather list representing the data buffer is limited to
                                           128 entries, and the size of each entry is limited to 64KB
                                           (fragments larger than 64KB are broken into fragments <= 64KB). */
                        size_t                 DataInSize             /*!< [in]  Byte size of the input data. Must be > 0.
                                                                                   If not a multiple of the HASH block size (64 for SHA-1 and SHA-224/256,
                                           128 for SHA-384/512), no further calls to ::CRYS_HMAC_Update are allowed in
                                           this context, and only ::CRYS_HMAC_Finish can be called to complete the
                                           computation. */
);


/*!
@brief This function finalizes the HMAC processing of a data block.

It receives a handle to the HMAC context that was previously initialized by ::CRYS_HMAC_Init, or by ::CRYS_HMAC_Update.
It completes the HASH calculation on the ipad and text, and then executes a new HASH operation with the key XOR opad and the previous
HASH operation result.

@return CRYS_OK on success.
@return A non-zero value from crys_hmac_error.h on failure.
*/

int lega_CRYS_HMAC_Finish(
                        CRYS_HMACUserContext_t  *ContextID_ptr,         /*!< [in]  Pointer to the HMAC context buffer allocated by the user, which is used
                                           for the HMAC machine operation. */
                        CRYS_HASH_Result_t       HmacResultBuff         /*!< [out] Pointer to the word-aligned 64 byte buffer. The actual size of the
                                           HASH result depends on CRYS_HASH_OperationMode_t. */
);


/*!
@brief This function is a service function that frees the context if the operation has failed.

The function executes the following major steps:
<ol><li> Checks the validity of all of the inputs of the function. </li>
<li> Clears the user's context.</li>
<li> Exits the handler with the OK code.</li></ol>

@return CRYS_OK on success.
@return a non-zero value from crys_hmac_error.h on failure.
*/

int lega_CRYS_HMAC_Free(
                        CRYS_HMACUserContext_t  *ContextID_ptr         /*!< [in]  Pointer to the HMAC context buffer allocated by the user, which is used for
                                          the HMAC machine operation. */
);


/*!
@brief This function processes a single buffer of data, and returns the data buffer's message digest.

@return CRYS_OK on success.
@return A non-zero value from crys_hmac_error.h on failure.
*/
int lega_CRYS_HMAC(
                        CRYS_HASH_OperationMode_t  OperationMode,       /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
                        uint8_t                    *key_ptr,            /*!< [in]  The pointer to the user's key buffer. */
                        uint16_t                    keySize,            /*!< [in]  The key size in bytes. */
                        uint8_t                    *DataIn_ptr,         /*!< [in]  Pointer to the input data to be HASHed.
                                                                                   The size of the scatter/gather list representing the data buffer is limited to 128
                                           entries, and the size of each entry is limited to 64KB (fragments larger than
                                           64KB are broken into fragments <= 64KB). */
                        size_t                    DataSize,           /*!< [in]  The size of the data to be hashed (in bytes). */
                        CRYS_HASH_Result_t          HmacResultBuff      /*!< [out] Pointer to the word-aligned 64 byte buffer. The actual size of the
                                           HMAC result depends on CRYS_HASH_OperationMode_t. */
);
#endif //_LEGA_HMAC_H_