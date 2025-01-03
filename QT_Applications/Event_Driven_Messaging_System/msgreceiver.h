#ifndef MSGRECEIVER_H
#define MSGRECEIVER_H

#include <QObject>
#include <QString>
#include <QDebug>

class MsgReceiver : public QObject {
    Q_OBJECT

public:
    explicit MsgReceiver(QObject* parent = nullptr);

public slots:
    void handleMessage(const QString& msg);

signals:
    void newMessage(const QString& msg);
};


#endif // MSGRECEIVER_
