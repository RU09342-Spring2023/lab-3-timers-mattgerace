/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: FILL THIS IN
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */

#include "GPIO_Driver.h"
#include <msp430.h>

void init();
void reset();
void updateCount();
int count = 0;
int cycles = 0;
int times = 0;
void main()
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    init();
    reset();
    __bis_SR_register(LPM3_bits | GIE);

    char button = gpioRead(2, 3); //make button sensitive to P2.3
    while (!button)
    {
        times = times + 2400;
        __delay_cycles(100000);  //delay for 0.1s
    }


}

void init ()
{
    gpioInit(1, 0, 1);
    gpioInit(6, 6, 1);
    gpioInit(2, 3, 0);
    gpioInit(4, 1, 0);

    P4REN |= BIT1;               // Enable Resistor on P4.1
    P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
    P2REN |= BIT3;                  // Enable resistor on P2.3
    P2OUT |= BIT3;                  // Configure resistor on P2.3 to be a pullup

    P2IES &= ~BIT3;                         // P2.3 Low --> High edge    (changed the edge sensitivity)
    P2IE |= BIT3;                           // P2.3 interrupt enabled
    P4IES &= ~BIT1;
    P4IE |= BIT1;

}

void reset ()
{
    count = 24000;
    TB1CCTL0 = CCIE;
    TB1CCR0 == count;
    TB1CTL = TBSSEL_1 | MC_2;

}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
    P6OUT ^= BIT6;
    TB1CCR0 += count;

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= !BIT3;
    count = times;
    times = 0;


}


#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;
    reset();

}
