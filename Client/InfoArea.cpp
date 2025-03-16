#include "InfoArea.h"

CShowArea::CShowArea(QObject* parent)
    : QObject(parent)
{
    initWindow();
}

CShowArea* CShowArea::getInstance()
{
    static CShowArea showArea;
    return &showArea;
}

CShowArea::~CShowArea()
{
    FREE_PTR(textptr);
    FREE_PTR(contentptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

void CShowArea::initWindow()
{
    QFont font;
    font.setFamily(CSHOWAREA_ENG_FAMILY);
    font.setPointSize(CSHOWAREA_ENG_SIZE);
    font.setBold(true);
    
    textptr = new QLabel("Game information");
    textptr->setAlignment(Qt::AlignCenter);
    textptr->setFont(font);

    font.setFamily(CSHOWAREA_CHN_FAMILY);
    font.setPointSize(CSHOWAREA_CHN_SIZE);
    font.setBold(false);
    contentptr = new QLabel("--------------\n--------------");
    contentptr->setAlignment(Qt::AlignCenter);
    contentptr->setFont(font);

    layoutptr = new QGridLayout;
    layoutptr->addWidget(textptr, 0, 0, 1, 1);
    layoutptr->addWidget(contentptr, 1, 0, 2, 1);

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CSHOWAREA_STYLE);
}

QWidget* CShowArea::getWidget() const
{
    return frameptr;
}

void CShowArea::showInfo(const QString& str)
{
    contentptr->setText(str);
}



CInfoArea::CInfoArea(QObject* parent)
	: QObject(parent)
{
    initWindow();
}

CInfoArea* CInfoArea::getInstance()
{
    static CInfoArea infoAreaptr;
    return &infoAreaptr;
}

CInfoArea::~CInfoArea()
{
    FREE_PTR(textptr);
    FREE_PTR(contentptr);
    FREE_PTR(okBtnptr);
    FREE_PTR(celBtnptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

void CInfoArea::initWindow()
{
    QFont font;
    font.setFamily(CINFOAREA_ENG_FAMILY);
    font.setPointSize(CINFOAREA_ENG_SIZE);
    font.setBold(true);

    textptr = new QLabel("CONFIRM INFORMATION");
    textptr->setAlignment(Qt::AlignCenter);
    textptr->setFont(font);

    font.setFamily(CINFOAREA_CHN_FAMILY);
    font.setPointSize(CINFOAREA_CHN_SIZE);
    contentptr = new QLabel("");
    contentptr->setAlignment(Qt::AlignCenter);
    contentptr->setFont(font);

    okBtnptr = new QPushButton("确定");
    celBtnptr = new QPushButton("取消");
    okBtnptr->setFont(font);
    celBtnptr->setFont(font);
    celBtnptr->setStyleSheet("background-color: red;");
    okBtnptr->setEnabled(false);
    okBtnptr->setEnabled(false);

    connect(okBtnptr, &QPushButton::clicked, [&]() {
        emit haveDownSignal(m_type, 1);
    });
    connect(celBtnptr, &QPushButton::clicked, [&]() {
        emit haveDownSignal(m_type, 0);
    });

    layoutptr = new QGridLayout;
    layoutptr->addWidget(textptr, 0, 0, 1, 2);
    layoutptr->addWidget(contentptr, 1, 0, 1, 2);
    layoutptr->addWidget(okBtnptr, 2, 0, 1, 1);
    layoutptr->addWidget(celBtnptr, 2, 1, 1, 1);
    
    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CINFOAREA_STYLE);
}

QWidget* CInfoArea::getWidget() const
{
    return frameptr;
}

bool CInfoArea::getEnabled() const
{
    return m_type != -1;
}

void CInfoArea::setType(const int& type, const QString& name)
{
    m_type = type;
    if (type == -1) {
        contentptr->setText("");
        okBtnptr->setEnabled(false);
        celBtnptr->setEnabled(false);
    }
    if (type == 0) {
        contentptr->setText("是否购买「" + name + "」?");
        okBtnptr->setEnabled(true);
        celBtnptr->setEnabled(true);
    }
    else if (type == 1) {
        contentptr->setText("是否为「" + name + "」\n建造一栋房子?");
        okBtnptr->setEnabled(true);
        celBtnptr->setEnabled(true);
    }
    else if (type == 2) {
        contentptr->setText("请将余额还清（抵押、出售）！");
        okBtnptr->setEnabled(true);
        celBtnptr->setEnabled(false);
    }
    else if (type >= 10) {
        contentptr->setText("你需要向「" + name + "」\n缴纳过路费" + QString::number(type));
        okBtnptr->setEnabled(true);
        celBtnptr->setEnabled(false);
    }
}

void CInfoArea::clear()
{
    QLayoutItem* child = nullptr;
    while ((child = layoutptr->takeAt(0)) != nullptr) {
        if (child->widget() != nullptr) {
            child->widget()->setParent(nullptr);
        }
    }
}
