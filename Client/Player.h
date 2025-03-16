#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qset.h>
#include "Grid.h"

// 玩家类
class CPlayers : public QObject {
	Q_OBJECT

public:
	explicit CPlayers(int id_, int init_money, QObject* parent = nullptr);
	~CPlayers();

	static QVector<CPlayers*> vPlayersptr;

private:
	int m_id;                             // 玩家ID
	int m_restMoney;                      // 剩余钱数
	QSet<int> houseSet;                   // 玩家拥有的房子ID
	QLabel* playerIdptr = nullptr;        // 显示玩家ID
	QLabel* restMoneyTextptr = nullptr;   // 余额
	QLabel* restMoneyptr = nullptr;       // 玩家余额
	QGridLayout* layoutptr = nullptr;     // 布局
	QFrame* frameptr = nullptr;

private:
	void initWindow();

public:
	QWidget* getWidget() const;
	int getRestMoney() const;
	int getAllMoney() const;
	int getHouses() const;
	void addMoney(const int&);
	void subMoney(const int&);
	void addHouse(const int&);
	void subHouse(const int&);
	void setOut(); // 破产
};




