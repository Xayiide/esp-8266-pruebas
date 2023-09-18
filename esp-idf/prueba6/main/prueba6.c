#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lcd.h"

void app_main()
{
    printf("Prueba LCD: %d\n", LCD_CLEARDISPLAY);
    vTaskDelay(2500 / portTICK_PERIOD_MS);
}

