#include "Game.h"
#include <qgridlayout.h>
#include "NumericAnswer.h"
#include "Play.h"
#include <qpushbutton.h>
#include <QTimer>
#include <qdebug.h>
#include <qmessagebox.h>
#include "utils.h"

Game::Game(QWidget* parent, int rows, int columns, std::string username, std::string owner)
	: QMainWindow(parent)
{
	m_owner = owner;
	this->username = username;
	ui.setupUi(this);
	this->resize(1400, 800);
	buttons.resize(rows * columns);
	QGridLayout* layout = new QGridLayout();
	//buttons.resize(rows * columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int index = i * columns + j;
			buttons[i * columns + j] = new QPushButton(QString::number(index));
			buttons[i * columns + j]->setMinimumSize(100, 100);
			buttons[i * columns + j]->setStyleSheet("border-image:transparent; background-color: white;");
			buttons[i * columns + j]->setStyleSheet("QPushButton {border-image:transparent; background-color: white; border: 2px solid black; border-radius: 10px;}");
			buttons[i * columns + j]->setCursor(Qt::PointingHandCursor);
			buttons[i * columns + j]->setText(QString::number(i * columns + j));
			buttons[i * columns + j]->setObjectName(QString::number(i * columns + j));
			layout->addWidget(buttons[i * columns + j], i, j);
			//bring the buttons closer together
			layout->setSpacing(0);
			connect(buttons[i * columns + j], &QPushButton::clicked, this, &Game::on_button_clicked);
		}
		//create a new row in the grid layout
		layout->setRowStretch(i, 1);
	}
	this->centralWidget()->setLayout(layout);
	layout->setGeometry(QRect(0, 0, 1400, 800));
	layout->setAlignment(Qt::AlignCenter);
	players();
	this->setWindowTitle(username.c_str());
	timer2 = new QTimer(this);
	timer2->start(200);
	connect(timer2, SIGNAL(timeout()), this, SLOT(updateGame()));
	timer = new QTimer(this);
	timer->start(200);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkStage()));

}
Game::~Game()
{}
void Game::players()
{
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:18080/players" },
		cpr::Body{ "&username=" + m_owner });
	std::string response = r.text;
	std::vector<std::string> players = split(response, ",");
	ui.Player1->setText(players[0].c_str());
	ui.Player2->setText(players[1].c_str());
	if (players[3] != "*")
	{
		ui.Player3->setText(players[2].c_str());
		ui.Player4->setText(players[3].c_str());

	}
	else if (players[2] == "*")
	{
		ui.Player3->setVisible(false);
		ui.Player4->setVisible(false);
	}
	else if (players[3] == "*")
	{
		ui.Player3->setText(players[2].c_str());
		ui.Player4->setVisible(false);
	}
}
void Game::checkStage()
{
	cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/checkStage" },
		cpr::Body{ "&username=" + username });
	std::string response = r.text;
	if (r.status_code == 200)
	{
		if (response == "Waiting")
		{
			updateGame();
			stage = response;
		}
		else if (response == "ABCDQuestion")
		{
			stage = response;
			Play* play = new Play(this, username);
			play->show();
		}
		else if (response == "NumericQuestion")
		{
			stage = response;
			NumericAnswer* numeric = new NumericAnswer(this, username);
			numeric->show();
		}
		else if (response == "SelectBase")
		{
			stage = response;
		}
		else if (response == "SelectRegions")
		{
			stage = response;
		}
		if (response == "Duel")
		{
			stage = response;
		}
		//check if response contains "End"
		if (response == "End")
		{
			stage = response;
			gameWinner();
			timer->stop();
			timer2->stop();
			
		}
	}
}
void Game::changeStage()
{
	cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/changeStage" },
		cpr::Body{ "&username=" + username });
}
bool Game::checkTurn()
{
	cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/checkTurn" },
		cpr::Body{ "&username=" + username });
	if (r.status_code == 200)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Game::makeButtonsUnusable()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setDisabled(true);
	}
}
void Game::updateGame()
{
	ui.GameStage->setText(stage.c_str());
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:18080/getRegions" });
	auto response = crow::json::load(r.text);
	/*
	bool isTurn = checkTurn();
	if (isTurn) {
		ui.IsPlayerTurn->setText(QString::fromStdString("Your Turn"));
	}
	else {
		ui.IsPlayerTurn->setText(QString::fromStdString("Opponent's Turn"));
	}
	*/
	for (const auto& region : response)
	{
		std::int16_t i = region["id"].i();
		std::string owner = region["owner"].s();
		std::int16_t points = region["points"].i();
		buttons[i]->setObjectName(owner.c_str());
		buttons[i]->setText(QString::number(points));
	}
	colorTheButtons();
	getPoints();
	this->update();
}
void Game::takeRegion()
{
	int ok = 0;
	if (checkTurn() && stage == "SelectRegions") {
		QObject* senderObj = sender();
		QPushButton* button = qobject_cast<QPushButton*>(senderObj);
		int i = searchIndex(button);
		std::string index = std::to_string(i);
		while (ok == 0) {
			cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/takeRegion" },
				cpr::Body{ "username=" + username + "&index=" + index });
			auto response = r.text;
			if (response == "Region already owned")
			{
				QMessageBox::information(this, "Error", "Base already owned");
			}
			else if (response == "Region taken")
			{
				ok = 1;
			}
		}
	}
	else
	{
		QMessageBox::information(this, "Error", "It's not your turn");
	}
}
void Game::getPoints()
{
	cpr::Response r = cpr::Get(cpr::Url{ "http://localhost:18080/getPlayersScore" });
	auto response = crow::json::load(r.text);
	ui.Player3Points->setVisible(false);
	ui.Player4Points->setVisible(false);
	for (const auto& player : response)
	{
		std::string name = player["username"].s();
		std::int16_t points = player["score"].i();
		std::string point = std::to_string(points);
		if (name == ui.Player1->text().toUtf8().constData())
		{
			ui.Player1Points->setText(point.c_str());
		}
		else if (name == ui.Player2->text().toUtf8().constData())
		{
			ui.Player2Points->setText(point.c_str());
		}
		else if (name == ui.Player3->text().toUtf8().constData())
		{
			ui.Player3Points->setVisible(true);
			ui.Player3Points->setText(point.c_str());

		}
		else if (name == ui.Player4->text().toUtf8().constData())
		{
			ui.Player4Points->setVisible(true);
			ui.Player4Points->setText(point.c_str());
		}
	}
	//this->update();

}
void Game::colorTheButtons()
{
	for (const auto& button : buttons)
	{
		if (button->objectName() == ui.Player1->text())
		{
			button->setStyleSheet("QPushButton {border-image:transparent; background-color: red; border: 2px solid black; border-radius: 10px;}");
		}
		else if (button->objectName() == ui.Player2->text())
		{
			button->setStyleSheet("QPushButton {border-image:transparent; background-color: blue; border: 2px solid black; border-radius: 10px;}");
		}
		else if (button->objectName() == ui.Player3->text() && ui.Player3->isVisible())
		{
			button->setStyleSheet("QPushButton {border-image:transparent; background-color: green; border: 2px solid black; border-radius: 10px;}");
		}
		else if (button->objectName() == ui.Player4->text() && ui.Player4->isVisible())
		{
			button->setStyleSheet("QPushButton {border-image:transparent; background-color: yellow; border: 2px solid black; border-radius: 10px;}");
		}
	}
	//	this->update();
}
int Game::searchIndex(QPushButton* a)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i] == a)
		{
			return i;
		}
	}
}
void Game::duel()
{
	if (checkTurn() && stage == "Duel") {
		QObject* senderObj = sender();
		QPushButton* button = qobject_cast<QPushButton*>(senderObj);
		std::string username2 = button->objectName().toUtf8().constData();
		int i = searchIndex(button);
		std::string index = std::to_string(i);
		cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/clicked" },
			cpr::Body{ "username=" + username + "&index=" + index + "&username2=" + username2 });
	}
	else
	{
		QMessageBox::information(this, "Error", "It's not your turn");
	}

}
void Game::gameWinner()
{
	cpr::Response r=cpr::Get(cpr::Url{ "http://localhost:18080/getWinner" });
	auto response = r.text;
	QString winner = QString(response.c_str());
	if (r.status_code == 200)
	{
		QMessageBox::information(this, "Game Over", "The winner is "+winner);
	}
}
void Game::on_button_clicked()
{
	if (stage == "SelectBase")
	{
		takeBase();
	}
	else if (stage == "SelectRegions")
	{
		takeRegion();
	}
	else if (stage == "Duel")
	{
		duel();
	}

}