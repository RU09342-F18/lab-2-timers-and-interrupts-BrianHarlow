#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 26 September 2018
// Microcontroller: MSP430F5529

// Program Description: This program was designed to blink D1 (P1.0) and D2 (P1.6) at different rates through the use of a single
//                      timer and two capture/compare registers. The timer is set to be in continuous mode while the capture/compare
//                      register value is incremented each time its interrupt flag is triggered so that the LED tied to the capture/compare
//                      register will blink at a consistent rate. This code is designed for the MSP430F5529 microcontroller.

/**
 * main.c
 */

int setFrequency(int frequency)
{
    int freq = 2048 / frequency;    // Calculates the desired CCRx value for desired blink frequency
    return freq;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	UCSCTL4 = SELA_0;           // Sets ACLK to XT1CLK at 32.768kHz

	P1SEL &= ~BIT0;             // Sets P1.0 to be GPIO
	P4SEL &= ~BIT7;             // Sets P4.7 to be GPIO

	P1DIR |= BIT0;              // Sets P1.0 to be output
	P4DIR |= BIT7;              // Sets P4.7 to be output
	
	TA0CTL = TASSEL_1 + ID_3 + MC_2;    // Timer setup

	TA0CCR1 = setFrequency(5);  // CCR1 setup
	TA0CCR2 = setFrequency(2);  // CCR2 setup
	TA0CCTL1 |= CCIE;           // Enable interrupt from CCR1
	TA0CCTL2 |= CCIE;           // Enable interrupt from CCR2

	_BIS_SR(GIE);               // Enable global interrupts
	while(1);
}

// ISR
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void) {
    // Switch TAIV, automatically resets the flags
    switch(TA0IV)
    {
    case 2:                             // Capture/Compare detect for CCR1
        P1OUT ^= BIT0;                  // Toggle status of D1 (P1.0)
        TA0CCR1 += setFrequency(5);     // Increments the value in CCR1 to maintain a constant blink frequency
        break;
    case 4:                             // Capture/Compare detect for CCR2
        P4OUT ^= BIT7;                  // Toggle status of D2 (P4.7)
        TA0CCR2 += setFrequency(2);     // Increments the value in CCR2 to maintain a constant blink frequency
        break;
    case 10:                            // Overflow detect

        break;
	}
}
