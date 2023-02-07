#include "Question.h"
void populateStorage(Storage& storage)
{
	std::string last_answer;
	bool ok = 1;
	std::ifstream fin("Questions.txt");
	while (!fin.eof())
	{
		std::string question;
		std::getline(fin, question);
		if (question[0] == 'Q' && question[1] == ':')
		{
			question = question.substr(2, question.size() - 2);
			std::string answerA, answerB, answerC, answerD, correctAnswer;
			std::getline(fin, correctAnswer);
			std::getline(fin, answerA);
			std::getline(fin, answerB);
			std::getline(fin, answerC);
			std::getline(fin, answerD);
			QuestionABCD a(question, answerA, answerB, answerC, answerD, correctAnswer);
			storage.insert(a);
		}
		else
		{
			std::string correctAnswer;
			std::getline(fin, correctAnswer);
			QuestionNumeric a(question, std::stof(correctAnswer));
			storage.insert(a);
		}
	}
}

std::vector<QuestionABCD> getQuestionsABCD(Storage storage, int size)
{
	std::vector<QuestionABCD> questions;
	for (const auto& questionABCD : storage.iterate<QuestionABCD>())
	{
		questions.push_back(questionABCD);

	}
	std::shuffle(questions.begin(), questions.end(), std::mt19937(std::random_device()()));
	std::vector<QuestionABCD> questionsABCD;
	for (int i = 0; i < size; i++)
	{
		questionsABCD.push_back(questions[i]);
	}
	return questionsABCD;
}

std::vector<QuestionNumeric> getQuestionsNumeric(Storage storage, int size)
{
	std::vector<QuestionNumeric> questions;
	for (const auto& questionNumeric : storage.iterate<QuestionNumeric>())
	{
		questions.push_back(questionNumeric);
	}
	std::shuffle(questions.begin(), questions.end(), std::mt19937(std::random_device()()));
	std::vector<QuestionNumeric> questionsNumeric;
	for (int i = 0; i < size; i++)
	{
		questionsNumeric.push_back(questions[i]);
	}
	return questionsNumeric;
}
