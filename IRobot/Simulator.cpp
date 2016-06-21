#include "Simulator.h"
using namespace std;
#define BASE 10

#ifndef WINDOWS

	void MessageBox(void* a, void* b, void* c, void* d) {};

#endif

Simulator::Simulator()
{
 	initHouseDescriptionsVector();
 	this->keyboardRobot = new KeyboardRobot;
 	this->config = new Config;
 	this->menus = new Menus;

 	menus->setSimulator(this);
}

void Simulator::beginSimulate()
{
	if (this->getConfig(RQST_SOUNDS))
		PlaySound(MAKEINTRESOURCE(WELCOME_HEY_MAMA), NULL, SND_ASYNC | SND_RESOURCE);

	this->menus->DisplayMainMenu();
}

bool Simulator::handleInterMenu()
{
 	while (true)
 	{
 		INTER_CHOISES choise = this->menus->DisplayIntermediateMenu();
 		switch (choise)
		{
			case INTER_CHOISES::RESUME:
				if (this->simStat == SimStatCodes::SIM || this->simStat == SimStatCodes::LOAD)
				{
					clear_screen();
					this->printConsole();
					this->drawRobot();
					this->resumeFlag = true;
					return false;
				}
				else if (this->simStat == SimStatCodes::SOLUTION|| this->simStat == SimStatCodes::ALGO)
				{
					this->simStat = SimStatCodes::LOAD;
					this->load_aux(string("tmp.house_saved"), this->getIndex(this->currentHouseDescription)+1);
					system("del tmp.house_saved"); 
					return false;
				}
				break;
			case INTER_CHOISES::BACK_TO_MM:
				this->currentHouseDescription->changeAccordingToFile();
		 		this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
				this->menus->DisplayMainMenu();
				return true;
				break;	
			case INTER_CHOISES::RESIMULATE:
				this->currentHouseDescription->changeAccordingToFile();
				this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
				this->clearConsole();
	 			this->currentPositionOnScreen = Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW);
				this->simStat = SimStatCodes::SIM;
 				this->Start(this->getIndex(this->currentHouseDescription));
				return true;
				break;
			case INTER_CHOISES::SAVE:
				if (this->saveSimulation() == SaveStat::SUCCESS)
				{
					messageBox(Save succeeded, The screen was saved successfully!, NULL);
				}
				else//save failed
				{
					messageBox(Save failed, The screen could not be saved.soory!, NULL);
				}
				break;
			case INTER_CHOISES::SHOW_SOLUTION:
				this->showSolution();
				break;
			case INTER_CHOISES::ALGO_RUN:
				this->beginSingleAlgo();
				this->simStat = SimStatCodes::SIM;
				break;
			case INTER_CHOISES::EXIT:
				this->menus->ExitPage();
				return true;
				break;
		}
	}
 }

bool Simulator::handleSolutionEscMenu()
{
	while (true)
	{
		SOLUTION_ESC_MENU_CHOISES choise = this->menus->DisplaySolutionEscMenu();
		switch (choise)
		{
		case SOLUTION_ESC_MENU_CHOISES::CONTINUE_SOLUTION:
			clear_screen();
			this->printConsole();
			this->drawRobot();
			gotoxy(BottomLeftCornerOfScreenCoordinates::Col, BottomLeftCornerOfScreenCoordinates::Row);
			cout << "Press any key to continue solution.";
			return false;
			break;
			
		case SOLUTION_ESC_MENU_CHOISES::START_SOLUTION:
			this->showSolution();
			return true;
			break;
		
		case SOLUTION_ESC_MENU_CHOISES::CONTINUE_SIMULATE:
			this->simStat = SimStatCodes::LOAD;
			this->load_aux(string("tmp.house_saved"), this->getIndex(this->currentHouseDescription) + 1);
			system("del tmp.house_saved");
			return false;
			break;
		case SOLUTION_ESC_MENU_CHOISES::START_SIMULATE:
			this->currentHouseDescription->changeAccordingToFile();
			this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
			this->clearConsole();
			this->currentPositionOnScreen = Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW);
			this->simStat = SimStatCodes::SIM;
			this->Start(this->getIndex(this->currentHouseDescription));
			return true;
			break;
		case SOLUTION_ESC_MENU_CHOISES::SOLUTION_BACK_TO_MM:
			this->currentHouseDescription->changeAccordingToFile();
			this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
			this->menus->DisplayMainMenu();
			return true;
			break;
		case SOLUTION_ESC_MENU_CHOISES::SOLUTION_EXIT:
			this->menus->ExitPage();
			return true;
			break;
		}
	}
}

void Simulator::drawRobot()
{
 	gotoxy(this->currentPositionOnScreen.getX(), this->currentPositionOnScreen.getY());
 	setTextColor(ROBOT_COLOR);
	cout << "@";
}

Point Simulator::getRealPositionInHouse(Point pointOnScreen)
{
 	Point offset = this->getOffset();
 	int HousexSize = this->currentHouseDescription->getXSize();
 	int HouseySize = this->currentHouseDescription->getYsize();
 	Point res = pointOnScreen;

 	res = res.distance(offset);

	if (res.getX() < 0)
		res = Point(HousexSize - abs(res.getX()),res.getY());
	else if (res.getX() >= HousexSize)
		res = Point(res.getX() - HousexSize,res.getY());
	if (res.getY() < 0)
		res = Point(res.getX(),HouseySize - abs(res.getY()));
	else if (res.getY()>HouseySize /*- 1*/)
		res = Point(res.getX(),res.getY() - HouseySize);
	return res;
}

void Simulator::move(Direction direction)
{
	switch (direction)
	{
	case Direction::North:
		if (this->currentPositionOnScreen.getY() == 0)
			this->currentPositionOnScreen = Point(this->currentPositionOnScreen.getX(),ConsoleScreenDimensions::ROWS - 1);
		else
			this->currentPositionOnScreen.move(0,-1);
		break;
	case Direction::South:
		if (this->currentPositionOnScreen.getY() == ConsoleScreenDimensions::ROWS - 1)
			this->currentPositionOnScreen.move(0, -this->currentPositionOnScreen.getY());
		else
			this->currentPositionOnScreen.move(0, 1);
		break;
	case Direction::East:
		if (this->currentPositionOnScreen.getX() == ConsoleScreenDimensions::COLS - 1)
			this->currentPositionOnScreen.move(-this->currentPositionOnScreen.getX(),0);
		else
			this->currentPositionOnScreen.move(1,0);
		break;
	case Direction::West:
		if (this->currentPositionOnScreen.getX() == 0)
			this->currentPositionOnScreen = Point(ConsoleScreenDimensions::COLS - 1, this->currentPositionOnScreen.getY());
		else
			this->currentPositionOnScreen.move(-1, 0);
		break;
	default:
		break;
	}
}

void Simulator::clean()
{
	char contentOfCurrentPositionOnScreen = this->getContentOfCurrentPositionOnScreen();
	if (contentOfCurrentPositionOnScreen >= '1' && contentOfCurrentPositionOnScreen <= '9')
	{
		if (this->getConfig(RQST_SOUNDS)&&simStat!=SimStatCodes::COMPET)
			PlaySound(MAKEINTRESOURCE(EAT), NULL, SND_ASYNC | SND_RESOURCE);
		int dirtLevel = contentOfCurrentPositionOnScreen - ASCII_VALUE_OF_ZERO_DIGIT;
		this->setContentOfCurrentPositionOnScreen(dirtLevel + ASCII_VALUE_OF_ZERO_DIGIT - 1);
		cleanedDust++;
	}
}

void Simulator::printContentOfCurrentPointOnScreen()
{
	int x = this->currentPositionOnScreen.getX(),
		y = this->currentPositionOnScreen.getY();
	gotoxy(this->currentPositionOnScreen.getX(), this->currentPositionOnScreen.getY());
	int dirtLevel = this->keyboardRobot->getSensor()->sense().dirtLevel;
	char contentOfCurrentPositionOnScreen;
	if (dirtLevel == SpecialDirtLevels::ZERO_DIRT_LEVEL)
		contentOfCurrentPositionOnScreen = SpecialPointsInHouse::ZeroDirtLevel;
	else if (dirtLevel == SpecialDirtLevels::DOCKING)
		contentOfCurrentPositionOnScreen = SpecialPointsInHouse::Docking;
	else contentOfCurrentPositionOnScreen = dirtLevel + '0';
	setTextColor(DIRT_COLOR);
	cout << contentOfCurrentPositionOnScreen;
	this->console[this->currentPositionOnScreen.getY()][this->currentPositionOnScreen.getX()] = contentOfCurrentPositionOnScreen;

	printIfWall(Point(x, y - 1));//north
	printIfWall(Point(x + 1, y));//east
	printIfWall(Point(x, y + 1));//south
	printIfWall(Point(x - 1, y));//west
}

void Simulator::printWalls()
{
	int x = this->currentPositionOnScreen.getX(),
		y = this->currentPositionOnScreen.getY();
	printIfWall(Point(x, y - 1));//north
	printIfWall(Point(x + 1, y));//east
	printIfWall(Point(x, y + 1));//south
	printIfWall(Point(x - 1, y));//west
}

void Simulator::beginSingleAlgo()//called from intermediate menu
{

	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();

	if (registrar.size() == 0) 
	{
		menus->DisplayGenericScreen("No Algorithms exists for this house.\n\nPress any key to continue.\n\n");
		WaitForKB();
		this->handleInterMenu();
		return;
	}

	auto algorithms = registrar.getAlgorithms();
	auto& algorithmNames = registrar.getAlgorithmNames();

	if (simStat == SimStatCodes::SIM|| simStat == SimStatCodes::LOAD)
	{
		this->saveSimulation(SimStatCodes::ALGO);
		this->simStat = SimStatCodes::ALGO;
	}
	menus->DisplayGenericScreen("Please choose an algorithm to show on this house:\n\n");
	for (int i = 0; i < algorithmNames.size(); i++)
	{
		cout << i << ": " << algorithmNames[i] << endl;
	}
	int choise;
	do
	{
		WaitForKB();
		choise = _getch() - '0';
	} while (choise < 0 || choise > algorithmNames.size()-1);

	this->algo = algorithms[choise].get();
	Sensor sensor(this);
	algo->setSensor(sensor);
	algo->setConfiguration(this->config->configMap);
	Start(this->getIndex(this->getCurrentHouseDescription()),"", choise);
	Sleep(2000);
	this->actionVector.clear();
	this->handleInterMenu();
	return;
}

bool Simulator::handleAlgoEscMenu()
{
	while (true)
	{
		ALGO_ESC_MENU_CHOISES choise = this->menus->DisplayAlgoEscMenu();
		switch (choise)
		{
		case ALGO_ESC_MENU_CHOISES::CONTINUE_ALGO:
			clear_screen();
			this->printConsole();
			this->drawRobot();
			
			gotoxy(BottomLeftCornerOfScreenCoordinates::Col, BottomLeftCornerOfScreenCoordinates::Row);
			cout << "Press any key to continue algorithm run.";
			return false;
			
			break;

		case ALGO_ESC_MENU_CHOISES::START_ALGO:
			this->beginSingleAlgo();
			return true;
			break;

		case ALGO_ESC_MENU_CHOISES::ALGO_CONTINUE_SIMULATE:
			this->simStat = SimStatCodes::LOAD;
			this->load_aux(string("tmp.house_saved"), this->getIndex(this->currentHouseDescription) + 1);
			system("del tmp.house_saved");
			return false;
			break;
		case ALGO_ESC_MENU_CHOISES::ALGO_START_SIMULATE:
			this->currentHouseDescription->changeAccordingToFile();
			this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
			this->clearConsole();
			this->currentPositionOnScreen = Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW);
			this->simStat = SimStatCodes::SIM;
			this->Start(this->getIndex(this->currentHouseDescription));
			return true;
			break;
		case ALGO_ESC_MENU_CHOISES::ALGO_BACK_TO_MM:
			this->currentHouseDescription->changeAccordingToFile();
			this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
			this->menus->DisplayMainMenu();
			return true;
			break;
		case ALGO_ESC_MENU_CHOISES::ALGO_EXIT:
			this->menus->ExitPage();
			return true;
			break;
		}
	}
}

void Simulator::printIfWall(Point p)
{
	char spotInfo = getContentOfPositionOnScreen(p);
	if (spotInfo == SpecialPointsInHouse::Wall)
	{
		setTextColor(WALLS_COLOR);
		gotoxy(p.getX(), p.getY());
		cout << spotInfo;
		this->console[p.getY()][p.getX()] = spotInfo;
	}
}

void Simulator::DisplayBottomLine()
{
	Battery* battery = this->keyboardRobot->getBattery();
	int currentBattery = battery->getCurrentBatteryLoad(),
		batteryCpacity = battery->getBatteryCapacity();
	int batteryInPrecentage = (currentBattery * 100 / batteryCpacity);
	gotoxy(BottomLeftCornerOfScreenCoordinates::Col, BottomLeftCornerOfScreenCoordinates::Row);
	cout << "                                                                            ";//Deleting the bottom line on screen
	gotoxy(BottomLeftCornerOfScreenCoordinates::Col, BottomLeftCornerOfScreenCoordinates::Row);
	setTextColor(BOTTOM_COLOR);
	int maxSteps = this->currentHouseDescription->getMaxSteps();
	if ((maxSteps - stepCount) < (maxSteps / 10)) setTextColor(LOW_BATTERY_COLOR);
	cout << "Iteration number: " << stepCount << "/" << maxSteps;
	setTextColor(BOTTOM_COLOR);
	cout << "  Dust in the house: " << DustAtBegining - cleanedDust;
	if (batteryInPrecentage <= 20) setTextColor(LOW_BATTERY_COLOR);
	cout << "  Battery status: " << batteryInPrecentage << "%";
}

Direction Simulator::getNextStepDirection(vector<string> actionsVector)
{
	int savedStepsNum = atoi(actionsVector[0].c_str());
	if (this->stepCount >= savedStepsNum)
	{
		this->simStat = SimStatCodes::SIM;
		Sleep(SLEEP_TIME);
		return this->keyboardRobot->getNextStepDirection();
	}
	else
	{
		string str = actionVector[stepCount + 1];
		char ch = str[str.length() - 1];
		switch (ch)
		{
			case SpecialKeys::UP_KEY1:
			case SpecialKeys::UP_KEY2:
				return Direction::North;
			case SpecialKeys::RIGHT_KEY1:
			case SpecialKeys::RIGHT_KEY2:
				return Direction::East;
			case SpecialKeys::DOWN_KEY1:
			case SpecialKeys::DOWN_KEY2:
				return Direction::South;
			case SpecialKeys::LEFT_KEY1:
			case SpecialKeys::LEFT_KEY2:
				return Direction::West;
			case SpecialKeys::STAY_KEY1:
			case SpecialKeys::STAY_KEY2:
				return Direction::Stay;
		}
	}
}

void Simulator::Start(int index, string fileNameToLoad, int algoNum)
{
	for (int i = index; i < this->houseDescriptionsVector.size();i++)
	{
		auto currHouse = this->houseDescriptionsVector[i];
		int stepCheck;
		int continueCheck;

		this->currentHouseDescription = currHouse;
		if (this->simStat == SimStatCodes::LOAD)
			convertFileToVector(fileNameToLoad, this->actionVector);

		if (simStat == SimStatCodes::SIM)
		{
			this->actionVector.clear();
			this->actionVector.push_back("0");
		}

			
		if (!currHouse->getValidBool())
			this->menus->displayFinishPage(currHouse->getValid(), false, 0, 0);
		else 
		{
			if (this->getConfig(RQST_SOUNDS))
				PlaySound(MAKEINTRESOURCE(WEREON), NULL, SND_ASYNC | SND_RESOURCE);

			this->DustAtBegining = currHouse->getDustAtBegining();
			this->isDeactivated = false;
			this->currentPositionOnScreen = DockingStationPositionOnScreen;
			this->cleanedDust = 0;
			this->stepCount = 0;
			this->begin = true;
			Battery battery(this);
			this->keyboardRobot->setBattery(&battery);
			this->keyboardRobot->getBattery()->Charge();//setting the battery to 100%
			//setting the sensor
			Sensor sensor(this);
			this->keyboardRobot->setSensor(sensor);
			
			clear_screen();
			drawRobot();//printing the first '@' in the center  */

			//=================================================			
			while (stepCheck = makeStep(algoNum) != BREAK_CODE && (continueCheck = continueSimulate()) == ContinueCodes::CONTINUE);
			//=================================================

			this->currentHouseDescription->changeAccordingToFile();
			this->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
			if (this->simStat == SimStatCodes::SOLUTION || this->simStat == SimStatCodes::ALGO)
				return;
			
			if (continueCheck == ContinueCodes::OUT_OF_BATTERY)
			{
				this->menus->displayFinishPage(Validation::BETTERY_RAN_OUT, false, stepCount, calculateScore());
			}
			if (continueCheck == ContinueCodes::MAX_STEPS)
			{
				this->menus->displayFinishPage(Validation::MAXSTEPS, false, stepCount, calculateScore());
			}
			if (continueCheck == ContinueCodes::FINISHED)
			{
				this->saveSolution();
				this->menus->displayFinishPage(Validation::VALID, true, stepCount, calculateScore());
			}
		}
		this->simStat = SimStatCodes::SIM;
	}
}

int Simulator::makeStep(int algoNum, Direction dir)
{
	if (this->simStat != SimStatCodes::COMPET && (this->begin ) && (this->simStat == SimStatCodes::SIM || this->simStat == SimStatCodes::ALGO || this->simStat == SimStatCodes::SOLUTION))
	{
			gotoxy(BottomLeftCornerOfScreenCoordinates::Col, BottomLeftCornerOfScreenCoordinates::Row);
			cout << "Press any key to begin...";
			this->resumeFlag = false;
			WaitForKB(); 
	}
 	this->begin = false;

	if (_kbhit())
	{
		char ch = _getch();
		if (ch == SpecialKeys::DOWN_KEY1 || ch == SpecialKeys::DOWN_KEY2 || ch == SpecialKeys::LEFT_KEY1 || ch == SpecialKeys::LEFT_KEY2 || ch == SpecialKeys::RIGHT_KEY1 || ch == SpecialKeys::RIGHT_KEY2 || ch == SpecialKeys::UP_KEY1 || ch == SpecialKeys::UP_KEY2 || ch == SpecialKeys::STAY_KEY1 || ch == SpecialKeys::STAY_KEY2 || ch == SpecialKeys::ESC_KEY)
			this->setLastPressedChar(ch);
	}

	if (this->keyboardRobot->getLastPressedChar() == SpecialKeys::ESC_KEY)
	{
		if (this->simStat == SimStatCodes::SOLUTION)
		{
			if (this->handleSolutionEscMenu())
				return BREAK_CODE;
			else
				WaitForKB();
		}
		else if (this->simStat == SimStatCodes::SIM)
		{
			if (handleInterMenu())
				return BREAK_CODE;
			else
				WaitForKB();
		}
		else if (this->simStat == SimStatCodes::ALGO)
		{
			if (this->handleAlgoEscMenu())
				return BREAK_CODE;
			else
				WaitForKB();
		}
	}

	static Direction direction = Direction::Stay;

 	if (this->simStat == SOLUTION)
 	{
 		Sleep(SLEEP_TIME);
 		direction = getSolutionDir();
 	}
    else if (this->simStat == SimStatCodes::LOAD)
 		direction = this->getNextStepDirection(actionVector);
	else if (this->simStat == SimStatCodes::ALGO)
	{
	Sleep(SLEEP_TIME);
		direction = this->algo->step(direction);
	}
	else if (this->simStat == SimStatCodes::COMPET)
	{
		direction = dir;
	}
	else if (this->simStat == SIM)
 	{
 		Sleep(SLEEP_TIME);
 		direction = this->keyboardRobot->getNextStepDirection();
		updateVector(direction);//updates actionVector head and back node
 	}
 		

 	if (!this->isStepValid(direction, this->keyboardRobot->getSensor()->sense()))
 	{
 		this->isDeactivated = true;
		if (this->simStat != SimStatCodes::COMPET)
		{
			this->menus->displayFinishPage(Validation::INVALID_STEP, false, this->stepCount, this->calculateScore());
			return BREAK_CODE;
		}
 	}
	else//valid step
 	{
 		this->clean();//downgrade the dust level by 1 in char** houseDescription, also increases the cleanedDust++

 		if (this->isRobotInDockingStation())
			this->keyboardRobot->getBattery()->Charge();
 		else
			this->keyboardRobot->getBattery()->Consume();
 		if(this->simStat != SimStatCodes::COMPET)
			printWalls();//prints walls in 4 directions
		if (this->simStat != SimStatCodes::COMPET)
		{
			if (direction != Direction::Stay)
			{
				if (this->getConfig(RQST_SOUNDS))
					PlaySound(MAKEINTRESOURCE(WALK), NULL, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

				this->printContentOfCurrentPointOnScreen();
				this->move(direction);
				drawRobot();
			}
			else//if direction==stay : blink the robot
			{
				if (stepCount % BLINK_SPEED < BLINK_SPEED / 2)
					drawRobot();
				else
					printContentOfCurrentPointOnScreen();
			}
		}
		else if (direction != Direction::Stay)
			this->move(direction);

 		stepCount++;
 	}
	if (this->simStat != SimStatCodes::COMPET)
		DisplayBottomLine();
	//when finished cleaning, color the docking station in green, and play the finish sound
 	if (DustAtBegining == cleanedDust && this->simStat != SimStatCodes::COMPET)
 	{
 		gotoxy(this->DockingStationPositionOnScreen.getX(), this->DockingStationPositionOnScreen.getY());
 		setTextColor(Color::GREEN);
 		cout << "D";
 		if (this->dummyCheck && this->getConfig(RQST_SOUNDS))
 		{
 			PlaySound(MAKEINTRESOURCE(FINISH_CLEANING_SOUND), NULL, SND_ASYNC | SND_RESOURCE);
			this->dummyCheck = false;
 		}
 	}
}

ContinueCodes Simulator::continueSimulate()
{
 	if (this->stepCount >= this->currentHouseDescription->getMaxSteps()) return ContinueCodes::MAX_STEPS;
 	if (this->keyboardRobot->getBattery()->getCurrentBatteryLoad() == 0 && !(this->isRobotInDockingStation())) return ContinueCodes::OUT_OF_BATTERY;
	if (this->isRobotInDockingStation() && (DustAtBegining - cleanedDust == 0))return ContinueCodes::FINISHED;
 	if (this->keyboardRobot->getBattery()->getCurrentBatteryLoad()>0 || this->isRobotInDockingStation()) return ContinueCodes::CONTINUE;
 	return ContinueCodes::CONTINUE;
}

void Simulator::printConsole()
{
 	char ch;
 	for (int i = 0; i < ConsoleScreenDimensions::ROWS; i++)
 	{
 		gotoxy(0, i);
 		for (int j = 0; j < ConsoleScreenDimensions::COLS; j++)
 		{
 			ch = this->console[i][j];
 			switch (ch)
 			{
 			case SpecialPointsInHouse::Wall:
 				setTextColor(WALLS_COLOR);
 				break;
 			case SpecialPointsInHouse::Docking:
 				setTextColor(DIRT_COLOR);
 				break;
 			default:
 				break;
 			}
 			if (ch <= '9'&&ch >= '1')
				setTextColor(DIRT_COLOR);
 			cout << ch;
 		}
 	}
}

int Simulator::calculateScore()
{
 	int position = 1;
	int score;
	if (this->isDeactivated)
		score = 0;
	else
	{
		score = 2000;
		if (this->DustAtBegining != cleanedDust || !(this->currentPositionOnScreen==DockingStationPositionOnScreen))
			position = 10;
		score -= (position - 1) * 50;
		score -= (DustAtBegining - cleanedDust) * 3;
		if (currentPositionOnScreen==DockingStationPositionOnScreen)
			score += 50;
		else
			score -= 200;
	}
	return score;
	}

int Simulator::getConfig(CONFIG_RQST rqst)
{
 	switch (rqst)
 	{
	 	case RQST_SOUNDS:
		return this->config->getSounds();
		break;
	case RQST_BATTERY_CAP:
		return config->BatteryCapacity;
		break;
	case RQST_BATTERY_CONSUMP:
		return config->BatteryConsumptionRate;
		break;
	case RQST_BATTERY_RECH:
		return config->BatteryRechargeRate;
	case RQST_MAX_STEPS_AFTER:
		return config->MaxStepsAfterWinner;
	default:
		return 0;
		break;
	}
}

void Simulator::clearConsole()
{
	for (int i = 0; i < ConsoleScreenDimensions::ROWS; i++)
		for (int j = 0; j < ConsoleScreenDimensions::COLS; j++)
 			this->console[i][j] = ' ';
}

void Simulator::play(const char* sound)
{
	if (this->getConfig(RQST_SOUNDS))
 	{
 		if (strcmp(sound, "welcome") == 0)
 		{
 			PlaySound(MAKEINTRESOURCE(WELCOME_HEY_MAMA), NULL, SND_ASYNC | SND_RESOURCE);
 			return;
		}

 		if (strcmp(sound, "exit") == 0)
 		{
 			PlaySound(MAKEINTRESOURCE(EXIT_SOUND), NULL, SND_ASYNC | SND_RESOURCE);
 			return;
		}
 		if (strcmp(sound, "finish_succsess") == 0)
 		{
 			PlaySound(MAKEINTRESOURCE(FINISH_SUCCSESS), NULL, SND_ASYNC | SND_RESOURCE);
 			return;
 		}
 		if (strcmp(sound, "finish_failed") == 0)
		{
			PlaySound(MAKEINTRESOURCE(FINISH_FAILED), NULL, SND_ASYNC | SND_RESOURCE);
			return;
		}
		if (strcmp(sound, "eat") == 0)
		{
			PlaySound(MAKEINTRESOURCE(EAT), NULL, SND_ASYNC | SND_RESOURCE);
			return;
		}
		if (strcmp(sound, "walk") == 0)
		{
			PlaySound(MAKEINTRESOURCE(WALK), NULL, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);
			return;
		}
		if (strcmp(sound, "walk") == 0)
		{
			PlaySound(MAKEINTRESOURCE(FINISH_CLEANING_SOUND), NULL, SND_ASYNC | SND_RESOURCE);
			return;
		}
	}
}

vector<string> Simulator::getFilesNamesVector(const string& filesNamesFile)
{
 	vector<string> filesNamesList;
 	char tmp[128];
 	fstream inFile(filesNamesFile);
 	inFile.getline(tmp, 128);
	filesNamesList.push_back(string(tmp));
	while (!inFile.eof())
	{
		inFile.getline(tmp, 128);
		if (strcmp(tmp, "") != 0)
		filesNamesList.push_back(string(tmp));
	}
	return filesNamesList;
	}

void Simulator::initHouseDescriptionsVector()
{
 	vector<string> filesNamesList = getFilesNamesVector();

 	if (filesNamesList.front().compare("") == 0)//no houses in the dir
 		return;

 	for (string fileName : filesNamesList)
 	{
 		HouseDescription* houseDescription = new HouseDescription(fileName);
		this->houseDescriptionsVector.push_back(houseDescription);
 	}
}

string Simulator::getSavedFileName()
{
	string res = this->currentHouseDescription->getFileName();
 	string name;
	this->menus->DisplayGenericScreen("Please enter name for your saved file: \n\n");
	cin >> name;
	name.insert(0, "-");
	res.insert(3, name);
 	res.insert(res.length(), "_saved");
 	validateSave(res);
 	return res;
}

SaveStat Simulator::saveSimulation(SimStatCodes simStat)
{
 	string fileName;
	if (simStat == SIM)
 		fileName = this->getSavedFileName();
 	else
 		fileName = "tmp.house_saved";
 	save_aux(fileName);
 	return SUCCESS;
}

void Simulator::save_aux(string fileName)
{
 	ofstream fileToSave;
 	fileToSave.open(fileName, ios::out | ofstream::trunc);//creates the file and resets it
	string str;

 	str = actionVector[0];
	fileToSave.write(str.c_str(), str.length());
	fileToSave.put('\n');
    int i = 0;
	while (i < this->actionVector.size())
	{
		string lastDir = "";
		if (i>0) lastDir = actionVector[i - 1];
	 		str = actionVector[i];
		if (i > 0 && str[str.length() - 1] != lastDir[lastDir.length() - 1])
	 	{
			fileToSave.write(str.c_str(), str.length());
	 		fileToSave.put('\n');
		}
		i++;
	}
	
	fileToSave.close();
}

SaveStat Simulator::saveSolution()
{
 	string fileName = this->currentHouseDescription->getFileName();

 	fileName += "_solution";
 	if (isBetterSol(fileName, this->stepCount + 1))
 		return BETTER_SOL;
	save_aux(fileName);

 	return SUCCESS;
}

bool Simulator::isBetterSol(const string& name, int steps)
{
 	if (!isFileExists(name))
 		return false;//file does'nt exists...

 	ifstream file(name);

 	char line[128];

 	file.getline(line, '\n');
	if (steps < atoi(line))
 		return false;
 	else
 		return true;

 	//file closed by destructor
}

void Simulator::updateVector(Direction direction)
{
 	string sc = to_string(this->stepCount + 1);//stepCount

 	actionVector[0] = sc;
 	char nextDirection;
 	switch (direction)
	{
 		case (Direction::North) :
			nextDirection = 'w';
			break;
		case (Direction::East) :
			nextDirection = 'd';
			break;
		case (Direction::South) :
			nextDirection = 'x';
			break;
		case (Direction::West) :
			nextDirection = 'a';
			break;
		case (Direction::Stay) :
			nextDirection = 's';
			break;
		default:
			break;
	}
	sc += ": ";
	sc += nextDirection;
	actionVector.push_back(sc);
}

Direction Simulator::getSolutionDir()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		if (ch == SpecialKeys::DOWN_KEY1 || ch == SpecialKeys::DOWN_KEY2 || ch == SpecialKeys::LEFT_KEY1 || ch == SpecialKeys::LEFT_KEY2 || ch == SpecialKeys::RIGHT_KEY1 || ch == SpecialKeys::RIGHT_KEY2 || ch == SpecialKeys::UP_KEY1 || ch == SpecialKeys::UP_KEY2 || ch == SpecialKeys::STAY_KEY1 || ch == SpecialKeys::STAY_KEY2 || ch == SpecialKeys::ESC_KEY)
			this->setLastPressedChar(ch);
	}

	Direction res;
	string str = actionVector[this->stepCount + 1];
	char tmp = str[(str.length()) - 1];

	switch (tmp)
	{
	case 'w':
		res = Direction::North;
		break;
	case 'a':
		res = Direction::West;
		break;
	case 'd':
		res = Direction::East;
		break;
	case 'x':
		res = Direction::South;
		break;
	case 's':
		res = Direction::Stay;
		break;
	}
	return res;
}

void Simulator::convertFileToVector(string fileName, vector<string>& actionVector)
{
	ifstream file(fileName);
	string tmp;
	char buffer[8];
	int nextSC, prevSC = 1, totalSteps;//stepCount
	actionVector.clear();
	file.getline(buffer, '\n');
	actionVector.push_back(buffer);//insert first row-step count
	totalSteps = atoi(buffer);
	
	file.getline(buffer, '\n');
	actionVector.push_back(buffer);//insert first line
	
	for (int i = 1; i < totalSteps; i++)
	{
		file.getline(buffer, '\n');
		nextSC = getNextSC(buffer);
		if (nextSC == 0)
			nextSC = totalSteps;
		for (int j = i; j < nextSC - 1; j++, i++)
		{
			duplicateElement(actionVector, j);
		}
		if (strcmp(buffer, "") != 0)
			actionVector.push_back(buffer);
		else
		{
			duplicateElement(actionVector, totalSteps - 1);
			return;
		
		}
	}
}

string Simulator::getSoultionFileName()
{
 	string fileName = this->getCurrentHouseDescriptionFileName();
 	fileName += "_solution";
 	return fileName;
}

void Simulator::showSolution()
{
	if (this->simStat != SimStatCodes::SOLUTION)
	{
		this->saveSimulation(SimStatCodes::SOLUTION);
		this->simStat = SimStatCodes::SOLUTION;
	}
 	string fileName = getSoultionFileName();
	if (!ifstream(fileName))//file not found
	{
		menus->DisplayGenericScreen("No solution exists for this house.\n\nPress any key to continue.\n\n");
		WaitForKB();
		this->handleInterMenu();
		return;
	}
 	convertFileToVector(fileName, this->actionVector);
 	Start(this->getIndex(this->getCurrentHouseDescription()));
 	this->actionVector.clear();
	this->handleInterMenu();
	return;
}

void Simulator::loadSavedSim()
{
 	int chosenHouseNumber = this->chooseHouseNumber();
	if (chosenHouseNumber == -1)
 		return;
	else if (chosenHouseNumber == 0)
 	{
 		this->menus->DisplayMainMenu();
 		return;
	}
	else
 	{
 		string chosenSavedFileName = this->chooseSavedHouseFile(chosenHouseNumber);
 		this->load_aux(chosenSavedFileName, chosenHouseNumber);
	}
}

void Simulator::load_aux(string chosenSavedFileName, int chosenHouseNumber)
{
	this->simStat = SimStatCodes::LOAD;
	this->setCurrentPositionOnScreen(Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW));
	this->clearConsole();
	this->Start(chosenHouseNumber - 1, chosenSavedFileName);
}

int Simulator::chooseHouseNumber()
{
 	this->menus->displayHeader();
 	int chosenHouseNumber;
 	int numberOfHouses = this->getHouseDescriprionsVector().size();
 	string chosenSavedFileName;
 	if (numberOfHouses == 0)
	{
		cout << "\n\n\nThere aren't houses in the root directory...\n\n";
 		this->menus->DisplayMainMenu();
 		return -1;
 	}
 	else
 	{
		cout << "\n\n\nThere are " << numberOfHouses << " houses numbered from 1 to " << numberOfHouses << "." << endl << "\nPlease choose an house number to start with: or zero to go back to main menu\n" << endl;
		cin >> chosenHouseNumber;
 	}
 	return chosenHouseNumber;
}

string Simulator::chooseSavedHouseFile(int chosenHouseNumber)
{
	if (chosenHouseNumber == 0)
	{
		this->menus->DisplayMainMenu();
		return string("");
	}
	else
	{
		this->menus->displayHeader();
		cout << "Starting from house number " << chosenHouseNumber << endl;
		vector<string> savedFilesForChosenFile = this->getSavedHousesFileNames(this->getHouseDescriprionsVector()[chosenHouseNumber - 1]->getFileName());
		if (savedFilesForChosenFile.size() == 0)
		{

			cout << "Sorry:There is no saved file for the file you have chosen,so press a key to go back to main menu" << endl;
			WaitForKB();
			this->menus->DisplayMainMenu();
			return string("");
		}
		else if (savedFilesForChosenFile.size() == 1)
			return savedFilesForChosenFile[0];
		else if (savedFilesForChosenFile.size() > 1)
		{
			cout << "Please choose a number of the saved file,you want , from the list of the saved files of the chosen house,below,or zero to go back to main menu:" << endl;
			int i = 1;
			for (string savedFileName : savedFilesForChosenFile)
			{
				cout << i << ")" << savedFileName << endl;
				i++;
			}
			int chosenSavedFileNumber;
			cin >> chosenSavedFileNumber;
			if (chosenSavedFileNumber == 0)
			{
				this->menus->DisplayMainMenu();
				return string("");
			}
			else
			{
				return savedFilesForChosenFile[chosenSavedFileNumber - 1];
			}
		}
	}
}

vector<string> Simulator:: getFilesNamesVector()
{
	vector<string> res;
	system("dir /a:-d /b | findstr /r /x [0-9][0-9][0-9].house > filesNames.txt");
	res = getFilesNamesVector("filesNames.txt");
	system("del filesNames.txt");
	return res;
}

Simulator::~Simulator()
{
	for (HouseDescription* houseDescription : this->houseDescriptionsVector) 
		delete houseDescription; delete this->config; 
	delete this->keyboardRobot;
	delete this->menus;
}

vector<string> Simulator::getSavedHousesFileNames()
{
	vector<string> res;
	system("dir /a:-d /b | findstr /r [0-9][0-9][0-9]-.*house_saved>filesNames.txt");
	res = getFilesNamesVector("filesNames.txt");
	system("del filesNames.txt");
	return res;
}

int Simulator::getIndex(HouseDescription* houseDescription)
{
	for (int i = 0; i < this->houseDescriptionsVector.size(); i++)
	{
		if (houseDescription == this->houseDescriptionsVector[i])
			return i;
	}
	return NOT_FOUND;
}

vector<string> Simulator::getSavedHousesFileNames(string houseDescriptionFileName)
{
	vector<string> res;
	vector<string> savedHousesFileNames = getSavedHousesFileNames();
	for (string str : savedHousesFileNames)
	{
		if (str.compare(0, 3, houseDescriptionFileName, 0, 3) == 0)
		{
			res.push_back(str);
		}
	}
	return res;
}
