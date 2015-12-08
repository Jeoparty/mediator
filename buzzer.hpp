#ifndef JEOPARDY_BUZZER_H
#define JEOPARDY_BUZZER_H

#include <string>

class buzzer
{
public:
    std::string device;
    unsigned char id;
    buzzer(std::string device, unsigned char id) : device(device), id(id) {}
};

#endif //JEOPARDY_BUZZER_H
