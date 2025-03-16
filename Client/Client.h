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
    NetworkManager* pNetwork = nullptr; // ����ͨ�Ź���
    QString m_ip;                       // IP��ַ
    int m_port;                         // �˿�
    int sendNumber = 0;                 // ������ţ��Լ��ģ�
    QQueue<GameMessage> rcvdMsgQueue;   // ������Ϣ
    QMutex m_mutex;                     // ������������Ϣ����

    int m_players;     // ��ҵ�����
    int m_seats;       // �Լ�������
    int m_curplayer;   // ��ǰ�غϵ����

    CStart* startptr = nullptr;          // ��ʼ���� 
    QLabel* logoptr = nullptr;           // Logo
    QGridLayout* pBoardCenter = nullptr; // ���̲���
    QPushButton* pGiveupBtn = nullptr;   // ���䰴��
    QPushButton* pExitBtn = nullptr;     // �˳�����
    QGridLayout* pWindow = nullptr;      // ���沼��

private:
    // ��ʼ����Ϸ����
    void initWindow();
    // ��ʼ��ѡ����Ϣ
    void initPlayersInfo(); 
    
    // ��ʼ������
    void initBoard();
    void initOneBoard(int, int, int);
    void initLayout();
   
    // д������
    void writeData(MYSIGNAL signal);

    //  ------ ���� ------
    // �ȴ����
    void waitLogin(const GameMessage& rcvdMsg);
    // ��Ϸ��ʼ
    void startGame(const GameMessage& rcvdMsg);
    // �����ɢ
    void endGame();
    // �غϿ�ʼ
    void beginRound(const GameMessage& rcvdMsg);
    // �������λ��
    void updatePlayerPos(const GameMessage& rcvdMsg);
    // �����ƾ���Ϣ�������Ӧ�������м��Ϸ�����
    void updateGameInfo1(const GameMessage& rcvdMsg);
    void updateGameInfo2(const GameMessage& rcvdMsg);
    // �Ʋ�
    void outPlayer(const GameMessage& rcvdMsg);
    // ��Ϸ����
    void gameOver(const GameMessage& rcvdMsg);
    // �������
    void viaStartPoint(const GameMessage& rcvdMsg);
    // ������ᡢ����
    void chanceDestiny(const GameMessage& rcvdMsg);
    // -----------------

public slots:
    // �����Ϣ������
    void enqueueMessage(const GameMessage& gameMsg);
    // ������Ϣ
    void processMessages();
    // ��Ϸ��ʼ
    void beginGame(int choose);
    // չʾ������ϸ��Ϣ
    void showHouseInfo(int house_id);
    // ������Ϣ֪ͨ������������
    void updateInfoArea(int type, int choose);
};
