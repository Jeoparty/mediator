#ifndef MEDIATOR_WATCHDOG_H
#define MEDIATOR_WATCHDOG_H

#include <thread>
#include <chrono>
#include <condition_variable>

class watchdog
{
private:
    std::function<void(void)> timeout_function;
    std::thread timer_thread;
    std::mutex wait_mutex;
    std::condition_variable cv;
    bool thread_done;
    bool stop_thread;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_kick;
    std::chrono::milliseconds delay;
public:
    watchdog(std::chrono::milliseconds delay, std::function<void(void)> timeout_function);
    watchdog(watchdog const&) = delete;
    watchdog& operator=(const watchdog &) = delete;
    ~watchdog();
    void timer_loop();
    void kick();
    void stop();
};

#endif //MEDIATOR_WATCHDOG_H
