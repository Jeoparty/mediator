#include "serial_buzzergroup.hpp"

#include <exception>
#include <list>

#include "serial_opcode.hpp"

using namespace std;
using namespace std::chrono;
using namespace boost::asio;

serial_buzzergroup::serial_buzzergroup(string device)
        : port(io, device),
          ping_watchdog(milliseconds(100), bind(&serial_buzzergroup::send_ping, this)),
          timeout_watchdog(milliseconds(250), bind(&serial_buzzergroup::on_timeout, this))
{
    this->device = device;
    this->connected = false;

    // Initialize serial port
    port.set_option(serial_port_base::baud_rate(9600));
    tcflush(port.lowest_layer().native_handle(), TCIOFLUSH);
    if (!port.is_open())
    {
        throw "Couldn't open serial port '" + device + "'";
    }
}

serial_buzzergroup::~serial_buzzergroup()
{
    stop_thread = true;
    port.close();
    if (serial_read_thread.get_id() == this_thread::get_id())
        serial_read_thread.detach();
    else
        serial_read_thread.join();
}

void serial_buzzergroup::start_threads()
{
    stop_thread = false;
    serial_read_thread = thread(&serial_buzzergroup::thread_loop, this);
}

void serial_buzzergroup::thread_loop()
{
    try {
        try
        {
            set<unsigned char> buzzers = perform_handshake();
            connected = true;
            buzzergroup_connected.raise(*this, buzzers);
        }
        catch (string s)
        {
            buzzergroup_connect_failed.raise(*this, s);
            return;
        }

        unsigned char opcode;
        while (!stop_thread) {
            timeout_watchdog.kick();
            ping_watchdog.kick();
            if (!connected)
            {
                unique_lock<mutex> write_lock(write_mutex);
                reset();
            }
            read(port, buffer(&opcode, 1));

            unsigned char buzzer_id;
            switch (static_cast<serial_opcode>(opcode))
            {
                case serial_opcode::SERIAL_READY:
                {
                    unique_lock<std::mutex> write_lock(write_mutex);
                    perform_handshake(false);
                    reset();
                    break;
                }
                case serial_opcode::PING:
                    send_pong();
                    break;
                case serial_opcode::PONG:
                    // Nothing to do
                    break;
                case serial_opcode::BUZZ:
                    read(port, buffer(&buzzer_id, 1));
                    if (connected)
                        buzzer_hit.raise(*this, buzzer_id);
                    break;
                case serial_opcode::CONNECT:
                    read(port, buffer(&buzzer_id, 1));
                    if (connected)
                        buzzer_connected.raise(*this, buzzer_id);
                    break;
                case serial_opcode::DISCONNECT:
                    read(port, buffer(&buzzer_id, 1));
                    if (connected)
                        buzzer_disconnected.raise(*this, buzzer_id);
                    break;
                default:
                {
                    unique_lock<std::mutex> write_lock(write_mutex);
                    reset();
                    break;
                }
            }
        }
    }
    catch (...)
    {
        // Nothing to do
    }

    connected = false;
    buzzergroup_disconnected.raise(*this, disconnect_reason::DISCONNECTED);
}

set<unsigned char> serial_buzzergroup::perform_handshake(bool include_first_byte)
{
    // Perform the handshake
    unsigned char buf[3];

    if (include_first_byte)
    {
        read(port, buffer(buf, 3));
    }
    else
    {
        buf[0] = 0x00;
        read(port, buffer(buf + 1, 2));
    }
    // Check magic number
    if (buf[0] != 0x00 || buf[1] != 0x42) {
        throw "Couldn't connect to '" + device + "' - invalid handshake received";
    }

    // Check protocol version
    if (buf[2] != 0x00) {
        throw "Couldn't connect to '" + device + "' - invalid protocol version '" + to_string(buf[2]) + "'";
    }

    // Send magic number
    buf[0] = 0x13;
    buf[1] = 0x37;
    write(port, buffer(buf, 2));

    // Receive list of connected buzzers
    set<unsigned char> buzzers;
    read(port, buffer(buf + 1, 1));
    for (unsigned char i = 0;i < buf[1];i++)
    {
        read(port, buffer(buf, 1));
        buzzers.insert(buf[0]);
    }
    timeout_watchdog.kick();
    ping_watchdog.kick();
    return buzzers;
}

void serial_buzzergroup::send_ping()
{
    unsigned char buf = static_cast<char>(serial_opcode::PING);
    unique_lock<mutex> write_lock(write_mutex);
    write(port, buffer(&buf, 1));
}

void serial_buzzergroup::send_pong()
{
    unsigned char buf = static_cast<char>(serial_opcode::PONG);
    unique_lock<mutex> write_lock(write_mutex);
    write(port, buffer(&buf, 1));
}

void serial_buzzergroup::set_color(unsigned char buzzer_id, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char buf[] = {static_cast<unsigned char>(serial_opcode::SET_COLOR), buzzer_id, r, g, b};
    unique_lock<mutex> write_lock(write_mutex);
    write(port, buffer(buf, sizeof(buf)));
}

void serial_buzzergroup::on_timeout()
{
    connected = false;
    buzzergroup_disconnected.raise(*this, disconnect_reason::TIMEOUT);
}

string serial_buzzergroup::get_id() const
{
    return device;
}

void serial_buzzergroup::reset()
{
    unsigned char buf = static_cast<unsigned char>(serial_opcode::RESET);
    connected = false;
    write(port, buffer(&buf, 1));
    tcflush(port.lowest_layer().native_handle(), TCIOFLUSH);
    set<unsigned char> buzzers = perform_handshake();
    connected = true;
    buzzergroup_connected.raise(*this, move(buzzers));
}
