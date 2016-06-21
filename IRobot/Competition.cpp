
#include <iomanip>
#include <thread>
#include "Competition.h"
#include "Score.h"

using namespace std;

bool isHouseValid(HouseDescription* house)
{
	if (house->getValid().find(Validation::VALID) != house->getValid().end())
		return true;
	else
		return false;
}

void Competition::startAllAlgos()
{

	Direction* dir = new Direction[algoAmount];
	for (int i = 0; i < algoAmount; i++)dir[i] = Direction::Stay;
	ContinueCodes *simusConRes = new ContinueCodes[algoAmount];

	if (fatalError)return;//if there is no algos
	checkHouses(getHDVector());
	
	hideCursor();
	clear_screen();
	gotoxy(27, 10);
	cout << "patience.... simulating....";
	float sumMaxSteps = 0,
		sumSC = 0;

	for (HouseDescription* house : getHDVector())
		sumMaxSteps += house->getMaxSteps();

	for (HouseDescription* currHouse : Simulator::getHDVector())//per each house, run all algos simultaniously
	{
		string houseName = currHouse->getFileName();
		houseName.erase(houseName.find(".house"), string::npos);
		gotoxy(27, 11);
		cout << "Now simulating house: " << houseName << " out of " << houseAmount << endl;

		if (!isHouseValid(currHouse)) continue;
		resetSimulators(currHouse);
		houseIndex++;
		stepCount = 0;
		gotWinner = false;
		int stepsAfterWinner = 0;
		bool dummy = true;

		while (continueCheck(stepsAfterWinner, stepCount, currHouse->getMaxSteps()))
		{
			sumSC++;
			gotoxy(38, 12);
			cout << "             \n                  ";
			gotoxy(38, 12);
			cout << setprecision(3)<<(sumSC / sumMaxSteps) * 100 << "%";
			gotoxy(27, 13);
			cout << "Step Number: " << stepCount;


			if (gotWinner || stepCount == currHouse->getMaxSteps() - maxStepsAfterWinner)
			{
				if (dummy) { alertAlgosOfWinner(); dummy = false; }
				if (gotWinner) stepsAfterWinner++;
			}
			for (int i = 0; i < algoAmount; i++)
			{
				gotoxy(27, 14);
				cout << "             ";
				gotoxy(27, 14);
				cout << "Algorithm: " << algorithmNames[i]<<" out of "<<algoAmount;
				if (!(simulators[i].getIsDeactivated()))//if he didnt steped over a wall || won || battery is charged
				{
					dir[i] = algorithms[i]->step(dir[i]);
					simulators[i].makeStep(NO_SPECIFIC_ALGO, dir[i]);
					if (simulators[i].getIsDeactivated())//meaning steped over a wall
					{
						scores[i].steppedWall();
						log(STEPED_WALL, algorithmNames[i], houseName,stepCount);
					}
					simusConRes[i] = simulators[i].continueSimulate();
					switch (simusConRes[i])
					{
					case (ContinueCodes::OUT_OF_BATTERY) :
						simulators[i].setIsDeactivated(true);
						scores[i].BatteryOut();
						log(BETTERY_RAN_OUT ,algorithmNames[i],houseName);
						break;
					case (ContinueCodes::FINISHED) :
						simulators[i].setIsDeactivated(true);
						scores[i].finished(stepCount);
						gotWinner = true;
						won(wonOrder[i]);
						break;
					}
					scores[i].setNumSteps(stepCount);
				}
			}
			stepCount++;
			
		}
		calculateHouseScores();
	}
	delete[] dir;
	delete[] simusConRes;
	calculateAvg();
	showScoreTable();
}

void Competition::log(Validation code, string algoName, string houseName, int sc)
{
	switch (code)
	{
	case BETTERY_RAN_OUT:
		errorLog.push_back("Algorithm " + algoName + " went out of battery while running on house " + houseName);
		break;
	case STEPED_WALL:
		errorLog.push_back("Algorithm " + algoName + " went into a wall while running on house " + houseName + "\n\tin step " + to_string(sc));
	case INVALID_DIMENTIONS:
		errorLog.push_back("House " + houseName + " has invalid dimentions.");
		break;
	case MULTI_DOCKING:
		errorLog.push_back("House " + houseName + " has more than one docking station.");
		break;
	case NO_DOCKING:
		errorLog.push_back("House " + houseName + " has no docking station");
		break;
	case PERIPH_ADD:
		errorLog.push_back("house " + houseName + " didn't had walls all over the peripherials.\n\t The program added it automatically. U welcome!");
		break;

	}
}

void Competition::log(string str)
{
	errorLog.push_back(str);
}

Competition::Competition()
{
	this->setSimStat(COMPET);
	this->menus.setSimulator(this);
	AlgorithmRegistrar& registrar = initRegistrar();
	if (algoAmount == 0) fatalError = true;
	this->algorithms = registrar.getAlgorithms();
	this->algorithmNames = registrar.getAlgorithmNames();
	initSimulators();
	initAlgos(algorithms);
	houseAmount = getHouseDescriprionsVector().size();
	initScores();
	wonOrder = new algosActual_aux[algoAmount];
}

Competition::~Competition()
{
	delete[] wonOrder;
	delete[] scores;
	delete[] sensors;
	delete[] batterys;
	delete[] simulators;

}

void Competition::initScores()
{
	scores = new Score[algoAmount];
	for (int i = 0; i < algoAmount; i++)
	{
		scores[i].algoName = algorithmNames[i];
		scores[i].setCompetition(this);
		scores[i].setHousesScoreData(houseAmount);
		scores[i].setHouseAmount(houseAmount);
	}
}

void Competition::resetSimulators(HouseDescription* currHouse)
{
	delete[] batterys;
	batterys = new Battery[this->algoAmount];
	for (int i = 0; i < algoAmount; i++)
	{
		batterys[i].setSimulator(simulators + i);
		batterys[i].Charge();

		simulators[i].setCurrentPositionOnScreen(Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW));

		simulators[i].setKBsensorBattery(batterys + i, sensors[i]);
		simulators[i].setCurrentHouseDescription(currHouse);
		simulators[i].setSimStat(SimStatCodes::COMPET);
		simulators[i].setIsDeactivated(false);
		simulators[i].setStepCount(0);
		simulators[i].setDustAtbegin(currHouse->getDustAtBegining());
		simulators[i].setCleanedDust(0);
	}
}

void Competition::initSimulators()
{
	simulators = new Simulator[this->algoAmount];
	batterys = new Battery[this->algoAmount];
	sensors = new Sensor[algoAmount];
	for (int i = 0; i < algoAmount; i++)
	{
		sensors[i].setSimulator(simulators + i);
		batterys[i].setSimulator(simulators + i);
		batterys[i].Charge();
		simulators[i].setKBsensorBattery(batterys + i, sensors[i]);
	}
}

AlgorithmRegistrar& Competition::initRegistrar()
{
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();
	algoAmount = registrar.size();
	if (algoAmount == 0)
	{
		menus.DisplayGenericScreen("No Algorithms exists.\n\nPress any key to continue.\n\n");
		WaitForKB();
	}
	return registrar;
}

void Competition::initAlgos(std::vector<std::unique_ptr<AbstractAlgorithm>>& algorithms)
{
	int i = 0;
	for (auto& algo : algorithms)
	{
		algo->setConfiguration(this->config.getConfigMap());
		algo->setSensor(sensors[i++]);
	}
}

bool Competition::continueCheck(int stepsAfterWinner,int stepCount,int ms)
{
	if (stepsAfterWinner == maxStepsAfterWinner || stepCount == ms)
		return false;
	
	for (int i = 0; i < algoAmount; i++)
		if (!simulators[i].getIsDeactivated())
			return true;
	return false;
}

void Competition::alertAlgosOfWinner()
{
	for (int i = 0; i < algoAmount; i++)
		algorithms[i]->aboutToFinish(maxStepsAfterWinner);
}

void Competition::won(algosActual_aux& strct)
{
	static int currentWon = 0;
	strct.actualPosition = currentWon;
	strct.sc = stepCount;
	currentWon++;
}

void Competition::setActualPositionsAtScores()
{
	int numOfPos = algoAmount;
	for (int i = 0; i < numOfPos-1; i++)
	{
		if (gotWinner)
			if (wonOrder[i].sc == wonOrder[i + 1].sc)
				wonOrder[i + 1].actualPosition = wonOrder[i].actualPosition;
			else
				wonOrder[i + 1].actualPosition = FINISHED_UNSUCCESSFULLY_GRADE;
	}
	for (int i = 0; i < numOfPos - 1; i++)
		scores[i].setActualPosition(wonOrder[i].actualPosition);

}

void Competition::setWinnerSteps()
{
	int steps;
	if (gotWinner)
		steps = wonOrder[0].sc;
	else
		steps = stepCount;
	for (int i = 0; i < algoAmount; i++)
	{
		scores[i].setWinnerSteps(steps);
	}
}

void Competition::setSimulationStep()
{
	for (int i = 0; i < algoAmount; i++)
	{
		scores[i].setSimulationStep(stepCount);
	}
}

void Competition::calculateHouseScores()
{
	setActualPositionsAtScores();
	setWinnerSteps();
	setSimulationStep();

	for (int i = 0; i < algoAmount; i++)
	{
		scores[i].setIsBackAtDocking(simulators[i].isRobotInDockingStation());
		scores[i].setCleanedDust(simulators[i].getCleanedDust());
		scores[i].setDustAtbegin(simulators[i].getDustAtbegin());
		scores[i].calcHouseScore();
	}
}

void Competition::calculateAvg()
{
	for (int i = 0; i < algoAmount; i++)
	{
		scores[i].calculateAvg();
	}
}

void Competition::showScoreTable()
{
	menus.displayHeader();
	string scoreTableText = "\n\nAnd the Winner is.....";
	setfill(" ");
	cout <<left<< setw(40 - scoreTableText.size()) << scoreTableText << "\n\n";
	printFirstRow();
	for (int i = 0; i < algoAmount; i++)
		printAlgoScores(scores[i]);
	printErrors();
	WaitForKB();

}

void Competition::printFirstRow()
{
	PRINT_SEP;
	cout << left << setw(ALGO_NAME_WIDTH) << "|";
	for (HouseDescription* house : getHDVector())
	{
		if (!isHouseValid(house)) continue;
		string name = house->getFileName();
		name.erase(name.find(".house"), string::npos);
		cout << "| " << left << setw(COL_WIDTH) << name;
	}
	cout << "| " << left << setw(COL_WIDTH-1)  << "AVG" << " |";
	cout << endl;
	PRINT_SEP;
}

void Competition::printAlgoScores(Score& score)
{
	int i = -1;
	string name = score.algoName;
	name.resize(ALGO_NAME_WIDTH -1);
	cout  << "|" << left << setw(ALGO_NAME_WIDTH-1) << name << "|";
	for (HouseDescription* house : getHDVector())
	{
		i++;
		if (!isHouseValid(house)) continue;
		cout << left << setw(COL_WIDTH+1) << score.getScoreData(i).score << "|";
	}
	cout << left << setw(COL_WIDTH+1) << (int)score.getAvg() << "|\n";
	PRINT_SEP;
}

void Competition::checkHouses(vector<HouseDescription*>& hdv)
{
	map<Validation, bool> val;
	for (auto currHouse : hdv)
	{
		string houseName = currHouse->getFileName();
		houseName.erase(houseName.find(".house"), string::npos);

		val = currHouse->getValid();


		if (val.find(Validation::INVALID_DIMENTIONS) != val.end())
			log(INVALID_DIMENTIONS, "", houseName);
		if (val.find(Validation::MULTI_DOCKING) != val.end())
			log(MULTI_DOCKING, "", houseName);
		if (val.find(Validation::NO_DOCKING) != val.end())
			log(NO_DOCKING, "", houseName);
		if (val.find(Validation::PERIPH_ADD) != val.end())
			log(PERIPH_ADD, "", houseName);
		if (val.find(Validation::MAXSTEPS) != val.end())
			log((string)"Line number 2 in " + currHouse->getFileName() + " shall be a positive number.");

	}
}

void Competition::printErrors()
{
	int i = 0;
	cout << "\n\nErrors:\n\n";
	for (string er : errorLog)
	{
		cout << ++i << ": " << er << endl;
	}
}