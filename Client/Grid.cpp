#include "Grid.h"

QVector<CGrid*> CGrid::vGridptr;
QVector<int> CGrid::vPlayersPos;
int CGrid::players = 0;

CGrid::CGrid(int grid_id, QObject* parent)
	: m_id(grid_id),
	QObject(parent)
{
	initWindow();
}

CGrid::~CGrid()
{
	for (auto ptr : vPlayersptr) {
		FREE_PTR(ptr);
	}
	FREE_PTR(houseBtnptr);
	FREE_PTR(layoutptr);
	FREE_PTR(frameptr);
}

// 移动玩家
bool CGrid::movePlayer(int playerId, int moveStep)
{
	int pastPos = vPlayersPos[playerId];
	int newPos = (pastPos + moveStep) % BOARD_COUNT;
	vPlayersPos[playerId] = newPos;

	vGridptr[pastPos]->updatePlayer();
	vGridptr[newPos]->updatePlayer();

	return newPos < pastPos;  // 是否经过了起点
}

bool CGrid::goCertainPos(int playerId, int pos)
{
	int pastPos = vPlayersPos[playerId];  // 玩家当前位置
	vPlayersPos[playerId] = pos;
	vGridptr[pastPos]->updatePlayer();
	vGridptr[pos]->updatePlayer();
	return pastPos > pos;
}

// 移除玩家
void CGrid::removePlayer(int playerId)
{
	int pos = vPlayersPos[playerId];
	vPlayersPos[playerId] = -1;
	vGridptr[pos]->updatePlayer();
}

// 复原
void CGrid::setCardInit(int player_id)
{
	for (int i = 0; i < BOARD_COUNT; i++) {
		auto gridptr = CGrid::vGridptr[i];
		if (gridptr->getHouseOwner() == player_id) {
			gridptr->setHouseOwner(-1);
		}
	}
}

CGrid& CGrid::operator=(const HouseInfo& houseInfo)
{
	m_name = houseInfo.name;
	m_ownerId = -1;
	for (int i = 0; i < 6; i++) {
		vTollPrice[i] = houseInfo.vTollPrice[i];
	}
	m_groundPrice = houseInfo.groundPrice;
	m_housePrice = houseInfo.housePrice;
	m_houses = 0;
	m_mortgage = houseInfo.mortgage;
	m_isMort = 0;

	return *this;
}

void CGrid::initPosArea(int col, QGridLayout* ptr)
{
	QFont font;
	font.setFamily(CGRID_POS_FONT_FAMILY);
	font.setPointSize(CGRID_POS_FONT_SIZE);
	font.setBold(true);

	QLabel* label = new QLabel("");
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet(CGRID_STYLE_NONE);
	label->setFont(font);
	ptr->addWidget(label, 1, col);
	vPlayersptr.push_back(label);
}

void CGrid::initWindow()
{
	*this = vHouseInfos[m_id];

	// 方格名字
	QFont font;
	font.setFamily(CGRID_HOUSE_FONT_FAMILY);
	font.setPointSize(CGRID_HOUSE_FONT_SIZE);
	font.setBold(true);

	// 初始化点击按钮
	houseBtnptr = new QPushButton(vHouseInfos[m_id].name);
	houseBtnptr->setFont(font);
	houseBtnptr->setStyleSheet(CGRID_STYLE_NONE);
	connect(houseBtnptr, &QPushButton::clicked, [&]() {
		emit showHouseInfoSignal(m_id);
	});

	layoutptr = new QGridLayout;
	layoutptr->addWidget(houseBtnptr, 0, 0, 1, MAX_PLAYERS);

	// 初始化玩家区
	for (int col = 0; col < MAX_PLAYERS; col++) {
		initPosArea(col, layoutptr);
	}

	frameptr = new QFrame;
	frameptr->setLayout(layoutptr);
	frameptr->setStyleSheet(CGRID_STYLE);
}

// 更新玩家
void CGrid::updatePlayer()
{
	int cnt = 0;
	for (int i = 0; i < players; i++) {
		if (vPlayersPos[i] == m_id) {
			QString style = "border: none; border-radius: 5px; background-color: " + vColorStr[i] + ";";
			vPlayersptr[cnt]->setStyleSheet(style);
			vPlayersptr[cnt]->setText(QString::number(cnt + 1));
			cnt++;
		}
	}
	for (int i = cnt; i < players; i++) {
		vPlayersptr[cnt]->setStyleSheet(CGRID_STYLE_NONE);
		vPlayersptr[cnt]->setText("");
	}
}

QWidget* CGrid::getWidget() const
{
	return frameptr;
}

void CGrid::setHouseOwner(int player_id)
{
	m_ownerId = player_id;
	m_houses = 0;
}

void CGrid::addHouse()
{
	++m_houses;
}

void CGrid::setMortStatus(bool flg)
{
	m_isMort = flg;
}

QString CGrid::getHouseName() const
{
	return m_name;
}

int CGrid::getHouseId() const
{
	return m_id;
}

inline int CGrid::getHouseOwner() const
{
	return m_ownerId;
}

int CGrid::getHouseToll() const
{
	return vTollPrice[m_houses];
}

int CGrid::getGroundPrice() const
{
	return m_groundPrice;
}

int CGrid::getHousePrice() const
{
	return m_housePrice;
}

int CGrid::getHouses() const
{
	return m_houses;
}

int CGrid::getMortgage() const
{
	return m_mortgage;
}

bool CGrid::getMortStatus() const
{
	return m_isMort;
}

House CGrid::getHouseInfo()
{
	House house;
	house.m_id = m_id;
	house.m_name = m_name;
	house.m_ownerId = m_ownerId;
	for (int i = 0; i < 6; i++) {
		house.vTollPrice[i] = vTollPrice[i];
	}
	house.m_groundPrice = m_groundPrice;
	house.m_housePrice = m_housePrice;
	house.m_houses = m_houses;
	house.m_mortgage = m_mortgage;
	house.m_isMort = m_isMort;

	return house;
}
