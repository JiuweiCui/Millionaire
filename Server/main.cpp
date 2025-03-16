#include <QApplication>
#include "Server.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qDebug() << "Main:" << QThread::currentThreadId();
    Server server;
    app.exec();

    return 0;
}
