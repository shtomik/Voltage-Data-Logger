/*
 * Short description for ds1307.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 02:32
 *
 * Version 0.1
*/

#include "ds1307.h"
#include "header.h"

extern const unsigned char ds1307_daysinmonth [];

void ds1307_init(void)
{
    i2c_init();
    _delay_us(10);
}

unsigned char ds1307_dec2bcd(unsigned char val)
{
    return val + 6 * (val / 10);
}

static unsigned char ds1307_bcd2dec(unsigned char val)
{
    return val - 6 * (val>>4);
}

static unsigned char ds1307_date2days(unsigned char y, unsigned char m, unsigned char d)
{
int days = d;
unsigned char i;

    for( i = 1; i < m; ++i )
        days += pgm_read_byte(ds1307_daysinmonth + i -1);
    if(m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y+3) / 4 - 1;
}

unsigned char ds1307_getdayofweek(unsigned char y, unsigned char m, unsigned char d)
{
    int day = ds1307_date2days(y,m,d);
    return (day + 6) % 7;
}

unsigned char ds1307_setdate(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second)
{
	//sanitize data
	if (second < 0 || second > 59 ||
		minute < 0 || minute > 59 ||
		hour < 0 || hour > 23 ||
		day < 1 || day > 31 ||
		month < 1 || month > 12 ||
		year < 0 || year > 99)
		return 8;

	//sanitize day based on month
	if(day > pgm_read_byte(ds1307_daysinmonth + month - 1))
		return 0;

	//get day of week
    unsigned char dayofweek = ds1307_getdayofweek(year, month, day);

	//write date
	i2c_start_wait(DS1307_ADDR | I2C_W);
	i2c_wrt(0x00);//stop oscillator
	i2c_wrt(ds1307_dec2bcd(second));
	i2c_wrt(ds1307_dec2bcd(minute));
	i2c_wrt(ds1307_dec2bcd(hour));
	i2c_wrt(ds1307_dec2bcd(dayofweek));
	i2c_wrt(ds1307_dec2bcd(day));
	i2c_wrt(ds1307_dec2bcd(month));
	i2c_wrt(ds1307_dec2bcd(year));
	i2c_wrt(0x00); //start oscillator
	i2c_stop();

return 1;
}

void ds1307_getdate(unsigned char *year, unsigned char *month, unsigned char *day, unsigned char *hour, unsigned char *minute, unsigned char *second)
{
	i2c_start_wait(DS1307_ADDR | I2C_W);
	i2c_wrt(0x00);//stop oscillator
	i2c_stop();

	i2c_rep_start(DS1307_ADDR | I2C_R);
	*second = ds1307_bcd2dec(i2c_readAck() & 0x7F);
	*minute = ds1307_bcd2dec(i2c_readAck());
	*hour = ds1307_bcd2dec(i2c_readAck());
	i2c_readAck();
	*day = ds1307_bcd2dec(i2c_readAck());
	*month = ds1307_bcd2dec(i2c_readAck());
	*year = ds1307_bcd2dec(i2c_readNak());
	i2c_stop();
}
