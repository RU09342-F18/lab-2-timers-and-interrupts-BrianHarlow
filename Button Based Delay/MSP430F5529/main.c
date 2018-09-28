#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 26 September 2018
// Microcontroller: MSP430F5529

// Program Description: This program is designed to use a button and an LED on a development board with the MSP430F5529 microcontroller
//                      to set the frequency of the LED. When you push the button and let it go the LED should begin to toggle so that
//                      it is illuminated for the amount of time the button was pressed, and then turned off for the same amount of time.

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    UCSCTL4 = SELA_0;           // Sets ACLK to XT1CLK at 32.768kHz

    P1SEL &= ~0x03;             // Sets P1.0 and P1.1 to be GPIO
    P1DIR |= BIT0;              // Sets P1.0 to be an output
    P1DIR &= ~BIT1;             // Sets P1.1 to be an input

    P1REN |= BIT1;              // Pull-up/pull-down resistor enabled for P1.1
    P1OUT |= BIT1;              // Pull-up/pull-down resistor set to pull-down for P1.1

    P1IE |= BIT1;               // Enables interrupt for P1.1
    P1IES |= BIT1;              // Set interrupt as falling edge of P1.1
    P1IFG &= ~BIT1;             // Clears the interrupt for P1.1

    TA0CTL = TASSEL_1 + ID_2 + MC_1 + TACLR + TAIE; // ACLK set to UP MODE with Timer A interrupt enabled
    TA0CCR0 = 0x19A;                                // Sets CCR0 register to value 0x19A

    _BIS_SR(LPM0_bits + GIE);   // Enter LPM0 and enable global interrupts
}

// ISR
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IES & BIT1)                            // Check condition for if the button is pressed down
    {
        P1IES &= ~BIT1;                          // Change interrupt to trigger on rising edge of P1.1
        TA0CTL = TASSEL_1 + ID_2 + MC_2 + TACLR; // Reset Timer A and set to Continuous Mode
    }
    else
    {
        TA0CTL = TASSEL_1 + ID_2 + MC_0;                // Forces timer into STOP Mode
        TA0CCR0 = TA0R;                                 // Stores time the button was pressed for in CCR0 register
        TA0CTL = TASSEL_1 + ID_2 + MC_1 + TACLR + TAIE; // Sets timer to UP Mode, clears the timer, and enables timer overflow interrupt
        P1IES |= BIT1;                                  // Change interrupt to trigger on falling edge of P1.1
    }
    P1IFG &= ~BIT1;                                     // Clears interrupt flag triggered by button press
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void) {
    // Switch TAIV, automatically resets the flags
    switch(TA0IV)
    {
    case 2:                 // Capture/Compare detect for CCR1

        break;
    case 4:                 // Capture/Compare detect for CCR2

        break;
    case 14:                // Overflow detect
        P1OUT ^= BIT0;      // Toggle status of D1
        break;
    }
}





