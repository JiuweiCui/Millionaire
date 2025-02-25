#pragma once
#include <QGridLayout>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QTimer>
#include <qframe.h>
#include "Constants.h"

// 倒计时界面
class CTimer : public QFrame {
	Q_OBJECT

public:
	explicit CTimer(int time, QWidget* parent = nullptr);
	~CTimer();

private:
	int m_rest_time;
	int m_time;
	QLabel* pText = nullptr;
	QLabel* pNum = nullptr;
	QTimer* pTime = nullptr;
	QPushButton* pPassBtn = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();

public:
	void start();
	void setEnabled(bool flg);

signals:
	void stopSignal(MYSIGNAL signal);
};

// 掷骰子界面
class CDice : public QFrame {
	Q_OBJECT

public:
	explicit CDice(QWidget* parent = nullptr);
	~CDice();

private:
	QLabel* pText = nullptr;
	QLabel* pNum = nullptr;
	QPushButton* pRollBtn = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();

public:
	void setRoll(bool flg);
	void setStep(int step);

signals:
	void haveMoveSignal(MYSIGNAL signal);
};

