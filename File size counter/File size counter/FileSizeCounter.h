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
	//���������� ������ ������������� ������
	std::list<std::string>* CheckNameList(std::list<FSTATUS>* fdata);
	//������� ����� ������ �� ����� NameList.txt
	std::list<std::string>* GetNameListFiles();
	//���������� ������������� ����� � ����
	void WriteMissingFiles(std::list<std::string> files);
	//��������� ������� ����� � ����������

	bool CheckF(std::string fname, std::list<FSTATUS>* fdata);
	//��������� ������ params �� �������
	bool CheckP(std::list<FSTATUS>* files);
	//��������������� ������� ��� CheckP. 
	bool HelpForCheckP(long &lt,int count,std::list<FSTATUS>* fdata);
	//���� ������ ���������� ������������ ���������. ����� ��� ������� ���������
	bool IsMaxSize(int size, int count, std::list<FSTATUS>* fdata);

	//� �������� ��� ������� ����� GetScan ����� ������� ����������. ��� �� �� ��������� ��������.
	//��� ������ ����� ������ ������� GetScan. �� ��� �� �� ������ ���� ��� ��� f-n ����������, �������� �� public. 


	std::list<FDATA>* FindFilesByMask(std::string dir, std::string mask="");
	std::list<FSTATUS>* GetGroupBySizeFiles(std::string dir,std::string mask="");


	std::list<FSTATUS>* GetSortedData(std::string dir, std::string mask="");

	int CountFilesFromDir(std::string dir,std::string mask = "");

	void GetScan();

	FileSizeCounter();
	~FileSizeCounter();
};