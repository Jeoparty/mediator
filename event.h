#ifndef MEDIATOR_EVENT_H
#define MEDIATOR_EVENT_H

#include <list>
#include <functional>

template<typename... params>
class event
{
private:
    std::list<std::function<void(params...)>> listeners;
public:
    void connect(std::function<void(params...)> listener)
    {
        listeners.push_back(listener);
    }
    void raise(params... parameters)
    {
        for (std::function<void(params...)> &listener : listeners)
        {
            listener(parameters...);
        }
    }
};


#endif //MEDIATOR_EVENT_H
