#include "adc.h"
#include "conf.h"
#include "usart.h"
#include <avr/interrupt.h>
#include "bit_utils.h"

volatile uint8_t adc_data_ready = 0;
volatile uint16_t avg_adc0 = 0;
volatile adc_cbuf_t cbuf_adc0;
volatile uint16_t adc_last_instant_value = 0; // Armazena o último valor lido

static inline uint16_t linearize_adc(uint16_t x_raw) {
    // Coloque sua equação de calibração aqui
    return x_raw;
}

void adc_init(void)
{
    CBUF_Init(cbuf_adc0);
    DIDR0 |= (1 << ADC0D);
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = ADC_TOP_CTC;
    ADCSRA |= (1 << ADSC);
}

// Função para calcular a média (Modo 1)
uint16_t ma_adc0(void)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < ADC_AVG_BUFFER_SIZE; i++) {
        sum += linearize_adc(cbuf_adc0.m_entry[i]);
    }
    avg_adc0 = (uint16_t)(sum >> ADC_AVG_BUFFER_SIZE_LOG2);
    return avg_adc0;
}

// Função para pegar o último valor e linearizar (Modo 2)
uint16_t get_last_adc_linearized(void)
{
    return linearize_adc(adc_last_instant_value);
}

ISR(ADC_vect)
{
    uint16_t current_reading = ADC;
    adc_last_instant_value = current_reading; // Guarda o valor instantâneo
    CBUF_Push(cbuf_adc0, current_reading);    // Coloca no buffer para o modo de média
    adc_data_ready = 1;
}
