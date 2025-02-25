#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QtWidgets/QWidget>
#include <qdebug.h>
#include <algorithm>
#include <random>
#include <qtimer.h>
#include "ui_Server.h"

// �ź�����
enum class MYSIGNAL
{
    ADD = 0,         // �������
    START = 1,       // ��Ϸ��ʼ
    END = 2,         // �����ɢ 
    BEGIN = 3,       // �غϿ�ʼ
    MOVE = 4,        // ��ǰ�غ������ǰ�ƶ� 
    BUY_COUNTRY = 5, // �����Ƥ
    BUY_HOUSE = 6,   // ������
    GIVE_TOLL = 7,   // ���ɹ�·��
    SELL = 8,        // ������
    MORT = 9,        // ��Ѻ����
    BUY_BACK = 10,   // ��ط���
    PASS = 11,       // �ûغ���ҽ���
    OUT = 12,        // �Ʋ�
    GIVE_UP = 13,    // ����  
    OVER = 14        // ��Ϸ����
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
