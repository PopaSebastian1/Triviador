#pragma once
#include <string>
#include<vector>
class Region
{
private:
	std::string name;
	int points;
	bool isBase;
	bool isOwned;
	std::vector<Region> neighbours;
	int id;
	std::string owner;

public:
	Region() {
		name = "";
		points = 0;
		isBase = false;
		isOwned = false;
		id = 0;
		owner = "";
	}
	Region(std::string name, int points, bool isBase, bool isOwned) : name(name), points(points), isBase(isBase), isOwned(isOwned) {}
	Region(std::string name, int points, bool isBase, bool isOwned, std::vector<Region> neighbours, std::string owner) : name(name), points(points), isBase(isBase), isOwned(isOwned), neighbours(neighbours), owner(owner) {}

	//Seteri
	void setName(std::string name) { this->name = name; }
	void setPoints(int points) { this->points = points; }
	void setIsBase(bool isBase) { this->isBase = isBase; }
	void setIsOwned(bool isOwned) { this->isOwned = isOwned; }
	void setNeighbours(std::vector<Region> neighbours) { this->neighbours = neighbours; }
	void setOwner(std::string Owner) { this->owner = Owner; }


	//Geteri
	std::string getName() const { return this->name; }
	int getPoints() const { return this->points; }
	bool getIsBase() const { return this->isBase; }
	bool getIsOwned() const { return this->isOwned; }
	std::vector<Region> getNeighbours() const { return this->neighbours; }
	std::string getOwner() const { return this->owner; }

	//Overload Operatori
	bool operator==(const Region& other) const { return this->name == other.name; }
	Region& operator=(const Region& other);




};

