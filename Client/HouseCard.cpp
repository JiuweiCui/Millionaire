#include "HouseCard.h"

CHouseCard::CHouseCard(const House& house, QWidget* parent)
    : QFrame(parent)
{
    initWindow(house);
}

CHouseCard::~CHouseCard()
{
    FREE_PTR(nameptr);
    FREE_PTR(ownerptr);
    FREE_PTR(groundPriceptr);
    for (auto ptr : vTollptr) {
        FREE_PTR(ptr);
    }
    FREE_PTR(housePriceptr);
    FREE_PTR(mortptr);
    FREE_PTR(isMortptr);
    FREE_PTR(housesptr);

    FREE_PTR(groundPriceNptr);
    for (auto ptr : vTollNptr) {
        FREE_PTR(ptr);
    }
    FREE_PTR(housePriceNptr);
    FREE_PTR(mortNptr);
    FREE_PTR(isMortNptr);
    FREE_PTR(housesNptr);
    FREE_PTR(layoutptr);
}


void CHouseCard::initWindow(const House& house)
{
    isSelected = false;
    m_houseid = house.m_id;

    QFont font;
    font.setFamily(CHOUSECARD_CHN_FAMILY);
    font.setBold(true);
    font.setPointSize(CHOUSECARD_CHN_SIZE);

    nameptr = new QLabel(house.m_name);
    nameptr->setAlignment(Qt::AlignCenter);
    nameptr->setFont(font);

    groundPriceptr = new QLabel("地皮价格:");
    groundPriceptr->setAlignment(Qt::AlignCenter);
    groundPriceptr->setFont(font);

    for (int i = 0; i < 6; i++) {
        QLabel* label = new QLabel("过路费" + QString::number(i) + ":");
        label->setAlignment(Qt::AlignCenter);
        label->setFont(font);
        vTollptr.push_back(label);
    }

    housePriceptr = new QLabel("建房价格:");
    housePriceptr->setAlignment(Qt::AlignCenter);
    housePriceptr->setFont(font);

    housesptr = new QLabel("房子数量:");
    housesptr->setAlignment(Qt::AlignCenter);
    housesptr->setFont(font);

    mortptr = new QLabel("抵押费:");
    mortptr->setAlignment(Qt::AlignCenter);
    mortptr->setFont(font);

    isMortptr = new QLabel("抵押状态:");
    isMortptr->setAlignment(Qt::AlignCenter);
    isMortptr->setFont(font);

    isMortNptr = new QLabel("否");
    isMortNptr->setAlignment(Qt::AlignCenter);
    isMortNptr->setFont(font);

    font.setFamily(CHOUSECARD_ENG_FAMILY);
    groundPriceNptr = new QLabel(QString::number(house.m_groundPrice));
    groundPriceNptr->setAlignment(Qt::AlignCenter);
    groundPriceNptr->setFont(font);

    for (int i = 0; i < 6; i++) {
        QLabel* label = new QLabel(QString::number(house.vTollPrice[i]));
        label->setAlignment(Qt::AlignCenter);
        label->setFont(font);
        vTollNptr.push_back(label);
    }

    housePriceNptr = new QLabel(QString::number(house.m_housePrice));
    housePriceNptr->setAlignment(Qt::AlignCenter);
    housePriceNptr->setFont(font);

    mortNptr = new QLabel(QString::number(house.m_mortgage));
    mortNptr->setAlignment(Qt::AlignCenter);
    mortNptr->setFont(font);

    housesNptr = new QLabel(QString::number(house.m_houses));
    housesNptr->setAlignment(Qt::AlignCenter);
    housesNptr->setFont(font);

    layoutptr = new QGridLayout;
    layoutptr = new QGridLayout;
    layoutptr->addWidget(nameptr, 0, 0, 1, 4);
    layoutptr->addWidget(groundPriceptr, 1, 0, 1, 2);
    layoutptr->addWidget(groundPriceNptr, 1, 2, 1, 2);

    layoutptr->addWidget(vTollptr[0], 2, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[0], 2, 1, 1, 1);
    layoutptr->addWidget(vTollptr[1], 2, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[1], 2, 3, 1, 1);

    layoutptr->addWidget(vTollptr[2], 3, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[2], 3, 1, 1, 1);
    layoutptr->addWidget(vTollptr[3], 3, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[3], 3, 3, 1, 1);

    layoutptr->addWidget(vTollptr[4], 4, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[4], 4, 1, 1, 1);
    layoutptr->addWidget(vTollptr[5], 4, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[5], 4, 3, 1, 1);

    layoutptr->addWidget(housePriceptr, 5, 0, 1, 1);
    layoutptr->addWidget(housePriceNptr, 5, 1, 1, 1);
    layoutptr->addWidget(housesptr, 5, 2, 1, 1);
    layoutptr->addWidget(housesNptr, 5, 3, 1, 1);

    layoutptr->addWidget(mortptr, 6, 0, 1, 1);
    layoutptr->addWidget(mortNptr, 6, 1, 1, 1);
    layoutptr->addWidget(isMortptr, 6, 2, 1, 1);
    layoutptr->addWidget(isMortNptr, 6, 3, 1, 1);
    
    setLayout(layoutptr);
    setStyleSheet(CHOUSECARD_UNSELECTED);
}

// 更新房子
void CHouseCard::updateHouses(int houses)
{
    housesNptr->setText(QString::number(houses));
}

// 设置该卡片选中状态
void CHouseCard::setSelected(bool selected)
{
    isSelected = selected;
    if (isMort) { // 如果是抵押
        setStyleSheet(selected ? CHOUSECARD_SELECTED : CHOUSECARD_MORT);
    }
    else {
        setStyleSheet(selected ? CHOUSECARD_SELECTED : CHOUSECARD_UNSELECTED);
    }
}

void CHouseCard::updateMort(bool mort)
{
    isMort = mort;
    isMortNptr->setText(isMort ? "是" : "否");
    setStyleSheet(isMort ? CHOUSECARD_MORT : CHOUSECARD_UNSELECTED);
    emit clicked(this, false);
}

int CHouseCard::getHouseId() const
{
    return m_houseid;
}

bool CHouseCard::getMortStatus() const
{
    return isMort;
}

// 鼠标点击事件处理
void CHouseCard::mousePressEvent(QMouseEvent* event)
{
    QFrame::mousePressEvent(event);
    emit clicked(this, !isSelected);
}
