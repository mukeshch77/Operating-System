    #include <iostream>
    #include <vector>
    using namespace std;

    // Function to check if the system is in a safe state
    bool isSafe(vector<vector<int>> &max, vector<vector<int>> &alloc, vector<int> &avail, int n, int m) {
        vector<vector<int>> need(n, vector<int>(m));
        vector<int> work = avail;
        vector<bool> finish(n, false);
        vector<int> safeSeq;

        // Calculate need matrix
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                need[i][j] = max[i][j] - alloc[i][j];

        // Find a process which can be satisfied
        int count = 0;
        while (count < n) {
            bool found = false;
            for (int p = 0; p < n; p++) {
                if (!finish[p]) {
                    int j;
                    for (j = 0; j < m; j++)
                        if (need[p][j] > work[j])
                            break;

                    if (j == m) {
                        for (int k = 0; k < m; k++)
                            work[k] += alloc[p][k];

                        safeSeq.push_back(p);
                        finish[p] = true;
                        count++;
                        found = true;
                    }
                }
            }

            if (!found) {
                cout << "System is not in safe state\n";
                return false;
            }
        }

        // System is in safe state
        cout << "System is in safe state.\nSafe sequence is: ";
        for (int i = 0; i < n; i++)
            cout << "P" << safeSeq[i] << (i != n-1 ? " -> " : "");
        cout << endl;
        return true;
    }

    // Function to request resources
    bool resourceRequest(vector<vector<int>> &max, vector<vector<int>> &alloc, vector<int> &avail,
                         int n, int m, int process, vector<int> &request) {
        // Check if request is valid (request <= need)
        for (int i = 0; i < m; i++)
            if (request[i] > (max[process][i] - alloc[process][i])) {
                cout << "Error: Process has exceeded its maximum claim.\n";
                return false;
            }

        // Check if request is valid (request <= available)
        for (int i = 0; i < m; i++)
            if (request[i] > avail[i]) {
                cout << "Resources not available. Process must wait.\n";
                return false;
            }

        // Try to allocate resources temporarily
        for (int i = 0; i < m; i++) {
            avail[i] -= request[i];
            alloc[process][i] += request[i];
        }

        // Check if the system would be in safe state after allocation
        bool safe = isSafe(max, alloc, avail, n, m);

        // If not safe, roll back the allocation
        if (!safe) {
            cout << "Allocation would lead to unsafe state. Rolling back.\n";
            for (int i = 0; i < m; i++) {
                avail[i] += request[i];
                alloc[process][i] -= request[i];
            }
            return false;
        }

        cout << "Request granted. System remains in safe state.\n";
        return true;
    }

    int main() {
        int n, m; // n = number of processes, m = number of resource types
        cout << "Enter number of processes: ";
        cin >> n;
        cout << "Enter number of resource types: ";
        cin >> m;

        vector<int> avail(m);
        vector<vector<int>> max(n, vector<int>(m));
        vector<vector<int>> alloc(n, vector<int>(m));

        // Input allocation matrix
        cout << "Enter allocation matrix (" << n << "x" << m << "):\n";
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                cin >> alloc[i][j];

        // Input max matrix
        cout << "Enter max matrix (" << n << "x" << m << "):\n";
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                cin >> max[i][j];

        // Input available resources
        cout << "Enter available resources (" << m << "):\n";
        for (int i = 0; i < m; i++)
            cin >> avail[i];

        // Check initial safe state
        isSafe(max, alloc, avail, n, m);

        // Resource request simulation
        char choice;
        do {
            int process;
            vector<int> request(m);

            cout << "\nEnter process number for resource request (0-" << n-1 << "): ";
            cin >> process;
            cout << "Enter request vector (" << m << "): ";
            for (int i = 0; i < m; i++)
                cin >> request[i];

            resourceRequest(max, alloc, avail, n, m, process, request);

            cout << "\nDo you want to make another request? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');

        return 0;
    }
