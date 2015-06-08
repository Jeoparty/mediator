#include "watchdog.hpp"

using namespace std;
using namespace std::chrono;

watchdog::watchdog(milliseconds delay, function<void(void)> timeout_function)
{
    this->timeout_function = timeout_function;
    this->delay = delay;
    thread_done = true;
    stop_thread = false;
}

watchdog::~watchdog()
{
    stop_thread = true;
    cv.notify_all();
    if (timer_thread.get_id() == this_thread::get_id())
        timer_thread.detach();
    else
        if (timer_thread.joinable())
            timer_thread.join();
}

void watchdog::timer_loop()
{
    while (last_kick + delay > high_resolution_clock::now() && !stop_thread)
    {
        unique_lock<mutex> lock(wait_mutex);
        cv.wait_until(lock, last_kick + delay);
    }
    if (!stop_thread)
    {
        timeout_function();
        timer_thread.detach();
    }
    thread_done = true;
}

void watchdog::kick()
{
    last_kick = high_resolution_clock::now();
    if (thread_done)
    {
        stop_thread = false;
        thread_done = false;
        timer_thread = thread(&watchdog::timer_loop, this);
    }
}

void watchdog::stop()
{
    stop_thread = true;
    cv.notify_all();
    timer_thread.detach();
}