#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

typedef enum state_machine {
    STATE_INITIALIZING,
    STATE_RUNNING,
    STATE_ERROR
} state_machine_t;

typedef struct measurements {
    uint16_t current_value;
} measurements_t;

extern volatile state_machine_t state_machine;
extern volatile measurements_t measurements;

void machine_init(void);
void machine_run(void);

#endif // MACHINE_H
