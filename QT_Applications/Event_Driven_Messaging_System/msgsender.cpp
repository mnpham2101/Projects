#include "msgsender.h"

MsgSender::MsgSender(QObject* parent) : QObject(parent) {}

void MsgSender::sendPriorityMessage(const QString& msg) {
    emit sendMessage(msg);
}
