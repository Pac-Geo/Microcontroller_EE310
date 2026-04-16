/*
 * File:   Init.h
 * Author: Geovani Palomec 
 *
 * Created on April 13, 2026, 2:00 PM
 */

#ifndef Initialization_H
#define Initialization_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/* =========================
   PIN DEFINITIONS
   ========================= */

/* System LED pin*/
#define SYS_LED_LAT     LATAbits.LATA0      // writes to the LED output pin
#define SYS_LED_TRIS    TRISAbits.TRISA0    

#define PR1_PORT        PORTAbits.RA1       // reads the logic level on PR1
#define PR1_TRIS        TRISAbits.TRISA1    // configures PR1 pin directio

#define PR2_PORT        PORTAbits.RA2       // read logic level on PR2
#define PR2_TRIS        TRISAbits.TRISA2    // configures PR2 pin direction

/* Emergency switch pin*/
#define EMG_SW_PORT     PORTBbits.RB0      // reads whether emergency is pressed
#define EMG_SW_TRIS     TRISBbits.TRISB0    // configures the emergency pin

/* Relay control pin */
#define RELAY_LAT       LATBbits.LATB1      
#define RELAY_TRIS      TRISBbits.TRISB1    // configures the relay control pin 
#define CONF_BUZZER_LAT   LATBbits.LATB2
#define CONF_BUZZER_TRIS  TRISBbits.TRISB2

/* 7-segment display port*/
#define SEG_PORT_LAT    LATD              // sends  pattern to display
#define SEG_PORT_TRIS   TRISD             

/*Keypad ports */

#define ROW1 LATCbits.LATC2
#define ROW2 LATCbits.LATC3
#define ROW3 LATCbits.LATC4
#define ROW4 LATCbits.LATC5

#define COL1 PORTCbits.RC6
#define COL2 PORTCbits.RC7
#define COL3 PORTBbits.RB3

/* -----------------------------
   LOGIC OPTIONS
   --------------------------------------- */

#define SENSOR_ACTIVE_LOW   1            

#define RELAY_ACTIVE_LOW    1               // defines logic level( ON)

#define SEG7_TYPE  1                     

/* -------------------------------
  CONSTANTS
   ---------------------------------- */

//#define SECRET_CODE_PR1         3U          // 1st Secrete VALUE
//#define SECRET_CODE_PR2         2U          // 2nd secrete VALUE

#define LOOP_DELAY_MS           10U         // loop delay unit
#define DIGIT_DONE_TIMEOUT_MS   1500U       
#define WRONG_CODE_ON_MS        2000U       
#define UNLOCK_ON_MS            3000U       
#define DEBOUNCE_MS             60U        
#define CONF_BEEP_ON_MS   120U
#define CONF_BEEP_OFF_MS  120U
#define CONF_BEEP_COUNT   2U
#define WRONG_CONFIRM_BUZZ_MS   2000U

#define DIGIT_DONE_TIMEOUT_TICKS   (DIGIT_DONE_TIMEOUT_MS / LOOP_DELAY_MS) 
#define DEBOUNCE_TICKS             (DEBOUNCE_MS / LOOP_DELAY_MS)          

/* -------------------
   STATES
   ------------------- */

typedef enum
{
    SET_CODE_PR1 = 0,
    SET_CODE_PR2,
    waitFor_PR1,
    STATE_WAIT_PR2,             // system waitis for the second code entry PR2
    STATE_CHECK_CODE,           // system has both inputs and compares
    Correct_Secret_Code,        // the entered values matched the secret code
    Wrong_Secret_Code,          // the entered values did not match
    Emergency_Pressed           // emergency switch was pressed
} system_state_t;

/* ------------------------------
   GLOBAL FLAGS / VARIABLES
   --------------------------------- */

extern volatile uint8_t Check_Emergency_SW; 

extern uint8_t PR1_Count;                   // stores PR1 triggers 
extern uint8_t PR2_Count;                   // stores PR2 triggers 

extern uint16_t PR1_DONE;                   
extern uint16_t PR2_DONE;                   

extern uint8_t PR1_Debounce;               
extern uint8_t PR2_Debounce;               

extern bool PR1Prev;                        // remembers previous PR1 state 
extern bool PR2Prev;                        // remembers previous PR2 state 

extern system_state_t SystemState;          // stores the current program state

extern uint8_t Stored_PR1;
extern uint8_t Stored_PR2;
#endif