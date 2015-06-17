#ifndef MEDIATOR_KEYBOARD_BUZZERGROUP_HPP
#define MEDIATOR_KEYBOARD_BUZZERGROUP_HPP

#include <fstream>
#include <thread>

#include "buzzergroup.hpp"

class keyboard_buzzergroup : public buzzergroup
{
private:
    std::string device;
    std::fstream file;
    std::thread input_thread;
    bool stop_thread;
    void thread_loop();
public:
    keyboard_buzzergroup(std::string device);
    keyboard_buzzergroup(keyboard_buzzergroup const &) = delete;
    keyboard_buzzergroup & operator=(const keyboard_buzzergroup &) = delete;
    virtual ~keyboard_buzzergroup();
    virtual void start_threads();
    virtual std::string get_id() const;
    virtual void set_color(unsigned char buzzer_id, unsigned char r, unsigned char g, unsigned char b);
};

#endif //MEDIATOR_KEYBOARD_BUZZERGROUP_HPP
