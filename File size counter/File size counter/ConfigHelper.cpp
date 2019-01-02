#include "ConfigHelper.h"
#include <sstream>
#include "DEBUGGING.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <algorithm>



std::list<int> ConfigHelper::SplitParams(std::string params, char delim)
{
	std::stringstream si(params);
	std::string item="";
	std::list<int> area;

	while (std::getline(si, item, delim)) {
		area.push_back(atoi(item.c_str()));
	}
	return area;
}

long ConfigHelper::MaxSize(std::list<FSTATUS>* fdata)
{
	long max = 0;
	for (auto fs : *fdata) {
		for (auto f : fs.files) {
			if (f.size > max) {
				max = f.size;
			}
		}
	}
	return max;
}

long ConfigHelper::MaxSizeCount(std::list<FDATA> fdata,int count,long& lt)
{
	long max = MaxSize(fdata,lt);

	if (max == 0) {
		return 0;
	}

	long real_count= std::count_if(fdata.begin(), fdata.end(), [&](FDATA fd) {
		return (fd.size == max);
	});

	lt = max;

	if (real_count != count) {
		return MaxSizeCount(fdata,count,lt);
	}

	return count;
}

long ConfigHelper::MaxSize(std::list<FDATA> fdata, int but)
{
	int max = 0;
	for (auto fd : fdata) {
		if (fd.size > max && fd.size < but) {
			max = fd.size;
		}
	}
	return max;
}

long ConfigHelper::MinSize(std::list<FDATA> mins)
{
	long min = 10000000;

	for (auto i : mins) {
		if (i.size < min && i.size > 0) {
			min = i.size;
		}
	}
	return min;
}

int ConfigHelper::GetSizeStream(std::fstream fs)
{
	fs.seekg(0, fs.end);
	int size = fs.tellg();
	fs.seekg(0,fs.beg);

	return size;
}

FCONFIG * ConfigHelper::LoadConfig(std::string path)
{
	FCONFIG* res = new FCONFIG();
	std::ifstream fs(path);

	if (!fs.is_open()) {
		WRITE_LOG("Open config file error");
		return nullptr;
	}

	char *mask = new char[256];
	fs.getline(mask, 256);

	char *offset = new char[256];
	fs.getline(offset, 256);

	char *params = new char[256];
	fs.getline(params, 256);


	res->mask = (std::string)mask;
	res->offset = atoi(offset);
	res->params = SplitParams((std::string)params,' ');

	//необходимо для правильного сравнения
	std::reverse(res->params.begin(), res->params.end());

	delete[] mask;
	delete[] offset;
	delete[] params;
	fs.close();

	return res;
}

std::string ConfigHelper::GetFileName(std::string name)
{
	int pos = name.rfind('.');
	if (pos <= 0)
		return "";

	name.erase(pos,name.length() - pos);
	return name;
}

std::string ConfigHelper::GetMeFolder()
{
	TCHAR buf[MAX_PATH];
	GetCurrentDirectory(sizeof(buf),buf);

	return (std::string)buf;
}

ConfigHelper::ConfigHelper()
{
}

ConfigHelper::~ConfigHelper()
{
}
