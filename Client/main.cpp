#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include "Client.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Client client("127.0.0.1", 1234);
    if (client.initConnect()) {
        client.show();
        app.exec();
    }

    return 0;
}
