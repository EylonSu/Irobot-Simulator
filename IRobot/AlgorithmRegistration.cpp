#include <iostream>
using namespace std;

#include "Direction.h"
#include "DirectionExt.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory, string name)
{
	AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory, name);
}