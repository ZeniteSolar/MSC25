#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "conf.h"
#include "cbuf.h"

typedef struct {
    uint8_t  m_putIdx;
    uint16_t m_entry[cbuf_adc0_SIZE];
} adc_cbuf_t;

extern volatile adc_cbuf_t cbuf_adc0;
extern volatile uint8_t adc_data_ready;
extern volatile uint16_t adc_last_instant_value; // Variável para o valor instantâneo

void adc_init(void);
uint16_t ma_adc0(void);
uint16_t get_last_adc_linearized(void);

#endif // ADC_H
