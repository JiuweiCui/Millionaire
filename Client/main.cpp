#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "Client.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Client client("127.0.0.1", 1234);
    client.show();
    //client.showMaximized();

    return a.exec();
}
