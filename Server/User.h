#pragma once
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <filesystem>
#include <fstream>
#include <string>
#include <memory>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
#include "utils.h"
namespace sql = sqlite_orm;
class User
{
public:
	//User() = default;
	User(std::string username = "", std::string password = "", uint8_t matchesPlayed = 0, uint8_t level = 0);
	~User();
	friend inline auto createStorageUser(const std::string& filename);
	int getId() const { return id; }
	std::string getUsername() const { return m_username; }
	std::string getPassword() const { return m_password; }
	uint8_t getMatchesPlayed() const { return m_matchesPlayed; }
	uint8_t getMatchesWon() const { return m_matchesWon; }
	uint8_t getLevel() const { return m_level; }

private:
	int id;
	std::string m_username;
	std::string m_password;
	uint8_t m_matchesPlayed;
	uint8_t m_matchesWon;
	uint8_t m_level;
};

inline auto createStorageUser(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table
		(
			"Users",
			sql::make_column("id", &User::id, sql::primary_key()),
			sql::make_column("username", &User::m_username),
			sql::make_column("password", &User::m_password),
			sql::make_column("matchesPlayed", &User::m_matchesPlayed),
			sql::make_column("matchesWon", &User::m_matchesWon),
			sql::make_column("level", &User::m_level)
		)
	);
}
using UsersStorage = decltype(createStorageUser(""));
class LoginHandler
{
	UsersStorage& db;
public:
	LoginHandler(UsersStorage& db);
	crow::response operator() (const crow::request& req) const;

};

class AccountHandler
{
	UsersStorage& db;
public:
	AccountHandler(UsersStorage& db);
	crow::json::wvalue operator() (const crow::request& req) const;
};

class RegisterHandler
{
	UsersStorage& db;

public:
	RegisterHandler(UsersStorage& db);
	crow::response operator() (const crow::request& req) const;
};
