#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 26 September 2018
// Microcontroller: MSP430G2553

// Program Description: This program is designed to use a button and an LED on a development board with the MSP430G2553 microcontroller
//                      to set the frequency of the LED. When you push the button and let it go the LED should begin to toggle so that
//                      it is illuminated for the amount of time the button was pressed, and then turned off for the same amount of time.

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL3 = LFXT1S_2;         // ACLK enable
	
	P1SEL &= ~0x09;             // Sets P1.0 and P1.3 to be GPIO
	P1SEL2 &= ~0x09;            // Sets P1.0 and P1.3 to be GPIO
	P1DIR |= BIT0;              // Sets P1.0 to be an output
	P1DIR &= ~BIT3;             // Sets P1.3 to be an input

    P1REN |= BIT3;              // Pull-up/pull-down resistor enabled for P1.3
    P1OUT |= BIT3;              // Pull-up/pull-down resistor set to pull-down for P1.3

    P1IE |= BIT3;               // Enables interrupt for P1.3
    P1IES |= BIT3;              // Set interrupt as falling edge of P1.3
    P1IFG &= ~BIT3;             // Clears the interrupt for P1.3

    TA0CTL = TASSEL_1 + ID_2 + MC_1 + TACLR + TAIE; // ACLK set to UP MODE with Timer A interrupt enabled
    TA0CCR0 = 0x96;                                 // Sets CCR0 register to value 0x96

	_BIS_SR(LPM0_bits + GIE);   // Enter LPM0 and enable global interrupts
}

// ISR
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IES & BIT3)                            // Check condition for if button is pressed down
    {
        P1IES &= ~BIT3;                          // Change interrupt to trigger on rising edge of P1.3
        TA0CTL = TASSEL_1 + ID_2 + MC_2 + TACLR; // Reset Timer A, set to Continuous Mode, and disable overflow interrupts
    }
    else
    {
        TA0CTL = TASSEL_1 + ID_2 + MC_0;                // Forces timer into STOP Mode
        CCR0 = TA0R;                                    // Stores time the button was pressed for in CCR0 register
        TA0CTL = TASSEL_1 + ID_2 + MC_1 + TACLR + TAIE; // Reset timer, set timer to UP Mode, and enable overflow interrupts
        P1IES |= BIT3;                                  // Change interrupt to trigger on falling edge of P1.3
    }
    P1IFG &= ~BIT3;                                     // Clears interrupt flag triggered by button press
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
    case 10:                // Overflow detect
        P1OUT ^= BIT0;      // Toggle status of D1
        break;
    }
}





