#pragma once
#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <qdir.h>
#include <QScreen>
#include "ui_Client.h"
#include "Timer.h"
#include "Grid.h"
#include "Player.h"
#include "InfoArea.h"
#include "StartUI.h"
#include "House.h"
#include "Confirm.h"

using VGrid = std::vector<CGrid*>;
using VPlayer = std::vector<CPlayers*>;
using VHouse = std::vector<CHouse*>;

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QString ip, int port = 1234, QWidget *parent = nullptr);
    ~Client();

private:
    Ui::ClientClass ui;

    QTcpSocket* pSocket = nullptr;
    QString m_ip;  // IP地址
    int m_port;    // 端口

    CStart* pStart = nullptr;  // 登录界面

    int m_players;   // 玩家的数量
    int m_seats;     // 自己的座次
    int m_curplayer; // 当前回合的玩家
    VINT vPlayPos;   // 玩家在棋盘上的位置
    VPlayer vCPlayers;

    VGrid vGrid;    // 棋盘方格指针 
    VHouse vHouse;  // 房子信息指针

    QLabel* pLogo = nullptr;
    CTimer* pTimer = nullptr;         // 回合倒计时界面
    CDice* pDice = nullptr;           // 骰子界面
    CInfoArea* pInfoArea = nullptr;   // 信息确认界面
    CShowArea* pShowArea = nullptr;   // 信息显示界面
    CHouseInfo* pHouseInfo = nullptr; // 房子信息界面
    CCard* pCard = nullptr;           // 卡片界面

    QGridLayout* pBoardCenter = nullptr; // 棋盘布局
    QPushButton* pGiveupBtn = nullptr;   // 认输按键
    QPushButton* pExitBtn = nullptr;      // 退出按键
    QGridLayout* pWindow = nullptr;      // 界面布局

private:
    // 连接
    void initConnect();
    
    // 初始化游戏界面
    void initWindow();

    // 初始化选手信息
    void initPlayersInfo(); 
    void initPlayersPos();

    // 初始化房子信息
    void initHousesInfo();

    // 初始化棋盘
    void initOneBoard(int i, int j, int cnt);
    void initBoard(); 

    //  *** 动作 ***
    // 等待玩家
    void waitLogin(QDataStream& in);
    // 游戏开始
    void startGame(QDataStream& in);
    // 房间解散
    void endGame();
    // 回合开始
    void beginRound(QDataStream& in);
    // 更新玩家位置
    void updatePos(QDataStream& in);
    // 更新牌局消息并完成相应操作（中间上方区域）
    void updateGameInfo(MYSIGNAL signal, QDataStream& in);
    // 破产
    void outPlayer(MYSIGNAL signal, QDataStream& in);
    // 游戏结束
    void gameOver(QDataStream& in);

    // 计算玩家所有资产（不包括余额）
    int getAllMoney(int player_id);
    // 玩家卡牌回收
    void setCardInit(int player_id);

private slots:
    // 读取数据
    void readData();
    // 写入数据
    void writeData(MYSIGNAL signal);
    // 游戏开始
    void beginGame(int choose);
    // 展示房子详细信息
    void showHouseInfo(int house_id);
    // 更新消息通知区域（中心区域）
    void updateInfoArea(int type, int choose);
};
