// OSC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
using namespace std;
int main()
{
    int CPU_Burst_I[] = { 5, 3, 5, 4, 6, 4, 3, 5 };
    int CPU_Burst_II[] = { 4, 5, 7, 12, 9, 4, 9, 7, 8 };
    int CPU_Burst_III[] = { 8, 12, 18, 14, 4, 15, 14, 5, 6 };
    int CPU_Burst_IV[] = { 3, 4, 5, 3, 4, 5, 6, 5, 3 };
    int CPU_Burst_V[] = { 16, 17, 5, 16, 7, 13, 11, 6, 3, 4 };
    int CPU_Burst_VI[] = { 11, 4, 5, 6, 7, 9, 12, 15, 8 };
    int CPU_Burst_VII[] = { 14, 17, 11, 15, 4, 7, 16, 10 };
    int CPU_Burst_VIII[] = { 4, 5, 6, 14, 16, 6 };

    int IO_Wait_Time_I[] = { 27, 31, 43, 18, 22, 26, 24 };
    int IO_Wait_Time_II[] = { 48, 44, 42, 37, 76, 41, 31, 43 };
    int IO_Wait_Time_III[] = { 33, 41, 65, 21, 61, 18, 26, 31 };
    int IO_Wait_Time_IV[] = { 35, 41, 45, 51, 61, 54, 82, 77 };
    int IO_Wait_Time_V[] = { 24, 21, 36, 26, 31, 28, 21, 13, 11 };
    int IO_Wait_Time_VI[] = { 22, 8, 10, 12, 14, 18, 24, 30 };
    int IO_Wait_Time_VII[] = { 46, 41, 42, 21, 32, 19, 33, 10 };
    int IO_Wait_Time_VIII[] = { 14, 33, 51, 73, 87 };
    int Turnaround_Time;
    int Response_Time;
    queue<int> waitList;
    queue<int> readyList;
}
