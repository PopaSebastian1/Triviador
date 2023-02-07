#pragma once
#include "Region.h"
#include <vector>
class GamePlayer
{
	std::string name;
	int score;
	bool active;
	Region base;
	int id;
	//std::vector<std::shared_ptr<Region>> regions;
public:
	GamePlayer() = default;
	GamePlayer(std::string name, int score, bool active, Region base, int id) : name(name), score(score), active(active), base(base), id(id) {}
	GamePlayer(std::string name, int score, bool active, Region base) : name(name), score(score), active(active), base(base) {}
	GamePlayer(std::string name, int score, bool active) : name(name), score(score), active(active) {}
	GamePlayer(std::string name, int score) : name(name), score(score) {}
	GamePlayer(std::string name) : name(name) {}
	GamePlayer(int id) : id(id) {}
	GamePlayer(int id, std::string name) : id(id), name(name) {}
	GamePlayer(int id, std::string name, int score) : id(id), name(name), score(score) {}
	GamePlayer(int id, std::string name, int score, bool active) : id(id), name(name), score(score), active(active) {}
	GamePlayer(int id, std::string name, int score, bool active, Region base) : id(id), name(name), score(score), active(active), base(base) {}

	//Seteri
	void setName(std::string name) { this->name = name; }
	void setScore(int score) { this->score = score; }
	void setActive(bool active) { this->active = active; }
	void setBase(Region base) { this->base = base; }
	void setId(int id) { this->id = id; }
	//void addRegion(std::shared_ptr<Region> region) { regions.push_back(region); }

	//Geteri
	std::string getName() const { return this->name; }
	int getScore() const { return this->score; }
	bool getActive() const { return this->active; }
	Region getBase() const { return this->base; }
	int getId() const { return this->id; }

	//Overload Operatori
	bool operator==(const GamePlayer& other) const { return this->id == other.id; }
	GamePlayer& operator=(const GamePlayer& other);

};

