/*
 * File:   cont_usart_puerto.c
 * Author: NicoU
 *
 * Created on 3 de febrero de 2022, 08:52 AM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart_config.h"

#define _XTAL_FREQ 4000000

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */

void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_ie      (void);
void usart_setup    (void);

void setup (void) {
    config_io();
    config_clock();
    config_ie();
    usart_setup();
    return;
}


/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.RCIF){
        PIR1bits.RCIF = 0;
    }
}

/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */
void main(void) {
    setup();
    while(1){
        
    }
    return;
}

void config_io (void) {
    ANSEL   = 0;
    ANSELH = 0;
    
    TRISA = 0; //
    TRISB = 0;
    TRISC = 0; // control display y usart
    TRISCbits.TRISC7 = 1; //RX como input
    TRISD = 0; // display
    
    PORTA = 0; //
    PORTB = 0;
    PORTC = 0; //
    PORTD = 0; //
    return;
}

void config_clock (void) {
    OSCCONbits.IRCF = 0b110;    //4MHz
    OSCCONbits.SCS  = 1;    //reloj interno
    return;
}

void config_ie(void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    PIE1bits.RCIE   = 1; //interrupcion del USART
    return;
}

void usart_setup (void){
    USART_CONFIG();
    PIR1bits.RCIF = 0;
}

