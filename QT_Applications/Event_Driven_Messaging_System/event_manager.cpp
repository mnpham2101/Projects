#include "event_manager.h"
#include "publisher.h"
#include "subscriber.h"

EventManager* EventManager::instance = nullptr;

void EventManager::handleBtnPress(int btnId, QString userID){
    CustomEvent::CustomEventPriority eventPriority;
    QEvent::Type eventType;
    switch (btnId) {
        case 1:     // broadcast event for all subscriber
            eventPriority = CustomEvent::CustomEventPriority::CriticalEventPriority;
            eventType = static_cast<QEvent::Type>(QEvent::User + 1);
            break;
        case 2:     // important event for some subscribers
            eventPriority = CustomEvent::CustomEventPriority::ImportantEventPriority;
            eventType = static_cast<QEvent::Type>(QEvent::User + 2);
            break;
        case 3:     // event per subscriber
            eventPriority = CustomEvent::CustomEventPriority::StatusEventPriority;
            eventType = static_cast<QEvent::Type>(QEvent::User + 3);
            break;
        case 4:
            eventPriority = CustomEvent::CustomEventPriority::IdleProcessingDoneEventPriority;

            eventType             = static_cast<QEvent::Type>(QEvent::User + userID.toInt());
            break;
        default: return;
    }
    std::unique_ptr<CustomEvent> event = std::make_unique<CustomEvent>(CustomEvent(eventType, eventPriority));
    if (eventType == static_cast<QEvent::Type>(QEvent::User + 1))
        Publisher::getInstance()->notifyAllSubscriber(event);
    else{
        // TODO: get subscriber
        // Publisher::getInstance()->notifySingleSubcriber(subscriber, event);
    }
}
