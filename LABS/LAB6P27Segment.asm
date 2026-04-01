;---------------------
; Title: Keypad
;---------------------
; Program Details:
; The purpose of this program is to check which number has been pressed 
; on the keypad and then turn on PORD accordingly, showing the binary
; representation. 
    
; Inputs: RB3. 4. 6. 7
; Outputs: RD0-7 & RB0,1,2
; Setup: The Curiosity Board

; Date: Mar 16, 2026
; File Dependencies / Libraries: It is required to include the
;   AssemblyConfig.inc in the Header Folder
; Compiler: xc8, V6.30
; Author: Farid Farahmand
    ;Co-Author: Geovani Palomec
; Versions:
;       V2.1: Implemented branches to prevent continuous overwriting scans, 
	    ; rewrote to use LATB only ensuring one column is active at a time, 
	    ; and added a key display to * ?HI?
; Useful links:
; Keypad: https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/ 
; 7Segment: https://www.xlitx.com/datasheet/5011AS.pdf
    ;Key note on 7seg: 
	;RD0 - Pin 7 (A)
	;RD1 - Pin 6 (B)
	;RD2 - Pin 4 (C)
	;RD3 - Pin 2 (D)
	;RD4 - Pin 1 (E)
	;RD5 - Pin 9 (F)
	;RD6 - Pin 10 (G)
	;RD7 - (optional DP, pin 5 or ignore)
    
    
;---------------------
; Initialization
;---------------------
#include "AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Constants
;---------------------
what_button EQU     20h
BYTE01      EQU     0xF2
BYTE02      EQU     0x32
REG00       EQU     0x00
REG01       EQU     0x01
REG02       EQU     0x02
REG10       EQU     0x10

;---------------------
; Program Organization
;---------------------
    PSECT absdata,abs,ovrld

    ORG     0
    GOTO    _setup

    ORG     0020H

;---------------------
; Macros
;---------------------
BYTE    MACRO   REGXX
    LFSR    1,REGXX
    MOVFF   POSTDEC1,REG00
    MOVFF   INDF1,REG00
ENDM

;---------------------
; Setup & Main Program
;---------------------
_setup:
    clrf    what_button
    clrf    WREG
    RCALL   _setupPortD
    RCALL   _setupPortB
    clrf    LATB

_main:
    RCALL   _check_keypad
    RCALL   _display_7seg
    GOTO    _main

;-------------------------------------
; Port Setup Functions
;-------------------------------------
_setupPortD:
    BANKSEL PORTD
    CLRF    PORTD
    BANKSEL LATD
    CLRF    LATD
    BANKSEL ANSELD
    CLRF    ANSELD
    BANKSEL TRISD
    MOVLW   0b00000000
    MOVWF   TRISD
    RETURN

_setupPortB:
    BANKSEL PORTB
    CLRF    PORTB
    BANKSEL LATB
    CLRF    LATB
    BANKSEL ANSELB
    CLRF    ANSELB
    BANKSEL TRISB
    MOVLW   0b11111000
    MOVWF   TRISB
    RETURN

;-------------------------------------
; Keypad Scan Function
;-------------------------------------
_check_keypad:
    movf    what_button, W      ; keep last good key by default

    ; -------------------------
    ; Scan column 1: 1,4,7,*
    ; -------------------------
    bcf     LATB, 0
    bcf     LATB, 1
    bcf     LATB, 2
    bsf     LATB, 0
    nop
    nop
    nop
    nop

    btfsc   PORTB, 3
    bra     key1
    btfsc   PORTB, 4
    bra     key4
    btfsc   PORTB, 7
    bra     key7
    btfsc   PORTB, 6
    bra     keyStar

    ; -------------------------
    ; Scan column 2: 2,5,8,0
    ; -------------------------
    bcf     LATB, 0
    bcf     LATB, 1
    bcf     LATB, 2
    bsf     LATB, 1
    nop
    nop
    nop
    nop

    btfsc   PORTB, 3
    bra     key2
    btfsc   PORTB, 4
    bra     key5
    btfsc   PORTB, 7
    bra     key8
    btfsc   PORTB, 6
    bra     key0

    ; -------------------------
    ; Scan column 3: 3,6,9,#
    ; -------------------------
    bcf     LATB, 0
    bcf     LATB, 1
    bcf     LATB, 2
    bsf     LATB, 2
    nop
    nop
    nop
    nop

    btfsc   PORTB, 3
    bra     key3
    btfsc   PORTB, 4
    bra     key6
    btfsc   PORTB, 7
    bra     key9
    btfsc   PORTB, 6
    bra     keyHash

    ; no new key found
    bcf     LATB, 0
    bcf     LATB, 1
    bcf     LATB, 2
    movwf   what_button
    return

key0:
    movlw   0
    bra     key_done
key1:
    movlw   1
    bra     key_done
key2:
    movlw   2
    bra     key_done
key3:
    movlw   3
    bra     key_done
key4:
    movlw   4
    bra     key_done
key5:
    movlw   5
    bra     key_done
key6:
    movlw   6
    bra     key_done
key7:
    movlw   7
    bra     key_done
key8:
    movlw   8
    bra     key_done
key9:
    movlw   9
    bra     key_done
keyStar:
    movlw   10
    bra     key_done
keyHash:
    movlw   11
    bra     key_done

key_done:
    bcf     LATB, 0
    bcf     LATB, 1
    bcf     LATB, 2
    movwf   what_button
    return

;-------------------------------------
; 7-Segment Display Function
;-------------------------------------
_display_7seg:
    movf    what_button, W

    ; Common Cathode 7-seg
    ; bit0=a bit1=b bit2=c bit3=d bit4=e bit5=f bit6=g

    xorlw   0
    bz      disp0
    movf    what_button, W
    xorlw   1
    bz      disp1
    movf    what_button, W
    xorlw   2
    bz      disp2
    movf    what_button, W
    xorlw   3
    bz      disp3
    movf    what_button, W
    xorlw   4
    bz      disp4
    movf    what_button, W
    xorlw   5
    bz      disp5
    movf    what_button, W
    xorlw   6
    bz      disp6
    movf    what_button, W
    xorlw   7
    bz      disp7
    movf    what_button, W
    xorlw   8
    bz      disp8
    movf    what_button, W
    xorlw   9
    bz      disp9
    movf    what_button, W
    xorlw   10
    bz      disp10   
    movf    what_button, W
    xorlw   11
    bz      disp11

    ; anything else = blank
    clrf    LATD
    return

disp0:
    movlw   0b00111111
    movwf   LATD
    return
disp1:
    movlw   0b00000110
    movwf   LATD
    return
disp2:
    movlw   0b01011011
    movwf   LATD
    return
disp3:
    movlw   0b01001111
    movwf   LATD
    return
disp4:
    movlw   0b01100110
    movwf   LATD
    return
disp5:
    movlw   0b01101101
    movwf   LATD
    return
disp6:
    movlw   0b01111101
    movwf   LATD
    return
disp7:
    movlw   0b00000111
    movwf   LATD
    return
disp8:
    movlw   0b01111111
    movwf   LATD
    return
disp9:
    movlw   0b01101111
    movwf   LATD
    return
    
dispH:
    movlw   0b01110110    ; H = b,c,e,f,g
    movwf   LATD
    return

dispI:
    movlw   0b00000110    ; I ? 1 (b,c)
    movwf   LATD
    return
    
disp10:
    rcall   dispH
    rcall   delay
    rcall   dispI
    rcall   delay
    return

disp11:
    clrf    LATD
    return

delay:
    movlw   0xFF
    
delay1:
    movwf   REG00
    
delay2:
    nop
    nop
    nop
    nop
    nop
    
    decfsz  REG00, F
    bra     delay2
    decfsz  WREG, F
    bra     delay1
    return
   
    END