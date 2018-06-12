#include "QtChatServer.h"

#include <QTcpSocket>
#include <QRegExp>
#include <QSettings>
#include "DbManager.h"

QtChatServer::QtChatServer(QObject *parent) : QTcpServer(parent)
{
    qDebug() << "QtChatServer Copyright 2018\nAuthor: Rafal Olszewski\ngithub.com/T8kme/";
}

void QtChatServer::incomingConnection(qintptr socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void QtChatServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();

    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");
        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users[client] = user;
            foreach(QTcpSocket *client, clients)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
        }
        else if(line == "/getArchive")
        {
            QStringList archiveMessages;
            QSqlQuery query("SELECT date, user, message FROM message");
            int sql_date = query.record().indexOf("date");
            int sql_user = query.record().indexOf("user");
            int sql_message = query.record().indexOf("message");
            while (query.next())
            {
               QString date = query.value(sql_date).toString();
               QString user = query.value(sql_user).toString();
               QString message = query.value(sql_message).toString();
               archiveMessages.append(date + " " + user + ": " + message);
            }
            client->write(QString("/getArchive:" + archiveMessages.join(",") + "\n").toUtf8());
            qDebug() << archiveMessages;
        }
        else if(users.contains(client))
        {
            QString message = line;
            QString user = users[client];
            QString ipaddress = client->peerAddress().toString();
            qDebug() << "IP: " + ipaddress + " User: " + user + " Message: " + message;
            database.addMessage(message, user, ipaddress);
            foreach(QTcpSocket *otherClient, clients)
                otherClient->write(QString(user + ":" + message + "\n").toUtf8());
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void QtChatServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    clients.remove(client);
    QString user = users[client];
    users.remove(client);
    sendUserList();
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void QtChatServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;
    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
