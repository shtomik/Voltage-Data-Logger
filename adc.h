/*
 * Short description for adc.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 01:09
 *
 * Version 0.1
*/

#ifndef ADC_H
#define ADC_H

/* Prototypes for functions */
void adc_init(void);             /* Initializing of ADC */
unsigned int start_base_adc(void); /*Старт измерения L1
                                     @return ADC*/
unsigned int start_half_adc(void); /*Старт измерения N
                                     @return ADC*/
#endif /* !ADC_H */
