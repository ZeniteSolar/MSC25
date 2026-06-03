#include "main.h"

void init(void)
{
    // 1. Inicializa Serial
    #ifdef USART_ON
        usart_init();
        usart_send_string("\n\n--- MSC25_1 Firmware ---\n");
    #endif

    // 2. Teste rápido do LED na inicialização
    // Isso confirma visualmente que o pino PC5 está correto
    #ifdef LED_ON
        set_bit(LED_DDR, LED1); // Configura PC5 como saída
        
        // Pisca 3 vezes rápido
        for(int i=0; i<3; i++) {
            set_bit(LED_PORT, LED1); 
            for(volatile long j=0; j<100000; j++); // Delay simples
            clr_bit(LED_PORT, LED1);
            for(volatile long j=0; j<100000; j++);
        }
        VERBOSE_MSG_INIT(usart_send_string("LED Init... OK\n"));
    #endif

    #ifdef WATCHDOG_ON
        wdt_enable(WDTO_2S);
    #endif
    wdt_reset();

    #ifdef SPI_ON
        DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);
        PORTB |= (1 << PB2);
        VERBOSE_MSG_INIT(usart_send_string("SPI... OK\n"));
    #endif

    #ifdef CAN_ON
        VERBOSE_MSG_INIT(usart_send_string("CAN... "));
        if (!can_init(BITRATE_500_KBPS)) {
            VERBOSE_MSG_INIT(usart_send_string("FALHA (Sem ACK)\n"));
        } else {
            VERBOSE_MSG_INIT(usart_send_string("OK\n"));
        }
    #endif

    #ifdef ADC_ON
        adc_init();
        VERBOSE_MSG_INIT(usart_send_string("ADC... OK\n"));
    #endif
    
 	#ifdef MACHINE_ON
		machine_init();
	#endif

    sei(); // Habilita interrupções
    usart_send_string("Sistema Iniciado.\n");
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