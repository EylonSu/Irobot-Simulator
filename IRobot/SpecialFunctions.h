#pragma once

#include <cctype>
#include <iostream>
#include <stdio.h>
#include <list>
#include <fstream>

using namespace std;

#include "Config.h"
#include "Defs.h"
#include "resource.h"


#ifndef WINDOWS	
	int _getch(void);
	int _kbhit(void);
	void Sleep(unsigned long);
#define messageBox(_head, _body) strcmp(L#_head, L#_body);

#else
	#include <stdlib.h>
	#include <conio.h>
	#include <windows.h>
	#include <process.h>
	#define messageBox(_head, _body, _flags) _messageBox(L#_head, L#_body, _flags)
	#pragma comment(lib, "winmm.lib")  
#endif

void gotoxy(int x, int y);
void setTextColor(Color color);
void hideCursor();
void clear_screen();
void copyStringsArr(char**& stringsArrNew,const char stringsArrOld[][DefaultHouseDimensions::Cols+1],  int ySize);
void copyStringsArr(char**& stringsArrNew, const char** stringsArrOld,int xSize, int ySize);

inline void WaitForKB() 
{
	if (_kbhit()) _getch();
	while (true) if (_kbhit()) break;
}
int _messageBox(const LPCWSTR head, LPCWSTR body, UINT flags = NULL);