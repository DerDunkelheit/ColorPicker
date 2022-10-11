#pragma once

#include <sstream>
#include <Walnut/Event.h>

class ActionEvent : public Event
{
public:
    ActionEvent(const std::string& action)
            : mAction(action) {}

    const std::string& GetAction() const { return mAction; }

    EVENT_CLASS_TYPE(ActionEvent)
    EVENT_CLASS_CATEGORY(EventCategoryActionEvents)
private:
    std::string mAction;
};