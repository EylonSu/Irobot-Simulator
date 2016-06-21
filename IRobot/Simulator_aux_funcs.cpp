#include "Simulator_aux_funcs.h"
using namespace std;
bool isStepValid(Direction direction, const SensorInformation& sensorInformation)
{
	return (direction == Direction::North && !(sensorInformation.isWall[0])) || (direction == Direction::East && !(sensorInformation.isWall[1])) || (direction == Direction::South && !(sensorInformation.isWall[2])) || (direction == Direction::West && !(sensorInformation.isWall[3]) || (direction == Direction::Stay));
}

void validateSave(string& fileName)
{
	ifstream file(fileName);
	while (file.good())
	{
		string newName;
		if ((messageBox(Confirmation, There is already a file in the name you requested.\nDo you want to override it ? , MB_YESNO)) == IDYES)
			return;
		else
		{
			cout << "Please enter the new name:\n";
			cin >> newName;
			fileName.erase(4, fileName.find(".house"));
			newName += ".hou";
			fileName.insert(4, newName);
		}
		file.close();
		file.open(fileName);
	}
	return;
}

inline bool isFileExists(const string& name) {
	ifstream f(name.c_str());
	return f.good();
	//file closed by destructor
}

bool isBetterSol(const string& name, int steps)
{
	if (!isFileExists(name))
		return false;//file does'nt exists...

	ifstream file(name);

	char line[128];

	file.getline(line, '\n');
	if (steps < atoi(line))
		return false;
	else
		return true;

	//file closed by destructor
}

bool isFirstRow(char* buffer)
{
	if (buffer[0] == '1')
		return true;
	else
		return false;
}

void duplicateElement(vector<string>& actionVector, int sc)
{
	string str = actionVector.back();
	int scEndLoc = str.find_first_of(':');
	str.erase(0, scEndLoc);
	str.insert(0, to_string(sc + 1));
	actionVector.push_back(str);
}

int getNextSC(char* buffer)
{
	if (strcmp(buffer, "") == 0)
		return 0;
	char sc[8];
	int count = strchr(buffer, ':') - buffer;
	strncpy_s(sc, buffer, count);
	int res = atoi(sc);
	return res;
}

int getHouseNum(string fileName)
{
	return atoi(fileName.c_str());
}