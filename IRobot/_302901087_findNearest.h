/*The findNearestNotCleanOrNotKnownPointAbstarctAlgorithm is a base abstract class for algorithms,which tries each time to find the nearest point
in the house,which is dirty or in which the dust level isn't known,becauese the robot still didn't get to it.
they do it by implementing the virtual pure method virtual list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point) (each algorithm in its own unique way),
,that is a member of this base abstract class.
This abstract class derives the AbstractAlgorithm class,and contains implementation of its pure virtual methods
(like the pure virtual method virtual Direction step(Direction lastStep)),and also contains other data and function members ,all 
,and*/

#pragma once

#include <iostream>
using namespace std;

#include "Point.h"
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include "Sensor.h"
#include "Config.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"



class _302901087_findNearest : public AbstractAlgorithm
{
	enum { MaximalStepsTillFinishingToStayInDocking = 10 };
protected:
	const AbstractSensor* sensor;
	struct CellInfo
	{
		int dirt = -1;
		int stepsToDocking = -1;
		bool isWall;
		CellInfo() {}
		CellInfo(int dirt, int stepsToDock, bool isWall = false) : dirt(dirt), stepsToDocking(stepsToDock), isWall(isWall) {}
	};
	map<Point, CellInfo> houseMapping;
	Direction currDir = Direction::East;
	Point docking = { 0,0 };
	Point position = { 0,0 };
	SensorInformation sensorInfo;
	int stepsFromDocking = -1;
	int totalSteps = 0;
	bool isFinished = false;
	int stepsTillFinishing = -1;
	list <Direction> currentRoute;
	Direction requestedStep;
	Battery* battery;
	Config* config;
	
	void updateAlgorithmInfo(Direction lastStep);
	int calcStepsToDocking(int stepsFromDocking, const Point& position);
	void updateStepsToDocking(int stepsToDocking, const Point& position);
	Direction findNearestNeighbourToDocking();
	bool _302901087_findNearest::ifStayInDocking();
	bool _302901087_findNearest::ifReturnToDocking();
	virtual list <Direction> findNearestNotCleanOrNotKnownPoint(const Point& point)=0;

public:
	_302901087_findNearest() { this->battery = new Battery(); }
	~_302901087_findNearest() { delete this->battery; }
	void setSensor(const AbstractSensor& sensor) { this->sensor = &sensor; }
	void setConfiguration(map<string, int> config)
	{
		this->battery->setBatteryCapacity(config["BatteryCapacity"]);
		this->battery->setBatteryConsumptionRate(config["BatteryConsumptionRate"]);
		this->battery->setBatteryRechargeRate(config["BatteryRechargeRate"]);
		this->battery->setCurrentBatteryLoad(this->battery->getBatteryCapacity());
	}
	virtual Direction step(Direction lastStep);
	void aboutToFinish(int stepsTillFinishing) { this->stepsTillFinishing = stepsTillFinishing; }
};

