/**
 ****************************************************************************************
 *
 * @file sonata_log.h
 *
 * @brief header file - log
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */

#ifndef _SONATA_LOG_H_
#define _SONATA_LOG_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"


/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */
/**
 * @brief Log level
 *
 */
typedef enum {
    SONATA_LOG_NONE,       /*!< No log output */
    SONATA_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    SONATA_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    SONATA_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    SONATA_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    SONATA_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} sonata_log_level;

/*
 * MACRO DEFINES
 ****************************************************************************************
 */
/*!
 * @brief Defalut log level
 */
#define LOG_LOCAL_LEVEL  SONATA_LOG_VERBOSE


/*!
 * @brief vprintf
 */
typedef int (*vprintf_like_t)(const char *, va_list);

/*
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format LOG_RESET_COLOR "\r\n"
#define SONATA_LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==SONATA_LOG_ERROR )           { sonata_logt_write(SONATA_LOG_ERROR,      tag, LOG_FORMAT(E, format), sonata_log_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_WARN )       { sonata_logt_write(SONATA_LOG_WARN,       tag, LOG_FORMAT(W, format), sonata_log_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_DEBUG )      { sonata_logt_write(SONATA_LOG_DEBUG,      tag, LOG_FORMAT(D, format), sonata_log_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_VERBOSE )    { sonata_logt_write(SONATA_LOG_VERBOSE,    tag, LOG_FORMAT(V, format), sonata_log_timestamp(), tag, ##__VA_ARGS__); } \
        else                                    { sonata_logt_write(SONATA_LOG_INFO,       tag, LOG_FORMAT(I, format), sonata_log_timestamp(), tag, ##__VA_ARGS__); } \
    } while(0)
#define SONATA_LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) SONATA_LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)
#define LOGE( tag, format, ... ) SONATA_LOG_LEVEL_LOCAL(SONATA_LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define LOGW( tag, format, ... ) SONATA_LOG_LEVEL_LOCAL(SONATA_LOG_WARN,    tag, format, ##__VA_ARGS__)
#define LOGI( tag, format, ... ) SONATA_LOG_LEVEL_LOCAL(SONATA_LOG_INFO,    tag, format, ##__VA_ARGS__)
#define LOGD( tag, format, ... ) SONATA_LOG_LEVEL_LOCAL(SONATA_LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define LOGV( tag, format, ... ) SONATA_LOG_LEVEL_LOCAL(SONATA_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
 */
//#define CFG_SONATA_LOG
#ifdef CFG_SONATA_LOG
#define SONATA_LOGT_FORMAT_SIMPLE(format)  "%s: " format "\r\n"
#define SONATA_LOGT_LEVEL_SIMPLE(level, tag, format, ...) do {                     \
        if (level==SONATA_LOG_ERROR )           { sonata_logt_write(SONATA_LOG_ERROR,      tag, SONATA_LOGT_FORMAT_SIMPLE(format), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_WARN )       { sonata_logt_write(SONATA_LOG_WARN,       tag, SONATA_LOGT_FORMAT_SIMPLE(format), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_DEBUG )      { sonata_logt_write(SONATA_LOG_DEBUG,      tag, SONATA_LOGT_FORMAT_SIMPLE(format), tag, ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_VERBOSE )    { sonata_logt_write(SONATA_LOG_VERBOSE,    tag, SONATA_LOGT_FORMAT_SIMPLE(format), tag, ##__VA_ARGS__); } \
        else                                    { sonata_logt_write(SONATA_LOG_INFO,       tag, SONATA_LOGT_FORMAT_SIMPLE(format), tag, ##__VA_ARGS__); } \
    } while(0)
#define SONATA_LOGT_LEVEL_LOCAL_SIMPLE(level, tag, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) SONATA_LOGT_LEVEL_SIMPLE(level, tag, format, ##__VA_ARGS__); \
    } while(0)
/*!
 * @brief  Log function With TAG
 * @example LOGTE("TagE","A Simple logE: %02X, %s", 0,__FUNCTION__ );
 * @note  LOGTX function will auto add linefeed for every log string.
 */
#define LOGTE( tag, format, ... ) SONATA_LOGT_LEVEL_LOCAL_SIMPLE(SONATA_LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define LOGTW( tag, format, ... ) SONATA_LOGT_LEVEL_LOCAL_SIMPLE(SONATA_LOG_WARN,    tag, format, ##__VA_ARGS__)
#define LOGTI( tag, format, ... ) SONATA_LOGT_LEVEL_LOCAL_SIMPLE(SONATA_LOG_INFO,    tag, format, ##__VA_ARGS__)
#define LOGTD( tag, format, ... ) SONATA_LOGT_LEVEL_LOCAL_SIMPLE(SONATA_LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define LOGTV( tag, format, ... ) SONATA_LOGT_LEVEL_LOCAL_SIMPLE(SONATA_LOG_VERBOSE, tag, format, ##__VA_ARGS__)

/********************************************* Log function for no TAG ***********************************************************/

#define SONATA_LOG_FORMAT_SIMPLE(format)  "" format ""
#define SONATA_LOG_LEVEL_SIMPLE(level, format, ...) do {                     \
        if (level==SONATA_LOG_ERROR )           { sonata_logt_write(SONATA_LOG_ERROR,     NULL, SONATA_LOG_FORMAT_SIMPLE(format), ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_WARN )       { sonata_logt_write(SONATA_LOG_WARN,      NULL, SONATA_LOG_FORMAT_SIMPLE(format), ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_DEBUG )      { sonata_logt_write(SONATA_LOG_DEBUG,     NULL, SONATA_LOG_FORMAT_SIMPLE(format), ##__VA_ARGS__); } \
        else if (level==SONATA_LOG_VERBOSE )    { sonata_logt_write(SONATA_LOG_VERBOSE,   NULL, SONATA_LOG_FORMAT_SIMPLE(format), ##__VA_ARGS__); } \
        else                                    { sonata_logt_write(SONATA_LOG_INFO,      NULL, SONATA_LOG_FORMAT_SIMPLE(format), ##__VA_ARGS__); } \
    } while(0)
#define SONATA_LOG_LEVEL_LOCAL_SIMPLE(level, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) SONATA_LOG_LEVEL_SIMPLE(level, format, ##__VA_ARGS__); \
    } while(0)

/*!
 * @brief  Log function without TAG
 * @example "LOGE("A Simple logE: %02X, %s r n", 0,__FUNCTION__ );"
 * @note  user should add linefeed for every log string if necessary
 */
#define LOGE( format, ... ) SONATA_LOG_LEVEL_LOCAL_SIMPLE(SONATA_LOG_ERROR,   format, ##__VA_ARGS__)
#define LOGW( format, ... ) SONATA_LOG_LEVEL_LOCAL_SIMPLE(SONATA_LOG_WARN,    format, ##__VA_ARGS__)
#define LOGI( format, ... ) SONATA_LOG_LEVEL_LOCAL_SIMPLE(SONATA_LOG_INFO,    format, ##__VA_ARGS__)
#define LOGD( format, ... ) SONATA_LOG_LEVEL_LOCAL_SIMPLE(SONATA_LOG_DEBUG,   format, ##__VA_ARGS__)
#define LOGV( format, ... ) SONATA_LOG_LEVEL_LOCAL_SIMPLE(SONATA_LOG_VERBOSE, format, ##__VA_ARGS__)

#else //CFG_SONATA_LOG

#define LOGTE( tag, format, ... )
#define LOGTW( tag, format, ... )
#define LOGTI( tag, format, ... )
#define LOGTD( tag, format, ... )
#define LOGTV( tag, format, ... )
#define LOGE( format, ... )
#define LOGW( format, ... )
#define LOGI( format, ... )
#define LOGD( format, ... )
#define LOGV( format, ... )

#endif //CFG_SONATA_LOG


/*
* FUNCTION DEFINITIONS
****************************************************************************************
*/

/*!
 * @brief Set custom log function. If not set, will use default print log function
 * @param func functions with vprintf format
 * @return
 */
vprintf_like_t sonata_log_set_vprintf(vprintf_like_t func);
/*!
 * @brief Set log level
 * @param level @see sonata_log_level
 */
void sonata_log_level_set(sonata_log_level level);


//Internal use
void sonata_logt_write(sonata_log_level level, const char *tag, const char *format, ...);

#endif //_SONATA_LOG_H_
