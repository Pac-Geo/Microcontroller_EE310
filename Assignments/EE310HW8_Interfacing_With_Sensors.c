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
// Versions: 2
//       V2: Implemented secrete code and readign RA6,RA7 capability
// Useful links:
 
//----------------------------
#include "Config.h"
#include "Init.h"
#include "Func.h"
//-----------------------------

//----- Secrete Code Set up ---------
unsigned char SECRET_DIGIT1 = 2;
unsigned char SECRET_DIGIT2 = 3;

unsigned char entered_digit1 = 0;
unsigned char entered_digit2 = 0;
//-----------------------------------

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

unsigned int adc_read(unsigned char channel)
{
    ADPCH = channel;              // choose ADC channel
    __delay_us(10);               // small settling time

    ADCON0bits.GO = 1;            // start conversion
    while(ADCON0bits.GO);         // wait until done

    return ((unsigned int)ADRESH << 8) | ADRESL;
}

#define DARK_THRESHOLD 300   // adjust later if needed

unsigned char is_dark(unsigned char channel)
{
    unsigned int val = adc_read(channel);

    if(val < DARK_THRESHOLD)
        return 1;   // dark
    else
        return 0;   // light
}

void buzzer_double_beep(void)
{
    LATDbits.LATD4 = 1;
    __delay_ms(150);
    LATDbits.LATD4 = 0;
    __delay_ms(150);

    LATDbits.LATD4 = 1;
    __delay_ms(150);
    LATDbits.LATD4 = 0;
}

void display_digit(unsigned char digit)
{
    // Common cathode:
    // 1 = ON, 0 = OFF

    // Turn everything OFF first
    LATDbits.LATD6 = 0;   // A
    LATDbits.LATD7 = 0;   // B
    LATEbits.LATE1 = 0;   // C
    LATCbits.LATC0 = 0;   // D
    LATCbits.LATC1 = 0;   // E
    LATDbits.LATD5 = 0;   // F
    LATBbits.LATB5 = 0;   // G

    LATEbits.LATE0 = 1; // DP / SYS LED always ON

    switch(digit)
    {
        case 1:
            LATDbits.LATD7 = 1;   // B
            LATEbits.LATE1 = 1;   // C
            break;

        case 2:
            LATDbits.LATD6 = 1;   // A
            LATDbits.LATD7 = 1;   // B
            LATCbits.LATC0 = 1;   // D
            LATCbits.LATC1 = 1;   // E
            LATBbits.LATB5 = 1;   // G
            break;

        case 3:
            LATDbits.LATD6 = 1;   // A
            LATDbits.LATD7 = 1;   // B
            LATEbits.LATE1 = 1;   // C
            LATCbits.LATC0 = 1;   // D
            LATBbits.LATB5 = 1;   // G
            break;

        case 4:
            LATDbits.LATD7 = 1;   // B
            LATEbits.LATE1 = 1;   // C
            LATDbits.LATD5 = 1;   // F
            LATBbits.LATB5 = 1;   // G
            break;

        default:
            break;
    }
}

unsigned char read_digit_PR1(void)
{
    unsigned char count = 0;
    unsigned char prev_dark = 0;
    unsigned int timeout = 0;

    while(1)
    {
        unsigned char now_dark = is_dark(0x06);   // RA6 / AN6

        // count one new cover event
        if((now_dark == 1) && (prev_dark == 0))
        {
            if(count < 4)
            {
                count++;
            }
            timeout = 0;
        }

        // once at least one cover happened, wait for no activity
        if(count > 0)
        {
            __delay_ms(50);
            timeout += 50;

            if(timeout >= 1500)
            {
                display_digit(count);
                buzzer_double_beep();
                return count;
            }
        }
        else
        {
            __delay_ms(50);
        }

        prev_dark = now_dark;
    }
}

unsigned char read_digit_PR2(void)
{
    unsigned char count = 0;
    unsigned char prev_dark = 0;
    unsigned int timeout = 0;

    while(1)
    {
        unsigned char now_dark = is_dark(0x07);   // RA7 / AN7

        // count one new cover event
        if((now_dark == 1) && (prev_dark == 0))
        {
            if(count < 4)
            {
                count++;
            }
            timeout = 0;
        }

        // once at least one cover happened, wait for no activity
        if(count > 0)
        {
            __delay_ms(50);
            timeout += 50;

            if(timeout >= 1500)
            {
                display_digit(count);
                buzzer_double_beep();
                return count;
            }
        }
        else
        {
            __delay_ms(50);
        }

        prev_dark = now_dark;
    }
}

int main(void)
{
    SYSTEM_Initialize();
    display_digit(0);
    
    while(1)
    {
        entered_digit1 = read_digit_PR1();
        entered_digit2 = read_digit_PR2();
       
    }

    return 0;
}