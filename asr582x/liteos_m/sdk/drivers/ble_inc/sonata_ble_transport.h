/**
 ****************************************************************************************
 *
 * @file sonata_ble_transport.h
 *
 * @brief header file - ble transport
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_BLE_TRANS_H_
#define _SONATA_BLE_TRANS_H_

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



/*
 * Type Definition
 ****************************************************************************************
 */
typedef struct sonata_ble_transport_pad_config
{
    void (*pf_init_pad_config)(void);
    void (*pf_wakeup_pad_config)(void);
    void (*pf_flow_on_pad_config)(void);
    void (*pf_flow_off_pad_config)(void);
}sonata_ble_transport_pad_config_t;


/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void sonata_ble_transport_init(sonata_ble_transport_pad_config_t *config);


extern void sonata_ble_transport_wkup(void);


#endif //_SONATA_BLE_TRANS_H_

