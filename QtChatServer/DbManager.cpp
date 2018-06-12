#include "DbManager.h"
DbManager::DbManager(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);
   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

bool DbManager::addMessage(const QString& message, const QString& user, QString& ipaddress)
{
   bool success = false;
   QSqlQuery query;
   query.prepare("INSERT INTO message (message,user,ipaddress) VALUES (:message,:user,:ipaddress)");
   query.bindValue(":message", message);
   query.bindValue(":user", user);
   if(ipaddress.isEmpty()) ipaddress = "localhost";
   query.bindValue(":ip", ipaddress);
   if(query.exec())
   {
       success = true;
       qDebug() << "addMessage success";
   }
   else
   {
        qDebug() << "addMessage error:  "
                 << query.lastError();
   }

   return success;
}

QStringList DbManager::getArchive() {
    QStringList messagesList;
    QSqlQuery query("SELECT * FROM message");
    int dateIndex = query.record().indexOf("date");
    int userIndex = query.record().indexOf("user");
    int messageIndex = query.record().indexOf("message");
    while (query.next())
    {
        QString message = query.value(dateIndex).toString() + " "
                + query.value(userIndex).toString() + ": "
                + query.value(messageIndex).toString();
        messagesList << message;
    }
    return messagesList;
}
