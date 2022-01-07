/**
 ****************************************************************************************
 *
 * @file sonata_sec_api.h
 *
 * @brief header file - security application interface
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_SEC_API_H_
#define _SONATA_SEC_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_utils_api.h"



/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */





/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */
/** security level **/
typedef enum{
    /** security mode 1 level 1: no security **/
    SONATA_SEC_M1_L1 = 0,
    /** security mode 1 level 2: unauthenticated pairing with encryption , no mitm protection **/
    SONATA_SEC_M1_L2,
    /** security mode 1 level 3: authenticated pairing with encryption, mitm protection **/
    SONATA_SEC_M1_L3,
    /** security mode 1 level 4: authenticated le secure connections pairing with encryption using a 128-bit stength encryption key **/
    SONATA_SEC_M1_L4,
    /** unauthenticated pairing with data signing **/
    SONATA_SEC_M2_L1,
    /** authenticated pairing with data signing **/
    SONATA_SEC_M2_L2,
}sonata_sec_lvl_t;

/**IO capabilities**/
typedef enum{
    /** display only **/
    SONATA_IO_CAP_DISPLAY_ONLY = 0,
    /** display yes no **/
    SONATA_IO_CAP_DISPLAY_YES_NO,
    /** keyboard only **/
    SONATA_IO_CAP_KB_ONLY,
    /** no input no output **/
    SONATA_IO_CAP_NO_INPUT_NO_OUTPUT,
    /** keyboard display **/
    SONATA_IO_CAP_KB_DISPLAY,
}sonata_iocap_t;

/** OOB data present flag values **/
typedef enum{
    /** OOB data not present **/
    SONATA_OOB_AUTH_DATA_NOT_PRESENT = 0,
    /** OOB data present **/
    SONATA_OOB_AUTH_DATA_PRESENT,
}sonata_oob_t;

/** authentication requirements **/




/*
 * Type Definition
 ****************************************************************************************
 */
typedef struct sonata_sec_api_cb{

    struct sonata_sec_api_cb * _next;
}sonata_sec_api_cb_t;



/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void sonata_sec_api_init(void);
extern void sonata_sec_api_cb_register(sonata_sec_api_cb_t *cb);





#endif //_SONATA_SEC_API_H_

