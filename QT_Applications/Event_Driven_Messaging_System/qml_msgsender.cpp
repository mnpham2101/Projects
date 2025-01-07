#include <qml_msgsender.h>
#include <event_manager.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>

QML_MsgSender::QML_MsgSender(QObject* parent) : QObject(parent) {
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("eventManager", EventManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return;
    QObject *rootObject = engine.rootObjects().first();
    QObject::connect(rootObject, SIGNAL(buttonPressed(int,QString)), EventManager::getInstance(), SLOT(handleBtnPress(int,QString)));
}
