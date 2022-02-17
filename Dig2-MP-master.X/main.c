/*
 * File:   main.c
 * Author: NicoU
 * Descripcion: Codigo maestro para la comunicacion SPI con 3 esclavos
 *
 * Created on 15 de febrero de 2022, 05:48 PM
 */


#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "SPI.h"
#include "lcd_8bitsA.h"
#include "UART.h"



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
//Slaves
#define slavePote   RB4
#define slaveBoton  RB6
#define slaveTemp   RB5

#define tiempo 50

#define t_escribir 10

//Funciones para la LCD, ojo no jala todavia si defino en main
//#define LCD_RS PORTDbits.RD0
//#define LCD_EN PORTDbits.RD1
//#define LCD_PORTD   PORTA
//#define LCD_TRIS    TRISA

/*
--------------------------------------------------------------------------------
 *                              Variables
--------------------------------------------------------------------------------
 */
uint8_t trash; //basura

uint8_t state; //estado de lectura de slaves


struct read {
    uint8_t volt;
    uint8_t cont;
    uint8_t temp;
} lector;

struct decadas {
    uint8_t cen;
    uint8_t dec;
    uint8_t uni;
}pot, bot, tem;

struct maps {
    uint16_t voltage;
}final;

struct texto {
    //para la lcd
    char adc [5];
    char boton [5];
    char sensor [5];
    
    //para UART
    char potenciometro[10];
    char botones[10];
    char temperatura[10];
}buffer; //se escriben los strings

const char intro[] = " S1:   S2:  S3:";
/*
--------------------------------------------------------------------------------
 *                       Prototipo de funciones
--------------------------------------------------------------------------------
 */
//configuracion
void setup          (void);
void config_io      (void);
void config_clock   (void);
void config_lcd     (void);
void config_ie      (void);
void config_tmr1    (void);

//operaciones
void division (uint16_t value, uint8_t *centena, uint8_t *decena, uint8_t *unidad);
void mapeo (uint8_t valor, uint16_t *mapeado);


void flip_slave (void);
void lcd_show (void);
void print (void);
/*
--------------------------------------------------------------------------------
 *                              Interrupcion
--------------------------------------------------------------------------------
 */

void __interrupt () isr(void){
    if (PIR1bits.TMR1IF){
        
        
        print();
        
        TMR1H = 11;             // preset for timer1 MSB register
        TMR1L = 220;             // preset for timer1 LSB register
        PIR1bits.TMR1IF = 0; //la bandera del tmr1 incia en 0
    }
}

/*
--------------------------------------------------------------------------------
 *                                 Funciones
--------------------------------------------------------------------------------
 */
void division (uint16_t value, uint8_t *centena, uint8_t *decena, uint8_t *unidad){   
    *centena = value/100;
    *decena = (value%100)/10;
    *unidad = (value%100)%10;
}

void mapeo (uint8_t valor, uint16_t *mapeado){
    *mapeado = valor*100/51;
}

void flip_slave (void){
    switch (state) {
        case 0: //slave ADC
            slavePote   = 0;
            slaveBoton  = 1;
            slaveTemp   = 1;
            __delay_ms(1); //slave select

            spiWrite(trash);
            lector.volt = spiRead(); //guardar la lectura en la variable

            __delay_ms(1);
            slavePote = 1; //deselect

            __delay_ms(tiempo);

            state = 1;
            break;
                
        case 1: //slave contador
            slavePote   = 1;
            slaveBoton  = 0;
            slaveTemp   = 1;
            __delay_ms(1); //slave select

            spiWrite(trash);
            lector.cont = spiRead(); //guardar la lectura en la variable

            __delay_ms(1);
            slaveBoton = 1; //deselect

            __delay_ms(tiempo);

            state = 2;
            break;

        case 2: //slave contador
            slavePote   = 1;
            slaveBoton  = 1;
            slaveTemp   = 0;
            __delay_ms(1); //slave select

            spiWrite(trash);
            lector.temp = spiRead(); //guardar la lectura en la variable

            __delay_ms(1);
            slaveTemp = 0; //deselect

            __delay_ms(tiempo);

            state = 0;
            break;

        default:
            slavePote   = 1;
            slaveBoton  = 1;
            slaveTemp   = 1;
            __delay_ms(1); //slave select

            spiWrite(trash);
            lector.volt = spiRead(); //guardar la lectura en la variable

            __delay_ms(1);
            slavePote = 1; //deselect

            __delay_ms(tiempo);
            
            
            state = 1;
            break;
        }
}

void lcd_show (void){
    //funcion para potenciometro
    mapeo (lector.volt, &final.voltage);      
    division (final.voltage, &pot.cen, &pot.dec, &pot.uni);

    sprintf(buffer.adc, "%d.%d%dV", pot.cen, pot.dec, pot.uni);
    LCD_setCursor (2, 1);
    LCD_writeString (buffer.adc);


    //funciom para botones
    division (lector.cont, &bot.cen, &bot.dec, &bot.uni);

    sprintf(buffer.boton, "%d%d%d", bot.cen, bot.dec, bot.uni);
    LCD_setCursor (2, 7);
    LCD_writeString (buffer.boton);


    //funcion para temperatura
    division (lector.temp, &tem.cen, &tem.dec, &tem.uni);
    sprintf(buffer.sensor, "%d%d%d", tem.cen, tem.dec, tem.uni);
    LCD_setCursor (2, 11);
    LCD_writeString (buffer.sensor);
}

void print (void){
    sprintf(buffer.potenciometro, "%d.%d%dV  ", pot.cen, pot.dec, pot.uni);
    sprintf(buffer.botones, "%d%d%d  ", bot.cen, bot.dec, bot.uni);
    sprintf(buffer.temperatura, "%d%d%d", tem.cen, tem.dec, tem.uni);

    uartWrite (&intro);
    uartEnter();
    uartWrite (&buffer.potenciometro);
    uartWrite (&buffer.botones);
    uartWrite (&buffer.temperatura);
    uartEnter();
    //__delay_ms(10);
}

/*
--------------------------------------------------------------------------------
 *                                 Main/loop
--------------------------------------------------------------------------------
 */
void main(void) {
    setup();
    
    while (1){

        flip_slave();
        
        lcd_show();
        
        //print();
       
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
    config_lcd();
    config_tmr1();
    config_ie();
    
    
    //configuracion de librerias
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    uartInit();
    
}

void config_io (void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    //definir el bit util de la UART en su libreria TRISC = TRISC | valor
    TRISD = 0;
    TRISE = 0;
    
    //limpie todos los valores
    PORTA = 0; //puerto para la LCD
    PORTB = 0x70;
    
    PORTC = 0; //UART y SPI
    PORTD = 0; //Enable y RS
}

void config_clock (void){
    OSCCONbits.IRCF = 0b110;    //4MHz
    OSCCONbits.SCS  = 1;    //reloj interno
}

void config_lcd (void){
    LCD_START();
    
    LCD_setCursor (1,1);
    LCD_writeString (" S1:  S2:  S3:");
}

void config_ie(void) {
    INTCONbits.GIE  = 1; //interrupciones gobales
    INTCONbits.PEIE = 1; //interrupciones perifericas
    
    PIE1bits.TMR1IE = 1; //interrupcion del tmr1 activada
}

void config_tmr1(void) {
    //Timer1 Registers Prescaler= 8 - TMR1 Preset = 3036 - Freq = 2.00 Hz - Period = 0.500000 seconds
    T1CONbits.T1CKPS1 = 1;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 1;   // bit 4
    T1CONbits.T1OSCEN = 0;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.T1SYNC = 1;    // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = 11;             // preset for timer1 MSB register
    TMR1L = 220;             // preset for timer1 LSB register
    
    //3036
    //
    PIR1bits.TMR1IF = 0; //la bandera del tmr1 incia en 0
}