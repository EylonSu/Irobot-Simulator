#include "Battery.h"
#include "Simulator.h"
using namespace std;
void Battery::initBySimulator()
{
	this->BatteryCapacity = simulator->getConfig(RQST_BATTERY_CAP);
	this->BatteryConsumptionRate = simulator->getConfig(RQST_BATTERY_CONSUMP);
	this->BatteryRechargeRate = simulator->getConfig(RQST_BATTERY_RECH);
	this->currentBatteryLoad = this->BatteryCapacity;
}

Battery::Battery(Simulator* simulator)
{
	this->simulator = simulator;
	initBySimulator();
}

void Battery::Consume()
{
 	if (this->currentBatteryLoad < 0)
 		this->currentBatteryLoad = 0;
 	else
 	{
 		this->currentBatteryLoad -= this->BatteryConsumptionRate;
 		if (this->currentBatteryLoad < 0)
 			this->currentBatteryLoad = 0;
 	}
}

void Battery::Charge()
{
	if (this->currentBatteryLoad > this->BatteryCapacity)
 		this->currentBatteryLoad = this->BatteryCapacity;
 	else
 	{
 		this->currentBatteryLoad += this->BatteryRechargeRate;
 		if (this->currentBatteryLoad > this->BatteryCapacity)
 			this->currentBatteryLoad = this->BatteryCapacity;
 	}
}
