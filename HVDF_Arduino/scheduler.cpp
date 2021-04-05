#include "scheduler.h"
#define MAX_TASKS 5

#if( schedUSE_TCB_ARRAY == 1 )
	static task TASKS_GLOBAL[MAX_TASKS]; // Where the tasks will be stored
	static TickType_t ticks = 0;
	const char *temp_task_name;
#endif 

int tasks_count = 0;
bool operator<(task a, task b)
{
	
	// Use for HVDF
	float A_value = a.value/a.capacity;
	float B_value = b.value/b.capacity;

	if (A_value > B_value) return true;
	if (B_value < A_value) return false;
	
	return false;
	// Use for EDF ONLY
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
		TaskHandle_t *pxCreatedTask, TickType_t xPhaseTick, TickType_t period, TickType_t capacity, TickType_t deadline, int Value ) {
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
		task->value;
		tasks_count++;

		Serial.print(name);
		Serial.print(" task created");
		Serial.println();
		Serial.flush();
		/*
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
		*/
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
	// Highest priority = Highest density Value
	// Highest priority = index 0 in TASKS_GLOBAL
	int n = sizeof(TASKS_GLOBAL) / sizeof(TASKS_GLOBAL[0]);
	int i, j, min;
	task temp;
	// Selection Sort Algorithm
	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i + 1; j < n; j++)
			if (TASKS_GLOBAL[j] < TASKS_GLOBAL[min] && TASKS_GLOBAL[j].set==true)
				min = j;
		temp = TASKS_GLOBAL[i];
		TASKS_GLOBAL[i] = TASKS_GLOBAL[min];
		TASKS_GLOBAL[min] = temp;
	}
	for (int i = 0; i < MAX_TASKS; i++) {
		//TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
		if(TASKS_GLOBAL[i].set==true){
			TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
		}
	}
}

static void execute() {

	//Serial.println(ticks);
	//Serial.flush();

	for (int i = 0; i < tasks_count; i++) {
		if(TASKS_GLOBAL[i].set){
			//Serial.print(TASKS_GLOBAL[i].name);
			//Serial.print(" ");
			//Serial.print(TASKS_GLOBAL[i].priotiry);
			//Serial.println();
		}
		
	}
	//TickType_t ticks_ = xTaskGetTickCount();
	
	//Serial.println(ticks);
	set_priorities();
	task *run_task_p = &TASKS_GLOBAL[get_available_high(ticks)];

	set_priorities();
	task * run_task = &TASKS_GLOBAL[get_available_high(ticks)];

	run_task->execution_time++;
	
    //cout << run_task->name << " - ";
	if(temp_task_name!=run_task->name && run_task->set){
		if(run_task_p->name != run_task->name){
			Serial.print(run_task_p->name);
			Serial.println(" missed deadline");
			Serial.println();
		}
		Serial.print("Executing: ");
		Serial.print(run_task->name);
		//Serial.print("here");
		Serial.println();
		Serial.flush();
		temp_task_name=run_task->name;
	}
	
	//vTaskDelay(1000 / portTICK_PERIOD_MS );
	if (run_task->execution_time >= run_task->capacity) {
		// Job finished executing
		//Serial.println("running");
		
		run_task->completed = true;
		
		run_task->execution_time = 0; // re-set
		
		run_task->times_executed++;
		if(run_task->name == "t1" || run_task->name == "t2" || run_task->name == "t3" || run_task->name == "t4"){
			run_task->period = run_task->relative_period*run_task->times_executed;
			//Serial.println(run_task->period);
			run_task->deadline = run_task->period + run_task->rel_deadline;
		}
		
	}
	
	ticks++;
	//return;
}

void vSchedulerStart() {
	//Serial.flush();
	set_priorities();
	//Serial.println("vSchedulerStart");
	for(;;){
		execute();
    }
	vTaskStartScheduler();
}

void vApplicationTickHook( void )
{

}; 

void create_execute(){
	// Create the execute task
	BaseType_t xReturnValue = xTaskCreate(execute,
					"execute"
					    ,  128
    					,  (void*)&ticks
    					,  5
    					,  NULL );

}

static void prvInitTCBArray( void ){
	// Initialize TASKS_GLOBAL array
	#if( schedUSE_TCB_ARRAY == 1 )
	for (int i = 0; i<MAX_TASKS;i++){
		TASKS_GLOBAL[i].set=false;
		TASKS_GLOBAL[i].name = "idle";
		TASKS_GLOBAL[i].pxCreatedTask = NULL;
	}
	#endif /* schedUSE_TCB_ARRAY */
}

void vSchedulerInit( void )
{
	prvInitTCBArray();
}
