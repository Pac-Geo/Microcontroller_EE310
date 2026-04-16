//---------------------------------
// Title: Interfacing with Sensors
//---------------------------------
// Author: Geovani Palomec
// Date: April 15, 2026
// Versions: 3
//       V3: Rewrote entire code to ensure proper hardware operations
// -----------------------------------------------------------
// Setup:   PIC18F47K42 Curiosity Board
// Compiler: xc8, V6.30
// File Dependencies / Libraries: 
//       Config.h, Init.h, Func.h, xc8.h, stdint.h, stdbool.h
// ------------------------------------------------------------
 
// Purpose:
//      This program controls a motor via the sensing interfaces using a
//      PIC18F47K42 Curiosity Board.
//          
// 1. When the system is enabled (power is on), DP LED(SYS_LED) is on.
// 2. The program contains a pre-selected code called the SECRET_CODE  
// 3. The system operates using two photoresistors (PR1 and PR2) switches.  
// 4. When the system is enabled, the user is be able to enter a SECRET_CODE 
//    using the touchless switches by covering the photoresistors.
//         - A two beep mechanism lets the user know that the inputs are entered
// 5. If the entered SECRET_CODE is correct, then the motor is turned on. 
// 6. If the entered SECRET_CODE is incorrect, the buzzer is turned on. 
// 7. The system has an Emergency switch which is used as an interrupt. 
//    If the Interrupt is enabled, the buzzer makes a distinct melody noise 
//         - The interrupt signal stops the main program when it is asserted. 
// 8. The system has one seven-segment display. Everytime a digit is entered, 
//    meaning a number is entered using the touchless switch PRx, 
//    then the number is displayed.  
// 9. The program has THREE specific header files. 
//      - One including all the configuration words, 
//      - one including all the initialization values, 
//      - one including all your functions.
// Inputs:
// RA1 : PR1 touchless sensor input
// RA2 : PR2 touchless sensor input
// RB0 : Emergency switch input
//
// Outputs:
//RA0      : System LED
// RB1      : Relay module control
// RD0-RD6  : 7-segment display outputs


#include "Config.h"
#include "Init.h"
#include "Func.h"

// -----------------------------------
//   GLOBAL VARIABLES
// -----------------------------------

volatile uint8_t Check_Emergency_SW = 0; 

uint8_t PR1_Count = 0;                   // tracks how many times PR1's activate
uint8_t PR2_Count = 0;                   // tracks how many times PR2's activate

uint16_t PR1_DONE = 0;                   
uint16_t PR2_DONE = 0;                   

uint8_t PR1_Debounce = 0;                
uint8_t PR2_Debounce = 0;                

bool PR1Prev = false;                    
bool PR2Prev = false;                  

system_state_t SystemState = waitFor_PR1; 

/* ---------------------------------
   7-SEGMENT
   --------------------------------- */

static const uint8_t Seg7_Digits[10] =
{
    0x3F,  //  display  0
    0x06,  // display  1
    0x5B,  // display  2
    0x4F,  //display  3
    0x66,  // display  4
    0x6D,  //  display  5
    0x7D,  //  display  6
    0x07,  // display  7
    0x7F,  // display  8
    0x6F   //  display  9
};

/* ------------------------------
   INTERRUPT SERVICE ROUTINE
   Emergency switch on RB0
   ------------------------------- */

void __interrupt(irq(IRQ_IOC), base(8)) ISR_IOC(void)
{
    if (PIR0bits.IOCIF && IOCBFbits.IOCBF0) // RB0 interrupt-on-change source
    {
        IOCBFbits.IOCBF0 = 0;               // clears the RB0
        PIR0bits.IOCIF = 0;                 // clears the global IOC interrupt
        RELAY_Off();
        
        Check_Emergency_SW = 1;             // emergency button press happened
        SystemState = Emergency_Pressed;    // forces emergency condition

        for (uint8_t i = 0; i < 2; i++) 
        {
            CONF_BUZZER_On();
            __delay_ms(150);
            CONF_BUZZER_Off();
            __delay_ms(100);
        }

        __delay_ms(250);                    

        for (uint8_t i = 0; i < 1; i++) {
            CONF_BUZZER_On();
            __delay_ms(350);
            CONF_BUZZER_Off();
            __delay_ms(100);
        }
    }
}

/* -------------------------------
   MAIN
   ----------------------------------- */

void main(void)                             // program starts here after reset
{
    SYSTEM_Initialize();                    
    Reset_To_Start();                       // clears system values
    SYS_LED_On();                           // turns on the status LED 

    while (1)                               // runs forever
    {
        if (Check_Emergency_SW)             
        {
            EmergencyOn();                  // resets the program 
            continue;                       //
        }

        Update_SensorsAndCounts();          // reads the photoresistors
        Process_System();                   // checks the current state 

        __delay_ms(LOOP_DELAY_MS);          
    }
}

/* --------------------------
   INITIALIZATION
   ------------------------------- */
void SYSTEM_Initialize(void)
{
    GPIO_Initialize();
    Emergency_Initialize();

    RELAY_Off();
    CONF_BUZZER_Off();
    SEG_Clear();
}

void GPIO_Initialize(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;

    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    LATD = 0x00;

    LATBbits.LATB2 = 0;

    SYS_LED_TRIS = 0;
    PR1_TRIS = 1;
    PR2_TRIS = 1;
    EMG_SW_TRIS = 1;
    RELAY_TRIS = 0;
    CONF_BUZZER_TRIS = 0;
    SEG_PORT_TRIS = 0x00;

    WPUBbits.WPUB0 = 1;
}

void Emergency_Initialize(void)             // configures interrupt behaviors
{
    
    IOCBNbits.IOCBN0 = 1;   // interrupt when RB0 goes from HIGH to LOW
    IOCBPbits.IOCBP0 = 0;   // disables rising-edge interrupt

    IOCBFbits.IOCBF0 = 0;                   // clears old interrupt flag
    PIR0bits.IOCIF = 0;                     // clears global interrupt

    PIE0bits.IOCIE = 1;                     

    INTCON0bits.GIEH = 1;                  
    INTCON0bits.GIEL = 1;                  
}

/* -------------------------------
   OUTPUT FUNCTIONS
   ---------------------------------- */

void SYS_LED_On(void)                       // turns on the system LED (DP)
{
    SYS_LED_LAT = 1;                       
}

void SYS_LED_Off(void)                      // turns off the system LED
{
    SYS_LED_LAT = 0;                        
}

void RELAY_On(void)                         // activates the relay module
{
    RELAY_LAT = 0;                          // active low, 0 active
}

void RELAY_Off(void)                        // deactivates the relay module
{
    RELAY_LAT = 1;                          // active low , 1 inactive 

}
void CONF_BUZZER_On(void)
{
    CONF_BUZZER_LAT = 1;
}

void CONF_BUZZER_Off(void)
{
    CONF_BUZZER_LAT = 0;
}

void Beep_ConfirmTwice(void)
{
    for (uint8_t i = 0; i < CONF_BEEP_COUNT; i++)
    {
        CONF_BUZZER_On();
        DelayMs_Blocking(CONF_BEEP_ON_MS);
        CONF_BUZZER_Off();
        DelayMs_Blocking(CONF_BEEP_OFF_MS);
    }
}

void Beep_WrongConfirm(void) 
{
    CONF_BUZZER_On();
    DelayMs_Blocking(WRONG_CONFIRM_BUZZ_MS);
    CONF_BUZZER_Off();
}

void Seg7_Display(uint8_t digit)            
{
    uint8_t pattern = 0x00;                 // start with all segments off 

    if (digit <= 9U)                       
    {
        pattern = Seg7_Digits[digit];       
    }

    SEG_PORT_LAT = pattern;                 
}

void SEG_Clear(void)                        // clears 7-segment display
{

    SEG_PORT_LAT = 0x00;                   
}

/* -----------------------------
   INPUT FUNCTIONS
   ------------------------- */

bool PR1_IsActive(void)                     // checks whether PR1 is triggered
{
    return (PR1_PORT == 0);                 // true when PR1 reads LO 
}

bool PR2_IsActive(void)                     // checks whether PR2 is triggered
{
    return (PR2_PORT == 0);                 //  true when PR2 reads LO
}

/* ----------------------------------
   RESET
   -------------------------------- */

void Reset_InputData(void)                  // clears all entry values 
{
    PR1_Count = 0;                          
    PR2_Count = 0;                          

    PR1_DONE = 0;                           
    PR2_DONE = 0;                           

    PR1_Debounce = 0;                       
    PR2_Debounce = 0;                       

    PR1Prev = false;                        // resets the remembered PR1 
    PR2Prev = false;                        // resets the remembered PR2 
}

void Reset_To_Start(void) 
{
    Reset_InputData();
    SystemState = waitFor_PR1;
    SEG_Clear();
    RELAY_Off();
    CONF_BUZZER_Off();
}

/* ---------------------------------
   SENSOR LOGIC
   --------------------------------- */

void Update_SensorsAndCounts(void)          // reads the sensors PR! & PR2
{
    bool pr1_active = PR1_IsActive();       // gets condition of PR1
    bool pr2_active = PR2_IsActive();       // gets condition of PR2

    if (PR1_Debounce > 0)                  
    {
        PR1_Debounce--;                     
    }

    if (PR2_Debounce > 0)                   
    {
        PR2_Debounce--;                     
    }

    switch (SystemState)                    
    {
        case waitFor_PR1:                   
        {
            if (pr1_active && !PR1Prev && (PR1_Debounce == 0)) //counts PR1 edge
            {
                if (PR1_Count < 4U)         // stops at allowed range
                {
                    PR1_Count++;            
                    Seg7_Display(PR1_Count);// shows first-digit count
                }

                PR1_DONE = 0;               // resets the timeout counter 
                PR1_Debounce = DEBOUNCE_TICKS; // starts the debounce delay 
            }

            if (PR1_Count > 0U)             
            {
                PR1_DONE++;                 
            }

            break;                          
        }

        case STATE_WAIT_PR2:                
        {
            if (pr2_active && !PR2Prev && (PR2_Debounce == 0)) //counts PR2 edge
            {
                if (PR2_Count < 4U)         // stops at allowed range
                {
                    PR2_Count++;            
                    Seg7_Display(PR2_Count);// shows second-digit count 
                }

                PR2_DONE = 0;               // resets the second-input timeout
                PR2_Debounce = DEBOUNCE_TICKS; // starts the debounce timer 
            }

            if (PR2_Count > 0U)             
            {
                PR2_DONE++;                 
            }

            break;                         
        }

        default:                           
            break;
    }

    PR1Prev = pr1_active;                   // stores current PR1 condition 
    PR2Prev = pr2_active;                   // stores current PR2 condition 
}

/* -------------------------------
   STATE MACHINE
   ------------------------- */

void Process_System(void)                   
{
    switch (SystemState)                    
    {
        case waitFor_PR1:                  
        {
            /*
             * The first number is considered complete only after
             * PR1 has been triggered at least once and then stays idle
             * long enough to reach the timeout.
             */
            if ((PR1_Count > 0U) && (PR1_DONE >= DIGIT_DONE_TIMEOUT_TICKS)) 
            {
                Beep_ConfirmTwice();
                SystemState = STATE_WAIT_PR2;
                PR2_DONE = 0;                 // clears PR2 timeout counter
            }
            break;                            
        }

        case STATE_WAIT_PR2:                  
        {
            if ((PR2_Count > 0U) && (PR2_DONE >= DIGIT_DONE_TIMEOUT_TICKS)) 
            {
                Beep_ConfirmTwice();
                SystemState = STATE_CHECK_CODE; 
            }
            break;                           
        }

        case STATE_CHECK_CODE:          // compare against stored secret code
        {
            if ((PR1_Count == SECRET_CODE_PR1) && (PR2_Count == SECRET_CODE_PR2)) 
            {
                SystemState = Correct_Secret_Code; // marks code as correct 
            }
            else
            {
                SystemState = Wrong_Secret_Code;// mismatch sends to wrong code
            }
            break;                                 
        }

        case Correct_Secret_Code:                
        {
            Handle_CorrectCode();                 // success
            Reset_To_Start();                     // clears everything 
            break;                               
        }

        case Wrong_Secret_Code:                   
        {
            Handle_WrongCode();                   //failure
            Reset_To_Start();                     // resets all values 
            break;                                
        }

        case Emergency_Pressed:                   
        {
            EmergencyOn();                        
            break;                                
        }

        default:                                  
        {
            Reset_To_Start();                    
            break;                              
        }
    }
}
/* ------------------------------------
   ACTIONS
   ---------------------------- */

void Handle_CorrectCode(void) {
    RELAY_On();

    for (uint16_t i = 0; i < UNLOCK_ON_MS; i++) {
        if (Check_Emergency_SW) break;
        __delay_ms(1);
    }

    RELAY_Off();
    SEG_Clear();
}

void Handle_WrongCode(void) 
{
    Beep_WrongConfirm();
    SEG_Clear();
}

void EmergencyOn(void)                       // runs after interrupt emergency 
{
    Check_Emergency_SW = 0;                  // clears the emergency flag 
    Reset_To_Start();                        // resets entire program 
}

/* ---------------------------------
   DELAY
   --------------------------------- */

void DelayMs_Blocking(uint16_t ms)           
{
    while (ms--)                           
    {
        __delay_ms(1);                      
    }
}