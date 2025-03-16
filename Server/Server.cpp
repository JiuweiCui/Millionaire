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
        
        // ����������ר���߳�
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

    std::random_device rd;  // ��ȡ���������
    std::mt19937 g(rd());   // ʹ�����ӳ�ʼ��Mersenne Twister������

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

// �����Ϣ�����У��̰߳�ȫ��
void Server::enqueueMessage(const GameMessage& gameMsg)
{
    QMutexLocker locker(&m_mutex);
    rcvdMsgQueue << gameMsg;
    // ֪ͨ���̴߳���
    QMetaObject::invokeMethod(this, "processMessages", Qt::QueuedConnection); 
}

// ������Ϣ
void Server::processMessages()
{
    // ��С��������
    GameMessage rcvdMsg;
    GameMessage sendMsg;                  // Ҫ���͵���Ϣ
    {
        QMutexLocker locker(&m_mutex);
        rcvdMsg = rcvdMsgQueue.dequeue(); // ��ǰҪ�������Ϣ
    }
    
    if (rcvdMsg.type == MYSIGNAL::START) {
        // ֹͣ����������
        if (pServer && pServer->isListening()) {
            pServer->pauseAccepting();
            qDebug() << "Server stopped accepting new connections.";
        }

        shuffleSeats();
        vIsCon.resize(m_players, 1);
        // ��Ϸ��ʼ�����кţ�����������
        sendMsg = { rcvdMsg.type, sendNumber++, 0, 0, 0 };
        for (int i = 0; i < m_totalplayers; i++) {
            sendMsg.pos = sendMsg.data1 = i;
            sendToOneClient(sendMsg);
        }

        m_curplayer = 0;
        sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
        /*
        QTimer::singleShot(2000, this, [&]() {
            // �غϿ�ʼ�����кţ��㲥����ǰ�غ����
            sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
        });
        */
    }
    else if (rcvdMsg.type == MYSIGNAL::END) {
        // �����ɢ�����кţ��㲥
        sendMsg = { rcvdMsg.type, sendNumber++, -1, 0, 0};
        sendToAllClients(sendMsg);
        stopServer();
        return;
    }
    else if (rcvdMsg.type == MYSIGNAL::MOVE) {
        // ���ǰ�������кţ��㲥��ǰ������
        sendMsg = { rcvdMsg.type, sendNumber++, -1, randomStep(6), 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::BUY_COUNTRY || rcvdMsg.type == MYSIGNAL::BUY_HOUSE) {
        // �������кţ��㲥��������ң�������ID
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.data1, rcvdMsg.data2 };
    }
    else if (rcvdMsg.type == MYSIGNAL::GIVE_TOLL) {
        // ��·�ѣ����кţ��㲥���ɷ���ң��շ����
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.data1, rcvdMsg.data2 };
    }
    else if (rcvdMsg.type == MYSIGNAL::SELL || rcvdMsg.type == MYSIGNAL::MORT ||
        rcvdMsg.type == MYSIGNAL::BUY_BACK) {
        // ���Ӳ��������кţ��㲥����ң�����ID
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, rcvdMsg.data1 };
    }
    else if (rcvdMsg.type == MYSIGNAL::PASS) {
        /*
        if (m_players <= 1) { // ��Ϸ����
            out << MYSIGNAL::OVER << 0;
            sendToClients(data);
            QTimer::singleShot(10000, this, &Server::stopServer);
            return;
        }
        */

        while (true) {
            m_curplayer = (m_curplayer + 1) % vIsCon.size();
            if (vIsCon[m_curplayer]) {
                if (m_stopSet.contains(m_curplayer)) { // �ý�ɫ����ͣ��һ��
                    m_stopSet.remove(m_curplayer);
                    continue;
                }
                else {
                    break;
                }
            }
        }
        // �غϿ�ʼ�����кţ��㲥����ǰ�غ����
        sendMsg = { MYSIGNAL::BEGIN, sendNumber++, -1, m_curplayer, 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::OUT1 || rcvdMsg.type == MYSIGNAL::OUT2 ||
        rcvdMsg.type == MYSIGNAL::OUT3 || rcvdMsg.type == MYSIGNAL::GIVE_UP) {
        //vIsCon[gameMsg.pos] = 0;
        m_players--;
        // �����̭-���������кţ��㲥�����
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, 0 };
        sendToAllClients(sendMsg);

        if (m_players <= 0) {
            QTimer::singleShot(2000, this, [&]() {
                int winner = 0;  // ��ʤ��
                for (int i = 0; i < vIsCon.size(); i++) {
                    if (vIsCon[i] == 1) {
                        winner = i;
                        break;
                    }
                }
                // ��Ϸ���������кţ��㲥��Ӯ��
                sendMsg = { MYSIGNAL::OVER, sendNumber++, -1, winner, 0 };
                sendToAllClients(sendMsg);
                stopServer();
            });
        }
        return;
    }
    else if (rcvdMsg.type == MYSIGNAL::VIA_START) {
        // ������㣬���кţ��㲥�����
        sendMsg = { rcvdMsg.type, sendNumber++, -1, rcvdMsg.pos, 0 };
    }
    else if (rcvdMsg.type == MYSIGNAL::CHANCE || rcvdMsg.type == MYSIGNAL::DESTINY) {
        int op_id = randomStep(18);
        if (op_id == 11) {
            m_stopSet.insert(rcvdMsg.pos);
        }
        // ����-���ˣ����кţ��㲥����ң��鿨ID
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
    // �Ͽ����пͻ�������
    for (auto client : vClients) {
        if (client) {
            client->requestInterruption();
            client->quit();
            client->wait(1000);
        }
    }

    if (pServer) {  // �رշ�����
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