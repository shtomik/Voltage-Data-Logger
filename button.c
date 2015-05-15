/*
 * Short description for button.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 01:36
 *
 * Version 0.1
*/

#include "button.h"
#include "header.h"

/*Описание всех переменных находится в месте их создания*/
extern unsigned char mode;
extern unsigned char menu;
extern unsigned char change_time;

extern unsigned char year;
extern unsigned char month;
extern unsigned char day;
extern unsigned char hour;
extern unsigned char minute;
extern unsigned char second;

extern char buf_time[9];
extern char buf_date[9];

void btt_menu(void)
{
menu = 1;
mode = 0;
change_time = 0;
stop_t2();
    lcd_cmd_full_byte(0b00001100);      /*Убрать курсор с LCD*/
	lcd_clr();
	lcd_str(menu_1);                    /*Output the first menu*/
}

void time_out(void)
{
    lcd_clr();
    /*Считываем с RTC дату или время*/
    ds1307_getdate(&year, &month, &day, &hour, &minute, &second);
    /*Делаем преобразование данных полученых с RTC*/
    sprintf(buf_time, "%02d:%02d:%02d", hour, minute, second);
    /*Создаем строку, которую можно будет вывести на LCD*/
    sprintf(buf_date, "%02d.%02d.%02d", day, month, year);
    /*Вывод даты и времени на LCD*/
    lcd_str(buf_time);
    lcd_gotoxy(1,0);
    lcd_str(buf_date);
}

void setup_time(void)
{
    if(menu == 2)
        change_time = 1;
    if(menu == 2 && (change_time > 0 && change_time < 7))
    {
        lcd_cmd_full_byte(0x0F);    /*Установка мигающего курсора*/
        change_time = 1;
        lcd_gotoxy(0,1);
    }
    if(menu == 0 && change_time == 0)
    {
        lcd_clr();
        lcd_str(error);
    }
}

void btt_set(void)
{
    if(menu == 2 && (change_time > 0 && change_time < 7))
    {
        ++change_time;
        switch(change_time)
        {
            case 2: lcd_gotoxy(0,4); break;//setup the minute
            case 3: lcd_gotoxy(0,7); break;//setup the second
            case 4: lcd_gotoxy(1,1); break;//setup the day
            case 5: lcd_gotoxy(1,4); break;//setup the month
            case 6: lcd_gotoxy(1,7); break;//setup the year
            default:change_time = 1;lcd_gotoxy(0,1);//setup the hour
        }
    }
	if(menu == 1)
    {
		switch(mode)
		{
			case 0: menu = 2;
                    time_out();
			break;
			case 1: menu = 2;
                    time_out();
                    setup_time();
			break;
            case 2: menu = 2;
                    lcd_gotoxy(1,0);
                    lcd_str("...");
                    start_t2();         /*Запуск таймера и измерения*/
            break;
			default:menu = 2;lcd_clr();lcd_str("Error!!!");
		}
	}
	if(menu == 0 && change_time == 0)
	{
		lcd_clr();
		lcd_str(error);
	}
}

void show_menu(void)
{
	mode %= count_of_menu;
	switch(mode)
	{
		case 0: /*first menu*/
		    lcd_clr();
			lcd_str(menu_1);
		break;
		case 1: /*second menu*/
			lcd_clr();
			lcd_str(menu_2);
		break;
        case 2: /*third menu*/
            lcd_clr();
            lcd_str(menu_3);
        break;
		default:lcd_clr();lcd_str("Error!!!");
	}
}

void set_time(void)
{
    /*Установка времени и даты на RTC*/
    ds1307_setdate(year, month, day, hour, minute, second);
    time_out();
    switch(change_time)
    /*Установка курсора для изменения величин*/
    {
        case 1:
            lcd_gotoxy(0,1);
        break;
        case 2:
            lcd_gotoxy(0,4);
        break;
        case 3:
            lcd_gotoxy(0,7);
        break;
        case 4:
            lcd_gotoxy(1,1);
        break;
        case 5:
            lcd_gotoxy(1,4);
        break;
        case 6:
            lcd_gotoxy(1,7);
        break;
        default:lcd_gotoxy(0,1);
    }
}

void btt_up(void)
{
    if(menu == 1)
    {
        if(mode == 0) mode = count_of_menu;
        --mode;
        show_menu();
    }
    if(menu == 2 && (change_time > 0 && change_time < 7))
    {
        switch(change_time)
        /*Увеличение показателя*/
        {
            case 1:
                if(++hour == 24) hour = 0;
            break;
            case 2:
                if(++minute == 60) minute = 0;
            break;
            case 3:
                if(++second == 60) second = 0;
            break;
            case 4:
                if(++day == 32) day = 1;
            break;
            case 5:
                if(++month == 13) month = 1;
            break;
            case 6:
                if(++year == 100) year = 0;
            break;
        }
        set_time();
    }
	if(menu == 0 && change_time == 0)
	{
		lcd_clr();
		lcd_str(error);
	}
}

void btt_down(void)
{
    if(menu == 1)
    {
        ++mode;
        show_menu();
    }
    if(menu == 2 && (change_time > 0 && change_time < 7))
    {
        switch(change_time)
        {
            case 1:
                if(--hour == 255) hour = 23;
            break;
            case 2:
                if(--minute == 255) minute = 59;
            break;
            case 3:
                if(--second == 255) second = 59;
            break;
            case 4:
                if(--day == 0) day = 31;
            break;
            case 5:
                if(--month == 0) month = 12;
            break;
            case 6:
                if(--year == 255) year = 99;
            break;
        }
        set_time();
    }
	if(menu == 0 && change_time == 0)
	{
		lcd_clr();
		lcd_str(error);
	}
}
