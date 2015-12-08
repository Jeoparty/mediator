#ifndef MEDIATOR_BUZZERGROUP_MANAGER_H
#define MEDIATOR_BUZZERGROUP_MANAGER_H

#include <memory>
#include <map>
#include <string>

#include "buzzergroup.hpp"
#include "event.hpp"
#include "device_type.hpp"
#include "buzzer.hpp"

class buzzergroup_manager
{
private:
    std::map<std::string, std::unique_ptr<buzzergroup>> buzzergroups;
    void on_buzzergroup_connect_failed(buzzergroup &, std::string);
    void on_buzzergroup_disconnected(buzzergroup &, disconnect_reason);
public:
    event<buzzer> buzzer_connected;
    event<buzzer> buzzer_disconnected;
    event<buzzer> buzzer_hit;
    event<std::string, std::string> buzzergroup_connect_failed;
    event<std::string, const std::set<unsigned char> &> buzzergroup_connected;
    event<std::string, disconnect_reason> buzzergroup_disconnected;
    void connect(std::string device, device_type);
    void disconnect_buzzergroups();
};

#endif //MEDIATOR_BUZZERGROUP_MANAGER_H
