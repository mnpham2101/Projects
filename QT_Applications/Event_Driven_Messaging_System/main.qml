import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "QML Interface"
    signal buttonPressed(int buttonID, string text)
    Column {
        spacing: 20
        anchors.fill: parent

        // Text input box
        TextField {
            id: inputField
            width: parent.width
            placeholderText: "Enter subscriber ID"
        }

        // Button to send message
        Button {
            id: subscriberMsgSendBtn
            text: "Call subscriber"
            onClicked: {
                userID = inputField.text
                inputField.clear()
            }
        }

        Button {
            id: broadcastToAll
            text: "Call All "
            onClicked: {
                userID = inputField.text
                // msgSender.sendMessage(inputField.text);
                buttonPressed(id, userID)
                inputField.clear()
            }
        }

        Button {
            id: broadcastToSome
            text: "Call Group "
            onClicked: {
                userID = inputField.text
                buttonPressed(id, userID)
                inputField.clear()
            }
        }

        // Connections {
        //     target: msgReceiver
        //     onNewMessage: {
        //         outputArea.text += msg + "\n";
        //     }
        // }

        // // Label for "log"
        // Label {
        //     text: "Log"
        //     font.pixelSize: 20
        // }

        // // Text output box
        // TextArea {
        //     id: outputArea
        //     width: parent.width
        //     height: parent.height * 0.4
        //     readOnly: true
        //     wrapMode: Text.Wrap
        // }
    }
}
