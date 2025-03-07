#pragma once
#include <QString>

const int BOARD_ROW = 17;   // 棋盘行数
const int BOARD_COL = 17;   // 棋盘列数
const int BOARD_COUNT = 2 * (BOARD_ROW + BOARD_COL) - 4; // 棋盘格数

const int MAX_HOUSES = 5;     // 最多房子数
const int MAX_PLAYERS = 4;    // 最多玩家数

const QString LOGIN_TEXT = "等待房主开始...";

const int PASS_TIME = 15;
const int MAX_OP_TIME = 30;  // 回合操作最长时间/s
const QString REST_TIME_TO_GO = "回合剩余时间"; // 回合剩余时间提示文本


const QString CUR_DICE_SHOW = "骰子点数"; // 回合骰子步数提示文本
const QString REST_MONEY = "余额:"; // 剩余金额提示文本
const int INIT_MONEY = 15000; // 玩家初始金钱数

using VINT = std::vector<int>;
using VVINT = std::vector<VINT>;

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


const QString COMFIRM_TYPE1 = "余额不足!";  // 余额不足

const int CARD_LEN = 8;

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
	OUT = 12,        // 破产
	GIVE_UP = 13,    // 认输  
	OVER = 14        // 游戏结束
};


// 玩家颜色
enum class COLOR
{
	RED = 0,
	YELLOW = 1,
	BLUE = 2,
	GREEN = 3
};

// 玩家颜色类型字符串
const std::vector<QString> vColorStr = {
	"red",
	"yellow",
	"blue",
	"green"
};


typedef struct HouseInfo {
	QString name;
	int vTollPrice[6];
	int buyGround;
	int buyHouse;
	int mortgage;  // 抵押
} HouseInfo;

const std::vector<HouseInfo> vHouseInfos = {
	{"开始", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"中国", {180, 2000, 6000, 14000, 17000, 20000}, 4000, 2000, 2000},
	{"日本", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"韩国", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"菲律宾", {40, 200, 600, 1800, 3200, 4500}, 600, 500, 500},
	{"马来西亚", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"越南", {220, 1200, 3600, 8500, 10250, 12000}, 2800, 1500, 1400},
	{"泰国", {120, 600, 1800, 5000, 7000, 9000}, 1600, 1000, 800},
	{"印度", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"斯里兰卡", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"伊朗", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"伊拉克", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"沙特阿拉伯", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900 },
	{"叙利亚", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},

	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"土耳其", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"黎巴嫩", {20, 100, 300, 900, 1600, 2500}, 600, 500, 300},
	{"以色列", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"俄罗斯", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"波兰", {80, 400, 1000, 3000, 4500, 6000}, 1200, 500, 500},
	{"德国", {200, 1000, 3000, 7500, 9250, 11000}, 2400, 1500, 1200},
	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"奥地利", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"瑞士", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"法国", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 2000},
	{"比利时", {180, 900, 2500, 7000, 8750, 10500}, 2200, 1500, 1100},
	{"机会", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"荷兰", {200, 1000, 3000, 7500, 9250, 11000}, 2400, 1500, 1200},
	{"英国", {180, 900, 2500, 7000, 8570, 10500}, 2200, 1500, 1100},

	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"芬兰", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"瑞典", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"丹麦", {220, 1200, 3600, 8500, 10250, 12000}, 2800, 1500, 1400},
	{"希腊", {120, 600, 1800, 5000, 7000, 9000}, 1600, 1000, 800},
	{"意大利", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"西班牙", {100, 500, 1500, 4500, 6250, 7500}, 1400, 1000, 700},
	{"葡萄牙", {160, 800, 2200, 6000, 8000, 10000}, 2000, 1000, 1000},
	{"埃及", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"摩洛哥", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"赞比亚", {20, 100, 300, 900, 1600, 2500}, 600, 500, 300},
	{"机会", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"南非", {40, 200, 600, 1800, 3200, 4500}, 600, 500, 300},
	{"加拿大", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"美国", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},

	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"墨西哥", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"洪都拉斯", {80, 400, 1000, 3000, 4500, 6000}, 1200, 1500, 600},
	{"尼加拉瓜", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"哥斯达黎加", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"命运", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"巴拿马", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"古巴", {260, 1300, 3900, 9000, 11000, 12750}, 3000, 2000, 1500},
	{"哥伦比亚", {140, 700, 2000, 5500, 7500, 9500}, 1800, 1000, 900},
	{"玻利维亚", {80, 400, 1000, 3000, 4500, 6000}, 1200, 500, 600},
	{"巴西", {220, 1100, 3300, 8000, 9750, 11500}, 2600, 1500, 1300},
	{"", {0, 0, 0, 0, 0, 0}, 0, 0, 0},
	{"阿根廷", {60, 300, 900, 2700, 4000, 5500}, 1000, 500, 500},
	{"智利", {350, 1750, 5000, 11000, 13000, 15000}, 3500, 2000, 1750},
	{"澳大利亚", {280, 1500, 4500, 10000, 12000, 14000}, 3200, 2000, 1600},
	{"新西兰", {350, 1750, 500, 11000, 13000, 15000}, 3500, 2000, 1750}
};


#define FREE_PTR(ptr) \
{                     \
    if (ptr) {        \
        delete ptr;   \
    }                 \
    ptr = nullptr;    \
}