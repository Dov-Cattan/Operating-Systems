// Dov_Cattan_Scheduler.h: Declaration Of Everything Used In The Simulation
#include <list>
#include <queue>
#include <string>

using namespace std;

struct process
{
	process() {};
	process(int U_ID, string P_ID, list<int> CPU, list<int> IO) : unique_ID(U_ID), process_Name(P_ID), CPU_Times(CPU), IO_Times(IO) {};
	string process_Name = "";
	int unique_ID = 0;
	string state = "new";
	int arrival_Time = 0;
	int initial_Arrival_Time = arrival_Time;
	int waiting_Time = 0;
	int turnaround_Time = 0;
	int response_Time = 0;
	int priority = 0;
	list<int> CPU_Times;
	list<int> IO_Times;
};

class scheduler
{
public:
	scheduler(string filepath);
	void simulate(string user_Input);
	void update_Job_Queue(process);
private:
	int global_Clock = 0;
	list<process> job_Queue;
	string input = "";
	int time_Quantum = INT_MAX;


	void display_ContextSwitch(list<process> ready_Queue, list<process> IO_Queue, process temp);
	void display_Simulation_End();
	void find_Average_Times();
	void restore_Processes(list<process>& IO_Queue, list<process>& ready_Queue);
};

bool CPU_Scheduling(process LHS, process RHS);
bool arrival_Scheduling(process LHS, process RHS);
bool priority_Scheduling(process LHS, process RHS);
