#ifndef _SENSOR_H_
#define _SENSOR_H_

#ifdef WINDOWS

#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
	
#endif
	
#include <iostream>
#include <stdio.h>
	

using namespace std;

#include "AbstractSensor.h"
#include "Config.h"
#include "Defs.h"
#include "Point.h"
#include "SpecialFunctions.h"
#include "SensorInformation.h"

	class Simulator;

	class Sensor : public AbstractSensor
	{
	private:
		Simulator* simulator;
	 	public:
	 		Sensor() {}
	 		Sensor(Simulator* simulator)
	 		{
	 			this->simulator = simulator;
	 		}
	 		
			SensorInformation sense() const;
			void setSimulator(Simulator* _sim) { this->simulator = _sim; }
	};
	
#endif