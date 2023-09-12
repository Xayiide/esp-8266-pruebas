#include <stdio.h>
#include "driver/adc.h"        /* adc_config_t                */
#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS          */
#include "freertos/task.h"     /* vTaskDelay                  */
#include "esp_log.h"           /* ESP_LOGI                    */

#define DEC_PLACE_MUL 10000

static void adc_task()
{
    uint16_t adc_data;
    double   voltage;
    int32_t  aux_v;

    while (1) {
        if (adc_read(&adc_data) == ESP_OK) {
            voltage = adc_data * (3.3 / 1023);
            aux_v   = voltage * DEC_PLACE_MUL;
            printf("[+] adc %d -- ", adc_data);
            printf("%d.%2u\r\n",
                    aux_v / DEC_PLACE_MUL, abs(aux_v) * DEC_PLACE_MUL);
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void app_main()
{
    adc_config_t adc_config;

    adc_config.mode    = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;
    ESP_ERROR_CHECK(adc_init(&adc_config));

    xTaskCreate(adc_task, "adc_task", 1024, NULL, 5, NULL);
}
