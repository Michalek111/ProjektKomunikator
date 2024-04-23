#include "mainwindow.h"
#include "chatserver.h"


#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    UserManager userManager;
    ChatServer server;
    server.startServer(1234);



    MainWindow w;
    w.show();
    return app.exec();
}
