#ifndef DBG_VRB_H
#define DBG_VRB_H

#include "conf.h"

#ifdef VERBOSE_ON
    #define VERBOSE_MSG(x) x
#else
    #define VERBOSE_MSG(x)
#endif

#ifdef VERBOSE_ON_ERROR
    #define VERBOSE_MSG_ERROR(x) x
#else
    #define VERBOSE_MSG_ERROR(x)
#endif

#ifdef VERBOSE_ON_CAN_APP
    #define VERBOSE_MSG_CAN_APP(x) x
#else
    #define VERBOSE_MSG_CAN_APP(x)
#endif

#ifdef VERBOSE_ON_ADC
    #define VERBOSE_MSG_ADC(x) x
#else
    #define VERBOSE_MSG_ADC(x)
#endif

#ifdef VERBOSE_ON_INIT
    #define VERBOSE_MSG_INIT(x) x
#else
    #define VERBOSE_MSG_INIT(x)
#endif

#ifdef VERBOSE_ON_MACHINE
    #define VERBOSE_MSG_MACHINE(x) x
#else
    #define VERBOSE_MSG_MACHINE(x)
#endif

#endif /* DBG_VRB_H */
