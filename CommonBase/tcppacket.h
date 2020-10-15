#ifndef TCP_PACKET_H
#define TCP_PACKET_H


#include <string>

#pragma pack(push)
#pragma pack(1)
struct Pheader
{
	unsigned int cmd;
	unsigned short length;
	unsigned int uid;
	unsigned short version;
};
#pragma pack(pop)

class TCPPacket
{
public:
	TCPPacket();
	virtual ~TCPPacket();
	void pack(unsigned int cmd = 0, unsigned int uid = 0, short ver = 0);
	int save(Pheader *h, std::string b);
	int safe_check();
public:
	struct Pheader header;
	std::string body;
	std::string data;
};


#endif // !TCP_PACKET_H