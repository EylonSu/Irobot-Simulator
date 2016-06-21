#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <string>

using namespace std;

#include "AbstractAlgorithm.h"

class AlgorithmRegistrar {
	vector<std::string> algorithmNames;
	vector<std::function<unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
	
	void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory, const string& name) {
		algorithmFactories.push_back(algorithmFactory);
		algorithmNames.push_back(name);
	}
public:
	friend struct AlgorithmRegistration;
	vector<unique_ptr<AbstractAlgorithm>> getAlgorithms()const 
	{
		vector<unique_ptr<AbstractAlgorithm>> algorithms;
		for (auto algorithmFactoryFunc : algorithmFactories) 
		{
			algorithms.push_back(algorithmFactoryFunc());
		}
		return algorithms;
	}
	const vector<std::string>& getAlgorithmNames()const {
		return algorithmNames;
	}
	size_t size()const {
		return algorithmFactories.size();
	}
	static AlgorithmRegistrar& getInstance() {
		static AlgorithmRegistrar instance;
		return instance;
	}
};
