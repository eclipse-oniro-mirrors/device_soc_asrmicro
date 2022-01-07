

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "at_api.h"
#ifdef ALIOS_SUPPORT
#include "aos/cli.h"
#endif
#include "lega_wlan_api.h"

#ifdef CFG_MRFOTA_TEST
uint8_t rfota_wifi_open = 0x0;
extern uint8_t rfota_ble_open;
extern uint8_t rfota_wifi_test;

static void lega_at_wlan_start(char *pwbuf, int blen, int argc, char **argv)
{
    uint8_t i = 0;
    char *ssid = "CMW-AP";
    //char *password = "12345678";
    lega_wlan_init_type_t conf = {0};
    conf.wifi_mode = STA;
    memset(conf.wifi_key, 0, 64);

    if(rfota_ble_open > 0)
    {
        printf("please close ble before wifi test!\n");
        return;
    }

    switch (argc) {
        case 3:
            if((strlen(argv[2]) > 7)&&(strlen(argv[2]) < 65))
                strcpy((char*)conf.wifi_key, (const char*)argv[2]);
            else if(strlen(argv[2]) < 8)
                printf("pwr lenth err.");
        case 2:
            if(strlen((char *)argv[1]) > 32)
            {
                printf("ssid lenth err.");
                return;
            }
            strcpy((char*)conf.wifi_ssid, (const char*)argv[1]);
            break;
        default:
            memcpy(conf.wifi_ssid, ssid, 32);
            break;
    }

    lega_wlan_open(&conf);
    rfota_wifi_open = 0x2;

}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_wifi_start =
{
   .name     = "rfota_wifi_start",
   .help     = "rfota_wifi_start [ssid] <pwd> for sta",
   .function = lega_at_wlan_start,
};
#endif
static void lega_at_wlan_stop(char *pwbuf, int blen, int argc, char **argv)
{
    if(rfota_wifi_open == 0)
    {
        printf("rfota wifi not open,don't need close.\n");
        return;
    }
    lega_wlan_close();

    rfota_wifi_open = 0x0;
}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_wifi_stop =
{
  .name     = "rfota_wifi_stop",
  .help     = "rfota_wifi_stop for station",
  .function = lega_at_wlan_stop,
};
#endif
static void lega_enable_rfota_wifi_test(char *pwbuf, int blen, int argc, char **argv)
{
    int c=1;
    if(strcmp(argv[c],"on")==0)
    {
        rfota_wifi_test = 1;
        printf("rfota wifi test begin\n");
    }
    else if(strcmp(argv[c],"off")==0)
    {
        rfota_wifi_test = 0;
        printf("rfota wifi test end\n");
    }
}
#ifdef ALIOS_SUPPORT
static struct cli_command rfota_wifi_cmd =
{
   .name     = "rfota_wifi_test",
   .help     = "rfota_wifi_test on/off",
   .function = lega_enable_rfota_wifi_test,
};


void lega_rfota_wifi_test_at_init(void)
{
    aos_cli_register_command(&rfota_wifi_cmd);
    aos_cli_register_command(&rfota_wifi_start);
    aos_cli_register_command(&rfota_wifi_stop);
}
#endif
#endif
