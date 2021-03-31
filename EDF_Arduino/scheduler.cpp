#include "scheduler.h"
#define MAX_TASKS 5
int tasks_count = 0;
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
task TASKS_GLOBAL[MAX_TASKS]; // Where the tasks will be stored

int find_available_index() {
	for (int i = 0; i < MAX_TASKS; i++) {
		if (TASKS_GLOBAL[i].set == false)
			return i;
	}
	return -1; // Global array of tasks is full
}

void vSchedulerPeriodicTaskCreate(TaskFunction_t pvTaskCode, const char *name, UBaseType_t uxStackDepth, void *pvParameters, UBaseType_t uxPriority,
		TaskHandle_t *pxCreatedTask, TickType_t xPhaseTick, TickType_t period, TickType_t capacity, TickType_t deadline ) {
	int index = find_available_index();
	if (index != -1) {
        task * task = &TASKS_GLOBAL[index];
        task->name = name;
        task->relative_period = period;
        task->deadline = deadline;
        task->capacity = capacity;
        task->priotiry = 0;
        task->set = true;
        task->completed = false;
        task->execution_time = 0;
        task->period = period;
        task->times_executed = 0;
        task->rel_deadline = deadline;
        
		tasks_count++;
	}
}

// Calculate the next arrival time
TickType_t next_arrival_time() {
	TickType_t temp = TASKS_GLOBAL[0].period;

	for (int i = 0; i < tasks_count; i++) {
		if (TASKS_GLOBAL[i].period < temp ) {

			temp = TASKS_GLOBAL[i].period;
		}
	}
	return temp;
}

// Retreive highest priority available task index
int get_available_high(TickType_t ticks) {
	task * task;

	for (int i = 0; i < tasks_count; i++) {
		// highest is 0
		task = &TASKS_GLOBAL[i];
		if (task->period == ticks) {
			task->completed = false;
			// I believe the issue is here
			task->deadline = task->period + task->rel_deadline;
		}
	}
	for (int i = 0; i < tasks_count; i++) {
		task = &TASKS_GLOBAL[i];
		if (task->completed == false) {
			return i;
		}
	}

	return MAX_TASKS; // Return idle task
}


void set_priorities() {
    // Need to sort TASKS_GLOBAL array according to deadlines
	// Highest priority = shortest deadline
	// Highest priority = index 0 in TASKS_GLOBAL
    TickType_t temp = TASKS_GLOBAL[0].deadline;

	for (int i = 0; i < MAX_TASKS; i++) {
		if (temp > TASKS_GLOBAL[i].deadline){
            TASKS_GLOBAL[0] = TASKS_GLOBAL[i];
        }
	}
    
    /*
    T1  T2
    2   1
    */
    //sort(TASKS_GLOBAL, TASKS_GLOBAL + tasks_count); // no #include <algorithm>
	for (int i = 0; i < MAX_TASKS; i++) {
		TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
	}
}

void execute(TickType_t ticks) {
	set_priorities();
	task *run_task = &TASKS_GLOBAL[get_available_high(ticks)];
	set_priorities();
	run_task = &TASKS_GLOBAL[get_available_high(ticks)];

	run_task->execution_time++;

    //cout << run_task->name << " - ";
    Serial.println(run_task->name);
    Serial.flush();
	if (run_task->execution_time >= run_task->capacity) {
		// Job finished executing
		run_task->completed = true;
		run_task->execution_time = 0; // re-set
		run_task->times_executed++;
		run_task->period = run_task->relative_period*run_task->times_executed;
		run_task->deadline = run_task->period + run_task->rel_deadline;
	}
}

void vSchedulerStart() {
	set_priorities();
	TickType_t ticks = xTaskGetTickCount();
	// Let computer run for 20 ticks
	for (;;) {
        //Serial.println(ticks);
       // Serial.flush();
		execute(ticks);
		ticks++;
	}
}