#include <stdint.h>

#ifndef HD44780_H
#define HD44780_H

#define RS 	4
#define RW 	5
#define E 	6

#define STROBE_DELAY 10

#define lcd_is_busy() 				(lcd_read(0) & 0x80)
#define lcd_write_data(d) 			lcd_write(d, 1)
#define lcd_write_instruction(i) 	lcd_write(i, 0)
#define lcd_set_ddram_address(a) 	lcd_write_instruction(0x80 | a)
#define lcd_cursor_shift_left()		lcd_shift(0, 0)
#define lcd_cursor_shift_right()	lcd_shift(0, 1)
#define lcd_screen_shift_left()		lcd_shift(1, 0)
#define lcd_screen_shift_right()	lcd_shift(1, 1)

uint8_t lcd_read(uint8_t);
void lcd_write(uint8_t, uint8_t);
void lcd_waint();
void lcd_init();
void lcd_shift(uint8_t, uint8_t);
void lcd_print(uint8_t *, uint8_t);

#endif
