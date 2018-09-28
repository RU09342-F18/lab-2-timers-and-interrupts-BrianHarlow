# Brian Harlow's Button Interrupt

## Program Purpose
This program is designed to toggle an LED by the push of a button using interrupts as opposed to polling. Interrupts are preferred so that the microcontroller can be put into low
power mode and ultimately consume less power to be more efficient.

## Functionality
The GPIO pins are configured so that D1 (P1.0) is an LED output and S1 (P1.3) is a button input. Interrupts for P1.3 are enabled and set to trigger on the rising edge of P1.3. Global interrupts are then enabled as the microcontroller is set to LPM0. An Interrupt Service Routine is configured to be triggered by the button being pressed and will toggle the status of D1.

## Microcontrollers Used
* MSP430G2553 - The button used is S1 (P1.3) and the LED used is D1 (P1.0).
* MSPFR2311 - The button used is S1 (P1.1) and the LED used is LED1(P1.0). 
