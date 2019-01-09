#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include "D:\Projects\с++\Сети\TCP_CLIENT_SERVER\Components.h"
#pragma warning(disable: 4996)

class TcpClient
{
private:
	SOCKET handler;
	SOCKADDR_IN addr;
	bool isListen = false;
	int sizeofaddr;

	std::string ip;
	int port;

	int GetMsgSize();
	bool ProcessPacket(PacketType packet);
public:
	bool Connect(const std::string ip,const int port);
	bool Connect();
	void Listen();
	void StartListening();
	bool SwitchToNonBlocking();
	bool SwitchToBlocking();
	void LoadConfig(std::string configfile);
	bool ParsConfig(std::string config);
	template < typename T >
	void Send(PacketType p, T o, int size);

	template < typename T >
	void SendAwaitAndRecv(PacketType packet, T msg);

	TcpClient();
	~TcpClient();
};

template<typename T>
void TcpClient::Send(PacketType packet, T msg,int size)
{
	send(handler, (char*)&packet, sizeof(PacketType),NULL);
	send(handler, msg, size,NULL);
}

template<typename T>
void TcpClient::SendAwaitAndRecv(PacketType packet, T msg)
{
	PacketType recv_packet;

	Send(packet,msg);

	timeval time;
	//время ожидания в секундах
	time.tv_sec = 60;//
	time.tv_usec = 0;

	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(handler, &readset);

	if (!SwitchToNonBlocking())
		return;

	//первый параметр нужен что бы указать сколько сокетов ожидают события(наверное)
	if (select(1, &readset, NULL, NULL, &time) <= 0) {
		std::cout << "timeout error" << std::endl;
		return;
	}

	int size = recv(handler,(char*)&recv_packet,sizeof(PacketType),NULL);

	SwitchToBlocking();

	FD_CLR(handler, &readset);

	ProcessPacket(recv_packet);
}
