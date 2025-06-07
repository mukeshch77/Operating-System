#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int priority;
    int waiting;
    int turnaround;
};

void calculateFCFS(vector<Process>& proc) {
    int total_Waiting = 0, total_Turnaround = 0;
    int current_Time = 0;

    for (int i = 0; i < proc.size(); i++) {
        if (current_Time < proc[i].arrival) {
            current_Time = proc[i].arrival;
        }
        proc[i].waiting = current_Time - proc[i].arrival;
        proc[i].turnaround = proc[i].waiting + proc[i].burst;
        current_Time += proc[i].burst;

        total_Waiting += proc[i].waiting;
        total_Turnaround += proc[i].turnaround;
    }

    cout << "\nFCFS Scheduling:\n";
    cout << "PID\tAT\tBT\tPriority\tWT\tTT\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAvg WT: " << (float)total_Waiting / proc.size();
    cout << "\nAvg TT: " << (float)total_Turnaround / proc.size() << endl;
}

void calculateSJF(vector<Process>& proc) {
    int total_Waiting = 0, total_Turnaround = 0;
    int current_Time = 0;
    vector<bool> completed(proc.size(), false);

    for (int completed_count = 0; completed_count < proc.size();) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_Time && !completed[i] && proc[i].burst < min_burst) {
                min_burst = proc[i].burst;
                idx = i;
            }
        }

        if (idx != -1) {
            proc[idx].waiting = current_Time - proc[idx].arrival;
            proc[idx].turnaround = proc[idx].waiting + proc[idx].burst;
            current_Time += proc[idx].burst;
            completed[idx] = true;
            total_Waiting += proc[idx].waiting;
            total_Turnaround += proc[idx].turnaround;
            completed_count++;
        } else {
            current_Time++;
        }
    }

    cout << "\nSJF Scheduling:\n";
    cout << "PID\tAT\tBT\tPriority\tWT\tTT\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAvg WT: " << (float)total_Waiting / proc.size();
    cout << "\nAvg TT: " << (float)total_Turnaround / proc.size() << endl;
}

void calculateSRTF(vector<Process>& proc) {
    int total_Waiting = 0, total_Turnaround = 0;
    int current_Time = 0;
    vector<int> remaining_burst(proc.size());

    for (int i = 0; i < proc.size(); i++) remaining_burst[i] = proc[i].burst;

    while (true) {
        int idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_Time && remaining_burst[i] > 0 && remaining_burst[i] < min_burst) {
                min_burst = remaining_burst[i];
                idx = i;
            }
        }

        if (idx != -1) {
            remaining_burst[idx]--;
            current_Time++;

            if (remaining_burst[idx] == 0) {
                proc[idx].turnaround = current_Time - proc[idx].arrival;
                proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;
                total_Waiting += proc[idx].waiting;
                total_Turnaround += proc[idx].turnaround;
            }
        } else {
            current_Time++;
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
    cout << "PID\tAT\tBT\tPriority\tWT\tTT\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAvg WT: " << (float)total_Waiting / proc.size();
    cout << "\nAvg TT: " << (float)total_Turnaround / proc.size() << endl;
}

void calculatePriority(vector<Process>& proc) {
    int total_Waiting = 0, total_Turnaround = 0;
    int Completed = 0, current_Time = 0;
    int min_Priority_index;

    while (Completed < proc.size()) {
        min_Priority_index = -1;
        int min_Priority = INT_MAX;

        for (int i = 0; i < proc.size(); i++) {
            if (proc[i].arrival <= current_Time && proc[i].waiting == -1) {
                if (proc[i].priority < min_Priority) {
                    min_Priority = proc[i].priority;
                    min_Priority_index = i;
                }
            }
        }

        if (min_Priority_index != -1) {
            proc[min_Priority_index].waiting = current_Time - proc[min_Priority_index].arrival;
            proc[min_Priority_index].turnaround = proc[min_Priority_index].waiting + proc[min_Priority_index].burst;
            current_Time += proc[min_Priority_index].burst;
            total_Waiting += proc[min_Priority_index].waiting;
            total_Turnaround += proc[min_Priority_index].turnaround;
            Completed++;
        } else {
            current_Time++;
        }
    }

    cout << "\nPriority Scheduling:\n";
    cout << "PID\tAT\tBT\tPriority\tWT\tTT\n";
    for (const auto& p : proc) {
        cout << p.id << "\t\t" 
             << p.arrival << "\t\t" 
             << p.burst << "\t\t" 
             << p.priority << "\t\t" 
             << p.waiting << "\t\t" 
             << p.turnaround << endl;
    }
    cout << "\nAvg WT: " << (float)total_Waiting / proc.size();
    cout << "\nAvg TT: " << (float)total_Turnaround / proc.size() << endl;
}

int main() {
    int n, choice;
    bool continue_program = true;

    cout << "Enter the no of processes: ";
    cin >> n;

    vector<Process> proc(n);

    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        cout << "Enter AT, BT, and priority for process " << proc[i].id << ": ";
        cin >> proc[i].arrival >> proc[i].burst >> proc[i].priority;
        proc[i].waiting = -1;
    }

    while (continue_program) {
        cout << "\nChoose Scheduling Algorithm:\n";
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
