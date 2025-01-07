QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        event_manager.cpp \
        main.cpp \
        publisher.cpp \
        qml_msgreceiver.cpp \
        qml_msgsender.cpp \
        subscriber.cpp \
        subscriber_manager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    custom_event.h \
    event_manager.h \
    i_publisher.h \
    i_subscriber.h \
    publisher.h \
    qml_msgreceiver.h \
    qml_msgsender.h \
    subscriber.h \
    subscriber_manager.h
