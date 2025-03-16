#include "Player.h"

QVector<CPlayers*> CPlayers::vPlayersptr;

CPlayers::CPlayers(int id_, int init_money, QObject* parent)
    : m_id(id_),
    m_restMoney(init_money),
    QObject(parent)
{
    initWindow();
}

CPlayers::~CPlayers()
{
    FREE_PTR(playerIdptr);
    FREE_PTR(restMoneyTextptr);
    FREE_PTR(restMoneyptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

void CPlayers::initWindow()
{
    QFont font;
    font.setFamily(CPLAYERS_CHN_FAMILY);
    font.setPointSize(CPLAYERS_CHN_SIZE);
    font.setBold(true);

    playerIdptr = new QLabel("玩家" + QString::number(m_id + 1));
    playerIdptr->setAlignment(Qt::AlignCenter);
    playerIdptr->setFont(font);
    
    restMoneyTextptr = new QLabel("余额");
    restMoneyTextptr->setAlignment(Qt::AlignCenter);
    restMoneyTextptr->setFont(font);

    font.setFamily(CPLAYERS_ENG_FAMILY);
    font.setPointSize(CPLAYERS_ENG_SIZE);
    restMoneyptr = new QLabel(QString::number(m_restMoney));
    restMoneyptr->setAlignment(Qt::AlignCenter);
    restMoneyptr->setFont(font);

    layoutptr = new QGridLayout;
    layoutptr->addWidget(playerIdptr, 0, 0);
    layoutptr->addWidget(restMoneyTextptr, 1, 0);
    layoutptr->addWidget(restMoneyptr, 2, 0);

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CPLAYERS_STYLE);
}

QWidget* CPlayers::getWidget() const
{
    return frameptr;
}

int CPlayers::getRestMoney() const
{
    return m_restMoney;
}

int CPlayers::getAllMoney() const
{
    int sumMoney = m_restMoney;
    for (int house_id : houseSet) {
        CGrid* gridptr = CGrid::vGridptr[house_id];
        if (!gridptr->getMortStatus()) {
            sumMoney += gridptr->getGroundPrice() / 2;
            sumMoney += gridptr->getHousePrice() * gridptr->getHouses() / 2;
        }
    }

    return sumMoney;
}

int CPlayers::getHouses() const
{
    int sumHouses = 0;
    for (int house_id : houseSet) {
        CGrid* gridptr = CGrid::vGridptr[house_id];
        sumHouses += gridptr->getHouses();
    }

    return sumHouses;
}

void CPlayers::addMoney(const int& money)
{
    m_restMoney += money;
    restMoneyptr->setText(QString::number(m_restMoney));
}

void CPlayers::subMoney(const int& money)
{
    m_restMoney -= money;
    restMoneyptr->setText(QString::number(m_restMoney));
}

void CPlayers::addHouse(const int& house_id)
{
    houseSet.insert(house_id);
}

void CPlayers::subHouse(const int& house_id)
{
    houseSet.remove(house_id);
}

void CPlayers::setOut()
{
    houseSet.clear();
    m_restMoney = 0;
    restMoneyptr->setText(QString::number(m_restMoney));
    frameptr->setStyleSheet(CPLAYERS_OUT_STYLE);
}
