#include <cctype>
#include "SpecialFunctions.h"
#include "Defs.h"
using namespace std;
#ifndef WINDOWS
	void gotoxy(int x, int y) {}
 	int _getch(void) { return 0; }
 	int _kbhit(void) { return 0; }
 	void Sleep(unsigned long) {}
 	void setTextColor(Color) {}
 	void hideCursor() {}
 	void clear_screen() {}
#else
 	void gotoxy(int x, int y)
 	{
		HANDLE hConsoleOutput;
 		COORD dwCursorPosition;
 		cout << flush;
 		dwCursorPosition.X = x;
 		dwCursorPosition.Y = y;
 		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 		SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
 	}

 	void setTextColor(Color colorToSet)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);

	}

 	void hideCursor()
 	{
		HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
 		CONSOLE_CURSOR_INFO CURSOR;
		CURSOR.dwSize = 1;
 		CURSOR.bVisible = FALSE;
		SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
	}

	void clear_screen()
	{
		system("cls");
	}

#endif

int _messageBox(const LPCWSTR head, LPCWSTR body, UINT flags)
{
	return MessageBox(NULL, body, head, flags);
}

void copyStringsArr(char**& stringsArrNew, const char stringsArrOld[][DefaultHouseDimensions::Cols + 1], int ySize)
{
 	stringsArrNew = new char*[ySize];
 	for (int i = 0; i < ySize; i++)
 	{
 		stringsArrNew[i] = new char[DefaultHouseDimensions::Cols + 1];
 		strcpy_s(stringsArrNew[i],100, stringsArrOld[i]);
 	}
}

void copyStringsArr(char**& stringsArrNew, const char** stringsArrOld, int xSize, int ySize)
{
 	stringsArrNew = new char*[ySize];
 	for (int i = 0; i < ySize; i++)
 	{

		stringsArrNew[i] = new char[xSize + 1];
 		strcpy_s(stringsArrNew[i],100, stringsArrOld[i]);
 	}
}

bool isHouseDescritionFileNameValid(char* fileName)
{
 	if (strlen(fileName) != HouseDescriptionFileNameLength)
 		return false;
	else
	{
 		for (int i = 0; i < 3; i++)
 		{
 			if (!(isdigit(fileName[i])))
 				return false;
 		}
 		if (strcmp(fileName + 5, ".House") == 0)
 				return true;
		else return true;
 	}
}

