#include "ADC.h"

void con_ADC (uint8_t ADC_valor){
    if (ADC_valor == 1){    // 1MHz
        ADCON1bits.ADFM     = 0; //justificacion del resultado ADC
        ADCON1bits.VCFG0    = 0; //referencia de voltaje en VDD
        ADCON1bits.VCFG1    = 0; //referencia de voltaje en VSS

        ADCON0bits.ADCS = 0b00; //refresco de 2.0 us
        ADCON0bits.ADON = 1;    //habilitar el ADC
    }
    
    if (ADC_valor == 2){    // 4MHz
        ADCON1bits.ADFM     = 0; //justificacion del resultado ADC
        ADCON1bits.VCFG0    = 0; //referencia de voltaje en VDD
        ADCON1bits.VCFG1    = 0; //referencia de voltaje en VSS

        ADCON0bits.ADCS = 0b01; //refresco de 2.0 us
        ADCON0bits.ADON = 1;    //habilitar el ADC
    }
    
    if (ADC_valor == 3){    // 8MHz
        ADCON1bits.ADFM     = 0; //justificacion del resultado ADC
        ADCON1bits.VCFG0    = 0; //referencia de voltaje en VDD
        ADCON1bits.VCFG1    = 0; //referencia de voltaje en VSS

        ADCON0bits.ADCS = 0b10; //refresco de 4.0 us
        ADCON0bits.ADON = 1;    //habilitar el ADC
    }
}