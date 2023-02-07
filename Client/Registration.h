#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Registration.h"
#include <cpr/cpr.h>
#include <crow.h>
#include <iostream>
#include<string>
#include <sstream>
#include "MainPage.h"
#include <qmessagebox.h>

class Registration : public QMainWindow
{
	Q_OBJECT

public:
	Registration(QWidget* parent = nullptr);
	~Registration();
	void setUsername(std::string username) { m_username = username; }
	void setPassword(std::string password) { m_password = password; }
	std::string getUsername()const { return m_username; }
	std::string getPassword()const { return m_password; }
public slots:
	void on_Login_clicked();
	void on_Register_clicked();
private:
	std::shared_ptr<MainPage> m_mainPage;
	Ui::RegistrationClass ui;
	std::string m_username;
	std::string m_password;

};
