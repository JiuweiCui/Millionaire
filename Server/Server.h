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
    int sendNumber = 0;               // ȫ�����к�
    QQueue<GameMessage> rcvdMsgQueue; // ������Ϣ

    QMutex m_mutex;
    int m_totalplayers;    // �������
    int m_curplayer;       // ��ǰ�غ����
    int m_players;         // ��ǰ�������
    QSet<int> m_stopSet;   // ��ͣ�ж������
    QVector<int> vIsCon;   // �Ƿ�����
    QVector<ClientHandle*> vClients;

private:
    void listenClients();     // �����ͻ���
    void shuffleSeats();      // ��������
    int randomStep(int mod);  // �������
    
    void sendToOneClient(const GameMessage& gameMsg);    // ����
    void sendToAllClients(const GameMessage& gameMsg);   // �㲥

private slots:
    void enqueueMessage(const GameMessage& gameMsg);
    void processMessages();
    void connectedFailed();
    void clientDisconnected();
    void stopServer();
};
