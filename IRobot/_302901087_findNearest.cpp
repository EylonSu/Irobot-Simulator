#include "_302901087_findNearest.h"

void _302901087_findNearest::updateStepsToDocking(int stepsToDocking, const Point& position)
{
	if (houseMapping[position].stepsToDocking>stepsToDocking || houseMapping[position].stepsToDocking == -1)
	{
		houseMapping[position].stepsToDocking = stepsToDocking;
		for (Point p : position.neighbours())
		{
			auto neighbour = houseMapping.find(p);
			if (neighbour != houseMapping.end())
			{
				CellInfo cell = neighbour->second;
				if (cell.stepsToDocking > stepsToDocking + 1 || (!cell.isWall && cell.stepsToDocking == -1))
					updateStepsToDocking(stepsToDocking + 1, p);
			}
		}
	}
}

int _302901087_findNearest::calcStepsToDocking(int stepsFromDocking, const Point& position)
{
	auto thisCellInfo = houseMapping.find(position);
	if (thisCellInfo != houseMapping.end() && thisCellInfo->second.stepsToDocking != -1 && thisCellInfo->second.stepsToDocking < stepsFromDocking)
		return thisCellInfo->second.stepsToDocking;
	int bestRouteSteps = stepsFromDocking;
	for (Point p : position.neighbours())
	{
		auto neighbour = houseMapping.find(p);
		if (neighbour != houseMapping.end() && neighbour->second.stepsToDocking != -1 && neighbour->second.stepsToDocking < bestRouteSteps - 1)
			bestRouteSteps = neighbour->second.stepsToDocking + 1;
	}
	if (bestRouteSteps < stepsFromDocking)
		updateStepsToDocking(bestRouteSteps, position);
	return bestRouteSteps;
}

void _302901087_findNearest::updateAlgorithmInfo(Direction lastStep)
{
	totalSteps++;
	if (this->stepsTillFinishing>0)
		this->stepsTillFinishing--;
	position.move(lastStep);
	this->sensorInfo = this->sensor->sense();
	this->stepsFromDocking = this->calcStepsToDocking(this->stepsFromDocking + 1, this->position);
	houseMapping[position] = { (sensorInfo.dirtLevel > 0) ? sensorInfo.dirtLevel - 1 : 0, stepsFromDocking };
	for (Direction d : directions)
	{
		Point p = position.neighbour(d);
		auto neighbour = houseMapping.find(p);
		if (neighbour == houseMapping.end())
		{
			CellInfo& cellInfo = houseMapping[p];
			cellInfo.isWall = sensorInfo.isWall[(int)d];
			if (!sensorInfo.isWall[(int)d])
				cellInfo.stepsToDocking = calcStepsToDocking(stepsFromDocking + 1, p);
		}
	}
}

Direction _302901087_findNearest::findNearestNeighbourToDocking()
{
	vector<Direction>notWallDirectionsVector;
	Direction res;
	if (this->position == this->docking)
		res = Direction::Stay;
	else
	{
		for (Direction d : directions)
		{
			if (!(this->houseMapping[this->position.neighbour(d)].isWall))
				notWallDirectionsVector.push_back(d);
		}
		if (notWallDirectionsVector.empty())
			res = Direction::Stay;
		else
		{
			res = notWallDirectionsVector[0];
			for (int i = 1; i < notWallDirectionsVector.size(); i++)
			{
				if (this->houseMapping[this->position.neighbour(notWallDirectionsVector[i])].stepsToDocking < this->houseMapping[this->position.neighbour(res)].stepsToDocking)
					res = notWallDirectionsVector[i];
			}
		}
	}
	return res;
}

bool _302901087_findNearest::ifStayInDocking()
{
	return this->position == this->docking && (this->battery->getCurrentBatteryLoad() < this->battery->getBatteryCapacity() || (stepsTillFinishing >= 0 && stepsTillFinishing <= MaximalStepsTillFinishingToStayInDocking) || isFinished);
}

bool _302901087_findNearest::ifReturnToDocking()
{
	int consumRate = this->battery->getBatteryConsumptionRate();
	int batLoad = this->battery->getCurrentBatteryLoad();
	bool inDocking = this->position == this->docking,
		lowBatt = consumRate * this->stepsFromDocking >= batLoad*0.85,
		about2finish = (this->stepsFromDocking > 0.8*stepsTillFinishing && stepsTillFinishing >= 0);
	
	return !inDocking && (lowBatt || about2finish || isFinished);
}

Direction _302901087_findNearest::step(Direction lastStep)
{

	/* The main idea:
	continue the same direction if possible (not a wall),
	but if we've alreay been at the next cell in the same direction, (
	go to the closer cell that we havn't been at yet.
	When we visited all cells, return to DS (Finished Cleaning).

	Battery handling:
	On each step update the cell memory on shortest route to DS.
	When battery drains low (or we're reaching a manual closing by the simulator due to other winner),
	go back to recharge on the shortest route.*/

	Direction res;
	this->updateAlgorithmInfo(lastStep);
	if (!(this->position == this->docking))
		this->battery->Consume();
	if (this->ifStayInDocking())
	{
		this->battery->Charge();
		res = Direction::Stay;
	}
	else if (this->sensorInfo.isWall[0] && this->sensorInfo.isWall[1] && this->sensorInfo.isWall[2] && this->sensorInfo.isWall[3])
		res = Direction::Stay;
	else if(this->ifReturnToDocking())
	{
		if (this->houseMapping[this->position].dirt > 0)
			res = Direction::Stay;
		else
			res = this->findNearestNeighbourToDocking();
	}
	else if (lastStep != requestedStep)
	{
		this->currentRoute.clear();
		if (this->houseMapping[this->position].dirt > 0)
			res = Direction::Stay;
		else
		{
			this->currentRoute = findNearestNotCleanOrNotKnownPoint(this->position);
			if (this->currentRoute.empty())
			{
				this->isFinished = true;
				res = this->findNearestNeighbourToDocking();
			}
			else
			{
				res = this->currentRoute.back();
				this->currentRoute.pop_back();
			}
		}
	}

	else if (this->currentRoute.empty())
	{
		if (this->houseMapping[this->position].dirt > 0)
			res = Direction::Stay;
		else
		{
			this->currentRoute = findNearestNotCleanOrNotKnownPoint(this->position);
			if (this->currentRoute.empty())
			{
				this->isFinished = true;
				res = this->findNearestNeighbourToDocking();
			}
			else
			{
				res = this->currentRoute.back();
				this->currentRoute.pop_back();
			}
		}
	}
	else
	{
		if (this->houseMapping[this->position].dirt > 0)
			res = Direction::Stay;
		else
		{
			res = this->currentRoute.back();
			if (this->sensorInfo.isWall[(int)res])
			{
				this->currentRoute.clear();
				this->currentRoute = findNearestNotCleanOrNotKnownPoint(this->position);
			}
			res = this->currentRoute.back();
			this->currentRoute.pop_back();
		}
	}
	this->requestedStep = res;
	return res;
}