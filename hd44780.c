#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"

uint8_t lcd_read(uint8_t rs) {
    uint8_t lcd_reply;
    /* set control pins to output */
    LCD_DDR |= (1 << RS) | (1 << RW) | (1 << E);
    /* set proper RS bit */
    LCD_PORT &= ~(1 << RS);
    LCD_PORT |= (rs << RS);
    /* RW bit == 1 for reading */
    LCD_PORT |= (1 << RW);
    /* set data pins to input */
    LCD_DDR &= 0xf0;
    /* make strobing pulse */
    LCD_PORT |= (1 << E);
    _delay_us(STROBE_DELAY);
    LCD_PORT &= ~(1 << E);
    /* store first 4 bits of reply */
    lcd_reply = LCD_PIN << 4;
    /* make strobing pulse */
    LCD_PORT |= (1 << E);
    _delay_us(STROBE_DELAY);
    LCD_PORT &= ~(1 << E);
    /* store last 4 bits of reply */
    lcd_reply |= LCD_PIN;
    return lcd_reply & 0x80;
}

void lcd_write(uint8_t data, uint8_t rs) {
    /* set control pins to output */
    LCD_DDR |= (1 << RS) | (1 << RW) | (1 << E);
    /* set proper RS bit */
    LCD_PORT &= ~(1 << RS);
    LCD_PORT |= (rs << RS);
    /* RW bit == 0 for writing */
    LCD_PORT &= ~(1 << RW);
    /* set data pins to output */
    LCD_DDR |= 0x0f;
    /* place 4 higher bits of data */
    LCD_PORT &= 0xf0;
    LCD_PORT |= data >> 4;
    /* make strobing pulse */
    LCD_PORT |= (1 << E);
    _delay_us(STROBE_DELAY);    
    LCD_PORT &= ~(1 << E);
    /* place 4 lower bits of data */
    LCD_PORT &= 0xf0;
    LCD_PORT |= (data & 0x0f);
    /* make strobing pulse */
    LCD_PORT |= (1 << E);
    _delay_us(STROBE_DELAY);    
    LCD_PORT &= ~(1 << E);
}

void lcd_wait() {
    while (lcd_is_busy()) { }
}

void lcd_init() {
    /* function set */
    lcd_wait();
    lcd_write_instruction(0x28);
    /* clear */
    lcd_wait();
    lcd_write_instruction(0x01);
    /* set display on */
    lcd_wait();
    lcd_write_instruction(0x0c);
    /* clear */
    lcd_wait();
    lcd_write_instruction(0x01);
}

void lcd_shift(uint8_t sc, uint8_t rl) {
    lcd_wait();
    _delay_ms(1);
    lcd_write_instruction(0x10 | (sc << 3) | (rl << 2));
}

void lcd_print(uint8_t *text, uint8_t length) {
    int i;
    for (i = 0; i < length; i++) {
        lcd_wait();
        lcd_write_data(text[i]);
    }
}
