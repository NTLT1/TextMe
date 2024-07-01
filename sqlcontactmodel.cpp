#include "sqlcontactmodel.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>
QString SqlContactModel::currentContactLogin;
SqlContactModel::SqlContactModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QString userLogin=loginModel->getCurrentUserLogin();
    QString tableName = userLogin + "Contacts";
    QSqlQuery query;
    createTable(userLogin);
    if (!query.exec("SELECT * FROM " + tableName))
        qFatal("Contacts SELECT query failed: %s", qPrintable(query.lastError().text()));
    setQuery(std::move(query));
    if (lastError().isValid())
        qFatal("Cannot set query on SqlContactModel: %s", qPrintable(lastError().text()));
}
void SqlContactModel::createTable(const QString &login)
{
    QSqlQuery query;
    QString tableName = login + "Contacts";

    if (QSqlDatabase::database().tables().contains(tableName)) {
        return;
    }
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS " + tableName +
            " ("
            "   'name' TEXT NOT NULL,"
            "   login TEXT NOT NULL,"
            "   PRIMARY KEY(name, login)"
            ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

void SqlContactModel::addContact(const QString &login, const QString &name, const QString &userLogin)
{
    QSqlQuery query;
    QString tableName = userLogin + "Contacts";
    if (!query.exec("INSERT INTO " + tableName + " (name, login) VALUES ('" + name + "', '" + login + "')"))
    {
        qFatal("Contacts INSERT query failed: %s", qPrintable(query.lastError().text()));
    }
    if (!query.exec("SELECT * FROM " + tableName))
        qFatal("Contacts SELECT query failed: %s", qPrintable(query.lastError().text()));
    setQuery(std::move(query));
    if (lastError().isValid())
        qFatal("Cannot set query on SqlContactModel: %s", qPrintable(lastError().text()));
}
void SqlContactModel::setContactLogin(int row)
{
    currentContactLogin = data(index(row, 1)).toString();
}
QString SqlContactModel::getContactLogin()
{
    return currentContactLogin;
}
void SqlContactModel::deleteContact(const QString &login, const QString &userLogin)
{
    QSqlQuery query;
    QString tableName = userLogin + "Contacts";
    if (!query.exec("DELETE FROM " + tableName + " WHERE login = '" + login + "'" ))
    {
        qFatal("Contacts DELETE query failed: %s", qPrintable(query.lastError().text()));
    }
    if (!query.exec("SELECT * FROM " + tableName))
        qFatal("Contacts SELECT query failed: %s", qPrintable(query.lastError().text()));
    setQuery(std::move(query));
    if (lastError().isValid())
        qFatal("Cannot set query on SqlContactModel: %s", qPrintable(lastError().text()));
}
