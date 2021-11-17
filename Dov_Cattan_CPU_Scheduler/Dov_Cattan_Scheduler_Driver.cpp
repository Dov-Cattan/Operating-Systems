//Dov_Cattan_Scheduling_Driver.cpp Outputs all the schedulng for the set processes
#include "Dov_Cattan_Scheduler.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main()
{
    cout.precision(2);
    cout << fixed;
    scheduler data("Dov_Cattan_Processes.txt");
    cout << "Please choose one of the Scheduling Options: 'FCFS' to simulate First Come For Serve, 'SJF' to simulate Shortest Job First, 'MLFQ' to simulate Multilevel Feedback Queue, or press enter to exit." << endl;
    string scheduling_Option;
    while (getline(cin, scheduling_Option)) {

        if (scheduling_Option !="FCFS" && scheduling_Option != "SJF" && scheduling_Option != "MLFQ") {
            cout << "Finished" << endl;
            return 0;
        }
        else if (scheduling_Option == "FCFS") {
            data.simulate("FCFS");
            cout << "To run another simulation, please choose one of the Scheduling Options: FCFS for First Come For Serve, SJF for Shortest Job First, MLFQ for Multilevel Feedback Queue, or enter to EXIT" << endl;
        }
        else if (scheduling_Option == "SJF") {
            data.simulate("SJF");
            cout << "To run another simulation, please choose one of the Scheduling Options: FCFS for First Come For Serve, SJF for Shortest Job First, MLFQ for Multilevel Feedback Queue, or enter to EXIT" << endl;
        }
        else if (scheduling_Option == "MLFQ") {
            data.simulate("MLFQ");
            cout << "To run another simulation, please choose one of the Scheduling Options: FCFS for First Come For Serve, SJF for Shortest Job First, MLFQ for Multilevel Feedback Queue, or enter to EXIT" << endl;
        }
        else
            system("cls"); // Clear the screen between runs.
         
    }
       
}
