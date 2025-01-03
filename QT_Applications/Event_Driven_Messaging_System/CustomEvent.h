#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <Qt>
#include <QEvent>

/*
 *
 * */
class CustomEvent : public QEvent {
    enum CustomEventPriority
    {
        // An important event
        ImportantEventPriority = Qt::HighEventPriority,        
        // A more important event
        MoreImportantEventPriority = ImportantEventPriority + 1,
        // A critical event
        CriticalEventPriority = 100 * MoreImportantEventPriority,
        // Not that important
        StatusEventPriority = Qt::LowEventPriority,
        // These are less important than Status events
        IdleProcessingDoneEventPriority = StatusEventPriority - 1
    };
    const CustomEventPriority priority;
public:
    CustomEvent (QEvent::Type CustomEventType, CustomEvent::CustomEventPriority CustomPriority)
        : QEvent(CustomEventType)
        , priority(CustomPriority){}
    CustomEventPriority getPriority() const { return priority;}
};




#endif // CUSTOMEVENT_H
