#include <stdio.h>

#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS          */
#include "freertos/task.h"     /* vTaskDelay                  */
#include "driver/adc.h"        /* adc_config_t                */
#include "esp_log.h"

#define VREF 3.3
#define TEMT6000_INTERNAL_RES 10000 /* 10 k ohms */
#define DEC_PLACE_MUL 10000

static void adc_task()
{
    uint16_t adc_data;
    double   volts, amps, microamps, lux;

    int32_t  print_v, print_lx;
    
    while (1)
    {
        /* adc_data da un rango de 0 a 1.0 Voltios */
        if (adc_read(&adc_data) == ESP_OK) {
            volts     = adc_data * (3.3 / 1023.0);
            amps      = volts / TEMT6000_INTERNAL_RES;
            microamps = amps * 1000000.0;
            lux       = microamps * 2.0; /* No es tan así pero vale */

            print_v   = volts * 10000;
            print_lx  = lux   * 10000;

            printf("[+] Lectura cruda: %d\n", adc_data);
            printf("[x] Volts        : %d.%2u\r\n", 
                    print_v / 10000, abs(print_v) * 10000);
            printf("[+] Lux          : %d.%2u\r\n",
                    print_lx / 10000, abs(print_lx) * 10000);
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
