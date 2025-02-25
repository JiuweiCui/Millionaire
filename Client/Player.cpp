#include "Player.h"

CInfo::CInfo(int money, VINT vId, QWidget* parent)
    : nMoney(money),
    vHouseId(vId),
    QWidget(parent)
{
    initWindow();
}

CInfo::~CInfo()
{
    FREE_PTR(pLayout);
}

void CInfo::initWindow()
{
    pLayout = new QGridLayout;
    QLabel* pMoney = new QLabel(REST_MONEY + QString::number(nMoney));
    pLayout->addWidget(pMoney);

    /*
    for (int nId : vHouseId) {
        // 待实现
    }
    */

    setLayout(pLayout);
}



CPlayers::CPlayers(int my_id_, int your_id_, int money, QWidget* parent)
    : my_id(my_id_),
    your_id(your_id_),
    m_restMoney(money),
    QFrame(parent)
{
    initWindow();
}

CPlayers::~CPlayers()
{
    FREE_PTR(pPlayerId);
    FREE_PTR(pInfo);
    FREE_PTR(pMoney);
}

void CPlayers::initWindow()
{
    QFont font;
    font.setFamily("楷体");
    font.setPointSize(12);
    font.setBold(true);

    pLayout = new QGridLayout;
    pPlayerId = new QLabel("玩家" + QString::number(your_id + 1));
    pPlayerId->setAlignment(Qt::AlignCenter);
    pPlayerId->setFont(font);
    
    pText = new QLabel("余额");
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);

    font.setFamily("Times New Roman");
    pMoney = new QLabel(QString::number(m_restMoney));
    pMoney->setAlignment(Qt::AlignCenter);
    pMoney->setFont(font);

    pLayout->addWidget(pPlayerId, 0, 0);
    pLayout->addWidget(pText, 1, 0);
    pLayout->addWidget(pMoney, 2, 0);

    setLayout(pLayout);
    QString style =
        "CPlayers { "
            "border: 1px solid black; "
            "border-radius: 5px; "
            "background-color: #E6E6E6;"
        "}";
    setStyleSheet(style);
}

int CPlayers::getMoney() const
{
    return m_restMoney;
}

void CPlayers::addMoney(const int& money)
{
    m_restMoney += money;
    pMoney->setText(QString::number(m_restMoney));
}

void CPlayers::subMoney(const int& money)
{
    m_restMoney -= money;
    pMoney->setText(QString::number(m_restMoney));
}

void CPlayers::addHouse(const int& id)
{
    vHouseId.push_back(id);
}

void CPlayers::setOut()
{
    QString style =
        "CPlayers { "
        "border: 1px solid black; "
        "border-radius: 5px; "
        "background-color: gray;"
        "}";
    setStyleSheet(style);
}

void CPlayers::showInfo()
{
    VINT vHouseId;
    for (int x : vHouseId) {
        vHouseId.push_back(x);
    }

    if (pInfo) {
        FREE_PTR(pInfo);
    }

    pInfo = new CInfo(m_restMoney, vHouseId);
    pInfo->show();
}
