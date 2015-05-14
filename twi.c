/*
 * Short description for twi.c
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 02:53
 *
 * Version 0.1
*/

#include "twi.h"
#include "header.h"

void i2c_init(void)
{
    /* Initialize TWI clock: 100kHz, TWPS = 1 => prescaler = 4 */
    TWSR = 1;
    /*For stable operation, must be > 10*/
    TWBR = ((F_CPU/SCL_CLOCK)-16)/8;
}

void i2c_stop(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    /* wait until stop condition is executed and bus released */
    while( TWCR & (1<<TWSTO) );
}

unsigned char i2c_start(unsigned char addr)
{
unsigned char twst;

    //send start condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    //wait until transmission complited
    while(!(TWCR & (1<<TWINT)));

    //check value of TWI status reg, mask prescaler bits
    twst = TW_STATUS & 0xF8;
    if( (twst != TW_START) && (twst != TW_REP_START) ) return 1;

    //send device address
    TWDR = addr;
    TWCR = (1<<TWINT) | (1<<TWEN);

    //wait until transmission completed and ACK/NACK has been received
    while( !(TWCR & (1<<TWINT)) );

    //check value of TWI Status reg, mask prescaler bits
    twst = TW_STATUS & 0xF8;
    if( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

return 0;
}

void i2c_start_wait(unsigned char addr)
{
unsigned char twst;

    while ( 1 )
    {
        //send Start condition
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

        //wait until transm compl
        while( !( TWCR & (1<<TWINT) ) );

        //check value of register TWI status, mask prescaler bits
        twst = TW_STATUS & 0xF8;
        if( ( twst != TW_START ) && ( twst != TW_REP_START ) ) continue;

        //send device address
        TWDR = addr;
        TWCR = (1<<TWINT) | (1<<TWEN);

        //wait until transmiss compl
        while( !( TWCR & (1<<TWINT) ) );

        //check value of TWI Regis status, mask ...
        twst = TW_STATUS & 0xF8;
        if( (twst == TW_MT_SLA_NACK) || (twst == TW_MR_DATA_NACK) )
        {
            //device busy, send stop condition to terminate write operation
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

            //wait until stop condition is executed and bus released
            while( TWCR & (1<<TWSTO) );

            continue;
        }
        break;
    }
}

unsigned char i2c_rep_start(unsigned char addr)
{
    return i2c_start( addr );
}

unsigned char i2c_wrt(unsigned char data)
{
unsigned char twst;

    //send data to the previously addressed device
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    //wait ...
    while( !( TWCR & (1<<TWINT) ) );

    //check value of TSR(TWI STATUS REGISTER). mask...
    twst = TW_STATUS & 0xF8;
    if( twst != TW_MT_DATA_ACK ) return 1;

return 0;
}

unsigned char i2c_readAck(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while( !( TWCR & (1<<TWINT) ) );

    return TWDR;
}

unsigned char i2c_readNak(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN);
    while( !( TWCR & (1<<TWINT) ) );

    return TWDR;
}
