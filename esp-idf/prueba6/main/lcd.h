#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include <stddef.h>

// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00


struct lcd {
    uint8_t _rs_pin;
    uint8_t _rw_pin;
    uint8_t _enable_pin;
    uint8_t _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;
    uint8_t _numlines;
    uint8_t _row_offsets[4];
};


void lcd_lcd(uint8_t rs, uint8_t enable,
             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

void lcd_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
              uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
              uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

void lcd_begin(uint8_t cols, uint8_t rows, uint8_t charsize);

void lcd_setRowOffsets(int row0, int row1, int row2, int row3);

void lcd_clear();

void lcd_home();

void lcd_setCursor(uint8_t col, uint8_t row);

void lcd_display();

void lcd_command(uint8_t value);

size_t lcd_write(uint8_t value);

void lcd_send(uint8_t value, uint8_t mode);

void lcd_write4bits(uint8_t value);

void lcd_write8bits(uint8_t value);

void lcd_pulseEnable();

#endif
