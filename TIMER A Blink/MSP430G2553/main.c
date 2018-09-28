#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 26 September 2018
// Microcontroller: MSP430G2553

// Program Description: This program was designed to blink D1 (P1.0) and D2 (P1.6) at different rates through the use of a single
//                      timer and two capture/compare registers. The timer is set to be in continuous mode while the capture/compare
//                      register value is incremented each time its interrupt flag is triggered so that the LED tied to the capture/compare
//                      register will blink at a consistent rate. This code is designed for the MSP430G2553 microcontroller.

/**
 * main.c
 */

int setFrequency(int frequency)
{
    int freq = 62500 / frequency;   // Calculates the desired CCRx value for desired blink frequency
    return freq;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1SEL &= ~0x41;             // Sets P1.0 and P1.6 to be GPIO
	P1SEL2 &= ~0x41;            // Sets P1.0 and P1.6 to be GPIO
	P1DIR |= 0x41;              // Sets P1.0 and P1.6 to be output
	// Timer setup
	TA0CTL = TASSEL_2 + ID_3 + MC_2;    // SMCLK selected, divider set to divide clock by 8, timer set to Continuous Mode
	// CCRx setup
	TA0CCR1 = setFrequency(5);          // Stores desired frequency in CCR1 register
	TA0CCR2 = setFrequency(3);          // Stores desired frequency in CCR2 register
	TA0CCTL1 |= CCIE;                   // Enables Capture/Compare interrupt for CCR1
	TA0CCTL2 |= CCIE;                   // Enables Capture/Compare interrupt for CCR2
	
	_BIS_SR(GIE);   // Enable global interrupts
	while(1);
}


// ISR
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void) {
    // Switch TAIV, automatically resets the flags
    switch(TA0IV)
    {
    case 2:                         // Capture/Compare detect for CCR1
        P1OUT ^= BIT0;              // Toggle status of D1 (P1.0)
        CCR1 += setFrequency(5);    // Increments the value in CCR1 to maintain a constant blink frequency
        break;
    case 4:                         // Capture/Compare detect for CCR2
        P1OUT ^= BIT6;              // Toggle status of D2 (P1.6)
        CCR2 += setFrequency(3);    // Increments the value in CCR2 to maintain a constant blink frequency
        break;
    case 10:                        // Overflow detect

        break;
    }
}
