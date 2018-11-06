#include<iostream>
#include"TcpClient.h"

void SetConfig() {
}

int main() {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Initial network error" << std::endl;
	}

	TcpClient client;
	client.LoadConfig("config.txt");
	client.Connect();

	TestStruct test;
	test.aaa = 1;
	test.bbb = 'a';

	client.SendAwaitAndRecv(TestStructMsg,test);

	system("pause");
	WSACleanup();
	return 0;
}