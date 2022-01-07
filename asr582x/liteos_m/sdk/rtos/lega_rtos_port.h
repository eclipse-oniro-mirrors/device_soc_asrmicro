#ifndef __LEGA_RTOS_PORT_H__
#define __LEGA_RTOS_PORT_H__


#include "los_arch_context.h"
// typedef unsigned int           UINTPTR;
#define lega_rtos_declare_critical()  UINTPTR uvIntSave
/** @brief Enter a critical session, all interrupts are disabled
  *
  * @return    none
  */
#define lega_rtos_enter_critical() do{uvIntSave = LOS_IntLock();}while(0)
//void lega_rtos_enter_critical( void );

/** @brief Exit a critical session, all interrupts are enabled
  *
  * @return    none
  */
#define lega_rtos_exit_critical() do{LOS_IntRestore(uvIntSave);}while(0)
//void lega_rtos_exit_critical( void );

#endif //__LEGA_RTOS_PORT_H__