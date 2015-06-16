#ifndef MEDIATOR_BUZZERGROUP_MANAGER_H
#define MEDIATOR_BUZZERGROUP_MANAGER_H

#include <memory>
#include <map>
#include <string>

#include "buzzergroup.hpp"
#include "event.hpp"

class buzzergroup_manager
{
private:
    std::map<std::string, std::unique_ptr<buzzergroup>> buzzergroups;
    void on_buzzergroup_connect_failed(buzzergroup &, std::string);
    void on_buzzergroup_disconnected(buzzergroup &);
    void disconnect_buzzergroups();
public:
    event<std::string, unsigned char> buzzer_connected;
    event<std::string, unsigned char> buzzer_disconnected;
    event<std::string, unsigned char> buzzer_hit;
    event<std::string, std::string> buzzergroup_connect_failed;
    event<std::string, const std::set<unsigned char> &> buzzergroup_connected;
    event<std::string> buzzergroup_disconnected;
    void connect(std::string device);
};

#endif //MEDIATOR_BUZZERGROUP_MANAGER_H
