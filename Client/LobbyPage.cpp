#include "LobbyPage.h"
LobbyPage::LobbyPage(QWidget* parent, std::string owner, std::string username) : QMainWindow(parent)
{
	ui.setupUi(this);
	m_Owner = owner;
	m_username = username;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(reincarca()));
	timer->start(200);
	timer2 = new QTimer(this);
	connect(timer2, SIGNAL(timeout()), this, SLOT(startGame()));
	timer2->start(200);
}


void LobbyPage::numberOfPlayers()
{
	if (ui.Player4->text() != QString("Empty"))
		nrPlayeri = 4;
	else if (ui.Player3->text() != QString("Empty"))
		nrPlayeri = 3;
	else nrPlayeri = 2;
}
int LobbyPage::startGame()
{
	LobbyPage::numberOfPlayers();
	auto r = cpr::Post(cpr::Url{ "http://localhost:18080/isStarted" });

	if (r.status_code == 200)
	{
		if (nrPlayeri == 2)
		{
			Game* game = new Game(nullptr, 3, 3, m_username, m_Owner);
			game->resize(1200, 600);
			this->destroy();
			game->show();
			timer2->stop();
			timer->stop();
			return 1;
		}
		else if (nrPlayeri == 3)
		{
			Game* game = new Game(nullptr, 5, 3, m_username, m_Owner);
			game->resize(1200, 600);
			game->show();
			this->destroy();
			timer2->stop();
			timer->stop();
			return 1;
		}
		else if (nrPlayeri == 4)
		{
			Game* game = new Game(nullptr, 6, 4, m_username, m_Owner);
			game->resize(1200, 600);
			this->destroy();
			game->show();
			timer->stop();
			timer2->stop();
			return 1;
		}
	}
}

void LobbyPage::stop(QTimer* a)
{
	a->stop();
}
