#ifndef _HOUSE_DESCRIPTION_
#define _HOUSE_DESCRIPTION_


#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

#include "Defs.h"
#include "Point.h"
#include "SpecialFunctions.h"

	class Point;
	
	class HouseDescription
	{
	private:
		int xSize;
		int ySize;
		char** house;
		int maxSteps;
		string fileName;
		map<Validation,bool> validationMap;
		bool valid = true;
		int dustAtBegining = 0;
		Point offset{ -1,-1 };
		Point DockingStationPositionOnScreen = Point(DockingStationPositionOnScreen::COL, DockingStationPositionOnScreen::ROW);

	public:
		HouseDescription() : maxSteps(1500), xSize(DefaultHouseDimensions::Cols), ySize(DefaultHouseDimensions::Rows),fileName(""){  copyStringsArr(this->house, DefaultHouse, DefaultHouseDimensions::Rows); }
		HouseDescription(const string& _fileName) : fileName(_fileName) { this->changeAccordingToFile(); setOffset(); }
		
		~HouseDescription() { if (this->valid) { for (int i = 0; i < this->ySize; i++) delete[]this->house[i]; delete[]this->house; } }
		
		void changeAccordingToFile();
		void putValid(Validation v) { this->validationMap[v] = true; }
		map<Validation, bool>& getValid() { return validationMap; }
		string getFileName() { return this->fileName; }
		void setFileName(const string& fileName) 
		{
			this->fileName = fileName;
			this->changeAccordingToFile();
		}
		
		char** getHouse() { return this->house; }
		int getXSize() const{return this->xSize;}
		int getYsize() const{	return this->ySize;}
		int getMaxSteps() { return this->maxSteps; }
		
		void setContentOfPoint (Point positionInHouse, char ch) { this->house[positionInHouse.getY()][positionInHouse.getX()] = ch; }
		bool isWall(int xCoord, int yCoord) const{return this->house[yCoord][xCoord] == SpecialPointsInHouse::Wall;}
		bool isDockingStation(int xCoord, int yCoord) const{return this->house[yCoord][xCoord] == SpecialPointsInHouse::Docking;}
		char getContentOfPoint(Point positionInHouse) const{return this->house[positionInHouse.getY()][positionInHouse.getX()];}
		Point getDockingStationPosition() const;
		bool areHouseDimentionsValid() const{return this->xSize >= houseDimentionsLimits::MinCols && this->xSize <= houseDimentionsLimits::MaxCols && this->ySize >= houseDimentionsLimits::MinRows && this->ySize <= houseDimentionsLimits::MaxRows;}
		int countDockingAndDust();
		bool putWallAroundHouse();
		void print() const;
		void validAndCopy();
		void HouseDescription::copy2House();
		bool getValidBool() { return valid; }
		int getDustAtBegining() { return dustAtBegining; }
		Point getOffset() { return this->offset; }
		void setOffset();

	};

 #endif

