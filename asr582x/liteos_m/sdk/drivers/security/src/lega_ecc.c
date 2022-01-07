#include "lega_ecc.h"
#include "lega_hw_common.h"
void lega_ecc_init()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret != 0)
        printf("ecc lock err\r\n");
#endif
}

void lega_ecc_deinit()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret != 0)
        printf("ecc unlock err\r\n");
#endif
}
const CRYS_ECPKI_Domain_t *lega_ecpki_getEcDomain(CRYS_ECPKI_DomainID_t domainId)
{
    return (CRYS_ECPKI_Domain_t *)CRYS_ECPKI_GetEcDomain(domainId);
}

int lega_ecpki_genkeypair(CRYS_RND_Context_t         *pRndContext,  /*in/out*/
                          const CRYS_ECPKI_Domain_t  *pDomain,      /*in*/
                          CRYS_ECPKI_UserPrivKey_t   *pUserPrivKey, /*out*/
                          CRYS_ECPKI_UserPublKey_t   *pUserPublKey, /*out*/
                          CRYS_ECPKI_KG_TempData_t   *pTempBuff,    /*in*/
                          CRYS_ECPKI_KG_FipsContext_t *pFipsCtx)    /*in*/
{
    return (int)CRYS_ECPKI_GenKeyPair(pRndContext, pDomain, pUserPrivKey, pUserPublKey, pTempBuff, pFipsCtx);
}

int lega_ecpki_buildprivkey(const CRYS_ECPKI_Domain_t *pDomain,        /*in */
                            const uint8_t             *pPrivKeyIn,     /*in*/
                            uint32_t                   privKeySizeInBytes,/*in*/
                            CRYS_ECPKI_UserPrivKey_t  *pUserPrivKey    /*out*/)
{
    return (int)CRYS_ECPKI_BuildPrivKey(pDomain, pPrivKeyIn, privKeySizeInBytes, pUserPrivKey);
}

int lega_dx_ecpki_buildpubkey(const CRYS_ECPKI_Domain_t   *pDomain,           /*in*/
                           uint8_t                     *pPublKeyIn,         /*in*/
                           uint32_t                     publKeySizeInBytes, /*in*/
                           EC_PublKeyCheckMode_t        checkMode,          /*in*/
                           CRYS_ECPKI_UserPublKey_t    *pUserPublKey,       /*out*/
                           CRYS_ECPKI_BUILD_TempData_t *tempBuff           /*in*/)

{
    return (int)_DX_ECPKI_BuildPublKey(pDomain, pPublKeyIn, publKeySizeInBytes, checkMode, pUserPublKey, tempBuff);
}


int lega_ecdh_svdp_dh(CRYS_ECPKI_UserPublKey_t *PartnerPublKey_ptr,        /*in*/
                      CRYS_ECPKI_UserPrivKey_t *UserPrivKey_ptr,           /*in*/
                      uint8_t                  *SharedSecretValue_ptr,     /*out*/
                      uint32_t                 *SharedSecrValSize_ptr,     /*in/out*/
                      CRYS_ECDH_TempData_t     *TempBuff_ptr               /*in*/ )
{
    return (int)CRYS_ECDH_SVDP_DH(PartnerPublKey_ptr, UserPrivKey_ptr, SharedSecretValue_ptr, SharedSecrValSize_ptr, TempBuff_ptr);
}

int lega_ecdsa_sign(CRYS_RND_Context_t              *pRndContext,           /*in/out*/
                    CRYS_ECDSA_SignUserContext_t    *pSignUserContext,      /*in/out*/
                    CRYS_ECPKI_UserPrivKey_t        *pSignerPrivKey,        /*in*/
                    CRYS_ECPKI_HASH_OpMode_t         hashMode,              /*in*/
                    uint8_t                         *pMessageDataIn,        /*in*/
                    uint32_t                         messageSizeInBytes,    /*in*/
                    uint8_t                         *pSignOut,              /*out*/
                    uint32_t                        *pSignOutSize           /*in*/)
{
    return (int)CRYS_ECDSA_Sign(pRndContext, pSignUserContext, pSignerPrivKey, hashMode, pMessageDataIn, messageSizeInBytes, pSignOut, pSignOutSize);
}

int lega_ecdsa_verify (
                                        CRYS_ECDSA_VerifyUserContext_t *pVerifyUserContext,  /*in/out*/
                                        CRYS_ECPKI_UserPublKey_t       *pUserPublKey,        /*in*/
                                        CRYS_ECPKI_HASH_OpMode_t        hashMode,            /*in*/
                                        uint8_t                        *pSignatureIn,        /*in*/
                                        uint32_t                        SignatureSizeBytes,  /*in*/
                                        uint8_t                        *pMessageDataIn,      /*in*/
                                        uint32_t                        messageSizeInBytes  /*in*/)
{
    return (int)CRYS_ECDSA_Verify (pVerifyUserContext, pUserPublKey, hashMode, pSignatureIn, SignatureSizeBytes, pMessageDataIn, messageSizeInBytes);
}