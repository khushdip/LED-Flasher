/* 
 * File:   cFlasher.c
 * Author: Khushdip Nijjar
 *
 * Created on May 30, 2019, 12:38 PM
 */

/*********************************************************************
Module:
main()

Description : Program turns on LED lights

Hardware Notes:
* PIC used
    * eg. PIC16F684 operating at 4 MHz, Tosc = 0.25 us
* I/O ports used and hardware attached
    * eg. Output:
                    RA5 - pin 2 connected to LED
                    RA0 - pin 13 connected to LED
                    RC5 - pin 5 connected to LED
                    RC4 - pin 6 connected to LED
                    RC3 - pin 7 connected to LED
                    RC2 - pin 8 connected to LED
                    RC1 - pin 9 connected to LED
                    RC0 - pin 10 connected to LED
        Input : Two switches on pin 11 (RA2) and pin 3 (RA4)
********************************************************************/

/*******************************************************************
        Include Files
********************************************************************/
#include "pic16f684.h"
#include "xc.h"

/*******************************************************************
        Symbolic Constants used by main()
********************************************************************/
#define TRUE 1
#define FALSE 0
#define ON 0
#define OFF 1
#define SWITCH0 RA2
#define SWITCH1 RA4
//PORTC LEDs
#define LED1 0b000001
#define LED2 0b000010
#define LED3 0b000100
#define LED4 0b001000
#define LED5 0b010000
#define LED6 0b100000
//PORTA LEDs
#define LED7 0b000001
#define LED8 0b100000
#define LED_OFF 0b000000
// _XTAL_FREQ must be defined for __delay_ms() macro
#define _XTAL_FREQ 4000000

/*******************************************************************
Local Function Prototypes
********************************************************************/
void Initialize(void); // Initialize peripheral SFRs and global variables
void msDelay(unsigned count); // Call the assembler __delay_ms macro
void turnOnAll(void);
void turnOffAll(void);
void patternOne(void);
void patternTwo(void);
void endToEnd(void);
void doublePattern(void);
void alternateDouble(int multiplier);
void rotateDouble(int multiplier);
void rotateDoubleBackwards(void);
void circlePattern(int multiplier);
void flashAllOn(void);
void flashAlternating(int multiplier);
void diagonalPattern(int multiplier);
void swipe(void);
void crash(void);
/*** Configuration Bit Settings *************************************************
* WDTE : Watch Dog Timer, PWRTE : Power Up Timer, CP : Code Protection, BOREN : Brown Out Detect
* FCMEN : Fail Safe Clock Monitor, MCLRE : MCLR Pin Function (pin 4), CPD : Data Code Protect
* IESO : Internal External Switchover mode,
* FOSC : Oscillator selection - HS : High Speed, LP : Low Power, XT : External crystal oscillator
*                               - INTOSCIO : Internal oscillator and I/O function on pin 4
*/
#pragma config WDTE=0,PWRTE=0,CP=1,BOREN=0,FCMEN=0,MCLRE=0,CPD=1,IESO=0,FOSC=INTOSCIO

/*******************************************************************
 * Global Variable Declarations
********************************************************************/

/*******************************************************************
main() function
********************************************************************/
int main (void)
{
 Initialize(); // Initialize the PIC hardware
 while (TRUE){ //loop forever
    
    if (SWITCH0 == OFF && SWITCH1 == OFF) {
        PORTC = LED_OFF; // turn off all LEDs
        PORTA = LED_OFF;
    }
    if (SWITCH0 == ON && SWITCH1 == OFF) {
        patternOne();
    }
    if (SWITCH0 == OFF && SWITCH1 == ON) {
        patternTwo();
    }

 }
}
/*******************************************************************
List of Functions
********************************************************************/
/*******************************************************************************
* Function: void Initialize(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: Initializes the microcontroller, the peripherals
* used in the application and any global variables
* used by multiple functions.
* Note: None
******************************************************************************/
void Initialize(void)
{
 CMCON0 = 7; // Turn off Comparators
 ANSEL = 0; // Make all PORTS Digital
 TRISA = 0b010100; // Setup RA2,RA4 as inputs, rest are outputs
 TRISC = 0b000000; // Setup all of PORTC pins to be outputs
}
/*******************************************************************************
* Function: void msDelay(unsigned count)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: Calls the assembler macro __delay_ms(value) in a loop
* Note: None
******************************************************************************/
void msDelay (unsigned count)
{
 for(int i = 0; i < count; i++){
 __delay_ms(1);
 }
}
/*******************************************************************************
* Function: void turnOnAll(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs are all on
* Note: None
******************************************************************************/
void turnOnAll(void)
{
    PORTA = LED7 | LED8;
    PORTC = LED1 | LED2 | LED3 | LED4 | LED5 | LED6;
}
/*******************************************************************************
* Function: void turnOffAll(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs are all off
* Note: None
******************************************************************************/
void turnOffAll(void) {
    PORTA = LED_OFF;
    PORTC = LED_OFF;
}
/*******************************************************************************
* Function: void patternOne(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void patternOne(void)
{
    //30 second pattern
    for(int i = 0; i < 3; i++) {
        circlePattern(i);
        for (int j = 0; j < 3; j++) {
            circlePattern(j);
            turnOnAll();
        }
        swipe();
    }
    flashAllOn();
    flashAlternating(0);
    endToEnd();
    doublePattern();
    swipe();
    endToEnd();
    rotateDouble(0);
    swipe();
    crash();
    rotateDouble(0);
}
/*******************************************************************************
* Function: void patternTwo(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void patternTwo(void)
{
    //60 second pattern
    flashAllOn();
    for (int i = 0; i < 2; i++) {
        diagonalPattern(i);
        turnOnAll();
        msDelay(100);
    }
    for (int i = 0; i < 3; i++) {
        diagonalPattern(i);
        swipe();
        doublePattern();
        for (int j = 0; j < 3; j++) {
            circlePattern(i);
        }
        rotateDoubleBackwards();
    }
    for (int i = 0; i < 5; i++) {
        rotateDouble(0);
    }
    for (int i = 0; i < 2; i++) {
        swipe();
        for (int j = 0; j < 4; j++) {
            circlePattern(j);
            alternateDouble(j);
        }
        crash();
        flashAllOn();
    }
    diagonalPattern(0);
    flashAlternating(0);
    circlePattern(0);
    endToEnd();
    flashAllOn();
}
/*******************************************************************************
* Function: void endToEnd(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void endToEnd(void)
{
    PORTA = LED7;
    PORTC = LED_OFF;
    msDelay(100);
    PORTC = LED1;
    msDelay(100);
    PORTC = LED1 | LED2;
    PORTA = LED_OFF;
    msDelay(100);
    PORTC = LED2 | LED3;
    msDelay(100);
    PORTC = LED3 | LED4;
    msDelay(100);
    PORTC = LED4;
    PORTA = LED8;
    msDelay(100);
    PORTC = LED_OFF;
    msDelay(100);
    PORTC = LED4;
    msDelay(100);
    PORTC = LED4 | LED5;
    PORTA = LED_OFF;
    msDelay(100);
    PORTC = LED5 | LED6;
    msDelay(100);
    PORTC = LED1 | LED6;
    msDelay(100);
    PORTC = LED1;
    msDelay(100);
    PORTA = LED7;
    msDelay(100);
    //backwards
    PORTA = LED_OFF;
    msDelay(100);
    PORTC = LED1 | LED6;
    msDelay(100);
    PORTC = LED5 | LED6;
    msDelay(100);
    PORTC = LED4 | LED5;
    msDelay(100);
    PORTC = LED4;
    PORTA = LED8;
    msDelay(100);
    PORTC = LED_OFF;
    msDelay(100);
    PORTC = LED4;
    msDelay(100);
    PORTC = LED3 | LED4;
    PORTA = LED_OFF;
    msDelay(100);
    PORTC = LED2 | LED3;
    msDelay(100);
    PORTC = LED1 | LED2;
    msDelay(100);
    PORTC = LED1;
    msDelay(100);
    PORTA = LED7;
    msDelay(100);
}
/*******************************************************************************
* Function: void doublePattern(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void doublePattern(void)
{
    turnOffAll();
    msDelay(150);
    PORTA = LED7 | LED8;
    PORTC = LED1 | LED4;
    msDelay(150);
    PORTA = LED_OFF;
    PORTC = LED2 | LED3 | LED5 | LED6;
    msDelay(150);
}
/*******************************************************************************
* Function: void rotateDouble(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void rotateDouble(int multiplier)
{
    PORTA = LED7;
    PORTC = LED1;
    msDelay(150 - multiplier * 30);
    PORTA = LED_OFF;
    PORTC = LED2 | LED3;
    msDelay(150 - multiplier * 30);
    PORTA = LED8;
    PORTC = LED4;
    msDelay(150 - multiplier * 30);
    PORTC = LED5 | LED6;
    PORTA = LED_OFF;
    msDelay(150 - multiplier * 30);
    
    for (int k = 0; k < 3; k++) {
        PORTC = LED1 | LED4;
        msDelay(100);
        PORTC = LED2 | LED5;
        msDelay(100);
        PORTC = LED3 | LED6;
        msDelay(100);
    }
}
/*******************************************************************************
* Function: void rotateDoubleBackwords(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void rotateDoubleBackwards(void)
{
    PORTA = LED_OFF;
    PORTC = LED5 | LED6;
    msDelay(150);
    PORTA = LED7;
    PORTC = LED1;
    msDelay(150);
    PORTC = LED2 | LED3;
    PORTA = LED_OFF;
    msDelay(150);
}
/*******************************************************************************
* Function: void alternateDouble(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void alternateDouble(int multiplier)
{
    turnOffAll();
    msDelay(150 - multiplier * 30);
    PORTC = LED1 | LED2;
    msDelay(150 - multiplier * 30);
    PORTC = LED2 | LED3;
    msDelay(150 - multiplier * 30);
    PORTC = LED3 | LED4;
    msDelay(150 - multiplier * 30);
    PORTC = LED4 | LED5;
    msDelay(150 - multiplier * 30);
    PORTC = LED5 | LED6;
    msDelay(150 - multiplier * 30);
    PORTC = LED1 | LED6;
    msDelay(150 - multiplier * 30);
}
/*******************************************************************************
* Function: void circlePattern(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void circlePattern(int multiplier)
{
    turnOffAll();
    msDelay(100 - multiplier * 30);
    PORTC = LED1;
    msDelay(100 - multiplier * 30);
    PORTC = LED2;
    msDelay(100 - multiplier * 30);
    PORTC = LED3;
    msDelay(100 - multiplier * 30);
    PORTC = LED4;
    msDelay(100 - multiplier * 30);
    PORTC = LED5;
    msDelay(100 - multiplier * 30);
    PORTC = LED6;
    msDelay(100 - multiplier * 30);
    PORTC = LED1;
    
    turnOffAll();
    msDelay(100 - multiplier * 30);
    PORTC = LED1;
    msDelay(100 - multiplier * 30);
    PORTC = LED6;
    msDelay(100 - multiplier * 30);
    PORTC = LED5;
    msDelay(100 - multiplier * 30);
    PORTC = LED4;
    msDelay(100 - multiplier * 30);
    PORTC = LED3;
    msDelay(100 - multiplier * 30);
    PORTC = LED2;
    msDelay(100 - multiplier * 30);
    PORTC = LED1;
}
/*******************************************************************************
* Function: void flashAllOn(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void flashAllOn(void) {
    for (int k = 300; k > 30; k -= 30) {
        turnOnAll();
        msDelay(k);
        turnOffAll();
        msDelay(k);
    }
}
/*******************************************************************************
* Function: void flashAlternating(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void flashAlternating(int multiplier)
{
    turnOffAll();
    msDelay(150 - multiplier * 30);
    PORTC = LED1 | LED3 | LED5;
    msDelay(150 - multiplier * 30);
    PORTC = LED2 | LED4 | LED6;
    msDelay(150 - multiplier * 30);
}
/*******************************************************************************
* Function: void diagonalPattern(int multiplier)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void diagonalPattern(int multiplier)
{
    turnOffAll();
    msDelay(100 - multiplier * 30);
    PORTA = LED7;
    msDelay(100 - multiplier * 30);
    PORTA = LED_OFF;
    PORTC = LED1;
    msDelay(150 - multiplier * 30);
    PORTC = LED4;
    msDelay(150 - multiplier * 30);
    PORTC = LED_OFF;
    PORTA = LED8;
    msDelay(150 - multiplier * 30);
    //backwards
    turnOffAll();
    msDelay(100 - multiplier * 30);
    PORTA = LED8;
    msDelay(100 - multiplier * 30);
    PORTA = LED_OFF;
    PORTC = LED4;
    msDelay(150 - multiplier * 30);
    PORTC = LED1;
    msDelay(150 - multiplier * 30);
    PORTC = LED_OFF;
    PORTA = LED7;
    msDelay(150 - multiplier * 30);
}

/*******************************************************************************
* Function: void swipe(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void swipe(void)
{
    turnOffAll();
    msDelay(150);
    PORTC = LED2 | LED3;
    msDelay(150);
    PORTA = LED7 | LED8;
    PORTC = LED1 | LED4;
    msDelay(150);
    PORTA = LED_OFF;
    PORTC = LED5 | LED6;
    msDelay(150);
    
    //backwards
    turnOffAll();
    msDelay(150);
    PORTC = LED5 | LED6;
    msDelay(150);
    PORTA = LED7 | LED8;
    PORTC = LED1 | LED4;
    msDelay(150);
    PORTA = LED_OFF;
    PORTC = LED2 | LED3;
    msDelay(150);
}
/*******************************************************************************
* Function: void crash(void)
* PreCondition: None
* Input: None
* Output: None
* Side Effects: None
* Overview: LEDs flash in attractive pattern
* Note: None
******************************************************************************/
void crash(void)
{
    turnOffAll();
    msDelay(150);
    PORTA = LED7 | LED8;
    msDelay(130);
    PORTC = LED1 | LED4;
    msDelay(110);
    PORTC = LED1 | LED2 | LED3 | LED4 | LED5 | LED6;
    msDelay(100);
    PORTC = LED1 | LED4;
    msDelay(110);
    PORTC = LED_OFF;
    msDelay(130);
    for (int k = 0; k < 3; k++) {
        PORTA = LED_OFF;
        msDelay(150);
        PORTA = LED7 | LED8;
        msDelay(150);
    }
}