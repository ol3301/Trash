#pragma once
#include <string>
#include <list>
#include "Models.h"

class ConfigHelper
{
public:
	std::list<int> SplitParams(std::string params, char delim);
	long MaxSize(std::list<FSTATUS>* fdata);
	long MaxSizeCount(std::list<FDATA> fdata, int count, long &lt);
	long MaxSize(std::list<FDATA> fdata, int but);
	long MinSize(std::list<FDATA> mins);
	int GetSizeStream(std::fstream fs);
	FCONFIG* LoadConfig(std::string path);
	std::string GetFileName(std::string name);
	std::string GetMeFolder();
	ConfigHelper();
	~ConfigHelper();
};