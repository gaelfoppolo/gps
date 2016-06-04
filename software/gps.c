
/**
 *	@file gps.c
 *	@author Gaël Foppolo (gaelfoppolo)
 *
 *	@brief File containing the GPS functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <math.h>

#include "main.h"
#include "gps.h"
#include "parser_nmea.h"
#include "oled.h"

unsigned int dataValid;

/**
 * @brief      Store GPS valid data
 */
gps_data GPSData;

void toggleGPS(unsigned int state) {
	(state == 1) ? (P4OUT |= BIT0) : (P4OUT &= ~BIT0);
	// (P4OUT & BIT0) ? (debug_printf("GPS active\n")) : (debug_printf("GPS desactive\n"));
}

void toggleGPSInterrupt(unsigned int state) {
	// toggle USART0 RX interrupt
    (state == 1) ? (IE1 |= URXIE0) : (IE1 &= ~URXIE0);
    // (IE1 & URXIE0) ? (debug_printf("GPS interupt active\n")) : (debug_printf("GPS interupt desactive\n"));
}

void enableUSARTforGPS(void) {
	// mise à zéro de l'UART
	UCTL0 = SWRST;

	// 8-bits char
	UCTL0 |= CHAR;  
	// USART Transmit Control Register              
    // BRCLK = SMCLK (page 272)
    UTCTL0 |= SSEL1;                   
    
    // Config baud rate at 9600
    // 8MHz/9600 ~ 833
    U0BR0 = 0x41;                         
    U0BR1 = 0x03;

    // modulation
    UMCTL0 = 0x21;

    UCTL0 &= ~SWRST; 
    
    // enable USART0 RX/TX
    ME1 |= URXE0 | UTXE0;     

    //P3.4,5 enabled
    P3SEL |= 0x30; 
    
    // P3.4 output = 1 (TX) direction
    // P3.5 input = 0 (RX) direction    
    P3DIR |= 0x10;
    P3DIR &= ~0x20;

    // P4,0 (ENABLE_GPS) and P4,2 (CMD_SWITCH)
    P4DIR |= 0x05;
}

void gpsSend(char* message){
	int i = 0;
	while (message[i] != '\0') {
		// waiting buffer ready
		while((IFG1 & UTXIFG0) == 0);
		TXBUF0 = message[i++];
	}
}

void usart0_rx(void) __interrupt[USART0RX_VECTOR] {
	char c, sentences[NUMBERS_OF_SENTENCE_MAX][NMEA_MAX_LENGTH];
	int i, line, column, nbSentences;
	// reset buffer
	line = 0, column = 0, nbSentences = 0;
	memset(sentences, 0, sizeof sentences);
	while (line < NUMBERS_OF_SENTENCE) {
		// wait begin of SENTENCE ($) if new line
		// else just wait new char
		if(column == 0) {
			do {
				while (!(IFG1 & URXIFG0));
				c = RXBUF0; // or RXBUF0;
			} while (c != '$');
		}
		else {
			while (!(IFG1 & URXIFG0));
			c = RXBUF0; // or RXBUF0;
		}
		sentences[line][column++] = c;
		
		// end of line, update 
		if (c == '\n') {
			line++;
			column = 0;
		}
	}
	
	// print SENTENCE
	for(i = 0; i < line; i++) {
		nmea_sentence_rmc frame;
		// check checksum
	    if (nmea_check(sentences[i], YES)) {
	    	// parsed succefully and valid
	        if (nmea_parse_rmc(&frame, sentences[i]) && (frame.valid)) {
	            GPSData.latitude = frame.latitude;
	            GPSData.longitude = frame.longitude;
	            GPSData.speed = frame.speed;
	            GPSData.heading = frame.heading;
	            dataValid = YES;
	            
	        }
	        if (dataValid == YES) {
	        	// new data need to refresh display
	            // so toggle comm with OLEd
	            if (modeSelected == M_COMPASS) {
	            	displayHasBeenUpdated = NO;
	            	toggleGPSInterrupt(NO);
	            }
	        }
	    }
	}

    // debug_printf("usart0 rx interrupt end\n");

}

float calcDistance(float lat1, float lon1, float lat2, float lon2) {
  float dLat = deg2rad(lat2-lat1);  // deg2rad below
  float dLon = deg2rad(lon2-lon1); 
  float a = sin(dLat/2.0)*sin(dLat/2.0)+cos(deg2rad(lat1))*cos(deg2rad(lat2))*sin(dLon/2.0)*sin(dLon/2.0); 
  float c = 2.0 * atan2(sqrt(a), sqrt(1.0-a));
  // Distance in km
  return 6371.0*c;
}

float deg2rad(float deg) {
  return deg*(acos(-1.0)/180.0);
}


