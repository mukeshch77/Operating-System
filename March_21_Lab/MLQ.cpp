#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id, arrival_time, burst_time, completion_time;
    int waiting_time, turnaround_time;
};

void sort_by_arrival(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });
}

void execute_fcfs(vector<Process>& queue, int& current_time) {
    for (auto& p : queue) {
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        p.waiting_time = current_time - p.arrival_time;
        p.completion_time = current_time + p.burst_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        current_time += p.burst_time;

        cout << "P" << p.id << " -> ";
    }
}

void update_processes(vector<Process>& processes, vector<Process>& queue) {
    for (auto& q : queue) {
        for (auto& p : processes) {
            if (p.id == q.id) {
                p = q;
                break;
            }
        }
    }
}

void execute_mlq(vector<Process>& processes) {
    vector<Process> queue1, queue2;
    int current_time = 0;

    for (auto& p : processes) {
        if (p.id == 0 || p.id == 1 || p.id == 4) {
            queue1.push_back(p);
        } else {
            queue2.push_back(p);
        }
    }

    sort_by_arrival(queue1);
    sort_by_arrival(queue2);

    cout << "\nExecuting Queue 1 (Higher Priority, FCFS):\n";
    execute_fcfs(queue1, current_time);
    cout << "END\n";

    update_processes(processes, queue1);

    cout << "\nExecuting Queue 2 (Lower Priority, FCFS):\n";
    execute_fcfs(queue2, current_time);
    cout << "END\n";

    update_processes(processes, queue2);

    double total_wt = 0, total_tat = 0;
    cout << "\nProcess\tAT\tBT\tWT\tTAT\n";
    for (auto& p : processes) {
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
        cout << "P" << p.id << "\t" << p.arrival_time << "\t" << p.burst_time << "\t"
             << max(0, p.waiting_time) << "\t" << p.turnaround_time << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << (total_wt / processes.size());
    cout << "\nAverage Turnaround Time: " << (total_tat / processes.size()) << "\n";
}

int main() {
    vector<Process> sample_data = {
        {0, 1, 3, 0, 0, 0},
        {1, 3, 6, 0, 0, 0},
        {2, 5, 8, 0, 0, 0},
        {3, 7, 4, 0, 0, 0},
        {4, 8, 5, 0, 0, 0}
    };

    cout << "=== Sample Data Execution ===\n";
    execute_mlq(sample_data);

    int n;
    cout << "\n\nEnter the number of processes: ";
    cin >> n;

    vector<Process> user_p(n);

    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        user_p[i].id = i;
        cout << "Process " << i << ": ";
        cin >> user_p[i].arrival_time >> user_p[i].burst_time;
        user_p[i].completion_time = 0;
        user_p[i].waiting_time = 0;
        user_p[i].turnaround_time = 0;
    }

    cout << "\n=== User Input Execution ===\n";
    execute_mlq(user_p);

    return 0;
}
