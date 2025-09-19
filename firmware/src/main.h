#ifndef MAIN_H
#define MAIN_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "conf.h"
#include "dbg_vrb.h" // Adicionado para corrigir o erro

// Inclui todos os módulos principais
#include "usart.h"
#include "adc.h"
#include "machine.h"
#include "can_app.h"
#include "bit_utils.h"

#ifdef CAN_ON
#include "can.h"
#endif

// Declaração da função de inicialização
void init(void);

#endif // MAIN_H
