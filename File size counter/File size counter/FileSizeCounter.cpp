#include "FileSizeCounter.h"
#include <Windows.h>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>

#pragma warning(disable:4996)

void FileSizeCounter::AddItem(FDATA fdata,std::list<FSTATUS>* area)
{
	for (auto i = area->begin(); i != area->end(); ++i) {
		if (i->size == fdata.size) {
			i->files.push_front(fdata);
			return;
		}
	}

	FSTATUS fs;
	fs.size = fdata.size;
	fs.files.push_front(fdata);

	area->push_front(fs);
}

std::list<std::string>* FileSizeCounter::CheckNameList(std::list<FSTATUS>* fdata)
{
	std::list<std::string> *namelist = GetNameListFiles();
	std::list<std::string> *missing = new std::list<std::string>();

	if (namelist == nullptr) {
		return missing;
	}

	for (auto fname : *namelist) {
		if (!CheckF(fname, fdata)) {
			missing->push_back(fname);
		}
	}

	return missing;
}

std::list<std::string>* FileSizeCounter::GetNameListFiles()
{
	std::ifstream f(namelist);

	if (!f.is_open()) {
		WRITE_LOG("Invalid open namelist file");
		return nullptr;
	}

	std::list<std::string>* list = new std::list<std::string>();

	std::string fname;

	while (std::getline(f, fname)) {
		list->push_back(fname);
	}

	f.close();
	return list;
}

void FileSizeCounter::WriteMissingFiles(std::list<std::string> files)
{
	std::ofstream o(missingfiles);

	if (!o.is_open()) {
		WRITE_LOG("Invalid create missing files");
		return;
	}
	
	for (auto i : files) {
		o << i + "\n";
	}

	o.close();
}

bool FileSizeCounter::CheckF(std::string fname, std::list<FSTATUS>* fdata)
{
	for (auto fst : *fdata) {
		for (auto fl : fst.files) {
			//с помощью функции GetFileName отсекаем разширение у файла.
			if (ch.GetFileName(fl.filename) == fname) {
				return true;
			}
		}
	}
	return false;
}

bool FileSizeCounter::CheckP(std::list<FSTATUS>* files)
{
	std::list<int> params = config->params;
	int count = params.back();
	params.pop_back();
	long max = ch.MaxSize(files);

	if (!IsMaxSize(max, count, files))
		return false;

	if (params.size() <= 0) {
		return true;
	}

	for (int i = 0; i <= params.size(); ++i) {
		count = params.back();
		params.pop_back();

		if (!HelpForCheckP(max, count, files)) {
			return false;
		}
	}
	
	return true;
}

bool FileSizeCounter::HelpForCheckP(long &lt, int count, std::list<FSTATUS>* fdata)
{
	std::list<FDATA> mins;
	for (auto fs : *fdata) {
		for (auto it : fs.files) {
			if (it.size < lt) {
				mins.push_back(it);
			}
		}
	}


	if (ch.MaxSizeCount(mins, count, lt) != count) {
		return false;
	}

	return true;
}

bool FileSizeCounter::IsMaxSize(int size,int count, std::list<FSTATUS>* fdata)
{
	int real_count = 0;
	for (auto fs : *fdata) {
		if (fs.size == size) {
			real_count += fs.files.size();
		}
	}	

	return (real_count == count);
}

std::list<FDATA>* FileSizeCounter::FindFilesByMask(std::string dir, std::string mask)
{
	std::list<FDATA>* res = new std::list<FDATA>();
	WIN32_FIND_DATA fdata;
	std::string path = "";

	if (mask != "") {
		path = dir + "\\" + mask;
	}
	else {
		path = dir + "\\" + config->mask;
	}

	HANDLE file;

	file = FindFirstFile(path.c_str(),&fdata);

	if (file == INVALID_HANDLE_VALUE) {
		WRITE_LOG("Invalid process finding file by mask");
		delete res;
		return nullptr;
	}

	do {
		FDATA fd;
		fd.filename = fdata.cFileName;

		if (fd.filename == "." || fd.filename == ".." || (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			continue;

		fd.size = fdata.nFileSizeLow;

		res->push_front(fd);
	} while (FindNextFile(file, &fdata));

	FindClose(file);

	return res;
}

std::list<FSTATUS>* FileSizeCounter::GetGroupBySizeFiles(std::string dir, std::string mask)
{
	std::list<FDATA>* files = FindFilesByMask(dir,mask);

	if (files == nullptr) {
		WRITE_LOG("FindFilesByMask return's nullptr");
		return nullptr;
	}

	std::list<FSTATUS>* groups = new std::list<FSTATUS>();

	for (auto it : *files) {
		AddItem(it,groups);
	}

	delete files;

	return groups;
}

void FileSizeCounter::GetScan()
{
	//std::string folder = ch.GetMeFolder();
	std::string folder = "C:\\Users\\Mazda\\Desktop\\test";

	std::list<FSTATUS>* sdata = GetSortedData(folder);

	if (sdata == nullptr) {
		return;
	}

	if (config->offset == 1) {
		auto checks = CheckNameList(sdata);

		if (checks->size() != 0) {
			WRITE_LOG("ERROR: Missing Files");
			WriteMissingFiles(*checks);
		}		
	
		delete checks;
	}

	if (config->params.size() > 0) {
		if (!CheckP(sdata)) {
			WRITE_LOG("ERROR: PARAMS INVALID");

			for (auto i : *sdata) {
				std::cout <<"byte: "+ std::to_string(i.size) + ": files " + std::to_string(i.files.size()) << std::endl;
			}

			return;
			delete sdata;
		}
	}

	delete sdata;
}

std::list<FSTATUS>* FileSizeCounter::GetSortedData(std::string dir, std::string mask)
{
	auto groups = GetGroupBySizeFiles(dir, mask);

	if (groups == nullptr) {
		WRITE_LOG("GetGroupBySizeFiles return's nullptr");
		return nullptr;
	}

	groups->sort([](FSTATUS f1,FSTATUS f2) {
		return (f1.size > f2.size);
	});

	return groups;
}

int FileSizeCounter::CountFilesFromDir(std::string dir, std::string mask)
{
	WIN32_FIND_DATA fdata;
	HANDLE file;
	
	std::string path="";

	if (mask != "") {
		path = dir + "\\" + mask;
	}
	else {
		path = dir + "\\" + config->mask;
	}

	file = FindFirstFile(dir.c_str(), &fdata);


	if (file == INVALID_HANDLE_VALUE) {
		return 0;
	}

	int counter = 0;

	do {
		std::string str = fdata.cFileName;

		if (str == "." || str == "..")
			continue;

		++counter;

	} while (FindNextFile(file, &fdata));

	FindClose(file);

	return counter;
}

FileSizeCounter::FileSizeCounter()
{
	config = ch.LoadConfig(configfilename);
}

FileSizeCounter::~FileSizeCounter()
{
	delete config;
}
