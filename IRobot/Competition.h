#pragma once

#include <vector>
#include <iostream>

using namespace std;

#include "Direction.h"
#include "DirectionExt.h"
#include "AlgorithmRegistrar.h"
#include "Sensor.h"
#include "Battery.h"
#include "HouseDescription.h"
#include "Defs.h"
#include "Menus.h"
#include "Simulator.h"


class Score;

#define PRINT_SEP cout<<"_________________________________________________________________________\n"		 
#define COL_WIDTH 5
#define ALGO_NAME_WIDTH 15

struct algosActual_aux
{
public:
	int actualPosition;
	int sc;
};

class Competition : public Simulator
{
	vector<unique_ptr<AbstractAlgorithm>> algorithms;
	vector<string> algorithmNames;
	Menus menus;
	Simulator* simulators;
	Config config;
	Sensor* sensors;
	Battery* batterys;
	Score* scores;
	vector<string> errorLog;
	algosActual_aux *wonOrder;

	unsigned int algoAmount = 0;
	bool fatalError = false;
	int maxStepsAfterWinner = config.getMaxStepsAfterWinner();
	int houseIndex = -1;
	int stepCount = 0;
	bool gotWinner = false;
	unsigned int houseAmount = 0;

public:
	Competition();
	~Competition();

	//=================get set funcs==================
	int getDustAtBegin() { return simulators[houseIndex].getDustAtbegin(); }
	int getCleanedDust() { return simulators[houseIndex].getCleanedDust(); }
	void setActualPositionsAtScores();
	void setWinnerSteps();
	int getHouseIndex() { return this->houseIndex; }
	void setSimulationStep();

	//=================member functions=============
	void initSimulators();
	AlgorithmRegistrar& initRegistrar();
	void startAllAlgos();
	void alertAlgosOfWinner();
	void won(algosActual_aux& strct);
	void initAlgos(std::vector<std::unique_ptr<AbstractAlgorithm>>& algorithms);
	void initScores();
	bool continueCheck(int stepsAfterWinner, int stepCount, int ms);
	void calculateHouseScores();
	void showScoreTable();
	void calculateAvg();
	void resetSimulators(HouseDescription* currHouse);
	void printFirstRow();
	void printAlgoScores(Score& score);
	void log(Validation code, string algoName, string houseName, int sc=-1);
	void log(string str);
	void printErrors();
	void checkHouses(vector<HouseDescription*>& hdv);

};