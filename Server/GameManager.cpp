#include "GameManager.h"
crow::response StartGameHandler::operator() (const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto player1 = urlArgs["player1"];
	auto player2 = urlArgs["player2"];
	auto player3 = urlArgs["player3"];
	auto player4 = urlArgs["player4"];
	GamePlayer p1(player1, 0);
	GamePlayer p2(player2, 0);
	gameManager.addPlayer(p1);
	gameManager.addPlayer(p2);
	if (player3 != "Empty")
	{
		GamePlayer p3(player3, 0);
		gameManager.addPlayer(p3);

	}
	if (player4 != "Empty")
	{
		GamePlayer p4(player4, 0);
		gameManager.addPlayer(p4);

	}
	gameManager.setPlayerCount(gameManager.getNumberOfPlayers());
	auto nr = gameManager.getNumberOfPlayers();
	if (nr == 2)
	{
		gameManager.setBoardDimension(9);
		gameManager.setLini(3);
		gameManager.setColoane(3);
		gameManager.setQuestionABCDdimension(30);
		gameManager.setQuestionNumericdimension(30);
		gameManager.setRoundType(Round::NumericQuestion);
		return crow::response(200);
	}
	else if (nr == 3)
	{

		gameManager.setBoardDimension(15);
		gameManager.setLini(5);
		gameManager.setColoane(3);
		gameManager.setQuestionABCDdimension(30);
		gameManager.setQuestionNumericdimension(30);
		gameManager.setRoundType(Round::NumericQuestion);
		return crow::response(201);

	}
	else if (nr == 4)
	{
		this->gameManager.setBoardDimension(24);
		gameManager.setLini(6);
		gameManager.setColoane(4);
		gameManager.setQuestionABCDdimension(30);
		gameManager.setQuestionNumericdimension(30);
		gameManager.setRoundType(Round::NumericQuestion);
		return crow::response(202);
	}
	else
	{
		return crow::response(400);
	}
}
GameManager::GameManager(int size) :roundType(Round::NumericQuestion)
{
	this->coloane = 0;
	this->lini = 0;
	this->playerCount = 0;
	this->regions.resize(size);
	this->players.resize(size);
	this->answerd.resize(size);
	this->rounds.resize(size);
	this->alreadyRequested.resize(size);
	this->regionsOwned = 0;
}
int GameManager::searchPlayer(std::string name)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].getName() == name) return i;
	}
}
void GameManager::duelRoundsIncrement()
{
	this->duelRounds++;
}
void GameManager::setDuelRounds()
{
	std::vector<GamePlayer>players = getPlayers();
	int nrRunde;
	int nr = players.size();
	if (nr == 2) nrRunde = 1;
	else if (nr == 3) nrRunde = 4;
	else if (nr == 4) nrRunde = 5;
	while (nrRunde > 0) {
		std::random_device device;
		std::mt19937 g(device());
		std::shuffle(players.begin(), players.end(), g); \
			for (int i = 0; i < players.size(); i++)
			{
				this->duelRanking.push_back(players[i].getName());
			}
		nrRunde--;
	}
}
void GameManager::addPlayerScore(int position, int score)
{
	players[position].setScore(players[position].getScore() + score);
}
bool GameManager::findAlreadyRequested(std::string name)
{
	for (auto s : alreadyRequested)
	{
		if (s == name) return true;
	}
	return false;
}
bool GameManager::isBaseStage()
{
	int nr = 0;
	for (auto c : regions)
	{
		if (c.getIsBase() == true) nr++;
	}
	if (nr == players.size()) return false;
	return true;
}
bool GameManager::isRegionStage()
{
	for (auto c : regions)
	{
		if (c.getIsOwned() == false) return true;
	}
	return false;
}
void GameManager::changeRegionOwner(std::string name, int region)
{
	regions[region].setOwner(name);
	regions[region].setIsBase(false);
	if (roundType == Round::SelectBase)
	{
		regions[region].setPoints(300);
	}
	else regions[region].setPoints(100);
}

void GameManager::createRankingForTeritory(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare>& a)
{
	while (!answersNumeric.empty())
	{
		int contor = 0;
		while (contor < answersNumeric.size() - 1)
		{
			a.push(answersNumeric.top());
			contor++;
		}
		answersNumeric.pop();
	}
}

void GameManager::addItemsInPq(std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> a)
{
	while (a.empty() == false)
	{
		answersNumeric.push(a.top());
		a.pop();
	}
}

void GameManager::setBase(int region)
{
	regions[region].setIsBase(true);
}

std::string GameManager::getDuelWinner()
{
	if (answersABCD[0].second == true)
	{
		if (answersABCD[1].second == true)
		{
			return "Draw";
		}
		else return answersABCD[0].first;
	}
	if (answersABCD[1].second == true)
	{
		return "Lost";
	}
	if (answersABCD[0].second == false && answersABCD[1].second == false)
	{
		return "Nobody Won";
	}
}
std::pair<std::string, int> GameManager::duelWinnerNumeric()
{
	return answerNumericForDuel.top();
}

std::string GameManager::checkGameWinner()
{
	int max_points = -1;
	std::string name;
	for (auto p : players)
	{
		if (p.getScore() > max_points)
		{
			max_points = p.getScore();
			name = p.getName();
		}
	}
	return name;
}

void GameManager::decrementRegionPoints(int index)
{
	this->regions[index].setPoints(this->regions[index].getPoints() - 100);
}

void GameManager::incrementRegionPoints(int index)
{
	this->regions[index].setPoints(this->regions[index].getPoints() + 100);
}


crow::response StartedHandler::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	if (gameManager.getNumberOfPlayers() >= 2)
	{
		return crow::response(200);
	}
	else
	{
		return crow::response(400);
	}
}

crow::json::wvalue GetQuestionABCDHandler::operator()(const crow::request& req) const
{
	crow::json::wvalue x;
	auto questions = gameManager.getQuestionABCD();
	x["question"] = questions[gameManager.getQuestionABCDCount()].getQuestion();
	x["answerA"] = questions[gameManager.getQuestionABCDCount()].getAnswerA();
	x["answerB"] = questions[gameManager.getQuestionABCDCount()].getAnswerB();
	x["answerC"] = questions[gameManager.getQuestionABCDCount()].getAnswerC();
	x["answerD"] = questions[gameManager.getQuestionABCDCount()].getAnswerD();
	//	gameManager.setQuestionABCDCount(gameManager.getQuestionABCDCount() + 1);
	return x;
}

crow::json::wvalue GetQuestionNumericHandler::operator()(const crow::request& req) const
{
	crow::json::wvalue x;
	auto questions = gameManager.getQuestionNumeric();
	x["question"] = questions[gameManager.getQuestionNumericCount()].getQuestion();
	return x;
}

crow::response GetRoundType::operator()(const crow::request& req) const
{
	auto url = parseUrlArgs(req.body);
	auto name = url["username"];
	Round a = gameManager.getRoundType();
	if (a == Round::Waiting)
	{
		return crow::response(200, "Waiting");
	}
	else if (a == Round::ABCDQuestion)
	{
		if (gameManager.findAlreadyRequested(name))
		{
			return crow::response(400, "AlreadyRequested");
		}
		else
		{
			gameManager.addAlreadyRequested(name);
			return crow::response(200, "ABCDQuestion");
		}
	}
	else if (a == Round::NumericQuestion)
	{
		if (gameManager.findAlreadyRequested(name))
		{
			return crow::response(400, "AlreadyRequested");
		}
		else
		{
			gameManager.addAlreadyRequested(name);
			return crow::response(200, "NumericQuestion");
		}
	}
	else if (a == Round::SelectBase)
	{
		return crow::response(200, "SelectBase");
	}
	else if (a == Round::Duel)
	{
		return crow::response(200, "Duel");
	}
	else if (a == Round::SelectRegions)
	{
		return crow::response(200, "SelectRegions");
	}
	else if (a == Round::End)
	{
		return crow::response(200, "End");
	}
}

crow::response ChangeRoundType::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	if (gameManager.findAlreadyRequested(username))
	{
		return crow::response(400);
	}
	else
	{
		gameManager.addAlreadyRequested(username);
		return crow::response(200);
	}
}

crow::response getNumericAnswers::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto answer = urlArgs["answer"];
	auto x = gameManager.getQuestionNumeric();
	int correctAnswer = x[gameManager.getQuestionNumericCount()].getCorrectAnswer();
	int answer1 = std::abs(correctAnswer - std::stoi(answer));

	auto questions = gameManager.getAnswersNumeric();
	questions.push(std::make_pair(username, answer1));
	gameManager.addAnswersNumeric(std::make_pair(username, answer1));
	auto opponents = gameManager.getOpponents();
	if (gameManager.getRegionSelected() != -1)
	{
		auto regions = gameManager.getRegions();
		if (opponents[0] == username || opponents[1] == username)
		{
			gameManager.addAnswersNumericForDuel(std::make_pair(username, answer1));
			int a = gameManager.getAnswersNumericForDuel().size();
			int op = opponents.size();
			if (gameManager.getAnswersNumericForDuel().size() == opponents.size())
			{
				gameManager.setQuestionNumericCount(gameManager.getQuestionNumericCount() + 1);
				if (gameManager.duelWinnerNumeric().first == opponents[0])
				{
					int runda = gameManager.duelRoundsGet();
					int runde = gameManager.getDuelRanking().size();
					if (regions[gameManager.getRegionSelected()].getPoints() > 100)
					{
						if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size() - 1)
						{
							gameManager.setRoundType(Round::End);
							int indexPlayer = gameManager.searchPlayer(opponents[1]);
							gameManager.addPlayerScore(indexPlayer, -100);
							gameManager.decrementRegionPoints(gameManager.getRegionSelected());
						}
						else {
							gameManager.decrementRegionPoints(gameManager.getRegionSelected());
							int indexPlayer = gameManager.searchPlayer(opponents[1]);
							gameManager.addPlayerScore(indexPlayer, -100);
							int indexPlayer1 = gameManager.searchPlayer(opponents[0]);
							gameManager.addPlayerScore(indexPlayer1, 100);
							gameManager.setRoundType(Round::Duel);
							gameManager.duelRoundsIncrement();
							gameManager.alreadyRequestedClear();
							gameManager.clearOpponents();
							gameManager.clearAnswersNumeric();
							gameManager.decrementRegionPoints(gameManager.getRegionSelected());

						}
						return crow::response(200);
					}
					else
					{
						int runda = gameManager.duelRoundsGet();
						int runde = gameManager.getDuelRanking().size();
						if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size() - 1)
						{
							gameManager.setRoundType(Round::End);
							gameManager.changeRegionOwner(gameManager.duelWinnerNumeric().first, gameManager.getRegionSelected());
							int indexPlayer = gameManager.searchPlayer(opponents[1]);
							int indexPlayerCastigator = gameManager.searchPlayer(opponents[0]);
							gameManager.addPlayerScore(indexPlayer, -100);
							gameManager.addPlayerScore(indexPlayerCastigator, 100);

						}
						else {
							int indexPlayer = gameManager.searchPlayer(opponents[1]);
							gameManager.addPlayerScore(indexPlayer, -100);
							int indexPlayerCastigator = gameManager.searchPlayer(opponents[0]);
							gameManager.addPlayerScore(indexPlayerCastigator, 100);
							gameManager.changeRegionOwner(gameManager.duelWinnerNumeric().first, gameManager.getRegionSelected());
							gameManager.setRoundType(Round::Duel);
							gameManager.duelRoundsIncrement();
							gameManager.alreadyRequestedClear();
							gameManager.clearOpponents();
							gameManager.clearAnswersNumeric();
						}
						return crow::response(200, "You won the region");
					}
				}
				else {
					if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size() - 1)
					{
						gameManager.setRoundType(Round::End);
						gameManager.clearAnswersNumeric();
						gameManager.searchPlayer(opponents[1]);
						gameManager.addPlayerScore(gameManager.searchPlayer(opponents[1]), 100);
						gameManager.incrementRegionPoints(gameManager.getRegionSelected());
						return crow::response(200, "You lost");
					}
					else
					{
						gameManager.setRoundType(Round::Duel);
						gameManager.clearAnswersNumeric();
						gameManager.searchPlayer(opponents[1]);
						gameManager.addPlayerScore(gameManager.searchPlayer(opponents[1]), 100);
						gameManager.incrementRegionPoints(gameManager.getRegionSelected());
						gameManager.duelRoundsIncrement();
						gameManager.alreadyRequestedClear();
					}
				}
			}

		}
	}
	else if (gameManager.getAnswersNumeric().size() == gameManager.getNumberOfPlayers())
	{
		gameManager.setQuestionNumericCount(gameManager.getQuestionNumericCount() + 1);
		if (gameManager.isBaseStage())
		{
			gameManager.setRoundType(Round::SelectBase);
		}
		else
		{
			std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, compare> a;
			gameManager.createRankingForTeritory(a);
			gameManager.addItemsInPq(a);
			gameManager.setRoundType(Round::SelectRegions);
			gameManager.alreadyRequestedClear();
		}
		return crow::response(200, "All players answered");
	}
	else
	{
		return crow::response(400);
	}
	return crow::response(400);
}
crow::response isYourTurn::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto ordine = gameManager.getAnswersNumeric();
	auto duelRanking = gameManager.getDuelRanking();
	if (gameManager.getRoundType() == Round::Duel)
	{
		if (duelRanking.size() == 0)
		{
			gameManager.setDuelRounds();
			duelRanking = gameManager.getDuelRanking();
		}
		if (duelRanking[gameManager.duelRoundsGet()] == username)
		{
			return crow::response(200);

		}
		else return crow::response(400);
	}
	if (ordine.top().first == username)
	{
		return crow::response(200);
	}
	else
	{
		return crow::response(400);
	}
}

crow::response takeBase::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto index = urlArgs["index"];
	auto regions = gameManager.getRegions();
	auto ordine = gameManager.getAnswersNumeric();
	if (regions[std::stoi(index)].getIsBase() == true)
	{
		return crow::response(200, "Base already owned");
	}
	else
	{
		gameManager.changeRegionOwner(username, std::stoi(index));
		gameManager.setBase(std::stoi(index));
		gameManager.deleteAnswersNumeric();
		gameManager.addRegionsOwned();
		int i = gameManager.searchPlayer(username);
		gameManager.addPlayerScore(i, 300);
		if (gameManager.isBaseStage() == false)
		{
			Sleep(2500);
			gameManager.setRoundType(Round::NumericQuestion);
			gameManager.alreadyRequestedClear();
		}
		return crow::response(200, "Base taken");
	}
}

crow::response takeRegion::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto index = urlArgs["index"];
	auto regions = gameManager.getRegions();
	auto ordine = gameManager.getAnswersNumeric();
	if (regions[std::stoi(index)].getOwner() == username)
	{
		return crow::response(200, "Region already owned");
	}
	else
	{
		gameManager.changeRegionOwner(username, std::stoi(index));
		gameManager.deleteAnswersNumeric();
		gameManager.addRegionsOwned();
		int i = gameManager.searchPlayer(username);
		gameManager.addPlayerScore(i, 100);
		int a = gameManager.getRegionsOwned();
		if (gameManager.getAnswersNumeric().size() == 0 && gameManager.getRegionsOwned() != gameManager.getRegions().size()) {
			gameManager.setRoundType(Round::NumericQuestion);
			gameManager.alreadyRequestedClear();
		}
		if (gameManager.getRegionsOwned() == gameManager.getRegions().size())
		{
			gameManager.setRoundType(Round::Duel);
		}
		return crow::response(200, "Region taken");
	}
}
crow::response clickedForDuel::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto attacker = urlArgs["username"];
	auto index = urlArgs["index"];
	auto defender = urlArgs["username2"];
	if (gameManager.getDuelRanking().size() == 0)
	{
		gameManager.setDuelRounds();
	}
	if (gameManager.getOpponents().size() == 2)
	{
		gameManager.clearOpponents();
	}
	std::vector<std::string> opponents;
	opponents.push_back(attacker);
	opponents.push_back(defender);
	gameManager.setOpponents(opponents);
	gameManager.alreadyRequestedClear();
	gameManager.setRegionSelected(std::stoi(index));
	gameManager.setRoundType(Round::ABCDQuestion);
	return crow::response(200);
}

crow::response getMultipleAnswers::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto answer = urlArgs["answer"];
	auto index = gameManager.getRegionSelected();
	auto regions = gameManager.getRegions();
	auto x = gameManager.getQuestionABCD();
	auto correctAnswer = x[gameManager.getQuestionABCDCount()].getCorrectAnswer();
	bool ok = 0;
	if (answer == correctAnswer)
	{
		ok = 1;
	}
	for (auto c : gameManager.getOpponents())
	{
		if (username == c)
		{
			gameManager.addAnswersABCD(std::make_pair(username, ok));
		}
	}
	if (gameManager.getPlayerCount() == gameManager.getAnswersABCD().size())
	{
		gameManager.setQuestionABCDCount(gameManager.getQuestionABCDCount() + 1);
		if (gameManager.getDuelWinner() == gameManager.getOpponents()[0])
		{
			if (regions[(index)].getPoints() > 100)
			{
				if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size() - 1)
				{
					gameManager.setRoundType(Round::End);
					int indexPlayer = gameManager.searchPlayer(gameManager.getOpponents()[1]);
					gameManager.decrementRegionPoints(gameManager.getRegionSelected());
					gameManager.addPlayerScore(indexPlayer, 100);
				}
				else {
					gameManager.decrementRegionPoints(gameManager.getRegionSelected());
					int indexPlayer = gameManager.searchPlayer(gameManager.getOpponents()[1]);
					gameManager.addPlayerScore(indexPlayer, 100);
					gameManager.setRoundType(Round::Duel);
					gameManager.duelRoundsIncrement();
					gameManager.alreadyRequestedClear();
					gameManager.clearOpponents();
					gameManager.clearABCDanswers();
				}
				return crow::response(200);
			}
			else
			{
				gameManager.changeRegionOwner(gameManager.getOpponents()[0], index);
				//increment score for winner
				int indexPlayer = gameManager.searchPlayer(gameManager.getOpponents()[0]);
				gameManager.addPlayerScore(indexPlayer, 100);
				gameManager.setRoundType(Round::Duel);
				gameManager.clearABCDanswers();
				gameManager.duelRoundsIncrement();
				//	gameManager.setRegions(reg);
				gameManager.clearOpponents();
				gameManager.alreadyRequestedClear();
				if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size()) gameManager.setRoundType(Round::End);
			}
			return crow::response(200, "You won");

		}
		else if (gameManager.getDuelWinner() == "Draw")
		{
			if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size()-1)
			{
				gameManager.clearABCDanswers();
				gameManager.alreadyRequestedClear();
				gameManager.setRoundType(Round::NumericQuestion);

			}
			gameManager.clearABCDanswers();
			gameManager.alreadyRequestedClear();
			gameManager.setRoundType(Round::NumericQuestion);
			gameManager.clearABCDanswers();
			int a = gameManager.getOpponents().size();
			int b = gameManager.duelRoundsGet();
			return crow::response(200, "Draw");
		}
		else if (gameManager.getDuelWinner() == "Lost")
		{
			if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size()-1)
			{
				gameManager.setRoundType(Round::End);
				gameManager.addPlayerScore(gameManager.searchPlayer(gameManager.getOpponents()[1]), 100);
				gameManager.incrementRegionPoints(gameManager.getRegionSelected());
			}
			else {
				gameManager.duelRoundsIncrement();
				gameManager.clearABCDanswers();
				gameManager.alreadyRequestedClear();
				gameManager.addPlayerScore(gameManager.searchPlayer(gameManager.getOpponents()[1]), 100);
				gameManager.incrementRegionPoints(gameManager.getRegionSelected());
				gameManager.alreadyRequestedClear();
				gameManager.setRoundType(Round::Duel);
				return crow::response(200, "You lost");
			}
		}
		else if (gameManager.getDuelWinner() == "Nobody Won")
		{
			if (gameManager.duelRoundsGet() == gameManager.getDuelRanking().size() - 1)
			{
				gameManager.setRoundType(Round::End);
				gameManager.clearABCDanswers();
				gameManager.searchPlayer(gameManager.getOpponents()[1]);
				gameManager.addPlayerScore(gameManager.searchPlayer(gameManager.getOpponents()[1]), 100);
				gameManager.incrementRegionPoints(gameManager.getRegionSelected());
				return crow::response(200, "You lost");
			}
			else
			{
				gameManager.setRoundType(Round::Duel);
				gameManager.clearABCDanswers();
				gameManager.searchPlayer(gameManager.getOpponents()[1]);
				gameManager.addPlayerScore(gameManager.searchPlayer(gameManager.getOpponents()[1]), 100);
				gameManager.incrementRegionPoints(gameManager.getRegionSelected());
				gameManager.duelRoundsIncrement();
				gameManager.alreadyRequestedClear();
			}
		}
		return crow::response(200, "All players answered");
	}
	else
	{
		return crow::response(400);
	}
}

crow::response getWinnerForGame::operator()(const crow::request& req) const
{	auto urlArgs = parseUrlArgs(req.body);
	if (gameManager.getRoundType() == Round::End)
	{
		std::string winner = gameManager.checkGameWinner();
		return crow::response(200, winner);
	}
	else
	{
		return crow::response(400);
	}
}
