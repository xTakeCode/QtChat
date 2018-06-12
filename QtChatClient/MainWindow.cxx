#include "MainWindow.h"
#include <QRegExp>
#include "sound.h"
#include <Qt>
#include "emoticons.h"
#include <QScrollBar>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    stackedWidget->setCurrentWidget(loginPage);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}


void MainWindow::on_loginButton_clicked()
{
    socket->connectToHost(serverLineEdit->text(), 4200);
}

void MainWindow::on_sayButton_clicked()
{
    QString message = sayLineEdit->text().trimmed();
    if(!message.isEmpty())
    {
        socket->write(QString(message + "\n").toUtf8());
    }
    sayLineEdit->clear();
    sayLineEdit->setFocus();
}

void MainWindow::on_archiveButton_clicked()
{
    socket->write(QString("/getArchive\n").toUtf8());
    stackedWidget->setCurrentWidget(archivePage);
    archiveTextEdit->clear();
    archiveTextEdit->append("Loading messages archive...");
    exitArchiveButton->setEnabled(false);
    qDebug() << "/getArchive command has been sent to the server";
}

void MainWindow::on_exitArchiveButton_clicked()
{
    archiveTextEdit->clear();
    stackedWidget->setCurrentWidget(chatPage);
}

void MainWindow::on_userListWidget_itemClicked(QListWidgetItem *item)
{
    QString clikedUser = item->text();
    if (checkIfBlocked(clikedUser))
    {
        blockedUsers.removeOne(item->text());
        item->setForeground(Qt::black);
        this->putMessage("Client", clikedUser + " unblocked!", false);
        return;
    }
    if (clikedUser != myUsername + "(you)")
    {
        blockedUsers.append(item->text());
        item->setForeground(Qt::red);
        this->putMessage("Client", clikedUser + " blocked!", false);
    }
}

void MainWindow::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        QRegExp messageRegex("^([^:]+):(.*)$");
        QRegExp usersRegex("^/users:(.*)$");
        QRegExp archiveRegex("^/getArchive:(.*)$");
        qDebug() << line;
        if(usersRegex.indexIn(line) != -1)
        {
            QStringList users = usersRegex.cap(1).split(",");
            userListWidget->clear();
            foreach(QString user, users)
                if (myUsername == user)
                    new QListWidgetItem(QPixmap(":/userme.png"), user.append("(you)"), userListWidget);
                else
                    new QListWidgetItem(QPixmap(":/user.png"), user, userListWidget);
        }
        else if(archiveRegex.indexIn(line) != -1)
        {
            QStringList archive = archiveRegex.cap(1).split(",");
            archiveTextEdit->clear();
            foreach(QString message, archive)
            {
                Emoticons::replaceEmoticon(&message);
                QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml(message);
                archiveTextEdit->textCursor().insertFragment(fragment);
                archiveTextEdit->append("");
            }
            exitArchiveButton->setEnabled(true);
            QScrollBar *vScrollBar = archiveTextEdit->verticalScrollBar();
            vScrollBar->triggerAction(QScrollBar::SliderToMaximum);
        }
        else if(messageRegex.indexIn(line) != -1)
        {
            QString user = messageRegex.cap(1);
            QString message = messageRegex.cap(2);
            Emoticons::replaceEmoticon(&message);
            if(!checkIfBlocked(user))
            {
                if (myUsername != user)
                    this->putMessage(user, message, true);
                else
                    this->putMessage(user, message, false);
            }
        }
    }
}

void MainWindow::connected()
{
    stackedWidget->setCurrentWidget(chatPage);
    myUsername = userLineEdit->text();
    socket->write(QString("/me:" + myUsername + "\n").toUtf8());
}

void MainWindow::putMessage(QString user, QString message, bool sound)
{
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml("<b>" + user + "</b>: " + message);
    roomTextEdit->textCursor().insertFragment(fragment);
    roomTextEdit->append("");
    if(sound) Sound::playMessageSound();
}

bool MainWindow::checkIfBlocked(QString username)
{
    foreach (const QString &user, blockedUsers)
    {
        if (user.contains(username))
        {
            return true;
        }
    }
    return false;
}
