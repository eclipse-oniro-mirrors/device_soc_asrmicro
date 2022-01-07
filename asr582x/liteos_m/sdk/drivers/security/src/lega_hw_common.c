#include "lega_hw_common.h"
#include "duet_cm4.h"
#include "duet_common.h"
#include <stdlib.h>
#include <string.h>
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
lega_mutex_t C310HwMutex;
#endif
CRYS_RND_Context_t   *rndContext_ptr = NULL;
CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr = NULL;
int hw_lib_init_flag = 0;
int lega_hw_lib_init()
{
    int ret = 0;
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    lega_rtos_init_mutex(&C310HwMutex);
#endif
    if (rndContext_ptr == NULL)
        return LEGA_HW_ERR;
    memset((char *)rndContext_ptr, 0, sizeof(CRYS_RND_Context_t) + sizeof(CRYS_RND_WorkBuff_t));
    rndWorkBuff_ptr = (CRYS_RND_WorkBuff_t *) ((char *)rndContext_ptr + sizeof(CRYS_RND_Context_t));
    //C310 clock enable
    REG_WR(SYS_REG_BASE_CLK1_ENABLE,REG_RD(SYS_REG_BASE_CLK1_ENABLE)|CC310_CLOCK_ENABLE);
    ret = SaSi_LibInit(rndContext_ptr, rndWorkBuff_ptr);
    if (ret != SA_SILIB_RET_OK){
        rndWorkBuff_ptr = NULL;
        return LEGA_HW_ERR;
    }
    hw_lib_init_flag = 1;
    return LEGA_HW_OK;
}

int lega_hw_lib_deinit()
{
    hw_lib_init_flag = 0;
    if (rndWorkBuff_ptr != NULL)
        rndWorkBuff_ptr = NULL;
    if (rndContext_ptr != NULL)
    {
        SaSi_LibFini((CRYS_RND_Context_t *) rndContext_ptr);
    }
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    lega_rtos_deinit_mutex(&C310HwMutex);
#endif
    return LEGA_HW_OK;
}
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
int lega_security_engine_init()
{
    if(rndContext_ptr != NULL)
        return -1;
    rndContext_ptr = lega_rtos_malloc(sizeof(CRYS_RND_Context_t) + sizeof(CRYS_RND_WorkBuff_t));
    if(rndContext_ptr == NULL)
        return -1;
    return lega_hw_lib_init();
}

int lega_security_engine_deinit()
{
    if(rndContext_ptr == NULL)
        return 0;
    lega_hw_lib_deinit();
    lega_rtos_free(rndContext_ptr);
    rndContext_ptr = NULL;
}
#endif