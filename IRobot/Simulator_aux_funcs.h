#pragma once

using namespace std;
#include <iostream>
#include "Simulator.h"
#include "Direction.h"
#include "DirectionExt.h"

struct SensorInformation;


bool isStepValid(Direction direction, const SensorInformation& sensorInformation);

void validateSave(string& fileName);

inline bool isFileExists(const string& name);

bool isBetterSol(const string& name, int steps);

bool isFirstRow(char* buffer);

void duplicateElement(vector<string>& actionVector, int sc);

int getNextSC(char* buffer);

int getHouseNum(string fileName);