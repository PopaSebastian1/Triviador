#pragma once
#include "DuelManager.h"
#include "Game.h"
#include "Question.h"
#include <vector>
#include <queue>
#include <random>
#include "GamePlayer.h"
enum class Round :uint8_t
{
	Waiting,
	NumericQuestion,
	ABCDQuestion,
	SelectBase,
	SelectRegions,
	Duel,
	End,
};
struct compare {
	bool operator() (const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
	{
		return a.second > b.second;
	}
};
class GameManager
{
	std::vector<GamePlayer> players;
	std::vector<Region> regions;
	std::vector<std::string > answerd;
	std::vector<std::string > rounds;
	std::vector<QuestionABCD>multipleAnswerQuestion;
	std::vector<QuestionNumeric>numericAnswerQuestion;
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> answersNumeric;
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> answerNumericForDuel;
	std::vector<std::pair<std::string, bool>>answersABCD;
	std::string winnerABCD;
	std::vector<std::string>duelRanking;
	std::vector<std::string>opponents;
	Round roundType;
	//DuelManager DuelManager;
	int playerCount;
	int questionABCDcount = 0;
	int questionNumericCount = 0;
	int duelRounds = 0;
	int lini, coloane;
	std::vector<std::string>alreadyRequested;
	int regionsOwned;
	int regionSelected = -1;
public:
	GameManager(int size = 0);
	void setPlayerCount(int playerCount) { this->playerCount = playerCount; }
	int getPlayerCount() const { return this->playerCount; }
	int getNumberOfPlayers() { return players.size(); }
	void setBoardDimension(int playerCount) { regions.resize(playerCount); }
	void setLini(int lini) { this->lini = lini; }
	void setColoane(int coloane) { this->coloane = coloane; }
	void setPlayers(std::vector<GamePlayer> players) { this->players = players; }
	int duelRoundsGet() const { return duelRounds; };
	void duelRoundsIncrement();
	void setDuelRounds();
	int searchPlayer(std::string name);
	void addPlayerScore(int index, int score);
	void addPlayer(GamePlayer player) { players.push_back(player); }
	void setQuestionABCD(std::vector<QuestionABCD> multipleAnswerQuestion) { this->multipleAnswerQuestion = multipleAnswerQuestion; }
	//std::vector<QuestionABCD> getQuestionABCD() { return this->multipleAnswerQuestion; }
	void setQuestionNumeric(std::vector<QuestionNumeric> numericAnswerQuestion) { this->numericAnswerQuestion = numericAnswerQuestion; }
	int getQuestionABCDCount() { return this->questionABCDcount; }
	int getQuestionNumericCount() { return this->questionNumericCount; }
	void setQuestionABCDCount(int questionABCDcount) { this->questionABCDcount = questionABCDcount; }
	void setQuestionNumericCount(int questionNumericCount) { this->questionNumericCount = questionNumericCount; }
	std::vector<QuestionABCD> getQuestionABCD() { return this->multipleAnswerQuestion; }
	std::vector<QuestionNumeric> getQuestionNumeric() { return this->numericAnswerQuestion; }
	void setQuestionABCDdimension(int nr) { this->multipleAnswerQuestion.resize(nr); }
	void setQuestionNumericdimension(int nr) { this->numericAnswerQuestion.resize(nr); }
	void setRoundType(Round roundType) { this->roundType = roundType; }
	Round getRoundType() { return this->roundType; }
	void setAlreadyRequestedDim(int size) { alreadyRequested.resize(size); }
	void addAlreadyRequested(std::string name) { alreadyRequested.push_back(name); }
	bool findAlreadyRequested(std::string name);
	bool isBaseStage();
	bool isRegionStage();
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> getAnswersNumericForDuel() { return this->answerNumericForDuel; }
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> getAnswersNumeric() { return this->answersNumeric; }
	void addAnswersNumeric(std::pair<std::string, int> answer) { answersNumeric.push(answer); }
	void setAnswersNumeric(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>) { this->answersNumeric = answersNumeric; }
	void addAnswersNumericForDuel(std::pair<std::string, int> answer) { answerNumericForDuel.push(answer); }
	void setAnswersNumericForDuel(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>) { this->answerNumericForDuel = answerNumericForDuel; }
	std::vector<Region> getRegions() { return this->regions; }
	void setRegions(std::vector<Region> regions) { this->regions = regions; }
	void deleteAnswersNumeric() { this->answersNumeric.pop(); }
	void changeRegionOwner(std::string name, int region);
	void alreadyRequestedClear() { alreadyRequested.clear(); }
	void clearABCDanswers() { answersABCD.clear(); }
	void createRankingForTeritory(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>& a);
	void addItemsInPq(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>a);
	void setBase(int region);
	int getRegionsOwned() const { return this->regionsOwned; }
	void setRegionsOwned(int regionsOwned) { this->regionsOwned = regionsOwned; }
	void addRegionsOwned() { this->regionsOwned++; }
	std::vector<GamePlayer> getPlayers() const { return this->players; }
	void setDuelRanking(std::vector<std::string> duelRanking) { this->duelRanking = duelRanking; }
	std::vector<std::string> getDuelRanking() { return this->duelRanking; }
	void setOpponents(std::vector<std::string> opponents) { this->opponents = opponents; }
	std::vector<std::string> getOpponents() const { return this->opponents; }
	void setWinnerABCD(std::string winnerABCD) { this->winnerABCD = winnerABCD; }
	std::string getWinnerABCD() const { return this->winnerABCD; }
	void addAnswersABCD(std::pair<std::string, bool>answer) { answersABCD.push_back(answer); }
	std::vector<std::pair<std::string, bool>> getAnswersABCD() { return this->answersABCD; }
	std::string getDuelWinner();
	void setRegionSelected(int regionSelected) { this->regionSelected = regionSelected; }
	int getRegionSelected() const { return this->regionSelected; }
	void clearOpponents() { opponents.clear(); }
	void decrementRegionPoints(int index);
	void incrementRegionPoints(int index);
	void clearAnswersNumeric() { this->answerNumericForDuel = std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>(); }
	std::pair<std::string, int> duelWinnerNumeric();
	std::string checkGameWinner();
};

//HANDLERS

class StartGameHandler
{
	GameManager& gameManager;
public:
	StartGameHandler(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class StartedHandler
{
	GameManager& gameManager;
public:
	StartedHandler(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class GetQuestionABCDHandler
{
	GameManager& gameManager;
public:
	GetQuestionABCDHandler(GameManager& gameManager) : gameManager(gameManager) {}
	crow::json::wvalue operator()(const crow::request& req) const;
};
class GetQuestionNumericHandler
{
	GameManager& gameManager;
public:
	GetQuestionNumericHandler(GameManager& gameManager) : gameManager(gameManager) {}
	crow::json::wvalue operator()(const crow::request& req) const;
};
class GetRoundType
{
	GameManager& gameManager;
public:
	GetRoundType(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class ChangeRoundType
{
	GameManager& gameManager;
public:
	ChangeRoundType(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class getNumericAnswers
{
	GameManager& gameManager;
public:
	getNumericAnswers(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class getMultipleAnswers
{
	GameManager& gameManager;
public:
	getMultipleAnswers(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class isYourTurn
{
	GameManager& gameManager;
public:
	isYourTurn(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class takeBase
{
	GameManager& gameManager;
public:
	takeBase(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class takeRegion
{
	GameManager& gameManager;
public:
	takeRegion(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class clickedForDuel {
	GameManager& gameManager;
public:
	clickedForDuel(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};
class getWinnerForGame
{
	GameManager& gameManager;
public:
	getWinnerForGame(GameManager& gameManager) : gameManager(gameManager) {}
	crow::response operator()(const crow::request& req) const;
};