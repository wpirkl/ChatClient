import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.0


Page {
    id: page
    Connections{
        target: uiManager

        onTextReceived: {
            // console.log("appending: " + message);
            chatHistory.append(message);
        }
    }

    function sendMessage() {
        uiManager.onSendClicked(textInput.text);
        textInput.text = "";
    }

    GridLayout {
        id: rowLayout
        rows: 2
        columns: 2
        anchors.fill: parent

        ScrollView {
            id: scrollView
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentItem: TextArea {
                id: chatHistory

                readOnly: true

                font.pointSize: 16
            }
        }

        TextField {
            id: textInput

            Layout.fillWidth: true
            Layout.leftMargin: 50
            Layout.rightMargin: 50
            Layout.minimumWidth: 100

            placeholderText: qsTr("text...")
            focus: true
            font.pointSize: 16
            Layout.alignment: Qt.AlignVCenter

            onAccepted: sendMessage()
        }

        Button {
            id: send
            flat: true
            text: qsTr("Send")
            Layout.columnSpan: 1
            Layout.alignment: Qt.AlignVCenter
            font.pointSize: 16
            onClicked: sendMessage()
        }
    }

    Button {
        id: disconnectButton
        text: 'X'
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        font.pointSize: 16
        flat: true
        onClicked: {
            uiManager.onDisconnectClicked()
        }
    }
}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:7;anchors_x:4;anchors_y:2}
}
 ##^##*/
