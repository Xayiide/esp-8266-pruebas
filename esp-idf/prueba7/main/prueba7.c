#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS            */
#include "freertos/task.h"     /* vTaskDelay                    */

#include "esp_err.h"           /* ESP_ERROR_CHECK               */
#include "esp_netif.h"         /* esp_netif_init()              */
#include "nvs.h"               /* nvs_flash_init()              */
#include "nvs_flash.h"         /* nvs_flash_init()              */
#include "esp_event.h"         /* esp_event_loop_create_default */

#include "ezconnect.h"

#define LED1 16
#define LED2 2
#define GPIO_OUTPUT_MASK ((1ULL << LED1) | (1ULL << LED2))


void app_main()
{
    const char *ssid = "SSID";
    const char *pass = "PASS";

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());


    ESP_ERROR_CHECK(ez_set_connection_info(ssid, pass))
    ESP_ERROR_CHECK(ezconnect());
}
