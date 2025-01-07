#ifndef I_PUBLISHER_H
#define I_PUBLISHER_H

#include <QCoreApplication>
#include <QObject>
#include <i_subscriber.h>

class IPublisher : public QObject{
    public:
    IPublisher(QObject * parent = nullptr) : QObject(parent) {};
    virtual ~IPublisher() = default;
    virtual void registerSubscriber(std::shared_ptr<ISubscriber>& subscriber) = 0;
    virtual void unregisterSubscriber(std::shared_ptr<ISubscriber>& subscriber) = 0;
    virtual void notifyAllSubscriber(std::unique_ptr<CustomEvent>& event) = 0;
    virtual void notifySingleSubcriber(std::shared_ptr<ISubscriber>& subscriber, std::unique_ptr<CustomEvent>& event) = 0;
};
#endif // I_PUBLISHER_H
