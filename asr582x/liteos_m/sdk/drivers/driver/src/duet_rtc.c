#include <stdio.h>
#include <time.h>
#include "duet.h"
#include "duet_cm4.h"
#include "duet_common.h"
#include "duet_rtc.h"
#ifdef CFG_RTC_INDEPENDENT
#include "pmu.h"
#endif

duet_rtc_time_t *p_duet_rtc_time = (duet_rtc_time_t *)(RTC_TIME_RETENTION_RAM_ADDR);

#ifdef CFG_RTC_INDEPENDENT
void SLEEP_IRQHandler(void)
{
    duet_intrpt_enter();
    struct tm tm1;
    struct tm *p_tm1;
    time_t time1;
    //RTC int sts check and clear
    if(REG_RD(DUET_IRQ_STS_REG_ADDR) & (1 << RTC_IRQ_BIT))
    {
        //clear irq
        RTC->CTRL &= ~RTC_INT_ENABLE;
        //time1 = GET_RTC_CURRENT_DATE();
        //time1 = (((time1 >> 17) & 0xFF) * 86400 + ((time1 >> 12) & 0x1F) * 3600 + ((time1 >> 6) & 0x3F) * 60 + (time1 & 0x3F));
        tm1.tm_year = p_duet_rtc_time->year;
        tm1.tm_mon = p_duet_rtc_time->month;
        tm1.tm_mday = p_duet_rtc_time->date;
        tm1.tm_hour = p_duet_rtc_time->hr;
        tm1.tm_min = p_duet_rtc_time->min;
        tm1.tm_sec = p_duet_rtc_time->sec;

        time1 = RTC_MAX_DAY * SECOND_PER_DAY - 1;

        time1 += mktime(&tm1);

        p_tm1 = gmtime(&time1);

        p_duet_rtc_time->year = p_tm1->tm_year;
        p_duet_rtc_time->month = p_tm1->tm_mon;
        p_duet_rtc_time->date = p_tm1->tm_mday;
        p_duet_rtc_time->hr = p_tm1->tm_hour;
        p_duet_rtc_time->min = p_tm1->tm_min;
        p_duet_rtc_time->sec = p_tm1->tm_sec;
        p_duet_rtc_time->weekday = p_tm1->tm_wday;

        while(REG_RD(DUET_IRQ_STS_REG_ADDR) & (1 << RTC_IRQ_BIT))
        {
            REG_WR(DUET_IRQ_STS_REG_ADDR,(1 << RTC_IRQ_BIT)); //clear rtc int flag
        }
        //enable int
        RTC->CTRL |= RTC_INT_ENABLE;
        //print_date(p_duet_rtc_time);
    }
#ifdef CFG_RTC_INDEPENDENT
    else
    {
        lega_drv_goto_active();
    }
#endif

    duet_intrpt_exit();
}
#endif

/**
 * This function will initialize the on board CPU real time clock
 *
 *
 * @param[in]  rtc  rtc device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_init(duet_rtc_dev_t *rtc)
{
    uint32_t reg_value = 0;
    if(NULL == rtc)
    {
        return EIO;
    }
    if(0 == rtc->port)
    {
        if(!(RTC->CTRL & RTC_ENABLE)) //for deep sleep wake up reboot case consider
        {
            p_duet_rtc_time->year = RTC_INIT_YEAR;
            p_duet_rtc_time->month = RTC_INIT_MONTH;
            p_duet_rtc_time->date = RTC_INIT_DATE;
            p_duet_rtc_time->hr = RTC_INIT_HOUR;
            p_duet_rtc_time->min = RTC_INIT_MINUTE;
            p_duet_rtc_time->sec = RTC_INIT_SECOND;
            p_duet_rtc_time->weekday = RTC_INIT_WEEKDAY;

            RTC->CTRL &= ~RTC_ENABLE;
            RTC->CNT_TICK = RTC_TICK_CNT;
            RTC->CNT_DATE = (RTC_REFRESH_DAY << 17 | RTC_REFRESH_HOUR << 12 | RTC_REFRESH_MINUTE << 6 | RTC_REFRESH_SECOND);
            RTC->CTRL |= RTC_CNT_CYCLE_ENABLE | RTC_INT_ENABLE | RTC_ENABLE; //sel internal RC clock, calibration needed
            //RTC->CTRL |= RTC_CNT_CYCLE_ENABLE | RTC_INT_ENABLE | RTC_ENABLE | CLK32K_SEL | SW_OVERRIDE_REG_POR; //sel ext 32.768k XTAL
            //open RTC interrupt
            reg_value = REG_RD(DUTE_IRQ_EN_REG) & (~SLEEP_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (reg_value | (SLEEP_IRQ_BIT)));
            NVIC_EnableIRQ(SLEEP_IRQn);
        }

        return 0;
    }
    return EIO;
}

/**
 * This function will return the value of time read from the on board CPU real time clock.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_get_time(duet_rtc_dev_t *rtc, duet_rtc_time_t *time)
{
    struct tm tm1;
    struct tm *p_tm1;
    time_t time1;

    if((NULL == rtc) || (NULL == time))
    {
        return EIO;
    }

    if(0 == rtc->port)
    {
        time1 = GET_RTC_CURRENT_DATE();
        time1 = (((time1 >> 17) & 0xFF) * 86400 + ((time1 >> 12) & 0x1F) * 3600 + ((time1 >> 6) & 0x3F) * 60 + (time1 & 0x3F));

        tm1.tm_year = p_duet_rtc_time->year;
        tm1.tm_mon = p_duet_rtc_time->month;
        tm1.tm_mday = p_duet_rtc_time->date;
        tm1.tm_hour = p_duet_rtc_time->hr;
        tm1.tm_min = p_duet_rtc_time->min;
        tm1.tm_sec = p_duet_rtc_time->sec;

        time1 += mktime(&tm1);

        p_tm1 = gmtime(&time1);

        time->year = p_tm1->tm_year;
        time->month = p_tm1->tm_mon;
        time->date = p_tm1->tm_mday;
        time->hr = p_tm1->tm_hour;
        time->min = p_tm1->tm_min;
        time->sec = p_tm1->tm_sec;
        time->weekday = p_tm1->tm_wday;

        return 0;
    }
    return EIO;
}

/**
 * This function will set MCU RTC time to a new value.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_set_time(duet_rtc_dev_t *rtc, const duet_rtc_time_t *time)
{
    if((NULL == rtc) || (NULL == time))
    {
        return EIO;
    }
    if(0 == rtc->port)
    {
        RTC->CTRL &= ~RTC_ENABLE;
        delay(200);
        RTC->CTRL |= RTC_ENABLE;
        *p_duet_rtc_time = *time;

        return 0;
    }
    return EIO;
}

/**
 * De-initialises an RTC interface, Turns off an RTC hardware interface
 *
 * @param[in]  RTC  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_rtc_finalize(duet_rtc_dev_t *rtc)
{
    uint32_t reg_value = 0;
    if(NULL == rtc)
    {
        return EIO;
    }
    if(0 == rtc->port)
    {
        reg_value = REG_RD(DUTE_IRQ_DIS_REG) & (~SLEEP_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (reg_value | (SLEEP_IRQ_BIT)));
        NVIC_DisableIRQ(SLEEP_IRQn);
        RTC->CTRL &= ~(RTC_ENABLE | RTC_INT_ENABLE | RTC_CNT_CYCLE_ENABLE);
        RTC->CNT_TICK = 0;
        RTC->CNT_DATE = 0;
        return 0;
    }
    return EIO;
}
