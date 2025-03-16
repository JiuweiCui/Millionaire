#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <qdebug.h>
#include <algorithm>
#include <random>
#include <qvector.h>
#include <qqueue.h>
#include "ui_Server.h"
#include "ClientHandle.h"

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();

private:
    QTcpServer* pServer = nullptr;
    int sendNumber = 0;               // 全局序列号
    QQueue<GameMessage> rcvdMsgQueue; // 接收消息

    QMutex m_mutex;
    int m_totalplayers;    // 玩家总数
    int m_curplayer;       // 当前回合玩家
    int m_players;         // 当前玩家总数
    QSet<int> m_stopSet;   // 暂停行动的玩家
    QVector<int> vIsCon;   // 是否在线
    QVector<ClientHandle*> vClients;

private:
    void listenClients();     // 监听客户端
    void shuffleSeats();      // 打乱座次
    int randomStep(int mod);  // 随机步数
    
    void sendToOneClient(const GameMessage& gameMsg);    // 单播
    void sendToAllClients(const GameMessage& gameMsg);   // 广播

private slots:
    void enqueueMessage(const GameMessage& gameMsg);
    void processMessages();
    void connectedFailed();
    void clientDisconnected();
    void stopServer();
};
