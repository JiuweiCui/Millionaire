#pragma once
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qgridlayout.h>
#include "Constants.h"

class CConfirm : public QDialog {
	Q_OBJECT

public:
	explicit CConfirm(int type, QWidget* parent = nullptr);
	~CConfirm();

	void InitWindow();


private:
	int m_type;
	QLabel* pName = nullptr;
	QPushButton* pOkBtn = nullptr;
	QGridLayout* pLayout = nullptr;
};

bool checkYesNo(int type);