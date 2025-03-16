#pragma once
#include <qwidget.h>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include "BlankCard.h"
#include "Constants.h"

// 开始界面
class CStart : public QFrame {
	Q_OBJECT

public:
	explicit CStart(int id, QWidget* parent = nullptr);
	~CStart();
	CStart(const CStart&) = delete;
	CStart& operator=(const CStart&) = delete;

private:
	int m_id;
	QLabel* logoptr = nullptr;
	QLabel* textptr = nullptr;
	QPushButton* okBtnptr = nullptr;
	QPushButton* celBtnptr = nullptr;
	QVector<CBlankCard*> vCardptr;
	QGridLayout* cardLayoutptr = nullptr;
	QGridLayout* layoutptr = nullptr;

private:
	void initWindow();	

public:
	void updatePlayers(int);
	
signals:
	void haveDownSignal(int choose);
};