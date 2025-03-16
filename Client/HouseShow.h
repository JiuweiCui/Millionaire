#pragma once
#include <qwidget.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "Grid.h"

// 房子信息展示类
class CHouseShow : public QObject {
	Q_OBJECT

public:
	
	static CHouseShow* getInstance();

	~CHouseShow();
	CHouseShow(const CHouseShow&) = delete;
	CHouseShow& operator=(const CHouseShow&) = delete;

private:
	QLabel* nameptr = nullptr;
	QLabel* ownerptr = nullptr;

	QLabel* groundPriceptr = nullptr;
	QVector<QLabel*> vTollptr;
	QLabel* housePriceptr = nullptr;
	QLabel* housesptr = nullptr;
	QLabel* mortptr = nullptr;
	QLabel* isMortptr = nullptr;

	QLabel* groundPriceNptr = nullptr;
	QVector<QLabel*> vTollNptr;
	QLabel* housePriceNptr = nullptr;
	QLabel* housesNptr = nullptr;
	QLabel* mortNptr = nullptr;
	QLabel* isMortNptr = nullptr;

	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	explicit CHouseShow(QObject* parent = nullptr);
	void initWindow();

public:
	QWidget* getWidget() const;
	void showHouseInfo(const House& house);  // 展示房子信息
};