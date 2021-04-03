#include "scheduler.h"
#define MAX_TASKS 5
int tasks_count = 0;
task TASKS_GLOBAL[MAX_TASKS]; // Where the tasks will be stored

bool operator<(task a, task b)
{
	// Overload operator to sort by deadlines
	if (a.deadline < b.deadline) return true;
	if (a.deadline > b.deadline) return false;
	return false;
}

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
		task->pvTaskCode = pvTaskCode;
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

		BaseType_t xReturnValue = xTaskCreate(pvTaskCode,
					name,
					uxStackDepth,
					pvParameters,
					uxPriority,
					pxCreatedTask);
			if(xReturnValue == pdPASS) {
				Serial.print(name);
				Serial.print(" task created");
				Serial.println();
				Serial.flush();
			}
			else
			{
				Serial.println("Task creation failed\n");
				Serial.flush();
			}
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
	int n = sizeof(TASKS_GLOBAL) / sizeof(TASKS_GLOBAL[0]);
	int i, j, min;
	task temp;
	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i + 1; j < n; j++)
			if (TASKS_GLOBAL[j] < TASKS_GLOBAL[min])
				min = j;
		temp = TASKS_GLOBAL[i];
		TASKS_GLOBAL[i] = TASKS_GLOBAL[min];
		TASKS_GLOBAL[min] = temp;
	}
    
    /*
    T1  T2
    2   1
    */
    //sort(TASKS_GLOBAL, TASKS_GLOBAL + tasks_count); // no #include <algorithm>
	for (int i = 0; i < MAX_TASKS; i++) {
		TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
		if(TASKS_GLOBAL[i].set==true){
			Serial.print(TASKS_GLOBAL[i].name);
			Serial.print(" Priority ");
			Serial.print(TASKS_GLOBAL[i].priotiry);
			Serial.println();
			Serial.flush();
		}
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
	
	Serial.flush();
	set_priorities();
	//Serial.println("vSchedulerStart");
	vTaskStartScheduler();
}

void vApplicationTickHook( void )
{
	TickType_t ticks = xTaskGetTickCount();
	
	//execute(ticks);
}; 