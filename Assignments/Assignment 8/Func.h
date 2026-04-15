
#ifndef FUNC_H
#define	FUNC_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

unsigned int adc_read(unsigned char channel);
unsigned char is_dark(unsigned char channel);
unsigned char read_digit_PR1(void);
unsigned char read_digit_PR2(void);
void buzzer_double_beep(void);
void display_digit(unsigned char digit);

void motor_on(void);
void motor_off(void);
void buzzer_error(void);

void emergency_siren(void);
void stepper_step_once(void);
void motor_run(void);
#endif
