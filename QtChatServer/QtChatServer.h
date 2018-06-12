#ifndef __QTCHATSERVERSERVER_H__
#define __QTCHATSERVERSERVER_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include "DbManager.h"

class QtChatServer : public QTcpServer
{
    Q_OBJECT

    public:
        QtChatServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();

    protected:
        void incomingConnection(qintptr socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
        DbManager database = DbManager("qsqlite.db");
};

#endif
