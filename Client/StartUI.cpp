#include "StartUI.h"

CStart::CStart(int id, QWidget* parent)
	: m_id(id),
	QFrame(parent)
{
	initWindow();
}

CStart::~CStart()
{
	for (auto ptr : vCardptr) {
		FREE_PTR(ptr);
	}
	FREE_PTR(logoptr);
	FREE_PTR(textptr);
	FREE_PTR(cardLayoutptr);
	FREE_PTR(okBtnptr);
	FREE_PTR(celBtnptr);
	FREE_PTR(layoutptr);
}

void CStart::initWindow()
{
	QFont font;
	font.setFamily(CLOGO_FONT_FAMILY);
	font.setPointSize(CLOGO_FONT_SIZE);
	font.setBold(true);

	logoptr = new QLabel(CLOGO_TEXT);
	logoptr->setAlignment(Qt::AlignCenter);
	logoptr->setFont(font);
	layoutptr = new QGridLayout;
	layoutptr->addWidget(logoptr, 0, 0, 1, MAX_PLAYERS);

	cardLayoutptr = new QGridLayout;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		auto ptr = new CBlankCard(i + 1, 1);
		cardLayoutptr->addWidget(ptr, 0, i);
		vCardptr.push_back(ptr);
	}
	layoutptr->addLayout(cardLayoutptr, 1, 1, 2, MAX_PLAYERS - 2);
	
	font.setFamily(CSTART_BUTTON_FAMILY);
	font.setPointSize(CSTART_BUTTON_SIZE);
	font.setBold(true);
	if (m_id == 0) {
		okBtnptr = new QPushButton("开始");
		okBtnptr->setFont(font);
		okBtnptr->setStyleSheet(CSTART_BUTTON_STYLE);

		celBtnptr = new QPushButton("退出");
		celBtnptr->setFont(font);
		celBtnptr->setStyleSheet(CSTART_BUTTON_STYLE);

		layoutptr->addWidget(okBtnptr, 3, MAX_PLAYERS / 2 - 1, 2, 1);
		layoutptr->addWidget(celBtnptr, 3, MAX_PLAYERS / 2, 2, 1);

		connect(okBtnptr, &QPushButton::clicked, [&] {
			emit haveDownSignal(1);
		});
		connect(celBtnptr, &QPushButton::clicked, [&] {
			emit haveDownSignal(0);
		});
	}
	else {
		textptr = new QLabel("等待房主开始...");
		textptr->setFont(font);
		textptr->setAlignment(Qt::AlignCenter);
		layoutptr->addWidget(textptr, 3, 0);
	}

	setLayout(layoutptr);
	setStyleSheet(CSTART_STYLE);
}

void CStart::updatePlayers(int players)
{
	for (int i = 0; i < players; i++) {
		vCardptr[i]->setColor(2);
	}
	for (int i = players; i < MAX_PLAYERS; i++) {
		vCardptr[i]->setColor(1);
	}
}

