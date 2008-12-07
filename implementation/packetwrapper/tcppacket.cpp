/* Project Includes */
#include "tcppacket.hpp"

/* Standard Library Includes */
#include <cstring>

/* System Includes */
#ifndef WIN32
	#include <arpa/inet.h>
#else
	#include <winsock.h>
#endif

/* Namespace Declarations */
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
	/* Deep copy the data */
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
		unsigned char doff;
		unsigned char flags;
		unsigned char window;
		unsigned short checksum;
		unsigned short urgent;
	} TCPHeader;

	/* Skip Ethernet header to IP header */
	IPHeader * ipHeader = (IPHeader *) (payload + ETHERNET_HEADER_SIZE);

	srcIP.set(ntohl(ipHeader->srcAddress));
	dstIP.set(ntohl(ipHeader->dstAddress));

	/* Skip to TCP header */
	TCPHeader * tcpHeader = (TCPHeader *) ((const unsigned char *) ipHeader + (ipHeader->version_hdrlength & 0x0F) * 4);

	srcPort = ntohs(tcpHeader->srcPort);
	dstPort = ntohs(tcpHeader->dstPort);

	seq = ntohl(tcpHeader->seq);
	ack = ntohl(tcpHeader->ack);

	ackFlag = ((tcpHeader->flags & TCP_FLAG_BIT_ACK) != 0);
	synFlag = ((tcpHeader->flags & TCP_FLAG_BIT_SYN) != 0);
	rstFlag = ((tcpHeader->flags & TCP_FLAG_BIT_RST) != 0);

	/* Skip to TCP data */
	const unsigned char * packetData = (const unsigned char *) tcpHeader + ((tcpHeader->doff & 0xF0) >> 4) * 4;

	/* Length of the data as indicated in the IP header */
	length = ntohs(ipHeader->length) - (packetData - (const unsigned char *) ipHeader);

	/* Length of the data actually captured */
	dataLength = payloadLength - (packetData - payload);

	/* There may be an Ethernet footer, which results when captured data size exceeds actual data size. Trim it off. */
	if (dataLength > length)
		dataLength = length;

	/* Copy only the data */
	unsigned char * newData = new unsigned char[dataLength];
	memcpy(newData, packetData, dataLength);

	data = (const unsigned char *) newData;

	return true;
}
