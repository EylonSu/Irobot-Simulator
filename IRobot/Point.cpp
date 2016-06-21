//------------------------------
// Point.cpp
//------------------------------
using namespace std;
#include <iostream>

#include "Point.h"

void Point::move(Direction d) {
	switch(d) {
	case Direction::East:
		return move(1, 0);
	case Direction::West:
		return move(-1, 0);
	case Direction::South:
		return move(0, 1);
	case Direction::North:
		return move(0, -1);
	case Direction::Stay:
		return move(0, 0);
	}
}
