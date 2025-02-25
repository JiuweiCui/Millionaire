#include "StartUI.h"

CStart::CStart(int id, QWidget* parent)
	: m_id(id),
	QFrame(parent)
{
	m_players = 0;
	initWindow();
}

CStart::~CStart()
{
	for (auto ptr : vCom) {
		FREE_PTR(ptr);
	}
	FREE_PTR(pPlayers);
	FREE_PTR(pOkBtn);
	FREE_PTR(pCelBtn);
	FREE_PTR(pLayout);
}

void CStart::initWindow()
{
	pPlayers = new QGridLayout;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		auto ptr = new CBlank(i + 1);
		pPlayers->addWidget(ptr, 0, i);
		vCom.push_back(ptr);
	}

	pLayout = new QGridLayout;
	pLayout->addLayout(pPlayers, 0, 0, 1, MAX_PLAYERS);
	if (m_id == 0) {
		pOkBtn = new QPushButton("START");
		pCelBtn = new QPushButton("CANCEL");
		pLayout->addWidget(pOkBtn, 1, MAX_PLAYERS / 2 - 1, 1, 1);
		pLayout->addWidget(pCelBtn, 1, MAX_PLAYERS / 2, 1, 1);

		connect(pOkBtn, &QPushButton::clicked, [&] {
			emit haveDownSignal(1);
		});
		connect(pCelBtn, &QPushButton::clicked, [&] {
			emit haveDownSignal(0);
		});
	}
	else {
		pText = new QLabel(LOGIN_TEXT);
		pText->setAlignment(Qt::AlignCenter);
		pLayout->addWidget(pText, 1, 0);
	}

	setLayout(pLayout);
}

void CStart::addPlayer()
{
	vCom[m_players++]->setColor();
}

int CStart::getPlayers()
{
	return m_players;
}

/*
bool checkStartGame(int id, CStart* pStart)
{
	pStart = new CStart(id);

	bool flg = (pStart->exec() == QDialog::Accepted);
	FREE_PTR(pStart);

	return flg;
}
*/

