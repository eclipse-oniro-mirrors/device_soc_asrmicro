/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DUET_RND_H_
#define _DUET_RND_H_

#include "sns_silib.h"
#include "crys_rnd.h"
#include "crys_error.h"
/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/

/*!
@brief This function initializes the RND context.
It must be called at least once prior to using this context with any API that requires it as a parameter (e.g., other RND APIs, asymmetric
cryptography key generation and signatures).
It is called as part of ARM TrustZone CryptoCell library initialization, which initializes and returns the primary RND context.
This primary context can be used as a single global context for all RND needs.
Alternatively, other contexts may be initialized and used with a more limited scope (for specific applications or specific threads).
The call to this function must be followed by a call to ::CRYS_RND_SetGenerateVectorFunc API to set the generate vector function.
It implements referenced standard [SP800-90] - 10.2.1.3.2 - CTR-DRBG Instantiate algorithm using AES (FIPS-PUB 197) and Derivation Function (DF).
\note Additional data can be mixed with the random seed (personalization data or nonce). If required, this data should be provided by calling ::CRYS_RND_AddAdditionalInput prior to using this API.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_Instantiation(CRYS_RND_Context_t
                           *rndContext_ptr,       /* !< [in/out]  Pointer to the RND context buffer allocated by the user, which is used to
                                           maintain the RND state, as well as pointers to the functions used for
                                           random vector generation. This context must be saved and provided as a
                                           parameter to any API that uses the RND module.
                                           \note the context must be cleared before sent to the function. */
                           CRYS_RND_WorkBuff_t *rndWorkBuff_ptr       /* !< [in/out] Scratchpad for the RND module's work. */
                         );

/*!
@brief Clears existing RNG instantiation state.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_UnInstantiation(CRYS_RND_Context_t *rndContext_ptr);       /* !< [in/out] Pointer to the RND context
 * buffer. */

/*!
@brief This function is used for reseeding the RNG with additional entropy and additional user-provided input.
(additional data should be provided by calling ::CRYS_RND_AddAdditionalInput prior to using this API).
It implements referenced standard [SP800-90] - 10.2.1.4.2 - CTR-DRBG Reseeding algorithm, using AES (FIPS-PUB 197) and Derivation Function (DF).

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_Reseeding(CRYS_RND_Context_t *rndContext_ptr,      /* !< [in/out] Pointer to the RND context buffer. */
                       CRYS_RND_WorkBuff_t *rndWorkBuff_ptr      /* !< [in/out] Scratchpad for the RND module's work. */
                     );

/****************************************************************************************/
/*!
@brief Generates a random vector according to the algorithm defined in referenced standard [SP800-90] - 10.2.1.5.2 - CTR-DRBG.
The generation algorithm uses AES (FIPS-PUB 197) and Derivation Function (DF).

\note
<ul id="noteb"><li> The RND module must be instantiated prior to invocation of this API.</li>
<li> In the following cases, Reseeding operation must be performed prior to vector generation:</li>
    <ul><li> Prediction resistance is required.</li>
    <li> The function returns CRYS_RND_RESEED_COUNTER_OVERFLOW_ERROR, stating that the Reseed Counter has passed its upper-limit (2^32-2).</li></ul></ul>

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_GenerateVector(CRYS_RND_State_t
                            *rndState_ptr,     /* !< [in/out] Pointer to the RND state structure, which is part of the RND context structure.
                                     Use rndContext->rndState field of the context for this parameter. */
                            uint16_t outSizeBytes,            /* !< [in]  The size in bytes of the random vector required. The maximal size is 2^16 -1 bytes. */
                            uint8_t *out_ptr                  /* !< [out] The pointer to output buffer. */
                          );

/****************************************************************************************/
/*!

@brief This function sets the RND vector generation function into the RND context.

It must be called after ::CRYS_RND_Instantiation, and prior to any other API that requires the RND context as parameter.
It is called as part of ARM TrustZone CryptoCell library initialization, to set the RND vector generation function into the primary RND context,
after ::CRYS_RND_Instantiation is called.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_SetGenerateVectorFunc(CRYS_RND_Context_t
                                   *rndContext_ptr,                     /* !< [in/out] Pointer to the RND context buffer allocated by the user,
                                              which is used to maintain the RND state as well as pointers
                                              to the functions used for random vector generation. */
                                   SaSiRndGenerateVectWorkFunc_t rndGenerateVectFunc       /* !< [in] Pointer to the random vector generation function.
                                              The pointer should point to the ::CRYS_RND_GenerateVector function. */
                                 );

/**********************************************************************************************************/
/*!
@brief Generates a random vector with specific limitations by testing candidates (described and used in FIPS 186-4: B.1.2, B.4.2 etc.).

This function will draw a random vector, compare it to the range limits, and if within range - return it in rndVect_ptr.
If outside the range, the function will continue retrying until a conforming vector is found, or the maximal retries limit is exceeded.
If maxVect_ptr is provided, rndSizeInBits specifies its size, and the output vector must conform to the range [1 < rndVect < maxVect_ptr].
If maxVect_ptr is NULL, rndSizeInBits specifies the exact required vector size, and the output vector must be the exact same
bit size (with its most significant bit = 1).
\note
The RND module must be instantiated prior to invocation of this API.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_GenerateVectorInRange(
    CRYS_RND_Context_t *rndContext_ptr,     /* !< [in/out] Pointer to the RND context buffer. */
    uint32_t rndSizeInBits,                /* !< [in]  The size in bits of the random vector required. The allowed size in range  2 <= rndSizeInBits < 2^19-1, bits. */
    uint8_t *maxVect_ptr,                  /* !< [in]  Pointer to the vector defining the upper limit for the random vector output, Given as little-endian byte array.
                                                                       If not NULL, its actual size is treated as [(rndSizeInBits+7)/8] bytes. */
    uint8_t *rndVect_ptr                   /* !< [in/out] Pointer to the output buffer for the random vector. Must be at least [(rndSizeInBits+7)/8] bytes.
                                                                 Treated as little-endian byte array. */
);

/*************************************************************************************/
/*!
@brief Used for adding additional input/personalization data provided by the user,
to be later used by the ::CRYS_RND_Instantiation/::CRYS_RND_Reseeding/::CRYS_RND_GenerateVector functions.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_AddAdditionalInput(CRYS_RND_Context_t
                                *rndContext_ptr,     /* !< [in/out] Pointer to the RND context buffer. */
                                uint8_t *additonalInput_ptr,            /* !< [in]  The Additional Input buffer. */
                                uint16_t additonalInputSize             /* !< [in]  The size of the Additional Input buffer. Must be <= 48, and a multiple of 4. */
                              );

/*!
@brief The CRYS_RND_EnterKatMode function sets KAT mode bit into StateFlag of global CRYS_RND_WorkingState structure.

The user must call this function before calling functions performing KAT tests.

\note Total size of entropy and nonce must be not great than: ::CRYS_RND_MAX_KAT_ENTROPY_AND_NONCE_SIZE, defined.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int duet_RND_EnterKatMode(CRYS_RND_Context_t *rndContext_ptr,     /* !< [in/out] Pointer to the RND context buffer. */
                          uint8_t *entrData_ptr,       /* !< [in] Entropy data. */
                          uint32_t entrSize,           /* !< [in] Entropy size in bytes. */
                          uint8_t *nonce_ptr,          /* !< [in] Nonce. */
                          uint32_t nonceSize,          /* !< [in] Entropy size in bytes. */
                          CRYS_RND_WorkBuff_t
                          *workBuff_ptr      /* !< [out] RND working buffer, must be the same buffer, which should be passed into
                            Instantiation/Reseeding functions. */
                        );

/**********************************************************************************************************/
/*!
@brief The CRYS_RND_DisableKatMode function disables KAT mode bit into StateFlag of global CRYS_RND_WorkingState structure.

The user must call this function after KAT tests before actual using RND module (Instantiation etc.).

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
void duet_RND_DisableKatMode(CRYS_RND_Context_t *rndContext_ptr     /* !< [in/out] Pointer to the RND context buffer. */
                           );

#endif // _DUET_RND_H_