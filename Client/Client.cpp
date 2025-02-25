#include "Client.h"

Client::Client(QString ip, int port, QWidget* parent)
    : m_ip(ip),
    m_port(port),
    QWidget(parent)
{
    // 显示窗口并最大化
    //showMaximized();

    // 获取屏幕的尺寸
    QScreen* pScreen = QApplication::primaryScreen();
    QRect rect = pScreen->availableGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    setFixedSize(screenWidth - 10, screenHeight - 50);

    initConnect();

    /*
    m_players = 4;
    m_curplayer = 0;
    m_seats = 0;

    initWindow();
    */
}

Client::~Client()
{
    if (pSocket) {
        pSocket->disconnectFromHost();
        pSocket->waitForDisconnected();
        pSocket->deleteLater();
        FREE_PTR(pSocket);
    }

    FREE_PTR(pStart);

    for (auto ptr : vGrid) {
        FREE_PTR(ptr);
    }

    for (auto ptr : vCPlayers) {
        FREE_PTR(ptr);
    }

    FREE_PTR(pLogo);
    FREE_PTR(pTimer);
    FREE_PTR(pDice);
    FREE_PTR(pInfoArea);
    FREE_PTR(pShowArea);
    FREE_PTR(pHouseInfo);
    FREE_PTR(pCard);
    
    FREE_PTR(pBoardCenter);
    FREE_PTR(pGiveupBtn);
    FREE_PTR(pExitBtn);
    FREE_PTR(pWindow);
}

void Client::initConnect()
{
    pSocket = new QTcpSocket;
    // 地址和端口
    pSocket->connectToHost(m_ip, m_port);
    if(!pSocket->waitForConnected(1000)) {
        return;
    }
    
    connect(pSocket, &QTcpSocket::readyRead, this, &Client::readData);
    connect(pSocket, &QTcpSocket::disconnected, [&]() {
        QApplication::quit();
    });
}

void Client::initWindow()
{
    // 初始化选手信息
    initPlayersInfo();

    // 初始化房子信息
    initHousesInfo();

    // 初始化棋盘
    initBoard();

    // 初始化玩家开始位置
    initPlayersPos();

    //ui.setupUi(this);
    //this->show();
    showMaximized();
}

// 初始化选手信息
void Client::initPlayersInfo()
{
    vPlayPos.resize(m_players, 0);
    for (int i = 0; i < m_players; i++) {
        CPlayers* ptr = new CPlayers(m_seats, i, INIT_MONEY);
        vCPlayers.push_back(ptr);
    }
}

// 初始化玩家位置
void Client::initPlayersPos()
{
    for (int i = 0; i < m_players; i++) {
        vGrid[0]->addPlayer(COLOR(i));
    }
}

// 初始化房子信息
void Client::initHousesInfo()
{
    vHouse.resize(BOARD_COUNT);
    for (int i = 0; i < BOARD_COUNT; i++) {
        vHouse[i] = new CHouse(i, -1, 0);
    }
}

// 初始化一个棋盘位置
void Client::initOneBoard(int i, int j, int cnt)
{
    CGrid* pGrid = new CGrid(cnt, m_players);
    connect(pGrid, &CGrid::showHouseInfoSignal, this, &Client::showHouseInfo);
    pBoardCenter->addWidget(pGrid, i, j);
    vGrid.push_back(pGrid);
}

// 初始化棋盘
void Client::initBoard()
{
    pBoardCenter = new QGridLayout;
    // 方格信息区域
    int cnt = 0;
    for (int i = BOARD_ROW - 1; i > 0; i--) {
        initOneBoard(i, 0, cnt++);
    }
    for (int j = 0; j < BOARD_COL - 1; j++) {
        initOneBoard(0, j, cnt++);
    }
    for (int i = 0; i < BOARD_ROW - 1; i++) {
        initOneBoard(i, BOARD_COL - 1, cnt++);
    }
    for (int j = BOARD_COL - 1; j > 0; j--) {
        initOneBoard(BOARD_ROW - 1, j, cnt++);
    }

    // 信息显示界面
    pShowArea = new CShowArea;
    pBoardCenter->addWidget(pShowArea, 1, BOARD_COL / 2 - 2, 3, 4 + BOARD_COL % 2);

    // 房子信息界面
    pHouseInfo = new CHouseInfo(12, 2);
    pBoardCenter->addWidget(pHouseInfo, 1, 1, 6, 3);

    // 中央Logo区域
    QFont font;
    font.setFamily("Gigi");
    font.setPointSize(60);
    font.setBold(true);

    pLogo = new QLabel;
    pLogo->setText("Happy Millionaire");
    pLogo->setAlignment(Qt::AlignCenter);
    pLogo->setFont(font);
    pBoardCenter->addWidget(pLogo, 4, 4, 3, BOARD_COL - 8);

    // 计时器区域
    pTimer = new CTimer(MAX_OP_TIME);
    pTimer->start();
    connect(pTimer, &CTimer::stopSignal, this, &Client::writeData);
    pBoardCenter->addWidget(pTimer, 1, BOARD_COL - 4, 3, 3);

    // 掷骰子区域
    pDice = new CDice;
    connect(pDice, &CDice::haveMoveSignal, this, &Client::writeData);
    pBoardCenter->addWidget(pDice, 4, BOARD_COL - 4, 3, 3);

    // 信息确认区域
    pInfoArea = new CInfoArea;
    connect(pInfoArea, &CInfoArea::haveDownSignal, this, &Client::updateInfoArea);
    pBoardCenter->addWidget(pInfoArea, 7, BOARD_COL / 2 - 1, 3, 2 + BOARD_COL % 2);

    // 卡片界面
    pCard = new CCard;
    pBoardCenter->addWidget(pCard, BOARD_ROW - 7, 1, 6, BOARD_COL - 2);
    connect(pCard, &CCard::opHouseSignal, this, &Client::writeData);
    
    FREE_PTR(pWindow);
    pWindow = new QGridLayout;
    /*
    QLayoutItem* item;
    while ((item = pWindow->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
        }
    }
    */

    int len = 16;
    pWindow->addLayout(pBoardCenter, 0, 0, len, len);
    
    font.setFamily("楷体");
    font.setPointSize(20);
    pExitBtn = new QPushButton("退出");
    pExitBtn->setFont(font);

    connect(pExitBtn, &QPushButton::clicked, [&]() {
        close();
    });

    pGiveupBtn = new QPushButton("认输");
    pGiveupBtn->setFont(font);
    pWindow->addWidget(pGiveupBtn, 0, 16, 1, 1);
    connect(pGiveupBtn, &QPushButton::clicked, [&]() {
        writeData(MYSIGNAL::GIVE_UP);
    });

    // 玩家信息
    for (int i = 0; i < m_players; i++) {
        pWindow->addWidget(vCPlayers[(m_seats + i) % m_players], len - 2 * (i + 1), len, 2, 1);
    }

    setLayout(pWindow);
    //QString style = "Client { background-image: url(':/Client/image/desktop.jpg'); }";
    //QString style = "Client { background-color: orange; }";
    //setStyleSheet(style);

    // 使用样式表设置背景渐变
    setStyleSheet(
        "Client {"
        " background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        " stop:0 #FFF9C4, " // 淡黄色
        " stop:0.5 #FFCCBC, " // 柔和的橙色
        " stop:1 #C5E1A5); " // 清新的绿色
        "}"
    );

}

void Client::readData()
{
    QDataStream in(pSocket);
    in.setVersion(QDataStream::Qt_6_8);
    MYSIGNAL signal;
    in >> signal;

    if (signal == MYSIGNAL::ADD) {
        waitLogin(in);
    }
    else if (signal == MYSIGNAL::START) {
        startGame(in);
    }
    else if (signal == MYSIGNAL::END) {
        endGame();
    }
    else if (signal == MYSIGNAL::BEGIN) {
        beginRound(in);
    }
    else if (signal == MYSIGNAL::MOVE) {
        updatePos(in);
    }
    else if (signal == MYSIGNAL::BUY_COUNTRY || signal == MYSIGNAL::BUY_HOUSE ||
        signal == MYSIGNAL::GIVE_TOLL || signal == MYSIGNAL::SELL || 
        signal == MYSIGNAL::MORT || signal == MYSIGNAL::BUY_BACK) {
        updateGameInfo(signal, in);
    }
    else if (signal == MYSIGNAL::OUT || signal == MYSIGNAL::GIVE_UP) {
        outPlayer(signal, in);
    }
    else if (signal == MYSIGNAL::OVER) {
        gameOver(in);
    }
}

void Client::writeData(MYSIGNAL signal)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);
    
    if (signal == MYSIGNAL::START || signal == MYSIGNAL::END) {
        out << signal;
    }
    else if (signal == MYSIGNAL::MOVE || signal == MYSIGNAL::BUY_COUNTRY ||
        signal == MYSIGNAL::BUY_HOUSE || signal == MYSIGNAL::GIVE_TOLL) {
        out << signal;
        pTimer->setEnabled(true);
    }
    else if (signal == MYSIGNAL::SELL || signal == MYSIGNAL::MORT ||
        signal == MYSIGNAL::BUY_BACK) {
        out << signal << pCard->getHouseId();
    }
    else if (signal == MYSIGNAL::PASS || signal == MYSIGNAL::OUT ||
        signal == MYSIGNAL::GIVE_UP) {
        out << signal;
    }
    
    pSocket->write(data);
    pSocket->flush(); // 确保所有数据都被写入到 socket
}

// 等待玩家
void Client::waitLogin(QDataStream& in)
{
    in >> m_players;
    if (!pStart) {
        pStart = new CStart(m_players - 1);
        connect(pStart, &CStart::haveDownSignal, this, &Client::beginGame);

        pWindow = new QGridLayout;
        pWindow->addWidget(pStart, 0, 0);
        setLayout(pWindow);
        //pStart->show();
    }
    while (pStart->getPlayers() < m_players) {
        pStart->addPlayer();
    }
}

// 游戏开始
void Client::startGame(QDataStream& in)
{
    in >> m_seats >> m_players;
    pStart->close();
    FREE_PTR(pStart);

    //m_players = 4;
    //m_curplayer = 0;
    //m_seats = 0;

    initWindow();
}

// 房间解散
void Client::endGame()
{
    pStart->close();
    this->close();
}

// 回合开始
void Client::beginRound(QDataStream& in)
{
    in >> m_curplayer;
    if (m_curplayer == m_seats) {
        pDice->setRoll(true);  // 自己的回合可以掷骰子
    }
    pTimer->start();   // 当前回合计时开始
}

// 更新玩家位置
void Client::updatePos(QDataStream& in)
{
    int nextStep;
    in >> nextStep;

    // 设置骰子步数
    pDice->setStep(nextStep);  
    
    // 更新终点位置
    int nTmpPos = (vPlayPos[m_curplayer] + nextStep) % vGrid.size();
    vGrid[vPlayPos[m_curplayer]]->subPlayer(COLOR(m_curplayer));
    vGrid[nTmpPos]->addPlayer(COLOR(m_curplayer));
    vPlayPos[m_curplayer] = nTmpPos;

    // 信息输出
    pShowArea->showInfo(GO_TEXT(m_curplayer, nextStep));
    
    // 展示信息
    pHouseInfo->show(*vHouse[nTmpPos]);

    if (vHouseInfos[nTmpPos].name == "") {  // 修改
        return;
    }

    // 加判断条件，只有是自己的回合才有权限操作
    if (m_curplayer != m_seats) {
        return;
    }

    // 有待处理的事件，不能点击 PASS 跳过
    pTimer->setEnabled(false);
    CHouse* ptr = vHouse[nTmpPos];
    if (ptr->getOwner() == -1) {  // 没人买
        pInfoArea->setType(0, ptr->getName());
    }
    else if (ptr->getOwner() == m_curplayer) {  // 自己的
        pInfoArea->setType(1, ptr->getName());
    }
    else if(!ptr->getStatus()) { // 别人的，并且没有抵押
        if (vCPlayers[m_seats]->getMoney() + getAllMoney(m_seats) < ptr->getToll()) {  // 破产
            writeData(MYSIGNAL::OUT);
            return;
        }
        else {
            pInfoArea->setType(ptr->getToll(), ptr->getName());
        }
    }
}

// 更新牌局消息并完成相应操作（中间上方区域）
void Client::updateGameInfo(MYSIGNAL signal, QDataStream& in)
{
    // signal, player_id, pos
    int player_id = 0;
    QString info = "";
    in >> player_id;
    if (signal == MYSIGNAL::BUY_COUNTRY) { // 购买国家
        int player_id = 0;
        in >> player_id;
        int pos = vPlayPos[player_id];  // 玩家当前的位置
        info = BUY_COUNTRY_TEXT(player_id, vHouse[pos]->getName());
        vCPlayers[player_id]->subMoney(vHouse[pos]->getBuyGound());
        vCPlayers[player_id]->addHouse(pos);
        vHouse[pos]->setOwner(player_id);

        if (player_id == m_seats) { // 如果是自己，增加卡牌
            pCard->addCard(*vHouse[pos]);
        }
    }
    else if (signal == MYSIGNAL::BUY_HOUSE) { // 购买房子    
        int pos = vPlayPos[player_id];  // 玩家当前的位置
        info = BUY_HOUSE_TEXT(player_id, vHouse[pos]->getName());
        vCPlayers[player_id]->subMoney(vHouse[pos]->getBuyHouse());
        vHouse[pos]->addHouse();
        
        if (player_id == m_seats) { // 如果是自己，更新卡牌上面显示的数字
            pCard->updateHouses(pos);
        }
    }
    else if(signal == MYSIGNAL::GIVE_TOLL) { // 缴纳过路费
        int pos = vPlayPos[player_id];  // 玩家当前的位置
        int owner = vHouse[pos]->getOwner();
        info = GIVE_TOLL_TEXT(player_id, owner, vHouse[pos]->getName());
        vCPlayers[player_id]->subMoney(vHouse[pos]->getToll());  // 余额减少
        vCPlayers[owner]->addMoney(vHouse[pos]->getToll());      // 余额增加
    }
    else if (signal == MYSIGNAL::SELL) { // 卖出房子
        int pos;
        in >> pos;
        info = SELL_HOUSE(player_id, vHouse[pos]->getName());
        vHouse[pos]->setOwner(-1);
        vHouse[pos]->setMortStatus(false);
        vCPlayers[player_id]->addMoney(vHouse[pos]->getBuyGound() >> 1);
        
        if (player_id == m_seats) { // 自己，删掉卡牌
            pCard->subCard();
        }
    }
    else if (signal == MYSIGNAL::MORT) { // 抵押房子
        int pos;
        in >> pos;
        info = MORT_HOUSE(player_id, vHouse[pos]->getName());
        vHouse[pos]->setMortStatus(true);
        vCPlayers[player_id]->addMoney(vHouse[pos]->getMortgage());
        
        if (player_id == m_seats) {
            pCard->updateMort(true); // 自己，更改卡牌状态
        }
    }
    else if (signal == MYSIGNAL::BUY_BACK) { // 买回房子
        int pos;
        in >> pos;
        info = BUY_BACK_HOUSE(player_id, vHouse[pos]->getName());
        vHouse[pos]->setMortStatus(false);
        vCPlayers[player_id]->subMoney(int(1.1 * vHouse[pos]->getMortgage()));

        if (player_id == m_seats) {
            pCard->updateMort(false); // 自己，更改卡牌状态
        }
    }

    pShowArea->showInfo(info);
    pInfoArea->setType(-1, "");
}

// 破产
void Client::outPlayer(MYSIGNAL signal, QDataStream& in)
{
    int player_id;
    in >> player_id;
    
    int pos = vPlayPos[player_id];
    vGrid[pos]->subPlayer(COLOR(player_id));  // 移除玩家
    m_players--;

    int rest_money = vCPlayers[player_id]->getMoney();
    vCPlayers[player_id]->subMoney(rest_money);  // 玩家资产归零
    vCPlayers[player_id]->setOut();
    if (signal == MYSIGNAL::OUT) {
        int owner = vHouse[pos]->getOwner();
        vCPlayers[owner]->addMoney(rest_money + getAllMoney(player_id));
        pShowArea->showInfo(OUT_GAME(player_id));
    }
    else {
        pShowArea->showInfo(GIVE_UP_GAME(player_id));
    }

    setCardInit(player_id);  // 卡牌复原
    pDice->setRoll(false);
    pTimer->setEnabled(false);

    if (player_id == m_seats) { // 如果是自己破产，回收卡牌 
        pCard->clearCard();
        writeData(MYSIGNAL::PASS);
    }

    FREE_PTR(pGiveupBtn);
    pWindow->addWidget(pExitBtn, 0, 16, 1, 1);
}

void Client::gameOver(QDataStream& in)
{
    int player_id;
    in >> player_id;
    pShowArea->showInfo(WINNER(player_id));
}

int Client::getAllMoney(int player_id)
{
    int sum_money = 0;
    for (auto ptr : vHouse) { // 计算玩家剩余资产
        if (ptr->getOwner() == player_id) {
            sum_money += ptr->getBuyGound() / 2;
        }
    }
    return sum_money;
}

void Client::setCardInit(int player_id)
{
    for (auto ptr : vHouse) {
        if (ptr->getOwner() == player_id) {
            ptr->setOwner(-1);
        }
    }
}

// 更新消息通知区域（中心区域）
void Client::updateInfoArea(int type, int choose)
{
    int pos = vPlayPos[m_seats];  // 自己当前所在位置
    if (type == 0) {
        if (!choose) { // 不买房子
            pInfoArea->setType(-1, "");
            pTimer->setEnabled(true);
            return;
        }
        if (vCPlayers[m_seats]->getMoney() >= vHouse[pos]->getBuyGound()) {
            writeData(MYSIGNAL::BUY_COUNTRY);
            return;
        }
    }
    else if (type == 1) {
        if (!choose) { // 不买房子
            pInfoArea->setType(-1, "");
            pTimer->setEnabled(true);
            return;
        }
        if (vCPlayers[m_seats]->getMoney() >= vHouse[pos]->getBuyHouse()) {
            writeData(MYSIGNAL::BUY_HOUSE);
            return;
        }
    }
    else if (type >= 10) {
        if (vCPlayers[m_seats]->getMoney() >= vHouse[pos]->getToll()) {
            writeData(MYSIGNAL::GIVE_TOLL);
            return;
        }
    }

    // 提示余额不足
    checkYesNo(1);
}

void Client::showHouseInfo(int house_id)
{
    pHouseInfo->show(*vHouse[house_id]);
}

// 游戏开始
void Client::beginGame(int choose)
{
    if (choose) {
        writeData(MYSIGNAL::START);
    }
    else {
        writeData(MYSIGNAL::END);
    }
}
