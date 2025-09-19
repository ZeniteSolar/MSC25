#include "can_app.h"
#include "conf.h"
#include "can_ids.h"
#include "machine.h"
#include "usart.h"
#include "can.h"

void can_app_task(void)
{
    // Contador para envio da mensagem de estado
    static uint16_t state_counter = 0;
    if (++state_counter >= (MACHINE_FREQUENCY / CAN_APP_SEND_STATE_FREQ_HZ)) {
        state_counter = 0;
        can_app_send_state();
    }

    // Contador para envio da mensagem de medição
    static uint16_t measurement_counter = 0;
    if (++measurement_counter >= (MACHINE_FREQUENCY / CAN_APP_SEND_MEASUREMENT_FREQ_HZ)) {
        measurement_counter = 0;
        can_app_send_measurements();
    }

    // Lógica para checar mensagens recebidas (se necessário)
    if (can_check_message()) {
        can_t msg;
        if (can_get_message(&msg)) {
            // Processar mensagem recebida aqui
        }
    }
}

void can_app_send_state(void)
{
    can_t msg;
    msg.id = CAN_MSG_MSC25_1_STATE_ID;
    msg.length = CAN_MSG_MSC25_1_STATE_LENGTH;
    msg.flags.rtr = 0;
    #if SUPPORT_EXTENDED_CANID
    msg.flags.extended = 0;
    #endif

    msg.data[CAN_MSG_MSC25_1_STATE_SIGNATURE_BYTE] = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MSC25_1_STATE_STATE_BYTE] = (uint8_t)state_machine;
    msg.data[CAN_MSG_MSC25_1_STATE_ERROR_BYTE] = 0; // Sem lógica de erro ainda

    if (can_send_message(&msg)) {
        VERBOSE_MSG_CAN_APP(usart_send_string("CAN State Enviado.\n"));
    } else {
        VERBOSE_MSG_CAN_APP(usart_send_string("Falha ao enviar CAN State.\n"));
    }
}

void can_app_send_measurements(void)
{
    can_t msg;
    msg.id = CAN_MSG_MSC25_1_MEASUREMENTS_ID;
    msg.length = CAN_MSG_MSC25_1_MEASUREMENTS_LENGTH;
    msg.flags.rtr = 0;
    #if SUPPORT_EXTENDED_CANID
    msg.flags.extended = 0;
    #endif

    uint16_t current = measurements.current_value;

    msg.data[CAN_MSG_MSC25_1_MEASUREMENTS_SIGNATURE_BYTE] = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MSC25_1_MEASUREMENTS_CURRENT_L_BYTE] = (uint8_t)(current & 0xFF);
    msg.data[CAN_MSG_MSC25_1_MEASUREMENTS_CURRENT_H_BYTE] = (uint8_t)(current >> 8);

    if (can_send_message(&msg)) {
        VERBOSE_MSG_CAN_APP(usart_send_string("CAN Medidas Enviado: "));
        VERBOSE_MSG_CAN_APP(usart_send_uint16(current));
        VERBOSE_MSG_CAN_APP(usart_send_string("\n"));
    } else {
        VERBOSE_MSG_CAN_APP(usart_send_string("Falha ao enviar CAN Medidas.\n"));
    }
}
