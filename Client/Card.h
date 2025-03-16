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

// ��Ƭ�洢����
class CCard : public QObject {
	Q_OBJECT

public:
	~CCard();
	static CCard* getInstance();

	CCard(const CCard&) = delete;
	CCard& operator=(const CCard&) = delete;
private:
	QLabel* infoTextptr = nullptr;       // �ı���ʾ
	QPushButton* sellBtnptr = nullptr;   // ����
	QPushButton* mortBtnptr = nullptr;   // ��Ѻ
	QPushButton* backBtnptr = nullptr;   // ���

	QHBoxLayout* hlayoutptr = nullptr;
	QWidget* containerptr = nullptr;
	QScrollArea* scrollAreaptr = nullptr;

	QGridLayout* layoutptr = nullptr;
	QQueue<CBlankCard*> vBlankptr;
	QVector<CHouseCard*> vCardptr;
	CHouseCard* currentptr = nullptr; // ��ǰѡ�еĿ�Ƭ
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

