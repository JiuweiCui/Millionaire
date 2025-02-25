#include "Server.h"

#define FREE_PTR(ptr) \
{                     \
    if (ptr) {        \
        delete ptr;   \
    }                 \
    ptr = nullptr;    \
}

Server::Server(QWidget *parent)
    : QWidget(parent)
{
    listenClients();
}

Server::~Server()
{
    for (auto pClient : vClients) {
        FREE_PTR(pClient);
    }
    FREE_PTR(pServer);
}


void Server::listenClients()
{
    pServer = new QTcpServer;
    if (!pServer->listen(QHostAddress::Any, 1234)) {
        return;
    }

    qDebug() << "Listening...";
    connect(pServer, &QTcpServer::newConnection, [&]() {
        qDebug() << "Connect...";
        
        QTcpSocket* pClient = pServer->nextPendingConnection();
        vClients.push_back(pClient);
        m_players = vClients.size();
        writeData(MYSIGNAL::ADD, -1);

        connect(pClient, &QTcpSocket::readyRead, this, &Server::readData);
        connect(pClient, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    });
}

void Server::sendToClients(const QByteArray& data) 
{
    for (QTcpSocket* pClient : vClients) {
        pClient->write(data);
    }
}

void Server::writeData(MYSIGNAL signal, int pos)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);
    
    if (signal == MYSIGNAL::ADD) {
        out << signal << m_players;
        sendToClients(data);
        return;
    }
    else if (signal == MYSIGNAL::START) {        
        shuffleSeats();
        data.clear();
        vIsCon.resize(m_players, 1);
        cur_players = 0;
        out << MYSIGNAL::BEGIN << cur_players;
        sendToClients(data); // 决定玩家位置之后，接着从玩家1开始
        return;
    }
    else if (signal == MYSIGNAL::END) {
        out << signal;
        sendToClients(data);
        QTimer::singleShot(1000, this, &Server::stopServer);
        return;
    }
    else if (signal == MYSIGNAL::MOVE) {
        out << signal << randomStep();
    }
    else if (signal == MYSIGNAL::BUY_COUNTRY || 
        signal == MYSIGNAL::BUY_HOUSE || 
        signal == MYSIGNAL::GIVE_TOLL) {
        out << signal;
        sendToClients(data);
        return;
    }
    else if (signal == MYSIGNAL::SELL || signal == MYSIGNAL::MORT ||
        signal == MYSIGNAL::BUY_BACK) {
        out << signal << pos << currentId;
        sendToClients(data);
        return;
    }
    else if (signal == MYSIGNAL::PASS) {
        /*
        if (m_players <= 1) {
            out << MYSIGNAL::OVER << 0;
            sendToClients(data);
            QTimer::singleShot(10000, this, &Server::stopServer);
            return;
        }
        */

        while (true) {
            cur_players = (cur_players + 1) % vIsCon.size();
            if (vIsCon[cur_players]) {
                break;
            }
        }
        out << MYSIGNAL::BEGIN << cur_players;
    }
    else if (signal == MYSIGNAL::OUT || signal == MYSIGNAL::GIVE_UP) {
        //vIsCon[pos] = 0;
        m_players--;
        out << signal << pos;
        sendToClients(data);
        return;
    }

    vClients[pos]->write(data);
    vClients[pos]->flush();
}

void Server::shuffleSeats()
{
    std::vector<int> nums(m_players);
    for (int i = 0; i < m_players; i++) {
        nums[i] = i;
    }

    std::random_device rd;  // 获取随机数种子
    std::mt19937 g(rd());   // 使用种子初始化Mersenne Twister生成器

    std::shuffle(nums.begin(), nums.end(), g);

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);
    for (int i = 0; i < m_players; i++) {
        data.clear();
        out << MYSIGNAL::START << nums[i] << m_players;
        vClients[i]->write(data);
        vClients[i]->flush();
    }
}

int Server::randomStep()
{
    srand(static_cast<unsigned int>(time(0)));
    return rand() % 6 + 1;
}


void Server::readData()
{
    QTcpSocket* pSocket = static_cast<QTcpSocket*>(sender());
    if (!pSocket) {
        return;
    }

    int pos = std::find(vClients.begin(), vClients.end(), pSocket) - vClients.begin();
    QDataStream in(pSocket);
    in.setVersion(QDataStream::Qt_6_8);
    MYSIGNAL signal;
    in >> signal >> currentId;

    writeData(signal, pos);
}

void Server::stopServer() {
    
    // 断开所有客户端连接
    for (QTcpSocket* pClient : vClients) {
        pClient->disconnectFromHost();
        pClient->deleteLater(); // 确保在适当的时候删除客户端socket
    }
    vClients.clear();

    // 停止监听新的连接
    if (pServer) {
        pServer->close();
        FREE_PTR(pServer);
    }

    qDebug() << "Server stopped.";
    QApplication::quit();
}

void Server::clientDisconnected()
{
    QTcpSocket* pClient = qobject_cast<QTcpSocket*>(sender());
    int pos = std::find(vClients.begin(), vClients.end(), pClient) - vClients.begin();
    if (vIsCon.size() > pos && vIsCon[pos]) {
        vIsCon[pos] = 0;
        m_players--;
    }

    if (m_players <= 0) {
        qDebug() << "All clients disconnected. Stopping server...";
        QTimer::singleShot(1000, this, &Server::stopServer);
    }
}
