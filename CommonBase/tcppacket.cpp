#include "tcppacket.h"

TCPPacket::TCPPacket()
{
	memset(&header, 0, TCP_HEAD_LEN);
}
TCPPacket::~TCPPacket()
{
}
int TCPPacket::safe_check()
{
	if(body.length() != (unsigned int)header.length) return -1;
	return header.cmd;
}
int TCPPacket::save_packet(TCPHeader *h, std::string b)
{
	if(!h) return -1;
	body = b;
	header.cmd = h->cmd;
	header.version = h->version;
	header.uid = h->uid;
	header.length = h->length;
	data.clear();
	data.append((const char*)&header, TCP_HEAD_LEN);
	data.append(body);
	return safe_check();
}
int TCPPacket::pack_packet(const std::string& b, unsigned int cmd, unsigned int uid, short ver)
{
	body = b;
	header.cmd = cmd;
	header.uid = uid;
	header.version = ver;
	header.length = static_cast<unsigned short>(body.length());
	data.clear();
	data.append((const char*)&header, TCP_HEAD_LEN);
	data.append(body);
	return safe_check();
}