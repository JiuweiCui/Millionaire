#pragma once
#include <qgridlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qframe.h>
#include "Constants.h"

// 方格类
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
	void initVariable();                   // 初始化变量
	void initWindow();                     // 初始化布局
	void initPos(int j, QGridLayout* ptr); // 初始化位置

public:
	void setBtnColor(int id);    // 设置按钮颜色
	void addPlayer(COLOR color); // 增加玩家
	void subPlayer(COLOR color); // 减少玩家

signals:
	void showHouseInfoSignal(int house_id);
};
