#include "subscriber.h"
#include "QDebug"

int Subscriber::subscriberID =0;

void Subscriber::handleMessage(std::unique_ptr<CustomEvent>& event){
    qDebug() << " SubscriberID: " << this->subscriberID;
    qDebug() << " Receive event Priority: " << event->getPriority() <<"\n";
}
