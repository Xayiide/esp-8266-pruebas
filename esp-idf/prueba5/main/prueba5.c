#include "driver/adc.h"        /* adc_config_t                */
#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS          */
#include "freertos/task.h"     /* vTaskDelay                  */
#include "esp_log.h"           /* ESP_LOGI                    */

static const char *TAG = "adc example";

static void adc_task()
{
    int x;
    uint16_t adc_data[100];

    while (1) {
        if (ESP_OK == adc_read(&adc_data[0])) {
            ESP_LOGI(TAG, "adc read: %d\r\n", adc_data[0]);
        }

        ESP_LOGI(TAG, "adc read fast:\r\n");

        if (ESP_OK == adc_read_fast(adc_data, 100)) {
            for (x = 0; x < 100; x++) {
                printf("%d\n", adc_data[x]);
            }
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void app_main()
{
    adc_config_t adc_config;

    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;
    ESP_ERROR_CHECK(adc_init(&adc_config));

    xTaskCreate(adc_task, "adc_task", 1024, NULL, 5, NULL);


}
