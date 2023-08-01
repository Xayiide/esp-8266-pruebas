#include "driver/gpio.h"       /* gpio_config, gpio_set_level */
#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS          */
#include "freertos/task.h"     /* vTaskDelay                  */

#define LED1 16
#define LED2 2
#define GPIO_OUTPUT_MASK ((1ULL << LED1) | (1ULL << LED2))


void app_main()
{
    gpio_config_t io_conf;

    io_conf.intr_type    = GPIO_INTR_DISABLE; /* Disable interrupts */
    io_conf.mode         = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_MASK;  /* Bit mask of active pins */
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 1;
    gpio_config(&io_conf);


    while (1) {
        gpio_set_level(LED1, 0);
        gpio_set_level(LED2, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        gpio_set_level(LED1, 1);
        gpio_set_level(LED2, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
