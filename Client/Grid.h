#pragma once
#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbutton.h>
#include "Constants.h"

typedef struct House {
	int m_id;
	QString m_name;    // ����
	int m_ownerId;     // ӵ�и÷��ӵ����ID
	int vTollPrice[6]; // ��·��
	int m_groundPrice; // ��Ƥ�۸�
	int m_housePrice;  // �Ƿ��۸�
	int m_houses;      // ��ǰ��������
	int m_mortgage;    // ��Ѻ��
	int m_isMort;      // �Ƿ��Ѻ
};


// ������
class CGrid : public QObject {
	Q_OBJECT

public:
	explicit CGrid(int grid_id, QObject* parent = nullptr);
	~CGrid();

	static QVector<CGrid*> vGridptr;    // ����ָ��
	static QVector<int> vPlayersPos;    // ���λ��
	static int players;                 // �������
	static bool movePlayer(int, int);   // �ƶ����
	static bool goCertainPos(int, int); // ȥָ��λ��
	static void removePlayer(int);      // �Ƴ����
	static void setCardInit(int);       // ��ҿ��ƻ���

	CGrid& operator=(const HouseInfo& houseInfo);

private:
	int m_id;          // ����ID
	QString m_name;    // ����
	int m_ownerId;     // ӵ�и÷��ӵ����ID
	int vTollPrice[6]; // ��·��
	int m_groundPrice; // ��Ƥ�۸�
	int m_housePrice;  // �Ƿ��۸�
	int m_houses;      // ��ǰ��������
	int m_mortgage;    // ��Ѻ��
	int m_isMort;      // �Ƿ��Ѻ

	QVector<QLabel*> vPlayersptr;
	QPushButton* houseBtnptr = nullptr;
	QGridLayout* layoutptr = nullptr;
	QFrame* frameptr = nullptr;

private:
	void initWindow();                     // ��ʼ������
	void initPosArea(int, QGridLayout*);   // ��ʼ��λ��
	void updatePlayer();                   // �������

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
