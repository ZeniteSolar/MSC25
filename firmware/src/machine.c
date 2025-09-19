#include "machine.h"
#include "conf.h"
#include "usart.h"
#include "adc.h"
#include "can_app.h"
#include "bit_utils.h"
#include <avr/interrupt.h>

volatile state_machine_t state_machine;
volatile measurements_t measurements;
volatile uint8_t machine_clk = 0;

static void set_state_initializing(void) { state_machine = STATE_INITIALIZING; }
static void set_state_running(void) { state_machine = STATE_RUNNING; }

void machine_init(void)
{
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2A = MACHINE_TOP_CTC;
    TIMSK2 |= (1 << OCIE2A);
    measurements.current_value = 0;
    set_state_initializing();
}

static void read_sensor_data(void)
{
    #if (ADC_READING_MODE == ADC_MODE_AVERAGED)
        // Modo 1: Pega o valor da média do buffer
        measurements.current_value = ma_adc0();
    #elif (ADC_READING_MODE == ADC_MODE_INSTANTANEOUS)
        // Modo 2: Pega o último valor lido, já linearizado
        measurements.current_value = get_last_adc_linearized();
    #else
        #error "Modo de leitura de ADC inválido selecionado em conf.h"
    #endif
}

void machine_run(void)
{
    if (!machine_clk) {
        return;
    }
    machine_clk = 0;

    if (adc_data_ready) {
        read_sensor_data();
        adc_data_ready = 0;
    }

    switch (state_machine)
    {
        case STATE_INITIALIZING:
            set_state_running();
            VERBOSE_MSG_MACHINE(usart_send_string("Estado: EXECUTANDO\n"));
            break;
        case STATE_RUNNING:
            #ifdef CAN_ON
                can_app_task();
            #endif
            #ifdef LED_ON
                static uint16_t led_counter = 0;
                if(++led_counter >= (MACHINE_FREQUENCY / 2) ) {
                    cpl_bit(LED_PORT, LED1);
                    led_counter = 0;
                }
            #endif
            break;
        default:
            set_state_initializing();
            break;
    }
}

ISR(TIMER2_COMPA_vect)
{
    machine_clk = 1;
}
