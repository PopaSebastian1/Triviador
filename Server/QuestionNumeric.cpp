#include "QuestionNumeric.h"

QuestionNumeric::QuestionNumeric(std::string question, int correctAnswer) :m_question(question), m_correctAnswer(correctAnswer)
{
}
QuestionNumeric& QuestionNumeric::operator=(const QuestionNumeric& other)
{
	m_question = other.getQuestion();
	m_correctAnswer = other.getCorrectAnswer();

	return *this;
}
QuestionNumeric::~QuestionNumeric()
{

}
void QuestionNumeric::setQuestion(std::string question)
{
	m_question = question;
}
void QuestionNumeric::setCorrectAnswer(int correctAnswer)
{
	m_correctAnswer = correctAnswer;
}
std::string QuestionNumeric::getQuestion() const
{
	return m_question;
}
int QuestionNumeric::getCorrectAnswer() const
{
	return m_correctAnswer;
}
std::ostream& operator<<(std::ostream& out, const QuestionNumeric& question)
{
	out << question.m_question << std::endl;
	return out;
}
