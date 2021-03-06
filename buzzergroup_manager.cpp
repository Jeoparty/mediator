#include "buzzergroup_manager.hpp"

#include "serial_buzzergroup.hpp"
#include "keyboard_buzzergroup.hpp"

using namespace std;
using namespace std::placeholders;

template class event<const buzzer &>;
template class event<std::string, std::string>;
template class event<std::string, const std::set<unsigned char> &>;
template class event<std::string, disconnect_reason>;

void buzzergroup_manager::connect(std::string device, device_type type)
{
    // Check if the device exists
    if (buzzergroups.count(device) == 0)
    {
        try
        {
            // If not connect it
            unique_ptr<buzzergroup> group;
            switch (type)
            {
                case device_type::SERIAL:
                    group.reset(new serial_buzzergroup(device));
                    break;
                case device_type::KEYBOARD:
                    group.reset(new keyboard_buzzergroup(device));
                    break;
            }

            group->buzzer_connected.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_connected.raise(buzzer(buzzergroup.get_id(), buzzer_id));});
            group->buzzer_disconnected.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_disconnected.raise(buzzer(buzzergroup.get_id(), buzzer_id));});
            group->buzzer_hit.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_hit.raise(buzzer(buzzergroup.get_id(), buzzer_id));});
            group->buzzergroup_connected.connect([this](buzzergroup &buzzergroup, const std::set<unsigned char> &connected_buzzers){this->buzzergroup_connected.raise(buzzergroup.get_id(), connected_buzzers);});
            group->buzzergroup_connect_failed.connect(bind(&buzzergroup_manager::on_buzzergroup_connect_failed, this, _1, _2));
            group->buzzergroup_disconnected.connect(bind(&buzzergroup_manager::on_buzzergroup_disconnected, this, _1, _2));
            group->start_threads();
            buzzergroups.emplace(device, unique_ptr<buzzergroup>(group.release()));
        }
        catch (string &s)
        {
            buzzergroup_connect_failed.raise(device, s);
        }
    }
}

void buzzergroup_manager::on_buzzergroup_connect_failed(buzzergroup &group, std::string error_message)
{
    buzzergroup_connect_failed.raise(group.get_id(), error_message);
    buzzergroups.erase(group.get_id());
}

void buzzergroup_manager::on_buzzergroup_disconnected(buzzergroup &group, disconnect_reason reason)
{
    buzzergroup_disconnected.raise(group.get_id(), reason);
    if (reason == disconnect_reason::DISCONNECTED || reason == disconnect_reason::ERROR)
        buzzergroups.erase(group.get_id());
}

void buzzergroup_manager::disconnect_buzzergroups()
{
    buzzergroups.clear();
}
