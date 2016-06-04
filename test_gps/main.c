#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>

#define BUFFSIZE 500
#define NUMBERSOFTRAME 4

char buffer[BUFFSIZE];
unsigned int oct, cpt, i, nbTrames;
char c;
// PMTK300 : controls how often the module checks the satellites and updates its position registers
// PMTK220 : controls how often the GPS module emits NMEA sentences
// refresh position every 5 seconds
// char* message = "$PMTK300,5000,0,0,0,0*18\r\n";
// send every 10 seconds
char* sendRate = "$PMTK220,1000*1F\r\n";
char* disableAll = "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";
char* onlyGGAandGPRMC = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";
//char* turnAll = "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";

void gpsSend(char* message);
void usart0_rx(void);
unsigned int delay(unsigned int x);

unsigned int delay(unsigned int x) {
    unsigned int i, j;
    for (i = 0; i<= x; i++) {
        for (j = 0; j < x*x; ++j) {}
    }
    debug_printf("end delay");   
    return 0;
}

void gpsSend(char* message){
	debug_printf("usart0 tx send\n");
	cpt = 0;
	while (message[cpt] != '\0') {
		// waiting buffer ready
		while((IFG1 & UTXIFG0) == 0);
		U0TXBUF = message[cpt++];
	}
	debug_printf("send\n");
}

void usart0_rx(void) __interrupt[UART0RX_VECTOR] {
	debug_printf("usart0 rx interrupt\n");
	
	// reset buffer
	oct = nbTrames = 0;
	memset(buffer, 0, sizeof buffer);

	//catching first $
	do {
		// waiting buffer ready
		while (!(IFG1 & URXIFG0));
		c = U0RXBUF;
	} while (c != '$');

	// store first $
	buffer[oct++] = c;

	// while dont have all trames
	while (nbTrames < NUMBERSOFTRAME) {
		// waiting buffer ready
		while (!(IFG1 & URXIFG0));
		c = U0RXBUF;
		buffer[oct++] = c;// or RXBUF0;
		
		// number of trames update
		if (c == '\n') {
			nbTrames++;
		}
	}

	// end buffer to print it correctly
	buffer[oct] = '\0';

	// print trame
	debug_printf("nb trames clean = %d\n", nbTrames);
	debug_printf("message = %s\n", buffer);

}

void main(void) {

    unsigned int i;

	// Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW+WDTHOLD;
  
	// sequence à faire pour le switch entre 2 clock (cf page 4-12 du guide de l'utilisateur)
	// Configuration du Basic Clock System Control
	// XT2 est allumé 
	BCSCTL1 &= ~XT2OFF;

	do {
	  IFG1 &= ~OFIFG;                   // Clear OSCFault flag
	  for (i = 0xFF; i > 0; i--);       // Time for flag to set
	} while ((IFG1 & OFIFG) != 0);        // OSCFault flag still set?
	
	// MCLK = SMCLK = XT2 (safe) avec SMCLK = 8Mhz
	BCSCTL2 |= SELM1+SELS;          

	// mise à zéro de l'UART
	UCTL0 |= SWRST;        
	
	// Format des caractères sur 8-bits
	UCTL0 = CHAR;                
    // UCLK = SMCLK
    UTCTL0 = SSEL1;                   
    
    // Config du taux de transmission pour 9600 bauds
    // 8MHz/9600 ~ 833
    U0BR0 = 0x41;                         
    U0BR1 = 0x03;

    UMCTL0 = 0x21; // modulation
    
    // Enable USART0 RX/TX
    ME1 |= URXE0 + UTXE0; 
    IE1 |= URXIE0; 

    //P3.4,5 activé
    P3SEL |= 0x30; 
    
    // P3.4 output = 1 (TX) direction
    // P3.5 input = 0 (RX) direction    
    P3DIR |= 0x10;
    P3DIR &= ~0x20;

	// P4,0 et 2 en sortie (0 : ENABLE_GPS, 2 : CMD_SWITCH)
    P4DIR |= 0x05;

	// on active le GPS, P4,0
	P4OUT |= 0x01;

	gpsSend(sendRate);
	gpsSend(disableAll);
	gpsSend(onlyGGAandGPRMC);

	// activation interruption globale
	_EINT();

    for(;;);

	debug_exit(0);
}

