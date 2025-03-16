#pragma once
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <QTextBrowser>
#include <qframe.h>
#include <qlabel.h>
#include <qwidget.h>
#include "Constants.h"

// ��Ϣ�����
class CShowArea : public QObject {
	Q_OBJECT

public:
	static CShowArea* getInstance();
	
	~CShowArea();
	CShowArea(const CShowArea&) = delete;
	CShowArea& operator=(const CShowArea&) = delete;

private:
	QLabel* textptr = nullptr;
	QLabel* contentptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	explicit CShowArea(QObject* parent = nullptr);
	void initWindow();

public:
	QWidget* getWidget() const;
	void showInfo(const QString& str = "");
};


// ȷ����Ϣ��
class CInfoArea : public QObject {
	Q_OBJECT

public:
	static CInfoArea* getInstance();
	
	~CInfoArea();
	CInfoArea(const CInfoArea&) = delete;
	CInfoArea& operator=(const CInfoArea&) = delete;

private:	
	int m_type;
	QLabel* textptr = nullptr;
	QLabel* contentptr = nullptr;
	QPushButton* okBtnptr = nullptr;
	QPushButton* celBtnptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	explicit CInfoArea(QObject* parent = nullptr);
	void initWindow();

public:
	QWidget* getWidget() const;
	bool getEnabled() const;
	void setType(const int& type, const QString& name = "");
	void clear();

signals:
	void haveDownSignal(int type, int choose);
};