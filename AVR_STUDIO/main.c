/*
 * Short description for main.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 05:11
 *
 * Version 0.1
*/

#include "header.h"

/*Выбор варианта из меню*/
unsigned char mode = 0;

/*Вход и возврат в меню
 *@value 0 - no entry in menu
         1 - in menu now
         2 - выполнение программ после меню, согласно выбранного пункта*/
unsigned char menu = 0;

/*Переменная в помощь :) Для изменения времени
 *@value 0 - no activate
         1 - change time and cursor in position (0,1)
         2..6 - cursor in position in change time...*/
unsigned char change_time = 0;

/*Количество дней в месяце*/
const unsigned char ds1307_daysinmonth [] PROGMEM = {31,28,31,30,31,30,31,31,30,31,30,31};

/*Переменные для времени и даты*/
unsigned char year = 0;
unsigned char month = 0;
unsigned char day = 0;
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;

/*Переменные для АЦП*/
/*Вспомогательная переменная, которая показывает статус измерения
  нуля(линии N) за полуволну
  @value 0 - не измерялось значине
         1 - уже измерялось*/
unsigned char flag = 0;

/*Выхлоп АЦП при измерении основной линии, храним здесь*/
unsigned int ACP = 0;

/*Выхлоп АЦП при измерении линии нуля(N)*/
unsigned int ACP_half = 0;

/*Переменная которая необходима для подсчета среднеквадратического
  значения*/
unsigned long long Asum = 0;

/*Число точек выборки*/
unsigned char N = 0;

/*Результирующее значение измерения напряжения полуволны*/
unsigned int rezult_measuring = 0;

char buf_time[9];//Хранение строки с временем ЧЧ:ММ:СС
char buf_date[9];//Хранение строки с датой ДД.ММ.ГГ
/*Взяли на один байт больше массивы для того чтобы при выводе строки на жки, эти массивы не стали друг за другом в памяти и мы одним выводом строки на жки не вывели случайно 2 массива, вместо одного, делаем "пробелы" между ними, в памяти, пустым байтом, это связано с тем, что функция вывода строки на жки, работает так, что ей нужен указатель и она выводит его содержимое, пока есть содержимое, только нулевой байт и указателя нет, она его не выводит, т.е. если массивы лежат рядом, и нет свободного байта, выведет одну строку не массив, а два массива, в одну строку, что есть ошибка вывода, поэтому мы таким образом резервируем между массивами и после нулевой байт, чтобы они читались как отдельно и их можно было вывести в строку и не было ошибок.*/

FATFS fs;// Объявление объекта FATFS
FRESULT res;// Переменная для возвращаемых значений, библиотеки FAT
BYTE buff[512];// Буфер для временного хранения, перед записью на флешку
WORD br; // Счетчик количества байт которые удалось записать
//BYTE test[10] = {'H', 'E', 'L', 'L', 'O', ' ', 'o', 'k', 0x0D, 0x0A};
//0x0D = \r  0x0A = \n

int main(void)
{
lcd_init();
ds1307_init();
adc_init();
timer_init();
asm("sei");
/*
    //test
	lcd_str("Hello, World");
	lcd_gotoxy(1,0);
	lcd_str("by shtomik");
	_delay_ms(1000);
	lcd_clr();
	lcd_str("after clr");*/

start_t();

    //test
    ds1307_setdate(12, 12, 31, 23, 01, 35);

//смонтировать microSD
res = pf_mount(&fs);
if(res == FR_OK)
{
    //диск смонтирован и мы продолжаем работу
    if(pf_open("test.txt") == FR_OK)
    {
        //открыли файл в корне test.txt
        //...
        pf_lseek(0);
            //заполняем буффер, надо набрать 512 байт инфы и писать
			//иначе будет много лишних пробелов
            //sprintf(buff, "%s\n%s", "Hello,World!", "by_sht");
            //записываем его на карту
            pf_write(buff, 512, &br);
			//финализируем запись
       		pf_write(NULL, 0, &br);
			pf_lseek(512);
			pf_write(buff, 512, &br);
        	//финализируем запись
        	pf_write(NULL, 0, &br);

		lcd_clr();
        lcd_str("ok");
    }
    else
    {
        //file not found or ...
        lcd_str("file not found");
    }
    //демонтируем диск передав функции нулевой указатель
    pf_mount(NULL);
	lcd_clr();
	lcd_str("file write");
}
else
{
    //не удалось смонтировать диск
	lcd_clr();
    lcd_str("not flash");
}

    while( 1 )
    {
        ;
    }

return 0;
}

void sumsquare(unsigned int a, unsigned int b)
{
unsigned long tmp = 0;
        /*0.680 - Коэффициент измерения, переводим выхлоп АЦП в вольты*/
        tmp = (((unsigned long)(a - b))*680)/1000;
        tmp = ( (unsigned long)tmp )*( (unsigned long)tmp );
        Asum = ((unsigned long long)Asum) + tmp;
        ++N;
}

unsigned int isqrt32(unsigned long from)
/*Нахождения корня числа за ~ 300 тактов*/
{
	unsigned long mask = 0x40000000;
	unsigned long sqr = 0, temp;
    do
	{
         temp = sqr | mask;
         sqr >>= 1;
         if( temp <= from )
		 {
             sqr |= mask;
             from -= temp;
         }
     } while( mask >>= 2 );
     if( sqr < from && sqr < 0xFFFF) ++sqr;
return (unsigned int)sqr;
}

ISR(TIMER2_COMP_vect)
/*Прерывание для измерения*/
{
    ACP = start_base_adc();
        if(flag == 0)
        {
           ACP_half = start_half_adc();
           flag = 1;
        }
    if(ACP > (ACP_half + 10))
    {
        sumsquare(ACP,ACP_half);
    }
    if(ACP < (ACP_half - 10))
    {
        sumsquare(ACP_half,ACP);
    }
	if((ACP < (ACP_half + 10)) && (ACP > (ACP_half - 10)))
    /*+ или - 10 делений, это зона нечувствительности +-5В для определения
      перехода через ноль*/
	{
        if( (N > 40) && (N < 50) )
        /*Среднее количество замеров которое должно пройти, для положительного
          значения напряжения, если меньше или больше, значит мы не всю полуволну
          меряем или больше чем полуволна*/
        {
            rezult_measuring = isqrt32( Asum / N );
            flag = 0;
            N = 0;
            Asum = 0;
        }
        else
		{
            flag = 0;
            N = 0;
            Asum = 0;
		}
	}
return;
}

ISR(TIMER0_COMP_vect)
/*Прерывание для кнопок*/
{
    /*
       Menu button, вход в меню и возврат в меню, 1 кнопка(PA0)
    */
	if(!(PIN & (1<<0)))
	{
		_delay_ms(10);
		while(!(PIN & (1<<0)));

		btt_menu();
	}

    /*
      Set button, выполняет пункт меню, сдвиг курсора на следующую
      позицию при изменении времени и даты, 2 кнопка(PA1)
    */
	if(!(PIN & (1<<1)))
	{
		_delay_ms(10);//Задержка для ликвидации дребезгов
		while(!(PIN & (1<<1)));//если кнопка нажата сейчас - asm("nop")

		btt_set();
	}

    /*
       Up button, стрелочка вверх, увеличивает счетчик на 1
       в меню и в режиме изменения времени и даты
       3 кнопка(PA2)
    */
	if(!(PIN & (1<<2)))
	{
		_delay_ms(10);
		while(!(PIN & (1<<2)));

		btt_up();
	}

    /*
       Down button, стрелочка вниз, уменьшает счетчик на 1
       в меню и в режиме изменения времени и даты
       4 кнопка(PA3)
    */
    if(!(PIN & (1<<3)))
    {
        _delay_ms(10);
        while(!(PIN & (1<<3)));

        btt_down();
    }
return;	
}
