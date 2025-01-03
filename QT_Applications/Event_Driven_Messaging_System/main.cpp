#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQmlContext>
#include <QThread>
#include "msgsender.h"
#include "msgreceiver.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    MsgSender sender;
    MsgReceiver receiver;
    QThread receiverThread;
    receiver.moveToThread(&receiverThread);
    QObject::connect(&sender, &MsgSender::sendMessage, &receiver, &MsgReceiver::handleMessage);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &receiverThread, &QThread::quit);
    QObject::connect(&receiverThread, &QThread::finished, &receiver, &QObject::deleteLater);
    receiverThread.start();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // Expose MsgSender and MsgReceiver to QML
    engine.rootContext()->setContextProperty("msgSender", &sender);
    engine.rootContext()->setContextProperty("msgReceiver", &receiver);

    return app.exec();
}
