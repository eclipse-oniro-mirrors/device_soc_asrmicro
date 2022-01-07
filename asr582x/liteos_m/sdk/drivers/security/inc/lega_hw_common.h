#ifndef _LEGA_HW_COMMON_H_
#define _LEGA_HW_COMMON_H_
#include "sns_silib.h"

#if defined(ALIOS_SUPPORT)
#include "lega_rhino.h"
extern lega_mutex_t C310HwMutex;
#elif defined(HARMONYOS_SUPPORT)
#include "lega_rtos.h"
extern lega_mutex_t C310HwMutex;
#elif defined(SYSTEM_SUPPORT_OS)
#include "lega_rtos_api.h"
extern lega_mutex_t C310HwMutex;
#endif

#define LEGA_HW_OK         0
#define LEGA_HW_ERR        -1

extern CRYS_RND_Context_t   *rndContext_ptr;
extern CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr;
extern int hw_lib_init_flag;

int lega_hw_lib_init();
int lega_hw_lib_deinit();
int lega_security_engine_init();
int lega_security_engine_deinit();

#endif //_LEGA_HW_COMMON_H_