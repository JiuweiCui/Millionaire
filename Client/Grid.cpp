#include "Grid.h"


CGrid::CGrid(int id, int players, QWidget* parent)
	: m_id(id),
	m_players(players),
	QFrame(parent)
{
	initVariable();
	initWindow();
}

CGrid::~CGrid()
{
	for (auto ptr : vLabelPtr) {
		FREE_PTR(ptr);
	}
	FREE_PTR(pHouseBtn);
	FREE_PTR(pPlayerArea);
	FREE_PTR(pLayout);
}

void CGrid::initVariable()
{
	vIsPlayers.resize(m_players, 0);
}

void CGrid::initPos(int j, QGridLayout* ptr)
{
	QFont font;
	font.setFamily("Times New Roman");
	font.setPointSize(8);
	font.setBold(true);
	QLabel* pLabel = new QLabel("");
	pLabel->setAlignment(Qt::AlignCenter);
	pLabel->setStyleSheet("border: none;");
	pLabel->setFont(font);
	ptr->addWidget(pLabel, 0, j);
	vLabelPtr.push_back(pLabel);
}

void CGrid::initWindow()
{
	// 方格名字
	QFont font;
	font.setFamily("楷体");
	font.setPointSize(6);
	font.setBold(true);
	pHouseBtn = new QPushButton(vHouseInfos[m_id].name);
	pHouseBtn->setFont(font);
	pHouseBtn->setStyleSheet("border: none;");
	connect(pHouseBtn, &QPushButton::clicked, [&]() {
		emit showHouseInfoSignal(m_id);
	});

	// 初始化玩家区
	pPlayerArea = new QGridLayout;
	for (int j = 0; j < MAX_PLAYERS; j++) {
		initPos(j, pPlayerArea);
	}

	pLayout = new QGridLayout;
	pLayout->addWidget(pHouseBtn, 0, 0);
	pLayout->addLayout(pPlayerArea, 1, 0);
	setLayout(pLayout);
	QString style =
		"CGrid { "
		"border: none;"
		"border-radius: 5px;"
		"background-color: rgba(255, 255, 255, 0.6);"
		"}";
	setStyleSheet(style);
}

void CGrid::setBtnColor(int id)
{
	QString style = "border: 1px solid black; background-color: " + vColorStr[id] + ";";
	pHouseBtn->setStyleSheet(style);
}

void CGrid::addPlayer(COLOR color)
{
	vIsPlayers[int(color)] = 1;
	for (int i = 0, cnt = 0; i < vIsPlayers.size(); i++) {
		if (vIsPlayers[i] == 1) {
			QString style = "border: none; border-radius: 5px; background-color: " + vColorStr[i] + ";";
			vLabelPtr[cnt]->setStyleSheet(style);
			vLabelPtr[cnt]->setText(QString::number(cnt + 1));
			cnt++;
		}
	}
}

void CGrid::subPlayer(COLOR color)
{
	vIsPlayers[int(color)] = 0;
	int cnt = 0;
	for (int i = 0; i < vIsPlayers.size(); i++) {
		if (vIsPlayers[i] == 1) {
			QString style = "border: none; border-radius: 5px; background-color: " + vColorStr[i] + ";";
			vLabelPtr[cnt]->setStyleSheet(style);
			vLabelPtr[cnt]->setText(QString::number(cnt + 1));
			cnt++;
		}
	}
	for (int i = cnt; i < m_players; i++) {
		vLabelPtr[i]->setStyleSheet("border: none;");
		vLabelPtr[i]->setText("");
	}
} 





