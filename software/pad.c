
/**
 *  @file pad.c
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the PAD functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>

#include "main.h"
#include "gps.h"
#include "pad.h"
#include "oled.h"

void initPAD(void) {
	// P2.0..4 as input (all)
    P2DIR &= ~0x1F;

    // enable interruptions P2IFG for each pin (0 -> 4)
    P2IE = 0x1F;

    // set mode of P2IFG, here 1, so high to low mode
    // for each pin (0 -> 4)
    P2IES = 0x1F;
    // debug_printf("PAD init\n");
}

void padInterrupt(void) __interrupt[PORT2_VECTOR] {
    
    switch(P2IN) {
    	//push
    	case PUSH:
    		// debug_printf("PUSH\n");
			// if in menu go in mode
			if (modeSelected == M_MENU) {
				// debug_printf("Selection du mode %d (todo)\n", getModeDisplay(CURRENT));
				modeSelected = modeDisplay+1;
				delay(0.1);
			} else {
				// else get back to menu
				// setModeDisplay(getModeDisplay(CURRENT)); // not sure if necessary
				toggleGPSInterrupt(NO);
				toggleOLEDInterrupt(YES);
				modeSelected = M_MENU;
			}
			break;
    	//top
		case TOP:
			// debug_printf("TOP\n");
			if (modeSelected == M_MENU) {
				// menu diplay -- mod 4
				oldModeDisplay = modeDisplay;
				modeDisplay = ((modeDisplay-1)%4);
				delay(0.1);
				displayHasBeenUpdated = NO;
			} else {
				// debug_printf("inside mode %d\n", modeSelected);
			}
			break;
      //right
      case RIGHT:
      	// debug_printf("RIGHT\n");
		if (modeSelected == M_MENU) {
			// debug_printf("aucun effet dans le menu\n");
		} else {
			// debug_printf("inside mode %d\n", modeSelected);
		}
		break;
      //bottom
      case BOTTOM:
      	// debug_printf("BOTTOM\n");
		if (modeSelected == M_MENU) {
			// menu diplay ++ mod 4
			oldModeDisplay = modeDisplay;
			modeDisplay = ((modeDisplay+1)%4);
			delay(0.1);
			displayHasBeenUpdated = NO;
		} else {
			// debug_printf("inside mode %d\n", modeSelected);
		}
			break;
      //left
      case LEFT:
		// debug_printf("LEFT\n");
		if (modeSelected == M_MENU) {
			// debug_printf("aucun effet dans le menu\n");
		} else {
			// debug_printf("inside mode %d\n", modeSelected);
		}
		break;
      default:
		break;
    }

    // clear P2IFG flag
    P2IFG &= ~0x1F;       
    
}

