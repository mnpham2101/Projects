#ifndef I_SUBSCRIBER_H
#define I_SUBSCRIBER_H

#include <QCoreApplication>
#include <QObject>
#include <custom_event.h>
class ISubscriber :public QObject {
public:
    ISubscriber(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~ISubscriber() = default;
    virtual void handleMessage (std::unique_ptr<CustomEvent>& event) = 0;
};

#endif // I_SUBSCRIBER_H
