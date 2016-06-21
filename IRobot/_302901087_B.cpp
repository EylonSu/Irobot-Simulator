#include "_302901087_B.h"

REGISTER_ALGORITHM(_302901087_B)

/*This method uses the bfs method to find the nearest not clean or not visited before points to the current given robot position in the house
by inserting into a queue points around the current robot's position,and a list of direction steps from the current robot's position in the house
to that points.
This method returns the list of direction steps from the current robot's position in the house to the nearest not clean or not visited before
point in the house*/

list <Direction> _302901087_B::findNearestNotCleanOrNotKnownPoint(const Point& point, set <Point>& visitedPoints)
{
	queue<pair<Point, list <Direction>>> q;
	list <Direction> res;
	pair<Point, list <Direction>> curr = { {0,0},res };
	
	visitedPoints.insert(point);
	curr.first = point;
	curr.second = res;
	q.push(curr);
	
	while (!q.empty())
	{
		curr = q.front();
		q.pop();
		visitedPoints.insert(curr.first);
		if (this->houseMapping[curr.first].dirt != 0 && !(curr.first==this->docking))
		{
			res = curr.second;
			return res;
		}
		else
		{
			pair<Point, list <Direction>> tmp = curr;
			for (Direction d : directions)
			{
				Point neighbour = tmp.first.neighbour(d);
				if (this->houseMapping.find(neighbour) != this->houseMapping.end() && !this->houseMapping[neighbour].isWall && visitedPoints.find(neighbour) == visitedPoints.end())
				{
					curr.first = neighbour;
					curr.second = tmp.second;
					curr.second.push_front(d);
					q.push(curr);
				}
			}
		}
	}
	return res;
}

list <Direction> _302901087_B::findNearestNotCleanOrNotKnownPoint(const Point& point)
{
	set <Point> visitedPoints;
	return this->findNearestNotCleanOrNotKnownPoint(point, visitedPoints);
}


