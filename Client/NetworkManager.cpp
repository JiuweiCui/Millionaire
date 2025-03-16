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

	// 初始化超时检测定时器
	pTimer = new QTimer;
	pTimer->moveToThread(this);
	connect(this, &NetworkManager::started, [this]() {
		//pTimer->start(TURN_TIME);
	});
	connect(pTimer, &QTimer::timeout, this, &NetworkManager::onTimeoutCheck);
	qDebug() << "This:" << QThread::currentThread();
}

// 创建消息
QByteArray NetworkManager::createMessage(const GameMessage& gameMsg)
{
	QByteArray msg;
	QDataStream out(&msg, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_6_8);

	out << gameMsg.type << gameMsg.seqNumber
		<< gameMsg.pos << gameMsg.data1 << gameMsg.data2;

	return msg;
}

// 读取消息
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

// 将消息发送给主线程（读线程）
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

	if (ackNumber == -1 || ackNumber == rcvdMsg.seqNumber) { // 本次所期望的消息
		ackNumber = rcvdMsg.seqNumber + 1; // 下一次需要的消息序号	
		emit sendMessageSignal(rcvdMsg);
	}

	GameMessage ackMsg = { MYSIGNAL::ACK, ackNumber, 0, 0, 0 };
	qDebug() << "ACK:" << rcvdMsg.seqNumber;
	pSocket->write(createMessage(ackMsg));
	pSocket->flush();
}

// 发送消息（写线程）
void NetworkManager::sendToServer(const GameMessage& gameMsg)
{
	int seq = gameMsg.seqNumber;
	PendingMessage pm = { createMessage(gameMsg), QDateTime::currentDateTime(), 0 };
	
	// 减小锁的粒度
	{
		QMutexLocker locker(&m_mutex);
		pendingMsgMap[seq] = pm;
	}

	// 立即发送
	if (pSocket && pSocket->state() == QAbstractSocket::ConnectedState) {
		qDebug() << "Write:" << int(gameMsg.type) << " " << gameMsg.seqNumber;
		pSocket->write(pm.rawData);
		pSocket->flush();
	}
}

// 超时检测（写线程）
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

		// 检查超时
		if (pm.lastTime.msecsTo(nowTime) > ACK_TIMEOUT) {
			if (pm.retryCount >= MAX_RETRY) {
				it.remove();
				continue;
			}

			// 重传
			pSocket->write(pm.rawData);
			pm.retryCount++;
			pm.lastTime = nowTime;
		}
	}
	pTimer->start(TURN_TIME);
}

