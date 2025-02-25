#include "House.h"

CHouse::CHouse(int id, int o_id, int houses, QWidget* parent)
    : m_id(id),
    m_ownId(o_id),
    m_houses(houses),
    QWidget(parent)
{
    initVariable();
}

CHouse::~CHouse()
{
}

CHouse& CHouse::operator=(const HouseInfo& houseInfo)
{
    m_name = houseInfo.name;
    for (int i = 0; i < 6; i++) {
        vTollPrice[i] = houseInfo.vTollPrice[i];
    }
    m_buyGround = houseInfo.buyGround;
    m_buyHouse = houseInfo.buyHouse;
    m_mortgage = houseInfo.mortgage;

    return *this;
}

void CHouse::initVariable()
{
    *this = vHouseInfos[m_id];
}

void CHouse::setOwner(int player_id)
{
    m_ownId = player_id;
    m_houses = 0;
}

void CHouse::addHouse()
{
    m_houses++;
}

void CHouse::setMortStatus(bool flg)
{
    isMort = flg;
}

QString CHouse::getName() const
{
    return m_name;
}

int CHouse::getHouseId() const
{
    return m_id;
}

int CHouse::getOwner() const
{
    return m_ownId;
}

int CHouse::getToll() const
{
    return vTollPrice[m_houses];
}

int CHouse::getBuyGound() const
{
    return m_buyGround;
}

int CHouse::getBuyHouse() const
{
    return m_buyHouse;
}

int CHouse::getHouses() const
{
    return m_houses;
}

int CHouse::getMortgage() const
{
    return m_mortgage;
}

bool CHouse::getStatus() const
{
    return isMort;
}

CHouseInfo::CHouseInfo(int size, int type, QWidget* parent)
    : m_size(size),
    m_type(type),
    QFrame(parent)
{
    initWindow();
}

CHouseInfo::~CHouseInfo()
{
    FREE_PTR(pName);
    FREE_PTR(pOwn);
    FREE_PTR(pBuyGround);   
    for (auto ptr : pToll) {
        FREE_PTR(ptr);
    }
    FREE_PTR(pBuyHouse);
    FREE_PTR(pMort);
    FREE_PTR(pIsMort);
    FREE_PTR(pHouses);
    
    FREE_PTR(pBuyGroundN);
    for (auto ptr : pTollN) {
        FREE_PTR(ptr);
    }
    FREE_PTR(pBuyHouseN);
    FREE_PTR(pMortN);
    FREE_PTR(pIsMortN);
    FREE_PTR(pHousesN);
    FREE_PTR(pLayout);
}

void CHouseInfo::initWindow()
{
    font.setFamily("楷体");
    font.setBold(true);
    font.setPointSize(m_size);

    pName = new QLabel("名称");
    pName->setAlignment(Qt::AlignCenter);
    pName->setFont(font);

    pOwn = new QLabel("拥有者");
    pOwn->setAlignment(Qt::AlignCenter);
    pOwn->setFont(font);
    
    pBuyGround = new QLabel("地皮价格:");
    pBuyGround->setAlignment(Qt::AlignCenter);
    pBuyGround->setFont(font);

    for (int i = 0; i < 6; i++) {
        QLabel* pLabel = new QLabel("过路费" + QString::number(i) + ":");
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setFont(font);
        pToll.push_back(pLabel);
    }

    pBuyHouse = new QLabel("建房价格:");
    pBuyHouse->setAlignment(Qt::AlignCenter);
    pBuyHouse->setFont(font);

    pHouses = new QLabel("房子数量:");
    pHouses->setAlignment(Qt::AlignCenter);
    pHouses->setFont(font);

    pMort = new QLabel("抵押费:");
    pMort->setAlignment(Qt::AlignCenter);
    pMort->setFont(font);

    pIsMort = new QLabel("抵押状态:");
    pIsMort->setAlignment(Qt::AlignCenter);
    pIsMort->setFont(font);

    pIsMortN = new QLabel("否");
    pIsMortN->setAlignment(Qt::AlignCenter);
    pIsMortN->setFont(font);

    font.setFamily("Times New Roman");
    pBuyGroundN = new QLabel("0");
    pBuyGroundN->setAlignment(Qt::AlignCenter);
    pBuyGroundN->setFont(font);

    for (int i = 0; i < 6; i++) {
        QLabel* pLabel = new QLabel("0");
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setFont(font);
        pTollN.push_back(pLabel);
    }

    pBuyHouseN = new QLabel("0");
    pBuyHouseN->setAlignment(Qt::AlignCenter);
    pBuyHouseN->setFont(font);

    pMortN = new QLabel("0");
    pMortN->setAlignment(Qt::AlignCenter);
    pMortN->setFont(font);

    pHousesN = new QLabel("0");
    pHousesN->setAlignment(Qt::AlignCenter);
    pHousesN->setFont(font);

    pLayout = new QGridLayout;
    pLayout->addWidget(pName, 0, 0, 1, 4);
    if (m_type == 2) {   
        pLayout->addWidget(pOwn, 1, 0, 1, 4);
    }
    pLayout->addWidget(pBuyGround, m_type, 0, 1, 2);
    pLayout->addWidget(pBuyGroundN, m_type, 2, 1, 2);

    pLayout->addWidget(pToll[0], m_type + 1, 0, 1, 1);
    pLayout->addWidget(pTollN[0], m_type + 1, 1, 1, 1);
    pLayout->addWidget(pToll[1], m_type + 1, 2, 1, 1);
    pLayout->addWidget(pTollN[1], m_type + 1, 3, 1, 1);

    pLayout->addWidget(pToll[2], m_type + 2, 0, 1, 1);
    pLayout->addWidget(pTollN[2], m_type + 2, 1, 1, 1);
    pLayout->addWidget(pToll[3], m_type + 2, 2, 1, 1);
    pLayout->addWidget(pTollN[3], m_type + 2, 3, 1, 1);

    pLayout->addWidget(pToll[4], m_type + 3, 0, 1, 1);
    pLayout->addWidget(pTollN[4], m_type + 3, 1, 1, 1);
    pLayout->addWidget(pToll[5], m_type + 3, 2, 1, 1);
    pLayout->addWidget(pTollN[5], m_type + 3, 3, 1, 1);

    pLayout->addWidget(pBuyHouse, m_type + 4, 0, 1, 1);
    pLayout->addWidget(pBuyHouseN, m_type + 4, 1, 1, 1);
    pLayout->addWidget(pHouses, m_type + 4, 2, 1, 2);
    pLayout->addWidget(pHousesN, m_type + 4, 3, 1, 2);

    pLayout->addWidget(pMort, m_type + 5, 0, 1, 1);
    pLayout->addWidget(pMortN, m_type + 5, 1, 1, 1);
    pLayout->addWidget(pIsMort, m_type + 5, 2, 1, 1);
    pLayout->addWidget(pIsMortN, m_type + 5, 3, 1, 1);

    setLayout(pLayout);
    /*
    QString style =
        "CHouseInfo { "
        "    border: 1px solid #FFF9C4;"
        "    border-radius: 10px; "
        "    background-color: #FFECB3;"
        "}";
    */
    QString style = "";
    if (m_type == 1) {
        style = "CHouseInfo {border: none; border-radius: 5px; background-color: lightblue}";
    }
    else {
        style = "CHouseInfo {border: 1px solid #E64A19; border-radius: 10px}";
    }
    setStyleSheet(style);
}

void CHouseInfo::show(const CHouse& house)
{
    *this = house;
    font.setFamily("楷体");
    pName->setText(vHouseInfos[m_id].name);
    pName->setFont(font);
    if (m_ownId == -1) {
        pOwn->setText("拥有者: 无");
    }
    else {
        pOwn->setText("拥有者: 玩家" + QString::number(m_ownId + 1));
    }
    pIsMortN->setText(QString::number(m_mortgage));

    font.setFamily("Times New Roman");
    for (int i = 0; i < 6; i++) {
        pTollN[i]->setText(QString::number(vTollPrice[i]));
        pTollN[i]->setFont(font);
    }
    pBuyGroundN->setText(QString::number(m_buyGround));
    pBuyHouseN->setText(QString::number(m_buyHouse));
    pHousesN->setText(QString::number(m_houses));
}

// 更新房子显示数量
void CHouseInfo::updateHouses()
{
    pHousesN->setText(QString::number(m_houses));
}

// 设置该卡片选中状态
void CHouseInfo::setSelected(bool selected)
{
    isSelected = selected;
    QString style = "";
    if (isSelected) {
        style = "CHouseInfo {border: 2px; border-radius: 5px; background-color: #add8e6;}";
    }
    else {
        style = "CHouseInfo {border: none; border-radius: 5px; background-color: lightblue;}";
    }
    setStyleSheet(style);
}

void CHouseInfo::updateMort()
{
    QString style = "CHouseInfo {border: none; border-radius: 5px; background-color: lightblue;}";
    setStyleSheet(style);
}

// 鼠标点击事件处理
void CHouseInfo::mousePressEvent(QMouseEvent* event)
{
    if (m_type == 2) {
        return;
    }
    isSelected = !isSelected;
    setSelected(isSelected);
    QFrame::mousePressEvent(event);
    emit clicked(this, isSelected);
}

CHouseInfo& CHouseInfo::operator=(const CHouse& house)
{
    m_id = house.getHouseId();
    m_ownId = house.getOwner();
    for (int i = 0; i < 6; i++) {
        vTollPrice[i] = vHouseInfos[m_id].vTollPrice[i];
    }
    m_buyGround = house.getBuyGound();
    m_buyHouse = house.getBuyHouse();
    m_houses = house.getHouses();
    m_mortgage = house.getMortgage();
    m_isMort = false;

    return *this;
}

CBlank::CBlank(int number, QWidget* parent)
    : QFrame(parent)
{
    QFont font;
    font.setFamily("STENCIL");
    font.setPointSize(50);
    font.setBold(true);
    pText = new QLabel(QString::number(number));
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);
    
    pLayout = new QGridLayout;
    pLayout->addWidget(pText);

    setLayout(pLayout);
    QString style =
        "CBlank { "
        "    border: none; "
        "    border-radius: 5px; "
        "    background-color: #E6E6E6;"
        "}";
    setStyleSheet(style);
}

CBlank::~CBlank()
{
    FREE_PTR(pText);
    FREE_PTR(pLayout);
}

void CBlank::setColor()
{
    QString style =
        "CBlank { "
        "    border: none; "
        "    border-radius: 5px; "
        "    background-color: green;"
        "}";
    setStyleSheet(style);
}

CCard::CCard(QWidget* parent)
    : QFrame(parent)
{
    initWindow();
}

CCard::~CCard()
{
    FREE_PTR(pInfo);
    FREE_PTR(pSell);
    FREE_PTR(pMort);
    FREE_PTR(pBuy);

    for (auto ptr : pCard) {
        FREE_PTR(ptr);
    }
    for (auto ptr : pBlank) {
        FREE_PTR(ptr);
    }
    FREE_PTR(pHlayout);
    FREE_PTR(pContainer);
    FREE_PTR(pScrollArea);
    FREE_PTR(pLayout);
}

void CCard::initWindow()
{
    QFont font;
    font.setFamily("Times New Roman");
    font.setPointSize(15);
    font.setBold(true);

    pInfo = new QLabel("YOUR COUNTRY");
    pInfo->setAlignment(Qt::AlignLeft);
    pInfo->setFont(font);

    font.setFamily("楷体");
    pSell = new QPushButton("出售");
    pSell->setFont(font);
    pSell->setEnabled(false);
    pMort = new QPushButton("抵押");
    pMort->setFont(font);
    pMort->setEnabled(false);
    pBuy = new QPushButton("买回");
    pBuy->setFont(font);
    pBuy->setEnabled(false);

    connect(pSell, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::SELL);
        });
    connect(pMort, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::MORT);
        });
    connect(pBuy, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::BUY_BACK);
        });

    pHlayout = new QHBoxLayout;
    pContainer = new QWidget;
    pContainer->setLayout(pHlayout);
    pScrollArea = new QScrollArea;
    pScrollArea->setWidget(pContainer);
    pScrollArea->setWidgetResizable(true);

    pLayout = new QGridLayout;
    pLayout->addWidget(pInfo, 0, 0, 1, 2);
    pLayout->addWidget(pSell, 0, 3, 1, 1);
    pLayout->addWidget(pMort, 0, 4, 1, 1);
    pLayout->addWidget(pBuy,  0, 5, 1, 1);
    pLayout->addWidget(pScrollArea, 1, 0, 1, 9);

    for (int i = 0; i < CARD_LEN; i++) {
        auto ptr = new CBlank(i + 1);
        pHlayout->addWidget(ptr);
        pBlank.push_back(ptr);
    }

    setLayout(pLayout);
    QString style =
        "CCard { "
        "    border: none; "
        "    border-radius: 5px; "
        "    background-color: lightblue;"
        "}";
    setStyleSheet(style);
}

void CCard::addCard(const CHouse& house)
{
    CHouseInfo* pHouseInfo = new CHouseInfo(8, 1);
    pHouseInfo->show(house);

    if (pBlank.size() > 0) {
        auto ptr = pBlank.front();
        int idx = pHlayout->indexOf(ptr); 
        pHlayout->removeWidget(ptr);
        pHlayout->insertWidget(idx, pHouseInfo);
        pBlank.pop_front();
        FREE_PTR(ptr);
    }
    else {
        pHlayout->addWidget(pHouseInfo);
    }

    pCard.push_back(pHouseInfo);
    connect(pHouseInfo, &CHouseInfo::clicked, this, &CCard::onCardClicked);
}

// 更新卡牌房子数量
void CCard::updateHouses(int house_id)
{
    for (auto ptr : pCard) {
        if (ptr->m_id == house_id) {
            ptr->m_houses++;
            ptr->updateHouses();
            break;
        }
    }
}

int CCard::getHouseId() const
{
    return pCurrent->m_id;
}

void CCard::subCard(CHouseInfo* card)
{
    if (!card) {
        card = pCurrent;
    }

    pCard.erase(std::remove(pCard.begin(), pCard.end(), card), pCard.end());
    pHlayout->removeWidget(card);
    FREE_PTR(card);
    auto ptr = new CBlank(pCard.size() + 1);
    pHlayout->insertWidget(pCard.size(), ptr);
    pBlank.push_front(ptr);
}

void CCard::clearCard()
{
    int i = pCard.size() - 1;
    while (i >= 0) {  // 倒序删除所有卡片
        subCard(pCard[i--]);
    }
}

void CCard::updateMort(bool flg)
{
    pCurrent->m_isMort = flg;
    onCardClicked(pCurrent, false);
}

// 点击某张卡片处理
void CCard::onCardClicked(CHouseInfo* card, bool selected)
{
    for (CHouseInfo* ptr : pCard) {
        if (ptr != card) {
            ptr->setSelected(false);
        }
        else {
            pCurrent = ptr;
        }
    }

    // 未选中卡片
    if (!selected) {
        pSell->setEnabled(false);
        pMort->setEnabled(false);
        pBuy->setEnabled(false);
        return;
    }

    pSell->setEnabled(!pCurrent->m_isMort);
    pMort->setEnabled(!pCurrent->m_isMort);
    pBuy->setEnabled(pCurrent->m_isMort);
}


