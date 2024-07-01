#include <QtCore>
#include <QGuiApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtQml>

#include "sqlcontactmodel.h"
#include "sqlconversationmodel.h"
#include "sqlloginmodel.h"

static bool databaseConnected = false;

static void connectToDataBaseOfUsers()
{
    if(databaseConnected)
        return;
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid()) {
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
        }
    }

    const QString fileName = "database.sqlite3";
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }

    databaseConnected = true;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SqlContactModel>("io.qt.TextMe", 1, 0, "SqlContactModel");
    qmlRegisterType<SqlConversationModel>("io.qt.TextMe", 1, 0, "SqlConversationModel");
    qmlRegisterType<sqlloginmodel>("io.qt.TextMe", 1, 0, "SqlloginModel");

    connectToDataBaseOfUsers();
    QQmlApplicationEngine engine;
    engine.load("D:/Qt Projects/TextMe/main.qml");
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
