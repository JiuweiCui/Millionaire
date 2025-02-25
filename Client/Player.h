#include <qgridlayout.h>
#include <unordered_map>
#include <qlabel.h>
#include <qpushbutton.h>
#include <unordered_set>
#include "Grid.h"

using UMAP = std::unordered_map<int, CGrid*>;
using USET = std::unordered_set<int>;

// 展示信息类
class CInfo : public QWidget {
public:

	CInfo(int money, VINT vId, QWidget* parent = nullptr);
	~CInfo();

private:
	void initWindow();

private:
	int nMoney;
	VINT vHouseId;
	QGridLayout* pLayout = nullptr;
};


// 玩家类
class CPlayers : public QFrame {
	Q_OBJECT

public:
	explicit CPlayers(int my_id_, int your_id_, int money, QWidget* parent = nullptr);
	~CPlayers();

private:
	int my_id;       // 玩家自己的id
	int your_id;     // 其他玩家的id
	int m_restMoney; // 剩余钱数
	VINT vHouseId;
	QLabel* pPlayerId = nullptr;      // 显示玩家ID
	CInfo* pInfo = nullptr;           // 点击按钮显示玩家详细信息
	QLabel* pText = nullptr;          // 余额文本
	QLabel* pMoney = nullptr;         // 玩家余额
	QGridLayout* pLayout = nullptr;   // 布局

private:
	void initWindow();

public:
	int getMoney() const;
	void addMoney(const int& money);
	void subMoney(const int& money);
	void addHouse(const int& id);
	void setOut(); // 破产

private slots:
	void showInfo(); // 展示信息
};




