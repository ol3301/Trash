#pragma once
#include <string>
#include <list>

struct FCONFIG {
	std::string mask = "";
	int offset = 0;
	std::list<int> params;
};

struct FDATA
{
	std::string filename;
	int size;
};

struct FSTATUS {
	long size;
	std::list<FDATA> files;
};
