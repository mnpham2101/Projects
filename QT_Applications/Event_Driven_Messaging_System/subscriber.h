#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <i_subscriber.h>

class Subscriber : public ISubscriber
{
    Q_OBJECT
    static int subscriberID;
public:
    explicit Subscriber(QObject *parent = nullptr) : ISubscriber(parent) { ++subscriberID; };
    void handleMessage(std::unique_ptr<CustomEvent>& event);
};

#endif // SUBSCRIBER_H
