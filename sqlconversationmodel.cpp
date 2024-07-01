#include "sqlconversationmodel.h"

void SqlConversationModel::createTable(const QString& tableName)
{
    if (QSqlDatabase::database().tables().contains(tableName)) {
        return;
    }
    QString contactTableName = loginModel->getCurrentUserLogin() + "Contacts";
    QSqlQuery query;
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS '" + tableName + "' ("
            "'author' TEXT NOT NULL,"
            "'recipient' TEXT NOT NULL,"
            "'timestamp' TEXT NOT NULL,"
            "'message' TEXT NOT NULL,"
            "FOREIGN KEY('author') REFERENCES " + contactTableName + " ( name ),"
            "FOREIGN KEY('recipient') REFERENCES " + contactTableName + " ( name )"
            ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

SqlConversationModel::SqlConversationModel(QObject *parent) :
    QSqlTableModel(parent),
    m_server(this)
{
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    const QString userLogin=loginModel->getCurrentUserLogin();
    const QString contactLogin = contactModel->getContactLogin();
    QString tableName = getTableName(userLogin, contactLogin);
    createTable(tableName);
    setTable(tableName);
    setSort(2, Qt::DescendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QString SqlConversationModel::recipient() const
{
    return m_recipient;
}

void SqlConversationModel::setRecipient(const QString &recipient)
{
    if (recipient == m_recipient)
        return;

    m_recipient = recipient;
    QString UserName = loginModel->getNameByLogin(loginModel->getCurrentUserLogin());
    const QString filterString = QString::fromLatin1(
        "(recipient = '%1' AND author = '%2') OR (recipient = '%2' AND author = '%1')").arg(m_recipient, UserName);
    setFilter(filterString);
    select();

    emit recipientChanged();
}

QVariant SqlConversationModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> SqlConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "author";
    names[Qt::UserRole + 1] = "recipient";
    names[Qt::UserRole + 2] = "timestamp";
    names[Qt::UserRole + 3] = "message";
    return names;
}

void SqlConversationModel::sendMessage(const QString &recipient, const QString &message)
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlRecord newRecord = record();
    QString UserName = loginModel->getNameByLogin(loginModel->getCurrentUserLogin());
    newRecord.setValue("author", UserName);
    newRecord.setValue("recipient", recipient);
    newRecord.setValue("timestamp", timestamp);
    newRecord.setValue("message", message);
    if (!insertRecord(rowCount(), newRecord)) {
        qWarning() << "Failed to send message:" << lastError().text();
        return;
    }
    submitAll();

    // Сигнал об изменении данных для обновления в QML
    emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));

    // Сброс модели для перечитывания данных
   select();
}
QString SqlConversationModel::getTableName(const QString &userLogin, const QString &contactLogin){
    QStringList sortedNames = {userLogin, contactLogin};
    sortedNames.sort();
    return QString("conversation_%1_%2").arg(sortedNames[0], sortedNames[1]);
}

/*void ChatServer::newConnection()
{
    QTcpSocket *socket = m_server.nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::disconnected);
    m_buffer.insert(socket, QByteArray());
}

void ChatServer::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    m_buffer[socket].append(socket->readAll());

    while (m_buffer[socket].contains('\n')) {
        int position = m_buffer[socket].indexOf('\n');
        QString message = QString::fromUtf8(m_buffer[socket].left(position));
        m_buffer[socket] = m_buffer[socket].mid(position + 1);

        emit newMessageReceived(message);
    }
}

void ChatServer::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;
    m_buffer.remove(socket);
    socket->deleteLater();
}
*/
