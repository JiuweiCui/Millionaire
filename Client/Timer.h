#pragma once
#include <QGridLayout>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QTimer>
#include <qframe.h>
#include "Constants.h"

// 倒计时界面
class CTimer : public QObject {
	Q_OBJECT

public:
	
	static CTimer* getInstance();
	
	~CTimer();
	CTimer(const CTimer&) = delete;
	CTimer& operator=(const CTimer&) = delete;

private:
	int m_restTime;  // 剩余时间
	QLabel* textptr = nullptr;
	QLabel* restTimeptr = nullptr;
	QTimer* timerptr = nullptr;
	QPushButton* passBtnptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	explicit CTimer(QObject* parent = nullptr);
	void initWindow();

public:
	QWidget* getWidget() const;
	void start();
	void setEnabled(bool flg);

signals:
	void stopSignal(MYSIGNAL signal);
};

// 掷骰子界面
class CDice : public QObject {
	Q_OBJECT

public:
	
	static CDice* getInstance();

	~CDice();
	CDice(const CDice&) = delete;
	CDice& operator=(const CDice&) = delete;

private:
	QLabel* textptr = nullptr;
	QLabel* numptr = nullptr;
	QPushButton* rollBtnptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	explicit CDice(QObject* parent = nullptr);
	void initWindow();

public:
	QWidget* getWidget() const;
	void setRoll(bool flg);
	void setStep(int step);

signals:
	void haveMoveSignal(MYSIGNAL signal);
};

