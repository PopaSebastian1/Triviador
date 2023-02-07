#pragma once
#include <string>
#include <iostream>
struct QuestionABCD
{
	int id;
	std::string m_question;
	std::string m_answerA;
	std::string m_answerB;
	std::string m_answerC;
	std::string m_answerD;
	std::string m_correctAnswer;
	QuestionABCD() = default;
	QuestionABCD(std::string question, std::string answerA, std::string answerB, std::string answerC, std::string answerD, std::string correctAnswer);
	//Get
	std::string getQuestion() const;
	std::string getAnswerA() const;
	std::string getAnswerB() const;
	std::string getAnswerC() const;
	std::string getAnswerD() const;
	std::string getCorrectAnswer() const;

	//Set
	void setQuestion(std::string question);
	void setAnswerA(std::string answerA);
	void setAnswerB(std::string answerB);
	void setAnswerC(std::string answerC);
	void setAnswerD(std::string answerD);
	void setCorrectAnswer(std::string correctAnswer);

	//Overloads
	friend std::ostream& operator<<(std::ostream& out, const QuestionABCD& question);
	QuestionABCD& operator=(const QuestionABCD& other);

	~QuestionABCD();
};


