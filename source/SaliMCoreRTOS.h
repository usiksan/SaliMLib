#ifndef SALIMCORERTOS_H
#define SALIMCORERTOS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define smTickCount  (static_cast<int>(xTaskGetTickCount()) * (1000 / configTICK_RATE_HZ))

#include "SaliMCore.h"

#endif // SALIMCORERTOS_H
