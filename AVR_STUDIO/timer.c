/*
 * Short description for timer.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 04:08
 *
 * Version 0.1
*/

#include "timer.h"
#include "header.h"

void timer_init(void)
{
OCR0 = 155;//Число совпадения, проверять кнопки 100 раз в секунду T0
OCR2 = 25;//Число совпадения, для запуска измерения точки синусоиды
          //50 раз за полупериод, 5000 раз в 1 секунду
TIMSK = 0b10000010;//Разрешить прерывание по совпадению T0 u T2
}

void start_t(void)
{
	TCNT0 = 0x00;//счетчик в ноль :)
	SFIOR = 0b00000001;//Сброс предделителя и начало отсчета
	TCCR0 = 0b00001101;//Установка предделителя и СТС режима, /1024
}

void stop_t(void)
{
	TCCR0 = 0x00;
}
