#include <iostream> // Not available for Arduino
#include <string> // Not available for Arduino
#include <algorithm> // Not available for Arduino
using namespace std;

#define MAX_TASKS 5
int tasks_count = 0;

struct RM_task {
	/* Given by user */
	string name;	// Name assigned to task
	int period;			// T: Period of the task
	int deadline;		// D: Relative deadline
	int capacity;		// C: Time of execution of the task
	
	/* Set by algorithm*/
	int priotiry;		// P: priority
	bool set;			// true when the task is set
	int execution_time;	// counting of execution time (resets when done)
	bool completed;
	int times_executed;
	int absolute_period;
};

bool operator<(RM_task a, RM_task b)
{
    if (a.period < b.period) return true;
    if (a.period > b.period) return false;
    return false;
}

RM_task TASKS_GLOBAL[MAX_TASKS]; // Where the tasks will be stored

int find_available_index() {
	for (int i = 0; i < MAX_TASKS; i++) {
		if (TASKS_GLOBAL[i].set == false)
			return i;
	}
	return -1; // Global array of tasks is full
}

void create_task(string name, int period, int deadline, int capacity) {
	int index = find_available_index();
	if(index != -1){
		TASKS_GLOBAL[index] = { name, period, deadline, capacity, 0, true, 0, false, 0, period};
		tasks_count++;
	}
}

void set_priorities() {
	sort(TASKS_GLOBAL, TASKS_GLOBAL + tasks_count);
	for (int i = 0; i < MAX_TASKS; i++) {
		TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
	}
}

int get_available_high(int ticks) {
	RM_task * task;

	for (int i = 0; i < tasks_count; i++) {
		// highest is 0
		task = &TASKS_GLOBAL[i];
		if (task->period == ticks) {
			task->completed = false;
		}
	}
	for (int i = 0; i < tasks_count; i++) {
		if (TASKS_GLOBAL[i].completed == false) {
			return i;
		}
	}
	
	return MAX_TASKS; // Return idle task
}
void execute_highest(int ticks) {
	// Here is where we actually check what to execute
	RM_task *run_task = &TASKS_GLOBAL[get_available_high(ticks)];
	run_task->execution_time++;
	cout << run_task->name << " - ";
	if (run_task->execution_time >= run_task->capacity) {
		run_task->completed = true;
		run_task->execution_time = 0;
		run_task->times_executed++;
		run_task->period = run_task->absolute_period*run_task->times_executed;
	}
}

void execute() {
	int ticks = 0; // computer time

	for (int i=0; i<50; i++) {
		execute_highest(ticks);
		ticks++;
	}
}
void start_tasks() {
	set_priorities();
	execute();
}

int main()
{
	//			name,  T,  D,  C
	create_task("t1", 8, 100, 1);
	create_task("t2", 15, 100, 3);
	create_task("t3", 20, 100, 4);
	create_task("t4", 22, 100, 6);
	start_tasks();
}
