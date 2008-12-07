#include "tcppacket.hpp"
#include <cstring>

#ifndef WIN32
	#include <arpa/inet.h>
#else
	#include <winsock.h>
#endif

#define ETHERNET_HEADER_SIZE	(14)
#define TCP_FLAG_BIT_ACK	(1 << 4)
#define TCP_FLAG_BIT_RST	(1 << 2)
#define TCP_FLAG_BIT_SYN	(1 << 1)

using namespace packetwrapper;
using namespace std;

TCPPacket::TCPPacket()
	: srcPort(0), srcIP(), dstPort(0), dstIP(), seq(0), ack(0),
	ackFlag(false), rstFlag(false), synFlag(false),
	length(0), dataLength(0), data(NULL)
{
}

TCPPacket::TCPPacket(TCPPacket & other)
	: srcPort(other.srcPort), srcIP(other.srcIP),
	dstPort(other.dstPort), dstIP(other.dstIP),
	seq(other.seq), ack(other.ack),
	ackFlag(other.ackFlag), rstFlag(other.rstFlag), synFlag(other.synFlag),
	length(other.length), dataLength(other.dataLength), data(NULL)
{
	unsigned char * newData = new unsigned char[dataLength];
	memcpy(newData, other.data, dataLength);

	data = (const unsigned char *) newData;
}

TCPPacket::~TCPPacket()
{
	delete data;
}

bool TCPPacket::parse(const unsigned char * payload, unsigned int payloadLength)
{
	typedef struct IPHeader{
		unsigned char version_hdrlength;
		unsigned char type;
		unsigned short length;
		unsigned short identification;
		unsigned short flags_fo;
		unsigned char ttl;
		unsigned char protocol;
		unsigned short checksum;
		unsigned int srcAddress;
		unsigned int dstAddress;
	} IPHeader;

	typedef struct TCPHeader {
		unsigned short srcPort;
		unsigned short dstPort;
		unsigned int seq;
		unsigned int ack;
		unsigned char doff; /* data offset + reserved */
		unsigned char flags;
		unsigned char window;
		unsigned short checksum;
		unsigned short urgent;
	} TCPHeader;

	/* Skip ethernet header */
	IPHeader * ipHeader = (IPHeader *) (payload + ETHERNET_HEADER_SIZE);

	srcIP.set(ntohl(ipHeader->srcAddress));
	dstIP.set(ntohl(ipHeader->dstAddress));

	TCPHeader * tcpHeader = (TCPHeader *) ((const unsigned char *) ipHeader + (ipHeader->version_hdrlength & 0x0F) * 4);

	srcPort = ntohs(tcpHeader->srcPort);
	dstPort = ntohs(tcpHeader->dstPort);

	seq = ntohl(tcpHeader->seq);
	ack = ntohl(tcpHeader->ack);

	ackFlag = ((tcpHeader->flags & TCP_FLAG_BIT_ACK) != 0);
	synFlag = ((tcpHeader->flags & TCP_FLAG_BIT_SYN) != 0);
	rstFlag = ((tcpHeader->flags & TCP_FLAG_BIT_RST) != 0);

	const unsigned char * packetData = (const unsigned char *) tcpHeader + ((tcpHeader->doff & 0xF0) >> 4) * 4;

	length = ntohs(ipHeader->length) - (packetData - (const unsigned char *) ipHeader);
	dataLength = payloadLength - (packetData - payload);

	if (dataLength > length)
		dataLength = length;

	unsigned char * newData = new unsigned char[dataLength];
	memcpy(newData, packetData, dataLength);

	data = (const unsigned char *) newData;

	return true;
}
