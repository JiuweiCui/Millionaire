#pragma once
#include <qthread.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qmutex.h>
#include <qobject.h>
#include <qhash.h>
#include "Constants.h"

class ClientHandle : public QThread {
	Q_OBJECT

public:
	explicit ClientHandle(QTcpSocket* socket, QThread* parent = nullptr);
	~ClientHandle();

private:
	int ackNumber = -1;                  // 确认序号
	QTcpSocket* pSocket = nullptr;       // 读写线程安全

	QTimer* pTimer = nullptr;
	QHash<int, PendingMessage> pendingMsgMap;  // 消息管理
	QMutex m_mutex;                      // 互斥锁保护哈希表
	std::atomic<bool> m_running{ true }; // 线程运行标志

private:
	void init();
	// 创建消息
	QByteArray createMessage(const GameMessage& gameMsg);
	// 读取消息
	GameMessage readMessage(const QByteArray& msg);

public:
	// 是否运行
	bool getRunning();

signals:
	// 发送客户端消息给主线程
	void sendMessageSignal(const GameMessage& gameMsg);
	// 断开连接
	void disconnectedSignal();
	// 连接失败
	void connectedFaliedSignal();

private slots:
	// 读取数据
	void onReadyRead();
	// 发送数据
	void sendToClient(const GameMessage& gameMsg);
	// 超时检测
	void onTimeoutCheck();
};



