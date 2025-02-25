#pragma once
#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qframe.h>
#include "Constants.h"

// ������
class CGrid : public QFrame {
	Q_OBJECT

public:
	explicit CGrid(int id, int players, QWidget* parent = nullptr);
	~CGrid();

private:
	int m_id;
	int m_players;
	VINT vIsPlayers;
	std::vector<QLabel*> vLabelPtr;
	QPushButton* pHouseBtn = nullptr;
	QGridLayout* pPlayerArea = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initVariable();                   // ��ʼ������
	void initWindow();                     // ��ʼ������
	void initPos(int j, QGridLayout* ptr); // ��ʼ��λ��

public:
	void setBtnColor(int id);    // ���ð�ť��ɫ
	void addPlayer(COLOR color); // �������
	void subPlayer(COLOR color); // �������

signals:
	void showHouseInfoSignal(int house_id);
};
