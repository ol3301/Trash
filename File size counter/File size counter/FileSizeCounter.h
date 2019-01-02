#pragma once
#include "ConfigHelper.h"

#include <string>
#include <list>
#include "DEBUGGING.h"
#include <iostream>
#include <fstream>

#include "Models.h"

class FileSizeCounter
{
private:
	const std::string configfilename = "Config.txt";
	const std::string namelist = "NameList.txt";
	const std::string missingfiles = "MissingFiles.txt";

	FCONFIG *config;
	ConfigHelper ch;
	void AddItem(FDATA fdata, std::list<FSTATUS>* area);

public:
	//возвращает список отсутствующих файлов
	std::list<std::string>* CheckNameList(std::list<FSTATUS>* fdata);
	//достает имена файлов из файла NameList.txt
	std::list<std::string>* GetNameListFiles();
	//записывает отсутствующие файлы в файл
	void WriteMissingFiles(std::list<std::string> files);
	//проверяет наличие файла в контейнере

	bool CheckF(std::string fname, std::list<FSTATUS>* fdata);
	//проверяет логику params из конфига
	bool CheckP(std::list<FSTATUS>* files);
	//вспомагательная функция для CheckP. 
	bool HelpForCheckP(long &lt,int count,std::list<FSTATUS>* fdata);
	//ищем нужное количество максимальных елементов. Нужно для первого вхождения
	bool IsMaxSize(int size, int count, std::list<FSTATUS>* fdata);

	//в принципе все функции кроме GetScan можно сделать приватными. Что бы не отвлекать внемание.
	//Всю работу будет делать функция GetScan. Но все же на случай если вам эти f-n пригодятся, оставляю их public. 


	std::list<FDATA>* FindFilesByMask(std::string dir, std::string mask="");
	std::list<FSTATUS>* GetGroupBySizeFiles(std::string dir,std::string mask="");


	std::list<FSTATUS>* GetSortedData(std::string dir, std::string mask="");

	int CountFilesFromDir(std::string dir,std::string mask = "");

	void GetScan();

	FileSizeCounter();
	~FileSizeCounter();
};