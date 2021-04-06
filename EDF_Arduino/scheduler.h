#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <projdefs.h>
#include <timers.h>
#include <list.h>
#include <croutine.h>
#include <portable.h>
#include <stack_macros.h>
#include <mpu_wrappers.h>
#include <FreeRTOSVariant.h>
#include <message_buffer.h>
#include <semphr.h>
#include <FreeRTOSConfig.h>
#include <stream_buffer.h>
#include <portmacro.h>
#include <event_groups.h>
#include <queue.h>
#include <Arduino.h>
#ifdef __cplusplus
extern "C" {
#endif
#define schedUSE_TCB_ARRAY 1
int find_available_index();
void vSchedulerPeriodicTaskCreate( TaskFunction_t pvTaskCode, const char *pcName, UBaseType_t uxStackDepth, void *pvParameters, UBaseType_t uxPriority,
		TaskHandle_t *pxCreatedTask, TickType_t xPhaseTick, TickType_t xPeriodTick, TickType_t xMaxExecTimeTick, TickType_t xDeadlineTick );//void set_priorities();
TickType_t next_arrival_time();
int get_available_high(TickType_t ticks);

void set_priorities();
void vSchedulerStart();
void create_execute();

struct task {
    TaskFunction_t pvTaskCode;
    const char *name;
    UBaseType_t uxStackDepth;
    void *pvParameters;
    UBaseType_t priotiry;
    TaskHandle_t *pxCreatedTask;
    TickType_t xPhaseTick;          // Phase given in software ticks. Counted from when vSchedulerStart is called.
    TickType_t relative_period;
    TickType_t capacity;    // C
    TickType_t deadline;
    // --------------------------------------->


	/* Given by user */
	//const char * name;		// Name assigned to task
	//int relative_period;// T: Period of the task
	//int deadline;		// D: Deadline
	//int capacity;		// C: Time of execution of the task

	/* Set by algorithm*/
	//int priotiry;		// P: priority
	bool set;			// true when the task is set
	bool completed;		// true if task completed in its frame
	TickType_t execution_time;	// Current execution time of a job
	TickType_t period;			// Changing period
	TickType_t times_executed;
	TickType_t rel_deadline;
};
void vSchedulerInit( void );
//static void prvInitTCBArray( void );
#ifdef __cplusplus
}
#endif
#endif /* SCHEDULER_H_ */