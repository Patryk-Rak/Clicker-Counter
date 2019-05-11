#include <msp430.h>
#include <driverlib.h>
#include <string.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myLcd.h"

main()
{

#define RED_ON 0x0007 // P1.0 is the Red LED
#define GREEN_ON 0x80 // GREEN LED
#define RED_OFF 0xFFFE
#define GREEN_OFF 0xFF7F
#define BUTTON1 0x02
#define BUTTON2 0x04
#define DEVELOPMENT 0x5A80 // Stop the watchdog timer
#define ENABLE_PINS 0xFFFE // Required to use inputs and outputs
//#define ACLK 0x0100 // Timer_A ACLK source
//#define UP 0x0010 // Timer_A UP mode
//#define TAIFG 0x0001 // Used to look at Timer A Interrupt FlaG

signed long i=0; // Number to be displayed
unsigned long j=0; // For delay
int clickercounter = 0;

void ScrollWords(char words[250]); // Works for messages of up to 250 characters

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
initGPIO(); // Initialize General Purpose Inputs and Outputs
initClocks(); // Initialize clocks
myLCD_init(); // Initialize Liquid Crystal Display
//ScrollWords("CLICKER COUNTER V1"); // Scroll this message across the LCD



while(1)
    {
//myLCD_displayNumber(i); // Display the number
//i = i+1; // Increment the number
//__delay_cycles(4000000);
//for(j=0;j<123456;j++); // Delay

        if((BUTTON1 & P1IN)==0)                 //PRZYCISK 1
        {
           __delay_cycles(1);
           while((BUTTON1 & P1IN)==0);
               P1OUT = RED_ON;
               clickercounter ++;
               myLCD_displayNumber(clickercounter);

        }
        else
        {
           P1OUT = RED_OFF;
           myLCD_displayNumber(clickercounter);
        }



        if((BUTTON2 & P1IN)==0)                 //PRZYCISK 2
        {
           __delay_cycles(1);
           while((BUTTON2 & P1IN)==0);
               P9OUT = GREEN_ON;
               clickercounter += 10;
               myLCD_displayNumber(clickercounter);

        }
        else
        {
           P9OUT = GREEN_OFF;
           myLCD_displayNumber(clickercounter);
        }



//===========================================================================

        if (clickercounter == 0)
        {
            myLCD_showSymbol(LCD_CLEAR,LCD_EXCLAMATION,0);
        }
        else if(clickercounter%3 == 0)
        {
            myLCD_showSymbol(LCD_TOGGLE,LCD_EXCLAMATION,0);
        }
        else
        {
            myLCD_showSymbol(LCD_CLEAR,LCD_EXCLAMATION,0);
        }


    }
}





void ScrollWords(char words[250])
{
unsigned int length; // Contains length of message to be displayed
unsigned int slot; // Slot to be displayed on LCD (1, 2, 3, 4, 5, or 6)
unsigned int amount_shifted; // Number of times message shifted so far
unsigned int offset; // Used with amount_shifted to get correct character to display
unsigned long delay; // Used to implement delay between scrolling iterations
unsigned char next_char; // Next character from message to be displayed
length = strlen(words); // Get the length of the message stored in words
amount_shifted=0; // We have not shifted the message yet
offset=0; // There is no offset yet
while( amount_shifted < length+7 ) // Loop as long as you haven't shifted all
{ // of the characters off the LCD screen
offset=amount_shifted; // Starting point in message for next LCD update
for(slot = 1;slot<=6;slot++) // Loop 6 times to display 6 characters at a time
{
next_char = words[offset-6]; // Get the current character for LCD slot
if(next_char && (offset>=6) && (offset<=length+6) ) // If character is not null AND
{ // LCD is not filled (offset>=6) AND
// You have not reached end of message
// (offset<=length+6)
myLCD_showChar(next_char,slot); // Show the next character on the LCD
// screen in correct slot
}
else // Else, slot on LCD should be blank
{
myLCD_showChar(' ',slot); // So, add a blank space to slot
}
offset++; // Update as you move across the message
}
__delay_cycles(400000);
//for(delay=0 ; delay<123456 ; delay=delay+1); // Delay between shifts
amount_shifted = amount_shifted + 1; // Update times words shifted across LCD
}
}

