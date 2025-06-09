#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

std::mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id) {
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    if (id % 2 == 0) {
        forks[left_fork].lock();  
        forks[right_fork].lock(); 
    } else {
        forks[right_fork].lock(); 
        forks[left_fork].lock();  
    }

    std::cout << "Philosopher " << id << " is eating" << std::endl;
    sleep(1);

    forks[left_fork].unlock();
    forks[right_fork].unlock();

    std::cout << "Philosopher " << id << " has finished eating" << std::endl;
}

int main() {
    std::vector<std::thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.push_back(std::thread(philosopher, i));
    }

    for (auto& philosopher_thread : philosophers) {
        philosopher_thread.join();
    }

    return 0;
}
