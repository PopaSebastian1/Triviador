#include "MainPage.h"
#include "Play.h"
MainPage::MainPage(QWidget* parent, std::string useraname, std::string password)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_username = useraname;
}
void MainPage::on_Cont_clicked()
{
	accountPage = new AccountPage(nullptr,m_username);
	accountPage->resize(800, 600);
	accountPage->show();
}
MainPage::~MainPage()
{}

void MainPage::on_Play_clicked()
{
	Play* play = new Play();
	play->resize(1200, 600);
	play->show();
}

void MainPage::on_doi_clicked()
{
	Game* game = new Game(nullptr, 3, 3, "paul","paul");
	
	game->resize(1200, 600);
	game->show();
}

void MainPage::on_trei_clicked()
{
	Game* game = new Game(nullptr, 3, 5, "paul", "paul");
	game->resize(1200, 600);
	game->show();
}

void MainPage::on_patru_clicked()
{
	Game* game = new Game(nullptr, 3, 6, "paul", "paul");
	game->resize(1200, 600);
	game->show();
}

void MainPage::on_JoinLobby_clicked()
{
	std::string ownerName = ui.OwnerName->text().toUtf8().constData();
	auto r = cpr::Post(cpr::Url{ "http://localhost:18080/joinLobby" },
		cpr::Body{ "&username=" + m_username, "&gameOwner=" + ownerName });
	if (r.status_code == 200)
	{
		LobbyPage* lobbyPage = new LobbyPage(nullptr, ownerName, m_username);
		lobbyPage->setAttribute(Qt::WA_DeleteOnClose, true);
		lobbyPage->resize(800, 600);
		lobbyPage->show();
	}
	else
	{
		QMessageBox::information(this, "Error", "We couldn't find a lobby for you");
	}
}

void MainPage::on_Lobby_clicked()
{
	auto r = cpr::Put(cpr::Url{ "http://localhost:18080/createLobby" },
		cpr::Body{ "username=" + m_username });
	if (r.status_code == 200)
	{
		LobbyPage* lobbyPage = new LobbyPage(nullptr, m_username, m_username);
		lobbyPage->setAttribute(Qt::WA_DeleteOnClose, true);
		lobbyPage->resize(800, 600);
		lobbyPage->show();
	}
	else
	{
		QMessageBox::information(this, "Error", "We");
	}
}

void MainPage::on_Exit_clicked()
{
	this->close();
	//Aici trebuie scos din data de baze player-ul ca fiind online
}
