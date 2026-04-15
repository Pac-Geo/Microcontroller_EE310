#ifndef INIT_H
#define INIT_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 4000000UL   // placeholder for now

void OSCILLATOR_Initialize(void);
void GPIO_Initialize(void);
void ADC_Initialize(void);
void INTERRUPT_Initialize(void);
void SYSTEM_Initialize(void);

#endif