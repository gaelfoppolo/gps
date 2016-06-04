/**
 *  @file led.c
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the LED functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>

#include "main.h"
#include "led.h"

void initLED(void) {
	// P1,0..4 output (all)
    P1DIR = 0x1F;
   	// clear state, ready to use
    P1OUT = 0x00;
    // debug_printf("LED init\n");
}

void toggleLED(int n, unsigned int state, unsigned int duration) {
	(state == 1) ? (P1OUT |= n) : (P1OUT &= ~n);
    if (duration > 0) {
    	delay(duration);
    	(state == 1) ? (P1OUT &= ~n) : (P1OUT |= n);
    }
}