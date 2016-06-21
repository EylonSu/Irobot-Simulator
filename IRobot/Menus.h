#pragma once

#include "Config.h"
#include <iostream>
#include <iomanip>
#include <cstringt.h>

using namespace std;

#include "SpecialFunctions.h"
#include "resource.h"
#include "Simulator.h"



enum Validation;

enum INTER_CHOISES { RESUME = 1, RESIMULATE, SAVE, SHOW_SOLUTION, ALGO_RUN, BACK_TO_MM = 8, EXIT = 9 };
enum SOLUTION_ESC_MENU_CHOISES { CONTINUE_SOLUTION = 1, START_SOLUTION = 2, CONTINUE_SIMULATE = 3, START_SIMULATE = 4, SOLUTION_BACK_TO_MM = 8, SOLUTION_EXIT = 9 };
enum ALGO_ESC_MENU_CHOISES { CONTINUE_ALGO = 1, START_ALGO = 2, ALGO_CONTINUE_SIMULATE = 3, ALGO_START_SIMULATE = 4, ALGO_BACK_TO_MM = 8, ALGO_EXIT = 9 };

class Menus
{
private:
 	Simulator* simulator;

public:
	Menus() {}
	Menus(Simulator* _simulator) { this->simulator = _simulator; }
	
	void setSimulator(Simulator* _simulator) { this->simulator = _simulator; }
	void displayHeader();
	void DisplayMainMenu();
	void DisplayInsturctions();
	void ExitPage();
	void ExitPage(char* textToShow);
	void Menus::displayFinishPage(Validation houseValidity, bool finished, int iterations, int score);
	void displayFinishPage(map<Validation, bool> houseValidity, bool finished = false, int iterations = 0, int score = 0);
	INTER_CHOISES DisplayIntermediateMenu();
	SOLUTION_ESC_MENU_CHOISES DisplaySolutionEscMenu();
	ALGO_ESC_MENU_CHOISES Menus::DisplayAlgoEscMenu();
	void startFromCertainHouse();
	void Menus::DisplayGenericScreen(const char* text);
};