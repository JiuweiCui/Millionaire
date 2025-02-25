#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QtWidgets/QWidget>
#include <qdebug.h>
#include <algorithm>
#include <random>
#include <qtimer.h>
#include "ui_Server.h"

// 信号类型
enum class MYSIGNAL
{
    ADD = 0,         // 增加玩家
    START = 1,       // 游戏开始
    END = 2,         // 房间解散 
    BEGIN = 3,       // 回合开始
    MOVE = 4,        // 当前回合玩家向前移动 
    BUY_COUNTRY = 5, // 购买地皮
    BUY_HOUSE = 6,   // 购买房子
    GIVE_TOLL = 7,   // 缴纳过路费
    SELL = 8,        // 卖房子
    MORT = 9,        // 抵押房子
    BUY_BACK = 10,   // 买回房子
    PASS = 11,       // 该回合玩家结束
    OUT = 12,        // 破产
    GIVE_UP = 13,    // 认输  
    OVER = 14        // 游戏结束
};

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

private:
    int cur_players;
    int m_players;
    QTcpServer* pServer = nullptr;
    std::vector<QTcpSocket*> vClients;
    std::vector<int> vIsCon;
    
    int currentId;

private:
    void listenClients();
    void sendToClients(const QByteArray& data);
    void writeData(MYSIGNAL signal, int pos);

    void shuffleSeats();
    int randomStep();
    
private slots:
    void readData();
    void stopServer();
    void clientDisconnected();
};
