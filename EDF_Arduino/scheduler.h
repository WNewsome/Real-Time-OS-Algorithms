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
	int times_executed;
	TickType_t rel_deadline;
};