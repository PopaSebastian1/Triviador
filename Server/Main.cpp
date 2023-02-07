#include <iostream>
#include <fstream>
#include"Question.h"
#include "Game.h"
#include "User.h"
#include "GameManager.h"
namespace sql = sqlite_orm;
int main()
{
	std::vector<QuestionABCD>multipleAnswer;
	std::vector<QuestionNumeric>numericAnswer;
	const std::string db_file = "questions.sqlite";
	const std::string db_file_user = "users.sqlite";
	const std::string db_file_game = "games.sqlite";
	Storage db = createStorage(db_file);
	UsersStorage dbUser = createStorageUser(db_file_user);
	GamesStorage dbGame = createStorageGame(db_file_game);
	dbUser.sync_schema();
	db.sync_schema();
	dbGame.sync_schema();
	int contorABCD = 0;
	int contorNumeric = 0;
	int size = 0;
	GameManager game(size);
	auto initQuestionCounts = db.count<QuestionABCD>();
	auto initQuestionNumeriCount = db.count<QuestionNumeric>();
	if (initQuestionCounts == 0 && initQuestionNumeriCount == 0)
	{
		populateStorage(db);
	}
	multipleAnswer = getQuestionsABCD(db, 30);
	numericAnswer = getQuestionsNumeric(db, 30);
	crow::SimpleApp app;
	CROW_ROUTE(app, "/")([]() {
		return "This is an example app of crow and sql-orm";
		});
	CROW_ROUTE(app, "/questionsABCD")([&db]() {
		std::vector<crow::json::wvalue> question_json;

		for (const auto& questionABCD : db.iterate<QuestionABCD>())
		{
			question_json.push_back(crow::json::wvalue{
				{"id", questionABCD.id},
				{"question", questionABCD.m_question},
				{"answerA", questionABCD.m_answerA},
				{"answerB", questionABCD.m_answerB},
				{"answerC", questionABCD.m_answerC},
				{"answerD", questionABCD.m_answerD},
				{"correctAnswer", questionABCD.m_correctAnswer}
				});
		}
		return crow::json::wvalue{ question_json };
		});
	CROW_ROUTE(app, "/questionsNumeric")([&db]()
		{

			std::vector<crow::json::wvalue> question_json;
			for (const auto& questionNumeric : db.iterate<QuestionNumeric>())
			{
				question_json.push_back(crow::json::wvalue{
					{"id", questionNumeric.id},
					{"question", questionNumeric.m_question},
					{"correctAnswer", questionNumeric.m_correctAnswer}
					});
			}
			return crow::json::wvalue{ question_json };
		});

	auto& addUser = CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::PUT);
	addUser(RegisterHandler(dbUser));
	auto& searchUser = CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST);
	searchUser(LoginHandler(dbUser));
	CROW_ROUTE(app, "/randomABCDQuestion")([&db]() {
		using namespace sqlite_orm;
		auto rows = db.select(sql::columns(&QuestionABCD::id, &QuestionABCD::m_question, &QuestionABCD::m_correctAnswer, &QuestionABCD::m_answerA, &QuestionABCD::m_answerB, &QuestionABCD::m_answerC, &QuestionABCD::m_answerD),
			sql::where(sql::c(&QuestionABCD::id) == 1));
		const auto& [id, question, m_correctAnswer, m_answerA, m_answerB, m_answerC, m_answerD] = rows[0];
		crow::json::wvalue q
		{
			{"id", id},
			{"question",question},
			{"correctAnswer",m_correctAnswer},
			{"answerA",m_answerA},
			{"answerB",m_answerB},
			{"answerC",m_answerC},
			{"answerD",m_answerD},
		};

		return q;
		});
	CROW_ROUTE(app, "/randomNumericQuestion")([&db]() {
		using namespace sqlite_orm;
		auto rows = db.select(sql::columns(&QuestionNumeric::id, &QuestionNumeric::m_question, &QuestionNumeric::m_correctAnswer),
			sql::where(sql::c(&QuestionNumeric::id) == 1));
		const auto& [id, question, m_correctAnswer] = rows[0];
		crow::json::wvalue q
		{
			{"id", id},
			{"question",question},
			{"correctAnswer",m_correctAnswer},
		};
		return q;
		});

	//CROW_ROUTE(app, "/Account")([&dbUser]() {
	//	std::vector<crow::json::wvalue> users_json;
	//	for (const auto& users : dbUser.iterate<User>())
	//	{
	//		users_json.push_back(crow::json::wvalue{
	//			{"id", users.getId()},
	//			{"username", users.getUsername()},
	//			{"matchesPlayed", users.getMatchesPlayed()},
	//			{"matchesWon", users.getMatchesWon()},
	//			{"level", users.getLevel()}
	//			});
	//	}
	//	return crow::json::wvalue{ users_json };
	//	});
	auto& account = CROW_ROUTE(app, "/Account").methods(crow::HTTPMethod::Get);
	account(AccountHandler(dbUser));
	
	//create a crow route to create a lobby
	auto& createLobby = CROW_ROUTE(app, "/createLobby").methods(crow::HTTPMethod::PUT);
	createLobby(CreateLobbyHandler(dbGame));
	//create a crow route to join a lobby
	auto& joinLobby = CROW_ROUTE(app, "/joinLobby").methods(crow::HTTPMethod::POST);
	joinLobby(JoinLobbyHandler(dbGame));
	//create a crow route to get all players in a lobby
	auto& getPlayers = CROW_ROUTE(app, "/players").methods(crow::HTTPMethod::Get);
	getPlayers(GetPlayersHandler(dbGame));
	//create a crow route to start a game
	auto& startGame = CROW_ROUTE(app, "/start").methods(crow::HTTPMethod::POST);
	startGame(StartGameHandler(game));
	auto& isStarted = CROW_ROUTE(app, "/isStarted").methods(crow::HTTPMethod::POST);
	isStarted(StartedHandler(game));
	game.setQuestionNumeric(numericAnswer);
	game.setQuestionABCD(multipleAnswer);
	auto& getQuestionABCDS = CROW_ROUTE(app, "/getQuestionABCD").methods(crow::HTTPMethod::Get);
	getQuestionABCDS(GetQuestionABCDHandler(game));
	auto& getQuestionNumerics = CROW_ROUTE(app, "/getQuestionNumeric").methods(crow::HTTPMethod::Get);
	getQuestionNumerics(GetQuestionNumericHandler(game));
	auto& getRoundType = CROW_ROUTE(app, "/checkStage").methods(crow::HTTPMethod::Post);
	getRoundType(GetRoundType(game));
	auto& changeStage = CROW_ROUTE(app, "/changeStage").methods(crow::HTTPMethod::Post);
	changeStage(ChangeRoundType(game));
	auto& getQuestionNumericAnswer = CROW_ROUTE(app, "/answerNumeric").methods(crow::HTTPMethod::Post);
	getQuestionNumericAnswer(getNumericAnswers(game));
	auto& turn = CROW_ROUTE(app, "/checkTurn").methods(crow::HTTPMethod::Post);
	turn(isYourTurn(game));
	auto& base = CROW_ROUTE(app, "/takeBase").methods(crow::HTTPMethod::Post);
	base(takeBase(game));
	CROW_ROUTE(app, "/getRegions")([&game]() {
		std::vector<crow::json::wvalue> x;
		auto y = game.getRegions();
		for (int i = 0; i < y.size(); i++)
		{
			x.push_back(crow::json::wvalue{
				{"id", i},
				{"owner", y[i].getOwner()},
				{"points",y[i].getPoints()},
				});

		}
		return  crow::json::wvalue{ x };
		});
	CROW_ROUTE(app, "/getPlayersScore")([&game]() {
		std::vector<crow::json::wvalue> x;
		auto y = game.getPlayers();
		for (int i = 0; i < y.size(); i++)
		{
			x.push_back(crow::json::wvalue{
				{"username", y[i].getName()},
				{"score", y[i].getScore()},
				});
		}
		return  crow::json::wvalue{ x };
		});
	auto& takeRegions = CROW_ROUTE(app, "/takeRegion").methods(crow::HTTPMethod::Post);
	takeRegions(takeRegion(game));
	auto& clicked = CROW_ROUTE(app, "/clicked").methods(crow::HTTPMethod::Post);
	clicked(clickedForDuel(game));
	auto& getABCDanswers = CROW_ROUTE(app, "/answerABCD").methods(crow::HTTPMethod::Post);
	getABCDanswers(getMultipleAnswers(game));
	auto& getWinner = CROW_ROUTE(app, "/getWinner").methods(crow::HTTPMethod::Get);
	getWinner(getWinnerForGame(game));
	
	app.port(18080).multithreaded().run();
	return 0;
}
