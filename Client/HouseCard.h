#pragma once
#include <qwidget.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <QScrollArea>
#include <qpushbutton.h>
#include "Grid.h"

// ·¿×Ó¿¨Æ¬Àà
class CHouseCard : public QFrame {
	Q_OBJECT

public:
	explicit CHouseCard(const House& house, QWidget* parent = nullptr);
	~CHouseCard();

private:
	int m_houseid;

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

	bool isSelected;
	bool isMort;

private:
	void initWindow(const House& house);

public:
	void updateHouses(int houses);
	void setSelected(bool selected);
	void updateMort(bool mort);
	int getHouseId() const;
	bool getMortStatus() const;

protected:
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void clicked(CHouseCard* card, bool selected);
};