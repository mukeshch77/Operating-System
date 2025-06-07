#!/bin/bash

count=10

until [ $count -lt 1 ] 
do
    echo $count
    ((count--))
done
echo "Go"

#include <stdio.h>
#include <stdbool.h>

#define P 5  // Number of processes
#define R 3  // Number of resources

void calculateAvailableResources(int available[], int allocation[][R], int max[][R], int need[][R]) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    for (int i = 0; i < R; i++) {
        available[i] = 0;
        for (int j = 0; j < P; j++) {
            available[i] += allocation[j][i];
        }
        available[i] = available[i] - max[i][i]; // Adjust available resource
    }
}

bool isSafeState(int available[], int allocation[][R], int max[][R], int need[][R]) {
    bool finish[P] = { false };
    int work[R];
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    while (true) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canProceed = true;
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }

    for (int i = 0; i < P; i++) {
        if (!finish[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int available[R] = { 3, 3, 2 };  // Available resources
    int max[P][R] = { 
        {7, 5, 3}, 
        {3, 2, 2}, 
        {9, 0, 2}, 
        {2, 2, 2}, 
        {4, 3, 3} 
    };  // Maximum demand for each process
    int allocation[P][R] = { 
        {0, 1, 0}, 
        {2, 0, 0}, 
        {3, 0, 2}, 
        {2, 1, 1}, 
        {0, 0, 2} 
    };  // Allocation of resources to processes
    int need[P][R];  // Need matrix

    // Calculate the available resources
    calculateAvailableResources(available, allocation, max, need);

    // Check if the system is in a safe state
    if (isSafeState(available, allocation, max, need)) {
        printf("The system is in a safe state.\n");
    } else {
        printf("The system is in an unsafe state.\n");
    }

    return 0;
}

