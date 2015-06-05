#ifndef MEDIATOR_BUZZERPOOL_H
#define MEDIATOR_BUZZERPOOL_H

#include <string>
#include <set>

#include "event.h"

class buzzergroup
{
public:
    event<buzzergroup &, unsigned char> buzzer_connected;
    event<buzzergroup &, unsigned char> buzzer_disconnected;
    event<buzzergroup &, unsigned char> buzzer_hit;
    event<buzzergroup &, std::string> buzzergroup_connect_failed;
    event<buzzergroup &, std::set<unsigned char>> buzzergroup_connected;
    event<buzzergroup &> buzzergroup_disconnected;
    virtual ~buzzergroup() = default;
    virtual void set_color(unsigned char buzzer_id, unsigned char r, unsigned char g, unsigned char b) = 0;
    virtual void start_threads() = 0;
};

#endif //MEDIATOR_BUZZERPOOL_H
