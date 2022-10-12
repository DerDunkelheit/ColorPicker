#pragma once

#include "Event.h"

class WindowFlagEvent : public Event
{
public:
    WindowFlagEvent(const std::string& flagName, bool value)
        : mFlagName(flagName), mValue(value) {}
    
    const std::string& GetFlagName() const { return mFlagName; }
    bool GetValue() const { return mValue; }

    EVENT_CLASS_CATEGORY(EventCategoryGLFW)
    EVENT_CLASS_TYPE(GLFWFlag)

private:
    std::string mFlagName;
    bool mValue;
};