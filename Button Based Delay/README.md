# Brian Harlow's Button Based Delay

## Program Purpose
This program was designed to blink an LED on an MSP430 development board at a frequency decided by the user. The user will push a button and hold it down for the duration that the user would like the LED to remain illuminated for.

## Functionality
Initially, the desired clock is enabled and the GPIO are set so that the desired LED is an output and the desired button is an input. Interrupts are enabled for the button input to be triggered on the falling edge of P1.3 and the interrupt flag is intially cleared. The ACLK clock is selected, divided by 4, and set to UP Mode. The CCR0 register is set to 0x96 to have an initial frequency of 10Hz. Global Interrupts are then enabled and the microcontroller is put into LPM0. One Interrupt Service Routine is activated when the button is pressed to clear the timer and switch it to Continous mode. When the button is released, the timer is stopped and the value that the timer is at is stored in the CCR0 register to change the blink frequency of the LED.

## Microcontrollers Used
* MSP430G2553 - The LED used is D1 (P1.0) and the button used is S1 (P1.3).
* MSP430F5529 - The LED used is LED1 (P1.0) and the button used is S2 (P1.1). 
