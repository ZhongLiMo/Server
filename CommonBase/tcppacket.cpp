#include "tcppacket.h"


TCPPacket::TCPPacket()
{
	memset(&header, 0, sizeof(struct Pheader));
}
TCPPacket::~TCPPacket()
{
}
int TCPPacket::safe_check()
{
	if(body.length() != (unsigned int)header.length) return -1;
	return header.cmd;
}
int TCPPacket::save(Pheader *h, std::string b)
{
	if(h == NULL){
		return -1;
	}
	header.cmd = h->cmd;
	header.length = h->length;
	header.uid = h->uid;
	header.version = h->version;
	body = b;
	data.clear();
	data.append((const char*)&header, sizeof(struct Pheader));
	data.append(body);
	return safe_check();
}
void TCPPacket::pack(unsigned int cmd, unsigned int uid, short ver)
{
	header.cmd = cmd;
	header.uid = uid;
	header.version = ver;
	header.length = body.length();
	data.clear();
	data.append((const char*)&header, sizeof(struct Pheader));
	data.append(body);
}
