/*
 * Short description for timer-2.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 04:13
 *
 * Version 0.1
*/

#include "timer-2.h"
#include "header.h"

void start_t2(void)
{
    TCNT2 = 0x00;//счетчик в ноль
    SFIOR = 0b00000010;//Сброс предделителя и начало отсчета T2
    TCCR2 = 0b00001101;//Установка предделителя и СТС режим, /128
}

void stop_t2(void)
{
    TCCR2 = 0x00;
}
