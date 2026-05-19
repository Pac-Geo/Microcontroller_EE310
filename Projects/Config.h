#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

// =====================================================
// CONFIG (PIC18F47K42) - WDT off
// =====================================================
#pragma config FEXTOSC  = OFF
#pragma config RSTOSC   = HFINTOSC_1MHZ
#pragma config CLKOUTEN = OFF
#pragma config CSWEN    = ON
#pragma config FCMEN    = ON
#pragma config MCLRE    = EXTMCLR

#pragma config WDTE     = OFF
#pragma config WDTCCS   = SC

#pragma config BOREN    = ON
#pragma config LPBOREN  = OFF
#pragma config STVREN   = ON
#pragma config PPS1WAY  = ON
#pragma config LVP      = OFF
#pragma config DEBUG    = OFF
#pragma config CP       = OFF

#pragma config MVECEN   = ON

#define _XTAL_FREQ 4000000UL

// =====================================================
// LCD
// =====================================================
#define LCD_RS_LAT   LATCbits.LATC2
#define LCD_E_LAT    LATCbits.LATC3
#define LCD_D4_LAT   LATCbits.LATC4
#define LCD_D5_LAT   LATCbits.LATC5
#define LCD_D6_LAT   LATCbits.LATC6
#define LCD_D7_LAT   LATDbits.LATD4

#define LCD_RS_TRIS  TRISCbits.TRISC2
#define LCD_E_TRIS   TRISCbits.TRISC3
#define LCD_D4_TRIS  TRISCbits.TRISC4
#define LCD_D5_TRIS  TRISCbits.TRISC5
#define LCD_D6_TRIS  TRISCbits.TRISC6
#define LCD_D7_TRIS  TRISDbits.TRISD4

// =====================================================
// BUTTONS
// =====================================================
#define START_TRIS   TRISDbits.TRISD2
#define START_PORT   PORTDbits.RD2

#define RESET_TRIS   TRISBbits.TRISB1
#define RESET_PORT   PORTBbits.RB1

// =====================================================
// OUTPUTS
// =====================================================
#define BUZZ_TRIS    TRISDbits.TRISD0
#define BUZZ_LAT     LATDbits.LATD0

#define READY_TRIS   TRISBbits.TRISB3
#define READY_LAT    LATBbits.LATB3

#define INTLED_TRIS  TRISBbits.TRISB0
#define INTLED_LAT   LATBbits.LATB0

#define L1_TRIS      TRISAbits.TRISA2
#define L1_LAT       LATAbits.LATA2

#define L2_TRIS      TRISAbits.TRISA1
#define L2_LAT       LATAbits.LATA1

#define L3_TRIS      TRISBbits.TRISB5
#define L3_LAT       LATBbits.LATB5

#define L4_TRIS      TRISBbits.TRISB4
#define L4_LAT       LATBbits.LATB4

#define L5_TRIS      TRISCbits.TRISC7
#define L5_LAT       LATCbits.LATC7

// =====================================================
// SERVO
// =====================================================
#define SERVO_TRIS   TRISAbits.TRISA0
#define SERVO_LAT    LATAbits.LATA0

// Servo analog feedback input
#define SERVO_FB_TRIS   TRISAbits.TRISA3

// =====================================================
// SERVO RANGE
// =====================================================
#define SERVO_US_ZERO  390
#define SERVO_US_MAX   2300

// ADC calibration
#define FB_ADC_MIN 120
#define FB_ADC_MAX 890

// =====================================================
// GLOBALS
// =====================================================
volatile uint8_t reset_request = 0;
volatile uint8_t resetting = 0;

volatile uint16_t servo_pulse_us = SERVO_US_ZERO;
volatile uint8_t servo_phase_hi = 0;

uint16_t prng_state = 0xACE1;
#endif