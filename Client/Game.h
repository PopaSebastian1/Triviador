#pragma once
#include <QMainWindow>
#include "ui_Game.h"
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <vector>
#include <iostream>
#include <cpr/cpr.h>
#include <qtimer.h>
#include <crow.h>
#include "Play.h"

class Game : public QMainWindow
{
	Q_OBJECT

public:
	Game(QWidget* parent = nullptr, int rows = 0, int columns = 0, std::string username = "", std::string Owner = "");
	~Game();
private slots:
	void on_button_clicked();
	void players();
	void checkStage();
	void changeStage();
	bool checkTurn();
	void makeButtonsUnusable();
	void takeBase()
	{
		int ok = 0;
		if (checkTurn() && stage == "SelectBase") {
			QObject* senderObj = sender();
			QPushButton* button = qobject_cast<QPushButton*>(senderObj);
			int i = searchIndex(button);
			std::string index = std::to_string(i);
			while (ok == 0) {
				cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/takeBase" },
					cpr::Body{ "username=" + username + "&index=" + index });
				std::string response = r.text;
				if (r.status_code == 200)
				{
					if (response == "Base already owned")
					{
						QMessageBox::information(this, "Error", "Base already owned");
					}
					else if (response == "Base taken")
					{
						ok = 1;
					}
				}
			}
		}
		else
		{
			QMessageBox::information(this, "Error", "It's not your turn");
		}
	};
	void takeRegion();
	void getPoints();
	void colorTheButtons();
	int searchIndex(QPushButton* button);
	void duel();
	void gameWinner();
public slots:
	void updateGame();
	void debugChangeStage(std::string stageName)
	{
		stage = stageName;
	};
private:
	Ui::GameClass ui;
	std::vector<QPushButton*>buttons;
	std::vector<std::pair<int, std::string>>scores;
	std::string m_Answer;
	std::string username;
	std::string m_owner;
	std::string stage;
	QTimer* timer;
	QTimer* timer2;
};
