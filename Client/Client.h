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
    QString m_ip;  // IP��ַ
    int m_port;    // �˿�

    CStart* pStart = nullptr;  // ��¼����

    int m_players;   // ��ҵ�����
    int m_seats;     // �Լ�������
    int m_curplayer; // ��ǰ�غϵ����
    VINT vPlayPos;   // ����������ϵ�λ��
    VPlayer vCPlayers;

    VGrid vGrid;    // ���̷���ָ�� 
    VHouse vHouse;  // ������Ϣָ��

    QLabel* pLogo = nullptr;
    CTimer* pTimer = nullptr;         // �غϵ���ʱ����
    CDice* pDice = nullptr;           // ���ӽ���
    CInfoArea* pInfoArea = nullptr;   // ��Ϣȷ�Ͻ���
    CShowArea* pShowArea = nullptr;   // ��Ϣ��ʾ����
    CHouseInfo* pHouseInfo = nullptr; // ������Ϣ����
    CCard* pCard = nullptr;           // ��Ƭ����

    QGridLayout* pBoardCenter = nullptr; // ���̲���
    QPushButton* pGiveupBtn = nullptr;   // ���䰴��
    QPushButton* pExitBtn = nullptr;      // �˳�����
    QGridLayout* pWindow = nullptr;      // ���沼��

private:
    // ����
    void initConnect();
    
    // ��ʼ����Ϸ����
    void initWindow();

    // ��ʼ��ѡ����Ϣ
    void initPlayersInfo(); 
    void initPlayersPos();

    // ��ʼ��������Ϣ
    void initHousesInfo();

    // ��ʼ������
    void initOneBoard(int i, int j, int cnt);
    void initBoard(); 

    //  *** ���� ***
    // �ȴ����
    void waitLogin(QDataStream& in);
    // ��Ϸ��ʼ
    void startGame(QDataStream& in);
    // �����ɢ
    void endGame();
    // �غϿ�ʼ
    void beginRound(QDataStream& in);
    // �������λ��
    void updatePos(QDataStream& in);
    // �����ƾ���Ϣ�������Ӧ�������м��Ϸ�����
    void updateGameInfo(MYSIGNAL signal, QDataStream& in);
    // �Ʋ�
    void outPlayer(MYSIGNAL signal, QDataStream& in);
    // ��Ϸ����
    void gameOver(QDataStream& in);

    // ������������ʲ�����������
    int getAllMoney(int player_id);
    // ��ҿ��ƻ���
    void setCardInit(int player_id);

private slots:
    // ��ȡ����
    void readData();
    // д������
    void writeData(MYSIGNAL signal);
    // ��Ϸ��ʼ
    void beginGame(int choose);
    // չʾ������ϸ��Ϣ
    void showHouseInfo(int house_id);
    // ������Ϣ֪ͨ������������
    void updateInfoArea(int type, int choose);
};
