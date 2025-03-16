#include "Card.h"

CCard::CCard(QObject* parent)
    : QObject(parent)
{
    initWindow();
}

CCard::~CCard()
{
    FREE_PTR(infoTextptr);
    FREE_PTR(sellBtnptr);
    FREE_PTR(mortBtnptr);
    FREE_PTR(backBtnptr);

    for (auto ptr : vCardptr) {
        FREE_PTR(ptr);
    }
    for (auto ptr : vBlankptr) {
        FREE_PTR(ptr);
    }
    FREE_PTR(hlayoutptr);
    FREE_PTR(containerptr);
    FREE_PTR(scrollAreaptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

CCard* CCard::getInstance()
{
    static CCard cardptr;
    return &cardptr;
}

void CCard::initWindow()
{
    QFont font;
    font.setFamily(CCARD_ENG_FAMILY);
    font.setPointSize(CCARD_ENG_SIZE);
    font.setBold(true);

    infoTextptr = new QLabel("YOUR COUNTRY");
    infoTextptr->setAlignment(Qt::AlignLeft);
    infoTextptr->setFont(font);

    font.setFamily(CCARD_CHN_FAMILY);
    sellBtnptr = new QPushButton("出售");
    sellBtnptr->setFont(font);
    sellBtnptr->setEnabled(false);
    
    mortBtnptr = new QPushButton("抵押");
    mortBtnptr->setFont(font);
    mortBtnptr->setEnabled(false);
    
    backBtnptr = new QPushButton("买回");
    backBtnptr->setFont(font);
    backBtnptr->setEnabled(false);

    connect(sellBtnptr, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::SELL);
    });
    connect(mortBtnptr, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::MORT);
    });
    connect(backBtnptr, &QPushButton::clicked, [&]() {
        emit opHouseSignal(MYSIGNAL::BUY_BACK);
    });

    hlayoutptr = new QHBoxLayout;
    containerptr = new QWidget;
    containerptr->setLayout(hlayoutptr);
    scrollAreaptr = new QScrollArea;
    scrollAreaptr->setWidget(containerptr);
    scrollAreaptr->setWidgetResizable(true);

    layoutptr = new QGridLayout;
    layoutptr->addWidget(infoTextptr, 0, 0, 1, 2);
    layoutptr->addWidget(sellBtnptr, 0, 3, 1, 1);
    layoutptr->addWidget(mortBtnptr, 0, 4, 1, 1);
    layoutptr->addWidget(backBtnptr, 0, 5, 1, 1);
    layoutptr->addWidget(scrollAreaptr, 1, 0, 1, 9);

    for (int i = 0; i < CARD_LEN; i++) {
        auto ptr = new CBlankCard(i + 1, 1);
        hlayoutptr->addWidget(ptr);
        vBlankptr.push_back(ptr);
    }

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CCARD_STYLE);
}

void CCard::addCard(const House& house)
{
    CHouseCard* houseCard = new CHouseCard(house);

    if (vBlankptr.size() > 0) {
        auto ptr = vBlankptr.front();
        int idx = hlayoutptr->indexOf(ptr);
        hlayoutptr->removeWidget(ptr);
        hlayoutptr->insertWidget(idx, houseCard);
        vBlankptr.pop_front();
        FREE_PTR(ptr);
    }
    else {
        hlayoutptr->addWidget(houseCard);
    }

    vCardptr.push_back(houseCard);
    connect(houseCard, &CHouseCard::clicked, this, &CCard::onCardClicked);
}

// 更新卡牌房子数量
void CCard::updateHouses(int house_id, int houses)
{
    for (auto ptr : vCardptr) {
        if (ptr->getHouseId() == house_id) {
            ptr->updateHouses(houses);
            break;
        }
    }
}

// 当前选中的房子ID
int CCard::getHouseId() const
{
    return currentptr->getHouseId();
}

QWidget* CCard::getWidget() const
{
    return frameptr;
}

void CCard::subCard(CHouseCard* card)
{
    if (!card) {
        card = currentptr;
    }

    vCardptr.erase(std::remove(vCardptr.begin(), vCardptr.end(), card), vCardptr.end());
    hlayoutptr->removeWidget(card);
    FREE_PTR(card);

    if (vCardptr.size() < CARD_LEN) {
        auto ptr = new CBlankCard(vCardptr.size() + 1, 1);
        hlayoutptr->insertWidget(vCardptr.size(), ptr);
        vBlankptr.push_front(ptr);
    }
}

void CCard::clearCard()
{
    int i = vCardptr.size() - 1;
    while (i >= 0) {  // 倒序删除所有卡片
        subCard(vCardptr[i--]);
    }
}

void CCard::updateMort(bool mort)
{
    currentptr->updateMort(mort);
    onCardClicked(currentptr, false);
}

// 点击某张卡片处理
void CCard::onCardClicked(CHouseCard* card, bool selected)
{
    for (CHouseCard* ptr : vCardptr) {
        if (ptr != card) {
            ptr->setSelected(false);
        }
        else {
            ptr->setSelected(selected);
            currentptr = ptr;
        }
    }

    // 未选中卡片
    if (!selected) {
        sellBtnptr->setEnabled(false);
        mortBtnptr->setEnabled(false);
        backBtnptr->setEnabled(false);
        return;
    }

    sellBtnptr->setEnabled(!currentptr->getMortStatus());
    mortBtnptr->setEnabled(!currentptr->getMortStatus());
    backBtnptr->setEnabled(currentptr->getMortStatus());
}


