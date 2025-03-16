#include "NetworkManager.h"

NetworkManager::NetworkManager(QTcpSocket* socket, QThread* parent)
	: pSocket(socket),
	QThread(parent)
{
	moveToThread(this);
	init();
}

NetworkManager::~NetworkManager()
{
	m_running = false;

	if (pTimer) {
		pTimer->stop();
		pTimer->deleteLater();
	}
}

void NetworkManager::init()
{
	connect(pSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
	connect(pSocket, &QTcpSocket::disconnected, [this]() {
		emit disconnectedSignal();
		return;
	});

	// ��ʼ����ʱ��ⶨʱ��
	pTimer = new QTimer;
	pTimer->moveToThread(this);
	connect(this, &NetworkManager::started, [this]() {
		//pTimer->start(TURN_TIME);
	});
	connect(pTimer, &QTimer::timeout, this, &NetworkManager::onTimeoutCheck);
	qDebug() << "This:" << QThread::currentThread();
}

// ������Ϣ
QByteArray NetworkManager::createMessage(const GameMessage& gameMsg)
{
	QByteArray msg;
	QDataStream out(&msg, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_8);

	out << gameMsg.type << gameMsg.seqNumber
		<< gameMsg.pos << gameMsg.data1 << gameMsg.data2;

	return msg;
}

// ��ȡ��Ϣ
GameMessage NetworkManager::readMessage(const QByteArray& msg)
{
	QDataStream in(msg);
	in.setVersion(QDataStream::Qt_6_8);
	GameMessage gameMsg;
	in >> gameMsg.type >> gameMsg.seqNumber
		>> gameMsg.pos >> gameMsg.data1 >> gameMsg.data2;

	return gameMsg;
}

bool NetworkManager::getRunning() const
{
	return m_running;
}

// ����Ϣ���͸����̣߳����̣߳�
void NetworkManager::onReadyRead()
{
	if (!pSocket) {
		return;
	}

	GameMessage rcvdMsg = readMessage(pSocket->readAll());

	if (rcvdMsg.type == MYSIGNAL::ACK) {
		QMutexLocker locker(&m_mutex);
		pendingMsgMap.remove(rcvdMsg.seqNumber - 1);
		return;
	}

	if (ackNumber == -1 || ackNumber == rcvdMsg.seqNumber) { // ��������������Ϣ
		ackNumber = rcvdMsg.seqNumber + 1; // ��һ����Ҫ����Ϣ���	
		emit sendMessageSignal(rcvdMsg);
	}

	GameMessage ackMsg = { MYSIGNAL::ACK, ackNumber, 0, 0, 0 };
	qDebug() << "ACK:" << rcvdMsg.seqNumber;
	pSocket->write(createMessage(ackMsg));
	pSocket->flush();
}

// ������Ϣ��д�̣߳�
void NetworkManager::sendToServer(const GameMessage& gameMsg)
{
	int seq = gameMsg.seqNumber;
	PendingMessage pm = { createMessage(gameMsg), QDateTime::currentDateTime(), 0 };
	
	// ��С��������
	{
		QMutexLocker locker(&m_mutex);
		pendingMsgMap[seq] = pm;
	}

	// ��������
	if (pSocket && pSocket->state() == QAbstractSocket::ConnectedState) {
		qDebug() << "Write:" << int(gameMsg.type) << " " << gameMsg.seqNumber;
		pSocket->write(pm.rawData);
		pSocket->flush();
	}
}

// ��ʱ��⣨д�̣߳�
void NetworkManager::onTimeoutCheck()
{
	if (!m_running) {
		return;
	}

	QMutexLocker locker(&m_mutex);

	QMutableHashIterator<int, PendingMessage> it(pendingMsgMap);
	while (it.hasNext()) {
		auto nowTime = QDateTime::currentDateTime();
		auto& pm = it.next().value();

		// ��鳬ʱ
		if (pm.lastTime.msecsTo(nowTime) > ACK_TIMEOUT) {
			if (pm.retryCount >= MAX_RETRY) {
				it.remove();
				continue;
			}

			// �ش�
			pSocket->write(pm.rawData);
			pm.retryCount++;
			pm.lastTime = nowTime;
		}
	}
	pTimer->start(TURN_TIME);
}

