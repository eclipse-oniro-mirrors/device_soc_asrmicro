
#ifndef __SYSTEM_CM4_H
#define __SYSTEM_CM4_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "duet_cm4.h"

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
extern void duet_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif
