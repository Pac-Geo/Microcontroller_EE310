/*
 * File:   Func.h
 * Author: Geovani Palomec 
 *
 * Created on April 13, 2026, 2:00 PM
 */

#ifndef Functions_H
#define Functions_H

#include "Config.h"
#include "Init.h"

/* -----------------------------
   FUNCTION PROTOTYPES
   --------------------------- */

void SYSTEM_Initialize(void);        // Init. system 
void GPIO_Initialize(void);          // sets pins
void Emergency_Initialize(void);     // enables emergency interrupt

void Seg7_Display(uint8_t digit);    
void SEG_Clear(void);                

void RELAY_On(void);                 // activates the relay 
void RELAY_Off(void);                // deactivates the relay 

void SYS_LED_On(void);               // turns on system LED (DP) 
void SYS_LED_Off(void);              // turns off the LED if needed

bool PR1_IsActive(void);             // checks the first PR1
bool PR2_IsActive(void);             // checks the second PR2


void Reset_InputData(void);          // clears all
void Reset_To_Start(void);           // Resets prog.


void Update_SensorsAndCounts(void);  // reads the PR's 
void Process_System(void);           

void Handle_CorrectCode(void);       
void Handle_WrongCode(void);         
void EmergencyOn(void);              

void DelayMs_Blocking(uint16_t ms); 

void CONF_BUZZER_On(void);
void CONF_BUZZER_Off(void);
void Beep_ConfirmTwice(void);
void Beep_WrongConfirm(void);
void Beep_Once(void);

uint8_t Keypad_GetKey(void);

#endif