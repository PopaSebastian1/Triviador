#pragma once
#include <memory>
#include <crow.h>
#include "User.h"
#include "utils.h"
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;
class Lobby
{
	int m_id;
	std::string m_owner;
	std::string player1;
	std::string player2;
	std::string player3;
	std::string player4;
	int m_isStarted;
public:
	Lobby(std::string owner = "", std::string player1 = "", std::string player2 = "", std::string player = "", std::string player4 = "", int isStarted = 0);
	~Lobby();
	void setId(int id)
	{
		m_id = id;
	}
	void setOwner(std::string owner)
	{
		m_owner = owner;
	}
	void setPlayer1(std::string player)
	{
		player1 = player;
	}
	void setPlayer2(std::string player)
	{
		player2 = player;
	}
	void setPlayer3(std::string player)
	{
		player3 = player;
	}
	void setPlayer4(std::string player)
	{
		player4 = player;
	}
	void setIsStarted(bool isStarted)
	{
		m_isStarted = isStarted;
	}
	int getId() const { return m_id; }
	std::string getOwner() const { return this->m_owner; }
	std::string getPlayer1() const { return this->player1; }
	std::string getPlayer2() const { return this->player2; }
	std::string getPlayer3() const { return this->player3; }
	std::string getPlayer4() const { return this->player4; }
	void addPlayer(std::string player);
	int isStarted() const { return m_isStarted; }
	friend inline auto createStorageGame(const std::string& filename);
};
inline auto createStorageGame(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table
		(
			"Games_table",
			sql::make_column("id", &Lobby::m_id, sql::primary_key()),
			sql::make_column("owner", &Lobby::m_owner),
			sql::make_column("player1", &Lobby::player1),
			sql::make_column("player2", &Lobby::player2),
			sql::make_column("player3", &Lobby::player3),
			sql::make_column("player4", &Lobby::player4),
			sql::make_column("isStarted", &Lobby::m_isStarted)
		)
	);
}
using GamesStorage = decltype(createStorageGame(""));


class CreateLobbyHandler
{
	GamesStorage& db;
public:
	CreateLobbyHandler(GamesStorage& db);
	crow::response operator() (const crow::request& req) const;
};


class JoinLobbyHandler
{
	GamesStorage& db;
public:
	JoinLobbyHandler(GamesStorage& db);
	crow::response operator() (const crow::request& req) const;
};


class GetPlayersHandler
{
	GamesStorage& db;
public:
	GetPlayersHandler(GamesStorage& db);
	crow::response operator() (const crow::request& req) const;
};

