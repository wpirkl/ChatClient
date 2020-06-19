import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: mainWindow

    visible: true
    width: 640
    height: 480
    title: qsTr("Chat Client") + " [" + username.text + "]"
    Material.theme: Material.Dark
    Material.accent: Material.Orange

    function connectClicked()
    {
        uiManager.onConnectToServerClicked(username.text, serverId.text, serverMode.checked)
    }

    onClosing: {
        uiManager.closing();
    }

    Connections{
        target: uiManager

        onConnected: {
            // console.log("connected");

            if(pageLoader.source == "qrc:/chat.qml") {  // it's already loaded
                stackView.push(pageLoader.item)
            } else {
                pageLoader.source = "qrc:/chat.qml"
            }
        }

        onDisconnected: {
            // console.log("disconnected");
            stackView.pop();
        }

        onError: {
            // console.log("error");
            if(message) {
                infoText.visible = true;
            } else {
                infoText.visible = false;
            }
            infoText.text = message;
        }
    }

    Loader {
        id: pageLoader
        objectName: "loader"

        asynchronous: false
        visible: false

        onLoaded: {
            stackView.push(pageLoader.item)
        }
    }

    StackView {
        id: stackView
        objectName: "stackView"

        anchors.fill: parent

        onCurrentItemChanged: {
            currentItem.forceActiveFocus()
        }

        initialItem: Page {
            id: connectPage

            RowLayout {
                spacing: 5
                anchors.fill: parent

                GridLayout {
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    columns: 2
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    Label {
                        text: qsTr("Username:")
                        font.pointSize: 16
                    }

                    TextField {
                        id: username

                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50
                        Layout.minimumWidth: 100

                        placeholderText: qsTr("Username")
                        focus: true
                        font.pointSize: 16

                        onAccepted: connectClicked()
                    }

                    Label {
                        text: qsTr("Server Address:")
                        font.pointSize: 16
                    }

                    TextField {
                        id: serverId

                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.leftMargin: 50
                        Layout.rightMargin: 50
                        Layout.minimumWidth: 100

                        placeholderText: qsTr("Server Address")
                        font.pointSize: 16

                        text: "localhost"

                        onAccepted: connectClicked()
                    }


                    CheckBox {
                        id: serverMode
                        text: qsTr("Server")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 2
                        font.pointSize: 16
                    }

                    Button {
                        id: connect
                        flat: true
                        text: qsTr("Connect")
                        Layout.columnSpan: 2
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        onClicked: {
                            uiManager.onConnectToServerClicked(username.text, serverId.text, serverMode.checked)
                        }
                        font.pointSize: 16
                    }

                    Text {
                        id: infoText
                        objectName: "infoText"

                        visible: false
                        color: "#ff0000"
                        text: qsTr("Info Text")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.columnSpan: 2
                        font.pointSize: 20
                    }

                }
            }
        }
    }
}
