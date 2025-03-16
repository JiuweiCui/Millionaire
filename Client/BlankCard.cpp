#include "BlankCard.h"

CBlankCard::CBlankCard(int number, int type, QWidget* parent)
    :m_num(number),
    m_type(type),
    QFrame(parent)
{
    initWindow();
}

CBlankCard::~CBlankCard()
{
    FREE_PTR(pText);
    FREE_PTR(pLayout);
}

void CBlankCard::initWindow()
{
    QFont font;
    font.setFamily(CBLANKCARD_FONT_FAMILY);
    font.setPointSize(CBLANKCARD_FONT_SIZE);
    font.setBold(true);
    pText = new QLabel(QString::number(m_num));
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);

    pLayout = new QGridLayout;
    pLayout->addWidget(pText);

    setLayout(pLayout);
    if (m_type == 1) {
        setStyleSheet(CBLANKCARD_STYLE_1);
    }
    else {
        setStyleSheet(CBLANKCARD_STYLE_2);
    }
}

void CBlankCard::setColor(int type)
{
    if (type == 1) {
        setStyleSheet(CBLANKCARD_STYLE_1);
    }
    else {
        setStyleSheet(CBLANKCARD_STYLE_2);
    }
}