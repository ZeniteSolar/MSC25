#include "usart.h"
#include "conf.h"
#include <avr/io.h>
#include <stdio.h>

#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD-1

void usart_init(void)
{
    UBRR0H = (uint8_t)(MYUBRR >> 8);
    UBRR0L = (uint8_t)MYUBRR;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void usart_send_char(char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void usart_send_string(const char *s)
{
    while (*s) {
        usart_send_char(*s++);
    }
}

void usart_send_uint16(uint16_t num)
{
    char buffer[6]; // 5 dígitos + null
    sprintf(buffer, "%u", num);
    usart_send_string(buffer);
}
