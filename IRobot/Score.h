#pragma once

#define FINISHED_UNSUCCESSFULLY_GRADE 10
#include <string>
#include <iostream>
using namespace std;

#include "Competition.h"

//Class Score represents a score of one algorithm.
//The score per each hous is stored in houseScoreData array. each cell is s house.
//Avg score for the algo is the float avg var

struct ScoreData
{
	bool steppedWall = false;
	bool BatteryOut = false;
	bool hasFinished = false;
	int positionInCompetition = -1;
	int actualPosition = -1;
	int winnerNumSteps = -1;
	int simulationSteps = -1;
	int thisNumSteps = -1;
	int cleanedDust = -1;
	int dustAtBegin = -1;
	bool isBackAtDocking = false;
	int score = -1;

	void calculate()
	{
		if (steppedWall) { score = 0; return; }

		if (BatteryOut)thisNumSteps = --simulationSteps;
		if (hasFinished)//finished successfully
			positionInCompetition = min(4, actualPosition);
		else
			positionInCompetition = FINISHED_UNSUCCESSFULLY_GRADE;

		score = max(0,
					2000
					- ((positionInCompetition - 1) * 50)
					+ ((winnerNumSteps - ++thisNumSteps) * 10)
					- ((dustAtBegin - cleanedDust) * 3)
					+ (isBackAtDocking ? 50 : -200));
	}

};

class Score
{
private:
	Competition* competition;
	ScoreData* housesScoreData;
	int houseAmount = -1;
	float avg = 0;
	
public:
	Score();
	~Score();

	//==============get set funcs==================
	void setActualPosition(int _num) { housesScoreData[competition->getHouseIndex()].actualPosition = _num; }
	void setWinnerSteps(int _num) { housesScoreData[competition->getHouseIndex()].winnerNumSteps = _num; }
	void setCompetition(Competition* comp) { competition = comp; }
	void setHouseAmount(int num) { houseAmount = num; }
	void setHousesScoreData(int num) { housesScoreData = new ScoreData[num]; }
	void setNumSteps(int num) { housesScoreData[competition->getHouseIndex()].thisNumSteps = num; }
	void setDustAtbegin(int num) { housesScoreData[competition->getHouseIndex()].dustAtBegin=num; }
	void setCleanedDust(int num) { housesScoreData[competition->getHouseIndex()].cleanedDust = num; }
	void setIsBackAtDocking(bool b) { housesScoreData[competition->getHouseIndex()].isBackAtDocking = b; }
	void setSimulationStep(int num) { housesScoreData[competition->getHouseIndex()].simulationSteps = num; }

	float getAvg() { return avg; }
	ScoreData getScoreData(int i) { return housesScoreData[i]; }

	//=========memeber functions===================
	string algoName;
	void steppedWall() { housesScoreData[competition->getHouseIndex()].steppedWall = true; }
	void BatteryOut() {housesScoreData[competition->getHouseIndex()].BatteryOut = true;}
	void finished(int sc) { housesScoreData[competition->getHouseIndex()].hasFinished = true; housesScoreData[competition->getHouseIndex()].thisNumSteps = sc; }
	void calculateAvg();
	void calcHouseScore();
};