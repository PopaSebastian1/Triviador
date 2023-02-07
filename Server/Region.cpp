#include "Region.h"

Region& Region::operator=(const Region& other)
{

	name = other.name;
	isBase = other.isBase;
	points = other.points;
	isOwned = other.isOwned;
	neighbours = other.neighbours;
	return *this;
}
