

#include "_302901087_C.h"

REGISTER_ALGORITHM(_302901087_C)

list<Direction> _302901087_C::convertFromMapPointsDistancesToDirectionsList(const Point& point, map<Point, int> map)
{
	list<Direction> res;
	Point curr = point;
	int distance = map[curr];
	while (distance > 0)
	{
		for (Direction d : directions)
		{
			Point tmp = curr.neighbour(d);
			if (map.find(tmp)!=map.end() && map[tmp] == distance - 1)
			{
				res.push_back(opposite(d));
				curr = tmp;
				distance--;
				break;
			}
		}
	}
	return res;
}

/*This method uses the bfs method to find the nearest not clean or not visited before points to the current given robot position in the house
by inserting into a queue points around the current robot's position,and the steps distance from the current robot's position in the house
to that points.(in cotradiction to the former _302901087_B class that inserted to the queue the points and the direction steps list from the
current robot's position in the house to them.)
This method also returns the list of direction steps from the current robot's position in the house to the nearest not clean or not visited before
point in the house*/

list <Direction> _302901087_C::findNearestNotCleanOrNotKnownPoint(const Point& point, set <Point>& visitedPoints)
{
	queue<pair<Point, int>> q;
	list <Direction> res;
	pair<Point, int> curr = { {0,0},0 };
	map<Point, int> map;

	visitedPoints.insert(point);
	curr.first = point;
	curr.second = 0;
	q.push(curr);

	while (!q.empty())
	{
		curr = q.front();
		map.insert(curr);
		visitedPoints.insert(curr.first);

		q.pop();
		if (this->houseMapping[curr.first].dirt != 0 && !(curr.first == this->docking))
		{
			res = convertFromMapPointsDistancesToDirectionsList(curr.first,map);
			return res;
		}
		else
		{
			pair<Point, int> tmp = curr;
			for (Direction d : directions)
			{
				Point neighbour = tmp.first.neighbour(d);
				if (this->houseMapping.find(neighbour) != this->houseMapping.end() && !this->houseMapping[neighbour].isWall && visitedPoints.find(neighbour) == visitedPoints.end())
				{
					curr.first = neighbour;
					curr.second=tmp.second+1;
					q.push(curr);
				}
			}
		}
	}
	return res;
}

list <Direction> _302901087_C::findNearestNotCleanOrNotKnownPoint(const Point& point)
{
	set <Point> visitedPoints;
	return this->findNearestNotCleanOrNotKnownPoint(point, visitedPoints);
}

