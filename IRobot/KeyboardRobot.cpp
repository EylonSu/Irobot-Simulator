#include <iostream>
using namespace std;

#include "Direction.h"
#include "DirectionExt.h"
#include "KeyboardRobot.h"
char KeyboardRobot::getLastPressedChar()
{
 	if (this->lastPressedChar != SpecialKeys::ESC_KEY)
 		return this->lastPressedChar;
 	else
 	{
 		this->lastPressedChar = SpecialKeys::STAY_KEY1;
 		return SpecialKeys::ESC_KEY;
 	}
}

Direction KeyboardRobot::getNextStepDirection()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		if (ch == SpecialKeys::DOWN_KEY1 || ch == SpecialKeys::DOWN_KEY2 || ch == SpecialKeys::LEFT_KEY1 || ch == SpecialKeys::LEFT_KEY2 || ch == SpecialKeys::RIGHT_KEY1 || ch == SpecialKeys::RIGHT_KEY2 || ch == SpecialKeys::UP_KEY1 || ch == SpecialKeys::UP_KEY2 || ch == SpecialKeys::STAY_KEY1 || ch == SpecialKeys::STAY_KEY2 || ch == SpecialKeys::ESC_KEY)
			this->lastPressedChar = ch;
	}
	switch (this->lastPressedChar)
	{
	case SpecialKeys::UP_KEY1:
	case SpecialKeys::UP_KEY2:
		if (this->sensor->sense().isWall[(int)Direction::North])
			this->robotDirection = Direction::Stay;
		else
			this->robotDirection=Direction::North;
		break;
	case SpecialKeys::RIGHT_KEY1:
	case SpecialKeys::RIGHT_KEY2:
		if (this->sensor->sense().isWall[(int)Direction::East])
			this->robotDirection = Direction::Stay;
		else
			this->robotDirection = Direction::East;
		break;
	case SpecialKeys::DOWN_KEY1:
	case SpecialKeys::DOWN_KEY2:
		if (this->sensor->sense().isWall[(int)Direction::South])
			this->robotDirection = Direction::Stay;
		else
			this->robotDirection = Direction::South;
		break;
	case SpecialKeys::LEFT_KEY1:
	case SpecialKeys::LEFT_KEY2:
		if (this->sensor->sense().isWall[(int)Direction::West])
			this->robotDirection = Direction::Stay;
		else
			this->robotDirection = Direction::West;
		break;
	case SpecialKeys::STAY_KEY1:
	case SpecialKeys::STAY_KEY2:
	case SpecialKeys::ESC_KEY:
	default:
		this->robotDirection = Direction::Stay;
	}
	
	return this->robotDirection;
}