#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <QObject>
#include <custom_event.h>

class EventManager : public QObject
{
private:
    Q_OBJECT
    /* Singleton
     * Defines slot functions to handle signal from GUI
     * */
    EventManager(){};
    static EventManager * instance;
public:

    static EventManager* getInstance(){
        if (!instance){
            instance = new EventManager();
        }
        return instance;
    }

public slots:
    void handleBtnPress (int buttonID, QString userID);

private:
    std::unique_ptr<CustomEvent> createCustomEvent (QEvent::Type customEventType, CustomEvent::CustomEventPriority type) {
        return std::make_unique<CustomEvent>(customEventType, type);
    };
};

#endif // EVENT_MANAGER_H
