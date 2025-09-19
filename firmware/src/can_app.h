#ifndef CAN_APP_H
#define CAN_APP_H

#include "dbg_vrb.h" // Adicionado para corrigir o erro de macro

void can_app_task(void);
void can_app_send_state(void);
void can_app_send_measurements(void);

#endif // CAN_APP_H
