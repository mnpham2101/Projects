#include "qml_msgreceiver.h"

QML_MsgReceiver::QML_MsgReceiver(QObject* parent) : QObject(parent) {};

void QML_MsgReceiver::handleMessage(const QString& msg) {
    emit newMessage(msg);
};

