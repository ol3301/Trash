#include<iostream>
#include"TcpServer.h"


void loop() {
	std::string s;
	std::cin >> s;
	loop();
}

int main() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Initial network error" << std::endl;
	}

	TcpServer server;

	server.StartListening(2020);


	loop();
	WSACleanup();
    return 0;
}
