#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

#define BUFFER_SIZE 5

class ProducerConsumer {
private:
    std::vector<int> buffer;
    int count = 0;
    std::mutex mtx;
    std::condition_variable cv_empty;
    std::condition_variable cv_full;

public:
    ProducerConsumer() : buffer(BUFFER_SIZE) {}

    void producer() {
        for (int i = 0; i < 10; i++) {
            int item = i + 1;
            std::unique_lock<std::mutex> lock(mtx);
            cv_empty.wait(lock, [this]() { return count < BUFFER_SIZE; });
            buffer[count++] = item;
            std::cout << "Producer produced: " << item << std::endl;
            cv_full.notify_one();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void consumer() {
        for (int i = 0; i < 10; i++) {
            int item;
            std::unique_lock<std::mutex> lock(mtx);
            cv_full.wait(lock, [this]() { return count > 0; });
            item = buffer[--count];
            std::cout << "Consumer consumed: " << item << std::endl;
            cv_empty.notify_one();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
};

int main() {
    ProducerConsumer pc;
    std::thread prod(&ProducerConsumer::producer, &pc);
    std::thread cons(&ProducerConsumer::consumer, &pc);
    prod.join();
    cons.join();
    return 0;
}
