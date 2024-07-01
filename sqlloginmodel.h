#ifndef SQLLOGINMODEL_H
#define SQLLOGINMODEL_H

#include <QSqlTableModel>
#include <QSqlQueryModel>

class sqlloginmodel : public QSqlQueryModel
{
    Q_OBJECT
    static QString currentUserLogin;
public:
    sqlloginmodel(QObject *parent = nullptr);
    void createTable();
    Q_INVOKABLE void addUser(const QString &name, const QString &login, const QString &password);
    Q_INVOKABLE bool isUserExist(const QString &login);
    Q_INVOKABLE bool checkLoginAndPassword(const QString &login, const QString &password);
    Q_INVOKABLE QString getNameByLogin(const QString &login);
    Q_INVOKABLE QString getCurrentUserLogin();
    Q_INVOKABLE void setCurrentUserLogin(const QString &login);
    Q_INVOKABLE void setUserStatus(const QString &login);
    Q_INVOKABLE void logOut(const QString &login);
};

#endif
