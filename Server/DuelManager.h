#pragma once
#include <crow.h>
#include <cpr/cpr.h>
#include "utils.h"
#include "Question.h"
#include "GamePlayer.h"
#include <sqlite_orm/sqlite_orm.h>
#include <memory>
#include "Game.h"

//Gameplay
class DuelManager
{
public:
	//playerOne is ALWAYS the one ATTACKING
	void NormalPlayerDuel(GamePlayer &playerOne, GamePlayer &playerTwo, Region &region);
	void BaseDuel(GamePlayer &playerOne, GamePlayer &playerTwo, Region &base);
};



//Server Handlers
class StartDuelHandler {
public:
	crow::response operator()(const crow::request& req);
};

