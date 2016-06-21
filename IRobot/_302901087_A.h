/* This is an algorithm class,that derives  from  the findNearestNotCleanOrNotKnownPoint class,and implements the pure abstract
list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point) method of the findNearestNotCleanOrNotKnownPoint class in a recursive
way. */

#pragma once

#include <utility>
#include "_302901087_findNearest.h"

class _302901087_A : public _302901087_findNearest
{
	virtual list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point);
	list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point,set <Point>& visitedPoints);
	
public:
	_302901087_A() {};
	~_302901087_A() {};
};

