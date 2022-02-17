/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef __UART_H
#define	__UART_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <pic16f887.h>

void uartInit (void);

void uartWrite (unsigned char *word);

void uartEnter (void);

#endif	/* XC_HEADER_TEMPLATE_H */

