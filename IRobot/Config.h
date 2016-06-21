#pragma once

#define WINDOWS

#include <map>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

#include "SpecialFunctions.h"
#include "Battery.h"

using namespace std;

enum CONFIG_RQST { RQST_SOUNDS, RQST_BATTERY_CAP, RQST_BATTERY_CONSUMP, RQST_BATTERY_RECH, RQST_MAX_STEPS_AFTER };
	class Config
	{
	private:
		friend class Simulator;
		int MaxStepsAfterWinner=200,
			BatteryCapacity=400,
			BatteryConsumptionRate=1,
			BatteryRechargeRate=20;
		bool sounds = true,
			configCtorFlag = true;
		map<string, int> configMap;
		void resetMap();

		//colors settings
#define HEADER_COLOR Color::CYAN
#define ROBOT_COLOR Color::RED
#define BOTTOM_COLOR Color::GREEN
#define WALLS_COLOR Color::LIGHTBLUE
#define DIRT_COLOR Color::DARKGREY
#define LOW_BATTERY_COLOR Color::RED
#define CHARGED_BATTERY_COLOR Color::GREEN

#define BLINK_SPEED 2
#define CONFIG_FILE_NAME "config.ini"
#define CONFIG_AMOUNT 4
#define BAD_VALUE -1
		
	public:
		Config();
		
		map<string, int> getConfigMap() { return this->configMap; }
		void setBatteryCapacity(int MaxSteps) { this->BatteryCapacity = BatteryCapacity; }
		void setBatteryConsumptionRate(int BatteryConsumptionRate) { this->BatteryConsumptionRate = BatteryConsumptionRate; }
		void setBatteryRechargeRate(int BatteryRechargeRate) { this->BatteryRechargeRate = BatteryRechargeRate; }
		int getBatteryCapacity() { return this->BatteryCapacity; }
		int getBatteryConsumptionRate() { return this->BatteryConsumptionRate; }
		int getBatteryRechargeRate() { return this->BatteryRechargeRate; }
		bool getSounds() { return this->sounds; }
		void toggleSounds() { this->sounds = (!sounds); }
		int getMaxStepsAfterWinner() { return MaxStepsAfterWinner; }
	};