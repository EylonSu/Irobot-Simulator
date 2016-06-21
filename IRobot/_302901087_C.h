/* This is also an algorithm class,that derives  from  the findNearestNotCleanOrNotKnownPoint class,and implements the pure abstract
list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point) method of the findNearestNotCleanOrNotKnownPoint class,
using  the bfs algorithm,but in a little different way. */

#pragma once

#include <utility>
#include "_302901087_findNearest.h"

class _302901087_C : public _302901087_findNearest
{
	list<Direction> convertFromMapPointsDistancesToDirectionsList(const Point& point, map<Point, int> map);
	virtual list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point);
	list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point, set <Point>& visitedPoints);

public:
	_302901087_C() {};
	~_302901087_C() {};
};

