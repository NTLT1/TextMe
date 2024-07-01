#ifndef SQLCONVERSATIONMODEL_H
#define SQLCONVERSATIONMODEL_H
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>

#include "sqlcontactmodel.h"
#include "sqlloginmodel.h"
#include <QSqlTableModel>

class SqlConversationModel : public QSqlTableModel
{
    SqlContactModel* contactModel;
    sqlloginmodel* loginModel;
    Q_OBJECT
    QTcpServer m_server;
    QList<QTcpSocket *> m_sockets;
    QString m_recipient;
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:
    explicit SqlConversationModel(QObject *parent = 0);
    Q_INVOKABLE void createTable(const QString& tableName);
    QString recipient() const;
    void setRecipient(const QString &recipient);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE void sendMessage(const QString &recipient, const QString &message);
    Q_INVOKABLE QString getTableName(const QString &userLogin, const QString &contactLogin);

signals:
    void recipientChanged();
    void newMessageAdded();
private slots:
    //void onNewConnection();
    //void onReadyRead();
};

/*class ChatServer : public QObject
{
    Q_OBJECT

public:
    ChatServer(int port);

signals:
    void newMessageReceived(const QString &message);

private slots:
    void newConnection();
    void readyRead();
    void disconnected();

private:
    QTcpServer m_server;
    QHash<QTcpSocket*, QByteArray> m_buffer;
    QSocketNotifier *m_notifier;
};*/

#endif
