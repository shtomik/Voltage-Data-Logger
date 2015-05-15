/*
 * Short description for ds1307.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 02:32
 *
 * Version 0.1
 *
 * Settings of iic bus and ds1307 RTC
*/

#ifndef DS1307_H
#define DS1397_H

/* Device address */
#define DS1307_ADDR (0x68<<1)
/* Reading from iic device in functions start or rep_start */
#define I2C_R   1
/* Writing to iic device */
#define I2C_W   0

/* Prototypes for functions */

void ds1307_init(void);      /*Инициализация устройства*/

/* Transform decimal value to BCD */
unsigned char ds1307_dec2bcd(unsigned char val);

/* Transform BCD value to decimal */
//static unsigned char ds1307_bcd2dec(unsigned char val);

/* Get number of days... */
//static unsigned char ds1307_date2days(unsigned char y, unsigned char m, unsigned char d);

unsigned char ds1307_getdayofweek(unsigned char y, unsigned char m, unsigned char d);

/* Set date */
unsigned char ds1307_setdate(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second);

/* Get date */
void ds1307_getdate(unsigned char *year, unsigned char *month, unsigned char *day, unsigned char *hour, unsigned char *minute, unsigned char *second);

#endif /* !DS1307_H */
