#include "Server.h"

Server::Server()
{
    listenClients();
}

Server::~Server()
{
    for (auto ptr : vClients) {
        FREE_PTR(ptr);
    }
}


void Server::listenClients()
{
    pServer = new QTcpServer;
    if (!pServer->listen(QHostAddress::Any, 1234)) {
        return;
    }

    qDebug() << "Listening...";
    connect(pServer, &QTcpServer::newConnection, [this]() {
        qDebug() << "Connected...";
        
        // 创建并启动专属线程
        QTcpSocket* pSocket = pServer->nextPendingConnection();
        pSocket->setParent(nullptr);
        ClientHandle* pClient = new ClientHandle(pSocket);
        pSocket->moveToThread(pClient);
        connect(pClient, &ClientHandle::sendMessageSignal, this, &Server::enqueueMessage);
        connect(pClient, &ClientHandle::connectedFaliedSignal, this, &Server::connectedFailed);
        connect(pClient, &ClientHandle::disconnectedSignal, [this, pClient]() {
            pClient->quit();
            pClient->wait();
            pClient->deleteLater();
            m_players--;
        });
        pClient->start();
        
        vClients.append(pClient);
        m_totalplayers = m_players = vClients.size();

        GameMessage sendMsg = { MYSIGNAL::ADD, sendNumber++, -1, m_players, 0 };
        sendToAllClients(sendMsg);
    });
}

void Server::shuffleSeats()
{
    QVector<int> vSeats(m_players);
    for (int i = 0; i < m_players; i++) {
        vSeats[i] = i;
    }

    std::random_device rd;  // 获取随机数种子
    std::mt19937 g(rd());   // 使用种子初始化Mersenne Twister生成器

    std::shuffle(vSeats.begin(), vSeats.end(), g);
    auto vTmp = vClients;
    for (int i = 0; i < vClients.size(); i++) {
        vClients[i] = vTmp[vSeats[i]];
    }
}

int Server::randomStep(int mod)
{
    srand(static_cast<unsigned int>(time(0)));
    return rand() % mod + 1;
}

void Server::sendToOneClient(const GameMessage& gameMsg)
{
    if (auto clientptr = qobject_cast<ClientHandle*>(vClients[gameMsg.pos])) {
        QMetaObject::invokeMethod(clientptr, "sendToClient",
            Qt::QueuedConnection, Q_ARG(GameMessage, gameMsg));
    }
}

void Server::sendToAllClients(const GameMessage& gameMsg)
{
    for (int i = 0; i < m_totalplayers; i++) {
        if (auto clientptr = qobject_cast<ClientHandle*>(vClients[i])) {
            QMetaObject::invokeMethod(clientptr, "sendToClient",
                Qt::QueuedConnection, Q_ARG(GameMessage, gameMsg));
        }
    }
}

// 添加消息到队列（线程安全）
void Server::enqueueMessage(const GameMessage& gameMsg)
{
    QMutexLocker locker(&m_mutex);
    rcvdMsgQueue << gameMsg;
    // 通知主线程处理
    QMetaObject::invokeMethod(this, "processMessages", Qt::QueuedConnection); 
}

// 处理消息
void Server::processMessages()
{
    // 减小锁的粒度
    GameMessage rcvdMsg;
    GameMessage sendMsg;                  // 要发送的消息
    {
        QMutexLocker locker(&m_mutex);
        rcvdMsg = rcvdMsgQueue.dequeue(); // 当前要处理的消息
    }
    
    if (rcvdMsg.type == MYSIGNAL::START) {
        // 停止接受新连接
        if (pServer && pServer->isListening()) {
            pServer->pauseAccepting();
            qDebug() << "Server stopped accepting new connections.";
        }

        shuffleSeats();
        vIsCon.resize(m_players, 1);
        // 游戏开始，序列号，单播，座次
        sendMsg = { rcvdMsg.type, sendNumber++, 0, 0, 0 };
        for (int i = 0; i < m_totalplayers; i++) {
            sendMsg.pos = sendMsg.data1 = i;
            sendToOneClient(sendMsg);
        }

        m_curplayer = 0;
        sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
        /*
        QTimer::singleShot(2000, this, [&]() {
            // 回合开始，序列号，广播，当前回合玩家
            sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
        });
        */
    }
    else if (rcvdMsg.type == MYSIGNAL::END) {
        // 房间解散，序列号，广播
        sendMsg = { rcvdMsg.type, sendNumber++, -1, 0, 0};
        sendToAllClients(sendMsg);
        stopServer();
        return;
    }
    else if (rcvdMsg.type == MYSIGNAL::MOVE) {
        // 玩家前进，序列号，广播，前进步数
        sendMsg = { rcvdMsg.type, sendNumber++, -1, randomStep(6), 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::BUY_COUNTRY || rcvdMsg.type == MYSIGNAL::BUY_HOUSE) {
        // 购买，序列号，广播，购买玩家，购买房子ID
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.data1, rcvdMsg.data2 };
    }
    else if (rcvdMsg.type == MYSIGNAL::GIVE_TOLL) {
        // 过路费，序列号，广播，缴费玩家，收费玩家
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.data1, rcvdMsg.data2 };
    }
    else if (rcvdMsg.type == MYSIGNAL::SELL || rcvdMsg.type == MYSIGNAL::MORT ||
        rcvdMsg.type == MYSIGNAL::BUY_BACK) {
        // 房子操作，序列号，广播，玩家，房子ID
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, rcvdMsg.data1 };
    }
    else if (rcvdMsg.type == MYSIGNAL::PASS) {
        /*
        if (m_players <= 1) { // 游戏结束
            out << MYSIGNAL::OVER << 0;
            sendToClients(data);
            QTimer::singleShot(10000, this, &Server::stopServer);
            return;
        }
        */

        while (true) {
            m_curplayer = (m_curplayer + 1) % vIsCon.size();
            if (vIsCon[m_curplayer]) {
                if (m_stopSet.contains(m_curplayer)) { // 该角色被暂停了一次
                    m_stopSet.remove(m_curplayer);
                    continue;
                }
                else {
                    break;
                }
            }
        }
        // 回合开始，序列号，广播，当前回合玩家
        sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::OUT1 || rcvdMsg.type == MYSIGNAL::OUT2 ||
        rcvdMsg.type == MYSIGNAL::OUT3 || rcvdMsg.type == MYSIGNAL::GIVE_UP) {
        //vIsCon[gameMsg.pos] = 0;
        m_players--;
        // 玩家淘汰-放弃，序列号，广播，玩家
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, 0 };
        sendToAllClients(sendMsg);

        if (m_players <= 0) {
            QTimer::singleShot(2000, this, [&]() {
                int winner = 0;  // 获胜者
                for (int i = 0; i < vIsCon.size(); i++) {
                    if (vIsCon[i] == 1) {
                        winner = i;
                        break;
                    }
                }
                // 游戏结束，序列号，广播，赢家
                sendMsg = { MYSIGNAL::OVER, sendNumber++, -1, winner, 0 };
                sendToAllClients(sendMsg);
                stopServer();
            });
        }
        return;
    }
    else if (rcvdMsg.type == MYSIGNAL::VIA_START) {
        // 经过起点，序列号，广播，玩家
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::CHANCE || rcvdMsg.type == MYSIGNAL::DESTINY) {
        int op_id = randomStep(18);
        if (op_id == 11) {
            m_stopSet.insert(rcvdMsg.pos);
        }
        // 机会-命运，序列号，广播，玩家，抽卡ID
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, op_id };
    }
    sendToAllClients(sendMsg);
}


void Server::connectedFailed()
{
    auto it = std::remove_if(vClients.begin(), vClients.end(), [](ClientHandle* t) {
        return !t->getRunning(); });
    vClients.erase(it, vClients.end());
}

void Server::stopServer()
{
    // 断开所有客户端连接
    for (auto client : vClients) {
        if (client) {
            client->requestInterruption();
            client->quit();
            client->wait(1000);
        }
    }

    if (pServer) {  // 关闭服务器
        pServer->close();
        pServer->deleteLater();
    }

    qDebug() << "Server stopped.";
    QApplication::quit();


    /*
    QTimer::singleShot(WAIT_CLOSE_TIME, this, [this]() {

    });
    */
}

void Server::clientDisconnected()
{
}