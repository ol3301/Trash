#include "TcpClient.h"
#include <thread>
#include <fstream>
#include <regex>


bool TcpClient::Connect(const std::string ip,const  int port)
{
	if (isListen)
		return false;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	if (connect(handler, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		std::cout << "Connection error" << std::endl;
		return false;
	}

	this->ip = ip;
	this->port = port;

	std::cout << "Connected to "+ip << std::endl;

	StartListening();

	return true;
}

bool TcpClient::Connect()
{
	return Connect(ip,port);
}

int TcpClient::GetMsgSize() {
	int msg_size;
	recv(handler,(char*)&msg_size,sizeof(int),NULL);
	return msg_size;
}

bool TcpClient::ProcessPacket(PacketType packet)
{
	int msg_size;
	char * mess = new char[1024];

	switch (packet) {

	case MsgText:
		recv(handler, mess, 1024, NULL);
		std::cout << mess << std::endl;
		return true;

	default:
		return false;
	}

	return true;
}

void TcpClient::Listen() {
	PacketType packet;

	while (isListen) {
		recv(handler, (char*)&packet, sizeof(PacketType), NULL);
		if (!ProcessPacket(packet)) {
			std::cout << "Unable packet" << std::endl;
		}
	}
}

void TcpClient::StartListening() {
	if (isListen) {
		std::cout << "isListen = true" << std::endl;
		return;
	}

	isListen = true;
	std::thread th(&TcpClient::Listen,this);
	th.detach();
	std::cout << "Start listening" << std::endl;
}

bool TcpClient::SwitchToNonBlocking()
{
	//переводим в неблокирующий режим
	BOOL mode = TRUE;
	if (ioctlsocket(handler, FIONBIO, (u_long*)&mode) == INVALID_SOCKET) {
		std::cout << "switch to non blocking error" << std::endl;
		return false;
	}
	return true;
}

bool TcpClient::SwitchToBlocking()
{
	//переводим в блокирующий режим
	BOOL mode = FALSE;
	if (ioctlsocket(handler, FIONBIO, (u_long*)&mode) == INVALID_SOCKET) {
		std::cout << "switch to blocking error" << std::endl;
		return false;
	}
	return true;
}

void TcpClient::LoadConfig(std::string configfile)
{
	std::ifstream r;
	r.open(configfile);

	if (!r.is_open()) {
		std::cout << "opening config.txt error" << std::endl;
		return;
	}

	std::string buf = "";

	while (!r.eof()) {
		char x;
		r >> x;
		buf += x;
	}

	r.close();

	buf = buf.substr(0,buf.length()-1);
	ParsConfig(buf);
}

bool TcpClient::ParsConfig(std::string config)
{
	size_t pos = config.find(':',0);

	if (pos == 0) {
		std::cout << "error parsing config" << std::endl;
		std::cout << "Config must be: ip:port" << std::endl;
		return false;
	}

	ip = config.substr(0, pos);
	port = std::stoi(config.substr(pos + 1, config.length() - pos));

	return true;
}

TcpClient::TcpClient()
{
	sizeofaddr = sizeof(addr);
	handler = socket(AF_INET,SOCK_STREAM,NULL);
	if (handler == INVALID_SOCKET) {
		std::cout << "created error " << std::endl;
	}
}

TcpClient::~TcpClient()
{
	closesocket(handler);
}
