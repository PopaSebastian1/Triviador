#include "Registration.h"
#include <regex>
Registration::Registration(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

Registration::~Registration()
{}

void Registration::on_Register_clicked()
{
	std::string username = ui.UsernameInput->text().toUtf8().constData();
	std::string password = ui.PassInput->text().toUtf8().constData();
	m_username = username;
	m_password = password;
	std::regex reg("^(?=.*[0-9]).{6,}$");
	if (std::regex_match(password, reg)) {
		auto response = cpr::Put(
			cpr::Url{ "http://localhost:18080/register" },
			cpr::Body{ "username=" + username + "&password=" + password }
		);

		if (response.status_code == 200)
		{
			QMessageBox::information(this, "Success", "Registration successful");
		}
		else
		{
			QMessageBox::information(this, "Error", "Registration failed");
		}
	}
	else {
		QMessageBox::information(this, "Error", "Password must contain at least 6 characters and at least one number");
	}
}

void Registration::on_Login_clicked()
{
	std::string username = ui.UsernameInput->text().toUtf8().constData();
	std::string password = ui.PassInput->text().toUtf8().constData();
	auto response = cpr::Post(
		cpr::Url{ "http://localhost:18080/login" },
		cpr::Body{ "username=" + username + "&password=" + password }
	);
	if (response.status_code == 200)
	{
		m_mainPage = std::make_shared<MainPage>(nullptr, username);
		//* mainPage = new MainPage(nullptr, username, password);
		m_mainPage->show();
	}
	else
	{
		QMessageBox::information(this, "Error", "Login failed");
	}
}
