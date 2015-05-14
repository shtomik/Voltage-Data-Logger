/*
 * Short description for twi.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 02:53
 *
 * Version 0.1
 *
 * Settings iic bus
*/

#ifndef TWI_H
#define TWI_H

#ifndef F_CPU
#define F_CPU   16000000UL
#endif

/* iic clock in HZ */
#define SCL_CLOCK   10000L

/* Prototypes for functions */

void i2c_init(void);             /* Инициализация шины */
void i2c_stop(void);             /* Stop the data transfer */

unsigned char i2c_start(unsigned char addr);
/* Проверка на свободную линию, отправка адреса и начало передачи */
/* addr - address and transfer direction of I2C device
 * @retval 0 - device accessible
 * @retval 1 - failed to access device
*/

void i2c_start_wait(unsigned char addr);
/* Issue a start condition and sends address and transfer direction */
/* If device busy, use ack polling to wait until device ready */
/* addr - address of device */

unsigned char i2c_rep_start(unsigned char addr);
/* Rep_start */

unsigned char i2c_wrt(unsigned char data);
/* Send one byte to I2C device */
/* @retval 0 - write successful, 1 - write failed */
/* data - data byte to be transfered */

unsigned char i2c_readAck(void);
/* Read one byte from device, request more data from device */
/* @return byte read from device */

unsigned char i2c_readNak(void);
/* Read one byte from device, read is followed by a stop condition */
/* @return byte read from device */

#endif /* !TWI_H */
