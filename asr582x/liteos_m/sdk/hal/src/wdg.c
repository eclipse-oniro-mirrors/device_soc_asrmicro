#include <stdio.h>
#include <stdint.h>
#include "duet_cm4.h"
#include "duet_wdg.h"
#include "wdg.h"

/**
 * This function will initialize the on board CPU hardware watch dog
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_wdg_init(wdg_dev_t *wdg)
{
    return duet_wdg_init((duet_wdg_dev_t *)wdg);
}

/**
 * Reload watchdog counter.
 *
 * @param[in]  wdg  the watch dog device
 */
void hal_wdg_reload(wdg_dev_t *wdg)
{
    return duet_wdg_reload((duet_wdg_dev_t *)wdg);
}

/**
 * This function performs any platform-specific cleanup needed for hardware watch dog.
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_wdg_finalize(wdg_dev_t *wdg)
{
    return duet_wdg_finalize((duet_wdg_dev_t *)wdg);
}

