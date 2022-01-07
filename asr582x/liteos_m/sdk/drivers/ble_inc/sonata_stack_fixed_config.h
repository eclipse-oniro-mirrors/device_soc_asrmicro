#ifndef SONATA_STACK_FIXED_CONFIG_H_
#define SONATA_STACK_FIXED_CONFIG_H_


/*
 * DEFINES
 ****************************************************************************************
 */
//////////////////////////////////////////
/*do not alter*/
#ifndef CFG_BLE
#define CFG_BLE
#endif

#define CFG_EMB

#define CFG_ALLROLES

#ifdef CFG_PLF_SONATA
#define CFG_ACT            8
#endif

#ifdef CFG_PLF_DUET
#define CFG_ACT            12
#endif
#define CFG_RAL            50

#define CFG_CON_CTE_REQ

#define CFG_CON_CTE_RSP

#define CFG_CONLESS_CTE_TX

#define CFG_CONLESS_CTE_RX

#define CFG_AOD

#define CFG_AOA

#define CFG_WLAN_COEX

#define CFG_NB_PRF         10

#endif /*SONATA_STACK_FIXED_CONFIG_H_*/