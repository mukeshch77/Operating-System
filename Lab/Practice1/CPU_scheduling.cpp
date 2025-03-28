#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// Structure to store process details
struct Process {
    int id;        // Process ID
    int bt;        // Burst Time (Execution time)
    int at;        // Arrival Time
    int wt;        // Waiting Time
    int tat;       // Turnaround Time
    int priority;  // Priority (used for Priority Scheduling)
};

// Function to calculate waiting time and turnaround time for FCFS
void FCFS(vector<Process>& processes) {
    processes[0].wt = 0;  // The first process doesn't wait
    processes[0].tat = processes[0].bt; // TAT for first process is its burst time

    // Calculate waiting time and turnaround time for other processes
    for (int i = 1; i < processes.size(); i++) {
        processes[i].wt = processes[i - 1].wt + processes[i - 1].bt - processes[i].at; // Adjusted for arrival time
        processes[i].tat = processes[i].wt + processes[i].bt; // Turnaround time = waiting time + burst time
    }
}

// Function to calculate waiting time and turnaround time for SJF (Non-Preemptive)
void SJF(vector<Process>& processes) {
    // Sort processes by burst time (smallest burst time first)
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.bt < b.bt;
    });

    FCFS(processes);  // Once sorted, we can use FCFS logic to calculate waiting time and TAT
}

// Function to calculate waiting time and turnaround time for Priority Scheduling (Non-Preemptive)
void priorityScheduling(vector<Process>& processes) {
    // Sort processes by priority (smallest priority value first)
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    FCFS(processes);  // Use FCFS logic after sorting by priority
}

// Function to calculate waiting time and turnaround time for SRTF (Preemptive)
void SRTF(vector<Process>& processes) {
    vector<int> remainingTime(processes.size());
    for (int i = 0; i < processes.size(); i++) {
        remainingTime[i] = processes[i].bt;  // Initially, remaining time is the burst time
    }

    int completed = 0;
    int currentTime = 0;
    while (completed != processes.size()) {
        int shortest = -1;
        int minTime = 1000; // A large number, assuming no burst time is this large

        // Find the process with the shortest remaining time at the current time
        for (int i = 0; i < processes.size(); i++) {
            if (remainingTime[i] > 0 && processes[i].at <= currentTime) {
                if (remainingTime[i] < minTime) {
                    minTime = remainingTime[i];
                    shortest = i;
                }
            }
        }

        if (shortest == -1) break;

        // Execute the process (reduce its remaining time)
        remainingTime[shortest]--;
        currentTime++;

        // If the process is finished, calculate its waiting time and turnaround time
        if (remainingTime[shortest] == 0) {
            completed++;
            processes[shortest].tat = currentTime - processes[shortest].at;
            processes[shortest].wt = processes[shortest].tat - processes[shortest].bt;
        }
    }
}

// Function to calculate average waiting time and average turnaround time
void calculateAverageTimes(vector<Process>& processes) {
    int totalWT = 0, totalTAT = 0;

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < processes.size(); i++) {
        totalWT += processes[i].wt;
        totalTAT += processes[i].tat;
    }

    // Calculate and display average waiting time and average turnaround time
    float avgWT = (float)totalWT / processes.size();
    float avgTAT = (float)totalTAT / processes.size();

    cout << "\nProcess\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < processes.size(); i++) {
        cout << "P" << processes[i].id << "\t" << processes[i].bt << "\t\t" 
             << processes[i].at << "\t\t" << processes[i].wt << "\t\t" << processes[i].tat << endl;
    }

    cout << "\nAverage Waiting Time: " << avgWT << endl;
    cout << "Average Turnaround Time: " << avgTAT << endl;
}

int main() {
    int n;
    int choice;

    // Get number of processes
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Get details of each process (ID, Burst Time, Arrival Time, and Priority for priority scheduling)
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "\nEnter burst time for Process " << processes[i].id << ": ";
        cin >> processes[i].bt;
        cout << "Enter arrival time for Process " << processes[i].id << ": ";
        cin >> processes[i].at;
        if (i != 0) {  // Priority is required only for non-first processes
            cout << "Enter priority for Process " << processes[i].id << ": ";
            cin >> processes[i].priority;
        }
    }

    // Ask user which scheduling algorithm to use
    cout << "\nSelect Scheduling Algorithm:\n";
    cout << "1. FCFS\n";
    cout << "2. SJF\n";
    cout << "3. SRTF\n";
    cout << "4. Priority Scheduling\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch(choice) {
        case 1:
            FCFS(processes);
            break;
        case 2:
            SJF(processes);
            break;
        case 3:
            SRTF(processes);
            break;
        case 4:
            priorityScheduling(processes);
            break;
        default:
            cout << "Invalid choice!";
            return 0;
    }

    // Calculate and display average times
    calculateAverageTimes(processes);

    return 0;
}



