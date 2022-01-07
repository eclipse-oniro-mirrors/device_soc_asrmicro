#include "lega_hash.h"
#include "lega_hw_common.h"
int lega_hash(
                        CRYS_HASH_OperationMode_t  OperationMode,       /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
                        uint8_t                   *DataIn_ptr,          /*!< [in]  Pointer to the input data to be HASHed.
                                                                                   The size of the scatter/gather list representing the data buffer is limited
                                           to 128 entries, and the size of each entry is limited to 64KB
                                           (fragments larger than 64KB are broken into fragments <= 64KB). */
                        size_t                   DataSize,            /*!< [in]  The size of the data to be hashed in bytes. */
                        CRYS_HASH_Result_t         HashResultBuff       /*!< [out] Pointer to a word-aligned 64 byte buffer. The actual size of the HASH
                                           result depends on CRYS_HASH_OperationMode_t. */
)
{
    int ret, ret0;
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    ret0 = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret0 != 0)
        printf("hash lock err\r\n");
#endif
    ret = (int)CRYS_HASH  (OperationMode, DataIn_ptr, DataSize, HashResultBuff);
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    ret0 = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret0 != 0)
        printf("hash unlock err\r\n");
#endif
    return ret;
}
int lega_hash_init(
                        CRYS_HASHUserContext_t     *ContextID_ptr,         /*!< [in]  Pointer to the HASH context buffer allocated by the user that is used
                                        for the HASH machine operation. */
                        CRYS_HASH_OperationMode_t  OperationMode           /*!< [in]  One of the supported HASH modes, as defined in CRYS_HASH_OperationMode_t. */
)
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret != 0)
        printf("hash lock err\r\n");
#endif
    return (int)CRYS_HASH_Init(ContextID_ptr, OperationMode);
}

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
)
{
    return (int)CRYS_HASH_Update(ContextID_ptr, DataIn_ptr, DataInSize);
}

int lega_hash_finish(
                        CRYS_HASHUserContext_t  *ContextID_ptr,         /*!< [in]  Pointer to the HASH context buffer allocated by the user that is used for
                                           the HASH machine operation. */
                        CRYS_HASH_Result_t       HashResultBuff         /*!< [in]  Pointer to the word-aligned 64 byte buffer. The actual size of the HASH
                                           result depends on CRYS_HASH_OperationMode_t. */
)
{
    return CRYS_HASH_Finish(ContextID_ptr, HashResultBuff);
}

void lega_hash_deinit()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret != 0)
        printf("hash unlock err\r\n");
#endif
}