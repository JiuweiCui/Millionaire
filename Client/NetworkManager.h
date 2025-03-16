#pragma once
#include <qthread.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qmutex.h>
#include <qobject.h>
#include <qhash.h>
#include "Constants.h"

class NetworkManager : public QThread {
	Q_OBJECT

public:
	explicit NetworkManager(QTcpSocket* socket, QThread* parent = nullptr);
	~NetworkManager();

	// 是否运行
	bool getRunning() const;

private:
	QTcpSocket* pSocket = nullptr;             // 读写线程安全
	int ackNumber = -1;                        // 确认序号
	QTimer* pTimer = nullptr;                  // 计时器
	QHash<int, PendingMessage> pendingMsgMap;  // 消息管理
	QMutex m_mutex;                            // 互斥锁保护哈希表
	std::atomic<bool> m_running{ true };       // 线程运行标志

private:
	//初始化
	void init();
	// 创建消息
	QByteArray createMessage(const GameMessage& gameMsg);
	// 读取消息
	GameMessage readMessage(const QByteArray& msg);

signals:
	// 发送客户端消息给主线程
	void sendMessageSignal(const GameMessage& gameMsg);
	// 断开连接
	void disconnectedSignal();

private slots:
	// 读取数据
	void onReadyRead();
	// 发送数据
	void sendToServer(const GameMessage& gameMsg);
	// 超时检测
	void onTimeoutCheck();
};




