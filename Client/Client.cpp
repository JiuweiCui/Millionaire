#include "Client.h"

Client::Client(QString ip, int port, QWidget* parent)
    : m_ip(ip),
    m_port(port),
    QWidget(parent)
{
}

Client::~Client()
{
    for (auto ptr : CGrid::vGridptr) {
        FREE_PTR(ptr);
    }

    for (auto ptr : CPlayers::vPlayersptr) {
        FREE_PTR(ptr);
    }

    FREE_PTR(pSocket);
    FREE_PTR(pNetwork);
    FREE_PTR(startptr);
    FREE_PTR(logoptr);
    FREE_PTR(pBoardCenter);
    FREE_PTR(pGiveupBtn);
    FREE_PTR(pExitBtn);
    FREE_PTR(pWindow);
}

int Client::initConnect()
{
    pSocket = new QTcpSocket;
    // 地址和端口
    pSocket->connectToHost(m_ip, m_port);
    if (!pSocket->waitForConnected(2000)) {
        return 0;
    }

    pNetwork = new NetworkManager(pSocket);
    pSocket->moveToThread(pNetwork);
    connect(pNetwork, &NetworkManager::sendMessageSignal, this, &Client::enqueueMessage);
    connect(pNetwork, &NetworkManager::disconnectedSignal, [this]() {
        pNetwork->quit();
        pNetwork->wait(1000);
        QApplication::quit();
    });
    pNetwork->start();

    setWindowFlags(Qt::CustomizeWindowHint);
    QScreen* pScreen = QApplication::primaryScreen();
    QRect rect = pScreen->availableGeometry();
    int screenWidth = rect.width();
    int screenHeight = rect.height();
    setFixedSize(screenWidth - 10, screenHeight - 10);

    return 1;
}

void Client::initWindow()
{
    FREE_PTR(pWindow);
    pWindow = new QGridLayout;

    // 初始化选手信息
    initPlayersInfo();

    // 初始化棋盘
    initBoard();

    // 初始化画面布局
    initLayout();
}

// 初始化选手信息
void Client::initPlayersInfo()
{
    for (int i = 0; i < m_players; i++) {
        auto ptr = new CPlayers(i, INIT_MONEY);
        CPlayers::vPlayersptr.push_back(ptr);
    }
}

// 初始化一个棋盘位置
void Client::initOneBoard(int row, int col, int pos)
{
    auto gridptr = new CGrid(pos);
    connect(gridptr, &CGrid::showHouseInfoSignal, this, &Client::showHouseInfo);
    pBoardCenter->addWidget(gridptr->getWidget(), row, col);
    CGrid::vGridptr.push_back(gridptr);
}

// 初始化棋盘
void Client::initBoard()
{
    pBoardCenter = new QGridLayout;

    CGrid::vPlayersPos.resize(MAX_PLAYERS, 0);
    CGrid::players = m_players;
    int pos = 0;
    for (int i = BOARD_ROW - 1; i > 0; i--) {
        initOneBoard(i, 0, pos++);
    }
    for (int j = 0; j < BOARD_COL - 1; j++) {
        initOneBoard(0, j, pos++);
    }
    for (int i = 0; i < BOARD_ROW - 1; i++) {
        initOneBoard(i, BOARD_COL - 1, pos++);
    }
    for (int j = BOARD_COL - 1; j > 0; j--) {
        initOneBoard(BOARD_ROW - 1, j, pos++);
    }

    // 玩家初始位置
    for (int i = 0; i < m_players; i++) {
        CGrid::movePlayer(i, 0);
    }
}

// 初始化总体布局
void Client::initLayout()
{
    // 信息显示界面
    auto showAreaptr = CShowArea::getInstance();
    pBoardCenter->addWidget(showAreaptr->getWidget(), 1, BOARD_COL / 2 - 2, 3, 4 + BOARD_COL % 2);

    // 房子信息界面
    auto houseShowptr = CHouseShow::getInstance();
    pBoardCenter->addWidget(houseShowptr->getWidget(), 1, 1, 6, 3);

    // 中央Logo区域
    QFont font;
    font.setFamily(CLOGO_FONT_FAMILY);
    font.setPointSize(CLOGO_FONT_SIZE);
    font.setBold(true);

    logoptr = new QLabel(CLOGO_TEXT);
    logoptr->setAlignment(Qt::AlignCenter);
    logoptr->setFont(font);
    pBoardCenter->addWidget(logoptr, 4, 4, 3, BOARD_COL - 8);

    // 计时器区域
    auto timerptr = CTimer::getInstance();
    timerptr->start();
    connect(timerptr, &CTimer::stopSignal, this, &Client::writeData);
    pBoardCenter->addWidget(timerptr->getWidget(), 1, BOARD_COL - 4, 3, 3);

    // 掷骰子区域
    auto diceptr = CDice::getInstance();
    connect(diceptr, &CDice::haveMoveSignal, this, &Client::writeData);
    pBoardCenter->addWidget(diceptr->getWidget(), 4, BOARD_COL - 4, 3, 3);

    // 信息确认区域
    auto infoAreaptr = CInfoArea::getInstance();
    connect(infoAreaptr, &CInfoArea::haveDownSignal, this, &Client::updateInfoArea);
    pBoardCenter->addWidget(infoAreaptr->getWidget(), 7, BOARD_COL / 2 - 1, 3, 2 + BOARD_COL % 2);

    // 卡片界面
    auto cardptr = CCard::getInstance();
    pBoardCenter->addWidget(cardptr->getWidget(), BOARD_ROW - 7, 1, 6, BOARD_COL - 2);
    connect(cardptr, &CCard::opHouseSignal, this, &Client::writeData);
    
    int len = 16;
    pWindow->addLayout(pBoardCenter, 0, 0, len, len);
    
    font.setFamily(EXIT_FONT_FAMILY);
    font.setPointSize(EXIT_FONT_SIZE);
    pExitBtn = new QPushButton("退出");
    pExitBtn->setFont(font);
    connect(pExitBtn, &QPushButton::clicked, [this]() {
        this->close();
    });

    pGiveupBtn = new QPushButton("认输");
    pGiveupBtn->setFont(font);
    pWindow->addWidget(pGiveupBtn, 0, 16, 1, 1);
    connect(pGiveupBtn, &QPushButton::clicked, [this]() {
        writeData(MYSIGNAL::GIVE_UP);
    });

    // 玩家信息
    for (int i = 0; i < m_players; i++) {
        pWindow->addWidget(CPlayers::vPlayersptr[(m_seats + i) % m_players]->getWidget(), 
            len - 2 * (i + 1), len, 2, 1);
    }

    setLayout(pWindow);
    setStyleSheet(CLIENT_STYLE);
}

// 等待玩家
void Client::waitLogin(const GameMessage& rcvdMsg)
{
    m_players = rcvdMsg.data1;
    if (!startptr) {
        startptr = new CStart(m_players - 1);
        connect(startptr, &CStart::haveDownSignal, this, &Client::beginGame);

        pWindow = new QGridLayout;
        pWindow->addWidget(startptr, 0, 0);
        setLayout(pWindow);
        QFrame tmp;
    }
    startptr->updatePlayers(m_players);
}

// 游戏开始
void Client::startGame(const GameMessage& rcvdMsg)
{
    m_seats = rcvdMsg.data1;
   startptr->close();
    FREE_PTR(startptr);

    initWindow();
}

// 房间解散
void Client::endGame()
{
    startptr->close();
    this->close();
}

// 回合开始
void Client::beginRound(const GameMessage& rcvdMsg)
{
    m_curplayer = rcvdMsg.data1;
    CTimer::getInstance()->start();   // 当前回合计时开始

    if (m_curplayer == m_seats && CPlayers::vPlayersptr[m_curplayer]->getRestMoney() < 0) { // 先把钱还上
        CDice::getInstance()->setRoll(false);
        CInfoArea::getInstance()->setType(2, "");
    }
    else {
        CDice::getInstance()->setRoll(m_curplayer == m_seats);  // 自己的回合可以掷骰子 
        CInfoArea::getInstance()->setType(-1, "");
    }
}

// 更新玩家位置
void Client::updatePlayerPos(const GameMessage& rcvdMsg)
{
    int nextStep = rcvdMsg.data1;

    // 设置骰子步数
    CDice::getInstance()->setStep(nextStep);

    // 信息输出
    CShowArea::getInstance()->showInfo(GO_TEXT(m_curplayer, nextStep));

    // 更新玩家位置
    bool isViaStart = CGrid::movePlayer(m_curplayer, nextStep);
    if (isViaStart) { // 经过起点
        if (m_curplayer == m_seats) {  // 自己的回合有操作权限
            writeData(MYSIGNAL::VIA_START);
        }
    }
    
    // 展示信息
    int newPos = CGrid::vPlayersPos[m_curplayer];
    CHouseShow::getInstance()->showHouseInfo(CGrid::vGridptr[newPos]->getHouseInfo());

    // 只有是自己的回合才有权限操作
    if (m_curplayer != m_seats) {
        return;
    }

    if (setSafe.contains(newPos)) {  // 安全格子，无事发生
        CTimer::getInstance()->setEnabled(true);
        return;
    }
    if (setChance.contains(newPos)) {
        writeData(MYSIGNAL::CHANCE);
        return;
    }
    if (setDestiny.contains(newPos)) {
        writeData(MYSIGNAL::DESTINY);
        return;
    }

    // 有待处理的事件，不能点击 PASS 跳过
    auto& gridptr = CGrid::vGridptr[newPos];
    if (gridptr->getHouseOwner() == -1) {  // 没人买
        CInfoArea::getInstance()->setType(0, gridptr->getHouseName());
    }
    else if (gridptr->getHouseOwner() == m_curplayer) {  // 自己的
        CInfoArea::getInstance()->setType(1, gridptr->getHouseName());
    }
    else if(!gridptr->getMortStatus()) { // 别人的，并且没有抵押
        if (CPlayers::vPlayersptr[m_curplayer]->getAllMoney() < gridptr->getHouseToll()) {  // 破产
            writeData(MYSIGNAL::OUT1);
            return;
        }
        else {
            CInfoArea::getInstance()->setType(gridptr->getHouseToll(), gridptr->getHouseName());
        }
    }
}

// 更新牌局消息并完成相应操作（中间上方区域）
void Client::updateGameInfo1(const GameMessage& rcvdMsg)
{
    QString info = "";
    int pos = CGrid::vPlayersPos[m_curplayer];
    CGrid* gridptr = CGrid::vGridptr[pos];
    CCard* cardptr = CCard::getInstance();

    // 只有回合内的玩家才能进行购买操作
    if (rcvdMsg.type == MYSIGNAL::BUY_COUNTRY) { // 购买国家
        int money = gridptr->getGroundPrice();
        CPlayers::vPlayersptr[m_curplayer]->subMoney(gridptr->getGroundPrice());
        gridptr->setHouseOwner(m_curplayer);

        if (m_curplayer == m_seats) { // 如果是自己，增加卡牌
            cardptr->addCard(gridptr->getHouseInfo());
        }

        info = BUY_COUNTRY_TEXT(m_curplayer, gridptr->getHouseName());
    }
    else if (rcvdMsg.type == MYSIGNAL::BUY_HOUSE) { // 购买房子    
        CPlayers::vPlayersptr[m_curplayer]->subMoney(gridptr->getHousePrice());
        gridptr->addHouse();
        
        if (m_curplayer == m_seats) { // 如果是自己，更新卡牌上面显示的数字
            cardptr->updateHouses(gridptr->getHouseId(), gridptr->getHouses());
        }

        info = BUY_HOUSE_TEXT(m_curplayer, gridptr->getHouseName());
    }
    else if(rcvdMsg.type == MYSIGNAL::GIVE_TOLL) { // 缴纳过路费
        int owner = gridptr->getHouseOwner();
        CPlayers::vPlayersptr[m_curplayer]->subMoney(gridptr->getHouseToll());
        CPlayers::vPlayersptr[owner]->addMoney(gridptr->getHouseToll());
        
        info = GIVE_TOLL_TEXT(m_curplayer, owner, gridptr->getHouseName());
    }

    CShowArea::getInstance()->showInfo(info);                           // 更改牌局信息
    CHouseShow::getInstance()->showHouseInfo(gridptr->getHouseInfo());  // 更改房子展示信息
    CInfoArea::getInstance()->setType(-1, "");                          // 更改信息确认区域
}

void Client::updateGameInfo2(const GameMessage& rcvdMsg)
{
    // 卖出房子，序列号，广播，玩家，房子ID
    QString info = "";
    CCard* cardptr = CCard::getInstance();
    CGrid* gridptr = CGrid::vGridptr[rcvdMsg.data2];
    int owner = gridptr->getHouseOwner();
    QString houseName = gridptr->getHouseName();

    // 回合外的玩家也可以进行如下操作
    if (rcvdMsg.type == MYSIGNAL::SELL) { // 卖出房子
        gridptr->setHouseOwner(-1);
        gridptr->setMortStatus(false);
        CPlayers::vPlayersptr[owner]->addMoney(gridptr->getGroundPrice() / 2);

        if (owner == m_seats) { // 自己，删掉卡牌
            cardptr->subCard();
        }

        info = SELL_HOUSE(owner, houseName);
    }
    else if (rcvdMsg.type == MYSIGNAL::MORT) { // 抵押房子
        gridptr->setMortStatus(true);
        CPlayers::vPlayersptr[owner]->addMoney(gridptr->getMortgage());

        if (owner == m_seats) {
            cardptr->updateMort(true); // 自己，更改卡牌状态（抵押）
        }
    
        info = MORT_HOUSE(owner, houseName);
    }
    else if (rcvdMsg.type == MYSIGNAL::BUY_BACK) { // 买回房子
        gridptr->setMortStatus(false);
        CPlayers::vPlayersptr[owner]->subMoney(int(1.1 * gridptr->getMortgage()));

        if (owner == m_seats) {
            cardptr->updateMort(false); // 自己，更改卡牌状态（非抵押）
        }

        info = BUY_BACK_HOUSE(owner, houseName);
    }

    CShowArea::getInstance()->showInfo(info);                           // 更改牌局信息
    CHouseShow::getInstance()->showHouseInfo(gridptr->getHouseInfo());  // 更改房子展示信息
    CInfoArea::getInstance()->setType(-1, "");                          // 更改信息确认区域
}

// 破产
void Client::outPlayer(const GameMessage& rcvdMsg)
{
    int player_id = rcvdMsg.data1;
    CGrid::removePlayer(player_id);  // 移除玩家
    m_players--;

    int pos = CGrid::vPlayersPos[player_id];    // 玩家位置
    int restMoney = CPlayers::vPlayersptr[player_id]->getAllMoney();  // 玩家剩余所有资产
    CPlayers::vPlayersptr[player_id]->setOut(); // 玩家卡牌置灰

    if (rcvdMsg.type == MYSIGNAL::OUT1) {       // 被别人淘汰
        int owner = CGrid::vGridptr[pos]->getHouseOwner();
        CPlayers::vPlayersptr[owner]->addMoney(restMoney);
        CShowArea::getInstance()->showInfo(OUT_GAME(player_id));
    }
    else if(rcvdMsg.type == MYSIGNAL::OUT2 || rcvdMsg.type == MYSIGNAL::OUT3) { // 被机会、命运淘汰
        CShowArea::getInstance()->showInfo(OUT_GAME(player_id));
    }
    else { // 认输
        CShowArea::getInstance()->showInfo(GIVE_UP_GAME(player_id));
    }

    CGrid::setCardInit(player_id);  // 卡牌复原
    if (player_id == m_seats) {     // 自己破产，回收卡牌 
        CCard::getInstance()->clearCard();
        
        if (m_curplayer == m_seats) { // 当前又是自己的回合，立刻结束  
            CInfoArea::getInstance()->setType(-1, "");
            CDice::getInstance()->setRoll(false);
            writeData(MYSIGNAL::PASS);
        }
    }

    FREE_PTR(pGiveupBtn);
    pWindow->addWidget(pExitBtn, 0, 16, 1, 1);
}

// 游戏结束
void Client::gameOver(const GameMessage& rcvdMsg)
{
    CShowArea::getInstance()->showInfo(WINNER(rcvdMsg.data1));
}

// 经过起点
void Client::viaStartPoint(const GameMessage& rcvdMsg)
{
    int player_id = rcvdMsg.data1;
    CShowArea::getInstance()->showInfo(VIA_START_POINT(player_id));
    CPlayers::vPlayersptr[player_id]->addMoney(2000);
}

// 机会与命运均为当前回合玩家触发
void Client::chanceDestiny(const GameMessage& rcvdMsg)
{
    int op_id = rcvdMsg.data2;

    std::pair<int, QString> resPair;
    if (rcvdMsg.type == MYSIGNAL::CHANCE) {
        resPair = GET_CHANCE(m_curplayer, op_id);
    }
    else {
        resPair = GET_DESTINY(m_curplayer, op_id);
    }
    CShowArea::getInstance()->showInfo(resPair.second);

    // 根据ID进行不同的操作
    if (op_id <= 8) {  // 盈利
        CPlayers::vPlayersptr[m_curplayer]->addMoney(resPair.first);

        // 如果是自己，跳过按钮可以点击
        if (m_curplayer == m_seats) {
            CTimer::getInstance()->setEnabled(true);
        }
        return;
    }
    if (op_id == 9) { // 退回起点
        CGrid::movePlayer(m_curplayer, 0);
        
        if (m_curplayer == m_seats) { 
            CTimer::getInstance()->setEnabled(true);
        }
        return;
    }
    if (op_id == 10) { // 机会与命运前往不同的地方
        int nextPos = (rcvdMsg.type == MYSIGNAL::CHANCE ? 37 : 27); // 前往位置
        bool visStart = CGrid::goCertainPos(m_curplayer, nextPos);

        // 如果是自己，进行相应操作
        if (m_curplayer == m_seats) {
            if (visStart) {
                writeData(MYSIGNAL::VIA_START);
            }
            CTimer::getInstance()->setEnabled(true);
        }
        return;
    }
    if (op_id == 11) {
        if (rcvdMsg.type == MYSIGNAL::CHANCE) {
            int restMoney = CPlayers::vPlayersptr[m_curplayer]->getRestMoney();
            CPlayers::vPlayersptr[m_curplayer]->subMoney(restMoney / 10);
        }

        if (m_curplayer == m_seats) {
            CTimer::getInstance()->setEnabled(true);
        }
        return;
    }

    if (op_id == 18) {
        resPair.first = CPlayers::vPlayersptr[m_curplayer]->getHouses();
        op_id = 15;  // 能够执行下面的if语句
    }

    if (op_id >= 12 && op_id <= 17) {
        if (CPlayers::vPlayersptr[m_curplayer]->getAllMoney() >= resPair.first) { // 总资产够
            CPlayers::vPlayersptr[m_curplayer]->subMoney(resPair.first);

            if (m_curplayer == m_seats && CPlayers::vPlayersptr[m_curplayer]->getRestMoney() >= 0) {
                CTimer::getInstance()->setEnabled(true);
            }
        }
        else { // 破产
            writeData(MYSIGNAL::OUT2);
        }
    }
}

// 写入数据
void Client::writeData(MYSIGNAL signal)
{
    GameMessage gameMsg;

    if (signal == MYSIGNAL::START || signal == MYSIGNAL::END) {
        gameMsg = { signal, sendNumber++, m_seats, 0, 0 };
    }
    else if (signal == MYSIGNAL::MOVE || signal == MYSIGNAL::BUY_COUNTRY ||
        signal == MYSIGNAL::BUY_HOUSE || signal == MYSIGNAL::GIVE_TOLL) {
        gameMsg = { signal, sendNumber++, m_seats, 0, 0 };
    }
    else if (signal == MYSIGNAL::SELL || signal == MYSIGNAL::MORT ||
        signal == MYSIGNAL::BUY_BACK) {
        gameMsg = { signal, sendNumber++, m_seats, CCard::getInstance()->getHouseId(), 0};
    }
    else if (signal == MYSIGNAL::PASS || signal == MYSIGNAL::OUT1 ||
        signal == MYSIGNAL::OUT2 || signal == MYSIGNAL::OUT3 ||
        signal == MYSIGNAL::GIVE_UP || signal == MYSIGNAL::VIA_START ||
        signal == MYSIGNAL::CHANCE || signal == MYSIGNAL::DESTINY) {
        gameMsg = { signal, sendNumber++, m_seats, 0, 0 };
    }

    QMetaObject::invokeMethod(pNetwork, "sendToServer",
        Qt::QueuedConnection, Q_ARG(GameMessage, gameMsg));
}

// 添加消息到队列
void Client::enqueueMessage(const GameMessage& gameMsg)
{
    QMutexLocker locker(&m_mutex);
    rcvdMsgQueue << gameMsg;
    // 通知主线程处理
    QMetaObject::invokeMethod(this, "processMessages", Qt::QueuedConnection);
}

// 处理消息
void Client::processMessages()
{
    // 减小锁的粒度
    GameMessage rcvdMsg;
    {
        QMutexLocker locker(&m_mutex);
        rcvdMsg = rcvdMsgQueue.dequeue(); // 当前要处理的消息
    }

    if (rcvdMsg.type == MYSIGNAL::ADD) {
        waitLogin(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::START) {
        startGame(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::END) {
        endGame();
    }
    else if (rcvdMsg.type == MYSIGNAL::BEGIN) {
        beginRound(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::MOVE) {
        updatePlayerPos(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::BUY_COUNTRY || rcvdMsg.type == MYSIGNAL::BUY_HOUSE ||
        rcvdMsg.type == MYSIGNAL::GIVE_TOLL) {
        updateGameInfo1(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::SELL || rcvdMsg.type == MYSIGNAL::MORT || 
        rcvdMsg.type == MYSIGNAL::BUY_BACK) {
        updateGameInfo2(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::OUT1 || rcvdMsg.type == MYSIGNAL::OUT2 ||
        rcvdMsg.type == MYSIGNAL::OUT3 || rcvdMsg.type == MYSIGNAL::GIVE_UP) {
        outPlayer(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::OVER) {
        gameOver(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::VIA_START) {
        viaStartPoint(rcvdMsg);
    }
    else if (rcvdMsg.type == MYSIGNAL::CHANCE || rcvdMsg.type == MYSIGNAL::DESTINY) {
        chanceDestiny(rcvdMsg);
    }
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

// 展示房子详细信息
void Client::showHouseInfo(int house_id)
{
    CHouseShow::getInstance()->showHouseInfo(CGrid::vGridptr[house_id]->getHouseInfo());
}

// 更新消息通知区域（中心区域）
void Client::updateInfoArea(int type, int choose)
{
    int pos = CGrid::vPlayersPos[m_seats];
    CGrid* gridptr = CGrid::vGridptr[pos];
    int restMoney = CPlayers::vPlayersptr[m_seats]->getRestMoney();  // 余额
    auto timerptr = CTimer::getInstance();

    if (type == 0) {
        if (!choose) { // 不买地皮
            CInfoArea::getInstance()->setType(-1, "");
            timerptr->setEnabled(true);
            return;
        }
        else if (restMoney >= gridptr->getGroundPrice()) {
            writeData(MYSIGNAL::BUY_COUNTRY);
            timerptr->setEnabled(true);
            return;
        }
    }
    else if (type == 1) {
        if (!choose) { // 不买房子
            CInfoArea::getInstance()->setType(-1, "");
            timerptr->setEnabled(true);
            return;
        }
        else if (restMoney >= gridptr->getHousePrice()) {
            writeData(MYSIGNAL::BUY_HOUSE);
            timerptr->setEnabled(true);
            return;
        }
    }
    else if (type == 2) {
        if (restMoney >= 0) {
            timerptr->setEnabled(true);
            return;
        }
    }
    else if (type >= 10) {
        if (restMoney >= gridptr->getHouseToll()) {
            writeData(MYSIGNAL::GIVE_TOLL);
            timerptr->setEnabled(true);
            return;
        }
    }
    // 提示余额不足
    checkYesNo(1);
}