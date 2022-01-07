#include <stdio.h>
#include <stdint.h>
#include "duet_cm4.h"
#include "duet_pwm.h"
#include "pwm.h"

/**
 * Initialises a PWM pin
 *
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_pwm_init(pwm_dev_t *pwm)
{
    return duet_pwm_init((duet_pwm_dev_t *)pwm);
}

/**
 * Starts Pulse-Width Modulation signal output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_pwm_start(pwm_dev_t *pwm)
{
    return duet_pwm_start((duet_pwm_dev_t *)pwm);
}

/**
 * Stops output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_pwm_stop(pwm_dev_t *pwm)
{
    return duet_pwm_stop((duet_pwm_dev_t *)pwm);
}

/**
 * change the para of pwm
 *
 * @param[in]  pwm  the PWM device
 * @param[in]  para the para of pwm
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_pwm_para_chg(pwm_dev_t *pwm, pwm_config_t para)
{
    duet_pwm_config_t duet_para;
    duet_para.duty_cycle = para.duty_cycle;
    duet_para.freq = para.freq;
    return duet_pwm_para_chg((duet_pwm_dev_t *)pwm, duet_para);
}

/**
 * De-initialises an PWM interface, Turns off an PWM hardware interface
 *
 * @param[in]  pwm  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_pwm_finalize(pwm_dev_t *pwm)
{
    return duet_pwm_finalize((duet_pwm_dev_t *)pwm);
}

