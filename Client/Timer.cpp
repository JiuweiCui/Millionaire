#include "Timer.h"


CTimer::CTimer(int time, QWidget* parent)
    : m_time(time),
    QFrame(parent)
{
    m_rest_time = m_time;

    initWindow();
}

CTimer::~CTimer()
{
    pTime->stop();
    FREE_PTR(pTime);
    FREE_PTR(pText);
    FREE_PTR(pNum);
    FREE_PTR(pPassBtn);
    FREE_PTR(pLayout);
}

void CTimer::initWindow()
{
    QFont font;
    font.setFamily("楷体");
    font.setPointSize(15);
    font.setBold(true);

    pText = new QLabel(REST_TIME_TO_GO);
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);

    font.setFamily("Times New Roman");
    pNum = new QLabel("");
    pNum->setAlignment(Qt::AlignCenter);
    pNum->setFont(font);

    pPassBtn = new QPushButton("PASS");
    pPassBtn->setFont(font);
    pPassBtn->setStyleSheet(
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

    connect(pPassBtn, &QPushButton::clicked, [&]() {
        pTime->stop();
        emit stopSignal(MYSIGNAL::PASS);
    });

    pLayout = new QGridLayout;
    pLayout->addWidget(pText, 0, 0,1, 4);
    pLayout->addWidget(pNum, 1, 0, 1, 4);
    pLayout->addWidget(pPassBtn, 2, 1, 2, 2);

    pTime = new QTimer;
    pTime->setInterval(1000);
    connect(pTime, &QTimer::timeout, [&]() {
        pNum->setText(QString::number(m_rest_time--));
        if (m_rest_time == -1) {
            emit stopSignal(MYSIGNAL::PASS);
        }
    });

    setLayout(pLayout);
    /*
    QString style =
        "CTimer { "
        "    border: none; "
        "    border-radius: 10px; "
        "    background-color: #FFECB3;"
        "}";
        */
    QString style = "CTimer {border: 1px solid #E64A19; border-radius: 10px}";
    setStyleSheet(style);
}

void CTimer::start()
{
    m_rest_time = m_time;
    pPassBtn->setEnabled(false);
    pTime->start();
}

void CTimer::setEnabled(bool flg)
{
    pPassBtn->setEnabled(flg);
}

CDice::CDice(QWidget* parent)
    : QFrame(parent)
{
    initWindow();
}

CDice::~CDice()
{
    FREE_PTR(pText);
    FREE_PTR(pNum);
    FREE_PTR(pRollBtn);
}

void CDice::initWindow()
{
    QFont font;
    font.setFamily("楷体");
    font.setPointSize(15);
    font.setBold(true);

    pText = new QLabel(CUR_DICE_SHOW);
    pText->setAlignment(Qt::AlignCenter);
    pText->setFont(font);
    
    pRollBtn = new QPushButton("掷骰子!");
    pRollBtn->setFont(font);
    pRollBtn->setStyleSheet(
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
    connect(pRollBtn, &QPushButton::clicked, [&]() {
        pRollBtn->setEnabled(false);
        emit haveMoveSignal(MYSIGNAL::MOVE);
    });

    font.setFamily("Times New Roman");
    pNum = new QLabel("0");
    pNum->setAlignment(Qt::AlignCenter);
    pNum->setFont(font);

    pLayout = new QGridLayout;
    pLayout->addWidget(pText, 0, 0, 1, 4);
    pLayout->addWidget(pNum, 1, 0, 1, 4);
    pLayout->addWidget(pRollBtn, 2, 1, 2, 2);
    setLayout(pLayout);

    setLayout(pLayout);
    /*
    QString style =
        "CDice { "
        "border: none; "
        "border-radius: 10px; "
        "background-color: #FFECB3;"
        "}";
    */
    QString style = "CDice {border: 1px solid #E64A19; border-radius: 10px}";
    setStyleSheet(style);
}

void CDice::setRoll(bool flg)
{
    pRollBtn->setEnabled(flg);
}

void CDice::setStep(int step)
{
    pNum->setText(QString::number(step));
}

