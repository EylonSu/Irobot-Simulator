#include "Menus.h"
#include "Competition.h"
using namespace std;

void Menus::displayHeader()	//sets the headline
{
	clear_screen();
 	system("Color 0D");
 	setTextColor(Color::YELLOW);
 	cout << endl <<
 			R"(  | || . \ ___ | |_  ___ _| |_    / __><_>._ _ _  _ _ | | ___ _| |_ ___  _ _ )" << endl <<
 			R"(  | ||   // . \| . \/ . \ | |     \__ \| || ' ' || | || |<_> | | | / . \| '_>)" << endl <<
 			R"(  |_||_\_\\___/|___/\___/ |_|     <___/|_||_|_|_|`___||_|<___| |_| \___/|_|)" << endl;
}

void Menus::DisplayMainMenu()
{
	bool start = true;
 	char mm1[] = "Press 1 to start the simulation on all the houses",
    mm2[] = "Press 2 to start the simulation from a specific house",
 	mm3[] = "press 3 to continue a saved simulation",
 	mm4[] = "Press 4 to toggle sound. Sound is now ",
	mm41[]= "Press 5 to simulate all algorithms",
	mm5[] = "Press 8 for instructions",
	mm6[] = "Press 9 to exit",
	copyrt[] = "Copyright Eran David && Eylon Sultan",
	noHouses[] = "\n\n\nThere is no houses in your root dir.\nPlease create a house and return.\nGoodBye!\n\n";
	displayHeader();
	setTextColor(HEADER_COLOR);
	if (this->simulator->getHouseDescriprionsVector().size() == 0)
	{
		cout << noHouses;
		WaitForKB();
		return;
	}
	while (true)
	{
		if (start) {

			displayHeader();
			setTextColor(HEADER_COLOR);

			gotoxy(40 - (int)(strlen(mm1) / 2), 6);
			cout << mm1 << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 7);
			cout << mm2 << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 8);
			cout << mm3 << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 9);
			cout << mm4; if (this->simulator->getConfig(RQST_SOUNDS)) cout << "on" << endl; else cout << "off" << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 10);
			cout << mm41 << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 11);
			cout << mm5 << endl;
			gotoxy(40 - (int)(strlen(mm1) / 2), 12);
			cout << mm6 << endl;
			gotoxy(40 - (int)(strlen(copyrt) / 2), 16);
			cout << copyrt;

			WaitForKB();
			start = false;
		}
		char ch = _getch() - '0';
		switch (ch)
		{
	 		case 1:
	 			this->simulator->setCurrentPositionOnScreen(Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW));
	 			this->simulator->clearConsole();
	 			this->simulator->setCurrentPositionOnScreen(Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW));
	 			this->simulator->setKeyboardLastPressedChar(SpecialKeys::STAY_KEY1);
				this->simulator->setSimStat(SimStatCodes::SIM);
	 			this->simulator->Start(0);
				start = true;
	 			break;
	 		case 2:
				this->simulator->setSimStat(SimStatCodes::SIM);
	 			this->startFromCertainHouse();
				start = true;
	 			break;
	 		case 3:
	 			this->simulator->loadSavedSim();
				start = true;
				break;
	 		case 4:
	 			this->simulator->toggleSound();
	 			if (this->simulator->getConfig(RQST_SOUNDS))
	 			{
	 				gotoxy(40 - (strlen(mm1) / 2), 9);
	 				cout << mm4 << "on ";
	 			}
				else
	 			{
	 				this->simulator->shutUp();
	 				gotoxy(40 - (strlen(mm1) / 2), 9);
	 				cout << mm4 << "off";
	 			}
	 			break;
			case 5:
			{
				Competition compet;
				compet.startAllAlgos();
				start = true;
				break; 
			}
			case 8:
	 			DisplayInsturctions();
				start = true;
	 			break;
	 		case 9:
	 			ExitPage();
	 			break;
	 		default:
	 			break;
 		}
	}
}

void Menus::DisplayInsturctions()
{
 	displayHeader();

 	gotoxy(40 - (strlen("instructions:") / 2), 6);
 	cout << "Instructions:\n" << endl;

 	cout << "the folowing simulation is working with key board pressing:" << endl;

 	cout << "'w' - for north direction." << endl;

 	cout << "'d' - for right direction." << endl;

 	cout << "'a' - for west direction." << endl;

 	cout << "'x' - for south direction." << endl;

 	cout << "'s' - for staying in position." << endl;

 	cout << "press enter to return the main menu." << endl;

 	WaitForKB();

 	DisplayMainMenu();
 	return;
}

void Menus::ExitPage()
{
	displayHeader();
	gotoxy(20, 6);
 	cout << "Goodbye! Hope you enjoyed our simulator!" << endl <<
		"Press any key to exit...\n";

 	this->simulator->play("exit");

 	WaitForKB();
 	exit(EXIT_SUCCESS);
}

void Menus::ExitPage(char* textToShow)
{
 	displayHeader();

 	gotoxy(0, 6);
 	cout << textToShow << endl <<
 		"Press any key to exit...\n";
 	this->simulator->play("exit");

 	WaitForKB();
 	exit(EXIT_SUCCESS);
}

INTER_CHOISES Menus::DisplayIntermediateMenu()
{
 	displayHeader();

	gotoxy(0, 6);
	cout << "Press 1 to continue the simulation" << endl <<
		"Press 2 to resimulate this house all over again" << endl <<
		"Press 3 to save the screen" << endl <<
		"Press 4 to show solution to this house" << endl <<
		"Press 5 to show algorithm run" << endl << 
 		"Press 8 to go to main menu" << endl <<
 		"To exit, press 9" << endl;

 	while (true)
 	{
 		WaitForKB();
 		INTER_CHOISES choise = (INTER_CHOISES)(_getch() - '0');
 		if (choise == INTER_CHOISES::EXIT)
 			ExitPage();
 		else
 			return choise;
 	}
}

SOLUTION_ESC_MENU_CHOISES Menus::DisplaySolutionEscMenu()
{
	displayHeader();

	gotoxy(0, 6);
	cout << "Press 1 to continue solution" << endl <<
		"Press 2 to start the solution of this house all over again" << endl <<
		"Press 3 to continue simulation(game) for this house" << endl <<
		"Press 4 to start simulation(game) for this house all over again" << endl <<
		"Press 8 to go to main menu" << endl <<
		"To exit, press 9" << endl;

	while (true)
	{
		WaitForKB();
		SOLUTION_ESC_MENU_CHOISES choise = (SOLUTION_ESC_MENU_CHOISES)(_getch() - '0');
		if (choise == SOLUTION_ESC_MENU_CHOISES::SOLUTION_EXIT)
			ExitPage();
		else
			return choise;
	}
}

void Menus::displayFinishPage(Validation houseValidity, bool finished, int iterations, int score)//old
{
	displayHeader();
	gotoxy(0, 6);
	switch (houseValidity)
	{
	case Validation::NULL_HOUSE:
		cout << "One or more of the following parameters: xSize, ySize, maxSteps of the current house, as they appear in the file description of the current house is zero or contains non-numeric value,so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
		break;
	case Validation::INVALID_DIMENTIONS:
		cout << "wrong house dimentions:\nOnly house with dimentions of " << houseDimentionsLimits::MinRows << " <= rows <= " << houseDimentionsLimits::MaxRows << " && " << houseDimentionsLimits::MinCols << " <= cols <= " << houseDimentionsLimits::MaxCols << " is valid,so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "\nThis was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
		break;
	case Validation::MULTI_DOCKING:
		cout << "The current house has more than one docking station,\nso the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "\nPlease press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
		break;
	case Validation::NO_DOCKING:
		cout << "The current house has no docking station, so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "\nThis was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
		break;
	case Validation::VALID:
		if (finished)
		{
			this->simulator->play("finish_success");
			if (this->simulator->getCurrentHouseDescription() != this->simulator->getHouseDescriprionsVector().back())
			{
				cout << "Congratulation! you've finished cleaning the house from all of it's dust!\n";
				cout << "You did it in " << iterations << " iterations.\n"
					"And your score is " << score << ".\n\n\n"
					"Press any key to go to next house";
				WaitForKB();
				return;
			}
			else
			{
				cout << "Congratulation! you've finished cleaning the last house from all of it's dust!\n";
				cout << "You did it in " << iterations << " iterations.\n"
					"And your score is " << score << ".\n\n\n"
					"Press any key to go back to main menu";
				WaitForKB();
				DisplayMainMenu();
				return;
			}
	case Validation::INVALID_STEP:
		cout << "You made " << iterations << " iterations on this house,but failed to clean it,\nbecause your the robot made an invali step and went on a wall in the house." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		//DisplayMainMenu();
		return;
	case Validation::BETTERY_RAN_OUT:
		cout << "You made " << iterations << " iterations on this house,but failed to clean it,\nbecause the robot's battery ran out." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		//DisplayMainMenu();
		return;
	case Validation::MAXSTEPS:
		cout << "You made " << iterations << " iterations on this house, but failed to clean it on time." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		//DisplayMainMenu();
		return;
		}
	}
}

void Menus::displayFinishPage(map<Validation, bool> houseValidity, bool finished, int iterations, int score)
{
	displayHeader();
	gotoxy(0, 6);

	if (houseValidity.find(Validation::NULL_HOUSE) != houseValidity.end())
	{
		cout << "One or more of the following parameters: xSize, ySize, maxSteps of the current house, as they appear in the file description of the current house is zero or contains non-numeric value,so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
	}
	if (houseValidity.find(Validation::INVALID_DIMENTIONS) != houseValidity.end())
	{
		cout << "wrong house dimentions:\nOnly house with dimentions of " << houseDimentionsLimits::MinRows << " <= rows <= " << houseDimentionsLimits::MaxRows << " && " << houseDimentionsLimits::MinCols << " <= cols <= " << houseDimentionsLimits::MaxCols << " is valid,\nso the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
	}
	if (houseValidity.find(Validation::MULTI_DOCKING) != houseValidity.end())
	{
		cout << "The current house has more than one docking station,so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
	}
	if (houseValidity.find(Validation::NO_DOCKING) != houseValidity.end())
	{
		cout << "The current house has no docking station, so the simulator will skip this house" << endl;
		if (this->simulator->getCurrentHouseDescription() == this->simulator->getHouseDescriprionsVector().back())
		{
			cout << "This was the last house." << "Please press any key to go back to main menu" << endl;
			WaitForKB();
			this->DisplayMainMenu();
		}
		else
		{
			cout << "Please press any key to go to the next house" << endl;
			WaitForKB();
		}
	}
	if (houseValidity.find(Validation::VALID) != houseValidity.end())
	{
		if (finished)
		{
			this->simulator->play("finish_success");
			if (this->simulator->getCurrentHouseDescription() != this->simulator->getHouseDescriprionsVector().back())
			{
				cout << "Congratulation! you've finished cleaning the house from all of it's dust!\n";
				cout << "You did it in " << iterations << " iterations.\n"
					"And your score is " << score << ".\n\n\n"
					"Press any key to go to next house";
				WaitForKB();
				return;
			}
			else
			{
				cout << "Congratulation! you've finished cleaning the last house from all of it's dust!\n";
				cout << "You did it in " << iterations << " iterations.\n"
					"And your score is " << score << ".\n\n\n"
					"Press any key to go back to main menu";
				WaitForKB();
				DisplayMainMenu();
				return;
			}
		}
	}
	if (houseValidity.find(Validation::INVALID_STEP) != houseValidity.end())
	{
		cout << "You made " << iterations << " iterations on this house,but failed to clean it,\nbecause your the robot made an invali step and went on a wall in the house." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		return;
	}
	if (houseValidity.find(Validation::BETTERY_RAN_OUT) != houseValidity.end())
	{
		cout << "You made " << iterations << " iterations on this house,but failed to clean it,\nbecause the robot's battery ran out." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		return;
	}
	if (houseValidity.find(Validation::MAXSTEPS) != houseValidity.end())
	{
		cout << "You made " << iterations << " iterations on this house, but failed to clean it on time." << endl <<
			"your score is " << score << ".\n\n\n"
			"Press any key to continue";
		WaitForKB();
		return;
	}

}

void Menus::startFromCertainHouse()
{
 	displayHeader();
 	int chosenHouseNumber;
 	int numberOfHouses = this->simulator->getHouseDescriprionsVector().size();
 	if (numberOfHouses == 0)
 	{
 		cout << "\nThere aren't houses in the root directory...\n";
 		DisplayMainMenu();
 	}
 	cout << "\nThere are " << numberOfHouses << " houses numbered from 1 to " << numberOfHouses << "." << endl << "Please choose an house number to start with:" << endl;
	bool validHouse = true;
	bool ValidNum = true;
	do
	{
		validHouse = true;
		ValidNum = true;
		cin >> chosenHouseNumber;
		ValidNum = chosenHouseNumber <= numberOfHouses && chosenHouseNumber >= 1;
		if (!ValidNum) {
			cout << "Incorrect choise, please insert a correct one: ";
			continue;
		}
		validHouse = this->simulator->getHDVector()[chosenHouseNumber-1]->getValidBool();
		if (!validHouse)
			cout << "House can't load due to house file problems.\nPlease choose an other house.\n";
	} while (!validHouse || !ValidNum);
 	displayHeader();
 	this->simulator->Start(chosenHouseNumber - 1);
}

void Menus::DisplayGenericScreen(const char* text)
{
 	clear_screen();
 	displayHeader();
 	cout << "\n\n\n";
 	cout << text ;
 	return;
}

ALGO_ESC_MENU_CHOISES Menus::DisplayAlgoEscMenu()
{
	displayHeader();

	gotoxy(0, 6);
	cout << 
		"Press 1 to continue algorithm run" << endl <<
		"Press 2 to start the algorithm run of this house all over again" << endl <<
		"Press 3 to continue simulation(game) for this house" << endl <<
		"Press 4 to start simulation(game) for this house all over again" << endl <<
		"Press 8 to go to main menu" << endl <<
		"To exit, press 9" << endl;

	while (true)
	{
		WaitForKB();
		ALGO_ESC_MENU_CHOISES choise = (ALGO_ESC_MENU_CHOISES)(_getch() - '0');
		if (choise == ALGO_ESC_MENU_CHOISES::ALGO_EXIT)
			ExitPage();
		else
			return choise;
	}
}