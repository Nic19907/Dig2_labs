/*
 * File:   pre_semaforo.c
 * Author: NicoU
 *
 * Created on 17 de enero de 2022, 06:38 PM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

#define _XTAL_FREQ 2000000

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
 *                              Definiciones
--------------------------------------------------------------------------------
 */
#define con_inc   PORTBbits.RB0
#define con_dec   PORTBbits.RB1

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */

/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */

void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_tmr1    (void);
void config_ie      (void);
void config_iocb    (void);
void config_pull    (void);

void setup (void) {
    config_io();
    config_clock();
    config_tmr1();
    config_ie();
    config_iocb();
    config_pull();
    return;
}

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.TMR1IF){
        PIR1bits.TMR1IF = 0; //apagar la bandera de tmr1
    }
    
    if (INTCONbits.RBIF) { //si se incrementa el contador
        if (!con_inc){ //incremente el valor de PORTA
            PORTA ++;
        }
        
        if (!con_dec){ //decremente el valor de PORTA
            PORTA --;
        }
        
        INTCONbits.RBIF = 0; //apagar bandera de botones
    }
}


/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */
void main(void) {
    setup();
    while (1){ //loop
    }
}

/*
--------------------------------------------------------------------------------
 *                               Tablas
--------------------------------------------------------------------------------
 */

/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */

void config_io (void) {
    ANSEL   = 0;
    ANSELH  = 0;
    
    TRISA = 0; //tira de leds
    TRISB = 0b11; //botones
    
    PORTA = 0; //tira de leds
    PORTB = 0; //botes y pote
    
    return;
}

void config_clock (void) {
    OSCCONbits.IRCF = 5;    //2MHz
    OSCCONbits.SCS  = 1;    //reloj interno
    return;
}

void config_tmr1(void) {
    //Timer1 Registers Prescaler= 1 - TMR1 Preset = 63536 - Freq = 500.00 Hz - Period = 0.002000 seconds
    T1CONbits.T1CKPS1 = 0;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0;   // bit 4
    T1CONbits.T1OSCEN = 1;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 248;             // preset for timer1 MSB register
    TMR1L = 48;             // preset for timer1 LSB register
    
    //63536
    PIR1bits.TMR1IF = 0; //la bandera del tmr1 incia en 0
    return;
}

void config_ie(void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    PIE1bits.TMR1IE = 1; //interrupcion del tmr1 activada
    INTCONbits.RBIE = 1; //interrupcion de los botones del puerto B activada
    return;
}

void config_iocb(void) {
    IOCBbits.IOCB0 = 1; // interrupcion del b0 activada
    IOCBbits.IOCB1 = 1; // interrupcion del b1 activada
    INTCONbits.RBIF = 0; //la bandera de los botones inicia en 0
    return;
}

void config_pull (void){
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0  = 1;
    WPUBbits.WPUB1  = 1;
    return;
}