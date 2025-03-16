#pragma once
#include <qdatetime.h>

#define FREE_PTR(ptr) \
{                     \
    if (ptr) {        \
        delete ptr;   \
    }                 \
    ptr = nullptr;    \
}

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
    OUT1 = 12,       // �������Ʋ�
    OUT2 = 13,       // ���ᡢ�����Ʋ�
    OUT3 = 14,       // ��ʱ�Ʋ�
    GIVE_UP = 15,    // ����  
    OVER = 16,       // ��Ϸ����
    VIA_START = 17,  // �������
    CHANCE = 18,     // ���� 
    DESTINY = 19,    // ����
    ACK = 20         // ȷ��
};

// ȫ����Ϣ�ṹ��
typedef struct GameMessage {
    MYSIGNAL type;       // ��Ϣ����
    int seqNumber;       // ��Ϣ���
    int pos;
    int data1;
    int data2;
} GameMessage;

// ��Ϣ����
typedef struct PendingMessage {
public:
    QByteArray rawData;          // ԭʼ����
    QDateTime lastTime;          // �����ʱ��
    int retryCount = 0;          // ��ʱ�ش�����
} PendingMessage;

#define TURN_TIME 1000     // ��ʱ���ʱ��
#define ACK_TIMEOUT 500   // ��ʱ�ش�ʱ��
#define MAX_RETRY 5       // ����ش�����

#define WAIT_CLOSE_TIME 2000 // �������ȴ��ر�����