#include "usart_config.h"

#define _XTAL_FREQ 4000000

void USART_CONFIG (void){
    SPBRG = 51;
    SPBRGH = 1;
    BAUDCTLbits.BRG16 = 1; //generacion de 16 bits de baudio
    
    TXSTAbits.SYNC = 0; //asincrono
    RCSTAbits.SPEN = 1; //habilita los pines en modo serial
    
    TXSTAbits.TXEN = 1; //con transmicion
    RCSTAbits.CREN = 0; //con recepcion
    
    TXSTAbits.TX9 = 0;  //tra de 8 bits
    RCSTAbits.RC9 = 0;  //res de 8 bits  
}
