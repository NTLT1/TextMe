import QtQuick
import QtQuick.Controls
import Qt.labs.folderlistmodel 1.0
import io.qt.TextMe

Page
{
    SqlloginModel {
        id: sqlloginModel
    }
    SqlContactModel{
        id: sqlcontactModel
    }
    SqlConversationModel{
        id: sqlconversationModel
    }

    function addContactTo(){
        var login = loginTextField.text;
        if(login === "")
        {
            addContactPopup.errorMessage = "Field must be filled";
            return;
        }
        if(!sqlloginModel.isUserExist(login))
        {
            addContactPopup.errorMessage = "User with login dont exsist";
            return;
        }
        var name=sqlloginModel.getNameByLogin(login)
        var UserLogin = sqlloginModel.getCurrentUserLogin();
        sqlcontactModel.addContact(login, name, UserLogin)

        addContactPopup.errorMessage = ""
        addContactPopup.close()
    }
    id: root
    header: ToolBar {
            Label {
                text: qsTr("Contacts")
                font.pixelSize: 20
                anchors.centerIn: parent
            }
            ToolButton {
                text: qsTr("LogOut")
                font.pixelSize: 15
                anchors.right: parent.right
                onClicked: {
                    sqlloginModel.logOut(sqlloginModel.getCurrentUserLogin())
                    root.StackView.view.pop()
                }
            }
            ToolButton {
                property string userName: sqlloginModel.getNameByLogin(sqlloginModel.getCurrentUserLogin())
                text: userName
                font.pixelSize: 15
                anchors.left: parent.left
                onClicked: {
                }
            }
        }
    ListView
    {
        id: listView
        anchors.fill: parent
        topMargin: 48
        leftMargin: 48
        bottomMargin: 48
        rightMargin: 48
        spacing: 20
        model: sqlcontactModel
        delegate: ItemDelegate
        {
            text: model.display
            width: ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin
            height: 50
            onClicked: {
                sqlcontactModel.setContactLogin(model.index)
                root.StackView.view.push("ConversationPage.qml", { inConversationWith: model.display })
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        contextMenu.popup(mouse.x, mouse.y)
                    }
                }
            }

            Menu {
                id: contextMenu
                MenuItem {
                    text: qsTr("Delete contact")
                    onTriggered: {
                        sqlcontactModel.setContactLogin(model.index)
                        var contactLogin = sqlcontactModel.getContactLogin()
                        var userLogin = sqlloginModel.getCurrentUserLogin()

                        sqlcontactModel.deleteContact(contactLogin, userLogin)
                    }
                }
            }
        }
        Button
            {
                id: addContactButton
                text: qsTr("+")
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    addContactPopup.open()
                }
            }
        Popup {
            id: addContactPopup
            x: (root.width - width) / 2
            y: (root.height - height) / 2
            width: 200
            height: 200
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            property string errorMessage: ""
            Column {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                TextField {
                    id: loginTextField
                    placeholderText: qsTr("Enter Login")
                }
                Button {
                    text: qsTr("Add Contact")
                    onClicked: {
                        addContactTo();
                        }
                    }
                Text {
                    color: "red"
                    text: addContactPopup.errorMessage
                    visible: addContactPopup.errorMessage !== ""
                }
            }
        }
    }
}
