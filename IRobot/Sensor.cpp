#include "Sensor.h"
#include "Simulator.h"
using namespace std;
SensorInformation Sensor::sense() const
{
	SensorInformation currentSensorInformation;
 	char ch = this->simulator->getContentOfCurrentPositionOnScreen();
 	switch (ch)
 	{
	case SpecialPointsInHouse::ZeroDirtLevel:
 		currentSensorInformation.dirtLevel = SpecialDirtLevels::ZERO_DIRT_LEVEL;
 		break;
 	case SpecialPointsInHouse::Docking:
 		currentSensorInformation.dirtLevel = SpecialDirtLevels::DOCKING;
 		break;
 	case SpecialPointsInHouse::Wall:
		currentSensorInformation.dirtLevel = SpecialDirtLevels::WALL;
		break;
 	default:
		currentSensorInformation.dirtLevel = 0;
		break;
	}

	if (ch <= '9'&&ch >= '0')
		currentSensorInformation.dirtLevel = ch - '0';
	Point realPosiotionInHouseOfCurrentPositionOnScreen=this->simulator->getRealPositionInHouse();
	currentSensorInformation.isWall[(int)Direction::North] = this->simulator->getContentOfRealPositionInHouse(Point(realPosiotionInHouseOfCurrentPositionOnScreen.getX(), realPosiotionInHouseOfCurrentPositionOnScreen.getY() - 1))== SpecialPointsInHouse::Wall;
	currentSensorInformation.isWall[(int)Direction::East ] = this->simulator->getContentOfRealPositionInHouse(Point(realPosiotionInHouseOfCurrentPositionOnScreen.getX() + 1, realPosiotionInHouseOfCurrentPositionOnScreen.getY())) == SpecialPointsInHouse::Wall;
	currentSensorInformation.isWall[(int)Direction::South] = this->simulator->getContentOfRealPositionInHouse(Point(realPosiotionInHouseOfCurrentPositionOnScreen.getX(), realPosiotionInHouseOfCurrentPositionOnScreen.getY() + 1)) == SpecialPointsInHouse::Wall;
	currentSensorInformation.isWall[(int)Direction::West] = this->simulator->getContentOfRealPositionInHouse(Point(realPosiotionInHouseOfCurrentPositionOnScreen.getX()-1, realPosiotionInHouseOfCurrentPositionOnScreen.getY())) == SpecialPointsInHouse::Wall;
	return currentSensorInformation;
}