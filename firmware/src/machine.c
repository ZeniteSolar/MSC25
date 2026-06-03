#include "machine.h"
#include "conf.h"
#include "adc.h"
#include "can_app.h"
#include "usart.h"      // Necessário para imprimir na serial
#include "bit_utils.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>      // Necessário para o sprintf

volatile state_machine_t state_machine;
volatile measurements_t measurements;
volatile uint8_t machine_clk = 0;

void machine_init(void) {
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2A = MACHINE_TOP_CTC;
    TIMSK2 |= (1 << OCIE2A);
    
    measurements.current_value = 0;
    state_machine = STATE_INITIALIZING;
}

void machine_run(void) {
    static uint16_t led_counter = 0;
    static uint8_t print_prescaler = 0; 

    if (!machine_clk) return;
    machine_clk = 0;

    if (adc_data_ready) {
        measurements.current_value = ma_adc0(); 
        adc_data_ready = 0;

        // Imprime na USART a cada 50 ciclos (~500ms)
        if (++print_prescaler >= 50) {
            print_prescaler = 0;
            char buf[50];
            uint16_t val = measurements.current_value;
            // Converte para formato decimal X.XX
            uint16_t inteiros = val / 100;
            uint16_t decimais = val % 100;
            sprintf(buf, "MSC25 -> Corrente: %u.%02u A\r\n", inteiros, decimais);
            usart_send_string(buf);
        }
    }

    switch (state_machine) {
        case STATE_INITIALIZING:
            state_machine = STATE_RUNNING;
            break;
            
        case STATE_RUNNING:
            #ifdef CAN_ON
                can_app_task();
            #endif
            
            #ifdef LED_ON
                if(++led_counter >= (MACHINE_FREQUENCY / 2)) {
                    cpl_bit(LED_PORT, LED1); 
                    led_counter = 0;
                }
            #endif
            break;
            
        default:
            state_machine = STATE_INITIALIZING;
            break;
    }
}

ISR(TIMER2_COMPA_vect) {
    machine_clk = 1;
}