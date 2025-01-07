#ifndef SUBSCRIBER_MANAGER_H
#define SUBSCRIBER_MANAGER_H

#include <QObject>
#include <mutex>
#include <condition_variable>
#include <deque>

class SubscriberManager
{
    std::mutex mtx;
    std::condition_variable cv;
    std::deque<int> taskQueue;
    int subscriberCount = 0;
    const int maxSubscriber = 10000;
    const int maxThreads = 1000;
public:
    /*
     * Singleton
     * */
    SubscriberManager();
    /*
     * Spawn new subscriber, increments ID whenever invoked, calls to register subscriber
     * Runs subscriber for time 300s, then closes the subscriber, calls Publisher to unregister subscriber
     * TODO: handles exception, calls Publisher to unregisters subscriber, decrements ID
     * */
    void admitSubscriber();
    /*
     * Calls admitSubscriber, per available threads.
     * Notify all other waiting threads after admitSubscriber returns.
     * */
    void workerThread();
};

#endif // SUBSCRIBER_MANAGER_H
