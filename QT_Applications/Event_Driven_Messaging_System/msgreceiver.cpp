#include "msgreceiver.h"

MsgReceiver::MsgReceiver(QObject* parent) : QObject(parent) {};

void MsgReceiver::handleMessage(const QString& msg) {
    emit newMessage(msg);
};

