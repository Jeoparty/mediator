#include "keyboard_buzzergroup.hpp"

#include <linux/input.h>

using namespace std;

keyboard_buzzergroup::keyboard_buzzergroup(std::string device)
{
    this->device = device;
    file.open(device, ios::in | ios::binary);
    if (!file.is_open())
    {
        throw "Couldn't open keyboard '" + device + "'";
    }
    file.exceptions(fstream::failbit | fstream::badbit | fstream::eofbit);
}

keyboard_buzzergroup::~keyboard_buzzergroup()
{
    stop_thread = true;
    file.close();
    if (this_thread::get_id() == input_thread.get_id())
        input_thread.detach();
    else
        input_thread.join();
}

void keyboard_buzzergroup::start_threads()
{
    stop_thread = false;
    input_thread = thread(bind(&keyboard_buzzergroup::thread_loop, this));
}

void keyboard_buzzergroup::thread_loop()
{
    set<unsigned char> buzzers;
    for (int i = 0;i < 255;i++)
    {
        buzzers.insert(i);
    }
    buzzergroup_connected.raise(*this, buzzers);
    buzzers.clear();
    try
    {
        while (!stop_thread)
        {
            input_event e;
            file.read(reinterpret_cast<char*>(&e), sizeof(e));
            if (e.value == 1)
            {
                buzzer_hit.raise(*this, static_cast<unsigned char>(e.code));
            }
        }
    }
    catch (ios_base::failure &e)
    {
        // If the thread was supposed to stop this exception is thrown by design and can be ignored
        if (!stop_thread)
        {
            buzzergroup_disconnected.raise(*this, disconnect_reason::ERROR);
            return;
        }
    }
    catch (...)
    {
        buzzergroup_disconnected.raise(*this, disconnect_reason::ERROR);
        return;
    }
    buzzergroup_disconnected.raise(*this, disconnect_reason::DISCONNECTED);
}

string keyboard_buzzergroup::get_id() const
{
    return device;
}

void keyboard_buzzergroup::set_color(unsigned char buzzer_id, unsigned char r, unsigned char g, unsigned char b)
{
    // Not supported by this device
}