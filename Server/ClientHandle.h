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
	int ackNumber = -1;                  // ȷ�����
	QTcpSocket* pSocket = nullptr;       // ��д�̰߳�ȫ

	QTimer* pTimer = nullptr;
	QHash<int, PendingMessage> pendingMsgMap;  // ��Ϣ����
	QMutex m_mutex;                      // ������������ϣ��
	std::atomic<bool> m_running{ true }; // �߳����б�־

private:
	void init();
	// ������Ϣ
	QByteArray createMessage(const GameMessage& gameMsg);
	// ��ȡ��Ϣ
	GameMessage readMessage(const QByteArray& msg);

public:
	// �Ƿ�����
	bool getRunning();

signals:
	// ���Ϳͻ�����Ϣ�����߳�
	void sendMessageSignal(const GameMessage& gameMsg);
	// �Ͽ�����
	void disconnectedSignal();
	// ����ʧ��
	void connectedFaliedSignal();

private slots:
	// ��ȡ����
	void onReadyRead();
	// ��������
	void sendToClient(const GameMessage& gameMsg);
	// ��ʱ���
	void onTimeoutCheck();
};



