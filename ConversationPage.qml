import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import io.qt.TextMe

Page {
    id: root

    property string inConversationWith
    SqlloginModel {
        id: sqlloginModel
    }
    property string userName: sqlloginModel.getNameByLogin(sqlloginModel.getCurrentUserLogin())
    header: ToolBar {
        ToolButton {
            text: qsTr("Back")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.pop()
        }

        Label {
            id: pageTitle
            text: inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    ColumnLayout {
        anchors.fill: parent
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
            verticalLayoutDirection: ListView.BottomToTop
            spacing: 12
            model: SqlConversationModel {
                recipient: inConversationWith
            }
            delegate: Column {
                anchors.right: sentByMe ? listView.contentItem.right : undefined
                spacing: 6
                readonly property bool sentByMe: model.recipient !== userName

                Row {
                    id: messageRow
                    spacing: 6
                    anchors.right: sentByMe ? parent.right : undefined

                    /*Label {
                        id: senderLabel
                        text: sentByMe ? userName : inConversationWith
                        color: sentByMe ? "lightgrey" : "steelblue"
                        anchors.baseline: senderLabel.anchors.baseline = messageRow.baseline
                        font.italic: true
                    }*/

                    Rectangle {
                        width: Math.min(messageText.implicitWidth + 24,listView.width)
                        height: messageText.implicitHeight + 24
                        color: sentByMe ? "lightgrey" : "steelblue"

                        Label {
                            id: messageText
                            text: model.message
                            color: sentByMe ? "black" : "white"
                            anchors.fill: parent
                            anchors.margins: 12
                            wrapMode: Label.Wrap
                        }
                    }
                }
                Label {
                    id: timestampText
                    text: Qt.formatDateTime(model.timestamp, "d MMM hh:mm")
                    color: "lightgrey"
                    anchors.right: sentByMe ? parent.right : undefined
                }
            }
            ScrollBar.vertical: ScrollBar {}
        }
       Pane {
           id: pane
           Layout.fillWidth: true

           RowLayout {
               width: parent.width
               TextArea {
                   id: messageField
                   Layout.fillWidth: true
                   placeholderText: qsTr("Compose message")
                   wrapMode: TextArea.Wrap
               }

               Button {
                    id: sendButton
                    text: qsTr("Send")
                    enabled: messageField.length > 0
                    onClicked: {
                        listView.model.sendMessage(inConversationWith, messageField.text);
                         messageField.text = "";
                    }
                }
            }
        }
    }
}
