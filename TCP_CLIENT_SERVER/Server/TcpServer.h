#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <mutex>
#include <list>
#include <algorithm>
#include "D:\Projects\с++\Сети\TCP_CLIENT_SERVER\Components.h"

#pragma warning(disable: 4996)

class TcpServer
{
private:
	SOCKET handler;
	SOCKADDR_IN addr;
	std::list<User> Users;

	bool isListen = false;
	int sizeofaddr;
	int port=-1;

	int GetMsgSize();
	bool ProcessPacket(SOCKET sock,PacketType packet);
	void Listen();
	void Socketlisten(SOCKET sock);
public:
	template < typename T >
	void Send(SOCKET sock,PacketType packet, T msg, int size);
	bool StartListening(const int port);
	TcpServer();
	~TcpServer();
};

class UdpServer : public TcpServer {

};

template<typename T>
void TcpServer::Send(SOCKET sock,PacketType packet, T msg,int size)
{
	send(sock,(char*)&packet,sizeof(PacketType),NULL);
	send(sock, msg, size,NULL);
}