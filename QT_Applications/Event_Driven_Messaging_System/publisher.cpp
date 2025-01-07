#include "publisher.h"

Publisher* Publisher::instance = nullptr;

void Publisher::registerSubscriber(std::shared_ptr<ISubscriber>& subscriber){
    if (subscriber)
        mSubscriberList.append(subscriber);
}

void Publisher::unregisterSubscriber(std::shared_ptr<ISubscriber>& subscriber){
    std::lock_guard<std::mutex> lock(mObserverMutex);
    auto it = std::remove_if(mSubscriberList.begin(), mSubscriberList.end(),
                             [subscriber](const std::weak_ptr<ISubscriber> localSubscriber)
                             {
                                 auto sub = localSubscriber.lock();
                                 return sub && sub == subscriber;
                             });
    mSubscriberList.erase(it, mSubscriberList.end());
    //TODO: caller must manage subscriber lifecycle. It is assumed that the subscriber lifecycle can still retained after this function return.
    //TODO: in case of exception or deallocation at subscriber side, this function should be called automatically to prevent Lapse Listener Problem
}

void Publisher::notifyAllSubscriber(std::unique_ptr<CustomEvent>& event){

    std::lock_guard<std::mutex> lock(mObserverMutex);
    // check if the subscriber is alive and post event
    // TODO: use QT 6.1 remove_if which is more concise
    auto it = std::remove_if(mSubscriberList.begin(), mSubscriberList.end(),
                             [](const std::weak_ptr<ISubscriber> localSubscriber)
                             {
                                 return localSubscriber.expired();
                             });
    mSubscriberList.erase(it, mSubscriberList.end());
    // TODO: check if event is still alive
    std::for_each( mSubscriberList.begin(), mSubscriberList.end(),
                    [&]( const std::weak_ptr<ISubscriber>& subscriber)
                    {
                        auto subscriberSharedPtr = subscriber.lock();
                        if (subscriberSharedPtr){
                            QCoreApplication::postEvent(subscriberSharedPtr.get(), event.get());
                        }
                    });

}

void Publisher::notifySingleSubcriber(std::shared_ptr<ISubscriber>& subscriber, std::unique_ptr<CustomEvent>& event){
    // check if the subscriber is alive and post event
    auto it = std::remove_if(mSubscriberList.begin(), mSubscriberList.end(),
                             [](const std::weak_ptr<ISubscriber> localSubscriber)
                             {
                                 return localSubscriber.expired();
                             });
    mSubscriberList.erase(it, mSubscriberList.end());
    QCoreApplication::postEvent(subscriber.get(), event.get());
}

