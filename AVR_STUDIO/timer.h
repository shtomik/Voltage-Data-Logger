/*
 * Short description for timer.h
 *
 * Author Shtoma Valeriy <shtomavaleriy@gmail.com>
 *
 * Created  2015-05-14 04:08
 *
 * Version 0.1
 *
 * Settings for timers
*/

#ifndef TIMER_H
#define TIMER_H

/*PA0..3 - Кнопки, такой же define в файле button.h
  в случае изменений править в 2-х местах*/
#ifndef PIN
#define PIN	PINA
#endif

/* Prototypes for functions */

void timer_init(void);  /* Функция инициализации таймера Т0 u T2 */
void start_t(void);             /* Start the timer T0 */
void stop_t(void);             /* Stop the timer T0 */

#endif /* !TIMER_H */
