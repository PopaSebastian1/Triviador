#pragma once
#include <vector>
#include <string>
#include "QuestionABCD.h"
#include "QuestionNumeric.h"
#include <random>
#include <algorithm>
#include <random>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <memory>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;
struct Question
{
};
inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table
		(
			"QuestionsABCD",
			sql::make_column("id", &QuestionABCD::id, sql::primary_key()),
			sql::make_column("question", &QuestionABCD::m_question),
			sql::make_column("answerA", &QuestionABCD::m_answerA),
			sql::make_column("answerB", &QuestionABCD::m_answerB),
			sql::make_column("answerC", &QuestionABCD::m_answerC),
			sql::make_column("answerD", &QuestionABCD::m_answerD),
			sql::make_column("correctAnswer", &QuestionABCD::m_correctAnswer)
		),
		sql::make_table(
			"QuestionsNumeric",
			sql::make_column("id", &QuestionNumeric::id, sql::primary_key()),
			sql::make_column("question", &QuestionNumeric::m_question),
			sql::make_column("correctAnswer", &QuestionNumeric::m_correctAnswer)
		)
	);
}
using Storage = decltype(createStorage(""));
void populateStorage(Storage& storage);
std::vector<QuestionABCD> getQuestionsABCD(Storage storage, int size);
std::vector<QuestionNumeric>getQuestionsNumeric(Storage storage, int size);