#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    float response_ratio;
};

float calculateResponseRatio(Process& p, int current_time) {
    int waiting_time = current_time - p.arrival_time;
    return (float)(waiting_time + p.burst_time) / p.burst_time;
}

void HRRN_Scheduling(vector<Process>& processes) {
    int n = processes.size();
    int current_time = 0;
    int completed_processes = 0;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    vector<bool> is_completed(n, false);
    while (completed_processes < n) {
        int idx = -1;
        float max_response_ratio = -1;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time) {
                processes[i].response_ratio = calculateResponseRatio(processes[i], current_time);
                if (processes[i].response_ratio > max_response_ratio) {
                    max_response_ratio = processes[i].response_ratio;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            is_completed[idx] = true;
            processes[idx].completion_time = current_time + processes[idx].burst_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;

            current_time = processes[idx].completion_time;
            completed_processes++;
        }
    }
}

void calculateAverageTimes(vector<Process>& processes) {
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    int n = processes.size();

    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    float average_waiting_time = total_waiting_time / n;
    float average_turnaround_time = total_turnaround_time / n;

    cout << "Average Waiting Time (AWT): " << average_waiting_time << endl;
    cout << "Average Turnaround Time (ATAT): " << average_turnaround_time << endl;
}

void printExecutionOrder(const vector<Process>& processes) {
    cout << "Execution Order: ";
    for (const auto& p : processes) {
        cout << "P" << p.id << " ";
    }
    cout << endl;
}

int main() {
    int n;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter Arrival Time and Burst Time for Process " << i << ": ";
        while (!(cin >> processes[i].arrival_time >> processes[i].burst_time)) {
            cout << "Invalid input. Please enter numeric values for Arrival Time and Burst Time: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    HRRN_Scheduling(processes);

    printExecutionOrder(processes);

    calculateAverageTimes(processes);

    return 0;
}
