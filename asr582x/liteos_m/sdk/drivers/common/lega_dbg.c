#include "lega_dbg.h"

uint32_t  GlobalDebugComponents = 0xFFFFFFFF;
uint32_t  GlobalDebugLevel = D_ERR;
uint8_t   GlobalDebugEn = 1;

char uwifi_m[6] = "uwifi";
char lwifi_m[6] = "lwifi";
char lwip_m[5]  = "Lwip";
char os_m[3]    = "OS";
char at_m[3]    = "AT";
char other_m[6] = "Other";

char *component_info(uint32_t module_t)
{
    if(module_t & LWIFI_COMP)
        return lwifi_m;
    else if(module_t & UWIFI_COMP)
        return uwifi_m;
    else if(module_t & OS_COMP)
        return os_m;
    else if(module_t & LWIP_COMP)
        return lwip_m;
    else if(module_t & AT_COMP)
        return at_m;

    return other_m;
}
