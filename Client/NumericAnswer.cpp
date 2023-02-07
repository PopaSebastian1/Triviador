#include "NumericAnswer.h"

NumericAnswer::NumericAnswer(QWidget* parent, std::string username)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_username = username;
	auto responseNumericQuestion = cpr::Get(cpr::Url{ "http://localhost:18080/getQuestionNumeric" });
	auto numericQuestionRow = crow::json::load(responseNumericQuestion.text);
	std::string numericQuestion = numericQuestionRow["question"].s();
	ui.Question->setText(numericQuestion.c_str());
	//set the title of the window to the username
	std::string title = m_username;
	this->setWindowTitle(title.c_str());
}

NumericAnswer::~NumericAnswer()
{


}

