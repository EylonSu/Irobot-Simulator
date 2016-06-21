/* This is also an algorithm class,that derives  from  the findNearestNotCleanOrNotKnownPoint class,and implements the pure abstract
list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point) method of the findNearestNotCleanOrNotKnownPoint class,but
this time it implements this function using  the bfs algorithm. */

#pragma once

#include <utility>
#include "_302901087_findNearest.h"

class _302901087_B : public _302901087_findNearest
{
	virtual list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point);
	list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point, set <Point>& visitedPoints);

public:
	_302901087_B() {};
	~_302901087_B() {};

};
