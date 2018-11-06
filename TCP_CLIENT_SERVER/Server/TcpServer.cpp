#include "TcpServer.h"
#include <thread>


int TcpServer::GetMsgSize()
{
	int msg_size;
	recv(handler, (char*)&msg_size, sizeof(int), NULL);
	return msg_size;
}

bool TcpServer::ProcessPacket(SOCKET sock,PacketType packet)
{
	switch (packet)
	{
	case MsgText:
		break;
	case TestStructMsg:
		TestStruct test;
		recv(sock,(char*)&test,sizeof(TestStruct),NULL);

		std::cout << "Received packet \n" + std::to_string(test.aaa) + "\n" + test.bbb << std::endl;

		++test.aaa;
		test.bbb = 'i';

		Send(sock,TestStructMsg,test);
		break;
	default:
		break;
	}
	return false;
}

bool TcpServer::StartListening(const int port) {
	if (isListen)
		return false;
	
	this->port = port;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(handler, (SOCKADDR*)&addr, sizeof(addr)) != NULL) {
		std::cout << "Bind's socket error" << std::endl;
		return false;
	}
	if (listen(handler, SOMAXCONN) != NULL) {
		std::cout << "Listen's socket error" << std::endl;
		return false;
	}

	isListen = true;

	std::thread th(&TcpServer::Listen,this);
	th.detach();

	return true;
}

void TcpServer::Listen()
{
	SOCKET conn;
	int _sizeaddr = sizeof(addr);
	while (isListen)
	{
		conn = accept(handler,(SOCKADDR*)&addr,&_sizeaddr);
		sockets.insert(conn);

		std::cout << "Connected client!" << std::endl;
		std::thread th(&TcpServer::Socketlisten,this,conn);
		th.detach();
	}
}

void TcpServer::Socketlisten(SOCKET sock)
{
	PacketType packet;
	while (true) {
		if (recv(sock, (char*)&packet, sizeof(PacketType), NULL) > 0) {
			ProcessPacket(sock, packet);
		}
		else {
			sockets.erase(sock);
			std::cout << "Client disconnected!" << std::endl;
			break;
		}
	}
}

TcpServer::TcpServer()
{
	sizeofaddr = sizeof(addr);
	handler = socket(AF_INET, SOCK_STREAM, NULL);
	if (handler == INVALID_SOCKET) {
		std::cout << "created error " << std::endl;
	}
}


TcpServer::~TcpServer()
{

}
