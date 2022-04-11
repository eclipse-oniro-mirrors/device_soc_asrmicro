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

#ifndef _LEGA_RND_H_
#define _LEGA_RND_H_

/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/
void lega_rnd_init(void);
void lega_rnd_deinit(void);

/*!
@brief This function is used for reseeding the RNG with additional entropy and additional user-provided input.
(additional data should be provided by calling ::CRYS_RND_AddAdditionalInput prior to using this API).
It implements referenced standard [SP800-90] - 10.2.1.4.2 - CTR-DRBG Reseeding algorithm, using AES (FIPS-PUB 197) and Derivation Function (DF).

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int lega_RND_Reseeding(
    uint8_t   *rndContext_ptr,      /* !< [in/out] Pointer to the RND context buffer. */
    uint8_t  *rndWorkBuff_ptr      /* !< [in/out] Scratchpad for the RND module's work. */
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
int lega_RND_GenerateVector(
    uint8_t *rndState_ptr,     /* !< [in/out] Pointer to the RND state structure, which is part of the RND context structure.
                                     Use rndContext->rndState field of the context for this parameter. */
    uint16_t
    outSizeBytes,            /* !< [in]  The size in bytes of the random vector required. The maximal size is 2^16 -1 bytes. */
    uint8_t   *out_ptr                  /* !< [out] The pointer to output buffer. */
);

/*************************************************************************************/
/*!
@brief Used for adding additional input/personalization data provided by the user,
to be later used by the ::CRYS_RND_Instantiation/::CRYS_RND_Reseeding/::CRYS_RND_GenerateVector functions.

@return CRYS_OK on success.
@return A non-zero value from crys_rnd_error.h on failure.
*/
int lega_RND_AddAdditionalInput(
    uint8_t *rndContext_ptr,     /* !< [in/out] Pointer to the RND context buffer. */
    uint8_t *additonalInput_ptr,            /* !< [in]  The Additional Input buffer. */
    uint16_t additonalInputSize             /* !< [in]  The size of the Additional Input buffer. Must be <= 48, and a multiple of 4. */
);

#endif // _LEGA_RND_H_