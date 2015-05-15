/*
 * Short description for button.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 01:36
 *
 * Version 0.1
 *
 * driver for buttons
*/

#ifndef BUTTON_H
#define BUTTON_H

/*Строки меню*/
#define menu_1  	"Time"
#define menu_2  	"Change Time"
#define menu_3      "Measuring"

/*Количество строк меню*/
#define count_of_menu	3

/*Пояснительный error, сообщает что нужно сделать*/
#define error		"Enter to menu"

#ifndef PIN /*Этот же define есть в timer.h, менять в двух местах*/
#define PIN PINA
#endif

/* Prototypes for functions */
void btt_menu(void);     /*Обработка нажатия кнопки Menu*/
void time_out(void);     /*Вывод даты и времени на ЖКИ
                          в форматированном виде*/
void setup_time(void);   /*Установка времени и вывод его на ЖКИ
                          Выход по кнопке Menu
                          Кнопка Set делает перемещение позиции
                          Up/Down увеличивает значение показателя*/
void btt_set(void);      /*Обработка нажатия кнопки Set
                          Выполняет выбранный пунк меню
                          Сдвиг курсора на следующую позицию
                          при изменении времени и даты*/
void show_menu(void);    /*Вывод названия меню на ЖКИ*/
void set_time(void);     /*Установка времени и даты RTC*/
void btt_up(void);       /*Обработка нажатия кнопки UP*/
void btt_down(void);     /*Обработка нажатия кнопки DOWN*/

#endif /* !BUTTON_H */
