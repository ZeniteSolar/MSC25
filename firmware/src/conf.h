#ifndef CONF_H
#define CONF_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "can_ids.h"

// --- MODO DE LEITURA DO ADC (NOVA SEÇÃO) ---
#define ADC_MODE_AVERAGED       1 // Usa a média do buffer circular (bom para estabilidade)
#define ADC_MODE_INSTANTANEOUS  2 // Usa a última leitura instantânea (bom para picos)

// Escolha seu modo aqui:
#define ADC_READING_MODE ADC_MODE_AVERAGED
// Escreva o modo na linha acima

// --- Ativação de Módulos ---
#define USART_ON
#define CAN_ON
#define ADC_ON
#define MACHINE_ON
#define SPI_ON
#define LED_ON
#define WATCHDOG_ON

// --- Configurações de Debug ---
#define VERBOSE_ON
#define VERBOSE_ON_INIT
#define VERBOSE_ON_MACHINE
#define VERBOSE_ON_ADC
#define VERBOSE_ON_CAN_APP
#define VERBOSE_ON_ERROR

// --- Configuração da Placa ---
#define CAN_SIGNATURE_SELF CAN_SIGNATURE_MSC25_1

// --- Configuração do ADC ---
#define ADC_FREQUENCY           1000
#define ADC_TIMER_PRESCALER     64
#define ADC_TOP_CTC             (F_CPU / (ADC_TIMER_PRESCALER * (unsigned long)ADC_FREQUENCY)) - 1
#define ADC_AVG_BUFFER_SIZE     32
#define ADC_AVG_BUFFER_SIZE_LOG2 5
#define cbuf_adc0_SIZE          ADC_AVG_BUFFER_SIZE

// --- Configuração da Máquina de Estados ---
#define MACHINE_FREQUENCY       100
#define MACHINE_TIMER_PRESCALER 1024
#define MACHINE_TOP_CTC         (F_CPU / (MACHINE_TIMER_PRESCALER * 2UL * MACHINE_FREQUENCY)) - 1

// --- Configuração da Aplicação CAN ---
#define CAN_APP_SEND_STATE_FREQ_HZ       CAN_MSG_MSC25_1_STATE_FREQUENCY
#define CAN_APP_SEND_MEASUREMENT_FREQ_HZ CAN_MSG_MSC25_1_MEASUREMENTS_FREQUENCY

// --- Configuração do LED de Status ---
#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED1        PB1

#endif // CONF_H
