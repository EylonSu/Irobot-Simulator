#ifndef _KEYBOARD_ROBOT_H_
#define _KEYBOARD_ROBOT_H_

	#ifdef WINDOWS

#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>

#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "Sensor.h"	
#include "Point.h"
#include "Defs.h"
#include "Direction.h"
#include "DirectionExt.h"

	class Sensor;
	class Battery;
	
	class KeyboardRobot
	{
	private:
		Direction robotDirection = Direction::Stay;
		const Sensor* sensor;
		char lastPressedChar = SpecialKeys::STAY_KEY1;
		Battery* battery;
	public:
		KeyboardRobot() {}//ctor
	
		void setLastPressedChar(char ch) { if (ch == SpecialKeys::DOWN_KEY1 || ch == SpecialKeys::DOWN_KEY2 || ch == SpecialKeys::LEFT_KEY1 || ch == SpecialKeys::LEFT_KEY2 || ch == SpecialKeys::RIGHT_KEY1 || ch == SpecialKeys::RIGHT_KEY2 || ch == SpecialKeys::UP_KEY1 || ch == SpecialKeys::UP_KEY2 || ch == SpecialKeys::STAY_KEY1 || ch == SpecialKeys::STAY_KEY2 || ch == SpecialKeys::ESC_KEY) this->lastPressedChar = ch; }
		void setSensor(const Sensor& sensor) { this->sensor = &sensor; }//recievs the sensor to keyboardRobot	
		const Sensor* getSensor() { return this->sensor; }
		void setBattery(Battery* battery) { this->battery = battery; }
		Battery* getBattery() { return this->battery; }
		char getLastPressedChar();
		Direction getNextStepDirection();
	};
	
#endif