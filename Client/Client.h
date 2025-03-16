#pragma once
#include <qapplication.h>
#include <qtcpsocket.h>
#include <qdebug.h>
#include <qwidget.h>
#include <qscreen.h>
#include <qmap.h>
#include <qhash.h>
#include <qqueue.h>
#include <qmutex.h>
#include "ui_Client.h"
#include "NetworkManager.h"
#include "Timer.h"
#include "Grid.h"
#include "Player.h"
#include "InfoArea.h"
#include "StartUI.h"
#include "Confirm.h"
#include "HouseShow.h"
#include "Card.h"

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QString ip, int port = 1234, QWidget *parent = nullptr);
    ~Client();

    int initConnect();

private:
    QTcpSocket* pSocket = nullptr;
    NetworkManager* pNetwork = nullptr; // 网络通信管理
    QString m_ip;                       // IP地址
    int m_port;                         // 端口
    int sendNumber = 0;                 // 发送序号（自己的）
    QQueue<GameMessage> rcvdMsgQueue;   // 接收消息
    QMutex m_mutex;                     // 互斥锁访问消息队列

    int m_players;     // 玩家的数量
    int m_seats;       // 自己的座次
    int m_curplayer;   // 当前回合的玩家

    CStart* startptr = nullptr;          // 开始界面 
    QLabel* logoptr = nullptr;           // Logo
    QGridLayout* pBoardCenter = nullptr; // 棋盘布局
    QPushButton* pGiveupBtn = nullptr;   // 认输按键
    QPushButton* pExitBtn = nullptr;     // 退出按键
    QGridLayout* pWindow = nullptr;      // 界面布局

private:
    // 初始化游戏界面
    void initWindow();
    // 初始化选手信息
    void initPlayersInfo(); 
    
    // 初始化棋盘
    void initBoard();
    void initOneBoard(int, int, int);
    void initLayout();
   
    // 写入数据
    void writeData(MYSIGNAL signal);

    //  ------ 动作 ------
    // 等待玩家
    void waitLogin(const GameMessage& rcvdMsg);
    // 游戏开始
    void startGame(const GameMessage& rcvdMsg);
    // 房间解散
    void endGame();
    // 回合开始
    void beginRound(const GameMessage& rcvdMsg);
    // 更新玩家位置
    void updatePlayerPos(const GameMessage& rcvdMsg);
    // 更新牌局消息并完成相应操作（中间上方区域）
    void updateGameInfo1(const GameMessage& rcvdMsg);
    void updateGameInfo2(const GameMessage& rcvdMsg);
    // 破产
    void outPlayer(const GameMessage& rcvdMsg);
    // 游戏结束
    void gameOver(const GameMessage& rcvdMsg);
    // 经过起点
    void viaStartPoint(const GameMessage& rcvdMsg);
    // 处理机会、命运
    void chanceDestiny(const GameMessage& rcvdMsg);
    // -----------------

public slots:
    // 添加消息到队列
    void enqueueMessage(const GameMessage& gameMsg);
    // 处理消息
    void processMessages();
    // 游戏开始
    void beginGame(int choose);
    // 展示房子详细信息
    void showHouseInfo(int house_id);
    // 更新消息通知区域（中心区域）
    void updateInfoArea(int type, int choose);
};
