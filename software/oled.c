
/**
 *  @file oled.c
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the OLED functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <stdlib.h>

#include "main.h"
#include "oled.h"
#include "pad.h"
#include "gps.h"

int displayHasBeenUpdated;
unsigned int modeDisplay = MD_SHUTDOWN;
unsigned int oldModeDisplay;

/** @brief The answer received by the OLED */
int answer = 0;
/** @brief Answer received? */
int flagReceive = 0;

void enableUSARTforOLED(void) {

	// mise à zéro de l'UART1
	UCTL1 = SWRST;

	// 8-bits char
	UCTL1 |= CHAR;  
	// USART Transmit Control Register              
    // BRCLK = SMCLK (page 272)
    UTCTL1 |= SSEL1;                   
    
    // Config baud rate at 9600
    // 8MHz/9600 ~ 833
    U1BR0 = 0x41;                         
    U1BR1 = 0x03;

    // modulation
    UMCTL1 = 0x21;

    UCTL1 &= ~SWRST;
    
    // enable USART1 RX/TX
    ME2 |= URXE1 | UTXE1;      

    //P3.6,7 enabled
    P3SEL |= 0xC0; 
       
    // P3.6 output = 1 (TX) direction    
    // P3.7 input = 0 (RX) direction    
    P3DIR |= 0x40;
    P3DIR &= ~0x80;

    // P4,1 (RESET_LCD) - P4.2 (CMD_SWITCH)
    P4DIR |= 0x06;
}

void resetOLED() {
	P4OUT &= ~BIT1;
	delay(3.0);
	P4OUT |= BIT1;
	delay(6.0);
	// debug_printf("OLED reset\n");
}

void toggleOLEDInterrupt(unsigned int state) {
	// toggle USART1 RX interrupt
    (state == 1) ? (IE2 |= URXIE1) : (IE2 &= ~URXIE1);
    // (IE2 & URXIE1) ? (debug_printf("OLED interupt active\n")) : (debug_printf("OLED interupt desactive\n"));
}

void sendChar(int c){
    while (!(IFG2 & UTXIFG1));
    TXBUF1 = c;
}

void usart1_rx (void) __interrupt[USART1RX_VECTOR]{  
    flagReceive = 1;
    answer = RXBUF1; 
}

void gfx_Clean() {
    do {
        sendChar(0xFF);
        sendChar(0xD7);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_BGcolour(int color) {
    do {
        sendChar(0xFF);
        sendChar(0x6E);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    } while (answer != OLED_ANSWER_ACK);
}

void gfx_PutString(char* string) {
	int i;
    do {
        sendChar(0x00);
        sendChar(0x06);
        i = 0;
        while (string[i] != '\0') { sendChar(string[i++]); }
        sendChar(0x00);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_RectangleFilled(int x1, int y1, int x2, int y2, int color){
    do {
        sendChar(0xFF);
        sendChar(0xCE);
        sendChar(0x00);
        sendChar(x1);
        sendChar(0x00);
        sendChar(y1);
        sendChar(0x00);
        sendChar(x2);
        sendChar(0x00);
        sendChar(y2);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    }while(answer != OLED_ANSWER_ACK);
}

void SSTimeout(int t) {
    do {
        sendChar(0x00);
        sendChar(0x0C);
        // shift
        sendChar(t >> 4);
        sendChar(t & 0x00FF);
        while(flagReceive != 1);
        flagReceive = 0;
    }while(answer != OLED_ANSWER_ACK);
}

void gfx_CalculateOrbit(int angle, int distance, int* x, int* y) {
    do {
        sendChar(0x00);
        sendChar(0x03);
        // shift
        sendChar(angle >> 8);
        sendChar(angle & (0x00FF));
        sendChar(0x00);
        sendChar(distance);
        while(flagReceive != 1);
        flagReceive = 0;
        
    } while(answer != OLED_ANSWER_ACK);
    

    while(flagReceive != 1);
    flagReceive = 0;

    while(flagReceive != 1);
    flagReceive = 0;
    *x = answer;

    while(flagReceive != 1);
    flagReceive = 0;  

    while(flagReceive != 1);
    flagReceive = 0;  
    *y = answer;
}

void gfx_DrawCircle(int x, int y, int radius, int color) {
	do {
        sendChar(0xFF);
        sendChar(0xCD);
        sendChar(0x00);
        sendChar(x);
        sendChar(0x00);
        sendChar(y);
        sendChar(0x00);
        sendChar(radius);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_DrawLine(int x1, int y1, int x2, int y2, int color) {
	do {
        sendChar(0xFF);
        sendChar(0xD2);
        sendChar(0x00);
        sendChar(x1);
        sendChar(0x00);
        sendChar(y1);
        sendChar(0x00);
        sendChar(x2);
        sendChar(0x00);
        sendChar(y2);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_MoveOrigin(int x, int y) {
	do {
        sendChar(0xFF);
        sendChar(0xD6);
        sendChar(0x00);
        sendChar(x);
        sendChar(0x00);
        sendChar(y);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_ScreenMode(int mode){
    do {
        sendChar(0xFF);
        sendChar(0x68);
        sendChar(0x00);
        sendChar(mode);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void txt_FGColor(int color){
    do {
        sendChar(0xFF);
        sendChar(0x7F);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void txt_BGColor(int color){
    do {
        sendChar(0xFF);
        sendChar(0x7E);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void setBaudRate(){
    do {
        sendChar(0x00);
        sendChar(0x0B);
        // 9600
        sendChar(0x01);
        sendChar(0x38);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void gfx_Rectangle(int x1, int y1, int x2, int y2, int color) {
    do {
        sendChar(0xFF);
        sendChar(0xCF);
        sendChar(0x00);
        sendChar(x1);
        sendChar(0x00);
        sendChar(y1);
        sendChar(0x00);
        sendChar(x2);
        sendChar(0x00);
        sendChar(y2);
        // shift
        sendChar(color >> 8);
        sendChar(color & (0x00FF));
		while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void txt_Width(int multi) {
    do {
        sendChar(0xFF);
        sendChar(0x7C);
        sendChar(0x00);
        sendChar(multi);
        while(flagReceive != 1);
        flagReceive = 0;
    } while(answer != OLED_ANSWER_ACK);
}

void initOLED() {
	gfx_Clean();
	// no scrolling
    SSTimeout(NO);
    // reverse display
    gfx_ScreenMode(YES);
}

void displayMainMenu() {
	gfx_BGcolour(NAVY);
	gfx_Clean();

	gfx_RectangleFilled(4, 4, 124, 31, LIGHTGREY);
	gfx_RectangleFilled(4, 35, 124, 62, LIGHTGREY);
	gfx_RectangleFilled(4, 66, 124, 93, LIGHTGREY);	
	gfx_RectangleFilled(4, 97, 124, 124, LIGHTGREY);

	txt_BGColor(LIGHTGREY);
	txt_FGColor(BLACK);

	gfx_MoveOrigin(35, 16);
	gfx_PutString("Boussole");
	gfx_MoveOrigin(30, 46);
	gfx_PutString("Navigation");
	gfx_MoveOrigin(15, 76);
	gfx_PutString("Enregistrement");
	gfx_MoveOrigin(45, 106);
	gfx_PutString("Arret");

	displayHasBeenUpdated = NO;
	
}

void updateMenu() {
	if (!displayHasBeenUpdated) {

        switch (oldModeDisplay) {
            case MD_COMPASS:
                gfx_RectangleFilled(4, 4, 124, 31, LIGHTGREY);
                gfx_MoveOrigin(35, 16);
                gfx_PutString("Boussole");
                break;
            case MD_NAVIG:
                gfx_RectangleFilled(4, 35, 124, 62, LIGHTGREY);
                gfx_MoveOrigin(30, 46);
                gfx_PutString("Navigation");
                break;
            case MD_RECORD:
                gfx_RectangleFilled(4, 66, 124, 93, LIGHTGREY);
                gfx_MoveOrigin(15, 76);
                gfx_PutString("Enregistrement");
                break;
            case MD_SHUTDOWN:
                gfx_RectangleFilled(4, 97, 124, 124, LIGHTGREY);
                gfx_MoveOrigin(45, 106);
                gfx_PutString("Arret");
                break;
        }
        delay(0.1);
		switch (modeDisplay) {
			case MD_COMPASS:
				gfx_Rectangle(4, 4, 124, 31, RED);
				gfx_MoveOrigin(35, 16);
				gfx_PutString("Boussole");
				break;
			case MD_NAVIG:
				gfx_Rectangle(4, 35, 124, 62, RED);
				gfx_MoveOrigin(30, 46);
				gfx_PutString("Navigation");
				break;
			case MD_RECORD:
				gfx_Rectangle(4, 66, 124, 93, RED);
				gfx_MoveOrigin(15, 76);
				gfx_PutString("Enregistrement");
				break;
			case MD_SHUTDOWN:
				gfx_Rectangle(4, 97, 124, 124, RED);
				gfx_MoveOrigin(45, 106);
				gfx_PutString("Arret");
				break;
		}

		
		displayHasBeenUpdated = YES;
	}
}

void displayHeader() {
    gfx_RectangleFilled(4, 4, 124, 31, LIGHTGREY);
    txt_BGColor(LIGHTGREY);
    txt_FGColor(BLACK);
    gfx_MoveOrigin(12, 10);
    gfx_PutString("Bouton central\nRevenir au menu");
}

void displayMessage(char* string) {
	if (!displayHasBeenUpdated) {
		txt_BGColor(NAVY);
		txt_FGColor(LIGHTGREY);
		gfx_MoveOrigin(25, 62);
		gfx_PutString(string);
        displayHeader();
		displayHasBeenUpdated = YES;
	}	
}

void displayCompass() {
    int x = -1;
    int y = -1;
    int newHeading;
    char bufLat[50];
    char bufLon[50];
    char buffer[5] = {0};
    char* orientation = calculateDirection();
    if (!displayHasBeenUpdated) {
        toggleOLEDInterrupt(YES);
        gfx_MoveOrigin(64, 70);
        if (0.0 <= GPSData.heading && GPSData.heading <= 360.0) {
            // shift by -90 (screen)
            newHeading = GPSData.heading-90;
            if(newHeading < 0) {
                newHeading = 360+newHeading;
            }
            txt_BGColor(NAVY);
            txt_FGColor(WHITE);
            // calculate (x,y) from radius and distance
            gfx_CalculateOrbit(newHeading, 30, &x, &y);
            if (0 <= x && x <= 128 && 0 <= y && y <= 128) {
                // reset display
                // gfx_Clean();
                gfx_RectangleFilled(0, 32, 128, 218, NAVY);
                delay(0.1);
                // display
                gfx_DrawLine(64, 70, x, y, RED);
                gfx_DrawCircle(64, 70, 30, WHITE);
            }
            // print heading
            gfx_MoveOrigin(25, 33);
            gfx_PutString("o");
            gfx_MoveOrigin(6, 42);
            itoa((int)GPSData.heading, buffer, 10);
            gfx_PutString(buffer);
            // print direction
            gfx_MoveOrigin(100, 42);
            gfx_PutString(orientation);
        }
        ftoa(bufLat, GPSData.latitude);
        bufLat[10] = '\0';
        ftoa(bufLon, GPSData.longitude);
        bufLon[10] = '\0';

        // diplay lat/lon
        gfx_MoveOrigin(4, 107);
        gfx_PutString("Lat :");
        gfx_MoveOrigin(40, 107);
        gfx_PutString(bufLat);
        gfx_MoveOrigin(4, 117);
        gfx_PutString("Lon :");
        gfx_MoveOrigin(40, 117);
        gfx_PutString(bufLon);

        // displayHeader();
        // updated so can fetch new data
        displayHasBeenUpdated = YES;
        toggleOLEDInterrupt(NO);
        toggleGPSInterrupt(YES);
    }
}

void ftoa(char* p, float x)
{
  int n,i=0,k=0;
  n=(int)x;
  while(n>0)
  {
    x/=10;
    n=(int)x;
    i++;
 }
 *(p+i) = '.';
 x *= 10;
 n = (int)x;
 x = x-n;
 while(n>0)
 {
   if(k == i)
        k++;
   *(p+k)=48+n;
   x *= 10;
   n = (int)x;
   x = x-n;
   k++;
 }
 *(p+k) = '';
}


char* calculateDirection() {
    if (0.0 <= GPSData.heading && GPSData.heading <= 360.0) {
        if(GPSData.heading <= 21.5) return NORTH;
        if(GPSData.heading <= 67.5) return NE;
        if(GPSData.heading <= 112.5) return EAST;
        if(GPSData.heading <= 157.5) return SE;
        if(GPSData.heading <= 202.5) return SOUTH;
        if(GPSData.heading <= 247.5) return SW;
        if(GPSData.heading <= 292.5) return WEST;
        if(GPSData.heading <= 337.5) return NW;
        else return NORTH;
    } else {
        return NONE;
    }
}
