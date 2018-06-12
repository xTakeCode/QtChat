#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QSqlRecord>

class DbManager
{
public:
    DbManager(const QString& path);
    bool addMessage(const QString& message, const QString& user, QString& ip);
    QStringList getArchive();
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
