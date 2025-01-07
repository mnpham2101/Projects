#ifndef QML_MSGSENDER_H
#define QML_MSGSENDER_H

#include <QObject>
#include <QString>

class QML_MsgSender : public QObject {
    Q_OBJECT

public:
    explicit QML_MsgSender(QObject* parent = nullptr);

signals:
    void sendMessage(const QString& msg, int itemID);

public slots:
    // void sendPriorityMessage(const QString& msg);
};

#endif // QML_MSGSENDER_H
