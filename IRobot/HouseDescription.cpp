#include "HouseDescription.h"
using namespace std;
Point HouseDescription::getDockingStationPosition() const
{
 	for (int i = 1; i < this->ySize - 1; i++)
 	{
 		for (int j = 1; j < this->xSize - 1; j++)
 		{
 			if (isDockingStation(j, i))
 				return Point(j, i);
 		}
 	}
 	return NO_POINT_FOUND;
 }

int HouseDescription::countDockingAndDust()
{
	int dockingCounter = 0;
	int dustCounter = 0;
 	int i = 1;
 	while (i < this->ySize - 1 && dockingCounter<2)
 	{
 		int j = 1;
 		while (j < this->xSize - 1 && dockingCounter<2)
 		{
 			if (this->house[i][j] == SpecialPointsInHouse::Docking)
 				dockingCounter++;
			if (this->house[i][j] >= '1' && this->house[i][j] <= '9')
				dustCounter += this->house[i][j] - '0';
 			j++;
 		}
 		i++;
 	}
	this->dustAtBegining = dustCounter;
	return dockingCounter;
}

bool HouseDescription::putWallAroundHouse()
{
 	bool res = true;
 	for (int i = 0; i < this->xSize; i++)
 	{
 		if (this->house[0][i] != 'W')
 		{
 			this->house[0][i] = 'W';
 			res = false;
 		}
 		if (this->house[this->ySize - 1][i] != 'W')
 		{
 			this->house[this->ySize - 1][i] = 'W';
 			res = false;
 		}
 	}
 	for (int i = 1; i < this->ySize - 1; i++)
 	{
 		if (this->house[i][0] != 'W')
 		{
 			this->house[i][0] = 'W';
 			res = false;
 		}
 		if (this->house[i][this->xSize - 1] != 'W')
 		{
 			this->house[i][this->xSize - 1] = 'W';
 			res = false;
 		}
 	}
	return res;
}

void HouseDescription::print() const
{
 	for (int i = 0; i < this->ySize; i++)
 	{
 		gotoxy(0, i);
 		for (int j = 0; j < this->xSize; j++)
 			cout << this->house[i][j];
 	}
 	cout << endl;
}

void HouseDescription::copy2House()
{
	ifstream inFile(this->fileName);
	char buff[120];
	inFile.getline(buff, 120);
	inFile.getline(buff, 120);
	inFile.getline(buff, 120);
	inFile.getline(buff, 120);
	this->house = new char*[this->ySize];
	for (int i = 0; i < this->ySize; i++)
	{
		this->house[i] = new char[this->xSize + 1];
		inFile.getline(buff, 120);
		if (strlen(buff) < xSize)
			fill(buff+ strlen(buff), buff+xSize, ' ');
		strncpy_s(this->house[i], this->xSize + 1, buff, this->xSize);
	}
	inFile.close();
}

void HouseDescription::validAndCopy()
{
	copy2House();
	
	//docking station
	if (this->countDockingAndDust() == 0)
	{
		this->putValid(NO_DOCKING);
		valid = false;
	}
	else if (this->countDockingAndDust() > 1)
	{
		this->putValid(Validation::MULTI_DOCKING);
		valid = false;
	}
	
	if (!putWallAroundHouse())
	{
		this->putValid(Validation::PERIPH_ADD);
		this->putValid(Validation::VALID);
		valid = true;
	}
	if (valid)
		this->putValid(Validation::VALID);
}

void HouseDescription::changeAccordingToFile()
{
 	ifstream inFile(this->fileName);
 	char buff[120];
 	
	inFile.getline(buff, 120);
 	
	inFile.getline(buff, 120);
 	this->maxSteps = atoi(buff);
	if (maxSteps == 0) {
		this->validationMap[Validation::MAXSTEPS] = true;
		valid = false;
	}
 	
	inFile.getline(buff, 120);
 	this->ySize = atoi(buff);
	if (ySize == 0)
	{
		this->validationMap[Validation::INVALID_DIMENTIONS]=true;
		valid = false;
	}
 
	inFile.getline(buff, 120);
 	this->xSize = atoi(buff);
	if (xSize == 0)
	{
		this->validationMap[Validation::INVALID_DIMENTIONS]=true;
		valid = false;
	}
	inFile.close();
	
	if (valid) validAndCopy();
}

void HouseDescription::setOffset()
{
	Point p = getDockingStationPosition();
	this->offset = this->DockingStationPositionOnScreen.distance(p);
}