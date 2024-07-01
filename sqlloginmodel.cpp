#include "sqlloginmodel.h"
#include "qcryptographichash.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
QString sqlloginmodel::currentUserLogin;
void sqlloginmodel::createTable()
{
    if (QSqlDatabase::database().tables().contains(QStringLiteral("Users"))) {
        return;
    }

    QSqlQuery query;
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS 'Users' ("
            "   'id' INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   'name' TEXT NOT NULL,"
            "   'login' TEXT NOT NULL,"
            "   'password' TEXT NOT NULL,"
            "   'status' TEXT NOT NULL DEFAULT 'offline'"
            ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

sqlloginmodel::sqlloginmodel(QObject *parent) :
    QSqlQueryModel(parent)
{
    createTable();
    QSqlQuery query;
    if (!query.exec("SELECT * FROM Users"))
        qFatal("Users SELECT query failed: %s", qPrintable(query.lastError().text()));

    setQuery(query);
    if (lastError().isValid())
        qFatal("Users SELECT query failed: %s", qPrintable(lastError().text()));
}

void sqlloginmodel::addUser(const QString &name, const QString &login, const QString &password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashedPassword = hash.result().toHex();

    QSqlQuery query;
    if (!query.exec("INSERT INTO Users (name, login, password, status) VALUES ('" + name + "', '" + login + "', '" + hashedPassword + "', 'offline')"))
        qFatal("Failed to insert user into database: %s", qPrintable(query.lastError().text()));
}

bool sqlloginmodel::checkLoginAndPassword(const QString &login, const QString &password)
{
    QSqlQuery query;
    if (!query.exec("SELECT password FROM Users WHERE login = '" + login + "'"))
        qFatal("Failed to authenticate user: %s", qPrintable(query.lastError().text()));

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(password.toUtf8());
        QString hashedPassword = hash.result().toHex();

        return storedPassword == hashedPassword;
    } else {
        return false;
    }
}

bool sqlloginmodel::isUserExist(const QString &login)
{
    QSqlQuery query;
    if (!query.exec("SELECT * FROM Users WHERE login = '" + login + "'"))
        qFatal("Failed to check user existence: %s", qPrintable(query.lastError().text()));

    return query.next();
}

QString sqlloginmodel::getNameByLogin(const QString &login)
{
    QSqlQuery query;
    if (!query.exec("SELECT name FROM Users WHERE login = '" + login + "'"))
        qFatal("Failed to find user by login: %s", qPrintable(query.lastError().text()));

    if (query.next()) {
        return query.value(0).toString();
    } else {
        return QString();
    }
}

QString sqlloginmodel::getCurrentUserLogin()
{
    return currentUserLogin;
}

void sqlloginmodel::setCurrentUserLogin(const QString &login){
    currentUserLogin=login;
}

void sqlloginmodel::setUserStatus(const QString &login){
    QSqlQuery query;
    if (!query.exec("UPDATE Users SET status = 'online' WHERE login = '" + login + "'"))
        qFatal("Failed to set user status: %s", qPrintable(query.lastError().text()));
}

void sqlloginmodel::logOut(const QString &login){
    QSqlQuery query;
    if (!query.exec("UPDATE Users SET status = 'offline' WHERE login = '" + login + "'"))
        qFatal("Failed to set user status: %s", qPrintable(query.lastError().text()));
}
