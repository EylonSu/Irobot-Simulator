#include "Score.h"
using namespace std;
Score::Score()
{
}

Score::~Score()
{
	delete[] housesScoreData;
}

void Score::calculateAvg()
{
	int validHouses = 0;
	for (int i = 0; i < houseAmount; i++)
	{
		if (this->housesScoreData[i].score != -1)
		{
			validHouses++;
			avg += housesScoreData[i].score;
		}
	}
	avg /= validHouses;

}

void Score::calcHouseScore()
{ 
	housesScoreData[competition->getHouseIndex()].calculate();
}
