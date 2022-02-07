/*
 * File:   lcd_potes.c
 * Author: NicoU
 * Programa: Dos potenciometros los cuales reflejan un valor en la LCD
 * 
 * Created on 17 de enero de 2022, 06:38 PM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADC_config.h"
#include "lcd_8bitsA.h"


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

//LCD
#define LCD_EN PORTCbits.RC5
#define LCD_RS PORTCbits.RC4
#define LCD_D0 PORTDbits.RD0
#define LCD_D1 PORTDbits.RD1
#define LCD_D2 PORTDbits.RD2
#define LCD_D3 PORTDbits.RD3
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
uint8_t volt1; //
uint8_t volt2; //

uint8_t pote1; //valor obtenido del module ADC 1
uint8_t pote2; //valor obtenido del module ADC 2

uint8_t adc_change; //cambiar el ADC que se este utilizando

uint8_t valor;
uint16_t value;
       

uint8_t cen; //son char ya que llevan decimales
uint8_t dec;
uint8_t uni;

char buffer[5]; //traductor de numero a char

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
void lcd_setup      (void);

void division       (uint16_t value);
void transform      (uint8_t valor);

void setup (void) {
    config_io();
    config_clock();
    config_tmr1();
    config_ie();
    config_adc();
    lcd_setup();
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
        TMR1H = 248;             // preset for timer1 MSB register
        TMR1L = 48;             // preset for timer1 LSB register
    }
    
    if (PIR1bits.ADIF){ //si se activa la bandera del ADC
        if (ADCON0bits.CHS == 0){
            pote1 = ADRESH;
        }
        else{
            pote2 = ADRESH;
        }
        PIR1bits.ADIF = 0;
    }
}

/*
--------------------------------------------------------------------------------
 *                                 Funciones
--------------------------------------------------------------------------------
 */

void division (uint16_t value){   
    cen = value/100;
    dec = (value%100)/10;
    uni = (value%100)%10;
}

void transform (uint8_t valor){
    uint16_t temp;      //se asigna una varible de 16 bits por el mapeo
    temp = valor*100/51;//funcion de mapoe de 255 a 500
    division(temp);     //se ingresa el valor mapeado a la funcion de division
    sprintf(buffer, "%d.%d%d", cen, dec, uni); //salen ya los datos como string
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
            switch (adc_change){
                case 0:
                    ADCON0bits.CHS = 0;
                    __delay_us(50);
                    adc_change = 1;
                    break;
                    
                case 1:
                    ADCON0bits.CHS = 1;
                    __delay_us(50);
                    adc_change = 0;
                    break;
                    
                default:
                    ADCON0bits.CHS = 0;
                    __delay_us(50);
                    adc_change = 1;
                    break;    
            }
            
            ADCON0bits.GO = 1;
            PIR1bits.ADIF = 0;
        }
        
        
        LCD_setCursor(2, 1);
        transform(pote1);       //ingresar el valor del potenciometro 1
        LCD_writeString(buffer);//se escribe en la lcd
        LCD_writeString("V ");   //espacio por orden
        
        transform(pote2);       
        LCD_writeString(buffer);
        LCD_writeString("V ");
        
        
    }
}

/*
--------------------------------------------------------------------------------
 *                              Configuracion
--------------------------------------------------------------------------------
 */
void config_io (void) {
    ANSEL   = 0;
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS1 = 1;
    ANSELH = 0;
    
    TRISA = 0; //
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1; //potenciometroa
    
    TRISB = 0;
    
    TRISC = 0; // control display
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
    return;
}

void config_adc(void){
    con_ADC (_4MHz);
    ADCON0bits.CHS  = 13;    //empezar por el AN13
    __delay_us(50);
}

void lcd_setup (void) {
    LCD_START();
    LCD_setCursor(1, 1);
    LCD_writeString("S1:   S2:  S3:"); //texto inicial
    
    //sprintf(buffer, "textp y ponerle %d", numero)
    //LCD_writeString(buffer)
}