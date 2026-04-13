//---------------------------------
// Title: Interfacing with Sensors
//---------------------------------

// Program Details: In C programming language
// The purpose of this program is to read from sensors and control actuators
    
// Inputs: RA6, RA7, RB0
// Outputs: RA0-RA4, RB5, RC0-RC1, RD4-RD7, RE0-RE1
// Setup: The Curiosity Board

// Date: April 13, 2026
// File Dependencies / Libraries: 
// Compiler: xc8, V6.30
// Author: Geovani Palomec
// Versions: 1
//       V1.1: Implemented Initialaztions and Init.h file
// Useful links:
 
//----------------------------
#include "Config.h"
#include "Init.h"
#include "Func.h"
//-----------------------------

void OSCILLATOR_Initialize(void)
{

    OSCCON1 = 0x60;   // NOSC = HFINTOSC, NDIV = 1
    OSCFRQ  = 0x02;   // HFINTOSC = 4 MHz
    OSCEN   = 0x00;
}

void GPIO_Initialize(void)
{
    // -------- Latch defaults --------
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    // -------- PORT A Setup --------
    // RA0 = Relay Signal         -> output
    // RA1 = Stepper IN4          -> output
    // RA2 = Stepper IN3          -> output
    // RA3 = Stepper IN2          -> output
    // RA4 = Stepper IN1          -> output
    // RA6 = PhotoResistor1       -> input
    // RA7 = PhotoResistor2       -> input
    TRISA = 0b11000000;
    
    // -------- PORT B Setup --------
    // RB0 = Emergency SW         -> input
    // RB5 = 7seg G               -> output
    TRISB = 0b00000001;

    // -------- PORT C Setup --------
    // RC0 = 7seg D               -> output
    // RC1 = 7seg E               -> output
    TRISC = 0b00000000;

    // -------- PORT D Setup --------
    // RD4 = Buzzer               -> output
    // RD5 = 7seg F               -> output
    // RD6 = 7seg A               -> output
    // RD7 = 7seg B               -> output
    TRISD = 0b00000000;
    
    // -------- PORT E Setup --------
    // RE0 = 7seg DP (SYS)
    // RE1 = 7seg C
    TRISE = 0b00000000;

    // ---------- ADC ----------
    // RA6, RA7 = analog
    ANSELA = 0b11000000;

    // --------- Digital --------
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
}

void ADC_Initialize(void)
{
    //Turn on ADC + set it to read voltages from 0?5V safely
    // Right justified result
    ADCON0 = 0x00;   //Resets ADC control settings  
    ADCON1 = 0x00;   // default reference = Vcc/GND
    ADCON2 = 0x00;
    
    // ADC enable
    ADCLK  = 0x3F;   // Set ADC clock speed
    ADREF  = 0x00;   // Vcc(5v)/GND(0v) as references
    ADCON0bits.ADON = 1; //Activates ADC
}

void INTERRUPT_Initialize(void)
{
    INTCON0bits.GIE = 0;
    INTCON0bits.IPEN = 0;

    PIR1bits.INT0IF = 0;    // clear INT0 flag
    INTCON0bits.INT0EDG = 1;     // rising edge
    PIE1bits.INT0IE = 1;    // enable INT0

    INTCON0bits.GIE = 1;
}

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize();
    GPIO_Initialize();
    ADC_Initialize();
    INTERRUPT_Initialize();
}

int main(void)
{
    SYSTEM_Initialize();

    while(1)
    {
        
    }

    return 0;
}