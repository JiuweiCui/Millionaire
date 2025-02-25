#pragma once

#include <QWidget>
#include <qgridlayout.h>
#include <qlabel.h>
#include <QScrollArea>
#include <QHBoxLayout>
#include <qpushbutton.h>
#include <deque>
#include "Constants.h"

// 房子类
class CHouse : public QWidget {
	Q_OBJECT

public:
	explicit CHouse(int id, int o_id, int houses, QWidget* parent = nullptr);
	~CHouse();

	CHouse& operator=(const HouseInfo& houseInfo);

private:
	int m_id;          // 房子ID
	int m_ownId;       // 拥有该房子的玩家ID
	QString m_name;    // 名字
	int vTollPrice[6]; // 过路费
	int m_buyGround;   // 地皮价格
	int m_buyHouse;    // 盖房价格
	int m_houses;      // 当前房子数量
	int m_mortgage;    // 抵押
	int isMort;        // 是否抵押

private:
	void initVariable();

public:
	void setOwner(int player_id);
	void addHouse();
	void setMortStatus(bool flg);

	QString getName() const;
	int getHouseId() const;
	int getOwner() const;
	int getToll() const;
	int getBuyGound() const;
	int getBuyHouse() const;
	int getHouses() const;
	int getMortgage() const;
	bool getStatus() const;
};

// 房子信息展示类
class CHouseInfo : public QFrame {
	Q_OBJECT

public:
	explicit CHouseInfo(int size, int type, QWidget* parent = nullptr);
	~CHouseInfo();

	CHouseInfo& operator=(const CHouse& house);

public:
	int m_id;          // 房子ID
	int m_ownId;       // 拥有该房子的玩家ID
	int vTollPrice[6]; // 过路费
	int m_buyGround;   // 地皮价格
	int m_buyHouse;    // 盖房价格
	int m_houses;      // 当前房子数量
	int m_mortgage;    // 抵押
	int m_isMort;      // 是否抵押

private:
	QFont font;
	int m_type;
	int m_size;

	QLabel* pName = nullptr;
	QLabel* pOwn = nullptr;
	
	QLabel* pBuyGround = nullptr;
	std::vector<QLabel*> pToll;
	QLabel* pBuyHouse = nullptr;
	QLabel* pHouses = nullptr;
	QLabel* pMort = nullptr;
	QLabel* pIsMort = nullptr;

	QLabel* pBuyGroundN = nullptr;
	std::vector<QLabel*> pTollN;
	QLabel* pBuyHouseN = nullptr;
	QLabel* pHousesN = nullptr;
	QLabel* pMortN = nullptr;
	QLabel* pIsMortN = nullptr;
	
	QGridLayout* pLayout = nullptr;

	bool isSelected;

private:
	void initWindow();

public:
	void show(const CHouse& house);
	void updateHouses();
	void setSelected(bool selected);
	void updateMort();

protected:
	void mousePressEvent(QMouseEvent *event) override;

signals:
	void clicked(CHouseInfo *card, bool selected);
};

class CBlank : public QFrame {
	Q_OBJECT

public:
	explicit CBlank(int number, QWidget* parent = nullptr);
	~CBlank();

private:
	QLabel* pText = nullptr;
	QGridLayout* pLayout = nullptr;

public:
	void setColor();
};

// 卡片存储界面
class CCard : public QFrame {
	Q_OBJECT

public:
	explicit CCard(QWidget* parent = nullptr);
	~CCard();

private:
	QLabel* pInfo = nullptr;  // 文本提示
	QPushButton* pSell = nullptr;  // 出售
	QPushButton* pMort = nullptr;  // 抵押
	QPushButton* pBuy = nullptr;   // 买回

	QHBoxLayout* pHlayout = nullptr;
	QWidget* pContainer = nullptr;
	QScrollArea* pScrollArea = nullptr;
	
	QGridLayout* pLayout = nullptr;
	std::deque<CBlank*> pBlank;
	std::vector<CHouseInfo*> pCard;
	CHouseInfo* pCurrent = nullptr;

private:
	void initWindow();

public:
	void addCard(const CHouse& house);
	void subCard(CHouseInfo* card = nullptr);
	void updateMort(bool flg);
	void updateHouses(int house_id);
	void clearCard();
	int getHouseId() const;

signals:
	void opHouseSignal(MYSIGNAL signal);	

private slots:
	void onCardClicked(CHouseInfo *card, bool selected);
};

