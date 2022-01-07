#include "systick_delay.h"
#ifdef ALIOS_SUPPORT
#include <stdio.h>
#include <k_config.h>
#include <k_err.h>
#include <k_sys.h>
#include <k_time.h>
#define SYSTICK_AMEND 1
#ifdef SYSTICK_AMEND
#include "duet_timer.h"
#endif
#elif defined HARMONYOS_SUPPORT
#include <stdio.h>
#define SYSTICK_AMEND 1
#ifdef SYSTICK_AMEND
#include "duet_timer.h"
#endif
#else
#include "FreeRTOS.h"
#include "task.h"
#endif
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
static u8  fac_us=0;
static u16 fac_ms=0;

#if defined ALIOS_SUPPORT || defined HARMONYOS_SUPPORT
#ifdef SYSTICK_AMEND
#define SYSTICK_AMEND_MAX_TIME 50000 //ms

duet_timer_dev_t tim={0};
sys_time_t sys_time=0;
volatile uint32_t sys_timer1_cnt = 0;

void timer1_callback(void)
{
    sys_timer1_cnt++;
}
#endif

void SysTick_Handler(void)
{
#ifdef ALIOS_SUPPORT
    krhino_intrpt_enter();
    krhino_tick_proc();
#endif
#ifdef SYSTICK_AMEND
#ifdef ALIOS_SUPPORT
    sys_time = krhino_sys_tick_get();
#else
#endif
    if(sys_time == 1)//first time, init timer
    {
        tim.port=1;
        tim.config.period = SYSTICK_AMEND_MAX_TIME*1000;//50s
        tim.config.reload_mode = TIMER_RELOAD_AUTO;
        tim.config.cb = timer1_callback;

        duet_timer_init(&tim);
        duet_timer_start(&tim);
    }
    else
    {//check hw time
        uint64_t passed_time = SYSTICK_AMEND_MAX_TIME*(sys_timer1_cnt+1) - duet_timer_get(&tim)/1000;
        while(passed_time > sys_time){
            krhino_tick_proc();
            passed_time -= 1;
        }
    }
#endif
#ifdef ALIOS_SUPPORT
    krhino_intrpt_exit();
#endif
}

uint32_t duet_systick_csr_get()
{
    uint32_t ul_systick_ctrl;
    ul_systick_ctrl = SysTick->CTRL;
    return ul_systick_ctrl;
}
void duet_systick_csr_set(uint32_t ctrl)
{
    SysTick->CTRL = ctrl;
}

void delay_init(u8 SYSCLK)
{
    u32 reload;
    //SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    fac_us=SYSCLK;
    reload=SYSCLK;
    reload= reload*1000000/1000;

    fac_ms=1000/1000;
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD=reload;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

#else

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}


void delay_init(u8 SYSCLK)
{
    u32 reload;
    //SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    fac_us=SYSCLK;
    reload=SYSCLK;
    reload= reload*1000000/configTICK_RATE_HZ;

    fac_ms=1000/configTICK_RATE_HZ;

    SysTick_Config(reload);
}
#endif

void delay_us(u32 nus)
{
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;
    ticks=nus*fac_us;
    told=SysTick->VAL;
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;
        }
    };
}

#ifndef ALIOS_SUPPORT
void delay_ms(u32 nms)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    {
        if(nms>=fac_ms)
        {
            vTaskDelay(nms/fac_ms);
        }
        nms%=fac_ms;
    }
    delay_us((u32)(nms*1000));
}
#endif

void delay_xms(u32 nms)
{
    u32 i;
    for(i=0;i<nms;i++) delay_us(1000);
}
