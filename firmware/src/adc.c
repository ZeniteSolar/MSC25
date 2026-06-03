#include "adc.h"
#include "conf.h"
#include "usart.h"
#include <avr/interrupt.h>
#include "bit_utils.h"

volatile uint8_t adc_data_ready = 0;
volatile adc_cbuf_t cbuf_adc0;
volatile uint16_t adc_last_instant_value = 0; 

// --- CALIBRAÇÃO FINAL ---
#define ADC_OFFSET_UNIT  504    // Zero Amperes (Mantido)
#define ADC_FACTOR       48     // RECALCULADO: De 43 para 51 (Ajuste para 3.05A)

void adc_init(void)
{
    CBUF_Init(cbuf_adc0);
    DIDR0 |= (1 << ADC0D); 
    ADMUX = (1 << REFS0); 
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = 0; 
    ADCSRA |= (1 << ADSC);
}

// FUNÇÃO DE SOBREAMOSTRAGEM (ALTA RESOLUÇÃO)
uint16_t ma_adc0(void)
{
    uint32_t sum_raw = 0;
    
    // 1. Soma 64 amostras
    for (uint8_t i = 0; i < ADC_AVG_BUFFER_SIZE; i++) {
        sum_raw += cbuf_adc0.m_entry[i];
    }
    
    // 2. Offset total de 64 amostras
    uint32_t offset_total = (uint32_t)ADC_OFFSET_UNIT * ADC_AVG_BUFFER_SIZE;
    
    int32_t diff_total;
    
    // 3. Diferença acumulada
    if (sum_raw > offset_total) {
        diff_total = (int32_t)(sum_raw - offset_total);
    } else {
        diff_total = (int32_t)(offset_total - sum_raw);
    }

    // 4. Aplica novo fator (51)
    uint32_t current_calibrated = (uint32_t)((diff_total * ADC_FACTOR) >> ADC_AVG_BUFFER_SIZE_LOG2);

    return (uint16_t)current_calibrated;
}

uint16_t get_last_adc_linearized(void)
{
    uint16_t x_raw = adc_last_instant_value;
    int32_t val;
    if (x_raw > ADC_OFFSET_UNIT) val = (x_raw - ADC_OFFSET_UNIT) * ADC_FACTOR;
    else val = (ADC_OFFSET_UNIT - x_raw) * ADC_FACTOR;
    return (uint16_t)val;
}

ISR(ADC_vect)
{
    uint16_t current_reading = ADC;
    adc_last_instant_value = current_reading; 
    CBUF_Push(cbuf_adc0, current_reading);   
    adc_data_ready = 1; 
}