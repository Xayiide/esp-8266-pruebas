#include <stdio.h>                 /* printf             */
#include <string.h>                /* memcpy, memset     */
#include "ezwifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h" /* EventBits_t        */
#include "esp_netif.h"
#include "esp_event.h"             /* esp_event_handler  */
#include "esp_wifi.h"              /* wifi_init_config_t */

#include "nvs.h"                   /* nvs_flash_init     */
#include "nvs_flash.h"             /* nvs_flash_init     */


static EventGroupHandle_t s_wifi_event_group;

void ezwifi_init_sta(const char *ssid, const char *pwd)
{
    printf("[+] Intentando conectar al SSID %s con contraseña %s\n",
            ssid, pwd);
    printf("[+] Longitud de %s: %d\n", ssid, strlen(ssid));
    printf("[+] Longitud de %s: %d\n", pwd, strlen(pwd));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    EventBits_t        bits;
    wifi_config_t      wifi_config;


    /* Poner a cero los arrays porque si no puede que al copiar el SSID y la
     * pass con memcpy no estén los strings acabados con NULL correctamente */
    memset(wifi_config.sta.ssid, 0, sizeof(wifi_config.sta.ssid));
    memset(wifi_config.sta.password, 0, sizeof(wifi_config.sta.password));

    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid));
    memcpy(wifi_config.sta.password, pwd, strlen(pwd));

    printf("-> %s\n", wifi_config.sta.ssid);
    printf("-> %s\n", wifi_config.sta.password);

    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
                                               ESP_EVENT_ANY_ID,
                                               &ezwifi_event_handler,
                                               NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
                                               IP_EVENT_STA_GOT_IP,
                                               &ezwifi_event_handler,
                                               NULL));

    if (strlen((char *) wifi_config.sta.password))
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    bits = xEventGroupWaitBits(s_wifi_event_group,
                               WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                               pdFALSE,
                               pdFALSE,
                               portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
        printf("[+] Conectado al SSID %s\n", ssid);
    else if (bits & WIFI_FAIL_BIT)
        printf("[!] Fallo al conectarse al SSID %s\n", ssid);
    else
        printf("[!!] ERROR INESPERADO\n");

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT,
                                                 IP_EVENT_STA_GOT_IP,
                                                 &ezwifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT,
                                                 ESP_EVENT_ANY_ID,
                                                 &ezwifi_event_handler));

    vEventGroupDelete(s_wifi_event_group);
}

void ezwifi_event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAX_RETRIES) {
            esp_wifi_connect();
            s_retry_num++;
            printf("[!] Intentando reconectar.\n");
        }
        else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        printf("[!] Fallo al intentar conectar.\n");
    }
    else if (event_base == IP_EVENT &&
             event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        printf("[+] IP: %s\n", ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

#ifdef PRUEBAS
void app_main()
{

    const char *ssid = "SSID";
    const char *pass = "PASSWORD";

    ESP_ERROR_CHECK(nvs_flash_init());
    ezwifi_init_sta(ssid, pass);
}
#endif
