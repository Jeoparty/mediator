#ifndef MEDIATOR_SERIAL_BUZZERPOOL_H
#define MEDIATOR_SERIAL_BUZZERPOOL_H

#include <string>
#include <thread>
#include <mutex>

#include <boost/asio.hpp>

#include "buzzergroup.h"
#include "watchdog.h"

class serial_buzzergroup : public buzzergroup
{
private:
    std::string device;
    boost::asio::io_service io;
    boost::asio::serial_port port;
    std::thread serial_read_thread;
    std::mutex write_mutex;
    watchdog ping_watchdog;
    watchdog timeout_watchdog;
    bool stop_thread;
    bool device_ready;
    void thread_loop();
    void send_pong();
public:
    serial_buzzergroup(std::string device);
    serial_buzzergroup(serial_buzzergroup const &) = delete;
    serial_buzzergroup & operator=(const serial_buzzergroup &) = delete;
    virtual ~serial_buzzergroup();
    virtual void start_threads();
    virtual std::string get_id() const;
    virtual bool is_ready() const;
    virtual void set_color(unsigned char buzzer_id, unsigned char r, unsigned char g, unsigned char b);
    std::set<unsigned char> perform_handshake(bool include_first_byte = true);
    void send_ping();
    void on_timeout();
};

#endif //MEDIATOR_SERIAL_BUZZERPOOL_H
