#pragma once

#include <qdialog.h>
#include <qwidget.h>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <deque>
#include "House.h"
#include "Constants.h"


class CStart : public QFrame {
	Q_OBJECT

public:
	explicit CStart(int id, QWidget* parent = nullptr);
	~CStart();

private:
	int m_id;
	int m_players;
	QLabel* pText = nullptr;
	QPushButton* pOkBtn = nullptr;
	QPushButton* pCelBtn = nullptr;
	std::vector<CBlank*> vCom;
	QGridLayout* pPlayers = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();	

public:
	void addPlayer();
	int getPlayers();

signals:
	void haveDownSignal(int choose);
};

//bool checkStartGame(int id, CStart* pStart = nullptr);