#include "buzzergroup_manager.h"

#include "serial_buzzergroup.h"

using namespace std;

void buzzergroup_manager::connect(std::string device)
{
    try
    {
        buzzergroups.at(device);
    }
    catch (out_of_range &)
    {
        buzzergroup *group = new serial_buzzergroup(device);
        group->buzzer_connected.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_connected.raise(buzzergroup.get_id(), buzzer_id);});
        group->buzzer_disconnected.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_disconnected.raise(buzzergroup.get_id(), buzzer_id);});
        group->buzzer_hit.connect([this](buzzergroup &buzzergroup, unsigned char buzzer_id){this->buzzer_hit.raise(buzzergroup.get_id(), buzzer_id);});
        group->buzzergroup_connected.connect([this](buzzergroup &buzzergroup, std::set<unsigned char> connected_buzzers){this->buzzergroup_connected.raise(buzzergroup.get_id(), connected_buzzers);});
        group->start_threads();
        buzzergroups.emplace(device, unique_ptr<buzzergroup>(group));
    }
}

void buzzergroup_manager::on_buzzergroup_connect_failed(buzzergroup &group, std::string error_message)
{
    buzzergroup_connect_failed.raise(group.get_id(), error_message);
    buzzergroups.erase(group.get_id());
}

void buzzergroup_manager::on_buzzergroup_disconnected(buzzergroup &group)
{
    buzzergroup_disconnected.raise(group.get_id());
    buzzergroups.erase(group.get_id());
}