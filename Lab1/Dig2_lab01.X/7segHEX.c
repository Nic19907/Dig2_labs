#include "7segHEX.h"

uint8_t tabla_7segHEX (uint8_t valor) {
    switch (valor){
        case 0: //0
            return 0b00111111;
            break;

        case 1: //1
            return 0b00000110;
            break;

        case 2: //2
            return 0b01011011;
            break;

        case 3: //3
            return 0b01001111;
            break;

        case 4: //4
            return 0b01100110;
            break;

        case 5: //5
            return 0b01101101;
            break;

        case 6: //6
            return 0b01111101;
            break;

        case 7: //7
            return 0b00000111;
            break;

        case 8: //8
            return 0b01111111;
            break;

        case 9: //9
            return 0b01100111;
            break;

        case 10: //A
            return 0b01110111;
            break;

        case 11: //b
            return 0b01111100;
            break;

        case 12: //C
            return 0b00111001;
            break;

        case 13: //d
            return 0b01011110;
            break;

        case 14: //E
            return 0b01111001;
            break;

        case 15: //F
            return 0b01110001;
            break;

        default:
            return 0b01010101;
            break;
    }
}