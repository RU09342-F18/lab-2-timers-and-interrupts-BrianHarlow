# Brian Harlow's TIMER A Blink

## Program Purpose
This program was designed to blink two LEDs on an MSP430 development board at a consistent rate using timers.

## Functionality
Initially, the desired clock is enabled and the GPIO pins are configured so that the two desired LEDs are outputs. The timer is set to Continuous mode and the desired frequencies are stored in the CCRx registers. Capture/Compare interrupts are enabled for both CCRx registers. Global interrupts are enabled. When a Capture/Compare interrupt is triggered, the Interrupt Service Routine runs and toggles the appropriate LED at the predetermined frequency. It also increments the CCRx register to maintain a constant blink frequency.

## Microcontrollers Used
* MSP430G2553 - The LEDs used are D1 (P1.0) and D2 (P1.6).
* MSP430F5529 - The LEDs used are LED1 (P1.0) and LED2 (P4.7). 
