#include "lcd_8bits.h"

#define _XTAL_FREQ 4000000

//sistema de incio para la lcd
void LCD_START(void){
    LCD_1PORT(0x00); //todo 0
    __delay_ms(20);
    LCD_CMD(0x30); // 0011 0000
    __delay_ms(5);
    LCD_CMD(0x30); // 0011 0000
    __delay_ms(11);
    LCD_CMD(0x30); // 0011 0000
//------------------------------ ya inicio --------------------------------
    //0011 1000
    LCD_CMD(0x38);
    
    //0001 0000
    LCD_CMD(0x10); //visualizador apagado
    
    //0000 0001
    LCD_CMD(0x01); //visualizador borrado
    
    //0000 0110
    LCD_CMD(0x06); //modo de caracteres: incremento sin desplazamiento
    
    //0001 0000
    LCD_CMD(0x10);
    
    //0000 1100
    LCD_CMD(0x0C);
    __delay_ms(100);
}


//funcion para puerto 1
void LCD_1PORT(char a)
{
	if(a & 1)
		LCD_D4 = 1;
	else
		LCD_D4 = 0;

	if(a & 2)
		LCD_D5 = 1;
	else
		LCD_D5 = 0;

	if(a & 4)
		LCD_D6 = 1;
	else
		LCD_D6 = 0;

	if(a & 8)
		LCD_D7 = 1;
	else
		LCD_D7 = 0;
}

//funcion para 2 puertos
void LCD_2PORT (char a){
    if (a & 1)
        LCD_D0 = 1;
    else
        LCD_D0 = 0;

    if (a & 2)
        LCD_D1 = 1;
    else
        LCD_D1 = 0;

    if (a & 4)
        LCD_D2 = 1;
    else
        LCD_D2 = 0;

    if (a & 8)
        LCD_D3 = 1;
    else
        LCD_D3 = 0;
    
    if (a & 16)
        LCD_D4 = 1;
    else
        LCD_D4 = 0;

    if (a & 32)
        LCD_D5 = 1;
    else
        LCD_D5 = 0;

    if (a & 64)
        LCD_D6 = 1;
    else
        LCD_D6 = 0;

    if (a & 128)
        LCD_D7 = 1;
    else
        LCD_D7 = 0;
}

//permite la carga en el comando
void LCD_CMD (char a){
    LCD_RS = 0; // los D son comandos
    LCD_EN = 1; // operacion normal
    PORTD = a;
    __delay_ms(5);
    LCD_EN = 0; //acceso a la lcd desabilitado
}

//limpia la pantalla
void LCD_CLR (){
    LCD_CMD (0);
    LCD_CMD (1);
}

//apunta el cursor a un lugar deseado
void LCD_setCursor (char a, char b){
    //escribir
    if (a == 1) {
        PORTD = ((b & 0x0F)| 0x80); //1000 xxxx
    }
    //leer
    else if (a == 2) {
        PORTD = ((b & 0x0F)| 0xC0); //1100 xxxx
    }
}






//permite escribir un char en la lcd
void LCD_writeChar (char a){
    LCD_RS = 1;
    PORTD = a;
    LCD_EN = 1;
    __delay_us(40);
    LCD_EN = 0;
}

//permite escribir palabras
void LCD_writeString (char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++){
        LCD_writeChar(a[i]);
    }
}

void LCD_shiftR (void){
    LCD_CMD(0b1);
    LCD_CMD(0X0C); //1100
}

void LCD_shiftL (void){
    LCD_CMD(0b01);
    LCD_CMD(0X08); //1000
}