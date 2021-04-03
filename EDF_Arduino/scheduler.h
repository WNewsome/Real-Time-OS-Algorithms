#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

int find_available_index();
void vSchedulerPeriodicTaskCreate( TaskFunction_t pvTaskCode, const char *pcName, UBaseType_t uxStackDepth, void *pvParameters, UBaseType_t uxPriority,
		TaskHandle_t *pxCreatedTask, TickType_t xPhaseTick, TickType_t xPeriodTick, TickType_t xMaxExecTimeTick, TickType_t xDeadlineTick );//void set_priorities();
TickType_t next_arrival_time();
int get_available_high(TickType_t ticks);
void execute(TickType_t ticks);
void set_priorities();

void vSchedulerStart();