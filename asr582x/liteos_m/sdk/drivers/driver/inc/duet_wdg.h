#ifndef _DUET_WDG_H_
#define _DUET_WDG_H_
#include <stdint.h>
#include <errno.h>

typedef struct {
    uint32_t timeout;  /* Watchdag timeout */
} duet_wdg_config_t;

typedef struct {
    uint8_t      port;   /* wdg port */
    duet_wdg_config_t config; /* wdg config */
    void        *priv;   /* priv data */
} duet_wdg_dev_t;

/**
 * This function will initialize the on board CPU hardware watch dog
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_init(duet_wdg_dev_t *wdg);

/**
 * Reload watchdog counter.
 *
 * @param[in]  wdg  the watch dog device
 */
void duet_wdg_reload(duet_wdg_dev_t *wdg);

/**
 * This function performs any platform-specific cleanup needed for hardware watch dog.
 *
 * @param[in]  wdg  the watch dog device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_wdg_finalize(duet_wdg_dev_t *wdg);

#endif //_LEGA_WDG_H_