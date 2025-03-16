#include "Confirm.h"

CConfirm::CConfirm(int type, QWidget* parent)
    : m_type(type),
    QDialog(parent)
{
    InitWindow();
}

CConfirm::~CConfirm()
{
    FREE_PTR(pName);
    FREE_PTR(pOkBtn);
    FREE_PTR(pLayout);
}

void CConfirm::InitWindow()
{
    setWindowFlags(Qt::CustomizeWindowHint);

    QFont font;
    font.setFamily("楷体");
    font.setPointSize(15);
    font.setBold(true);
    if (m_type == 1) {
        pName = new QLabel("余额不足!");
    }
    pName->setFont(font);
    pName->setAlignment(Qt::AlignCenter);
    
    pOkBtn = new QPushButton("确定");
    pOkBtn->setFont(font);
    pOkBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #FF8F00;"
        "    border: none;"
        "    border-radius: 25px;"
        "    width: 50px;"
        "    height: 50px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E64A19;"
        "}");
    connect(pOkBtn, &QPushButton::clicked, [&]() {
        this->accept();
        });

    pLayout = new QGridLayout;
    pLayout->addWidget(pName, 0, 0, 1, 4);
    pLayout->addWidget(pOkBtn, 1, 1, 1, 2);

    setLayout(pLayout);
    QString style = "CConfirm {border-radius: 20px; background-color: #FFECB3;}";
    setStyleSheet(style);
    setFixedSize(300, 200);
}

bool checkYesNo(int type)
{
    CConfirm* pConfirm = new CConfirm(type);
    bool result = (pConfirm->exec() == QDialog::Accepted);

    FREE_PTR(pConfirm);

    return result;
}