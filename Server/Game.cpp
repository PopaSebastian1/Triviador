#include "Game.h"
Lobby::Lobby(std::string owner, std::string player1, std::string player2, std::string player3, std::string player4, int isStarted)
{
	m_id = -1;
	m_owner = owner;
	this->player1 = owner;
	this->player2 = player2;
	this->player3 = player3;
	this->player4 = player4;
	m_isStarted = isStarted;
}

Lobby::~Lobby()
{
}

void Lobby::addPlayer(std::string player)
{
	if (player1 == "")
	{
		player1 = player;
	}
	else if (player2 == "*")
	{
		player2 = player;
	}
	else if (player3 == "*")
	{
		player3 = player;
	}
	else if (player4 == "*")
	{
		player4 = player;
	}
}
using GamesStorage = decltype(createStorageGame(""));

CreateLobbyHandler::CreateLobbyHandler(GamesStorage& db) :db(db)
{
}

crow::response CreateLobbyHandler::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	for (auto game : db.iterate<Lobby>())
	{
		if (game.isStarted() == true && username == game.getOwner())
		{
			return crow::response(400);
		}
	}
	Lobby lobby(username, "*", "*", "*", "*", 1);
	db.insert(lobby);
	return crow::response(200);
}

JoinLobbyHandler::JoinLobbyHandler(GamesStorage& db) :db(db)
{
}

crow::response JoinLobbyHandler::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	auto gameOwner = urlArgs["gameOwner"];
	for (auto game : db.iterate<Lobby>())
	{
		if (game.isStarted() == 0)
		{
			return crow::response(401);
		}
		else if (game.getOwner() == gameOwner)
		{

			game.addPlayer(username);
			db.update(game);
			return crow::response(200);
		}
	}
	return crow::response(400);
}

GetPlayersHandler::GetPlayersHandler(GamesStorage& db) :db(db)
{
}

crow::response GetPlayersHandler::operator()(const crow::request& req) const
{
	auto urlArgs = parseUrlArgs(req.body);
	auto username = urlArgs["username"];
	for (auto game : db.iterate<Lobby>())
	{
		if (game.getOwner() == username)
		{
			std::string players = game.getPlayer1() + "," + game.getPlayer2() + "," + game.getPlayer3() + "," + game.getPlayer4();
			return crow::response(200, players);
		}
	}
	return crow::response(401);
}

