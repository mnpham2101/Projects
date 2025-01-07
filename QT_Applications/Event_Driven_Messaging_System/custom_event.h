#ifndef CUSTOM_EVENT_H
#define CUSTOM_EVENT_H

#include <Qt>
#include <QEvent>

/*
 *
 * */
class CustomEvent : public QEvent {

public:
    enum CustomEventPriority
    {
        // An important event
        ImportantEventPriority = Qt::HighEventPriority,        
        // A critical event
        CriticalEventPriority = 100 * ImportantEventPriority,
        // Not that important
        StatusEventPriority = Qt::LowEventPriority,
        // These are less important than Status events
        IdleProcessingDoneEventPriority = StatusEventPriority - 1
    };

    CustomEvent (QEvent::Type CustomEventType, CustomEvent::CustomEventPriority CustomPriority)
        : QEvent(CustomEventType)
        , priority(CustomPriority){}
    CustomEventPriority getPriority() const { return priority;}

private:
    const CustomEventPriority priority;
};




#endif // CUSTOM_EVENT_H
