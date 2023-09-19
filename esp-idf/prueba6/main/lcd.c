#include "lcd.h"
#include "driver/gpio.h"       /* gpio_config, gpio_set_level */
#include "freertos/FreeRTOS.h" /* portTICK_PERIOD_MS          */
#include "freertos/task.h"     /* vTaskDelay                  */
#include <stddef.h>            /* size_t                      */

static struct lcd this;

unsigned long long GPIO_OUTPUT_MASK = 1;


void lcd_lcd(uint8_t rs, uint8_t enable,
             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    lcd_init(1, rs, 255, enable, d0, d1, d2,d3, 0, 0, 0, 0);
}


void lcd_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
              uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
              uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    this._rs_pin       = rs;
    this._rw_pin       = rw;
    this._enable_pin   = enable;
    this._data_pins[0] = d0;
    this._data_pins[1] = d1;
    this._data_pins[2] = d2;
    this._data_pins[3] = d3;
    this._data_pins[4] = d4;
    this._data_pins[5] = d5;
    this._data_pins[6] = d6;
    this._data_pins[7] = d7;

    if (fourbitmode)
        this._displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        this._displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    lcd_begin(16, 2, LCD_5x8DOTS);
}


void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    int i;
    gpio_config_t io_conf;
    io_conf.intr_type    = GPIO_INTR_DISABLE; /* Disable interrupts */
    io_conf.mode         = GPIO_MODE_OUTPUT;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 1;


    if (lines > 1) {
        this._displayfunction |= LCD_2LINE;
    }

    this._numlines = lines;

    lcd_setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        this._displayfunction |= LCD_5x10DOTS;
    }

    GPIO_OUTPUT_MASK |= (1ULL << this._rs_pin);

    if (this._rw_pin != 255) {
        GPIO_OUTPUT_MASK |= (1ULL << this._rw_pin);
    }

    GPIO_OUTPUT_MASK |= (1ULL << this._enable_pin);

    for (i = 0; i < ((this._displayfunction & LCD_8BITMODE) ? 8 : 4); i++) {
        GPIO_OUTPUT_MASK |= (1ULL << this._data_pins[i]);
    }

    io_conf.pin_bit_mask = GPIO_OUTPUT_MASK;  /* Bit mask of active pins */
    gpio_config(&io_conf);

    vTaskDelay(50 / portTICK_PERIOD_MS); /* 50 ms */

    gpio_set_level(this._rs_pin, 0);
    gpio_set_level(this._enable_pin, 0);

    if (this._rw_pin != 255) {
        gpio_set_level(this._rw_pin, 0);
    }

    if (! (this._displayfunction & LCD_8BITMODE)) {
        lcd_write4bits(0x03);
        vTaskDelay(5 / portTICK_PERIOD_MS); /* 5 ms */

        lcd_write4bits(0x03);
        vTaskDelay(5 / portTICK_PERIOD_MS);

        lcd_write4bits(0x03);
        vTaskDelay(5 / portTICK_PERIOD_MS);

        lcd_write4bits(0x02);
    }
    else {
        lcd_command(LCD_FUNCTIONSET | this._displayfunction);
        vTaskDelay(5 / portTICK_PERIOD_MS);

        lcd_command(LCD_FUNCTIONSET | this._displayfunction);
        vTaskDelay(1 / portTICK_PERIOD_MS);

        lcd_command(LCD_FUNCTIONSET | this._displayfunction);
    }

    lcd_command(LCD_FUNCTIONSET | this._displayfunction);

    this._displayfunction = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_display();

    lcd_clear();

    this._displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    lcd_command(LCD_ENTRYMODESET | this._displaymode);
}

void lcd_setRowOffsets(int row0, int row1, int row2, int row3)
{
    this._row_offsets[0] = row0;
    this._row_offsets[1] = row1;
    this._row_offsets[2] = row2;
    this._row_offsets[3] = row3;
}

void lcd_clear()
{
    lcd_command(LCD_CLEARDISPLAY);
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

void lcd_home()
{
    lcd_command(LCD_RETURNHOME);
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines =
            sizeof(this._row_offsets) / sizeof(*this._row_offsets);

    if (row >= max_lines) {
        row = max_lines - 1;
    }
    if (row >= this._numlines) {
        row = this._numlines - 1;
    }

    lcd_command(LCD_SETDDRAMADDR | (col + this._row_offsets[row]));
}

void lcd_display()
{
    this._displaycontrol |= LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | this._displaycontrol);
}

inline void lcd_command(uint8_t value)
{
    lcd_send(value, 0);
}

inline size_t lcd_write(uint8_t value)
{
    lcd_send(value, 1);
    return 1;
}

void lcd_send(uint8_t value, uint8_t mode)
{
    gpio_set_level(this._rs_pin, mode);

    if (this._rw_pin != 255) {
        gpio_set_level(this._rw_pin, 0);
    }

    if (this._displayfunction & LCD_8BITMODE) {
        lcd_write8bits(value);
    }
    else {
        lcd_write4bits(value >> 4);
        lcd_write4bits(value);
    }
}

void lcd_write4bits(uint8_t value)
{
    int i;

    for (i = 0; i < 4; i++) {
        gpio_set_level(this._data_pins[i], (value >> i) & 0x01);
    }

    lcd_pulseEnable();
}

void lcd_write8bits(uint8_t value)
{
    int i;

    for (i = 0; i < 8; i++) {
        gpio_set_level(this._data_pins[i], (value >> i) & 0x01);
    }

    lcd_pulseEnable();
}

void lcd_pulseEnable()
{
    gpio_set_level(this._enable_pin, 0);
    /* TODO: delay 1 microsecond */
    gpio_set_level(this._enable_pin, 1);
    /* TODO: delay 1 microsecond */
    gpio_set_level(this._enable_pin, 0);
    /* TODO: delay 100 microseconds */
}
