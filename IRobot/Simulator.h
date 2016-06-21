#pragma once

#include "Config.h"

#ifdef WINDOWS

#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>

#endif

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <list>
#include <vector>
#include <map>

using namespace std;

#include "Direction.h"
#include "DirectionExt.h"
#include "Simulator_aux_funcs.h"
#include "Sensor.h"
#include "Config.h"
#include "KeyboardRobot.h"
#include "HouseDescription.h"
#include "SpecialFunctions.h"
#include "Point.h"
#include "Defs.h"
#include "Battery.h"
#include "Menus.h"
#include "resource.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"


#define BREAK_CODE 1
#define SPACEBAR_ASCII 32
#define BASE 10
#define NO_SPECIFIC_ALGO -1


//------------------------------forward decleration---------------------------------
	struct SensorInformation;
	class KeyboardRobot;
	class HouseDescription;
	class Sensor;
	class Menus;
	class Config;
	
	class Simulator
	{
	private:
		vector<HouseDescription*> houseDescriptionsVector;
		KeyboardRobot* keyboardRobot;
		Config* config;
		Menus* menus;
		HouseDescription* currentHouseDescription;
		AbstractAlgorithm* algo;
		
	
		Point DockingStationPositionOnScreen = Point(DockingStationPositionOnScreen::COL,DockingStationPositionOnScreen::ROW);
		Point currentPositionOnScreen= DockingStationPositionOnScreen;
		int cleanedDust;
		int DustAtBegining;
		int stepCount = 0;
		bool isDeactivated = false;//indicates wether the robot stepped over walls or not.
		//Point RealDockingStation;
		char console[ConsoleScreenDimensions::ROWS][ConsoleScreenDimensions::COLS]={ ' ' };
		bool begin = true;//indicates the begining of the game- before any direction was set
		bool dummyCheck = true;
		SimStatCodes simStat = SIM;
		vector<string> actionVector;
		bool resumeFlag = false;


		//-------------------------------private functions------------------------------
		vector<string> getFilesNamesVector();
		vector<string> getFilesNamesVector(const string& filesNamesFile);
		Direction(*nextStepFuncPtr)();
		void convertFileToVector(string fileName, vector<string>& actionVector);
		int Simulator::chooseHouseNumber();
		string Simulator::chooseSavedHouseFile(int chosenHouseNumber);
		bool handleSolutionEscMenu();
		void setLastPressedChar(char ch) { this->keyboardRobot->setLastPressedChar(ch); }
		Direction getSolutionDir();
		bool handleInterMenu();
		void printIfWall(Point p);
		void printWalls();
		void beginSingleAlgo();
		bool handleAlgoEscMenu();

	public:
		Simulator();
		~Simulator();
		
		//---------------------get set funcs---------------------
		vector<string> getSavedHousesFileNames();
		vector<string> getSavedHousesFileNames(string houseDescriptionFileName);
		void setCurrentHouseDescription(HouseDescription* houseDescription) { this->currentHouseDescription = houseDescription; }
		HouseDescription* getCurrentHouseDescription() const { return this->currentHouseDescription; }
		void setHouseDescriprionsVector(const vector<HouseDescription*>& houseDescriptionsVector) { this->houseDescriptionsVector = houseDescriptionsVector; }
		vector<HouseDescription*> getHouseDescriprionsVector() const{ return this->houseDescriptionsVector; }
		int getIndex(HouseDescription* houseDescription);
		int getConfig(CONFIG_RQST rqst);
		void loadSavedSim();
		string getCurrentHouseDescriptionFileName() { return this->currentHouseDescription->getFileName(); }
		Point getOffset() { return this->currentHouseDescription->getOffset(); /*return this->DockingStationPositionOnScreen.distance(this->currentHouseDescription->getDockingStationPosition());*/ }
		Point getCurrentPositionOnScreen() { return this->currentPositionOnScreen; }
		Point getRealPositionInHouse(Point pointOnScreen);//for the sensor to retrieve info about this spot TDL merge into overload
		Point getRealPositionInHouse() { return this->getRealPositionInHouse(this->currentPositionOnScreen); }
		char getContentOfCurrentPositionOnScreen() { return (this->currentHouseDescription)->getContentOfPoint(this->getRealPositionInHouse(this->currentPositionOnScreen)); }//gets the dirtyLevel of current position TDL over load to follow function
		char getContentOfPositionOnScreen(Point positionOnScreen) { return (this->currentHouseDescription)->getContentOfPoint(this->getRealPositionInHouse(positionOnScreen)); }
		char getContentOfRealPositionInHouse(Point realPositionInHouse) { return this->currentHouseDescription->getContentOfPoint(realPositionInHouse); }
		void setCurrentPositionOnScreen(Point p) { this->currentPositionOnScreen = p; }
		void setKeyboardLastPressedChar(char ch) { this->keyboardRobot->setLastPressedChar(ch); }
		void setContentOfPositionOnScreen(Point positionOnScreen, char ch){Point realPositionInHouse = this->getRealPositionInHouse(positionOnScreen);this->currentHouseDescription->setContentOfPoint(realPositionInHouse, ch); }
		void setContentOfCurrentPositionOnScreen(char ch){Point realPositionInHouseOfcurrentPositionOnScreen = this->getRealPositionInHouse();this->currentHouseDescription->setContentOfPoint(realPositionInHouseOfcurrentPositionOnScreen, ch);}
		bool getIsDeactivated() { return isDeactivated; }
		vector<HouseDescription*> getHDVector() { return this->houseDescriptionsVector; }
		void setIsDeactivated(bool b) { this->isDeactivated = b; }
		void setSimStat(SimStatCodes stat) { simStat = stat; }
		void setKBsensorBattery(Battery* b, Sensor& sensorInfo) { this->keyboardRobot->setBattery(b); this->keyboardRobot->setSensor(sensorInfo); }
		void setStepCount(int num) { stepCount = num; }
		int getDustAtbegin() { return DustAtBegining; }
		int getCleanedDust() { return cleanedDust; }
		void setCleanedDust(int num) { cleanedDust = num; }
		void setDustAtbegin(int num) { this->DustAtBegining = num; }

		//-----------------------member functions-----------------------
		void toggleSound() { this->config->toggleSounds(); };
		void shutUp() { PlaySound(NULL, NULL, NULL); }
		void play(const char* sound);

		bool isRobotInDockingStation() { return this->currentPositionOnScreen == this->DockingStationPositionOnScreen; }
		bool isStepValid(Direction direction, const SensorInformation& sensorInformation) { return (direction == Direction::North && !(sensorInformation.isWall[(int)Direction::North])) || (direction == Direction::East && !(sensorInformation.isWall[(int)Direction::East])) || (direction == Direction::South && !(sensorInformation.isWall[(int)Direction::South])) || (direction == Direction::West && !(sensorInformation.isWall[(int)Direction::West]) || (direction == Direction::Stay)); }
		void drawRobot();//draw @ 
		void printContentOfCurrentPointOnScreen();
		void clean();
		void move(Direction direction);
		void Start(int index, string fileNameToLoad = "", int algoNum=-1);
		void DisplayBottomLine();
		void printConsole();
		void clearConsole();
		int calculateScore();
		void beginSimulate();
		SaveStat saveSolution();//save solution
		SaveStat saveSimulation(SimStatCodes simStat = SIM);//save simulation
		void updateVector(Direction direction);
		string Simulator::getSavedFileName();
		void showSolution();
		string getSoultionFileName();
		Direction getKBdirection() { return this->keyboardRobot->getNextStepDirection(); }
		void save_aux(string fileName);
		void load_aux(string chosenSavedFileName, int chosenHouseNumber);
		Direction getNextStepDirection(vector<string> actionsVector);
		bool isBetterSol(const string& name, int steps);
		void initHouseDescriptionsVector();
		int makeStep(int algoNum = -1, Direction dir=Direction::Stay);
		ContinueCodes continueSimulate();		
	};
