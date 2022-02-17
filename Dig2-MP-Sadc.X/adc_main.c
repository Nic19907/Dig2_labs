/*
 * File:   adc_main.c
 * Author: NicoU
 *
 * Created on 15 de febrero de 2022, 08:31 PM
 */




#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

#include "SPI.h"
#include "ADC.h"

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

//esta en rb5
/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
uint8_t voltaje;
uint8_t trash;

/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */
void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_ie      (void);
void config_adc     (void);

/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */
void __interrupt() isr(void){
    if (PIR1bits.SSPIF){ //interrupcion de esclavo
        trash = spiRead();
        spiWrite(voltaje);
        PIR1bits.SSPIF = 0; //apagar banderas
    }
    
    if (PIR1bits.ADIF){ //interrupcion adc
        voltaje = ADRESH; //guardar el valor del ADRESH en una variable
        //PORTD = ADRESH;
        PIR1bits.ADIF = 0; //apagar bandera
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
    
    while (1){
        if (ADCON0bits.GO == 0){
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
        //PORTD = voltaje;
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
    config_adc();
    
    //configuracion del spi
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

void config_io (void) {
    ANSEL = 0;
    ANSELH = 0;
    ANSELHbits.ANS13 = 1;//RB5 como analogico
    
    TRISB = 0;
    TRISBbits.TRISB5 = 1; //RB5 como entrada
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
    
    
    PIE1bits.ADIE   = 1; //interrupcion del ADC
    PIE1bits.SSPIE   = 1; //del SPI
}

void config_adc(void){
    con_ADC (_4MHz);
    ADCON0bits.CHS  = 13;    //empezar por el AN13
    __delay_us(50);
    PIR1bits.ADIF = 0;
}