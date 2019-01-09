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
	char * mess = new char[1024];

	switch (packet)
	{
	case NewUser:
		//recv(sock,mess,1024,NULL);
		//std::cout << *mess << std::endl;

	
	case MsgText:
		recv(sock, mess, 1024, NULL);
		for (auto v : Users) {
			Send(v.handler, MsgText, mess,1024);
		}


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

		User us;
		us.handler = conn;
		Users.push_back(us);

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
			auto user = std::find_if(Users.begin(), Users.end(), [&](User u) {
			    return sock == u.handler ? true : false;
			});

			if (user != Users.end()) {
				Users.erase(user);
				std::cout << "Client disconnected!" << std::endl;
			}
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
