import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "QML Interface"

    Column {
        spacing: 20
        anchors.fill: parent

        // Label for "send priority msg"
        Label {
            text: "Send Priority Msg"
            font.pixelSize: 20
        }

        // Text input box
        TextField {
            id: inputField
            width: parent.width
            placeholderText: "Enter your message"
        }

        // Button to send message
        Button {
            text: "Send"
            onClicked: {
                msgSender.sendPriorityMessage(inputField.text);
                inputField.clear();
            }
        }

        Connections {
            target: msgReceiver
            onNewMessage: {
                outputArea.text += msg + "\n";
            }
        }

        // Label for "log"
        Label {
            text: "Log"
            font.pixelSize: 20
        }

        // Text output box
        TextArea {
            id: outputArea
            width: parent.width
            height: parent.height * 0.4
            readOnly: true
            wrapMode: Text.Wrap
        }
    }
}
