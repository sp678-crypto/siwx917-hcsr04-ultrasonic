// hcsr04.h
#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>
#include "sl_status.h"

sl_status_t hcsr04_init(void);
int32_t hcsr04_measure_cm(void);

#endif
