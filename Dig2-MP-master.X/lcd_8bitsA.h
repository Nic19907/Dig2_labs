/* 
 * File: lcd_8bitsA.h   
 * Author: Nicolas Urioste
 * Comments: Codigo para una lcd 16x2
 * Revision history: 15/02/2022
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <pic16f887.h>

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 4000000
#endif

//valores para definir en el main, sino esos son los defectos
#ifndef LCD_RS
    #define LCD_RS RB7
#endif

#ifndef LCD_EN
    #define LCD_EN RD0
#endif

#ifndef LCD_PORT
    #define LCD_PORT    PORTA
#endif

#ifndef LCD_TRIS
    #define LCD_TRIS    TRISA
#endif
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

