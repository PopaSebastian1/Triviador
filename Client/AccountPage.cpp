#include "AccountPage.h"


void AccountPage::displayUsername()
{
	auto responseAccount = cpr::Get(cpr::Url{ "http://localhost:18080/Account" },
		cpr::Body{ "username=" + m_username });
	auto AccountRow = crow::json::load(responseAccount.text);
	std::string username = AccountRow["username"].s();
	ui.usernameEdit->setText(username.c_str());
	auto matchesPlayed = AccountRow["matchesPlayed"].i();
	ui.matchesPlayedEdit->setText(QString::number(matchesPlayed));
	auto matchesWon = AccountRow["matchesWon"].i();
	ui.matchesWonEdit->setText(QString::number(matchesWon));
	auto level = AccountRow["level"].i();
	ui.levelEdit->setText(QString::number(level));
}

AccountPage::AccountPage(QWidget* parent,std::string username)
	: QMainWindow(parent)
{
	m_username = username;
	ui.setupUi(this);
	displayUsername();
}

AccountPage::~AccountPage()
{}
