#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

class ResourceAllocationGraph {
private:
    int processes, resources;
    vector<vector<int>> allocation;  // Process -> Resource (allocation)
    vector<vector<int>> request;     // Process -> Resource (requests)
    vector<int> available;           // Available resources
    unordered_map<int, set<int>> graph; // Graph representation

public:
    ResourceAllocationGraph(int p, int r) : processes(p), resources(r) {
        allocation.resize(p, vector<int>(r, 0));
        request.resize(p, vector<int>(r, 0));
        available.resize(r, 0);
    }

    void inputGraph() {
        cout << "Enter allocated resources for each process:\n";
        for (int i = 0; i < processes; i++) {
            cout << "Process " << i << ": ";
            for (int j = 0; j < resources; j++) {
                cin >> allocation[i][j];
            }
        }

        cout << "Enter maximum required resources for each process:\n";
        for (int i = 0; i < processes; i++) {
            cout << "Process " << i << ": ";
            for (int j = 0; j < resources; j++) {
                int maxNeed;
                cin >> maxNeed;
                request[i][j] = maxNeed - allocation[i][j]; // Request = MaxNeed - Allocated
            }
        }

        cout << "Enter available resources: ";
        for (int j = 0; j < resources; j++) {
            cin >> available[j];
        }
    }

    void buildGraph() {
        // Clear the graph
        graph.clear();

        // Add process-to-resource edges (if process is requesting a resource)
        for (int i = 0; i < processes; i++) {
            for (int j = 0; j < resources; j++) {
                if (request[i][j] > 0) {
                    graph[i].insert(j + processes); // Edge: P -> R
                }
            }
        }

        // Add resource-to-process edges (if resource is allocated to a process)
        for (int j = 0; j < resources; j++) {
            for (int i = 0; i < processes; i++) {
                if (allocation[i][j] > 0) {
                    graph[j + processes].insert(i); // Edge: R -> P
                }
            }
        }
    }

    bool detectCycleUtil(int node, vector<bool> &visited, vector<bool> &recStack) {
        if (!visited[node]) {
            visited[node] = true;
            recStack[node] = true;

            for (int neighbor : graph[node]) {
                if (!visited[neighbor] && detectCycleUtil(neighbor, visited, recStack))
                    return true;
                else if (recStack[neighbor])
                    return true;
            }
        }
        recStack[node] = false;
        return false;
    }

    bool detectDeadlock() {
        int totalNodes = processes + resources; // Nodes = P + R
        vector<bool> visited(totalNodes, false);
        vector<bool> recStack(totalNodes, false);

        for (int i = 0; i < totalNodes; i++) {
            if (detectCycleUtil(i, visited, recStack))
                return true;
        }
        return false;
    }

    void displayGraph() {
        cout << "\nResource Allocation Graph (RAG):\n";
        for (auto &entry : graph) {
            cout << (entry.first < processes ? "Process " : "Resource ") 
                 << entry.first << " -> ";
            for (int node : entry.second) {
                cout << (node < processes ? "Process " : "Resource ") << node << ", ";
            }
            cout << endl;
        }
    }
};

int main() {
    int p, r;
    cout << "Enter number of processes: ";
    cin >> p;
    cout << "Enter number of resources: ";
    cin >> r;

    ResourceAllocationGraph rag(p, r);
    rag.inputGraph();
    rag.buildGraph();
    rag.displayGraph();

    if (rag.detectDeadlock()) {
        cout << "\nDeadlock detected!" << endl;
    } else {
        cout << "\nNo deadlock in the system." << endl;
    }

    return 0;
}
