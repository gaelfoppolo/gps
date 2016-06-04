#include <__cross_studio_io.h>
#include  <msp430x16x.h>

unsigned int delay (unsigned int x) {
    unsigned int i, j;
    for (i = 0; i<= x; i++) {
        for (j = 0; j < x; ++j) {}
    }   
    return 0;
}

void main(void){
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    
    // P1 en sortie (toutes les LEDs)
    P1DIR |= 0x1F;

   	// clear l'état des LEDs
    P1OUT = 0x0;

    // on choisit P2.0,4 comme input
    P2DIR &= ~0x1F;

    // active les interruptions P2IFG pour les pins à 1
    // donc ici on active pour les 0 à 5
    P2IE |= 0x1F;

    // flag d'interruption pour chaque pin de P2
    // 3F set le flag pour les pin 0 à 5 de P2
    //P2IFG &= ~0x3F;

    // set le mode de P2IFG pour les pin 0 à 5
    // ici on les mets à 1, donc transition de high à low
    P2IES |= 0x1F;

    // on active l'interruption globale
    // == __enable_interrupt
    // enables global interrupts by setting the GIE bit in the status register
    _EINT();

    while(1) {}

    debug_printf(0);
} 

void P2(void) __interrupt[PORT2_VECTOR] {
    
    // j'ai fait un case moche parce que flemme de faire des if ;)
    switch(P2IN) {
      //push
      case 0x1E:
        debug_printf("PUSH\n");
        P1OUT |= BIT0;
        break;
      //top
      case 0x1D:
        debug_printf("TOP\n");
        P1OUT |= BIT2;
        break;
      //RIGHT
      case 0x0F:
        debug_printf("RIGHT\n");
        P1OUT |= BIT3;
        break;
      //bottom
      case 0x1B:
        debug_printf("BOTTOM\n");
        P1OUT |= BIT1;
        break;
      //push
      case 0x17:
        debug_printf("LEFT\n");
        P1OUT |= BIT4;
        break;
      default:
        debug_printf("what button is that?!\n");
        break;
    }
    
    // on attend et mets toute les LEDs éteintes
    delay(100);
    P1OUT = 0x00;

    // on clear P2 IFG
    P2IFG &= ~0x1F;       
    
}

