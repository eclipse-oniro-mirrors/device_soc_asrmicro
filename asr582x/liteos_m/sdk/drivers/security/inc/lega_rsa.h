#ifndef _LEGA_RSA_H_
#define _LEGA_RSA_H_

#include "crys_rsa_types.h"
#include "crys_rsa_kg.h"
#include "crys_rsa_build.h"
#include "crys_rsa_schemes.h"
#include "crys_rsa_prim.h"
#include "sns_silib.h"
#define TST_MAX_PRVT_EXP_SIZE_IN_BYTES              256
typedef enum RSAType_enum
{
     NON_CRT_MODE   ,
     CRT_MODE  ,
     RSATypeLast= 0x7FFFFFFF,
}RSAType_enum;

void lega_rsa_init();
void lega_rsa_deinit();

int lega_rsa_keypair_gen(
                    CRYS_RND_Context_t *rndContext_ptr,
                    uint8_t             *pubExp_ptr,
                    uint16_t             pubExpSizeInBytes,
                    uint32_t             keySize,
                    CRYS_RSAUserPrivKey_t *userPrivKey_ptr,
                    CRYS_RSAUserPubKey_t  *userPubKey_ptr,
                    CRYS_RSAKGData_t      *keyGenData_ptr,
                    CRYS_RSAKGFipsContext_t    *pFipsCtx);
int lega_rsa_keypair_crt_gen(
                                        CRYS_RND_Context_t *rndContext_ptr,        /*!< [in/out] Pointer to the RND context buffer. */
                                        uint8_t             *pubExp_ptr,           /*!< [in]  The pointer to the public exponent (public key). */
                                        uint16_t             pubExpSizeInBytes,    /*!< [in]  The public exponent size in bytes. */
                                        uint32_t             keySize,              /*!< [in]  The size of the key, in bits. Supported sizes are
                                                                                              512, 1024, 2048, 3072 and 4096 bit. */
                                        CRYS_RSAUserPrivKey_t *userPrivKey_ptr,    /*!< [out] Pointer to the private key structure. */
                                        CRYS_RSAUserPubKey_t  *userPubKey_ptr,     /*!< [out] Pointer to the public key structure. */
                                        CRYS_RSAKGData_t      *keyGenData_ptr,     /*!< [in] Pointer to a temporary structure required for the KeyGen operation. */
                                        CRYS_RSAKGFipsContext_t    *pFipsCtx       /*!< [in] Pointer to temporary buffer used in case FIPS certification if required. */);
int lega_RSA_Get_PubKey(
                                CRYS_RSAUserPubKey_t *UserPubKey_ptr,   /*!< [in] A pointer to the public key structure. */
                                uint8_t  *Exponent_ptr,                 /*!< [out] A pointer to the exponent stream of bytes (Big-Endian format). */
                                uint16_t *ExponentSize_ptr,             /*!< [in/out] the size of the exponent buffer in bytes,
                                              it is updated to the actual size of the exponent, in bytes. */
                                uint8_t  *Modulus_ptr,                  /*!< [out] A pointer to the modulus stream of bytes (Big-Endian format).
                                           The MS (most significant) bit must be set to '1'. */
                                uint16_t *ModulusSize_ptr               /*!< [in/out] the size of the modulus buffer in bytes, it is updated to the actual
                                              size of the modulus, in bytes. */
);

#if !defined(_INTERNAL_CRYS_NO_RSA_DECRYPT_SUPPORT) && !defined(_INTERNAL_CRYS_NO_RSA_SIGN_SUPPORT)
int lega_RSA_Get_PrivKey(CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
                       uint8_t             *PrivExponent_ptr,
                       uint16_t            *PrivExponentSize_ptr,
                       uint8_t             *PubExponent_ptr,
                       uint16_t            *PubExponentSize_ptr,
                       uint8_t             *Modulus_ptr,
                       uint16_t            *ModulusSize_ptr );
int lega_RSA_Get_PrivKeyCRT(CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
                          uint8_t *P_ptr,
                          uint16_t *PSize_ptr,
                          uint8_t *Q_ptr,
                          uint16_t *QSize_ptr,
                          uint8_t *dP_ptr,
                          uint16_t *dPSize_ptr,
                          uint8_t *dQ_ptr,
                          uint16_t *dQSize_ptr,
                          uint8_t *qInv_ptr,
                          uint16_t *qInvSize_ptr);
int lega_RSA_Get_PrivKeyModulus(CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
                          uint8_t *N_ptr,
                          uint16_t *NSize_ptr);
#endif //!defined(_INTERNAL_CRYS_NO_RSA_DECRYPT_SUPPORT) && !defined(_INTERNAL_CRYS_NO_RSA_SIGN_SUPPORT)

int lega_rsa_build_pubkey(
                                    CRYS_RSAUserPubKey_t *UserPubKey_ptr,   /*!< [out] Pointer to the public key structure. */
                                    uint8_t *Exponent_ptr,                  /*!< [in]  Pointer to the exponent stream of bytes (Big-Endian format). */
                                    uint16_t ExponentSize,                  /*!< [in]  The size of the exponent (in bytes). */
                                    uint8_t *Modulus_ptr,                   /*!< [in]  Pointer to the modulus stream of bytes (Big-Endian format).
                                               The most significant bit (MSB) must be set to '1'. */
                                    uint16_t ModulusSize                    /*!< [in]  The modulus size in bytes. Supported sizes are 64, 128, 256, 384 and 512. */
);
int lega_rsa_build_prikey(
                                    CRYS_RSAUserPrivKey_t   *UserPrivKey_ptr,   /*!< [out] Pointer to the public key structure.*/
                                    uint8_t               *PrivExponent_ptr,    /*!< [in]  Pointer to the private exponent stream of bytes (Big-Endian format). */
                                    uint16_t               PrivExponentSize,    /*!< [in]  The size of the private exponent (in bytes). */
                                    uint8_t               *PubExponent_ptr,     /*!< [in]  Pointer to the public exponent stream of bytes (Big-Endian format). */
                                    uint16_t               PubExponentSize,     /*!< [in]  The size of the public exponent (in bytes). */
                                    uint8_t               *Modulus_ptr,         /*!< [in]  Pointer to the modulus stream of bytes (Big-Endian format).
                                               The most significant bit must be set to '1'. */
                                    uint16_t               ModulusSize          /*!< [in]  The modulus size in bytes. Supported sizes are 64, 128, 256, 384 and 512. */
);
int lege_rsa_build_prikey_crt(
                                      CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,   /*!< [out] Pointer to the public key structure. */
                                      uint8_t *P_ptr,                           /*!< [in]  Pointer to the first factor stream of bytes (Big-Endian format). */
                                      uint16_t PSize,                           /*!< [in]  The size of the first factor (in bytes). */
                                      uint8_t *Q_ptr,                           /*!< [in]  Pointer to the second factor stream of bytes (Big-Endian format). */
                                      uint16_t QSize,                           /*!< [in]  The size of the second factor (in bytes). */
                                      uint8_t *dP_ptr,                          /*!< [in]  Pointer to the first factor's CRT exponent stream of bytes
                                               (Big-Endian format). */
                                      uint16_t dPSize,                          /*!< [in]  The size of the first factor's CRT exponent (in bytes). */
                                      uint8_t *dQ_ptr,                          /*!< [in]  Pointer to the second factor's CRT exponent stream of bytes
                                               (Big-Endian format). */
                                      uint16_t dQSize,                          /*!< [in]  The size of the second factor's CRT exponent (in bytes). */
                                      uint8_t *qInv_ptr,                        /*!< [in]  Pointer to the first CRT coefficient stream of bytes (Big-Endian format). */
                                      uint16_t qInvSize                         /*!< [in]  The size of the first CRT coefficient (in bytes). */
);
int lega_rsa_sign(
                   CRYS_RND_Context_t *rndContext_ptr,              /*!< [in/out] Pointer to the RND context buffer. */
                   CRYS_RSAPrivUserContext_t *UserContext_ptr,      /*!< [in]  Pointer to a temporary context for internal use. */
                   CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,          /*!< [in]  Pointer to the private key data structure of the user.
                                                                               The representation (pair or quintuple) and hence the algorithm (CRT or not CRT)
                                           is determined by the Private Key build function -
                                                                               ::CRYS_RSA_Build_PrivKey or ::CRYS_RSA_Build_PrivKeyCRT. */
                   CRYS_RSA_HASH_OpMode_t rsaHashMode,              /*!< [in]  One of the supported SHA-x HASH modes, as defined in ::CRYS_RSA_HASH_OpMode_t.
                                           (MD5 is not supported). */
                   CRYS_PKCS1_MGF_t MGF,                            /*!< [in]  The mask generation function. [PKCS1_2.1] defines only MGF1, so the only value
                                           allowed for [PKCS1_2.1] is CRYS_PKCS1_MGF1. */
                   uint16_t     SaltLen,                            /*!< [in]  The Length of the Salt buffer (relevant for PKCS#1 Ver 2.1 only, typically lengths is 0 or hash Len).
                                                                               FIPS 186-4 requires, that SaltLen <= hash len. If SaltLen > KeySize - hash Len - 2, the function
                                                                               returns an error.  */
                   uint8_t     *DataIn_ptr,                         /*!< [in]  Pointer to the input data to be signed.
                                                                               The size of the scatter/gather list representing the data buffer is limited to 128
                                           entries, and the size of each entry is limited to 64KB (fragments larger than
                                           64KB are broken into fragments <= 64KB). */
                   uint32_t     DataInSize,                         /*!< [in]  The size (in bytes) of the data to sign. */
                   uint8_t     *Output_ptr,                         /*!< [out] Pointer to the signature. The buffer must be at least PrivKey_ptr->N.len bytes
                                           long (i.e. the modulus size in bytes). */
                   uint16_t    *OutputSize_ptr,                     /*!< [in/out] Pointer to the signature size value - the input value is the signature
                                          buffer size allocated, the output value is the signature size used.
                                                                                  he buffer must be equal to PrivKey_ptr->N.len bytes long
                                          (i.e. the modulus size in bytes). */
                   CRYS_PKCS1_version PKCS1_ver                     /*!< [in]  [PKCS1_1.5] or [PKCS1_2.1], according to the functionality required. */
);
#define lega_rsa_pss_sign(rndContext_ptr, UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
        lega_rsa_sign(rndContext_ptr, UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define lega_rsa_pkcs1v15_sign(rndContext_ptr, UserContext_ptr,UserPrivKey_ptr,hashFunc,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
        lega_rsa_sign(rndContext_ptr, (UserContext_ptr),(UserPrivKey_ptr),(hashFunc),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),(DataInSize),(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)

int lega_rsa_verify(
                            CRYS_RSAPubUserContext_t *UserContext_ptr,      /*!< [in]  Pointer to a temporary context for internal use. */
                            CRYS_RSAUserPubKey_t *UserPubKey_ptr,           /*!< [in]  Pointer to the public key data structure of the user. */
                            CRYS_RSA_HASH_OpMode_t rsaHashMode,             /*!< [in]  One of the supported SHA-x HASH modes, as defined in ::CRYS_RSA_HASH_OpMode_t.
                                               (MD5 is not supported). */
                            CRYS_PKCS1_MGF_t MGF,                           /*!< [in]  The mask generation function. [PKCS1_2.1] defines only MGF1, so the only
                                               value allowed for [PKCS_2.1] is CRYS_PKCS1_MGF1. */
                            uint16_t SaltLen,                               /*!< [in]  The Length of the Salt buffer. Relevant only for [PKCS1_2.1].
                                               Typical lengths are 0 or hash Len (20 for SHA-1).
                                                                                       The maximum length allowed is [modulus size - hash Len - 2]. */
                            uint8_t     *DataIn_ptr,                        /*!< [in]  Pointer to the input data to be verified.
                                                                                        The size of the scatter/gather list representing the data buffer is
                                            limited to 128 entries, and the size of each entry is limited to 64KB
                                            (fragments larger than 64KB are broken into fragments <= 64KB). */
                            uint32_t     DataInSize,                        /*!< [in]  The size (in bytes) of the data whose signature is to be verified. */
                            uint8_t     *Sig_ptr,                           /*!< [in]  Pointer to the signature to be verified.
                                                                                       The length of the signature is PubKey_ptr->N.len bytes
                                               (i.e. the modulus size in bytes). */
                            CRYS_PKCS1_version PKCS1_ver                    /*!< [in]  [PKCS1_1.5] or [PKCS1_2.1], according to the functionality required. */
);

#define lega_rsa_pss_verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr)\
        lega_rsa_verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER21)
#define lega_rsa_pkcs1v15_verify(UserContext_ptr,UserPubKey_ptr,hashFunc,DataIn_ptr,DataInSize,Sig_ptr)\
        lega_rsa_verify(UserContext_ptr,UserPubKey_ptr,hashFunc,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER15)
int lega_rsa_encrypt(
                        CRYS_RND_Context_t *rndContext_ptr,         /*!< [in/out] Pointer to the RND context buffer. */
                        CRYS_RSAUserPubKey_t  *UserPubKey_ptr,      /*!< [in]  Pointer to the public key data structure. */
                        CRYS_RSAPrimeData_t   *PrimeData_ptr,       /*!< [in]  Pointer to a temporary structure that is internally used as workspace for the
                                           Encryption operation. */
                        CRYS_RSA_HASH_OpMode_t hashFunc,            /*!< [in]  The HASH function to be used. One of the supported SHA-x HASH modes, as defined
                                           in ::CRYS_RSA_HASH_OpMode_t (MD5 is not supported).*/
                        uint8_t  *L,                                /*!< [in]  The label input pointer. Relevant for [PKCS1_2.1] only. NULL by default.
                                           NULL for [PKCS1_1.5]. */
                        uint16_t  Llen,                             /*!< [in]  The label length. Relevant for [PKCS1_2.1] only. Zero by default.
                                           Must be <=2048. Zero for [PKCS1_1.5]. */
                        CRYS_PKCS1_MGF_t MGF,                       /*!< [in]  The mask generation function. [PKCS1_2.1] defines MGF1, so the only value
                                           allowed here is CRYS_PKCS1_MGF1. */
                        uint8_t   *DataIn_ptr,                      /*!< [in]  Pointer to the data to encrypt. */
                        uint16_t   DataInSize,                      /*!< [in]  The size (in bytes) of the data to encrypt. The data size must be:
                                                                                - For [PKCS1_2.1], DataSize <= modulus size - 2*HashLen - 2.
                                                                                - For [PKCS1_1.5], DataSize <= modulus size - 11. */
                        uint8_t   *Output_ptr,                      /*!< [out] Pointer to the encrypted data. The buffer must be at least modulus size bytes long. */
                        CRYS_PKCS1_version  PKCS1_ver               /*!< [in]  [PKCS1_1.5] or [PKCS1_2.1], according to the functionality required. */
);
#define lega_rsa_oaep_encrypt(rndContext_ptr, UserPubKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr)\
        lega_rsa_encrypt(rndContext_ptr, UserPubKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,CRYS_PKCS1_VER21)

#define lega_rsa_pkcs1v15_encrypt(rndContext_ptr, UserPubKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr)\
        lega_rsa_encrypt(rndContext_ptr, UserPubKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr, DataInSize, Output_ptr,CRYS_PKCS1_VER15)
int lega_rsa_decrypt(
                                CRYS_RSAUserPrivKey_t  *UserPrivKey_ptr,    /*!< [in]  Pointer to the private key data structure of the user. */
                                CRYS_RSAPrimeData_t    *PrimeData_ptr,      /*!< [in]  Pointer to a temporary structure that is internally used as workspace
                                               for the decryption operation. */
                                CRYS_RSA_HASH_OpMode_t  hashFunc,           /*!< [in]  The HASH function to be used. One of the supported SHA-x HASH modes,
                                               as defined in ::CRYS_RSA_HASH_OpMode_t (MD5 is not supported). */
                                uint8_t                 *L,                 /*!< [in]  The label input pointer. Relevant for [PKCS1_2.1] only. NULL by default.
                                               NULL for [PKCS1_1.5]. */
                                uint16_t              Llen,                 /*!< [in]  The label length. Relevant for [PKCS1_2.1] only. Zero by default.
                                               Zero for [PKCS1_1.5]. */
                                CRYS_PKCS1_MGF_t        MGF,                /*!< [in]  The mask generation function. [PKCS1_2.1] defines MGF1, so the only
                                               value allowed here is CRYS_PKCS1_MGF1. */
                                uint8_t              *DataIn_ptr,           /*!< [in]  Pointer to the data to decrypt. */
                                uint16_t              DataInSize,           /*!< [in]  The size (in bytes) of the data to decrypt. DataSize must be &le;
                                               the modulus size. */
                                uint8_t              *Output_ptr,           /*!< [in]  Pointer to the decrypted data. The buffer must be at least
                                               PrivKey_ptr->N.len bytes long (i.e. the modulus size in bytes). */
                                uint16_t             *OutputSize_ptr,       /*!< [in]  Pointer to the byte size of the buffer pointed to by Output_buffer.
                                               The size must be:
                                                                                       <ul><li> For PKCS #1 v2.1: Modulus size > OutputSize >=
                                                (modulus size - 2*HashLen - 2).</li>
                                                                                       <li> For PKCS #1 v1.5: Modulus size > OutputSize >= (modulus size - 11).
                                                                                        The value pointed by OutputSize_ptr is updated after decryption with
                                            the actual number of bytes that are loaded to Output_ptr.</li></ul> */
                                CRYS_PKCS1_version      PKCS1_ver           /*!< [in]  [PKCS1_1.5] or [PKCS1_2.1], according to the functionality required. */
);

#define lega_rsa_oaep_decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr)\
        lega_rsa_decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)


#define lega_rsa_pkcs1v15_decrypt(UserPrivKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
        lega_rsa_decrypt(UserPrivKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER15)

#endif //_LEGA_RSA_H_