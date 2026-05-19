#ifndef FUNC_H
#define FUNC_H

#include "Config.h"

// =====================================================
// HELPERS
// =====================================================
void all_seq_leds_off(void);
void buzzer_off(void);
void buzzer_on(void);
void make_line16(char out[17], const char *in);

// =====================================================
// LCD
// =====================================================
void lcd_pulse_enable(void);
void lcd_write4(uint8_t n);
void lcd_cmd(uint8_t c);
void lcd_data(uint8_t d);
void lcd_goto(uint8_t row,uint8_t col);
void lcd_print_16(const char *s16);
void lcd_init(void);

// =====================================================
// TIMER1
// =====================================================
void tmr1_init_1us_freerun(void);
void tmr1_zero(void);

// =====================================================
// ADC
// =====================================================
void adc_init(void);
uint16_t adc_read_an3(void);

// =====================================================
// TIMER0 SERVO
// =====================================================
void tmr0_reload_us(uint16_t us);
void servo_timer0_init(void);

// =====================================================
// RANDOM
// =====================================================
void prng_seed_from_timers(void);
uint16_t prng_next(void);
uint16_t rand_range_ms(uint16_t min_ms,uint16_t max_ms);

// =====================================================
// IOC
// =====================================================
void ioc_init_rb1_reset(void);

// =====================================================
// UI
// =====================================================
void show_ready(void);
void show_started(void);
void false_start_buzz(void);

// =====================================================
// LOGIC
// =====================================================
uint8_t wait_ms_check(uint16_t ms);
uint8_t run_f1_sequence(void);
uint32_t measure_reaction_ms(uint16_t timeout_ms);

uint16_t servo_us_for_reaction_5pt(uint32_t t_ms);

void do_reset_sequence(void);

#endif