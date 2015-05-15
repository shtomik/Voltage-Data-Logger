/*
 * Short description for header.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 05:11
 *
 * Version 0.1
*/

#ifndef HEADER_H
#define HEADER_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <compat/twi.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "lcd-lib.h"
#include "twi.h"
#include "ds1307.h"
#include "adc.h"
#include "timer-2.h"
#include "button.h"
#include "timer.h"
#include "integer.h"
#include "diskio.h"
#include "pff.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#endif /* !HEADER_H */
