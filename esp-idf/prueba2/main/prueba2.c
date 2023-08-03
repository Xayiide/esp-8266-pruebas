#include <stdlib.h>            /* malloc */
#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS             */
#include "freertos/task.h"     /* vTaskDelay                     */
#include "driver/gpio.h"
#include "driver/uart.h"

#define LED_OUT 13 /* D7 */
#define GPIO_OUTPUT_MASK ((1ULL << LED_OUT))

#define BUF_SIZE 1024

static void uart_task()
{
    int            len;
    uint8_t       *data;
    uart_config_t  uart_config = {
        .baud_rate = 74880,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);

    data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        len =  uart_read_bytes(UART_NUM_0, data, BUF_SIZE,
                               20 / portTICK_RATE_MS);
        if (len == 1) {
            printf("Se ha leído el dato: %d\n", *data);
            if (*data == '1') /* '1' o dcimal 49 (ASCII) */
                gpio_set_level(LED_OUT, 1);
            else if (*data == '0') /* '0' o decimal 48 (ASCII) */
                gpio_set_level(LED_OUT, 0);
        }
    }
}

void app_main()
{
    gpio_config_t io_conf;

    io_conf.intr_type    = GPIO_INTR_DISABLE;
    io_conf.mode         = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_MASK;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 1;
    gpio_config(&io_conf);

    xTaskCreate(uart_task, "uart_task", 1024, NULL, 10, NULL);
}
