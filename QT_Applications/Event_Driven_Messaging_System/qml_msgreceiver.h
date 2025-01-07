#ifndef QML_MSGRECEIVER_H
#define QML_MSGRECEIVER_H

#include <QObject>
#include <QString>
#include <QDebug>

class QML_MsgReceiver : public QObject {
    Q_OBJECT

public:
    explicit QML_MsgReceiver(QObject* parent = nullptr);

public slots:
    void handleMessage(const QString& msg);

signals:
    void newMessage(const QString& msg);
};


#endif // MSGRECEIVER_
