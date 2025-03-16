#pragma once
#include <qwidget.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <QHBoxLayout>
#include <qpushbutton.h>
#include <qqueue.h>
#include "HouseShow.h"
#include "BlankCard.h"
#include "HouseCard.h"
#include "Grid.h"

// 卡片存储界面
class CCard : public QObject {
	Q_OBJECT

public:
	~CCard();
	static CCard* getInstance();

	CCard(const CCard&) = delete;
	CCard& operator=(const CCard&) = delete;
private:
	QLabel* infoTextptr = nullptr;       // 文本提示
	QPushButton* sellBtnptr = nullptr;   // 出售
	QPushButton* mortBtnptr = nullptr;   // 抵押
	QPushButton* backBtnptr = nullptr;   // 买回

	QHBoxLayout* hlayoutptr = nullptr;
	QWidget* containerptr = nullptr;
	QScrollArea* scrollAreaptr = nullptr;

	QGridLayout* layoutptr = nullptr;
	QQueue<CBlankCard*> vBlankptr;
	QVector<CHouseCard*> vCardptr;
	CHouseCard* currentptr = nullptr; // 当前选中的卡片
	QFrame* frameptr = nullptr;

private:
	explicit CCard(QObject* parent = nullptr);
	void initWindow();

public:
	void addCard(const House& house);
	void subCard(CHouseCard* card = nullptr);
	void updateHouses(int house_id, int houses);
	void updateMort(bool mort);
	void clearCard();
	int getHouseId() const;
	QWidget* getWidget() const;

signals:
	void opHouseSignal(MYSIGNAL signal);

private slots:
	void onCardClicked(CHouseCard* card, bool selected);
};

