#ifndef USART_H
#define USART_H

#include <stdint.h>

void usart_init(void);
void usart_send_char(char data);
void usart_send_string(const char *s);
void usart_send_uint16(uint16_t num);

#endif // USART_H
