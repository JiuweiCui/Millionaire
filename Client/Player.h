#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qset.h>
#include "Grid.h"

// �����
class CPlayers : public QObject {
	Q_OBJECT

public:
	explicit CPlayers(int id_, int init_money, QObject* parent = nullptr);
	~CPlayers();

	static QVector<CPlayers*> vPlayersptr;

private:
	int m_id;                             // ���ID
	int m_restMoney;                      // ʣ��Ǯ��
	QSet<int> houseSet;                   // ���ӵ�еķ���ID
	QLabel* playerIdptr = nullptr;        // ��ʾ���ID
	QLabel* restMoneyTextptr = nullptr;   // ���
	QLabel* restMoneyptr = nullptr;       // ������
	QGridLayout* layoutptr = nullptr;     // ����
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
	void setOut(); // �Ʋ�
};




