// Dov_Cattan_Scheduler.cpp : Implements the Simulation declared in the header.

#include "Dov_Cattan_Scheduler.h"
#include <iostream> // For cin/cout
#include <string>   // For operations on strings
#include <fstream>  // For file i/o
#include <sstream>	// For stringstream object, to make list of burst times.
#include <algorithm>// For stable_sort, for SJF
#include <iomanip>	// For setw

using namespace std;

bool CPU_Scheduling(process LHS, process RHS)
{// Return true if lhs is less than the rhs, otherwise return false.
    return (LHS.CPU_Times.front() < RHS.CPU_Times.front());
}

bool arrival_Scheduling(process LHS, process RHS)
{// Return true if lhs is less than the rhs, otherwise return false.
    return (LHS.arrival_Time < RHS.arrival_Time);
}

bool priority_Scheduling(process LHS, process RHS)
{// Return true if lhs is less than the rhs, otherwise return false.
    return (LHS.priority < RHS.priority);
}

scheduler::scheduler(string filepath = "Dov_Cattan_Processes.txt")
{
    ifstream f_Input(filepath);
    if (f_Input.fail())
    {
        cout << "File opening failed in constructor!" << endl;
        exit(1);
    }

    string input;
    stringstream input2;
    process temp;
    int U_ID = 0;
    while (!f_Input.eof())
    {
        temp = process();
        getline(f_Input, input, ' ');
        temp.process_Name = input;
        temp.unique_ID = U_ID++;

        getline(f_Input, input);
        input = input.substr(1, input.length() - 2);

        int is_IO = 0;
        input2 = stringstream(input);
        while (getline(input2, input, ','))
        {
            if (is_IO % 2)
                temp.IO_Times.push_back(stoi(input));
            else temp.CPU_Times.push_back(stoi(input));
            is_IO++;
        }

        job_Queue.push_back(temp);
    }
    f_Input.close();
}

void scheduler::simulate(string user_Input) {
    input = user_Input;
    transform(input.begin(), input.end(), input.begin(), ::tolower); // transforming each line to lowercase
    global_Clock = 0; // initialize global clock to zero
    list<process> ready_Queue; // initialize ready queue
    list<process> IO_Queue; // initialize io queue
    for (process i : job_Queue)
    {// Keeps the process id, process name, cpu times and io times then pushes it to the ready queue.
        i = process(i.unique_ID, i.process_Name, i.CPU_Times, i.IO_Times);
        ready_Queue.push_back(i);
    }
    if (input == "sjf") // Sort ready queue by ascending CPU time if the input is equal to SJF.
        ready_Queue.sort(CPU_Scheduling);
    else if (input == "mlfq") // Otherwise sort the ready queue by priority if the input is equal to MLFQ.
        ready_Queue.sort(priority_Scheduling);

    process currently_Running_Process;
    while (!IO_Queue.empty() || !ready_Queue.empty())
    {// Continue running while there are processes in the ready or the I/O queue.
        while (!ready_Queue.empty())
        {// Continue running CPU as long as processes in ready queue.
            currently_Running_Process = ready_Queue.front();
            ready_Queue.pop_front();

            if (currently_Running_Process.state == "new")
            {// sets the response time if there is none.
                currently_Running_Process.response_Time = global_Clock - currently_Running_Process.arrival_Time;
                currently_Running_Process.state = "ready";
            }

            // Set or update waiting time of current process.
            currently_Running_Process.waiting_Time += global_Clock - currently_Running_Process.arrival_Time;

            // Set time time_quantum, if priority is 0 or 1. 
            if (input == "mlfq")
            {
                switch (currently_Running_Process.priority)
                {
                case 0: // Round Robin with time_quantum 5.
                    time_Quantum = 5;
                    break;
                case 1: // Round Robin with time_quantum 10.
                    time_Quantum = 10;
                    break;
                default:// FCFS, could use if() statements to handle different datatypes.
                    time_Quantum = INT_MAX;
                    break;
                }
            }

            // Increment the global clock, adjust process state.
            if (currently_Running_Process.CPU_Times.front() > time_Quantum)
            {
                global_Clock += time_Quantum;
                currently_Running_Process.CPU_Times.front() -= time_Quantum;
                currently_Running_Process.state = "ready";
                currently_Running_Process.priority += 1;
            }
            else // CPU burst <= time_quantum
            {
                global_Clock += currently_Running_Process.CPU_Times.front();
                currently_Running_Process.CPU_Times.pop_front();
                currently_Running_Process.state = "waiting";
            }

            // If I/O burst of a process is complete, remove from I/O queue and add to ready queue.
            IO_Queue.sort(arrival_Scheduling); // Schedule processes.
            restore_Processes(IO_Queue, ready_Queue);

            // Check if currently running process is complete.
            if (currently_Running_Process.CPU_Times.empty())
                currently_Running_Process.state = "terminated";

            // Set new arrival/turnaround time, and add to appropriate queue.
            if (currently_Running_Process.state == "ready")
            {
                currently_Running_Process.arrival_Time = global_Clock;
                ready_Queue.push_back(currently_Running_Process);

                if (input == "sjf") // Sort ready queue by ascending CPU time if SJF.
                    ready_Queue.sort(CPU_Scheduling);
                else if (input == "mlfq") // Sort ready queue by priority if MLFQ.
                    ready_Queue.sort(priority_Scheduling);
            }
            else if (currently_Running_Process.state == "waiting")
            {
                currently_Running_Process.arrival_Time = global_Clock + currently_Running_Process.IO_Times.front();
                currently_Running_Process.IO_Times.pop_front();
                IO_Queue.push_back(currently_Running_Process);
                IO_Queue.sort(arrival_Scheduling); // Schedule processes.
            }
            else if (currently_Running_Process.state == "terminated")
            {
                currently_Running_Process.turnaround_Time = global_Clock - currently_Running_Process.initial_Arrival_Time;
                update_Job_Queue(currently_Running_Process);
            }

            display_ContextSwitch(ready_Queue, IO_Queue, currently_Running_Process);
        }
        // While there are processes in IO queue, increment global clock.
        if (!IO_Queue.empty())
            global_Clock += 1;

        // Check the arrival time of each process, and restore to ready queue when I/O is complete.

        restore_Processes(IO_Queue, ready_Queue);
    }

    display_Simulation_End();

    return;
}

void scheduler::update_Job_Queue(process completed_Process)
{// Updates job queue with the values from a completed process.
    for (list<process>::iterator process_Iterator = job_Queue.begin(); process_Iterator != job_Queue.end(); ++process_Iterator)
    {// Iterates through job_queue to find process that matches input process.
        if ((*process_Iterator).unique_ID == completed_Process.unique_ID)
        {// Transfers the appropriate values from the completed process, to the job queue process.
            (*process_Iterator).response_Time = completed_Process.response_Time;
            (*process_Iterator).turnaround_Time = completed_Process.turnaround_Time;
            (*process_Iterator).waiting_Time = completed_Process.waiting_Time;
        }
    }
}

void scheduler::restore_Processes(list<process>& IO_Queue, list<process>& ready_Queue)
{// Schedule processes, and restore processes from sorted IO queue to ready queue, only if I/O burst of a process is complete.
    if (IO_Queue.empty()) return;

    for (list<process>::iterator process_Iterator = IO_Queue.begin(); process_Iterator != IO_Queue.end(); ++process_Iterator)
    {// Completes all IO queue, from beginning to end.
        if (global_Clock >= (*process_Iterator).arrival_Time)
        {// Only runs if IO burst of selected process is complete.
            (*process_Iterator).state = "ready";
            ready_Queue.push_back(*process_Iterator);
            IO_Queue.erase(process_Iterator);
            restore_Processes(IO_Queue, ready_Queue);
            break;
        }
    }
    if (input == "sjf") // Sort ready queue by ascending CPU time if SJF.
        ready_Queue.sort(CPU_Scheduling);
    else if (input == "mlfq") // Sort ready queue by priority if MLFQ.
        ready_Queue.sort(priority_Scheduling);

    return;
}

void scheduler::display_ContextSwitch(list<process> ready_Queue, list<process> IO_Queue, process temp)
{
    // Display the current execution time.
    cout << "The current execution time is: " << global_Clock << endl;

    // Display the current running process.
    cout << "The running process was " << temp.process_Name << endl;

    if (!ready_Queue.empty())
    {// If ready queue has processes, display them.
        cout << "The processes in the ready queue and their CPU burst times are:" << endl;
        for (process i : ready_Queue)
        {// Display the next cpu burst times for every process in the ready queue.
            cout << i.process_Name << ": "
                << i.CPU_Times.front()
                << endl;
        }
    }

    IO_Queue.sort(arrival_Scheduling);
    if (!IO_Queue.empty())
    {// If IO queue has processes, display them.
        cout << "The processes in I/O and their remaining I/O burst times are:" << endl;
        for (process i : IO_Queue)
        {// Display the remaining time for every process for its I/O burst completion.
            cout << i.process_Name << ": "
                << ((i.arrival_Time > global_Clock) ? i.arrival_Time - global_Clock : 0)
                << endl;
        }
    }

    if (temp.CPU_Times.size() == 0 && temp.IO_Times.size() == 0)
    {// Display only if a process has completed all CPU and I/O bursts.
        cout << temp.process_Name << " has completed its total execution!" << endl;
    }

    // Add another newline to format output.
    cout << "-----" << endl;
    return;
}

void scheduler::display_Simulation_End()
{
    // Display total time needed to run all processes.
    string uppercase_Input(input);
    transform(input.begin(), input.end(), uppercase_Input.begin(), ::toupper);
    cout << "The total runtime for " << uppercase_Input << " is: " << global_Clock << endl;

    // Display CPU Utilization as the sum of all CPU times, divided by total CPU running time.
    float CPU_Utilization = 0.0;
    for (process i : job_Queue)
    {
        for (int j : i.CPU_Times)
        {
            CPU_Utilization += j;
        }
    }
    CPU_Utilization /= global_Clock;
    cout << "The % CPU Utilization is " << CPU_Utilization * 100 << "%" << endl;

    // Display the individual and average times for each process in the list.
    find_Average_Times();

    return;
}

void scheduler::find_Average_Times()
{
    float average_Waiting_Time = 0.0, average_Response_Time = 0.0, average_Turnaround_Time = 0.0;

    // Set width to length of the string "Process ID" plus 1, i.e. 11.
    cout << setw(13) << left
        << "Process Name "
        << setw(12) << left
        << "T_w"
        << setw(12) << left
        << "T_tr"
        << setw(12) << left
        << "T_r" << endl;
    cout << "---------------------------------------" << endl;
    for (process i : job_Queue)
    {
        average_Waiting_Time += i.waiting_Time;
        average_Response_Time += i.response_Time;
        average_Turnaround_Time += i.turnaround_Time;
        cout << setw(13) << left
            << i.process_Name
            << setw(12) << left
            << i.waiting_Time
            << setw(12) << left
            << i.turnaround_Time
            << setw(12) << left
            << i.response_Time << endl;
        cout << "---------------------------------------" << endl;
    }

    int n = job_Queue.size();
    average_Waiting_Time /= n;
    average_Response_Time /= n;
    average_Turnaround_Time /= n;

    cout << setw(13) << left
        << "Average: "
        << setw(12) << left
        << average_Waiting_Time
        << setw(12) << left
        << average_Turnaround_Time
        << setw(12) << left
        << average_Response_Time << endl;
    return;
}