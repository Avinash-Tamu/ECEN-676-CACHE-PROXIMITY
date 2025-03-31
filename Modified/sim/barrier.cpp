#include <condition_variable>
#include <mutex>
#include <atomic>
#include <iostream>

class Barrier {
private:
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<unsigned> count;
    std::atomic<unsigned> generation;
    unsigned threshold;

public:
    explicit Barrier(unsigned count) : count(count), generation(0), threshold(count) {}

    void arrive_and_wait() {
        unsigned gen = generation.load();
        if (count.fetch_sub(1) == 1) {
            // Last thread to arrive, reset and notify
            count.store(threshold);
            generation++;
            std::cout << "Barrier passed: Generation " << generation.load() << std::endl;  // Debugging log
            cv.notify_all();
        } else {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this, gen] { return gen != generation; });
        }
    }
};
