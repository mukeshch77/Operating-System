#include <iostream>
#include <vector>
#include <iomanip>

#define TIME_QUANTUM_1 4
#define TIME_QUANTUM_2 8
#define MAX_EXECUTIONS 100  // For storing execution history

using namespace std;

struct Process {
    int id, arrivalTime, burstTime, remainingTime, waitingTime, turnaroundTime;
};

struct GanttBlock {
    int processID, startTime, endTime;
};

vector<GanttBlock> ganttChart;

void inputProcesses(vector<Process>& processes, int n) {
    for (int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "Enter arrival time and burst time for process P" << p.id << ": ";
        cin >> p.arrivalTime >> p.burstTime;
        p.remainingTime = p.burstTime;
        p.waitingTime = p.turnaroundTime = 0;
        processes.push_back(p);
    }
}

void executeRoundRobin(vector<Process>& processes, int& currentTime, int quantum, int& completedProcesses, int queueLevel) {
    for (auto& p : processes) {
        if (p.remainingTime > 0 && p.arrivalTime <= currentTime) {
            int execTime = min(p.remainingTime, quantum);
            cout << "\nTime " << currentTime << " - " << (currentTime + execTime) << ": P" << p.id << " (Queue " << queueLevel << ")";
            ganttChart.push_back({p.id, currentTime, currentTime + execTime});
            currentTime += execTime;
            p.remainingTime -= execTime;

            if (p.remainingTime == 0) {
                p.turnaroundTime = currentTime - p.arrivalTime;
                p.waitingTime = p.turnaroundTime - p.burstTime;
                completedProcesses++;
            }
        }
    }
}

void executeFCFS(vector<Process>& processes, int& currentTime, int& completedProcesses) {
    for (auto& p : processes) {
        if (p.remainingTime > 0) {
            cout << "\nTime " << currentTime << " - " << (currentTime + p.remainingTime) << ": P" << p.id << " (Queue 3 - FCFS)";
            ganttChart.push_back({p.id, currentTime, currentTime + p.remainingTime});
            currentTime += p.remainingTime;
            p.turnaroundTime = currentTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;
            p.remainingTime = 0;
            completedProcesses++;
        }
    }
}

void printGanttChart() {
    cout << "\n\nGantt Chart:\n-------------------------------------------------\n";
    for (const auto& block : ganttChart) {
        cout << "| P" << block.processID << " ";
    }
    cout << "|\n-------------------------------------------------\n0";
    for (const auto& block : ganttChart) {
        cout << "      " << block.endTime;
    }
    cout << "\n";
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    
    vector<Process> processes;
    inputProcesses(processes, n);
    
    int completedProcesses = 0, currentTime = 0;
    
    cout << "\nExecution Order:";
    cout << "\n------------------------------------------------";
    
    executeRoundRobin(processes, currentTime, TIME_QUANTUM_1, completedProcesses, 1);
    executeRoundRobin(processes, currentTime, TIME_QUANTUM_2, completedProcesses, 2);
    executeFCFS(processes, currentTime, completedProcesses);
    
    cout << "\n------------------------------------------------";
    
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (const auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }
    
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time (AWT): " << (totalWaitingTime / n);
    cout << "\nAverage Turnaround Time (ATAT): " << (totalTurnaroundTime / n) << "\n";
    
    printGanttChart();
    
    return 0;
}
