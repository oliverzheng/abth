/* Project Includes */
#include "tcppacket.hpp"
#include "networkheaders.hpp"

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
	/* Extract Ethernet header */
	EthernetHeader * ethernetHeader = (EthernetHeader *) payload;

	dstIP.setMAC(ethernetHeader->dstAddress);
	srcIP.setMAC(ethernetHeader->srcAddress);

	/* Skip to IP header */
	IPHeader * ipHeader = (IPHeader *) (payload + sizeof(EthernetHeader));

	srcIP.set(ntohl(ipHeader->srcAddress));
	dstIP.set(ntohl(ipHeader->dstAddress));

	/* Skip to TCP header */
	TCPHeader * tcpHeader = (TCPHeader *) ((const unsigned char *) ipHeader + (ipHeader->version_hdrlength & 0x0F) * 4);

	srcPort = ntohs(tcpHeader->srcPort);
	dstPort = ntohs(tcpHeader->dstPort);

	seq = ntohl(tcpHeader->seq);
	ack = ntohl(tcpHeader->ack);

	ackFlag = ((tcpHeader->flags & tcpHeader->FLAG_ACK) != 0);
	synFlag = ((tcpHeader->flags & tcpHeader->FLAG_SYN) != 0);
	rstFlag = ((tcpHeader->flags & tcpHeader->FLAG_RST) != 0);

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

unsigned char * TCPPacket::construct()
{
	int packetLength = FULL_HEADER_SIZE + dataLength;

	unsigned char * packet = new unsigned char[packetLength];

	EthernetHeader * ethernetHeader = (EthernetHeader *) packet;
	IPHeader * ipHeader = (IPHeader *) ((unsigned char *) ethernetHeader + sizeof(*ethernetHeader));
	TCPHeader * tcpHeader = (TCPHeader *) ((unsigned char *) ipHeader + sizeof(*ipHeader));

	/* Construct Ethernet header */
	memcpy(ethernetHeader->dstAddress, dstIP.macAddress, sizeof(dstIP.macAddress));
	memcpy(ethernetHeader->srcAddress, srcIP.macAddress, sizeof(srcIP.macAddress));
	ethernetHeader->type = htons(ethernetHeader->ETHER_TYPE);

	/* Construct IP header */
	ipHeader->version_hdrlength = (ipHeader->VERSION << 4) | (sizeof(*ipHeader) / 4);
	ipHeader->type = ipHeader->TYPE;
	ipHeader->length = htons(sizeof(*ipHeader) + sizeof(*tcpHeader) + dataLength);

	ipHeader->identification = htons(ipHeader->IDENTIFICATION);
	ipHeader->flags_fo = htons(ipHeader->FLAGS_FO);

	ipHeader->ttl = ipHeader->TTL;
	ipHeader->protocol = ipHeader->PROTOCOL_TCP;
	ipHeader->checksum = 0;

	ipHeader->srcAddress = htonl(srcIP.getRaw());
	ipHeader->dstAddress = htonl(dstIP.getRaw());

	/* Calculate IP checksum */
	unsigned int ipChecksum = 0;
	unsigned short * ipHeaderShort = (unsigned short *) ipHeader;

	for (unsigned int i = 0; i < (sizeof(*ipHeader) / 2); i++)
		ipChecksum += ntohs(ipHeaderShort[i]);
	
	while (ipChecksum >> 16)
		ipChecksum = (ipChecksum & 0xFFFF) + (ipChecksum >> 16);

	ipChecksum = (~ipChecksum) & 0xFFFF;

	ipHeader->checksum = htons(ipChecksum);
	
	/* Construct TCP header */
	tcpHeader->srcPort = htons(srcPort);
	tcpHeader->dstPort = htons(dstPort);

	tcpHeader->seq = htonl(seq);
	tcpHeader->ack = htonl(ack);

	tcpHeader->doff = (sizeof(*tcpHeader) / 4) << 4;
	tcpHeader->flags = ((ackFlag) ? tcpHeader->FLAG_ACK : 0) |
			   ((rstFlag) ? tcpHeader->FLAG_RST : 0) |
			   ((synFlag) ? tcpHeader->FLAG_SYN : 0);
	tcpHeader->window = tcpHeader->WINDOW;

	tcpHeader->checksum = 0;
	tcpHeader->urgent = 0;

	/* Calculate TCP checksum */

	unsigned int tcpChecksum = 0;

	unsigned char * tcpHeaderChar = (unsigned char *) tcpHeader;
	unsigned int tcpLength = sizeof(*tcpHeader) + dataLength;

	unsigned short * srcIP = (unsigned short *) &(ipHeader->srcAddress);
	unsigned short * dstIP = (unsigned short *) &(ipHeader->dstAddress);
	
	for (unsigned int i = 0; i < (sizeof(srcIP) / 2); i++)
		tcpChecksum += ntohs(srcIP[i]);

	for (unsigned int i = 0; i < (sizeof(dstIP) / 2); i++)
		tcpChecksum += ntohs(dstIP[i]);

	for (unsigned int i = 0; i < tcpLength; i++)
		tcpChecksum += ((unsigned short) tcpHeaderChar[i]) << ((i % 2) ? 0 : 8);

	tcpChecksum += ((unsigned short) ipHeader->PROTOCOL_TCP) + (tcpLength);

    	while (tcpChecksum >> 16)
		tcpChecksum = (tcpChecksum & 0xFFFF) + (tcpChecksum >> 16);
		
	tcpChecksum = ~tcpChecksum;

	tcpHeader->checksum = htons(tcpChecksum);

	/* Copy data */
	memcpy(packet + FULL_HEADER_SIZE, data, dataLength);

	return packet;
}
