#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <i_subscriber.h>

class Subscriber : public ISubscriber
{
    Q_OBJECT
public:
    explicit Subscriber(QObject *parent = nullptr) : ISubscriber(parent) { };
    void handleMessage(std::unique_ptr<QEvent>& event);

};

#endif // SUBSCRIBER_H
