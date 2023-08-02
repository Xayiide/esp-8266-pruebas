#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "driver/gpio.h"

#define LED1 16
#define LED2 2
#define GPIO_OUTPUT_MASK ((1ULL << LED1) | (1ULL << LED2))

#define EXAMPLE_ESP_WIFI_SSID      "ejemploSSID"
#define EXAMPLE_ESP_WIFI_PASS      "ejemploPASS"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10

static EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
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
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        printf("[+] IP: %s\n", ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    EventBits_t        bits;
    wifi_config_t      wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };

    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,
                                               ESP_EVENT_ANY_ID,
                                               &event_handler,
                                               NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,
                                               IP_EVENT_STA_GOT_IP,
                                               &event_handler,
                                               NULL));

    if (strlen((char *) wifi_config.sta.password)) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    bits = xEventGroupWaitBits(s_wifi_event_group,
                               WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                               pdFALSE,
                               pdFALSE,
                               portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
        printf("[+] Conectado al SSID %s\n", EXAMPLE_ESP_WIFI_SSID);
    else if (bits & WIFI_FAIL_BIT)
        printf("[!] Fallo al conectarse al SSID %s\n", EXAMPLE_ESP_WIFI_SSID);
    else
        printf("[!!] ERROR INESPERADO\n");

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT,
                                                 IP_EVENT_STA_GOT_IP,
                                                 &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT,
                                                 ESP_EVENT_ANY_ID,
                                                 &event_handler));
    vEventGroupDelete(s_wifi_event_group);
}

void app_main()
{
    gpio_config_t io_conf;

    ESP_ERROR_CHECK(nvs_flash_init());

    io_conf.intr_type    = GPIO_INTR_DISABLE;
    io_conf.mode         = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_MASK;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 1;
    gpio_config(&io_conf);

    wifi_init_sta();

    while (1) {
        gpio_set_level(LED1, 0);
        gpio_set_level(LED2, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        gpio_set_level(LED1, 1);
        gpio_set_level(LED2, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
