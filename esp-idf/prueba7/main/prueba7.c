#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS */
#include "freertos/task.h"     /* vTaskDelay         */

#include "esp_err.h"           /* ESP_ERROR_CHECK    */
#include "nvs.h"               /* nvs_flash_init()   */
#include "nvs_flash.h"         /* nvs_flash_init()   */

#include "ezwifi.h"

#define LED1 16
#define LED2 2
#define GPIO_OUTPUT_MASK ((1ULL << LED1) | (1ULL << LED2))


void app_main()
{
    const char *ssid = "SSID";
    const char *pass = "PASS";

    ESP_ERROR_CHECK(nvs_flash_init());
    //ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_create_default());


    ezwifi_init_sta(ssid, pass);
}
