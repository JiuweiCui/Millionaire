#pragma once
#include <qwidget.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "Constants.h"

// �հ׿�Ƭ��
class CBlankCard : public QFrame {
	Q_OBJECT

public:
	explicit CBlankCard(int number, int type, QWidget* parent = nullptr);
	~CBlankCard();

private:
	int m_num;
	int m_type;

	QLabel* pText = nullptr;
	QGridLayout* pLayout = nullptr;

private:
	void initWindow();

public:
	void setColor(int type);
};