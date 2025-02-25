#include <qgridlayout.h>
#include <unordered_map>
#include <qlabel.h>
#include <qpushbutton.h>
#include <unordered_set>
#include "Grid.h"

using UMAP = std::unordered_map<int, CGrid*>;
using USET = std::unordered_set<int>;

// չʾ��Ϣ��
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


// �����
class CPlayers : public QFrame {
	Q_OBJECT

public:
	explicit CPlayers(int my_id_, int your_id_, int money, QWidget* parent = nullptr);
	~CPlayers();

private:
	int my_id;       // ����Լ���id
	int your_id;     // ������ҵ�id
	int m_restMoney; // ʣ��Ǯ��
	VINT vHouseId;
	QLabel* pPlayerId = nullptr;      // ��ʾ���ID
	CInfo* pInfo = nullptr;           // �����ť��ʾ�����ϸ��Ϣ
	QLabel* pText = nullptr;          // ����ı�
	QLabel* pMoney = nullptr;         // ������
	QGridLayout* pLayout = nullptr;   // ����

private:
	void initWindow();

public:
	int getMoney() const;
	void addMoney(const int& money);
	void subMoney(const int& money);
	void addHouse(const int& id);
	void setOut(); // �Ʋ�

private slots:
	void showInfo(); // չʾ��Ϣ
};




