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

#define start_race  PORTBbits.RB5
#define BOT_P1      PORTBbits.RB6
#define BOT_P2      PORTBbits.RB7

#define ROJO        PORTBbits.RB0
#define AMARILLO    PORTBbits.RB1
#define VERDE       PORTBbits.RB2

#define WIN_P1      PORTBbits.RB3
#define WIN_P2      PORTBbits.RB4

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
uint8_t segundero; //contador para segundos

uint8_t activador; //verifica el estado de start_race

uint8_t race_mode; //la carrera ya inicio


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

uint8_t tabla_7seg (uint8_t uwu); //tabla para transofrmar un valor a un display
// de 7 segmentos

void setup (void) {
    config_io();
    config_clock();
    config_tmr1();
    config_ie();
    config_iocb();
    return;
}

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.TMR1IF){
        segundero++; //cada interrupcion se aumenta
        PIR1bits.TMR1IF = 0; //apagar la bandera de tmr1
    }
    
    if (INTCONbits.RBIF) { //si inicia la carrera
        if (start_race){
            activador = 1;
        }
        
        if (race_mode) {
            if (BOT_P1) {
                PORTA = PORTA << 1;
            }
            
            if (BOT_P2) {
                PORTD = PORTD << 1;
            }
        }
        INTCONbits.RBIF = 0; //apagar bandera de botones
        
        
    }

    return;
}

/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */
void main(void) {
    setup();
    while (1){ //loop
        switch (activador) {
            case 0: //no se a decidido iniciar la carrera
                PORTB = 0;
                PORTC = 0b00001001;
                segundero = 0;
                race_mode = 0;
                break;
                
            case 1: //etapa pre carrera
                if (segundero == 1){ //lleva 1 segundo
                    ROJO = 1; //enciende la luz roja
                    PORTC = tabla_7seg(segundero-1); //marca 3 en 7 seg
                }
                else if (segundero == 2){ // lleva 2 segundo
                    ROJO = 0; //apaga la luz roja
                    AMARILLO = 1; //enciende la luz amarilla
                    PORTC = tabla_7seg(segundero-1); //marca 2 en el 7 seg
                }
                else if (segundero == 3){ // lleva 3 segundos
                    AMARILLO = 0;
                    VERDE = 1;
                    PORTC = tabla_7seg(segundero-1);
                }
                else if (segundero == 4) { // lleva 4 segundos
                    ROJO = 1;
                    AMARILLO = 1;
                    VERDE = 1;
                    PORTC = tabla_7seg(segundero-1);
                    
                    PORTA = 1;
                    PORTD = 1;
                }
                else if (segundero > 4) {
                    segundero = 5;
                    race_mode = 1;
                }
                break;
                
            default:
                PORTB = 0b11001100;
                break;
        }
        
        if (PORTAbits.RA7){ //si llega a la meta ensenna que gano
            WIN_P1  = 1;
            PORTC   = 0b00000110;
            race_mode = 0;
        }
        if (PORTDbits.RD7){ //si llega a la meta ensenna que gano
            WIN_P2  = 1;
            PORTC   = 0b01011011;
            race_mode = 0;
        }
        
        if (start_race && (PORTBbits.RB3 || PORTBbits.RB4)){
            PORTA = 0;
            PORTB = 0;
            PORTC = 0b00001001;
            PORTD = 0;
            segundero = 0;
            activador = 1;
            race_mode = 0;
        }
    }
}

/*
--------------------------------------------------------------------------------
 *                               Multiplex
--------------------------------------------------------------------------------
 */
uint8_t tabla_7seg (uint8_t uwu){
    switch(uwu)
    {
        case 0: //3
            return 0b01001111;
            break;
            
        case 1: //2
            return 0b01011011;
            break;
            
        case 2: //1
            return 0b00000110;
            break;
            
        case 3: //0
            return 0b00111111;
            break;
                      
        default:
            return 0b01010101;
            break;
    }
}


/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */

void config_io(void) {
    ANSEL   = 0; //no analogicos
    ANSELH  = 0;
    
    TRISA   = 0; // contador corredor 1
    TRISB   = 0b11100000; // semaforo y ganador
    TRISC   = 0; // 7 segmentos
    TRISD   = 0; // contador corredor 2
    //TRISE   = 1; // pushbuttons
    
    PORTA   = 0;
    PORTB   = 0;
    PORTC   = 0;
    PORTD   = 0;
    //PORTE   = 0; //todos los puertos empiezan en 0
      
    return;
}

void config_clock(void) {
    OSCCONbits.IRCF = 5;    //2MHz
    OSCCONbits.SCS  = 1;    //reloj interno
    return;
}

void config_tmr1(void) {
    
    //Periodo de interrupcion de .5 segundos o 2Hz
    
    T1CONbits.T1CKPS1 = 1;   // 
    T1CONbits.T1CKPS0 = 1;   // Prescaler 1:8
    T1CONbits.T1OSCEN = 0;   // Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 11;              // preset for timer1 MSB register
    TMR1L = 220;             // preset for timer1 LSB register
    
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
    IOCBbits.IOCB5 = 1; // interrupcion del b5 activada
    IOCBbits.IOCB6 = 1; // interrupcion del b6 activada
    IOCBbits.IOCB7 = 1; // interrupcion del b7 activada
    
    INTCONbits.RBIF = 0; //la bandera de los botones inicia en 0
    return;
}