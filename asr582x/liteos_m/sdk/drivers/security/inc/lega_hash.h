#ifndef _LEGA_HASH_H_
#define _LEGA_HASH_H_
#include "lega_hw_common.h"
#include "crys_hash.h"
int lega_hash(
                        CRYS_HASH_OperationMode_t  OperationMode,       /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
                        uint8_t                   *DataIn_ptr,          /*!< [in]  Pointer to the input data to be HASHed.
                                                                                   The size of the scatter/gather list representing the data buffer is limited
                                           to 128 entries, and the size of each entry is limited to 64KB
                                           (fragments larger than 64KB are broken into fragments <= 64KB). */
                        size_t                   DataSize,            /*!< [in]  The size of the data to be hashed in bytes. */
                        CRYS_HASH_Result_t         HashResultBuff       /*!< [out] Pointer to a word-aligned 64 byte buffer. The actual size of the HASH
                                           result depends on CRYS_HASH_OperationMode_t. */
);
int lega_hash_init(
                        CRYS_HASHUserContext_t     *ContextID_ptr,         /*!< [in]  Pointer to the HASH context buffer allocated by the user that is used
                                        for the HASH machine operation. */
                        CRYS_HASH_OperationMode_t  OperationMode           /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
);
int lega_hash_update(
                        CRYS_HASHUserContext_t  *ContextID_ptr,         /*!< [in]  Pointer to the HASH context buffer allocated by the user, which is used for the
                                           HASH machine operation. */
                        uint8_t                 *DataIn_ptr,            /*!< [in]  Pointer to the input data to be HASHed.
                                                                                   The size of the scatter/gather list representing the data buffer is limited to
                                           128 entries, and the size of each entry is limited to 64KB
                                           (fragments larger than 64KB are broken into fragments <= 64KB). */
                        size_t                 DataInSize             /*!< [in]  Byte size of the input data. Must be > 0.
                                                                                    If not a multiple of the HASH block size (64 for MD5, SHA-1 and SHA-224/256,
                                            128 for SHA-384/512), no further calls
                                                                                    to CRYS_HASH_Update are allowed in this context, and only CRYS_HASH_Finish
                                            can be called to complete the computation. */
);
int lega_hash_finish(
                        CRYS_HASHUserContext_t  *ContextID_ptr,         /*!< [in]  Pointer to the HASH context buffer allocated by the user that is used for
                                           the HASH machine operation. */
                        CRYS_HASH_Result_t       HashResultBuff         /*!< [in]  Pointer to the word-aligned 64 byte buffer. The actual size of the HASH
                                           result depends on CRYS_HASH_OperationMode_t. */
);

void lega_hash_deinit();

#endif //_LEGA_HASH_H_