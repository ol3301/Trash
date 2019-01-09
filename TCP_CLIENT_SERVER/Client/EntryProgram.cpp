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

	while (true)
	{
		TcpClient client;
		client.LoadConfig("config.txt");
		client.Connect();
		char * mess = new char[1024];
		mess = (char*)"Hello world!";
		client.Send(MsgText,mess,1024);

		system("pause");
	}

	WSACleanup();
	return 0;
}