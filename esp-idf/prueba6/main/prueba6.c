#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lcd.h"

void app_main()
{
    lcd_lcd(7, 6, 5, 4, 3, 2);

    printf("Prueba LCD\n");
    vTaskDelay(2500 / portTICK_PERIOD_MS);
}

