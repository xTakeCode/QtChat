#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QTcpSocket> 
#include <QTextDocumentFragment>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent=0);

    private slots:
        void on_loginButton_clicked();
        void on_sayButton_clicked();
        void on_archiveButton_clicked();
        void on_exitArchiveButton_clicked();
        void on_userListWidget_itemClicked(QListWidgetItem *item);
        void readyRead();
        void connected();

    private:
        void createMenus();
        void putMessage(QString user, QString message, bool sound);
        bool checkIfBlocked(QString username);
        QTcpSocket *socket;
        QString myUsername;
        QStringList blockedUsers;
};
