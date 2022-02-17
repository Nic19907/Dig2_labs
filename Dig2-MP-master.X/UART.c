/*
 * File:   UART.c
 * Author: NicoU
 *
 * Created on 16 de febrero de 2022, 02:56 PM
 */

#include "UART.h"

void uartInit(void) {
    SPBRGH = 0; //tablazo
    SPBRG = 25; //tablazo
    
    RCSTAbits.SPEN = 1; //RX y TX como puertos seriales
    RCSTAbits.CREN = 1; //activa el recieber
    
    TXSTAbits.SYNC = 0; //USART en modo asincrono
    
    //transmicion
    TXSTAbits.TX9 = 0; //comunicacion de 8 bits
    TXSTAbits.TXEN = 1; //se habilita la trnasmicion
    TXSTAbits.BRGH = 1; //En modo de alta velocidad
    
    RCSTAbits.RX9 = 0; //recepcion de 8 bits
    
    //se activa una bandera RCIF cuando se transforma un character
    //para ver recepcion chequear RCREG
    //para que jale la transmicion hay que enviar datos al TXREG
    BAUDCTLbits.BRG16 = 0; //baud rate de 8 bits
    BAUDCTLbits.WUE = 0; //funciona de manera normal
    
    PIR1bits.RCIF = 0;
    PIR1bits.TXIF = 0;
}

void uartWrite (unsigned char *word){
    while (*word != 0){                 //Verifica que el puntero aumente
        TXREG = (*word);                //Envía el caracter que toca de la cadena
        while(!TXSTAbits.TRMT);         //Espera a que se haya enviado el dato
        word++;                         //Aumenta el apuntador para ir al
    }                                   //siguente caracter
    return;
}

void uartEnter (void){
    TXREG = 13;
}
