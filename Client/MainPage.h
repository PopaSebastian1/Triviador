#pragma once
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QMainWindow>
#include "ui_MainPage.h"
#include "AccountPage.h"
#include <cpr/cpr.h>
#include <crow.h>
#include "LobbyPage.h"
class MainPage : public QMainWindow
{
	Q_OBJECT

public:
	MainPage(QWidget* parent = nullptr, std::string username = "", std::string password = "");
	~MainPage();

public slots:
	void on_Cont_clicked();
	void on_Play_clicked();
	void on_doi_clicked();
	void on_trei_clicked();
	void on_patru_clicked();
	void on_JoinLobby_clicked();
	void on_Lobby_clicked();
	void on_Exit_clicked();

private:
	Ui::MainPageClass ui;
	AccountPage* accountPage;
	std::string m_username;
};
