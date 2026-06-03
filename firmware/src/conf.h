#ifndef CONF_H
#define CONF_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define SUPPORT_EXTENDED_CANID 0

#include "can_ids.h"

#define USART_ON
#define CAN_ON
#define ADC_ON
#define MACHINE_ON
#define LED_ON
#define SPI_ON

#define CAN_SIGNATURE_SELF CAN_SIGNATURE_MSC19_5 

#define MACHINE_FREQUENCY               100
#define CAN_APP_SEND_STATE_FREQ_HZ       1  
#define CAN_APP_SEND_MEASUREMENT_FREQ_HZ 10 

#define MACHINE_TIMER_PRESCALER 1024
#define MACHINE_TOP_CTC (F_CPU / (MACHINE_TIMER_PRESCALER * 2UL * MACHINE_FREQUENCY)) - 1

#define ADC_AVG_BUFFER_SIZE     64
#define ADC_AVG_BUFFER_SIZE_LOG2 6
#define cbuf_adc0_SIZE          ADC_AVG_BUFFER_SIZE

#define LED_PORT    PORTC
#define LED_DDR     DDRC
#define LED1        PC5

#endif