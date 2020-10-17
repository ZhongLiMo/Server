#ifndef TCP_PACKET_H
#define TCP_PACKET_H

#include <string>
#include <memory>

#pragma pack(push)
#pragma pack(1)
struct TCPHeader
{
	unsigned int cmd;
	unsigned short version;
	unsigned int uid;
	unsigned short length;
};
#pragma pack(pop)

const int TCP_HEAD_LEN = sizeof(TCPHeader);

class TCPPacket
{
public:
	TCPPacket();
	~TCPPacket();
	int safe_check();
	int save_packet(TCPHeader *h, std::string b);
	int pack_packet(const std::string& b, unsigned int cmd = 0, unsigned int uid = 0, short ver = 0);
	static std::shared_ptr<TCPPacket> CreateNew() { return std::shared_ptr<TCPPacket>(new TCPPacket); }
public:
	TCPHeader header;
	std::string body;
	std::string data;
};

#endif // !TCP_PACKET_H