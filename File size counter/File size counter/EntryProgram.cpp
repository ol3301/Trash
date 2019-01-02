#include "FileSizeCounter.h"
#include "ConfigHelper.h"

int main() {
	setlocale(LC_ALL,"Rus");
	 

	FileSizeCounter f;
	f.GetScan();

	system("pause");
	return 0;
}