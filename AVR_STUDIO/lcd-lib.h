/*
 * Short description for lcd-lib.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 03:30
 *
 * Version 0.1
 *
 * driver for HD44780 (AtMega16)
 * D4-D7 = PC4-PC7
 * RS = PC2
 * E = PC3
 * RW = GROUND
*/

#ifndef LCD_LIB_H
#define LCD_LIB_H

/*PC2*/
#define	RS	2
/*PC3*/
#define E	3
/*В случае смены порта - сменить только define's*/
#define PORT	PORTC
#define DDR     DDRC

/* Prototypes for function */

void lcd_cmd_half_byte(unsigned char cmd);
/*Функция передачи полубайта для инициализации ЖКИ*/

void lcd_cmd_full_byte(unsigned char cmd);
/*(4bit) Функция для записи команд в LCD */

void lcd_gotoxy(unsigned char x, unsigned char y);
/*Выставить позицию на LCD, х - линия, у - позиция*/

void lcd_clr(void);         /*Clear display*/

void lcd_putchar(unsigned char data);
/* Функция для вывода символа на LCD */

void lcd_str(char *s);          /*Выводим строку на LCD*/

void lcd_init(void);            /* Инициализация ЖКИ модуля */
	                                /*4-bit interface*/

#endif /* !LCD_LIB_H */
