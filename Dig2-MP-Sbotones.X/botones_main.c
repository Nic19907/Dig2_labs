/*
 * File:   botones_main.c
 * Author: NicoU
 *
 * Created on 16 de febrero de 2022, 11:26 AM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

#include "SPI.h"


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
#define INC RB7
#define DEC RB5

#define LEDS PORTD

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
uint8_t trash;
uint8_t contador;
/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */
void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_ie      (void);
void config_iocb    (void);
void config_pull    (void);

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.SSPIF){
        trash = spiRead();
        spiWrite(LEDS);
        PIR1bits.SSPIF = 0; //apagar banderas
    }
    
    if (INTCONbits.RBIF){
        if (!INC){
            contador++;
        }
        if (!DEC){
            contador--;
        }
        INTCONbits.RBIF = 0;
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
        LEDS = contador;
    }
    return;
}

/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */
void setup (void){
    config_io();
    config_clock();
    config_ie();
    config_iocb();
    config_pull();
    
    //config spi
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

void config_io (void) {
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0b10100000; //botones
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
}

void config_clock (void) {
    OSCCONbits.IRCF = 0b110;    //4MHz
    OSCCONbits.SCS  = 1;    //reloj interno
}

void config_ie (void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    INTCONbits.RBIE = 1; //interrupciones de botones
    PIE1bits.SSPIE   = 1; //del SPI
}

void config_iocb(void){
    
    IOCBbits.IOCB5 = 1;
    IOCBbits.IOCB7= 1;
    INTCONbits.RBIF = 0; //la bandera empieza en 0
}

void config_pull(void){
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB5 = 1;
    WPUBbits.WPUB7 = 1;
}