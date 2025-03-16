#pragma once
#include <qstring.h>
#include <qset.h>
#include <qvector.h>
#include <qdatetime.h>

// Client 棋盘参数设置
const int BOARD_ROW = 17;   // 棋盘行数
const int BOARD_COL = 17;   // 棋盘列数
const int BOARD_COUNT = 2 * (BOARD_ROW + BOARD_COL) - 4; // 棋盘格数
#define MAX_HOUSES 5        // 最多房子数
#define MAX_PLAYERS 4       // 最多玩家数
#define MAX_OP_TIME 9000    // 回合操作最长时间
#define INIT_MONEY 15000    // 玩家初始金钱数
#define CARD_LEN 6          // 存储区卡片数量

#define EXIT_FONT_FAMILY "楷体"  // 退出按钮字体样式
#define EXIT_FONT_SIZE 20        // 退出按钮字体大小

// 淡黄色 柔和的橙色 清新的绿色
#define CLIENT_STYLE "Client { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "\
							  "stop:0 #FFF9C4, stop:0.5 #FFCCBC,stop:1 #C5E1A5); }"
// end Client


// CStart
#define CSTART_BUTTON_FAMILY "楷体"
#define CSTART_BUTTON_SIZE 30
#define CSTART_BUTTON_STYLE "QPushButton { background-color: #FF8F00; border: none;"\
										  "border-radius: 25px; width: 50px; height: 50px; }"\
							"QPushButton:hover { background-color: #E64A19; }"
#define CSTART_STYLE "QFrame { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "\
							  "stop:0 #FFF9C4, stop:0.5 #FFCCBC,stop:1 #C5E1A5); }"\
					 "QLabel { border: none; }"
// end CStart


// CLogo
#define CLOGO_FONT_FAMILY "Gigi"       // 中央Logo字体样式
#define CLOGO_FONT_SIZE 60             // 中央Logo字体大小
#define CLOGO_TEXT "Happy Millionaire" // 中央Logo字体内容
// end CLogo


// CGrid
#define CGRID_STYLE_NONE "border: none;"
#define CGRID_HOUSE_FONT_FAMILY "楷体"
#define CGRID_HOUSE_FONT_SIZE 8
#define CGRID_POS_FONT_FAMILY "Times New Roman"
#define CGRID_POS_FONT_SIZE 8
#define CGRID_STYLE "QFrame { border: none; border-radius: 5px; background-color: rgba(255, 255, 255, 0.6); }"
// end CGrid


// CHouseShow
#define CHOUSESHOW_CHN_FAMILY "楷体"
#define CHOUSESHOW_CHN_SIZE 10
#define CHOUSESHOW_ENG_FAMILY "Times New Roman"
#define CHOUSESHOW_ENG_SIZE 10
#define CHOUSESHOW_STYLE "QFrame { border: 1px solid #E64A19; border-radius: 10px }"\
						 "QLabel { border: none;}"
// end CHouseShow


// CTimer CDice
#define CTIMER_CHN_FAMILY "楷体"
#define CTIMER_CHN_SIZE 15
#define CTIMER_ENG_FAMILY "Times New Roman"
#define CTIMER_BUTTON_STYLE "QPushButton { background-color: #FF8F00; border: none;"\
                                         " border-radius: 25px; width: 50px; height: 50px; }"\
							"QPushButton:hover { background-color: #E64A19; }"
#define CTIMER_STYLE "QFrame { border: 1px solid #E64A19; border-radius: 10px }"\
				     "QLabel { border: none;}"
// end CTimer


// CShowArea
#define CSHOWAREA_ENG_FAMILY "STENCIL"
#define CSHOWAREA_ENG_SIZE 16
#define CSHOWAREA_CHN_FAMILY "楷体"
#define CSHOWAREA_CHN_SIZE 14
#define CSHOWAREA_STYLE "QFrame { border: 1px solid #E64A19; border-radius: 10px }"\
						"QLabel { border: none;}"
// end CShowArea


// CInfoArea
#define CINFOAREA_ENG_FAMILY "STENCIL"
#define CINFOAREA_ENG_SIZE 15
#define CINFOAREA_CHN_FAMILY "楷体"
#define CINFOAREA_CHN_SIZE 12
#define CINFOAREA_STYLE "QFrame { border: 1px solid #E64A19; border-radius: 10px }"\
						"QLabel { border: none;}"
// end CInfoArea


// CBlankCard
#define CBLANKCARD_FONT_FAMILY "STENCIL"
#define CBLANKCARD_FONT_SIZE 50
#define CBLANKCARD_STYLE_1 "QFrame { border: none; border-radius: 5px; background-color: #E6E6E6; }"
#define CBLANKCARD_STYLE_2 "QFrame { border: none; border-radius: 5px; background-color: green; }"
// end CBlankCard


// CCard
#define CCARD_ENG_FAMILY "Times New Roman"
#define CCARD_ENG_SIZE 15
#define CCARD_CHN_FAMILY "楷体"
#define CCARD_STYLE "QFrame { border: none; border-radius: 5px; background-color: lightblue; }"
// end CCard


// CHouseCard
#define CHOUSECARD_CHN_FAMILY "楷体"
#define CHOUSECARD_CHN_SIZE 8
#define CHOUSECARD_ENG_FAMILY "Times New Roman"
#define CHOUSECARD_ENG_SIZE 8
#define CHOUSECARD_SELECTED "CHouseCard { border: 2px solid gray; border-radius: 5px; background: #BEEEFD; }"\
							"QLabel { border: none; background: #BEEEFD; }"
#define CHOUSECARD_UNSELECTED "QFrame { border: none; border-radius: 5px; background: lightblue; }"
#define CHOUSECARD_MORT "QFrame { border: none; border-radius: 5px; background: gray; }"
// end CHouseCard  #8AACB8


// CPlayers
#define CPLAYERS_CHN_FAMILY "楷体"
#define CPLAYERS_CHN_SIZE 10
#define CPLAYERS_ENG_FAMILY "Times New Roman"
#define CPLAYERS_ENG_SIZE 10
#define CPLAYERS_STYLE "QFrame { border: 1px solid black; border-radius: 5px; background-color: #E6E6E6; }"\
					   "QLabel { border: none; background: #E6E6E6; }"
#define CPLAYERS_OUT_STYLE "QFrame { border: 1px solid black; border-radius: 5px; background-color: gray; }"\
						   "QLabel { border: none; background: gray; }"
// end CPlayers

#define FREE_PTR(ptr) \
{                     \
    if (ptr) {        \
        delete ptr;   \
    }                 \
    ptr = nullptr;    \
}

// 信号类型
enum class MYSIGNAL
{
	ADD = 0,         // 增加玩家
	START = 1,       // 游戏开始
	END = 2,         // 房间解散 
	BEGIN = 3,       // 回合开始
	MOVE = 4,        // 当前回合玩家向前移动 
	BUY_COUNTRY = 5, // 购买地皮
	BUY_HOUSE = 6,   // 购买房子
	GIVE_TOLL = 7,   // 缴纳过路费
	SELL = 8,        // 卖房子
	MORT = 9,        // 抵押房子
	BUY_BACK = 10,   // 买回房子
	PASS = 11,       // 回合结束
	OUT1 = 12,       // 被别人破产
	OUT2 = 13,       // 机会、命运破产
	OUT3 = 14,       // 超时破产
	GIVE_UP = 15,    // 认输  
	OVER = 16,       // 游戏结束
	VIA_START = 17,  // 经过起点
	CHANCE = 18,     // 机会 
	DESTINY = 19,    // 命运
	ACK = 20
};

// 全局消息结构体
typedef struct GameMessage {
	MYSIGNAL type;       // 消息类型
	int seqNumber;       // 消息序号
	int pos;             // 客户端
	int data1;
	int data2;
} GameMessage;

// 消息跟踪
typedef struct PendingMessage {
	QByteArray rawData;          // 原始数据
	QDateTime lastTime;          // 最后发送时间
	int retryCount = 0;          // 超时重传次数
} PendingMessage;

#define TURN_TIME 1000        // 超时检测时间
#define ACK_TIMEOUT 500       // 超时重传时间
#define MAX_RETRY 5           // 最大重传次数
#define WAIT_CLOSE_TIME 5000  // 服务器等待关闭连接

typedef struct HouseInfo {
	QString name;
	int vTollPrice[6];
	int groundPrice;
	int housePrice;
	int mortgage;  // 抵押
} HouseInfo;

// 玩家颜色类型字符串
const QVector<QString> vColorStr = { "red", "yellow", "blue", "green" };

// 机会
const QSet setChance = { 3, 29, 44 };
// 命运
const QSet setDestiny = { 18, 38, 53 };
// 安全地
const QSet setSafe = { 0, 10, 16, 24, 32, 48, 59 };

const QVector<HouseInfo> vHouseInfos = {
	{"赤道（起点）", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 0
	{"中国", {180, 2000, 6000, 14000, 17000, 20000}, 4000, 2000, 2000},
	{"日本", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"机会", {0, 0, 0, 0, 0, 0}, 0, 0, 0},  // 3
	{"韩国", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"菲律宾", {40, 200, 600, 1800, 3200, 4500}, 600, 500, 500},
	{"马来西亚", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"越南", {220, 1200, 3600, 8500, 10250, 12000}, 2800, 1500, 1400},
	{"泰国", {120, 600, 1800, 5000, 7000, 9000}, 1600, 1000, 800},
	{"印度", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"太平洋", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 10
	{"斯里兰卡", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"伊朗", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"伊拉克", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"沙特阿拉伯", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900 },
	{"叙利亚", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},

	{"地中海", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 16
	{"土耳其", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 18
	{"黎巴嫩", {20, 100, 300, 900, 1600, 2500}, 600, 500, 300},
	{"以色列", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"俄罗斯", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"波兰", {80, 400, 1000, 3000, 4500, 6000}, 1200, 500, 500},
	{"德国", {200, 1000, 3000, 7500, 9250, 11000}, 2400, 1500, 1200},
	{"印度洋", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 24
	{"奥地利", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"瑞士", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"法国", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 2000},
	{"比利时", {180, 900, 2500, 7000, 8750, 10500}, 2200, 1500, 1100},
	{"机会", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 29
	{"荷兰", {200, 1000, 3000, 7500, 9250, 11000}, 2400, 1500, 1200},
	{"英国", {180, 900, 2500, 7000, 8570, 10500}, 2200, 1500, 1100},

	{"北极", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 32
	{"芬兰", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"瑞典", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"丹麦", {220, 1200, 3600, 8500, 10250, 12000}, 2800, 1500, 1400},
	{"希腊", {120, 600, 1800, 5000, 7000, 9000}, 1600, 1000, 800},
	{"意大利", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 38
	{"西班牙", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"葡萄牙", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"埃及", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"摩洛哥", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"赞比亚", {20, 100, 300, 900, 1600, 2500}, 600, 500, 300},
	{"机会", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 44
	{"南非", {40, 200, 600, 1800, 3200, 4500}, 600, 500, 300},
	{"加拿大", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"美国", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},

	{"夏威夷", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 48
	{"墨西哥", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"洪都拉斯", {80, 400, 1000, 3000, 4500, 6000}, 1200, 1500, 600},
	{"尼加拉瓜", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"哥斯达黎加", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 53
	{"巴拿马", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"古巴", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"哥伦比亚", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"玻利维亚", {80, 400, 1000, 3000, 4500, 6000}, 1200, 500, 600},
	{"巴西", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"大西洋", {0, 0, 0, 0, 0, 0}, 0, 0, 0}, // 59
	{"阿根廷", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"智利", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"澳大利亚", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"新西兰", {350, 1750, 500, 11000, 13000, 15000}, 3500, 2000, 1750}
};

inline std::pair<int, QString> GET_CHANCE(int player_id, int id)
{
	QString text = QString("「玩家%1」").arg(player_id + 1);
	int money = 0;
	switch (id) {
		case 1:  money = 3000; text += "荣获诺贝尔文学奖，获得3000！"; break;
		case 2:  money = 2500; text += "在俄罗斯开采矿石，获利2500！"; break;
		case 3:  money = 2000; text += "完成徒步行遍中国的壮举，获得奖金2000！"; break;
		case 4:  money = 2000; text += "受邀至斯维加斯表演魔术，酬劳2000！"; break;
		case 5:  money = 1000; text += "前往泰国学习泰拳，得赞助1000！"; break;
		case 6:  money =  500; text += "在火星上发现了第一个液态水湖，获得500！"; break;
		case 7:  money =  500; text += "获得采访英国女王的机会，获得500！"; break;
		case 8:  money =  300; text += "带旅游团参观智利国家自然历史博物馆，获得300！"; break;
		case 9:  money =    0; text += "误乘飞往菲律宾的航班，退回起点！"; break;
		case 10: money =    0; text += "乘坐飞机到意大利度假（无需付过路费）！"; break;
		case 11: money =    0; text += "乘坐波兰的有轨电车发生故障，\n失去所有现金的1/10！"; break;

		case 12: money =  500; text += "打碎摩洛哥陶瓷碗，罚款500！"; break;
		case 13: money =  300; text += "在赞比亚探险时遭遇野兽袭击，损失300！"; break;
		case 14: money = 1000; text += "去瑞士滑雪，买雪橇花费1000！"; break;
		case 15: money = 1000; text += "在日本旅行时发生地震，损失1000！"; break;
		case 16: money =  200; text += "到巴西观看国际足联世界杯，门票200！"; break;
		case 17: money =  600; text += "迷失在亚马逊雨林，损失600！"; break;
		case 18: money =  250; text += "缴纳房产税，房屋每栋250！"; break;
	}
	return { money, text };
}

inline std::pair<int, QString> GET_DESTINY(int player_id, int id)
{
	QString text = QString("「玩家%1」").arg(player_id + 1);
	int money = 0;
	switch (id) {
		case 1:  money = 3000; text += "救济非洲旱灾灾民，得奖金3000！"; break;
		case 2:  money = 2000; text += "埃及金字塔内发现神奇木乃伊，获得2000！"; break;
		case 3:  money = 1000; text += "横越马六甲海峡，奖励1000!"; break;
		case 4:  money = 1000; text += "在撒哈拉沙漠严重缺水，得救济金1000！"; break;
		case 5:  money = 1000; text += "获得德国慕尼黑大学硕士学位，得1000！"; break;
		case 6:  money =  500; text += "探索墨西哥玛雅古文明，奖金500！"; break;
		case 7:  money =  500; text += "成功登顶珠穆朗玛峰，奖金500！"; break;
		case 8:  money =  500; text += "在印度旅行时见义勇为，奖金500！"; break;
		case 9:  money =    0; text += "用左手与越南友人行礼，触犯禁忌，退回起点！"; break;
		case 10: money =    0; text += "到法国尼斯滨海度假胜地享受沙滩与海浪\n（无需付过路费）！"; break;
		case 11: money =    0; text += "航班因天气延误，暂停一次！"; break;

		case 12: money =  500; text += "在澳大利亚遭受沙尘暴，损失500！"; break;
		case 13: money =  300; text += "与家人共进晚餐吃惠灵顿牛排，费用300！"; break;
		case 14: money = 1000; text += "前往中南美洲学习种植咖啡豆，付学费1000！"; break;
		case 15: money = 1000; text += "游览意大利威尼斯城市风光，付船租金1000！"; break;
		case 16: money =  200; text += "畅游迪斯尼乐园，门票200！"; break;
		case 17: money =  200; text += "请朋友吃韩国烤肉，付200!"; break;
		case 18: money =  250; text += "支付地税，房屋每栋250！"; break;
	}
	return { money, text };
}

inline QString GO_TEXT(int player_id, int next)
{
	return QString("「玩家%1」前行%2步!").arg(player_id + 1).arg(next);
}

inline QString BUY_COUNTRY_TEXT(int player_id, QString country_name)
{
	return QString("「玩家%1」购买「%2」!").arg(player_id + 1).arg(country_name);
}

inline QString BUY_HOUSE_TEXT(int player_id, QString country_name)
{
	return QString("「玩家%1」给「%2」购买一栋房子!").arg(player_id + 1).arg(country_name);
}

inline QString GIVE_TOLL_TEXT(int player_id, int owner, QString country_name)
{
	return QString("「玩家%1」向「玩家%2」缴纳「%3」过路费!").arg(player_id + 1).arg(owner).arg(country_name);
}

inline QString SELL_HOUSE(int player_id, QString country_name)
{
	return QString("「玩家%1」卖出「%2」!").arg(player_id + 1).arg(country_name);
}

inline QString MORT_HOUSE(int player_id, QString country_name)
{
	return QString("「玩家%1」抵押「%2」!").arg(player_id + 1).arg(country_name);
}

inline QString BUY_BACK_HOUSE(int player_id, QString country_name)
{
	return QString("「玩家%1」买回「%2」!").arg(player_id + 1).arg(country_name);
}

inline QString OUT_GAME(int player_id)
{
	return QString("「玩家%1」破产!").arg(player_id + 1);
}

inline QString GIVE_UP_GAME(int player_id)
{
	return QString("「玩家%1」认输!").arg(player_id + 1);
}

inline QString WINNER(int player_id)
{
	return QString("「玩家%1」获胜!").arg(player_id + 1);
}

inline QString VIA_START_POINT(int player_id)
{
	return QString("「玩家%1」经过赤道，获得2000!").arg(player_id + 1);
}