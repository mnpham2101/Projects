#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <QObject>
#include <i_publisher.h>

class Publisher : public IPublisher
{
    Q_OBJECT
    QList<std::weak_ptr<ISubscriber>> subscriberList;
public:
    explicit Publisher(QObject *parent = nullptr) : IPublisher(parent){} ;
    /*
     * Adds subscriber to QList, invoked by client.
     * Not thread safe: subscriber can go dead while subscriber is registered. Client must invoke unregisterSubscriber
     * Doesnt have ownership over subscriber.
     * */
    void registerSubscriber(std::shared_ptr<ISubscriber>& subscriber) override;
    /*
     * Removes subscriber from QList, invoked by client
     * Dont care race condition.
     * Client manages the subscriber ownership. Subscriber could be alive after being removed from subscriberList.
     * This function must be invoked if subscriber is dead
     * */
    void unregisterSubscriber(std::shared_ptr<ISubscriber>& subscriber) override;
    /*
     * postEvent that a subscriber listens to. The event is high priority, broadcast event.
     * Check if the subscriber/event is alive before postEvent
     * Should we care Race condition?? */
    void notifyAllSubscriber(std::unique_ptr<QEvent>& event) override;
    /*
     * postEvent that a subscriber listens to. The event isof any priority, unicast event.
     * Check if the subscriber/event is alive before postEvent
     * Should we care Race condition?? */
    void notifySingleSubcriber(std::shared_ptr<ISubscriber>& subscriber, std::unique_ptr<QEvent>& event) override;
};

#endif // PUBLISHER_H
