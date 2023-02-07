#include "GamePlayer.h"

GamePlayer& GamePlayer::operator=(const GamePlayer& other)
{
	name = other.name;
	active = other.active;
	base = other.base;
	score = other.score;
	id = other.id;

	// trebuie returnat
	GamePlayer gamePlayer(name, score, active, base, id);
	return gamePlayer;
}
