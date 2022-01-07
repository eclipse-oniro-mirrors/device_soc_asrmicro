#ifndef _LEGA_DBG_H_
#define _LEGA_DBG_H_

#include "lega_types.h"
#include <stdio.h>

#define D_CRT       1
#define D_ERR       2
#define D_WRN       3
#define D_INF       4
#define D_DBG       5

#define D_LWIFI_KE        BIT(0)
#define D_LWIFI_DMA       BIT(1)
#define D_LWIFI_MM        BIT(2)
#define D_LWIFI_TX        BIT(3)
#define D_LWIFI_RX        BIT(4)
#define D_LWIFI_PHY       BIT(5)
#define D_LWIFI           BIT(6)

#define D_UWIFI_DATA      BIT(11)
#define D_UWIFI_CTRL      BIT(12)
#define D_UWIFI_SUPP      BIT(13)

#define D_OS              BIT(14)
#define D_LWIP            BIT(15)

#define D_AT              BIT(16)

#define LWIFI_COMP        (D_LWIFI_KE | D_LWIFI_DMA | D_LWIFI_MM | \
                           D_LWIFI_TX | D_LWIFI_RX | D_LWIFI_PHY | D_LWIFI)
#define UWIFI_COMP        (D_UWIFI_DATA | D_UWIFI_CTRL | D_UWIFI_SUPP)
#define OS_COMP           (D_OS)
#define LWIP_COMP         (D_LWIP)
#define AT_COMP           (D_AT)


extern uint32_t  GlobalDebugComponents;
extern uint32_t  GlobalDebugLevel;
extern uint8_t   GlobalDebugEn;
extern uint32_t lega_rtos_get_time(void);
extern char *component_info(uint32_t module_t);

#define component_t(module_t) component_info(module_t)

#define dbg(Level, Comp, Fmt, ...)\
    do {\
        if(GlobalDebugEn && (Comp & GlobalDebugComponents) && (Level <= GlobalDebugLevel)) {\
            printf("[%u] ",(unsigned int)lega_rtos_get_time());\
            printf(Fmt "\r\n", ## __VA_ARGS__);\
        }\
    }while(0)

#endif //_LEGA_DBG_H_
