#include <iostream>
#include <thread>
#include <semaphore.h>
#include <vector>

constexpr int NUM_PHILOSOPHERS = 5;
sem_t forks[NUM_PHILOSOPHERS];  // Using unnamed semaphores

void philosopher(int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    if (id % 2 == 0) {
        sem_wait(&forks[left]);
        sem_wait(&forks[right]);
    } else {
        sem_wait(&forks[right]);
        sem_wait(&forks[left]);
    }

    std::cout << "Philosopher " << id << " is eating\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    sem_post(&forks[left]);
    sem_post(&forks[right]);

    std::cout << "Philosopher " << id << " finished eating\n";
}

int main() {
    std::vector<std::thread> philosophers;

    // Initialize semaphores
    for (auto& fork : forks) {
        sem_init(&fork, 0, 1);  // 0 = not shared, 1 = initial value
    }

    // Create threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    // Join threads
    for (auto& ph : philosophers) {
        ph.join();
    }

    // Cleanup
    for (auto& fork : forks) {
        sem_destroy(&fork);
    }

    return 0;
}
