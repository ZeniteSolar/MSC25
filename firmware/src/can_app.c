#include "can_app.h"
#include "conf.h"
#include "can_ids.h"
#include "machine.h"
#include "can.h"
#include <string.h> 

void can_app_task(void) {
    static uint16_t state_counter = 0;
    if (++state_counter >= (MACHINE_FREQUENCY / CAN_APP_SEND_STATE_FREQ_HZ)) {
        state_counter = 0;
        can_app_send_state();
    }

    static uint16_t measurement_counter = 0;
    if (++measurement_counter >= (MACHINE_FREQUENCY / CAN_APP_SEND_MEASUREMENT_FREQ_HZ)) {
        measurement_counter = 0;
        can_app_send_measurements();
    }
}

void can_app_send_state(void) {
    can_t msg;
    memset(&msg, 0, sizeof(can_t)); 

    msg.id = (uint32_t)CAN_MSG_MSC19_5_STATE_ID;
    msg.length = 3; 
    
    msg.data[0] = CAN_SIGNATURE_SELF; 
    msg.data[1] = (uint8_t)state_machine;
    msg.data[2] = 0; 
    
    can_send_message(&msg);
}

void can_app_send_measurements(void) {
    can_t msg;
    memset(&msg, 0, sizeof(can_t)); 

    msg.id = (uint32_t)CAN_MSG_MSC19_5_ADC_ID; 
    msg.length = 7; 
    
    uint16_t val = measurements.current_value;

    msg.data[0] = CAN_SIGNATURE_SELF; 
    msg.data[1] = (uint8_t)(val & 0xFF); 
    msg.data[2] = (uint8_t)(val >> 8);   
    msg.data[3] = msg.data[1];           
    msg.data[4] = msg.data[2]; 
    msg.data[5] = msg.data[1];           
    msg.data[6] = msg.data[2];

    can_send_message(&msg);
}