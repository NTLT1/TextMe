import QtQuick
import QtQuick.Controls
import Qt.labs.folderlistmodel 1.0
import io.qt.TextMe 
Page
{
    SqlloginModel {
        id: sqlloginModel
    }
    function saveToDatabase()
    {
        var name = registerNameTextField.text;
        var login = registerLoginTextField.text;
        var password = registerPasswordTextField.text;
        if (name === "" || login === "" || password === "")
        {
            registerPopup.errorMessage = "All fields must be filled"
            return;
        }
        if(sqlloginModel.isUserExist(login))
        {
            registerPopup.errorMessage = "User with this login already exists"
            return;
        }
        sqlloginModel.addUser(name, login, password);
        registerPopup.errorMessage = ""
        registerPopup.close()
    }
    function logIn(){
        var login=loginTextField.text;
        var password=passwordTextField.text;
        if(sqlloginModel.checkLoginAndPassword(login, password))
        {
            sqlloginModel.setCurrentUserLogin(login)
            sqlloginModel.setUserStatus(login)
            errorMessage.text = ""
            return true;
        }
        else{
            return false;
        }
    }

    id: loginPage
    header: ToolBar {
            Label {
                text: qsTr("Login")
                font.pixelSize: 20
                anchors.centerIn: parent
            }
        }
    Column {
        spacing: 20
        padding: 20
        anchors.centerIn: parent
        Text {
            id: errorMessage
            color: "red"
            text: ""
            visible: text !== ""
        }
        TextField {
            id: loginTextField
            placeholderText: qsTr("Enter login")
        }

        TextField {
            id: passwordTextField
            placeholderText: qsTr("Enter password")
            echoMode: TextInput.Password
        }

        Button {
            text: qsTr("Login")
            onClicked: {
                if(logIn())
                {
                    loginPage.StackView.view.push("ContactPage.qml")
                }
                else{
                    errorMessage.text = "Login or password is incorrect"
                }
            }
        }

        Button {
            id: registerButton
            text: qsTr("Register")
            onClicked: {
                registerPopup.open()
            }
        }
    }

    Popup {
        id: registerPopup
        x: (loginPage.width - width) / 2
        y: (loginPage.height - height) / 2
        width: Math.min(loginPage.width, loginPage.height) / 5 * 3
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        property string errorMessage: ""
        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20
            TextField {
                id: registerNameTextField
                width: parent.width
                height: 40
                placeholderText: qsTr("Enter Name")
            }
            TextField {
                id: registerLoginTextField
                property string textField: ""
                width: parent.width
                height: 40
                placeholderText: qsTr("Enter login")
            }

            TextField {
                id: registerPasswordTextField
                width: parent.width
                height: 40
                placeholderText: qsTr("Enter password")
                echoMode: TextInput.Password
            }

            Button {
                width: parent.width
                height: 50
                text: qsTr("Register")
                onClicked: {
                    saveToDatabase()
                    }
                }
            Text {
                color: "red"
                text: registerPopup.errorMessage
                visible: registerPopup.errorMessage !== ""
            }
        }
    }
}

