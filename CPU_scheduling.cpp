#include <iostream>
#include <vector>
#include <climits>  // For INT_MAX
using namespace std;

struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int priority;   // Priority
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

// Function to calculate FCFS (First-Come, First-Served)
void calculateFCFS(vector<Process>& proc) {
    int total_waiting = 0, total_turnaround = 0;
    int current_time = 0;

    for (int i = 0; i < proc.size(); i++) {
        if (current_time < proc[i].arrival) {
            current_time = proc[i].arrival; // Wait for the process to arrive
        }
        proc[i].waiting = current_time - proc[i].arrival;
        proc[i].turnaround = proc[i].waiting + proc[i].burst;
        current_time += proc[i].burst;

        total_waiting += proc[i].waiting;
        total_turnaround += proc[i].turnaround;
    }

    cout << "\nFCFS Scheduling:\n";
    cout << "Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAverage Waiting Time: " << (float)total_waiting / proc.size();
    cout << "\nAverage Turnaround Time: " << (float)total_turnaround / proc.size() << endl;
}

// Function to calculate SJF (Shortest Job First)
void calculateSJF(vector<Process>& proc) {
    int total_waiting = 0, total_turnaround = 0;
    int current_time = 0;
    vector<bool> completed(proc.size(), false);

    for (int completed_count = 0; completed_count < proc.size();) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_time && !completed[i] && proc[i].burst < min_burst) {
                min_burst = proc[i].burst;
                idx = i;
            }
        }

        if (idx != -1) {
            proc[idx].waiting = current_time - proc[idx].arrival;
            proc[idx].turnaround = proc[idx].waiting + proc[idx].burst;
            current_time += proc[idx].burst;
            completed[idx] = true;
            total_waiting += proc[idx].waiting;
            total_turnaround += proc[idx].turnaround;
            completed_count++;
        } else {
            current_time++;
        }
    }

    cout << "\nSJF Scheduling:\n";
    cout << "Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAverage Waiting Time: " << (float)total_waiting / proc.size();
    cout << "\nAverage Turnaround Time: " << (float)total_turnaround / proc.size() << endl;
}

// Function to calculate SRTF (Shortest Remaining Time First)
void calculateSRTF(vector<Process>& proc) {
    int total_waiting = 0, total_turnaround = 0;
    int current_time = 0;
    vector<int> remaining_burst(proc.size());

    for (int i = 0; i < proc.size(); i++) remaining_burst[i] = proc[i].burst;

    while (true) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_time && remaining_burst[i] > 0 && remaining_burst[i] < min_burst) {
                min_burst = remaining_burst[i];
                idx = i;
            }
        }

        if (idx != -1) {
            remaining_burst[idx]--;
            current_time++;

            if (remaining_burst[idx] == 0) {
                proc[idx].turnaround = current_time - proc[idx].arrival;
                proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;
                total_waiting += proc[idx].waiting;
                total_turnaround += proc[idx].turnaround;
            }
        } else {
            current_time++;
        }

        bool all_completed = true;
        for (int i = 0; i < proc.size(); i++) {
            if (remaining_burst[i] > 0) {
                all_completed = false;
                break;
            }
        }
        if (all_completed) break;
    }

    cout << "\nSRTF Scheduling:\n";
    cout << "Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAverage Waiting Time: " << (float)total_waiting / proc.size();
    cout << "\nAverage Turnaround Time: " << (float)total_turnaround / proc.size() << endl;
}

// Function to calculate Priority Scheduling
void calculatePriority(vector<Process>& proc) {
    int total_waiting = 0, total_turnaround = 0;
    int completed = 0, current_time = 0;
    int min_priority_index;

    while (completed < proc.size()) {
        min_priority_index = -1;
        int min_priority = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_time && proc[i].waiting == -1) {
                if (proc[i].priority < min_priority) {
                    min_priority = proc[i].priority;
                    min_priority_index = i;
                }
            }
        }

        if (min_priority_index != -1) {
            proc[min_priority_index].waiting = current_time - proc[min_priority_index].arrival;
            proc[min_priority_index].turnaround = proc[min_priority_index].waiting + proc[min_priority_index].burst;
            current_time += proc[min_priority_index].burst;
            total_waiting += proc[min_priority_index].waiting;
            total_turnaround += proc[min_priority_index].turnaround;
            completed++;
        } else {
            current_time++;
        }
    }

    cout << "\nPriority Scheduling:\n";
    cout << "Process ID\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.priority << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAverage Waiting Time: " << (float)total_waiting / proc.size();
    cout << "\nAverage Turnaround Time: " << (float)total_turnaround / proc.size() << endl;
}

int main() {
    int n, choice;
    bool continue_program = true;

    // Get the number of processes
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> proc(n);

    // Get process details: arrival time, burst time, and priority
    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;  // Assign process ID
        cout << "Enter arrival time, burst time, and priority for process " << proc[i].id << ": ";
        cin >> proc[i].arrival >> proc[i].burst >> proc[i].priority;
        proc[i].waiting = -1;  // Initialize waiting time as -1 (not yet calculated)
    }

    // Loop for the menu until the user chooses to exit
    while (continue_program) {
        cout << "\nChoose scheduling algorithm:\n";
        cout << "1. FCFS\n2. SJF\n3. SRTF\n4. Priority\n5. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                calculateFCFS(proc);
                break;
            case 2:
                calculateSJF(proc);
                break;
            case 3:
                calculateSRTF(proc);
                break;
            case 4:
                calculatePriority(proc);
                break;
            case 5:
                cout << "Exiting the program.\n";
                continue_program = false;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
