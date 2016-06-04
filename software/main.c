
/**
 *	@file main.c
 *	@author Gaël Foppolo (gaelfoppolo)
 *
 *	@brief File containing the main functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>

#include "main.h"
#include "led.h"
#include "pad.h"
#include "gps.h"
#include "parser_nmea.h"
#include "oled.h"

unsigned int modeSelected;

void main(void) {

	// Stop watchdog timer to prevent time out reset
        WDTCTL = WDTPW+WDTHOLD;

    configureClock();

    initLED();
    initPAD();

    // communication with GPS
    toggleCommunication(COMM_GPS);
    enableUSARTforGPS();
    // disable GPS interrupt
    toggleGPSInterrupt(NO);
    // activate GPS
    toggleGPS(YES);
    // config GPS
    gpsSend(RATE_2SEC);
	gpsSend(DISABLE_ALL);
	gpsSend(RMC_ONLY);
	// so far no data receive
	dataValid = NO;
    
    enableUSARTforOLED();
    // activate OLED interrupt
    toggleOLEDInterrupt(YES);
    // activate OLED
    resetOLED();

    // config menu
   	oldModeDisplay = modeDisplay;
	modeDisplay = MD_COMPASS;
   	modeSelected = M_MENU;

	// global interrupt
	_EINT();

	// init global oled
	initOLED();	

	/**
	 * Menu entry point
	 */
	mainMenu:
	// display menu
	displayMainMenu();

	// until stay on menu, update
	while(modeSelected == M_MENU) {
		updateMenu();
	}
	
	// mode has been selected
	switch(modeSelected) {
		case M_COMPASS:
			goto compass;
			break;		
		case M_NAVIG:
			goto navig;
			break;
		case M_RECORD:
			goto record;
			break;
		case M_SHUTDOWN:
			goto end;
			break;
	}

	/**
	 * Compass entry point
	 */
	compass:
	// clean, waiting message
	gfx_Clean();
	displayHasBeenUpdated = NO;
	displayMessage("En attente\nde donnees...");
	displayHasBeenUpdated = NO;
	// activate GPS interrupt to get data
	toggleCommunication(COMM_GPS);
	toggleOLEDInterrupt(NO);
	toggleGPSInterrupt(YES);
	
	// waiting valid data
	while((modeSelected != M_MENU) && !dataValid);
	
	// data are valid, display compass
	while((modeSelected != M_MENU)) {
		displayCompass();
	}

	// if here meaning mode is menu so get back to menu
	displayHasBeenUpdated = NO;
	goto mainMenu;

	/**
	 * Navigation entry point
	 */
	navig:
	gfx_Clean();
	displayHasBeenUpdated = NO;
	while(modeSelected != M_MENU) {
		// navig do
		displayMessage("Navig");
	}

	// get out of mode, back to menu
	goto mainMenu;
	
	/**
	 * Record entry point
	 */
	record:
	gfx_Clean();
	displayHasBeenUpdated = NO;
	while(modeSelected != M_MENU) {
		// record do
		displayMessage("Record");
	}

	// get out of mode, back to menu
	goto mainMenu;

	/**
	 * Shutdown entry point
	 */
	end:
	// display message
	gfx_Clean();
	displayHasBeenUpdated = NO;
	displayMessage("Arrêt");	
	// wait before continue
	delay(0.5);
	gfx_BGcolour(BLACK);
	gfx_Clean();

	debug_exit(0);
}

void toggleCommunication(unsigned int state) {
	// set or clear P4.2
	(state == 1) ? (P4OUT |= BIT2) : (P4OUT &= ~BIT2);
	// (P4OUT & BIT2) ? (debug_printf("Communication USB active\n")) : (debug_printf("Communication GPS active\n"));
}

void configureClock(void) {
	unsigned int i;
	// Switch between 2 clock (4-12 User Guide)
	// Config Basic Clock System Control
	// XT2 is on 
	BCSCTL1 &= ~XT2OFF;

	do {
	  IFG1 &= ~OFIFG;                   // Clear OSCFault flag
	  for (i = 0xFF; i > 0; i--);       // Time for flag to set
	} while ((IFG1 & OFIFG) != 0);        // OSCFault flag still set?
	
	// MCLK = SMCLK = XT2 (safe)
	// with SMCLK = 8Mhz (page 125)
	BCSCTL2 |= SELM1+SELS;  
}

void delay(float x) {
	float i, j = x*35000;
    for (i = 0.0; i < j; i++) {}
}
