#pragma once

#include <QMainWindow>
#include "ui_NumericAnswer.h"
#include <cpr/cpr.h>
#include <crow.h>
#include <string>

class NumericAnswer : public QMainWindow
{
	Q_OBJECT

public:
	NumericAnswer(QWidget* parent = nullptr, std::string username = "");
	~NumericAnswer();
public slots:
	void on_sendAnswer_clicked()
	{
		std::string answer = ui.answer->text().toUtf8().constData();
		if (answer == "")
		{
			answer = "0";
		}
		auto r = cpr::Post(cpr::Url{ "http://localhost:18080/answerNumeric" },
			cpr::Body{ "username=" + m_username + "&answer=" + answer });
		this->close();
	}
private:
	Ui::NumericAnswerClass ui;
	std::string m_username;
};
