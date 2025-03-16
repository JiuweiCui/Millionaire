#pragma once
#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbutton.h>
#include "Constants.h"

typedef struct House {
	int m_id;
	QString m_name;    // 名字
	int m_ownerId;     // 拥有该房子的玩家ID
	int vTollPrice[6]; // 过路费
	int m_groundPrice; // 地皮价格
	int m_housePrice;  // 盖房价格
	int m_houses;      // 当前房子数量
	int m_mortgage;    // 抵押费
	int m_isMort;      // 是否抵押
};


// 方格类
class CGrid : public QObject {
	Q_OBJECT

public:
	explicit CGrid(int grid_id, QObject* parent = nullptr);
	~CGrid();

	static QVector<CGrid*> vGridptr;    // 方格指针
	static QVector<int> vPlayersPos;    // 玩家位置
	static int players;                 // 玩家总数
	static bool movePlayer(int, int);   // 移动玩家
	static bool goCertainPos(int, int); // 去指定位置
	static void removePlayer(int);      // 移除玩家
	static void setCardInit(int);       // 玩家卡牌回收

	CGrid& operator=(const HouseInfo& houseInfo);

private:
	int m_id;          // 方格ID
	QString m_name;    // 名字
	int m_ownerId;     // 拥有该房子的玩家ID
	int vTollPrice[6]; // 过路费
	int m_groundPrice; // 地皮价格
	int m_housePrice;  // 盖房价格
	int m_houses;      // 当前房子数量
	int m_mortgage;    // 抵押费
	int m_isMort;      // 是否抵押

	QVector<QLabel*> vPlayersptr;
	QPushButton* houseBtnptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	void initWindow();                     // 初始化布局
	void initPosArea(int, QGridLayout*);   // 初始化位置
	void updatePlayer();                   // 更新玩家

public:
	QWidget* getWidget() const;

	void setHouseOwner(int player_id);
	void addHouse();
	void setMortStatus(bool flg);

	QString getHouseName() const;
	int getHouseId() const;
	inline int getHouseOwner() const;
	int getHouseToll() const;
	int getGroundPrice() const;
	int getHousePrice() const;
	int getHouses() const;
	int getMortgage() const;
	bool getMortStatus() const;
	House getHouseInfo();

signals:
	void showHouseInfoSignal(int house_id);
};
