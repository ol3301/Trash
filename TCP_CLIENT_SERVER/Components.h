

enum PacketType {
	MsgText,
	NewUser
};

struct User {
	SOCKET handler;
	std::string name;
};