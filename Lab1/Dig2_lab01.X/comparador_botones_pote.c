/*
 * File:   pre_semaforo.c
 * Author: NicoU
 * Programa: Tiene un potenciometro de entrada analogica que saca su resultado en
 * el 7 segmentos, botones que incrementan un contador y si el valor del pote es
 * mayor al contador se enciende un led
 * 
 * Created on 17 de enero de 2022, 06:38 PM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC_config.h"
#include "7segHEX.h"

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
 *                              Definiciones
--------------------------------------------------------------------------------
 */
#define Tdec PORTDbits.RD0
#define Tuni PORTDbits.RD1
#define mayor PORTDbits.RD2

#define con_inc   PORTBbits.RB0
#define con_dec   PORTBbits.RB1
/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */

uint8_t flip; //cambia de estados el multiplexor

uint8_t dec; //decenas de la division para el 7seg
uint8_t uni; //unidades de la division para el 7seg

uint8_t voltaje; //valor obtenido del module ADC

uint8_t display1; //display para la decenas
uint8_t display0; //display para las unidades

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
void config_adc     (void);
void config_iocb    (void);
void config_pull    (void);

void setup (void) {
    config_io();
    config_clock();
    config_tmr1();
    config_ie();
    config_adc();
    config_iocb();
    config_pull();
    return;
}

void division (uint8_t value); //codigo para dividir la variable en
//decenas y unidades

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.TMR1IF){ //si se activa la bandera del tmr1       
        switch (flip){ //cambio de estados en el multiplexor
            case 0: //ensenna las decenas
                Tuni = 0;
                Tdec = 1;
                PORTC = display1;
                flip = 1; //se cambia su valor para activar el siguiente estado
                break;
            case 1: //ensenna las unidades
                Tuni = 1;
                Tdec = 0;
                PORTC = display0;
                flip = 0;
                break;
                
            default: //en caso de fallos
                Tuni = 1;
                Tdec = 1;
                PORTC = 0b11111111;
                flip = 1;
                break;
        }
        PIR1bits.TMR1IF = 0; //apagar la bandera de tmr1
        TMR1H = 248;             // preset for timer1 MSB register
        TMR1L = 48;             // preset for timer1 LSB register
    }    
    if (PIR1bits.ADIF){ //si se activa la bandera del ADC
        voltaje = ADRESH; //colocar el valor del ADRESH en una variable
        PIR1bits.ADIF = 0; //apagar la bandera del ADC
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
    ADCON0bits.GO = 1;
    flip = 0; //iniciar el flip en 0
    
    while (1){ //loop
        if (ADCON0bits.GO == 0){
            //__delay_us(50);
            ADCON0bits.GO = 1;
            PIR1bits.ADIF = 0;
        }
        
        if (voltaje > PORTA){
            PORTDbits.RD2 = 1;
        }
        
        if (voltaje <= PORTA){
            PORTDbits.RD2 = 0;
        }
        
        division (voltaje);
        display1 = tabla_7segHEX (dec);
        display0 = tabla_7segHEX (uni);
    }
}

/*
--------------------------------------------------------------------------------
 *                               Division
--------------------------------------------------------------------------------
 */
void division (uint8_t value){
    dec = value/16; //el resultado son las decenas
    uni = value%16; //el residuo son las unidades
    return;
}
/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */

void config_io (void) {
    ANSEL   = 0;
    ANSELH  = 0;
    ANSELHbits.ANS13 = 1; //potenciometro
    
    TRISA = 0; //tira de leds
    TRISB = 0b111; //botones
    TRISBbits.TRISB5 = 1; // potenciometro
    TRISC = 0; //7seg
    TRISD = 0; //transistores
    
    PORTA = 0; //tira de leds
    PORTB = 0; //botes y pote
    PORTC = 0; //7 seg
    PORTD = 0; //transistores
    
    return;
}

void config_clock (void) {
    OSCCONbits.IRCF = 0b110;    //2MHz
    OSCCONbits.SCS  = 1;    //reloj interno
    return;
}

void config_tmr1(void) {
    //Timer1 Registers Prescaler= 1 - TMR1 Preset = 63536 - Freq = 500.00 Hz - Period = 0.002000 seconds
    T1CONbits.T1CKPS1 = 0;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0;   // bit 4
    T1CONbits.T1OSCEN = 0;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 248;             // preset for timer1 MSB register
    TMR1L = 48;             // preset for timer1 LSB register
    
    //55536
    //
    PIR1bits.TMR1IF = 0; //la bandera del tmr1 incia en 0
    return;
}

void config_ie(void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    PIE1bits.TMR1IE = 1; //interrupcion del tmr1 activada
    PIE1bits.ADIE   = 1; //interrupcion del ADC
    INTCONbits.RBIE = 1; //interrupcion de los botones del puerto B activada
    return;
}

void config_adc(void){
    con_ADC (_4MHz);
    ADCON0bits.CHS  = 13;    //empezar por el AN13
    __delay_us(50);
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