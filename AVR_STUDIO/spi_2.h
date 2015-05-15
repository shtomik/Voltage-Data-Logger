#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   6 
#define SPI_MOSI   5
#define SPI_SCK    7
#define SPI_SS     4

/*_______________ макросы ____________________*/


/*запретить SPI устройство*/
#define SPI_DisableSS_m(ss)  do{SPI_PORTX |= (1<<(ss)); }while(0)

/*разрешить SPI устройство*/
#define SPI_EnableSS_m(ss)   do{SPI_PORTX &= ~(1<<(ss)); }while(0)

/*статус SPI устройства*/
#define SPI_StatSS_m(ss)    (!(SPI_PORTX & (1<<(ss))))

/*передать байт данных по SPI*/
#define SPI_WriteByte_m(data)  do{ SPDR = data; while(!(SPSR & (1<<SPIF))); }while(0)

/*прочитать байт данных по SPI*/
#define SPI_ReadByte_m(data)  do{ SPDR = 0xff; while(!(SPSR & (1<<SPIF))); data = SPDR;}while(0)


/* ______________ встраиваемые функции _____________*/


/*получить байт данных по SPI*/
inline static unsigned char SPI_ReadByte_i(void){
   SPDR = 0xff;
   while(!(SPSR & (1<<SPIF)));
   return SPDR;   
}

/*  _________________ функции ________________ */

/*инициализаци€ SPI модул€*/
void SPI_Init(void); 

/*отправить байт данных по SPI*/
void SPI_WriteByte(unsigned char data); 

/*получить байт данных по SPI*/
unsigned char SPI_ReadByte(void);

/*отправить и получить байт данных по SPI*/
unsigned char SPI_WriteReadByte(unsigned char data);

/*отправить несколько байт данных по SPI*/
void SPI_WriteArray(unsigned char num, unsigned char *data);

/*отправить и получить несколько байт данных по SPI*/
void SPI_WriteReadArray(unsigned char num, unsigned char *data);


#endif //SPI_H
