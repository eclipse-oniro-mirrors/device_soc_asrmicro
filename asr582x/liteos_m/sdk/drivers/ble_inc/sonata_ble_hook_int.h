/**
 ****************************************************************************************
 *
 * @file sonata_ble_hook_int.h
 *
 * @brief This file contains the declare of the rom internal hook
 *
 *
 *
 *
 ****************************************************************************************
 */
#ifndef _SONATA_BLE_HOOK_INT_H_
#define _SONATA_BLE_HOOK_INT_H_

#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include <string.h>
#include "rwip.h"

/*
 * TYPEDEF
 *****************************************************************************************
 */
#if (BLE_PROFILES)
typedef void (*PF_PRF_CREATE)(uint8_t conidx);
typedef void (*PF_PRF_CLEANUP)(uint8_t conidx, uint8_t reason);
typedef uint16_t (*PF_PRF_GET_ID_FROM_TASK)(uint16_t task);
typedef uint16_t (*PF_PRF_GET_TASK_FROM_ID)(uint16_t id);
typedef void (*PF_PRF_INIT)(uint8_t init_type);
typedef uint8_t (*PF_PRF_ADD_PROFILE)(void * params, uint16_t* prf_task);
#endif //(BLE_PROFILES)

typedef uint8_t (*PF_FLASH_IDENTIFY)(uint8_t* id, void (*callback)(void));
typedef uint8_t (*PF_FLASH_ERASE)(uint8_t flash_type, uint32_t offset, uint32_t size, void (*callback)(void));
typedef uint8_t (*PF_FLASH_WRITE)(uint8_t flash_type, uint32_t offset, uint32_t length, uint8_t *buffer, void (*callback)(void));
typedef uint8_t (*PF_FLASH_READ)(uint8_t flash_type, uint32_t offset, uint32_t length, uint8_t *buffer, void (*callback)(void));
typedef void (*PF_EFUSE_INIT)(void);
typedef uint32_t (*PF_EFUSE_READ)(uint8_t addr);



typedef void (*PF_RF_INIT)(void *api);
typedef    void (*PF_RESET_START)(void);
typedef    void (*PF_RESET_END)(void);

typedef struct sonata_ble_hook_int{
#if (BLE_PROFILES)
    PF_PRF_CREATE pf_prf_create;
    PF_PRF_CLEANUP pf_prf_cleanup;
    PF_PRF_GET_ID_FROM_TASK pf_prf_get_id_from_task;
    PF_PRF_GET_TASK_FROM_ID pf_prf_get_task_from_id;
    PF_PRF_INIT pf_prf_init;
    PF_PRF_ADD_PROFILE pf_prf_add_profile;
#endif // BLE_PROFILES

    PF_FLASH_IDENTIFY pf_flash_identify;
    PF_FLASH_ERASE pf_flash_erase;
    PF_FLASH_WRITE pf_flash_write;
    PF_FLASH_READ pf_flash_read;
    PF_EFUSE_INIT pf_efuse_init;
    PF_EFUSE_READ pf_efuse_read;

    PF_RF_INIT pf_rf_init;
    PF_RESET_START pf_reset_start;
    PF_RESET_END   pf_reset_end;
}sonata_ble_hook_int_t;


/*
 * VARIABLE DECLARATION
 *****************************************************************************************
 */
#if (BLE_PROFILES)
extern PF_PRF_CREATE pf_prf_create;
extern PF_PRF_CLEANUP pf_prf_cleanup;
extern PF_PRF_GET_ID_FROM_TASK pf_prf_get_id_from_task;
extern PF_PRF_GET_TASK_FROM_ID pf_prf_get_task_from_id;
extern PF_PRF_INIT pf_prf_init;
extern PF_PRF_ADD_PROFILE pf_prf_add_profile;
#endif //(BLE_PROFILES)
extern PF_FLASH_IDENTIFY pf_flash_identify;
extern PF_FLASH_ERASE pf_flash_erase;
extern PF_FLASH_WRITE pf_flash_write;
extern PF_FLASH_READ pf_flash_read;
extern PF_EFUSE_INIT pf_efuse_init;
extern PF_EFUSE_READ pf_efuse_read;

extern PF_RF_INIT pf_rf_init;

extern void (*pf_ble_util_buf_llcp_tx_free)(uint16_t buf);
extern void (*pf_ble_util_buf_acl_tx_free)(uint16_t buf);
extern void (*pf_ble_util_buf_adv_tx_free)(uint16_t buf);
extern uint16_t (*pf_ble_util_buf_rx_alloc)(void);
extern int16_t (*pf_llm_tx_path_comp_get)(void);
extern bool (*pf_rwip_active_check)(void);
extern rwip_time_t (*pf_rwip_time_get)(void);
extern uint8_t (*pf_lld_scan_restart)(void);
extern uint8_t (*pf_rwip_param_get) (uint8_t param_id, uint8_t * lengthPtr, uint8_t *buf);
extern void (*pf_platform_reset)(uint32_t error);
extern void (*pf_rwip_aes_encrypt)(const uint8_t *key, const uint8_t* val);
extern void (*pf_rwip_prevent_sleep_clear)(uint16_t prv_slp_bit);
extern void (*pf_rwip_prevent_sleep_set)(uint16_t prv_slp_bit);
extern void (*pf_rwip_timer_hus_set)(uint32_t target, uint32_t half_us_delay);
extern void (*pf_rwip_timer_1ms_set)(rwip_time_t target);
extern void (*pf_rwip_timer_hs_set)(uint32_t target);
extern void (*pf_rwip_sw_int_req)(void);
extern int    (*pf_printf)(const char *__restrict, ...);
extern void (*pf_lld_con_tx)(uint8_t link_id);
extern void (*pf_lld_con_tx_prog)(uint8_t link_id);
//extern void (*pf_assert_err)(int cond);
//extern void (*pf_assert_param)(int param0, int param1);
//extern void (*pf_assert_warn)(int param0, int param1);

extern int (*pf_rtos_init_semaphore)(void **semaphore, int value);
extern int (*pf_rtos_get_semaphore)(void **semaphore, uint32_t timeout_ms);
extern int (*pf_rtos_set_semaphore)(void **semaphore);

extern uint8_t *p_sw_to_hw;
extern void (*pf_srand)(unsigned int seed);
extern int (*pf_rand)(void);

extern void (*pf_global_int_disable)(void);
extern void (*pf_global_int_restore)(void);


/*
 * FUNCTION DECLARATION
 *****************************************************************************************
 */
extern void sonata_ble_hook_int_register(sonata_ble_hook_int_t *hook);
#endif // _SONATA_BLE_HOOK_INT_H_

