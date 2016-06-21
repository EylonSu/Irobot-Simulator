#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "Config.h"

class Simulator;
	class Battery
	{
	private:
		int BatteryCapacity;
		int BatteryConsumptionRate;
		int BatteryRechargeRate;
		int currentBatteryLoad;
		Simulator* simulator;
	public:
		Battery() { simulator = NULL; }
		Battery(Simulator* simulator);
		
		int getBatteryCapacity() { return this->BatteryCapacity; }
		int getBatteryConsumptionRate() { return this->BatteryConsumptionRate; }
		int getBatteryRechargeRate() { return this->BatteryRechargeRate; }
		int getCurrentBatteryLoad() { return this->currentBatteryLoad; }
		void setBatteryCapacity(int num) { BatteryCapacity = num; }
		void setBatteryConsumptionRate(int num) { BatteryConsumptionRate = num; }
		void setBatteryRechargeRate(int num) { BatteryRechargeRate = num; }
		void setCurrentBatteryLoad(int num) { currentBatteryLoad = num; }

		void Battery::initBySimulator();
		void setSimulator(Simulator* sim) { simulator = sim; initBySimulator(); }
		void Consume();
		void Charge();
	};
	
#endif

