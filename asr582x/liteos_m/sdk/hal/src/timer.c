#include <stdio.h>
#include <stdint.h>
#include "duet_cm4.h"
#include "duet_timer.h"
#include "timer.h"

/**
 * init a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_timer_init(timer_dev_t *tim)
{
    return duet_timer_init((duet_timer_dev_t *)tim);
}

/**
 * start a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_timer_start(timer_dev_t *tim)
{
    return duet_timer_start((duet_timer_dev_t *)tim);
}

/**
 * stop a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  none
 */
void hal_timer_stop(timer_dev_t *tim)
{
    return duet_timer_stop((duet_timer_dev_t *)tim);
}

/**
 * De-initialises an TIMER interface, Turns off an TIMER hardware interface
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_timer_finalize(timer_dev_t *tim)
{
    return duet_timer_finalize((duet_timer_dev_t *)tim);
}
