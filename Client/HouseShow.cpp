#include "HouseShow.h"

CHouseShow::CHouseShow(QObject* parent)
    : QObject(parent)
{
    initWindow();
}

CHouseShow* CHouseShow::getInstance()
{
    static CHouseShow houseShowptr;
    return &houseShowptr;
}

CHouseShow::~CHouseShow()
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
    FREE_PTR(frameptr);
}

void CHouseShow::initWindow()
{
    QFont font;
    font.setFamily(CHOUSESHOW_CHN_FAMILY);
    font.setBold(true);
    font.setPointSize(CHOUSESHOW_CHN_SIZE);

    nameptr = new QLabel("名称");
    nameptr->setAlignment(Qt::AlignCenter);
    nameptr->setFont(font);

    ownerptr = new QLabel("拥有者");
    ownerptr->setAlignment(Qt::AlignCenter);
    ownerptr->setFont(font);

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

    font.setFamily(CHOUSESHOW_ENG_FAMILY);
    groundPriceNptr = new QLabel("0");
    groundPriceNptr->setAlignment(Qt::AlignCenter);
    groundPriceNptr->setFont(font);

    for (int i = 0; i < 6; i++) {
        QLabel* label = new QLabel("0");
        label->setAlignment(Qt::AlignCenter);
        label->setFont(font);
        vTollNptr.push_back(label);
    }

    housePriceNptr = new QLabel("0");
    housePriceNptr->setAlignment(Qt::AlignCenter);
    housePriceNptr->setFont(font);

    mortNptr = new QLabel("0");
    mortNptr->setAlignment(Qt::AlignCenter);
    mortNptr->setFont(font);

    housesNptr = new QLabel("0");
    housesNptr->setAlignment(Qt::AlignCenter);
    housesNptr->setFont(font);

    layoutptr = new QGridLayout;
    layoutptr->addWidget(nameptr, 0, 0, 1, 4);
    layoutptr->addWidget(ownerptr, 1, 0, 1, 4);
    layoutptr->addWidget(groundPriceptr, 2, 0, 1, 2);
    layoutptr->addWidget(groundPriceNptr, 2, 2, 1, 2);

    layoutptr->addWidget(vTollptr[0], 3, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[0], 3, 1, 1, 1);
    layoutptr->addWidget(vTollptr[1], 3, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[1], 3, 3, 1, 1);

    layoutptr->addWidget(vTollptr[2], 4, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[2], 4, 1, 1, 1);
    layoutptr->addWidget(vTollptr[3], 4, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[3], 4, 3, 1, 1);

    layoutptr->addWidget(vTollptr[4], 5, 0, 1, 1);
    layoutptr->addWidget(vTollNptr[4], 5, 1, 1, 1);
    layoutptr->addWidget(vTollptr[5], 5, 2, 1, 1);
    layoutptr->addWidget(vTollNptr[5], 5, 3, 1, 1);

    layoutptr->addWidget(housePriceptr, 6, 0, 1, 1);
    layoutptr->addWidget(housePriceNptr, 6, 1, 1, 1);
    layoutptr->addWidget(housesptr, 6, 2, 1, 1);
    layoutptr->addWidget(housesNptr, 6, 3, 1, 1);

    layoutptr->addWidget(mortptr, 7, 0, 1, 1);
    layoutptr->addWidget(mortNptr, 7, 1, 1, 1);
    layoutptr->addWidget(isMortptr, 7, 2, 1, 1);
    layoutptr->addWidget(isMortNptr, 7, 3, 1, 1);

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CHOUSESHOW_STYLE);
}

QWidget* CHouseShow::getWidget() const
{
    return frameptr;
}

void CHouseShow::showHouseInfo(const House& house)
{
    nameptr->setText(house.m_name);

    if (house.m_ownerId == -1) {
        ownerptr->setText("拥有者: 无");
    }
    else {
        ownerptr->setText("拥有者: 玩家" + QString::number(house.m_ownerId + 1));
    }
    mortNptr->setText(QString::number(house.m_mortgage));
    isMortNptr->setText(house.m_isMort ? "是" : "否");

    for (int i = 0; i < 6; i++) {
        vTollNptr[i]->setText(QString::number(house.vTollPrice[i]));
    }
    groundPriceNptr->setText(QString::number(house.m_groundPrice));
    housePriceNptr->setText(QString::number(house.m_housePrice));
    housesNptr->setText(QString::number(house.m_houses)); 
}