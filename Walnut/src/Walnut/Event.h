#pragma once

#include <string>
#include <functional>
#include <iostream>

#define BIT(x) (1 << (x))

//Events are currently blocking, For the future , 
// a better strategy might be create an Event queue system.

enum class EventType
{
    None = 0,
    ActionEvent,
    KeyPressed, KeyReleased, KeyTyped,
};

enum EventCategory
{
    None = 0,
    EventCategoryActionEvents = BIT(1),
    EventCategoryInput = BIT(2),
    EventCategoryKeyboard = BIT(3),
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event
{
public:
    virtual ~Event() = default;

    bool Handled = false;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    bool IsInCategory(EventCategory category) const
    {
        return GetCategoryFlags() & category;
    }
};

class EventDispatcher
{
    template<typename T>
    using EventFu = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event)
        : mEvent(event)
    {
    }

    template<typename T>
    bool Dispatch(const EventFu<T> func)
    {
        if (mEvent.GetEventType() == T::GetStaticType())
        {
            mEvent.Handled = func(*reinterpret_cast<T*>(&mEvent));
            return true;
        }
        return false;
    }
private:
    Event& mEvent;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}