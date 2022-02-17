#include "lcd_8bitsA.h"



void flip_EN (void) {
    LCD_EN = 1;
    __delay_ms(5);
    LCD_EN = 0;
}

//enviar comandos a la LCD
void LCD_CMD (char cmd){
    LCD_RS = 0; // los D son comandos
    LCD_PORT = cmd;
    flip_EN ();
    return;
}

void LCD_START(void){
    LCD_EN = 0;
    LCD_RS = 0;
    LCD_PORT = 0;
    LCD_TRIS = 0;
    
    //*********************** modo de incio ******************************
    __delay_ms(20);
    LCD_CMD (0x30); //0011 0000
    
    __delay_ms(5);
    LCD_CMD (0x30); //0011 0000
    
    __delay_us(200);
    LCD_CMD (0x30); //0011 0000
 
    //*********************** configuracion ******************************
    //function set: modo 8 bits, modo de 2 lineas, formato 5x11
    LCD_CMD (0x3C); //0011 1000=38, 
    
    //display ON/OFF: display encendido, cursor apagado, blink off
    LCD_CMD (0x0C); //0000 1100
    
    //cursor display or display shift: mover el cursor a la izq y decrementa x 1
    LCD_CMD (0x10); //0001 0000
    
    //clear display:
    LCD_CMD (0x01); //0000 0001
    
    //entry mode: cursor se mueve a la derechha DDRAM inc 1, no shiftea
    LCD_CMD (0x06); //0000 0110
}

//limpiar la LCD
void LCD_CLR (void){
    LCD_CMD (0x01);
}

//colocar el cursor de la lcd
void LCD_setCursor (char fila, char columna){
    switch (fila){
        //display superior
        case 1:
            LCD_CMD(((columna & 0x0F)-1) | 0x80); //1000 ????
            break;
        
        //display inferior
        case 2:
            LCD_CMD(((columna & 0x0F)-1) | 0xC0); //1100 ????
            break;
            
        default:
            LCD_CMD(((columna & 0x0F)-1) | 0x80); //1000 ????
            break;
    }
}

//escribir algo en la LCD
void LCD_write (char value){
    LCD_RS = 1; //direcciones de los caracters
    LCD_PORT = value; //el puerto es igual a lo ingresado
    flip_EN();
    LCD_RS = 0;
    return;
}

//mandar texto a la LCD
void LCD_writeString (char *text) {
    int i;
    for (i = 0; text[i] != '\0'; i++){
        LCD_write(text[i]);
    }
    return;
}

void LCD_shiftR (void){
    LCD_CMD(0x01); //0000 0001
    LCD_CMD(0x0C); //0000 1100
    return;
}

void LCD_shiftL (void){
    LCD_CMD(0x01); //0000 0001
    LCD_CMD(0x08); //0000 1000
    return;
}