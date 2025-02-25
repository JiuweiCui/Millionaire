#pragma once
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <QTextBrowser>
#include <qframe.h>
#include <qlabel.h>
#include <qwidget.h>
#include "Constants.h"

// 信息输出类
class CShowArea : public QFrame {
	Q_OBJECT

public:
	explicit CShowArea(QWidget* parent = nullptr);
	~CShowArea();

private:
	QLabel* pConst = nullptr;
	QLabel* pText = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();

public:
	void showInfo(const QString& str = "");
};


// 确认信息类
class CInfoArea : public QFrame {
	Q_OBJECT

public:
	explicit CInfoArea(QWidget* parent = nullptr);
	~CInfoArea();

private:	
	int m_type;
	QLabel* pText = nullptr;
	QPushButton* pOkBtn = nullptr;
	QPushButton* pCelBtn = nullptr;
	QLabel* pLabel = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();

public:
	void showInfo(const QString& str = "");
	void setType(const int& type, const QString& name = "");
	void clear();

signals:
	void haveDownSignal(int type, int choose);
};