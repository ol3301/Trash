#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#pragma warning(disable: 4996)

enum PacketType {
	MsgText,
	TestStructMsg
};

struct TestStruct {
	int aaa;
	char bbb;
};

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
	void Send(PacketType p, T o);

	template < typename T >
	void SendAwaitAndRecv(PacketType packet, T msg);

	TcpClient();
	~TcpClient();
};

template<typename T>
inline void TcpClient::Send(PacketType packet, T msg)
{
	send(handler, (char*)&packet, sizeof(PacketType),NULL);
	send(handler, (char*)&msg, sizeof(T),NULL);
}

template<typename T>
inline void TcpClient::SendAwaitAndRecv(PacketType packet, T msg)
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
