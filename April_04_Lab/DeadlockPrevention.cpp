#include <iostream>
#include <vector>

using namespace std;

class DeadlockPrevention {
private:
    int numProcesses, numResources;
    vector<int> available;                    // Available instances of each resource
    vector<vector<int>> allocation;           // Current allocations
    vector<vector<int>> maxNeed;              // Maximum need per process

public:
    DeadlockPrevention(int p, int r) {
        numProcesses = p;
        numResources = r;
        available.resize(r);
        allocation.assign(p, vector<int>(r, 0));
        maxNeed.assign(p, vector<int>(r, 0));
    }

    // Function to input resource data
    void inputData() {
        cout << "\nEnter available instances of each resource:\n";
        for (int i = 0; i < numResources; i++) {
            cout << "R" << i << ": ";
            cin >> available[i];
        }

        cout << "\nEnter maximum resource needs per process:\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "P" << i << ":\n";
            for (int j = 0; j < numResources; j++) {
                cout << "Max R" << j << ": ";
                cin >> maxNeed[i][j];
            }
        }
    }

    // Function to print current allocation state
    void printState() {
        cout << "\nCurrent Allocation State:\n";
        cout << "Process\tAllocated Resources\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "P" << i << "\t";
            for (int j = 0; j < numResources; j++) {
                cout << allocation[i][j] << " ";
            }
            cout << endl;
        }

        cout << "\nAvailable Resources:\n";
        for (int j = 0; j < numResources; j++) {
            cout << "R" << j << ": " << available[j] << "\t";
        }
        cout << endl;
    }

    // Function to check if request follows increasing order
    bool isIncreasingOrder(const vector<int>& request) {
        for (int i = 1; i < numResources; i++) {
            if (request[i] > 0 && request[i - 1] == 0)
                return false;
        }
        return true;
    }

    // Function to check if request includes all resources needed (prevents Hold and Wait)
    bool isAllResourcesRequested(int processID, const vector<int>& request) {
        for (int i = 0; i < numResources; i++) {
            if (request[i] < maxNeed[processID][i])
                return false;
        }
        return true;
    }

    // Request resources with prevention checks
    bool requestResources(int processID, vector<int>& request) {
        if (!isIncreasingOrder(request)) {
            cout << "\nRequest denied: Must request resources in increasing order (prevent Circular Wait).\n";
            return false;
        }

        if (!isAllResourcesRequested(processID, request)) {
            cout << "\nRequest denied: Must request all needed resources at once (prevent Hold and Wait).\n";
            return false;
        }

        for (int i = 0; i < numResources; i++) {
            if (request[i] > available[i]) {
                cout << "\nRequest denied: Not enough resources available.\n";
                return false;
            }
        }

        // Allocate resources
        for (int i = 0; i < numResources; i++) {
            available[i] -= request[i];
            allocation[processID][i] += request[i];
        }

        cout << "\nRequest granted: Resources allocated to P" << processID << ".\n";
        return true;
    }

    // Simulate resource requests for all processes
    void simulate() {
        for (int p = 0; p < numProcesses; p++) {
            vector<int> request(numResources);
            cout << "\nEnter request for P" << p << ":\n";
            for (int j = 0; j < numResources; j++) {
                cout << "Request R" << j << ": ";
                cin >> request[j];
            }

            requestResources(p, request);
            printState();
        }

        cout << "\nDeadlock Prevention Simulation Complete.\n";
    }
};

int main() {
    int numProcesses, numResources;
    cout << "Enter number of processes (min 2): ";
    cin >> numProcesses;
    cout << "Enter number of resources (min 3): ";
    cin >> numResources;

    DeadlockPrevention dp(numProcesses, numResources);
    dp.inputData();
    dp.printState();
    dp.simulate();

    return 0;
}
