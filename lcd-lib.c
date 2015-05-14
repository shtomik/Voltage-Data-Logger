/*
 * Short description for lcd-lib.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 03:30
 *
 * Version 0.1
*/

#include "lcd-lib.h"
#include "header.h"

void lcd_cmd_half_byte(unsigned char cmd)
{
unsigned char tmp;
	
    /*Set RS = 0, E = 1, send the hi 4 bits*/
	tmp = (cmd & ~(1<<RS)) | (1<<E);
	PORT = tmp;
	asm("nop");
	PORT = tmp & ~(1<<E);
	asm("nop");
}

void lcd_cmd_full_byte(unsigned char cmd)
{
unsigned char tmp;

    /*send the first 4 bits*/
	tmp = (cmd & ~(1<<RS)) | (1<<E);
	PORT = tmp;
	asm("nop");
	PORT = tmp & ~(1<<E);
	asm("nop");
	_delay_us(50);// Задержка для выполнения команды контроллером ЖКИ

    /*send the second 4 bits*/
	tmp = ((cmd<<4) & ~(1<<RS)) | (1<<E);
	PORT = tmp;
	asm("nop");
	PORT = tmp & ~(1<<E);
	asm("nop");
	_delay_us(50);
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
    /*Только на видимой части LCD*/
    if((x > 1) || (y > 15)) return;

    if(x == 0) lcd_cmd_full_byte( y | (1<<7) );
	else
	{
		x = 0x40;
		x = x+y;
		lcd_cmd_full_byte(x | (1<<7));
	}	
}

void lcd_clr(void)
{
    /*return home an address 00 on LCD*/
	lcd_cmd_full_byte(0b10000000);
	_delay_ms(2);
    /*Clear the display*/
	lcd_cmd_full_byte(0b00000001);
	_delay_ms(50);
}

void lcd_putchar(unsigned char data)
{
unsigned char tmp;

    /*Вывод первой половины байта данных*/
	tmp = (data | (1<<RS)) | (1<<E);
	PORT = tmp;
	asm("nop");
	PORT = tmp & ~(1<<E);
	asm("nop");
	_delay_ms(2);

    /*Вывод второй половины байта данных*/
	tmp = ((data<<4) | (1<<RS)) | (1<<E);
	PORT = tmp;
	asm("nop");
	PORT = tmp & ~(1<<E);
	asm("nop");
	_delay_ms(2);
}

void lcd_str(char *s)
{
	while(*s)
		lcd_putchar(*s++);
}

void lcd_init(void)
{
DDR = 0xFC;
PORT = 0x00;
_delay_ms(2000);

    /* First function set */
	lcd_cmd_half_byte(0b00110000);
	_delay_ms(5);
    /* Second function set */
	lcd_cmd_half_byte(0b00110000);
	_delay_ms(100);
    /* Third function set */
	lcd_cmd_half_byte(0b00110000);
	_delay_ms(2);
    /* Last function set */
	lcd_cmd_half_byte(0b00100000);
	_delay_ms(2);
	
    /* Function set */
	lcd_cmd_full_byte(0x28);
    /* Display on/off */
	lcd_cmd_full_byte(0b00001100);
    /* Clear display */
	lcd_cmd_full_byte(0b00000001);
    /* Entry mode */
	lcd_cmd_full_byte(0x06);
}
