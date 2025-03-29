#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

sem_t *rw_mutex;
sem_t *mutex;
int read_count = 0;

void reader(int id) {
    for (int i = 0; i < 3; ++i) {
        sem_wait(mutex);
        ++read_count;
        if (read_count == 1) sem_wait(rw_mutex);
        sem_post(mutex);

        std::cout << "Reader " << id << "reading" << std::endl;
        sleep(1);

        sem_wait(mutex);
        --read_count;
        if (read_count == 0) sem_post(rw_mutex);
        sem_post(mutex);

        sleep(2);
    }
}

void writer(int id) {
    for (int i = 0; i < 2; ++i) {
        sem_wait(rw_mutex);

        std::cout << "Writer " << id << "writing" << std::endl;
        sleep(2);

        sem_post(rw_mutex);

        sleep(2);
    }
}

int main() {
    rw_mutex = sem_open("/rw_mutex", O_CREAT, 0644, 1);
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    for (int i = 1; i <= 3; ++i) readers.emplace_back(reader, i);
    for (int i = 1; i <= 2; ++i) writers.emplace_back(writer, i);

    for (auto& r : readers) r.join();
    for (auto& w : writers) w.join();

    sem_close(rw_mutex);
    sem_unlink("/rw_mutex");
    sem_close(mutex);
    sem_unlink("/mutex");

    return 0;
}
