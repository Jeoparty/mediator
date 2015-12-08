#ifndef MEDIATOR_BUZZER_H
#define MEDIATOR_BUZZER_H

#include <string>

class buzzer
{
public:
    std::string device;
    unsigned char id;
    buzzer() = default;
    buzzer(std::string device, unsigned char id) : device(device), id(id) {}
    bool operator==(const buzzer &other) const
    {
        return this->id == other.id && this->device == other.device;
    }
    bool operator!=(const buzzer &other) const
    {
        return !(*this == other);
    }

};

#endif // MEDIATOR_BUZZER_H
