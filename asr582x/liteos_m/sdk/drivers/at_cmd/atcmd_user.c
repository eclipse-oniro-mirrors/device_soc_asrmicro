
#ifdef AT_USER_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "target_config.h"
#if (LOSCFG_USE_SHELL == 1)
#include "shell.h" 
#include "shcmd.h"
#endif
#include "duet_flash_kv.h"
#include "flash.h"
#include "duet_flash.h"
/*
 ************************************************************
 *                    USER AT CMD START
 *
 ************************************************************
 */
#ifdef CFG_HARMONY_SUPPORT
void openharmony_init()
{
    extern void  OHOS_SystemInit(void);
    OHOS_SystemInit();
}
#endif
int at_test_1(int argc, char **argv)
{
    printf("%s...\n",__func__);
    return 0;
}

int at_harmony_xts(int argc, char **argv)
{
#ifdef CFG_HARMONY_SUPPORT
    openharmony_init();
#endif
    return 0;
}

int at_hilink_start(int argc, char **argv)
{
   // hilink_main_wrap();
    return 0;
}

#include "lega_wlan_api_aos.h"
static int ap_conn_func(int argc, char **argv)
{
    #if 0
    extern void lalala_test(void);
    lalala_test();
    return 0;
    #else

    int32_t ret;
    uint32_t start_ip = 0;
    lega_wlan_init_type_t conf;
    memset(&conf,0,sizeof(lega_wlan_init_type_t));
    conf.wifi_mode = STA;

    printf("doing...\n");
    switch(argc)
    {
        case 1:
            break;
        case 2:
            if (strcmp(argv[1],"sta") == 0)
            {
                lega_wlan_open(&conf);
                return 0;
            }
            else
            {
                return -1;
            }
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if ((strcmp(argv[1],"sta") == 0))
            {
                if(strlen((char *)argv[2]) > 32)
                {
                    return -1;
                }
                strcpy((char*)conf.wifi_ssid, (char*)argv[2]);

                if(argc >= 4)
                {
                    if((strlen(argv[3]) > 4)&&(strlen(argv[3]) < 65))
                        strcpy((char*)conf.wifi_key, (char*)argv[3]);
                    else if((strlen(argv[3]) == 1) && (convert_str_to_int(argv[3]) == 0))
                        memset(conf.wifi_key, 0, 64);
                    else
                    {
                        printf("param4 error, input 0 or pwd(length >= 5)\n");
                        return -1;
                    }
                }

                if(argc >= 5)
                {
                    conf.channel = convert_str_to_int(argv[4]);
                    if((conf.channel > 13) || (conf.channel < 1))
                    {
                        printf("param5 error,channel range:1~13");
                        return -1;
                    }
                }
            }
            else if((strcmp(argv[1],"ap") == 0))
            {
                conf.wifi_mode = SOFTAP;
                if(strlen((char *)argv[2]) > 32)
                {
                    return -1;
                }
                strcpy((char*)conf.wifi_ssid, (char*)argv[2]);

                if(argc >= 4)
                {
                    if((strlen(argv[3]) > 7)&&(strlen(argv[3]) < 65))
                        strcpy((char*)conf.wifi_key, (char*)argv[3]);
                    else if((strlen(argv[3]) == 1) && (convert_str_to_int(argv[3]) == 0))
                        memset(conf.wifi_key, 0, 64);
                    else
                    {
                        printf("param4 error, input 0 or pwd(length >= 8)\n");
                        return -1;
                    }
                }

                if(argc >= 5)
                {
                    conf.channel = convert_str_to_int(argv[4]);
                    if((conf.channel > 13) || (conf.channel < 1))
                    {
                        printf("param5 error,channel range:1~13");
                        return -1;
                    }
                }
            }
            else
            {
                printf("param2 error,not support\r\n");
                return -1;
            }
            break;
        default:
            printf("param num error,1~7\r\n");
            return -1;
    }
    //lega_wlan_err_stat_cb_register(ms_hal_wlan_err_stat_handler);

    lega_wlan_clear_pmk();
    lega_wlan_open(&conf);

    return 0;
    #endif

}

static int ap_close_func(int argc, char **argv)
{
    #include "lega_wlan_api.h"
    lega_wlan_close();
    return 0;
}

int at_kv_clear(int argc, char **argv)
{
    hal_flash_erase(PARTITION_PARAMETER_2, 0, KV_MAX_SIZE);
    printf("kv clear done\r\n");
    delay(1000);
    NVIC_SystemReset();
    return 0;
}

/*
 ************************************************************
 *                    USER AT CMD END
 *
 ************************************************************
 */
#ifdef LWIP_APP_IPERF
static int lega_at_wifi_iperf(int argc, char **argv)
{
    lega_wifi_iperf(argc, argv);
    return 0;
}
#endif
void lega_at_user_cmd_register(void)
{
#if (LOSCFG_USE_SHELL == 1)
    osCmdReg(CMD_TYPE_EX, "at_test1", 0, (CMD_CBK_FUNC)at_test_1);
    osCmdReg(CMD_TYPE_EX, "at_harmony_xts", 0, (CMD_CBK_FUNC)at_harmony_xts);
    osCmdReg(CMD_TYPE_EX, "at_hilink_start", 0, (CMD_CBK_FUNC)at_hilink_start);
    osCmdReg(CMD_TYPE_STD, "ap_conn", 0, (CMD_CBK_FUNC)ap_conn_func);
    osCmdReg(CMD_TYPE_EX, "ap_close", 0, (CMD_CBK_FUNC)ap_close_func);
    osCmdReg(CMD_TYPE_EX, "kv_clear", 0, (CMD_CBK_FUNC)at_kv_clear);
#ifdef LWIP_APP_IPERF
    osCmdReg(CMD_TYPE_STD, "iperf", 0, (CMD_CBK_FUNC)lega_at_wifi_iperf);
#endif
#endif
}

#endif
