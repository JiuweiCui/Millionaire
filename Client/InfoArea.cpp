#include "InfoArea.h"

CShowArea::CShowArea(QWidget* parent)
    : QFrame(parent)
{
    initWindow();
}

CShowArea::~CShowArea()
{
    FREE_PTR(pConst);
    FREE_PTR(pText);
    FREE_PTR(pLayout);
}

void CShowArea::initWindow()
{
    QFont font;
    font.setFamily("STENCIL");
    font.setPointSize(16);
    font.setBold(true);
    pConst = new QLabel("Game information");
    pConst->setAlignment(Qt::AlignCenter);
    pConst->setFont(font);

    font.setFamily("楷体");
    font.setPointSize(14);
    font.setBold(false);
    pText = new QLabel("--------------\n--------------");
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);

    pLayout = new QGridLayout;
    pLayout->addWidget(pConst, 0, 0, 1, 1);
    pLayout->addWidget(pText, 1, 0, 2, 1);

    setLayout(pLayout);
    
    /*
    QString style =
        "CShowArea { "
        "    border: none; "
        "    border-radius: 10px; "
        "    background-color: #FFECB3;"
        "}";
    */
    QString style = "CShowArea {border: 1px solid #E64A19; border-radius: 10px}";
    setStyleSheet(style);
}

void CShowArea::showInfo(const QString& str)
{
    pText->setText(str);
}

CInfoArea::CInfoArea(QWidget* parent)
	: QFrame(parent)
{
    initWindow();
}

CInfoArea::~CInfoArea()
{
    FREE_PTR(pText);
    FREE_PTR(pLabel);
    FREE_PTR(pOkBtn);
    FREE_PTR(pCelBtn);
    FREE_PTR(pLayout);
}

void CInfoArea::initWindow()
{
    QFont font;
    font.setFamily("STENCIL");
    font.setPointSize(15);
    font.setBold(true);

    pText = new QLabel("CONFIRM INFORMATION");
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);

    font.setFamily("楷体");
    font.setPointSize(12);
    pLabel = new QLabel("");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFont(font);

    font.setFamily("Times New Roman");
    pOkBtn = new QPushButton("YES");
    pCelBtn = new QPushButton("NO");
    pOkBtn->setFont(font);
    pCelBtn->setFont(font);
    pCelBtn->setStyleSheet("background-color: red;");
    pOkBtn->setEnabled(false);
    pCelBtn->setEnabled(false);

    connect(pOkBtn, &QPushButton::clicked, [&]() {
        emit haveDownSignal(m_type, 1);
        });
    connect(pCelBtn, &QPushButton::clicked, [&]() {
        emit haveDownSignal(m_type, 0);
        });

    pLayout = new QGridLayout;
    pLayout->addWidget(pText, 0, 0, 1, 2);
    pLayout->addWidget(pLabel, 1, 0, 1, 2);
    pLayout->addWidget(pOkBtn, 2, 0, 1, 1);
    pLayout->addWidget(pCelBtn, 2, 1, 1, 1);
    setLayout(pLayout);

    /*
    QString style =
        "CInfoArea { "
        "    border: none; "
        "    border-radius: 10px; "
        "    background-color: #FFECB3;"
        "}";
    */
    QString style = "CInfoArea {border: 1px solid #E64A19; border-radius: 10px}";
    setStyleSheet(style);
}

void CInfoArea::showInfo(const QString& str)
{
    pLabel->setText(str);
}

void CInfoArea::setType(const int& type, const QString& name)
{
    m_type = type;
    if (type == -1) {
        pLabel->setText("");
        pOkBtn->setEnabled(false);
        pCelBtn->setEnabled(false);
    }
    if (type == 0) {
        pLabel->setText("是否购买「" + name + "」?");
        pOkBtn->setEnabled(true);
        pCelBtn->setEnabled(true);
    }
    else if (type == 1) {
        pLabel->setText("是否为「" + name + "」\n建造一栋房子?");
        pOkBtn->setEnabled(true);
        pCelBtn->setEnabled(true);
    }
    else if (type >= 10) {
        pLabel->setText("你需要向「" + name + "」\n缴纳过路费" + QString::number(type));
        pOkBtn->setEnabled(true);
        pCelBtn->setEnabled(false);
    }
}

void CInfoArea::clear()
{
    //clear();

    QLayoutItem* child = nullptr;
    while ((child = this->pLayout->takeAt(0)) != nullptr) {
        if (child->widget() != nullptr) {
            child->widget()->setParent(nullptr);
        }
    }
}
