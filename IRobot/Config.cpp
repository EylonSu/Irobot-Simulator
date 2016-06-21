#include "Config.h"
using namespace std;
bool checkConfigFileExistense()
{
	system("dir /b /x config.ini > tmp.txt");
	ifstream file("tmp.txt");
	const unsigned int config_FN_size = sizeof(CONFIG_FILE_NAME);
	char line[config_FN_size];
	file.getline(line, config_FN_size);
	file.close();
	system("del tmp.txt");
	if (strcmp(line, CONFIG_FILE_NAME) != 0)
		return false;
	else
		return true;
}

bool isNum(char* num)
{
	int len = strlen(num);
	for (int i = 0; i < len; i++)
		if (num[i]<'0' || num[i]>'9')
			return false;
	return true;
}


bool checkKeyValue(char* key, char* value)
{
	string settings[] = { "MaxStepsAfterWinner","BatteryCapacity","BatteryConsumptionRate","BatteryRechargeRate" };
	
	if ((strcmp(key, "") == 0) || key[0]=='\n')
		return true;

	if (!isNum(value))
		return false;
	
	for (string str : settings)
		if (str.compare(key) == 0)
			return true;

	return false;
}

bool extractLine(ifstream& configFile, char* key, int& value)
{
	char Cvalue[10];
	configFile.get(key, 128, '=');
	configFile.get();//for the'='
	configFile.getline(Cvalue, '\n');
	if (!checkKeyValue(key, Cvalue))
	{
		messageBox(Config file problem, config.ini exists but have bad values.\nProgram will now exit., NULL);
		return false;
	}
	value = atoi(Cvalue);
	
	return true;
}

bool getMap(map<string, int>& configMap)
{
	ifstream configFile(CONFIG_FILE_NAME);

	while (!configFile.eof())
	{
		char keys[128]; 
		int value = BAD_VALUE;
		if(!extractLine(configFile, keys, value))
			return false;
		configMap[string(keys)] = value;
	}
	configFile.close();
	return true;
}

void Config::resetMap()
{
	auto& m = this->configMap;
	
	m["MaxStepsAfterWinner"] = 200;
	m["BatteryCapacity"] = 400;
	m["BatteryConsumptionRate"] = 1;
	m["BatteryRechargeRate"] = 20;

	return;
}

Config::Config()
{
	if (!checkConfigFileExistense())//file  dowsn't exists
		return;//set default values
	else
	{
		ifstream configFile(CONFIG_FILE_NAME);
		if (!configFile.good())//file is unreadable
		{
			messageBox(Config file problem, config.ini exists but cannot be opened!\nProgram will now exit., NULL);
			configCtorFlag = false;
			return;
		}
		else//file is good to go..
		{
			this->resetMap();
			
			if (!getMap(this->configMap))
			{
				configCtorFlag = false;
				return;
			}
		}
	}
}