#pragma once
#include <qdatetime.h>

#define FREE_PTR(ptr) \
{                     \
    if (ptr) {        \
        delete ptr;   \
    }                 \
    ptr = nullptr;    \
}

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
    OUT1 = 12,       // 被别人破产
    OUT2 = 13,       // 机会、命运破产
    OUT3 = 14,       // 超时破产
    GIVE_UP = 15,    // 认输  
    OVER = 16,       // 游戏结束
    VIA_START = 17,  // 经过起点
    CHANCE = 18,     // 机会 
    DESTINY = 19,    // 命运
    ACK = 20         // 确认
};

// 全局消息结构体
typedef struct GameMessage {
    MYSIGNAL type;       // 消息类型
    int seqNumber;       // 消息序号
    int pos;
    int data1;
    int data2;
} GameMessage;

// 消息跟踪
typedef struct PendingMessage {
public:
    QByteArray rawData;          // 原始数据
    QDateTime lastTime;          // 最后发送时间
    int retryCount = 0;          // 超时重传次数
} PendingMessage;

#define TURN_TIME 1000     // 超时检测时间
#define ACK_TIMEOUT 500   // 超时重传时间
#define MAX_RETRY 5       // 最大重传次数

#define WAIT_CLOSE_TIME 2000 // 服务器等待关闭连接