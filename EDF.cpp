/* EDF Algorithm
- No schedulability test required
- If a task misses its deadline it gets dropped
*/
#include <iostream> // Not available for Arduino
#include <string> // Not available for Arduino
#include <algorithm> // Not available for Arduino
using namespace std;

#define MAX_TASKS 5
int tasks_count = 0;

struct task {
	/* Given by user */
	string name;		// Name assigned to task
	int relative_period;// T: Period of the task
	int deadline;		// D: Deadline
	int capacity;		// C: Time of execution of the task

	/* Set by algorithm*/
	int priotiry;		// P: priority
	bool set;			// true when the task is set
	bool completed;		// true if task completed in its frame
	int execution_time;	// Current execution time of a job
	int frame;			// current frame number of a job
	int period;			// Changing period
	int times_executed;
	int rel_deadline;
};

bool operator<(task a, task b)
{
	// Overload operator to sort by deadlines
	if (a.deadline < b.deadline) return true;
	if (a.deadline > b.deadline) return false;
	return false;
}

task TASKS_GLOBAL[MAX_TASKS]; // Where the tasks will be stored

int find_available_index() {
	for (int i = 0; i < MAX_TASKS; i++) {
		if (TASKS_GLOBAL[i].set == false)
			return i;
	}
	return -1; // Global array of tasks is full
}

void create_task(string name, int period, int deadline, int capacity) {
	int index = find_available_index();
	if (index != -1) {
		TASKS_GLOBAL[index] = { name, period, deadline, capacity, 
			0, true, false, 0, 1, period, 0, deadline};
		tasks_count++;
	}
}

void set_priorities() {
	sort(TASKS_GLOBAL, TASKS_GLOBAL + tasks_count);
	for (int i = 0; i < MAX_TASKS; i++) {
		TASKS_GLOBAL[i].priotiry = MAX_TASKS - i;
	}
}

// Calculate the next arrival time
int next_arrival_time() {
	int temp = TASKS_GLOBAL[0].period;

	for (int i = 0; i < tasks_count; i++) {
		if (TASKS_GLOBAL[i].period < temp ) {

			temp = TASKS_GLOBAL[i].period;
		}
	}
	return temp;
}

// Retreive highest priority available task index
int get_available_high(int ticks) {
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

void execute(int ticks) {
	set_priorities();
	task *run_task = &TASKS_GLOBAL[get_available_high(ticks)];
	set_priorities();
	run_task = &TASKS_GLOBAL[get_available_high(ticks)];
	run_task->execution_time++;
	cout << run_task->name << " - ";
	if (run_task->execution_time >= run_task->capacity) {
		// Job finished executing
		run_task->completed = true;
		run_task->execution_time = 0; // re-set
		run_task->frame++;
		run_task->times_executed++;
		run_task->period = run_task->relative_period*run_task->times_executed;
		run_task->deadline = run_task->period + run_task->rel_deadline;
	}
}

void start_tasks() {
	set_priorities();
	int ticks = 0;
	// Let computer run for 20 seconds
	for (int i = 0; i < 20; i++) {
		if (ticks == 17) {
			int x = 1;
		}
		execute(ticks);
		ticks++;
	}
}

int main()
{
	//			name,  T,  D,  C

	// Test 1 https://www.youtube.com/watch?v=ejPXTOcMRPA
	create_task("t1", 20, 7, 3);
	create_task("t2", 5, 4, 2);
	create_task("t3", 10, 8, 2);

	// Test 2: from HW 3
	/*
	create_task("t1", 10, 10, 4);
	create_task("t2", 17, 17, 12);
	create_task("t3", 5, 5, 3);
	*/

	
	start_tasks();
}