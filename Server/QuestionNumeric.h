#pragma once
#include <string>
#include <iostream>
struct QuestionNumeric
{
	int id;
	std::string m_question;
	int m_correctAnswer;
	QuestionNumeric(std::string question, int correctAnswer);
	QuestionNumeric() = default;
	//Get
	std::string getQuestion() const;
	int getCorrectAnswer() const;
	//Set
	void setQuestion(std::string question);
	void setCorrectAnswer(int correctAnswer);
	//Overload operator
	friend std::ostream& operator<<(std::ostream& out, const QuestionNumeric& question);
	QuestionNumeric& operator=(const QuestionNumeric& other);

	~QuestionNumeric();
};