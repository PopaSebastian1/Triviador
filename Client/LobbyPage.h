#pragma once
#include <QMainWindow>
#include "Game.h"
#include <qtimer.h>
#include "utils.h"
#include <cpr/cpr.h>
#include "ui_LobbyPage.h"

class LobbyPage :
	public QMainWindow
{
	Q_OBJECT

private:
	Ui::Lobby ui;
	std::string m_username;
	int nrPlayeri;
	std::string m_Owner;
	QTimer* timer;
	QTimer* timer2;
public slots:
	void on_StartGame_clicked() {
		//switch in functie de numarul de playeri conectati
	/*	Game* game = new Game(nullptr, 3, 3);
		game->resize(1200, 600);
		game->show();*/
		std::string player1 = ui.Player1->text().toUtf8().constData();
		std::string player2 = ui.Player2->text().toUtf8().constData();
		std::string player3 = ui.Player3->text().toUtf8().constData();
		std::string player4 = ui.Player4->text().toUtf8().constData();
		auto r = cpr::Post(cpr::Url{ "http://localhost:18080/start" },
			cpr::Body{ "player1=" + player1 + "&player2=" + player2 + "&player3=" + player3 + "&player4=" + player4 });
	};
	void reincarca()
	{
		cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:18080/players" },
			cpr::Body{ "&username=" + m_Owner });
		std::string response = r.text;
		qDebug() << r.text.c_str();
		std::vector<std::string> players = split(response, ",");
		ui.Player1->setText(m_Owner.c_str());
		if (players[1] != "*")
		{
			ui.Player2->setText(players[1].c_str());
		}
		if (players[2] != "*")
		{
			ui.Player3->setText(players[2].c_str());
		}
		if (players[3] != "*")
		{
			ui.Player4->setText(players[3].c_str());
		}
		this->update();
	}
	void numberOfPlayers();
	/*void createPage();*/
	int startGame();
	void stop(QTimer* a);
public:
	LobbyPage(QWidget* parent = nullptr, std::string owener = "", std::string username = "");
	~LobbyPage() {
		timer->stop();
		timer2->stop();
	}
};

