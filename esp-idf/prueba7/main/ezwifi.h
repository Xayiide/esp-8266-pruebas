#ifndef _EZWIFI_H_
#define _EZWIFI_H_

#include "esp_event.h" /* esp_event_base_t */
#include <stdint.h>    /* ints             */

#define MAX_RETRIES        10
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1


void ezwifi_init_sta(const char *ssid, const char *pwd);
void ezwifi_event_handler(void *, esp_event_base_t, int32_t, void *);


#endif
