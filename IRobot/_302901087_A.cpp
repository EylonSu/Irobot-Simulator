#include "_302901087_A.h"
 
//REGISTER_ALGORITHM(_302901087_A)

/*This method finds the approximately nearest not clean or not visited before,points in the house to the current robot's position 
in the house ,in a recursive way,and returns a direction steps list from the current robotws position in the house to the nearest found point.*/

list<Direction> _302901087_A::findNearestNotCleanOrNotKnownPoint(const Point& point, set <Point>& visitedPoints)
{
	list<Direction> res;

	if (!(this->houseMapping[point].isWall) && this->houseMapping[point].dirt != 0 && !(point==this->docking))
	{
		visitedPoints.insert(point);
		return res;
	}
	else
	{
		visitedPoints.insert(point);
		vector <list<Direction>> dirListsVector;
		list<Direction> dirList;
		for (Direction d : directions)
		{
			Point neighbour = point.neighbour(d);
			dirList.clear();
			if (this->houseMapping.find(neighbour) != this->houseMapping.end() && !(this->houseMapping[neighbour].isWall) && visitedPoints.find(neighbour) == visitedPoints.end())
			{
				dirList = this->findNearestNotCleanOrNotKnownPoint(neighbour, visitedPoints);
				dirList.push_back(d);
			}

			if(!dirList.empty())
				dirListsVector.push_back(dirList);
		}
		if (dirListsVector.empty())
		{
			return res;
		}
		else
		{
			res = dirListsVector[0];
			for (int i = 1; i < dirListsVector.size(); i++)
			{
				if (dirListsVector[i].size() < res.size())
					res = dirListsVector[i];
			}
		}
		return res;
	}
}

list <Direction> _302901087_A::findNearestNotCleanOrNotKnownPoint(const Point& point)
{
	set <Point> visitedPoints;
	return this->findNearestNotCleanOrNotKnownPoint(point, visitedPoints);
}


