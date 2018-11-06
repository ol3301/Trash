#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <mutex>
#include <set>
#pragma warning(disable: 4996)

enum PacketType {
	MsgText,
	TestStructMsg
};

struct TestStruct {
	int aaa;
	char bbb;
};


class TcpServer
{
private:
	SOCKET handler;
	SOCKADDR_IN addr;
	std::set<SOCKET> sockets;

	bool isListen = false;
	int sizeofaddr;
	int port=-1;

	int GetMsgSize();
	bool ProcessPacket(SOCKET sock,PacketType packet);
	void Listen();
	void Socketlisten(SOCKET sock);
public:
	template < typename T >
	void Send(SOCKET sock,PacketType packet, T msg);
	bool StartListening(const int port);
	TcpServer();
	~TcpServer();
};

template<typename T>
inline void TcpServer::Send(SOCKET sock,PacketType packet, T msg)
{
	send(sock,(char*)&packet,sizeof(PacketType),NULL);
	send(sock,(char*)&msg,sizeof(T),NULL);
}
