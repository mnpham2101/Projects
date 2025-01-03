#include "publisher.h"

// Publisher::Publisher(QObject *parent)
//     : QObject{parent}
// {}

void Publisher::registerSubscriber(std::shared_ptr<ISubscriber>& subscriber){
    if (subscriber)
        subscriberList.append(subscriber);
}

void Publisher::unregisterSubscriber(std::shared_ptr<ISubscriber>& subscriber){
    auto it = std::remove_if(subscriberList.begin(), subscriberList.end(),
                             [subscriber](const std::weak_ptr<ISubscriber> localSubscriber)
                             {
                                 auto sub = localSubscriber.lock();
                                 return sub && sub == subscriber;
                             });
    subscriberList.erase(it, subscriberList.end());
    //TODO: caller must manage subscriber lifecycle. It is assumed that the subscriber lifecycle can still retained after this function return.
    //TODO: in case of exception or deallocation at subscriber side, this function should be called automatically to prevent Lapse Listener Problem
}

void Publisher::notifyAllSubscriber(std::unique_ptr<QEvent>& event){
    // check if the subscriber is alive and post event
    // TODO: subscribers could be dead in between any of the lines, but Publisher doesn't have ownership. Should we care?
    for (auto& subscriber : subscriberList) {
        if (!subscriber.expired())
            QCoreApplication::postEvent(subscriber.lock().get(), event.get());
        else{
            // subscriberList.removeOne(subscriber);
        }
    }
}

void Publisher::notifySingleSubcriber(std::shared_ptr<ISubscriber>& subscriber, std::unique_ptr<QEvent>& event){
    // check if the subscriber is registered, alive and post event
    // TODO: subscribers could be dead in between any of the lines, but Publisher doesn't have ownership. Should we care?
    QCoreApplication::postEvent(subscriber.get(), event.get());
}

