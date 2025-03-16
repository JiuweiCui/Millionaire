#include "Timer.h"


CTimer::CTimer(QObject* parent)
    : QObject(parent)
{
    initWindow();
}

CTimer* CTimer::getInstance()
{
    static CTimer timeptr;
    return &timeptr;
}

CTimer::~CTimer()
{
    timerptr->stop();
    FREE_PTR(timerptr);
    FREE_PTR(textptr);
    FREE_PTR(restTimeptr);
    FREE_PTR(passBtnptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

void CTimer::initWindow()
{
    m_restTime = MAX_OP_TIME;
    QFont font;
    font.setFamily(CTIMER_CHN_FAMILY);
    font.setPointSize(CTIMER_CHN_SIZE);
    font.setBold(true);

    textptr = new QLabel("回合剩余时间");
    textptr->setAlignment(Qt::AlignCenter);
    textptr->setFont(font);

    font.setFamily(CTIMER_ENG_FAMILY);
    restTimeptr = new QLabel("0");
    restTimeptr->setAlignment(Qt::AlignCenter);
    restTimeptr->setFont(font);

    passBtnptr = new QPushButton("PASS");
    passBtnptr->setFont(font);
    passBtnptr->setStyleSheet(CTIMER_BUTTON_STYLE);

    connect(passBtnptr, &QPushButton::clicked, [&]() {
        timerptr->stop();
        emit stopSignal(MYSIGNAL::PASS);
    });

    layoutptr = new QGridLayout;
    layoutptr->addWidget(textptr, 0, 0,1, 4);
    layoutptr->addWidget(restTimeptr, 1, 0, 1, 4);
    layoutptr->addWidget(passBtnptr, 2, 1, 2, 2);

    timerptr = new QTimer;
    timerptr->setInterval(1000);
    connect(timerptr, &QTimer::timeout, [&]() {
        restTimeptr->setText(QString::number(m_restTime--));
        if (m_restTime == -1) {
            timerptr->stop();
            emit stopSignal(MYSIGNAL::OUT3);
        }
    });

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CTIMER_STYLE);
}

QWidget* CTimer::getWidget() const
{
    return frameptr;
}

void CTimer::start()
{
    m_restTime = MAX_OP_TIME;
    passBtnptr->setEnabled(false);
    timerptr->start();
}

void CTimer::setEnabled(bool flg)
{
    passBtnptr->setEnabled(flg);
}

CDice::CDice(QObject* parent)
    : QObject(parent)
{
    initWindow();
}

CDice* CDice::getInstance()
{
    static CDice diceptr;
    return &diceptr;
}

CDice::~CDice()
{
    FREE_PTR(textptr);
    FREE_PTR(numptr);
    FREE_PTR(rollBtnptr);
    FREE_PTR(layoutptr);
    FREE_PTR(frameptr);
}

void CDice::initWindow()
{
    QFont font;
    font.setFamily(CTIMER_CHN_FAMILY);
    font.setPointSize(CTIMER_CHN_SIZE);
    font.setBold(true);

    textptr = new QLabel("骰子点数");
    textptr->setAlignment(Qt::AlignCenter);
    textptr->setFont(font);
    
    rollBtnptr = new QPushButton("掷骰子!");
    rollBtnptr->setFont(font);
    rollBtnptr->setStyleSheet(CTIMER_BUTTON_STYLE);
    connect(rollBtnptr, &QPushButton::clicked, [&]() {
        rollBtnptr->setEnabled(false);
        emit haveMoveSignal(MYSIGNAL::MOVE);
    });

    font.setFamily(CTIMER_ENG_FAMILY);
    numptr = new QLabel("0");
    numptr->setAlignment(Qt::AlignCenter);
    numptr->setFont(font);

    layoutptr = new QGridLayout;
    layoutptr->addWidget(textptr, 0, 0, 1, 4);
    layoutptr->addWidget(numptr, 1, 0, 1, 4);
    layoutptr->addWidget(rollBtnptr, 2, 1, 2, 2);

    frameptr = new QFrame;
    frameptr->setLayout(layoutptr);
    frameptr->setStyleSheet(CTIMER_STYLE);
}

QWidget* CDice::getWidget() const
{
    return frameptr;
}

void CDice::setRoll(bool flg)
{
    rollBtnptr->setEnabled(flg);
}

void CDice::setStep(int step)
{
    numptr->setText(QString::number(step));
}

