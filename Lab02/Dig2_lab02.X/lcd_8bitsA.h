/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#include <pic16f887.h>

#define _XTAL_FREQ 4000000

//volatile unsigned *LCD_PORT = &PORTD;
//volatile unsigned *LCD_TRIS = &TRISD;


#define LCD_RS PORTCbits.RC2
#define LCD_EN PORTCbits.RC3

#define LCD_D0 PORTDbits.RD0
#define LCD_D1 PORTDbits.RD1
#define LCD_D2 PORTDbits.RD2
#define LCD_D3 PORTDbits.RD3
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7
#define LCD_PORT    PORTD
//***************************LCD comandos ******************************


void LCD_START  (void);     //codigo para inciar la lcd

void flip_E (void);
//void LCD_1PORT  (char a);
//void LCD_2PORT  (char b);

void LCD_CMD    (char cmd);
void LCD_CLR    (void);     //limpia la lcd

void LCD_setCursor (char fila, char columna); //donde colocar el cursor

void LCD_write (char value); //escribir un dato char
void LCD_writeString (char *text); //escribir un string


void LCD_shiftR (void); //mover a la derecha
void LCD_shiftL (void); //mover a la izq


#endif	/* XC_HEADER_TEMPLATE_H */

