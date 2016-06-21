#ifndef _DEFS_H_
#define _DEFS_H_

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

#include "Point.h"


//sounds def

#define FINISH_FAILED 107//IDR_WAVE1
#define FINISH_CLEANING_SOUND 108//IDR_WAVE2
#define WALK 109//IDR_WAVE3
#define FINISH_SUCCSESS 110//IDR_WAVE4
#define WELCOME_HEY_MAMA 111//IDR_WAVE5
#define WEREON 112//IDR_WAVE6
#define EAT 113//IDR_WAVE7
#define EXIT_SOUND 111//IDR_WAVE5
#define MARIO_SOUND 116//IDR_WAVE9


	enum SaveStat { SUCCESS, BETTER_SOL };

	enum Validation { NULL_HOUSE, INVALID_DIMENTIONS, NO_DOCKING, MULTI_DOCKING, VALID, MAXSTEPS, BETTERY_RAN_OUT, INVALID_STEP, PERIPH_ADD, STEPED_WALL};

	enum ContinueCodes { CONTINUE, FINISHED, OUT_OF_BATTERY, MAX_STEPS };

	enum SimStatCodes { SIM, LOAD, SOLUTION, ALGO, COMPET };

	//enum errorCodes { ER_BATTERY_OUT, ER_STEPED_WALL, ER_INVALID_DIMENTIONS, ER_MULTI_DOCKING, ER_NO_DOCKING, ER_PERIPH_ADD };

	enum { SLEEP_TIME = 30 };

	enum SpecialKeys{UP_KEY1='w',UP_KEY2='W',RIGHT_KEY1='d',RIGHT_KEY2='D',DOWN_KEY1='x',DOWN_KEY2='X',LEFT_KEY1='a',LEFT_KEY2='A',STAY_KEY1='s',STAY_KEY2='S',ESC_KEY=27};

	enum ConsoleScreenDimensions{ROWS=24,COLS=80};

	enum DockingStationPositionOnScreen{ROW=12,COL=40};

	enum DefaultHouseDimensions { Rows = 19, Cols = 79 };

	enum SpecialPointsInHouse {Wall='W',Docking='D',ZeroDirtLevel=' '};

	enum SpecialDirtLevels {WALL=-2,DOCKING=-1,ZERO_DIRT_LEVEL=0};

	enum houseDimentionsLimits{MinRows=5,MaxRows=20,MinCols=10,MaxCols=79};
	
	enum{NOT_FOUND=-1};
	
	enum Color 
	{
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		CYAN = 3,
		RED = 4,
		MAGENTA = 5,
		BROWN = 6,
		LIGHTGREY = 7,
		DARKGREY = 8,
		LIGHTBLUE = 9,
		LIGHTGREEN = 10,
		LIGHTCYAN = 11,
		LIGHTRED = 12,
		LIGHTMAGENTA = 13,
		YELLOW = 14,
		WHITE = 15
	};


#define ASCII_VALUE_OF_ZERO_DIGIT '0'

#define NO_POINT_FOUND Point(-1,-1)

	const char DefaultHouse[DefaultHouseDimensions::Rows][DefaultHouseDimensions::Cols + 1] =
	{   "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W  1    D      9                                                              W", // 1
		"W          WWWWWWW                                W                           W", // 2
		"W             W                                    W                          W", // 3
		"W             W                                   W                           W", // 4
		"W             W           WWWWWWWWWWWWWWWWWWWWWWWWW                           W", // 5
		"W             W                                                               W", // 6
		"W             W                                                               W", // 7
		"W                          WWWWWWWWWWWWWWWWWWWWWW  WWWWWWW                    W", // 8
		"W                                                  W     W                    W", // 9
		"W              W                                   W     W                    W", // 10
		"W              W                                   W     W                    W", // 11
		"W              W                                   W     W                    W", // 12
		"W              W                                   WW   WW                    W", // 13
		"W              W                                    W   W                     W", // 14
		"W              W                                                              W", // 15
		"W              W                                                              W", // 16
		"W              W                                                              W", // 17
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" };// 18

enum BottomLeftCornerOfScreenCoordinates{Col = 0, Row = 25};

#define HouseDescriptionFileNameLength  strlen("000.House") 

#define BatteryConfigurationFile string("BatteryConfigurationFile.ini")

#endif