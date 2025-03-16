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

	// �Ƿ�����
	bool getRunning() const;

private:
	QTcpSocket* pSocket = nullptr;             // ��д�̰߳�ȫ
	int ackNumber = -1;                        // ȷ�����
	QTimer* pTimer = nullptr;                  // ��ʱ��
	QHash<int, PendingMessage> pendingMsgMap;  // ��Ϣ����
	QMutex m_mutex;                            // ������������ϣ��
	std::atomic<bool> m_running{ true };       // �߳����б�־

private:
	//��ʼ��
	void init();
	// ������Ϣ
	QByteArray createMessage(const GameMessage& gameMsg);
	// ��ȡ��Ϣ
	GameMessage readMessage(const QByteArray& msg);

signals:
	// ���Ϳͻ�����Ϣ�����߳�
	void sendMessageSignal(const GameMessage& gameMsg);
	// �Ͽ�����
	void disconnectedSignal();

private slots:
	// ��ȡ����
	void onReadyRead();
	// ��������
	void sendToServer(const GameMessage& gameMsg);
	// ��ʱ���
	void onTimeoutCheck();
};




