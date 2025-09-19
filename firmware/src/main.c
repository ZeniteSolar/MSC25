#include "main.h"

void init(void)
{
    #ifdef USART_ON
        usart_init();
        usart_send_string("\n\n--- MSC25_1 Sensor de Corrente ---\n");
        usart_send_string("Inicializando...\n");
    #endif

    #ifdef WATCHDOG_ON
        wdt_enable(WDTO_2S); // Watchdog de 2 segundos
        VERBOSE_MSG_INIT(usart_send_string("WATCHDOG... OK!\n"));
    #endif

    wdt_reset();

    #ifdef LED_ON
        set_bit(LED_DDR, LED1);
        VERBOSE_MSG_INIT(usart_send_string("LED... OK!\n"));
    #endif

    #ifdef SPI_ON
        // Configuração dos pinos SPI (CS como saída)
        DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);
        PORTB |= (1 << PB2); // CS em high
        VERBOSE_MSG_INIT(usart_send_string("SPI... OK!\n"));
    #endif

    wdt_reset();

    #ifdef CAN_ON
        VERBOSE_MSG_INIT(usart_send_string("CAN (500kbps)..."));
        if (!can_init(BITRATE_500_KBPS)) {
            VERBOSE_MSG_INIT(usart_send_string(" FALHA!\n"));
        } else {
            VERBOSE_MSG_INIT(usart_send_string(" OK!\n"));
        }
    #endif

    wdt_reset();

    #ifdef ADC_ON
        VERBOSE_MSG_INIT(usart_send_string("ADC..."));
        adc_init();
        VERBOSE_MSG_INIT(usart_send_string(" OK!\n"));
    #endif
    
    wdt_reset();

 	#ifdef MACHINE_ON
        VERBOSE_MSG_INIT(usart_send_string("Maquina de Estados..."));
		machine_init();
        VERBOSE_MSG_INIT(usart_send_string(" OK!\n"));
	#endif

    wdt_reset();

    // Habilita interrupções globais
    sei();
    usart_send_string("Inicializacao completa. Executando...\n");
}

int main(void)
{
    init();

	for(;;)
    {
		#ifdef WATCHDOG_ON
            wdt_reset();
		#endif

        #ifdef MACHINE_ON
            machine_run();
        #endif
	}
}
