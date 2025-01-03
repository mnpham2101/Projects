#ifndef MSGSENDER_H
#define MSGSENDER_H

#include <QObject>
#include <QString>

class MsgSender : public QObject {
    Q_OBJECT

public:
    explicit MsgSender(QObject* parent = nullptr);

signals:
    void sendMessage(const QString& msg);

public slots:
    void sendPriorityMessage(const QString& msg);
};

#endif // MSGSENDER_H
