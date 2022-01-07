/****************************************************************************
* This confidential and proprietary software may be used only as authorized *
* by a licensing agreement from ARM Israel.                                 *
* Copyright (C) 2015 ARM Limited or its affiliates. All rights reserved.    *
* The entire notice above must be reproduced on all authorized copies and   *
* copies may only be made to the extent permitted by a licensing agreement  *
* from ARM Israel.                                                          *
*****************************************************************************/


#ifndef CRYS_ECPKI_DOMAIN_H
#define CRYS_ECPKI_DOMAIN_H


/*!
@file
@brief Defines the ecpki build domain API.
*/


#include "crys_error.h"
#include "crys_ecpki_types.h"

#ifdef __cplusplus
extern "C"
{
#endif




/**********************************************************************************
 *                CRYS_ECPKI_GetEcDomain function                     *
 **********************************************************************************/

/*!
 * @brief  The function returns a pointer to an ECDSA saved domain (one of the supported domains).
 *
 * @return Domain pointer on success.
 * @return NULL on failure.
 */

const CRYS_ECPKI_Domain_t *CRYS_ECPKI_GetEcDomain(CRYS_ECPKI_DomainID_t domainId /*!< [in] Index of one of the domain Id (must be one of the supported domains). */);

#ifdef __cplusplus
}
#endif

#endif
