#ifndef SQLCONTACTMODEL_H
#define SQLCONTACTMODEL_H
#include "qsqlquerymodel.h"
#include "sqlloginmodel.h"
#include <QSqlQueryModel>

class SqlContactModel : public QSqlQueryModel
{
    Q_OBJECT
    sqlloginmodel *loginModel;
    static QString currentContactLogin;
public:
    SqlContactModel(QObject *parent = nullptr);
    void createTable(const QString &login);
    Q_INVOKABLE void addContact(const QString &login, const QString &name, const QString &userLogin);
    Q_INVOKABLE void setContactLogin(int row);
    Q_INVOKABLE QString getContactLogin();
    Q_INVOKABLE void deleteContact(const QString &login, const QString &userLogin);
};

#endif
